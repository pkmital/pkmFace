/*
 *  pkmCalibrator.cpp
 *  xcode
 *
 *  Created by Mr. Magoo on 5/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "pkmPoseCalibrator.h"

pkmPoseCalibrator::pkmPoseCalibrator()
{
	numCalibrationPoints	= 10;		// number of calibration points
	numViews				= 2;		// observations for each calibration point
	numPoints				= 4;		// number of eigen values to train on
	
	eigenValues = Mat(numViews * numCalibrationPoints, numPoints, CV_64FC1);
	
	// keep an index of the current view for each calibration point
	for (int i = 0; i < numCalibrationPoints; i++) {
		currentView.push_back(0);
		bCurrentViewLoaded.push_back(false);
	}
	
	// store the calibration points (screen coordinates to map to)
	vector<double> p_x, p_y;
	// 
	//  1    2    3
	//         10
	//  4    5    6
	//           
	//  7    8    9
	//
	
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(0.5);
		p_y.push_back(0.5);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(-1.0);
		p_y.push_back(1.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(0.0);
		p_y.push_back(1.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(1.0);
		p_y.push_back(1.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(-1.0);
		p_y.push_back(0.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(0.0);
		p_y.push_back(0.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(1.0);
		p_y.push_back(0.0);
	}
	for(int i = 0; i < numViews; i++)
	{	
		p_x.push_back(-1.0);
		p_y.push_back(-1.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(0.0);
		p_y.push_back(-1.0);
	}
	for(int i = 0; i < numViews; i++)
	{
		p_x.push_back(1.0);
		p_y.push_back(-1.0);
	}
	
	poseX = cv::Mat(p_x).clone();
	poseY = cv::Mat(p_y).clone();
	//printMatrix(poseX, "poseX");
	//printMatrix(poseY, "poseY");
	
	bBuiltModel = false;
}
pkmPoseCalibrator::~pkmPoseCalibrator()
{
	
}

void pkmPoseCalibrator::addExample(int calibrationPoint, Mat &values)
{		
	printf("Adding example for point %d.\n", calibrationPoint);
	
	if (values.rows != 1 || values.cols < numPoints) {
		printf("[ERROR]: expected a 1x%d matrix.  Received a %dx%d matrix.\n", 
			   numPoints, 
			   values.rows, 
			   values.cols);
		return;
	}
	
	// get the index in the huge matrix of observations
	int indx = calibrationPoint*numViews + currentView[calibrationPoint];
	
	// store these values
	Mat r = eigenValues.row(indx);
	values.colRange(0, numPoints).copyTo(r);
	printMatrix(r, "new row");
	
	// increment the number of views loaded
	currentView[calibrationPoint] = (currentView[calibrationPoint] + 1) % numViews;
	
	// check if we have cycled around the number of views needed to training this point
	bCurrentViewLoaded[calibrationPoint] = (currentView[calibrationPoint]==0) | 
	bCurrentViewLoaded[calibrationPoint];
}

void pkmPoseCalibrator::getPose(Mat &values, Mat &pose_x, Mat &pose_y)
{
	if (bBuiltModel) {
		Mat px = values.colRange(0, 4) * poseXInv;
		px.copyTo(pose_x);
		Mat py = values.colRange(0, 4) * poseYInv;
		py.copyTo(pose_y);
		printMatrix(px, "pose_x");
		printMatrix(py, "pose_y");
	} 
}

bool pkmPoseCalibrator::isReadyForTraining()
{
	bool ready = true;
	for (int i = 0; i < numCalibrationPoints; i++) {
		ready &= bCurrentViewLoaded[i];
	}
	return ready;
}

bool pkmPoseCalibrator::modelPose()
{
	if (isReadyForTraining()) {
		printf("Modeling pose...\n");
		
		printMatrix(eigenValues, "eigenValues");
		Mat ete = eigenValues.t() * eigenValues;
		printMatrix(ete, "ete");
		Mat etei = ete.inv();
		printMatrix(etei, "etei");
		Mat eteit =  etei * eigenValues.t();
		printMatrix(eteit, "eteit");
		
		Mat epx = eteit * poseX;
		epx.copyTo(poseXInv);
		Mat epy = eteit * poseY;
		epy.copyTo(poseYInv);
		
		printMatrix(epx, "poseXInv");
		printMatrix(epy, "poseYInv");
		bBuiltModel = true;
		return true;
	}
	else {
		printf("Not all calibration points have been trained!\n");
		return false;
	}
	
}

void pkmPoseCalibrator::printMatrix(Mat X, string name)
{
	printf("%s: %d, %d\n", name.c_str(), X.rows, X.cols);
	for (int i = 0; i < X.rows; i++) {
		for (int j = 0; j < X.cols; j++) {
			printf("%3.4lf ", X.at<double>(i,j));
		}
		printf("\n");
	}
	printf("\n");
}