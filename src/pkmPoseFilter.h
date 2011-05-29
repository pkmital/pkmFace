/*
 *  pkmPoseFilter.h

	Simple mean filter which could be extended to incorporate other filters
 
 *  Created by Mr. Magoo on 5/29/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <vector.h>


class pkmPoseFilter
{
public:
	pkmPoseFilter(int fs = 5)
	{
		filterSize = fs;
		
		for (int i = 0; i < filterSize; i++) {
			xes.push_back(0);
			yes.push_back(0);
		}
	}
	
	void addExample(double x, double y)
	{
		xes.erase(xes.begin());
		yes.erase(yes.begin());
		
		xes.push_back(x);
		yes.push_back(y);
		
		curr_x = xes[0];
		curr_y = yes[0];
		for (int i = 1; i < filterSize; i++) {
			curr_x += xes[i];
			curr_y += yes[i];
		}
		
		curr_x /= filterSize;
		curr_y /= filterSize;
	}
	
	double getX()
	{
		return curr_x;
	}
	
	double getY()
	{
		return curr_y;
	}
	
private:
	vector<double>				xes, yes;
	double						curr_x, curr_y;
	int							filterSize;
	
};