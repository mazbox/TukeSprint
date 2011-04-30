/**
 * AppSettings.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#pragma once
#include "ofMain.h"

class AppSettings {
public: 
	static int colourMode;
	
	static ofColor color1;
	static ofColor color2;
	static ofColor color3;
	
	static float volume;

	// the default image of any imagery
	static ofImage *image;
	
	// is the camera available
	static bool cameraAvailable;
};