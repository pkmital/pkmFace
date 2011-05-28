/*
 
 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com
 
 */

#include "pkmFaceModeler.h"

pkmFaceModeler::pkmFaceModeler(int n)
{
	char cascadeFileName[] = "../model/haarcascade_frontalface_alt2.xml";
	faceDet.LoadCascade(cascadeFileName);
	
	currentExample			= 0;		// current example to train
	numExamples				= n;		// number of examples before model is built
	
	bModelLoaded			= false;
	bDraw					= false;
}

pkmFaceModeler::~pkmFaceModeler()
{
	
}

bool pkmFaceModeler::addExample(Mat &img, Mat &shape)
{
	if (bModelLoaded) {
		return true;
	}
	
	currentExample++;
	
	int numPoints = shape.rows / 2;
	
	// save the image
	char filename[256];
	sprintf(filename, "img_%03d.jpg", currentExample);
	imwrite(filename, img);
	imageFiles.push_back(filename);
	
	// save the points
	FILE *fp;
	sprintf(filename, "img_%03d.pts", currentExample);
	fp = fopen(filename, "w");
	fprintf(fp, "points %d\n", numPoints);
	for (int i = 0; i < numPoints; i++) {
		fprintf(fp, "%f %f\n", shape.at<double>(i,0),shape.at<double>(i+numPoints,0));
	}
	fclose(fp);
	pointFiles.push_back(filename);
	
	// check if we can build a model yet
	if (currentExample >= numExamples) {
		return buildModel();
	}
	else {
		return false;
	}
}

// we've learned enough examples, now train an AAM, storing to "model.aam"
bool pkmFaceModeler::buildModel()
{
	int type = TYPE_AAM_IC;
	int level = 1;
	int color = 3;
	aamModel.Build(pointFiles, imageFiles, type, level, color);
	aamModel.BuildDetectMapping(pointFiles, imageFiles, faceDet, 100);
	aamModel.WriteModel("model.aam");
	bModelLoaded = true;
	return true;
}

// load a model we've saved before
bool pkmFaceModeler::loadExistingModel()
{
	aamModel.ReadModel("model.aam");
	bModelLoaded = true;
	return bModelLoaded;
}

// how many examples have we stored so far?
int pkmFaceModeler::getCurrentCount()
{
	return currentExample;
}

// update with new frame
void pkmFaceModeler::update(Mat &frame)
{
	// if we have loaded a model
	if (bModelLoaded) {
		// track the face
		IplImage image = frame;
		if (aamModel.InitShapeFromDetBox(aamShape, faceDet, &image) == false) {
			// no face found
			bDraw = false;
			return;
		}
		// found a face, now fit an AAM
		else {
			bDraw = true;
			aamModel.Fit(&image, aamShape, 1, false);
			
			// output eigen-values to console
			//aamModel.PrintAppearanceVector();
			
			// store appearance vector
			CvMat *appearanceVector = aamModel.GetAppearanceVector();
			currentAppearanceVector = Mat(appearanceVector, true);
		}
	}
}

void pkmFaceModeler::draw(Mat &frame)
{
	if (bDraw)
	{
		// draw the appearance model in the image
		IplImage image = frame;
		aamModel.Draw(&image, aamShape, 2);
	}
}

// return the matrix of appearance values
Mat pkmFaceModeler::getAppearanceVector()
{
	if (bModelLoaded)
		return currentAppearanceVector;
	else {
		return Mat();
	}
}