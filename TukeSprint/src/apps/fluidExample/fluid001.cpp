 #include "fluid001.h"
#include "ParticleMemo.h"
#include "msaColor.h"
#include "ParticleSystem.h"
#include "AppSettings.h"

fluid001 *myApp;
#pragma mark Custom methods


//--------------------------------------------------------------
fluid001::fluid001(string N, string icon, int cellsX, float vis, float colorDiff, float colorFade, int solver, int drawMode, int particleType){

	Name = N;
	iconPath = icon;

	drawFluid					= true;
	drawParticles				= true;
	renderUsingVA				= true;
	fluidSolver.doRGB 			= true;

	fluidCellsX 				= cellsX;
	fluidSolver.viscocity 		= vis;
	fluidSolver.colorDiffusion	= colorDiff;
	fluidSolver.fadeSpeed 		= colorFade;
	fluidSolver.solverIterations= solver;
	fluidDrawer.drawMode 		= drawMode;

	fluidSolver.doVorticityConfinement 	= 0;
	fluidSolver.wrap_x 			= 0;
	fluidSolver.wrap_y 			= 0;

	particleSystem.setParticleType(particleType);


}


//--------------------------------------------------------------

fluid001::~fluid001(){

}


//--------------------------------------------------------------
void fluid001::init() {
	myApp=this;

	setupOCV();

	//ADD refrences to Global sound Variables
	sample[0].loadSound("1.wav", false);
	sample[1].loadSound("2.wav", false);
	sample[2].loadSound("3.wav", false);


	windowResized(ofGetWidth(), ofGetHeight());


	// setup fluid stuff
	fluidSolver.setup(100, 100);

   	fluidSolver.enableRGB(true).setFadeSpeed(0.006).setDeltaT(0.5).setVisc(0.00017).setColorDiffusion(0.00011);
	fluidDrawer.setup(&fluidSolver);




}

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
void fluid001::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce) {

	float speed = dx * dx  + dy * dy * window.aspectRatio2;

	cout << dx << " " << dy << endl;

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

			fluidSolver.r[index]  += AppSettings::color1.r;
			fluidSolver.g[index]  += AppSettings::color1.g;
			fluidSolver.b[index]  += AppSettings::color1.b;


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

void fluid001::start(){
    printf("started fluid001");
}

void fluid001::stop(){
    sample[SAMPLE].stop();
}

void fluid001::setupOCV(){

	colorImg.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayImage.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayLast.allocate(VISION_WIDTH,VISION_HEIGHT);
	flow.allocate(VISION_WIDTH,VISION_HEIGHT);

}

//--------------------------------------------------------------
void fluid001::updateOCV(){

   		colorImg.setFromPixels(video->getPixels(), VISION_WIDTH, VISION_HEIGHT);

		colorImg.mirror(false, true);
		grayImage = colorImg;

		flow.calc(grayLast, grayImage, 7);
		flow.filter(1);//"FLOWFILTERVALUE?");
		grayLast = grayImage;

}
//--------------------------------------------------------------
void fluid001::opticalFlowToFluid() {

	int								x, y;
	float							dx, dy;

	float iw					= 1.0f/flow.captureWidth;
	float iy					= 1.0f/flow.captureHeight;

	int particleEmitCounter		= 0;
	float flowThreshold			= 100;
	float opticalFlowFluidMult	= 0.001;
	float multX					= (float)ofGetWidth()/flow.captureWidth;
	float multY					= (float)ofGetHeight()/flow.captureHeight;

	for ( y = 0; y < flow.captureHeight; y+=flow.captureRowsStep ){
		for ( x = 0; x < flow.captureWidth; x+=flow.captureColsStep ){

			dx					= cvGetReal2D( flow.vel_x, y, x );
			dy					= cvGetReal2D( flow.vel_y, y, x );
			if(dx*dx+dy*dy > flowThreshold) {

			addToFluid((float)x/flow.captureWidth, (float)y/flow.captureHeight, dx*opticalFlowFluidMult, dy*opticalFlowFluidMult);

			}
		}
	}
}
//--------------------------------------------------------------
void fluid001::update(){

	ofBackground(0, 0, 0);

	updateOCV();

	fluidSolver.update();

	sample[SAMPLE].setVolume(fluidSolver._avgSpeed*10000);

	sample[SAMPLE].setSpeed(1.0-((float)mouseY/(float)ofGetHeight()));

	//always use mouse interaction
	pmouseX = mouseX;
	pmouseY = mouseY;

	if(AppSettings::cameraAvailable){
		opticalFlowToFluid();
	}

}
//--------------------------------------------------------------
void fluid001::draw(){
	ofSetBackgroundAuto(drawFluid);

	if(drawFluid) {
		fluidDrawer.draw(0, 0, window.width, window.height);
	}

	if(drawParticles) particleSystem.updateAndDraw();

}
//--------------------------------------------------------------
void fluid001::windowResized(int w, int h) {

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
void fluid001::keyPressed (int key){

}
//--------------------------------------------------------------
void fluid001::mouseMoved(int x, int y){

	float mouseNormX		= x * window.invWidth;
   	float mouseNormY		= y * window.invHeight;
	float mouseVelX			= (x - pmouseX) * window.invWidth;
	float mouseVelY			= (y - pmouseY) * window.invHeight;

	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);

	mouseY = y;
	mouseX = x;
}
//--------------------------------------------------------------
void fluid001::mouseDragged(int x, int y, int button) {

	float mouseNormX		= x * window.invWidth;
	float mouseNormY		= y * window.invHeight;
	float mouseVelX			= (x - pmouseX) * window.invWidth;
	float mouseVelY			= (y - pmouseY) * window.invHeight;

	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);

	mouseY = y;
	mouseX = x;
}
//--------------------------------------------------------------
void fluid001::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void fluid001::mouseReleased(){

}
