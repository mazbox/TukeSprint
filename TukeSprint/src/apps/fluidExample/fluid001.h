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
	
	fluid001(string N, string icon, int cellsX, float vis, float colorDiff, float colorFade, int solver, int drawMode, int particleType);
	~fluid001();
	
	void init();
	
	string getName() { return Name; } 
	string getScreenshotFileName() { return iconPath; }
	
	void update();
	void draw();
	void exit();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void windowResized(int w, int h);
	
	
	//deals with openCV
	void updateOCV();
	void setupOCV();
	
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);
	void opticalFlowToFluid();
			
	int						fluidCellsX;
	bool					resizeFluid;
	bool					drawFluid;
	bool					drawParticles;
	bool					renderUsingVA;
	
	ofxMSAFluidSolver		fluidSolver;
	ofxMSAFluidDrawer		fluidDrawer;	
	
	ParticleSystem			particleSystem;
	
	int						pmouseX, pmouseY;
	
	ofSoundPlayer 			sample[3];
	
	
	string Name;
	string iconPath;

	
	// ----- OPEN CV STUFF
	ofVideoGrabber 			vidGrabber;
	ofxCvColorImage			colorImg;
	
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
	
	int					mouseX, mouseY;	
};

extern fluid001 *myApp;



