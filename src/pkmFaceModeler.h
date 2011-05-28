/*
 *  pkmFaceModeler.h
 *  xcode
 *
 *  Created by Mr. Magoo on 5/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>
#include "AAM_IC.h"
#include "VJfacedetect.h"

using namespace std;
using namespace cv;

class pkmFaceModeler
{
public:
	pkmFaceModeler(int n = 100)
	{
		char cascadeFileName[] = "../model/haarcascade_frontalface_alt2.xml";
		faceDet.LoadCascade(cascadeFileName);
		
		currentExample			= 0;		// current example to train
		numExamples				= n;		// number of examples before model is built
		
		bModelLoaded			= false;
		bDraw					= false;
	}
	
	~pkmFaceModeler()
	{
		
	}
	
	bool addExample(Mat &img, Mat &shape)
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
	
	bool buildModel()
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
	
	bool loadExistingModel()
	{
		aamModel.ReadModel("model.aam");
		bModelLoaded = true;
		return bModelLoaded;
	}
	
	int getCurrentCount()
	{
		return currentExample;
	}
	
	void update(Mat &frame)
	{
		if (bModelLoaded) {
			IplImage image = frame;
			if (aamModel.InitShapeFromDetBox(aamShape, faceDet, &image) == false) {
				bDraw = false;
				return;
			}
			else {
				bDraw = true;
				aamModel.Fit(&image, aamShape, 1, false);
			}
		}
	}
	
	void draw(Mat &frame)
	{
		if (bDraw)
		{
			IplImage image = frame;
			aamModel.Draw(&image, aamShape, 2);
		}
	}
	
	
private:
	
	vector<string>			imageFiles,
							pointFiles;
	
	int						numExamples,
							currentExample;
				  
	AAM_Pyramid				aamModel;
	AAM_Shape				aamShape;
	VJfacedetect			faceDet;
	
	bool					bModelLoaded, bDraw;
	
};