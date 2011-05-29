/*
 
 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com
 
 */

#pragma once


#include "pkmFaceTracker.h"
#include "pkmFaceModeler.h"
#include "pkmPoseCalibrator.h"
#include "pkmPoseFilter.h"
#include <opencv2/opencv.hpp>

class app
{
public:
	app();
	~app();
	void setup(int w = 640, int h = 480, int n = 200);
	void update();
	void draw();
	void keyPressed(int c);
private:
	pkmFaceTracker			*faceTracker;
	pkmFaceModeler			*faceModeler;
	pkmPoseCalibrator		*poseCalibrator;
	pkmPoseFilter			poseFilter;
	
	CvCapture				*camera; 
	IplImage				*cameraImage;
	Mat						frame;
	string					window_faceTracker;
	
	int						numExamples;
	
	int						width, 
							height;
	
	bool					bTraining, bAppearanceModelBuilt, bPoseModelBuilt;
};