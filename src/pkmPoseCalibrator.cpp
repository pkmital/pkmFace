/*
 *  pkmCalibrator.cpp

 Ax = b
 
 using the pseudo-inverse to solve the equation for x:
 inv(A'*A)*A'*b = x;
 
				A									x		  =					b
 [calibrationPoints * numViews X numPoints] * [numPoints X 1] = [ calibrationPoints * numViews x 1 ]
 
 [10 * 10 X 4] * [4 x 1] = [10 * 10 x 1]
 
 [100 X 4] * [4 x 1] = [100 x 1]
 
 A is the observed values of the equation
 x is the set of factors of the equation I am solving for
 b is the known rotations for each observation
 

 possible equations:
 
 A*e1 + B*e2 + C*e3 + D*e4
 A = [A B C D]
 x = [e1 e2 e3 e4]'
 
 A*e1 + B*e2 + C*e3 + D*e4 + E
 A = [A B C D E]
 x = [e1 e2 e3 e4 1]'
 
 A*e1 + B*e2 + C*e3 + 
 D*e1*e2 + E*e1*e3 + F*e2*e3 + 
 G*e1*e2*e3 + H
 A = [A B C D E F G H]  
 x = [e1 e2 e3 e1*e2 e1*e3 e2*e3 e1*e2*e3 1]'
 
 A*e1 + B*e2 + C*e3 + D*e4 + 
 E*e1*e2 + F*e1*e3 + G*e1*e4 + H*e2*e3 + I*e2*e4 + J*e3*e4 + 
 K*e1*e2*e3*e4 + L
 A = [A B C D E F G H I J K L] 
 x = [e1 e2 e3 e4 e1*e2 e1*e3 e1*e4 e2*e3 e2*e4 e3*e4 e1*e2*e3*e4 1]'
 
 *
 */

#include "pkmPoseCalibrator.h"

pkmPoseCalibrator::pkmPoseCalibrator()
{
	numCalibrationPoints	= 10;				// number of calibration points
	numViews				= 10;				// observations for each calibration point
	numPoints				= 5;				// number of eigen values to train on
	numFactors				= numPoints;
	decompType				= DECOMP_SVD;		// how should we do the inverse?
	
	eigenValues = Mat(numViews * numCalibrationPoints, numFactors, CV_64FC1);
	
	// keep an index of the current view for each calibration point
	for (int i = 0; i < numCalibrationPoints; i++) {
		currentView.push_back(0);
		bCurrentViewLoaded.push_back(false);
	}
	
	// store the calibration points (screen coordinates to map to)
	vector<double> p_x, p_y;
	// this is the calibration grid i'm using
	// press the keys while looking in these general directions (ideally also
	// show the calibration grid on screen but ...)
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
		p_y.push_back(-0.5);
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
		Mat px = values.colRange(0, numPoints) * poseXInv;
		px.copyTo(pose_x);
		Mat py = values.colRange(0, numPoints) * poseYInv;
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
		Mat etei = ete.inv(decompType);
		Mat eteit =  etei * eigenValues.t();
		
		Mat epx = eteit * poseX;
		epx.copyTo(poseXInv);
		Mat epy = eteit * poseY;
		epy.copyTo(poseYInv);
		
		printMatrix(poseXInv, "poseXInv");
		printMatrix(poseYInv, "poseYInv");
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