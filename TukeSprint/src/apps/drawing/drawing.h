#pragma once

//#define DEBUG

#include "TukeApp.h"
#include "ofxOpenCv.h"
#include "Blob.h"
#include "ofxDirList.h"
#include "AppSettings.h"

class drawing : public TukeApp, public AppSettingsListener{

	public:
		void update();
		void draw();
		void init();
		void start();
		void colorChanged() { blobColor=AppSettings::color1; crossColor=AppSettings::color2; backgroundColor=AppSettings::color3;};
		void imageChanged() {selImage=AppSettings::image;};
		string getName() { return "Catch the Sun"; } 
		string getScreenshotFileName() { return "resources/icons/drawing.png"; }

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

	vector<blob> blobs;
	
	int current;
	ofPoint currMouse, prevMouse;
	int timeElapsed;
	
	int BLOB_SIZE;
	int LOWPASS;  //must be the same as size of the array smoothPos
	//ofSoundPlayer  beats;
	
//	ofVideoGrabber 		vidGrabber;
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	ofxCvContourFinder 	contourFinder;
	
	ofSoundPlayer sample;
	
	ofPoint targetPos;
	bool explode;
	bool usingMouseInput;
	int explosionTimer;
	
	int 				threshold;
	bool				bLearnBakground;
	
	int _x,_y;
	
	ofPoint blobPosition;
	ofPoint lastBlobPosition;
	int indexSmoothPos;
	ofPoint smoothPos[45];
	float pulsatingCircle;
	

	vector<string> files;
	string currentSound;
	ofColor blobColor;
	ofColor crossColor;
	ofColor backgroundColor;
	ofImage *selImage;
	
	int numFiles;
	int transparency;
};

