/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#include "AAM_MovieAVI.h"

using namespace std;

//============================================================================
AAM_MovieAVI::AAM_MovieAVI(): capture(0), capimg(0), image(0)
{	
}

AAM_MovieAVI::~AAM_MovieAVI()
{
	Close();
}


void AAM_MovieAVI::Open(const char* videofile)
{
	capture = cvCaptureFromAVI(videofile);
	if(!capture)
	{
		AAM_FormatMSG("CANNOT open video file \"%s\" !\n", videofile);
		AAM_ERROR(errmsg);
	}
	
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, 0);
	capimg = cvQueryFrame( capture );
	image = cvCreateImage(cvGetSize(capimg), capimg->depth, capimg->nChannels);
}

//============================================================================
void AAM_MovieAVI::Close()
{
	cvReleaseCapture(&capture);
	capture = 0;
	cvReleaseImage(&image);
	image = 0;
}

//============================================================================
IplImage* AAM_MovieAVI:: ReadFrame(int frame_no )
{
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frame_no);
	capimg = cvQueryFrame( capture );

	if(capimg->origin == 0)
		cvCopy(capimg, image);
	else
		cvFlip(capimg, image);

	return image;
}
