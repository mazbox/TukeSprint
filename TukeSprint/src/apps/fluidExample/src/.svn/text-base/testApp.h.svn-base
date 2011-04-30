#pragma once

#include "ofMain.h"
#include "ofxMSAFluid.h"
#include "ParticleSystem.h"
#include "ofxControlPanel.h"
#include "customDrawer.h"
#include "ofxOpenCv.h"
#include "ofxCvOpticalFlowLK.h"

#define USE_GUI		
#define SAMPLE 2
#define VISION_WIDTH  320
#define VISION_HEIGHT 240


class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void windowResized(int w, int h);
	
	void updateGui();
	void updateOCV();
	void setupGui();
	void setupOCV();
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);
	void opticalFlowToFluid();
	
	int inputmode;
		
	customDrawer		colorPick;
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;	
	
	ParticleSystem		particleSystem;
	
	int					pmouseX, pmouseY;
	
	ofSoundPlayer sample[3];

	
	// ----- OPEN CV STUFF
	ofVideoGrabber 		vidGrabber;
	ofxCvColorImage		colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayLast;
	ofxCvOpticalFlowLK		flow;
	
	// cache these for slightly better performance
	struct {
		int				width;
		int				height;
		float			invWidth;
		float			invHeight;
		float			aspectRatio;
		float			aspectRatio2;
	} window;
	
	
#ifdef USE_GUI 
	ofxControlPanel gui;

#endif
	
	
};

extern testApp *myApp;



