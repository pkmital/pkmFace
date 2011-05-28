/*
 
 Builds on top of GreatYao's aam-library for building an active apperance model.
 
 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com
 
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
	pkmFaceModeler(int n = 100);
	
	~pkmFaceModeler();
	
	bool addExample(Mat &img, Mat &shape);
	
	// we've learned enough examples, now train an AAM, storing to "model.aam"
	bool buildModel();
	
	// load a model we've saved before
	bool loadExistingModel();
	
	// how many examples have we stored so far?
	int getCurrentCount();
	
	// update with new frame
	void update(Mat &frame);
	
	void draw(Mat &frame);
	
	// return the matrix of appearance values
	Mat getAppearanceVector();
	
	
private:
	Mat							currentAppearanceVector;
	
	vector<string>				imageFiles,
								pointFiles;
	
	int							numExamples,
								currentExample;
				  
	AAM_Pyramid					aamModel;
	AAM_Shape					aamShape;
	VJfacedetect				faceDet;
	
	bool						bModelLoaded, bDraw;
	
};