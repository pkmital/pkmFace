/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#ifndef AAM_MOVIEAVI_H
#define AAM_MOVIEAVI_H

#include "AAM_Config.h"

class AAM_EXPORTS AAM_MovieAVI
{
public:
	AAM_MovieAVI();
	~AAM_MovieAVI();

	// Open a AVI file
	void Open(const char* videofile);
	
	// Close it
	void Close();

	// Get concrete frame of the video
	// Notice: for speed up you have no need to release the returned image
	IplImage* ReadFrame(int frame_no = -1);
	
	// frame count of this video
	const int FrameCount()const
	{return (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);}

private:
	CvCapture* capture;
	IplImage* capimg;//captured from video
	IplImage *image;
	
};

#endif // !
