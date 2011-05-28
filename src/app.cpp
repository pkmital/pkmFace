/*

 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com 
 
 */

#include "app.h"

app::app()
{
	
}
app::~app()
{
	delete faceTracker;
	delete faceModeler;
}

void app::setup(int w, int h, int n)
{
	width				= w;
	height				= h;
	
	// initialize face tracker
	numExamples			= n;								// number of images to train face model on
	faceModeler			= new pkmFaceModeler(numExamples);	// appearance model
	faceTracker			= new pkmFaceTracker();				// shape model
	poseCalibrator		= new pkmPoseCalibrator();				// pose calibration
	
	// open a camera
	camera				= cvCreateCameraCapture(CV_CAP_ANY); 
	cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, (double)width);
	cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, (double)height);
	
	if(!camera)
	{
		printf("[ERROR]: Could not open any cameras!\n");
		std::exit(1);
	}
	
	// initialize a window
	window_faceTracker = "Face Tracking";
	cvNamedWindow(window_faceTracker.c_str(), 1);
	
	bTraining					= false;
	bAppearanceModelBuilt		= false;
	bPoseModelBuilt				= false;
}

void app::update()
{
	cameraImage = cvQueryFrame(camera); 
	if(!cameraImage)
		return; 
	frame = cameraImage;
	
	if(bPoseModelBuilt) {
		faceModeler->update(frame);
		Mat av = faceModeler->getAppearanceVector();
		Mat pose_x, pose_y;
		poseCalibrator->getPose(av, pose_x, pose_y);
	}
	else if(bAppearanceModelBuilt) {
		faceModeler->update(frame);
	}	
	else {
		faceTracker->update(frame);
		
		if (bTraining) {
			Mat shape = faceTracker->getShapeModel();
			if(faceModeler->addExample(frame, shape))
				bAppearanceModelBuilt = true;
		}
	}
}

void app::draw()
{
	if (bPoseModelBuilt) {
		faceModeler->draw(frame);
	}
	else if(bAppearanceModelBuilt) {
		faceModeler->draw(frame);
	}
	else {
		faceTracker->drawShapeModel(frame);
		int count = faceModeler->getCurrentCount();
		char buf[256];
		sprintf(buf, "%03d / %03d", count, numExamples);
		putText(frame,
				buf,
				Point(10,20),
				CV_FONT_HERSHEY_SIMPLEX,
				0.5,
				CV_RGB(255,255,255));
		
	}
	
	imshow(window_faceTracker.c_str(), frame);
}

void app::keyPressed(int c)
{
	// reset the shape model
	if(c == 'd')
		faceTracker->reset();
	// start training the AAM
	else if(c == ' ')
	{
		bTraining = !bTraining;
	}	
	// load a pre-existing AAM
	else if(c == 'l')
	{
		bAppearanceModelBuilt = faceModeler->loadExistingModel();
	}
	
	// train for calibration 
	else if(c >= '1' && c <= '9' || c == '0' )
	{
		char cc = char(c);
		int point = atoi(&cc);
		Mat av = faceModeler->getAppearanceVector();
		poseCalibrator->addExample(point, av);
	}
	
	else if(c == 't')
	{
		bPoseModelBuilt = poseCalibrator->modelPose();
	}
}
