/*
 
 Wraps Jason Mora Saragih's FaceTracker code in a cpp object 
 (see COPYRIGHT/README in /src/facetracker for Jason's project info)

 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com
 
*/

#pragma once

#include <Tracker.h>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv; 

class pkmFaceTracker
{
public:
	pkmFaceTracker(float s = 1.0f)
	{
		scale			= s;
		char ftFile[]	= "../model/face2.tracker";
		char conFile[]	= "../model/face.con";
		char triFile[]	= "../model/face.tri";
		
		// load face tracker model
		model			= new FACETRACKER::Tracker(ftFile);
		tri				= FACETRACKER::IO::LoadTri(triFile);
		con				= FACETRACKER::IO::LoadCon(conFile);
		
		// tracking parameters 
		wSize1.resize(1);
		wSize2.resize(3);
		wSize1[0]		= 5;		// 7
		wSize2[0]		= 11;		// 11
		wSize2[1]		= 9;		// 9
		wSize2[2]		= 7;		// 7
		
		nIter			= 3;		// 5
		clamp			= 5;		// 3
		fTol			= 0.99;		// 0.01
		
		fpd				= -1;		// -1
		bFCheck			= false;
		bFailed			= true;
		
	}
	
	~pkmFaceTracker()
	{
		delete model;
	}
	
	void update(Mat &frame)
	{
		// scale down
		if(scale == 1)
			im = frame; 
		else 
			cv::resize(frame,
					   im,
					   cv::Size(scale * frame.cols,
								scale * frame.rows));
		
		// flip horizontal
		// cv::flip(im, im, 1); 
		
		// go to grayscale
		cv::cvtColor(im, gray, CV_BGR2GRAY);
		
		std::vector<int> wSize; 
		if(bFailed)
			wSize = wSize2; 
		else 
			wSize = wSize1; 
		
		if(model->Track(gray,
					    wSize,
					    fpd,
					    nIter,
					    clamp,
					    fTol,
					    bFCheck) == 0)
		{
			int idx = model->_clm.GetViewIdx(); 
			bFailed = false;
		}
		else {
			model->FrameReset(); 
			bFailed = true;
		}

	}
	
	void drawShapeModel(Mat &image)
	{
		Mat shape = model->_shape;
		int i,n = shape.rows/2; cv::Point p1,p2; cv::Scalar c;
		
		//draw triangulation
		c = CV_RGB(0,0,0);
		for(i = 0; i < tri.rows; i++){
			//if(visi.at<int>(tri.at<int>(i,0),0) == 0 ||
			//   visi.at<int>(tri.at<int>(i,1),0) == 0 ||
			//   visi.at<int>(tri.at<int>(i,2),0) == 0)continue;
			p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
						   shape.at<double>(tri.at<int>(i,0)+n,0));
			p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
						   shape.at<double>(tri.at<int>(i,1)+n,0));
			cv::line(image,p1,p2,c);
			p1 = cv::Point(shape.at<double>(tri.at<int>(i,0),0),
						   shape.at<double>(tri.at<int>(i,0)+n,0));
			p2 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
						   shape.at<double>(tri.at<int>(i,2)+n,0));
			cv::line(image,p1,p2,c);
			p1 = cv::Point(shape.at<double>(tri.at<int>(i,2),0),
						   shape.at<double>(tri.at<int>(i,2)+n,0));
			p2 = cv::Point(shape.at<double>(tri.at<int>(i,1),0),
						   shape.at<double>(tri.at<int>(i,1)+n,0));
			cv::line(image,p1,p2,c);
		}
		//draw connections
		c = CV_RGB(0,0,255);
		for(i = 0; i < con.cols; i++){
			//if(visi.at<int>(con.at<int>(0,i),0) == 0 ||
			//   visi.at<int>(con.at<int>(1,i),0) == 0)continue;
			p1 = cv::Point(shape.at<double>(con.at<int>(0,i),0),
						   shape.at<double>(con.at<int>(0,i)+n,0));
			p2 = cv::Point(shape.at<double>(con.at<int>(1,i),0),
						   shape.at<double>(con.at<int>(1,i)+n,0));
			cv::line(image,p1,p2,c,1);
		}
		//draw points
		char buf[256];
		for(i = 0; i < n; i++){    
			//if(visi.at<int>(i,0) == 0)continue;
			p1 = cv::Point(shape.at<double>(i,0),shape.at<double>(i+n,0));
			c = CV_RGB(255,0,0); 
			sprintf(buf, "%d", i);
			cv::putText(image, buf, p1, cv::FONT_HERSHEY_PLAIN, 1, c);
			cv::circle(image,p1,2,c);
		}
	}
	
	void reset()
	{
		model->FrameReset();
	}
	
	Mat getShapeModel()
	{
		return model->_shape;
	}
	
private:

	FACETRACKER::Tracker			*model;
	
	Mat								im, 
									gray,
									tri,
									con;
	
	std::vector<int>				wSize1; 
	std::vector<int>				wSize2; 
	
	int								fpd,
									nIter; 
	
	float							scale;	
	
	double							clamp,
									fTol; 
	
	bool							bFCheck, bFailed;
};