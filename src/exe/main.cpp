/*
 
 by Parag K Mital
 Copyright Parag K Mital 2011, All rights reserved.
 http://pkmital.com
 
 */
#include <opencv/highgui.h>
#include "app.h"

int main()
{
	int c = 0;
	app *myApp = new app();
	
	myApp->setup();
	
	while (1) {
		myApp->update();
		myApp->draw();
		c = cvWaitKey(10);
		if (c == 27) {
			break;
		}
		else
			myApp->keyPressed(c);
	}
	
	return 0;
}