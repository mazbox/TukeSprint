#include "testApp.h"
#include "Particle.h"
#include "msaColor.h"
#include "ParticleSystem.h"


#pragma mark Custom methods
//--------------------------------------------------------------
void fadeToColor(float r, float g, float b, float speed) {
	
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(myApp->window.width, 0);
    glVertex2f(myApp->window.width, myApp->window.height);
    glVertex2f(0, myApp->window.height);
    glEnd();

}
//--------------------------------------------------------------
void testApp::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce) {
   
	float speed = dx * dx  + dy * dy * window.aspectRatio2;    

    if(speed > 0) {
        if(x<0) x = 0; 
        else if(x>1) x = 1;
        if(y<0) y = 0; 
        else if(y>1) y = 1;
		
        float colorMult = 50;
        float velocityMult = 30;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(sample[SAMPLE].getIsPlaying()==false){
			sample[SAMPLE].play();
		}
		

		//APPSETTINGS - color picker from global variables
		if(addColor) {
			msaColor drawColor;
			
			fluidSolver.r[index]  += colorPick.col.r;
			fluidSolver.g[index]  += colorPick.col.g;
			fluidSolver.b[index]  += colorPick.col.b;

			if(drawParticles) particleSystem.addParticles(x * window.width, y * window.height, 10);
		}
		
		if(addForce) {
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		

		//APPSETTINGS - fade to color?
		if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
	
	
}
//--------------------------------------------------------------
#pragma mark App callbacks
//char sz[] = "ofxMSAFluid Demo | (c) 2009 Mehmet Akten | www.memo.tv";
//--------------------------------------------------------------


void testApp::setPresets(){

	fluidCellsX				= 150;	
	drawFluid				= true;	
	drawParticles				= true;	
	renderUsingVA				= true;	
	fluidSolver.doRGB 			= true;	
	
	fluidCellsX 				= 
	fluidSolver.viscocity 			=
	fluidSolver.colorDiffusion		= 
	fluidSolver.fadeSpeed 			=
	fluidSolver.solverIterations 		= 
	fluidDrawer.drawMode 			= 
	
	fluidSolver.doVorticityConfinement 	= 
	fluidSolver.wrap_x 			= 
	fluidSolver.wrap_y 			= 
	
	particleSystem.setParticleType(0);

}

//--------------------------------------------------------------

void testApp::exit() {

}

//--------------------------------------------------------------

void testApp::setupOCV(){

	//openCV things
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(VISION_WIDTH,VISION_HEIGHT);
	colorImg.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayImage.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayLast.allocate(VISION_WIDTH,VISION_HEIGHT);
	flow.allocate(VISION_WIDTH,VISION_HEIGHT);
	
}
//--------------------------------------------------------------
void testApp::setup() {	 

	setupOCV();

	//ADD refrences to Global sound Variables
	sample[0].loadSound("1.wav", false);
	sample[1].loadSound("2.wav", false);
	sample[2].loadSound("3.wav", false);
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);

   	fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);

	//set presets for how the aplication will look - this is all that will change in the different versions
	setPresets();
	
	window.aspectRatio	= 1;
	
	// set up OF stuff
	ofBackground(0, 0, 5);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableAlphaBlending();

}

//--------------------------------------------------------------
void testApp::updateOCV(){

    bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), VISION_WIDTH, VISION_HEIGHT);
		
		colorImg.mirror(false, true);
       		grayImage = colorImg;
		
		flow.calc(grayLast, grayImage, 7);
		flow.filter("FLOWFILTERVALUE?");
		grayLast = grayImage;
	}	
}
//--------------------------------------------------------------
void testApp::opticalFlowToFluid() {
	
	int x, y;
	float dx, dy;
	
	float iw			= 1.0f/flow.captureWidth;
	float iy			= 1.0f/flow.captureHeight;
	
	int particleEmitCounter = 0;
	float flowThreshold = 100;
	float opticalFlowFluidMult = 0.001;
	float multX = (float)ofGetWidth()/flow.captureWidth;
	float multY = (float)ofGetHeight()/flow.captureHeight;
	
	for ( y = 0; y < flow.captureHeight; y+=flow.captureRowsStep ){
		for ( x = 0; x < flow.captureWidth; x+=flow.captureColsStep ){
			
			dx = cvGetReal2D( flow.vel_x, y, x );
			dy = cvGetReal2D( flow.vel_y, y, x );
			if(dx*dx+dy*dy > flowThreshold) {

			addToFluid((float)x/flow.captureWidth, (float)y/flow.captureHeight, dx*opticalFlowFluidMult, dy*opticalFlowFluidMult);
			
			}
		}
	}
}
//--------------------------------------------------------------
void testApp::update(){

	updateGui();
	updateOCV();
	
	fluidSolver.update();

	//average speed in fluid is used to decay the volume  of the sample
	//sample is triggered in addfluid function.

	sample[SAMPLE].setVolume(fluidSolver._avgSpeed*10000);

	sample[SAMPLE].setSpeed(1.0-((float)mouseY/(float)ofGetHeight()));
	
	//always use mouse interaction
	pmouseX = mouseX;
	pmouseY = mouseY;
	
	//APPSETTINGS - if camera is detected on startup use camera - 
	if(appSettings->camIsAvailable){
		opticalFlowToFluid();
	}
	
}
//--------------------------------------------------------------
void testApp::draw(){
	ofSetBackgroundAuto(drawFluid);
	
	if(drawFluid) {
		fluidDrawer.draw(0, 0, window.width, window.height);
	}
	
	if(drawParticles) particleSystem.updateAndDraw();

}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
	window.width		= w;
	window.height		= h;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
	
	fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
	fluidDrawer.setup(&fluidSolver);

}

#pragma mark Input callbacks
//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    switch(key) {

		case 'f':
			ofToggleFullscreen();
			break;

    }
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

	float mouseNormX = x * window.invWidth;
   	float mouseNormY = y * window.invHeight;
    	float mouseVelX = (x - pmouseX) * window.invWidth;
    	float mouseVelY = (y - pmouseY) * window.invHeight;

    	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);
	
	mouseY = y;
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
	float mouseNormX = x * window.invWidth;
	float mouseNormY = y * window.invHeight;
	float mouseVelX = (x - pmouseX) * window.invWidth;
	float mouseVelY = (y - pmouseY) * window.invHeight;
		
	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, false);
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::mouseReleased(){

}