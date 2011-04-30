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
    float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	//printf("%f, %f\n", dx, dy);
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
		
		if(addColor) {
			msaColor drawColor;
	
			//int hue = lroundf((x + y) * 180 + ofGetFrameNum()) % 360;
			
			fluidSolver.r[index]  += colorPick.col.r;
			fluidSolver.g[index]  += colorPick.col.g;
			fluidSolver.b[index]  += colorPick.col.b;

			if(drawParticles) particleSystem.addParticles(x * window.width, y * window.height, 10);
		}
		
		if(addForce) {
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		
		if(!drawFluid && ofGetFrameNum()%5 ==0) fadeToColor(0, 0, 0, 0.1);
    }
	
	
}
//--------------------------------------------------------------
#pragma mark App callbacks
char sz[] = "ofxMSAFluid Demo | (c) 2009 Mehmet Akten | www.memo.tv";
//--------------------------------------------------------------
void testApp::setupGui(){
	//---------- PANEL
	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 60, 200));
	ofxControlPanel::setTextColor(simpleColor(VISION_HEIGHT, 50, 50, 255));
	
	gui.loadFont("Kai.ttf", 10);		
	gui.setup("fluidExample", 0, 0, 900, 440);
	
	gui.addPanel("", 8, false);
	gui.addPanel("Flow Control", 8, false);
	gui.addPanel("Input", 8, false);
	gui.addPanel("Partile Type", 8, false);
	
	//--------- PANEL 1
	gui.setWhichPanel(0);
	gui.setWhichColumn(0);
	
	gui.addSlider("fluidCellsX", "fluid_CellsX", 100, 20, 400, true);
	gui.addToggle("resizeFluid", "resize_Fluid", 0);
	gui.addSlider("fs.viscosity", "fs_viscosity", 0.0002, 0.0, 0.01, false);
	gui.addSlider("fs.colorDiffusion", "fs_colorDiffusion", 0.0003, 0.0, 0.01, false);
	gui.addSlider("fs.fadeSpeed", "fs_fadeSpeed", 0.1, 0.0, 0.5, false);
	gui.addSlider("fs.solverIterations", "fs_solverIterations", 1, 1, 20, true);
	//gui.addSlider("fd.drawMode", "fd_drawMode", 1, 0, FLUID_DRAW_MODE_COUNT-1, true);
	vector <string> modes;
	modes.push_back("FLUID_DRAW_COLOR");
	modes.push_back("FLUID_DRAW_MOTION");
	modes.push_back("FLUID_DRAW_SPEED");
	modes.push_back("FLUID_DRAW_VECTORS");
	
	gui.addTextDropDown("fd.drawMode", "fd_drawMode", 0, modes);
	
	gui.setWhichColumn(1);
	gui.addToggle("fs.doRGB", "fs_doRGB", 0);
	gui.addToggle("fs.doVorticityConfinement", "fs_doVorticityConfinement", 0);
	gui.addToggle("drawFluid", "draw_Fluid", 0);
	gui.addToggle("drawParticles", "draw_Particles", 0);
	gui.addToggle("renderUsingVA", "render_UsingVA", 0);
	gui.addToggle("fs.wrapX", "fs_wrapX", 0);
	gui.addToggle("fs.wrapY", "fs_wrapY", 0);
	
	gui.setWhichColumn(6);
	gui.addCustomRect("colorPicker", &colorPick, 256, 256);
	
	//--------- PANEL 2
	gui.setWhichPanel(1);
	gui.setWhichColumn(0);
	
	gui.addDrawableRect("colorImg", &colorImg, 200, 150);
	gui.addDrawableRect("grayImg", &grayImage, 200, 150);
	
	gui.setWhichColumn(3);
	gui.addDrawableRect("flow", &flow, 200, 150);
	gui.addSlider("optical flow smoothing", "opticalflowBlur", 1, 1, 6, true);


	//--------- PANEL 3
	gui.setWhichPanel(2);
	gui.setWhichColumn(0);
	vector <string> names;
	names.push_back("mouse");
	names.push_back("flow");

	gui.addTextDropDown("Input Method", "Input_Method", 0, names);
	
	//--------- PANEL 4
	gui.setWhichPanel(3);
	gui.setWhichColumn(0);
	vector <string> particletype;
	particletype.push_back("LINES");
	particletype.push_back("LINE_LOOP");
	particletype.push_back("POLYGON");
	particletype.push_back("TRIANGLE_STRIP");
	particletype.push_back("LINES_ADJACENCY_EXT");
	particletype.push_back("TRIANGLES_ADJACENCY_EXT");
	particletype.push_back("TRIANGLE_FAN");
	
	gui.addTextDropDown("Particle Type", "Particle_Type", 0, particletype);
	
	
	fluidCellsX			= 150;
	
	drawFluid			= true;
	gui.setValueB("draw_Fluid", 1, 0);
	drawParticles		= true;
	gui.setValueB("draw_Particles", 1, 0);
	renderUsingVA		= true;
	gui.setValueB("render_UsingVA", 1, 0);	
	fluidSolver.doRGB = true;
	gui.setValueB("fs_doRGB", 1, 0);
	resizeFluid			= true;
	gui.loadSettings("guiSettings.xml");
	
}
//--------------------------------------------------------------
void testApp::exit() {
	gui.saveSettings("guiSettings.xml");
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
	
	setupGui();
	setupOCV();
	sample[0].loadSound("1.wav", false);
	sample[1].loadSound("2.wav", false);
	sample[2].loadSound("3.wav", false);
	
	
	// setup fluid stuff
	fluidSolver.setup(100, 100);

    fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	window.aspectRatio	= 1;
	
	// set up OF stuff
	ofBackground(0, 0, 5);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableAlphaBlending();

}
//--------------------------------------------------------------
void testApp::updateGui(){
	//------ update all of the gui elements into the fluid program variables
	
	resizeFluid = gui.getValueB("resize_Fluid");
	fluidCellsX = gui.getValueI("fluid_CellsX");
	fluidSolver.viscocity = gui.getValueF("fs_viscosity");
	fluidSolver.colorDiffusion = gui.getValueF("fs_colorDiffusion");
	fluidSolver.fadeSpeed = gui.getValueF("fs_fadeSpeed");
	fluidSolver.solverIterations = gui.getValueI("fs_solverIterations");
	fluidDrawer.drawMode = gui.getValueI("fd_drawMode");
	
	fluidSolver.doRGB = gui.getValueB("fs_doRGB");
	fluidSolver.doVorticityConfinement = gui.getValueB("fs_doVorticityConfinement");
	drawFluid = gui.getValueB("draw_Fluid");
	drawParticles = gui.getValueB("draw_Particles");
	renderUsingVA = gui.getValueB("render_UsingVA");
	fluidSolver.wrap_x = gui.getValueB("fs_wrapX");
	fluidSolver.wrap_y = gui.getValueB("fs_wrapY");
	
	inputmode = gui.getValueI("Input_Method");
	
	if(resizeFluid) 	{
		fluidSolver.setSize(fluidCellsX, fluidCellsX / window.aspectRatio);
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
		gui.setValueB("resize_Fluid", 0, 0);
	}
	
	particleSystem.setParticleType(gui.getValueI("Particle_Type"));
	
	gui.update();
	
}
//--------------------------------------------------------------
void testApp::updateOCV(){

    bool bNewFrame = false;
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		
		colorImg.setFromPixels(vidGrabber.getPixels(), VISION_WIDTH,VISION_HEIGHT);
		
		colorImg.mirror(false, true);
        grayImage = colorImg;
		
		flow.calc(grayLast, grayImage, 7);
		flow.filter(gui.getValueF("opticalflowBlur"));
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

				addToFluid((float)x/flow.captureWidth, (float)y/flow.captureHeight, dx*opticalFlowFluidMult, dy*opticalFlowFluidMult);//, bool addColor = true, bool addForce = true);
				//fluid->addForceAtPos(x * iw, y * iy, dx * opticalFlowFluidMult, dy * opticalFlowFluidMult);
				/*particleEmitCounter++;
				if(particleEmitCounter % (globals.cube2.emitSkipFrames+1) == 0) {
					globals.physics.addParticle(new Cube2(x * iw * ofGetWidth(), y * iy * ofGetHeight(), 0.0f, dx * globals.cube2.camVelMult, dy * globals.cube2.camVelMult));
				}*/
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

	
	
	
	//if input mode is '0' use mouse as interaction (see mouse listener)
	if(inputmode==0){
		pmouseX = mouseX;
		pmouseY = mouseY;
	}
	//if input mode is '1' use optical flow as interaction
	if(inputmode==1){
		opticalFlowToFluid();
	}
	
	//if input mode is '2' use facial Gestures as interaction	
	if(inputmode==2){

	}
	
}
//--------------------------------------------------------------
void testApp::draw(){
	ofSetBackgroundAuto(drawFluid);
	
	if(drawFluid) {
		fluidDrawer.draw(0, 0, window.width, window.height);
	}
	
	if(drawParticles) particleSystem.updateAndDraw();

	ofPushStyle();
	
	gui.draw();	
		
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
	window.width		= w;
	window.height		= h;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
	
	resizeFluid = true;
}
#pragma mark Input callbacks
//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    switch(key) {

		case ' ':
			
			gui.toggleView();
				
			glClear(GL_COLOR_BUFFER_BIT);
			break;

		case 'f':
			ofToggleFullscreen();
			break;
		case 's':
			static char fileNameStr[255];
			sprintf(fileNameStr, "output_%i.png", ofGetFrameNum());
			static ofImage imgScreen;
			imgScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			printf("Saving file: %s\n", fileNameStr);
			imgScreen.saveImage(fileNameStr);
			break;	
    }
}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if(inputmode==0){
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;
    float mouseVelX = (x - pmouseX) * window.invWidth;
    float mouseVelY = (y - pmouseY) * window.invHeight;

    addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);
	}
	mouseY = y;
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	if(inputmode==0){
		float mouseNormX = x * window.invWidth;
		float mouseNormY = y * window.invHeight;
		float mouseVelX = (x - pmouseX) * window.invWidth;
		float mouseVelY = (y - pmouseY) * window.invHeight;
		
	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, false);
	}
	
	gui.mouseDragged(x, y, button);
}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	gui.mousePressed(x, y, button);
}
//--------------------------------------------------------------
void testApp::mouseReleased(){
	gui.mouseReleased();
}