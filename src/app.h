
#pragma once


#include "pkmFaceTracker.h"
#include "pkmFaceModeler.h"
#include <opencv2/opencv.hpp>

class app
{
public:
	app()
	{
		
	}
	~app()
	{
		delete faceTracker;
		delete faceModeler;
	}
	
	void setup(int w = 320, int h = 240)
	{
		width				= w;
		height				= h;
		
		// initialize face tracker
		faceModeler			= new pkmFaceModeler();		// appearance model
		faceTracker			= new pkmFaceTracker();		// shape model
		
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
		
		bTraining			= false;
		bModelBuilt			= false;
	}
	
	void update()
	{
		cameraImage = cvQueryFrame(camera); 
		if(!cameraImage)
			return; 
		frame = cameraImage;
		
		if (bModelBuilt) {
			faceModeler->update(frame);
		}
		else {
			faceTracker->update(frame);
			
			if (bTraining) {
				Mat shape = faceTracker->getShapeModel();
				if(faceModeler->addExample(frame, shape))
					bModelBuilt = true;
			}
		}
	}
	
	void draw()
	{
		if (bModelBuilt) {
			faceModeler->draw(frame);
		}
		else {
			faceTracker->drawShapeModel(frame);
			int count = faceModeler->getCurrentCount();
			char buf[256];
			sprintf(buf, "%03d / 100", count);
			putText(frame,
					buf,
					Point(10,20),
					CV_FONT_HERSHEY_SIMPLEX,
					0.5,
					CV_RGB(255,255,255));
			
		}

		imshow(window_faceTracker.c_str(), frame);
	}
	
	void keyPressed(int c)
	{
		if(char(c) == 'd')
			faceTracker->reset();
		else if(char(c) == ' ')
		{
			bTraining = !bTraining;
		}	
		else if(char(c) == 'l')
		{
			bModelBuilt = faceModeler->loadExistingModel();
		}
	}
	
private:
	pkmFaceTracker			*faceTracker;
	
	pkmFaceModeler			*faceModeler;
	
	CvCapture				*camera; 
	IplImage				*cameraImage;
	Mat						frame;
	string					window_faceTracker;
	
	int						width, 
							height;
	
	bool					bTraining, bModelBuilt;
};