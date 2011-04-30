#pragma once

#include "TukeApp.h"
#include "ofxMSAFluid.h"
#include "ParticleSystem.h"
#include "ofxOpenCv.h"
#include "ofxCvOpticalFlowLK.h"
	
#define SAMPLE 2
#define VISION_WIDTH  320
#define VISION_HEIGHT 240


class fluid001 : public TukeApp{
public:
	void init();
	string getName() { return "Fluid 1"; } 
	string getScreenshotFileName() { return "icons/fluid1.png"; }
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


	//caontrol all of the ingoing presets
	void setPresets();
	
	
	//deals with openCV
	void updateOCV();
	void setupOCV();
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);
	void opticalFlowToFluid();
			
	int				fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	
	ofxMSAFluidSolver		fluidSolver;
	ofxMSAFluidDrawer		fluidDrawer;	
	
	ParticleSystem			particleSystem;
	
	int				pmouseX, pmouseY;
	
	ofSoundPlayer 			sample[3];

	
	// ----- OPEN CV STUFF
	ofVideoGrabber 			vidGrabber;
	ofxCvColorImage			colorImg;
	
	ofxCvGrayscaleImage 		grayImage;
	ofxCvGrayscaleImage 		grayLast;
	ofxCvOpticalFlowLK		flow;
	
	// cache these for slightly better performance
	struct {
		int			width;
		int			height;
		float			invWidth;
		float			invHeight;
		float			aspectRatio;
		float			aspectRatio2;
	} window;
	
	int mouseX, mouseY;	
};

extern fluid001 *myApp;



