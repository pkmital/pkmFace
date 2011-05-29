/*
 *  pkmCalibrator.h
 
 [ R11 R12 R13 ]   [ x ]   [ dx ]     [ X ] 
 [ R21 R22 R23 ] * [ y ] + [ dy ]  =  [ Y ]
 [ R31 R32 R33 ]   [ z ]   [ dz ]     [ Z ]
 
 So there is some mapping of the first 4 eigenvalues of the AAM and the 3 translation 
 values of the central bounding box to these 3 rotation + 3 translation values...
 
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <vector.h>

using namespace std;
using namespace cv;

class pkmPoseCalibrator
{
public:
	pkmPoseCalibrator();
	~pkmPoseCalibrator();
	
	void addExample(int calibrationPoint, Mat &values);
	
	void getPose(Mat &values, Mat &pose_x, Mat &pose_y);
	
	bool isReadyForTraining();
	
	bool modelPose();
	
	void printMatrix(Mat X, string name = "");
	
private:
	vector<int>				currentView;
	vector<bool>			bCurrentViewLoaded;
	Mat						poseX,
							poseY;
	Mat						eigenValues;
	Mat						poseXInv,
							poseYInv;
	
	int						numCalibrationPoints, 
							numViews, 
							numPoints,	
							numFactors;
	
	int						decompType;
	
	bool					bBuiltModel;
};