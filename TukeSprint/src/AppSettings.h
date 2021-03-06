/**
 * AppSettings.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#pragma once
#include "ofMain.h"
class AppSettingsListener {
public:
	virtual void colorChanged() {}
	virtual void imageChanged() {}
	virtual void soundChanged() {}
};

class AppSettings {
public: 
	static int colorScheme;
	
	static ofColor color1;
	static ofColor color2;
	static ofColor color3;
	
	static string soundFile;
	
	static float micVolume;
	static float outputVolume;

	static float micLevel;
	// the default image of any imagery
	static ofImage *image;
	
	// is the camera available
	static bool cameraAvailable;
	static bool mirrorCamera;
	static vector<AppSettingsListener*> listeners;
	static void addListener(AppSettingsListener *listener);
};