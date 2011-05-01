#include "AppSettings.h"
#include "deathBox.h"
#include "stdio.h"
#include "AppSettings.h"

//--------------------------------------------------------------
deathBox::deathBox() {	

}

//--------------------------------------------------------------
void deathBox::init() {
	
	
	colorImg.allocate(320,240);
	
	//a counter and bool for timing explosion animations
	counter=0;
	
	//a counter to time container version
	contCounter=0;
	
	myBool=false;
	//contBool=false;
	contBool=true;

	
	cutoff = 1000.f;

	//nImages=DIR.listDir("images");
	//images= new ofImage[nImages];
	//cout<< nImages <<"nImages \n";
	//for(int i=0;i<nImages;i++){
	//	images[i].loadImage(DIR.getPath(i));	
	//}
	
	
	filterThreshold=0.9f;
	
	ofEnableAlphaBlending();

	//APPSETTINGS
	red_colour		= 127;
	green_colour		=127;
	blue_colour		=127;
	
	//we dont trust ofGetScreenHeight with box2D physics
	screenH=ofGetHeight();
	screenW=ofGetWidth();
	
	//amplitute of input
	inputLevel = 0;
	
	//amplitude of high freq
	filteredInputLevel = 0;
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	ofToggleFullscreen();
	bDrawLines  = false;
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 20);
	
	//this call is creating the floor at the bottom of the window; not the screen!?
	//box2d.createFloor();
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
	//ofSoundStreamSetup(0,2,this, 44100, 256, 1);	
	//left = new float[256];
	//right = new float[256];
		
	//bufferCounter = 0;
	//drawCounter = 0;	
	
	lineStrip.setWorld(box2d.getWorld());
	lineStrip.clear();
	
	lineStrip.addPoint(0,screenH);
	lineStrip.addPoint(screenW,screenH);
	lineStrip.createShape();

	//setting up the upDirection and upForce
	upDirection.x = 20;
	upDirection.y = 20;
	upForce.x = 20;
	upForce.y = 10;
	
	threshold=0.27;
	
}

//--------------------------------------------------------------


//--------------------------------------------------------------
void deathBox::update() {
		
	ofBackground(0, 0, 0);
	
	colorImg.setFromPixels(video->getPixels(), 320, 240);
	
	box2d.update();
	
	//generateParticles(820);
	if(contBool==false){
		generateParticles(screenH-80,0,screenW);
		
		if(container.body!=NULL){
			container.destroyShape();
		}
		
	}
	else{
		if(contCounter<500){
			generateParticles(screenH-80,screenW/3,screenW-(screenW/3));
			contCounter++;
			//cout<<contCounter<<" contCounter \n";
			
		}
		drawContainer();
		
	}
	
	
	if(customParticles.size()>500){
		customParticles.erase(customParticles.begin() );
		customParticles[0]->destroyShape();
	}
	
	
	
	filter.calc_filter_coeffs(1,cutoff,2,0,false);
	
	threshold = AppSettings::micLevel;
	
	
	cout << inputLevel << endl;
	
	
	for (int i=0; i<customParticles.size(); i++){
		
		if(inputLevel>threshold){

			if(contBool==false){
				customParticles[i]->addAttractionPoint((screenW/2,0,0), 0, 1.0f, 1);
				customParticles[i]->addImpulseForce(upDirection.y, upForce.y);
				//customParticles[i]->addImpulseForce(upDirection.y, inputLevel*10);
			}
			if(contBool==true){
				customParticles[i]->addAttractionPoint((screenW,0,0), 0, 1.0f, 1);
				//customParticles[i]->addAttractionPoint((0,0,0), 0, 1.0f, 1);
				
				customParticles[i]->addImpulseForce(upDirection.y, upForce.y);
				
			}	
		}		
	}
}



//--------------------------------------------------------------
void deathBox::draw() {

	//if(gui.getValueI("DIFF_MODE")!=0){
	//	images[gui.getValueI("DIFF_MODE")-1].draw(screenW/3,screenH/2, screenW/3,screenH/2);
	//}
	
	ofPushStyle();
	colorImg.draw(screenW/3, screenH/2, screenW/3, screenH/2);
	ofPopStyle();
	
	
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i]->draw();
	}
	

	//explode on high freq
	if(contBool==false){
		popOnHighFreq();
	}
	
	if(ofGetFrameNum()>500){
		for(int i=1;i<30;i++){
			//customParticles[i]->explode();
		}
	}

	if (contBool==true){		
		drawWalls();
	}
	
}

//--------------------------------------------------------------
void deathBox::audioReceived (float * input, int bufferSize, int nChannels){	
	
	float max = 0;
	float maxFiltered = 0;
	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		
		//get max value from buffer
		float value = MAX(ABS(input[i*2]),ABS(input[i*2+1]));
		
		if(inputLevel<value) inputLevel = value;
		
		//if its not peaking reduce level by a bit
		else inputLevel *= 0.9995;
		
		float filteredOutput = filter.filter((input[i*2]+input[i*2+1])*0.5);
		if(filteredInputLevel<filteredOutput) filteredInputLevel = filteredOutput;
		else filteredInputLevel *= 0.9995;
		
	}
	bufferCounter++;
	
}
//--------------------------------------------------------------
void deathBox::keyPressed(int key) {
	
	//setting the threshold...
	if(key == 'k') {
		threshold+=0.2;
	}
	if(key == 'c') {
		contBool=!contBool;
	}
	if(key == 'm') {
		threshold-=0.2;
	}
	if(key == 'f') {
		ofToggleFullscreen();
	}
	
}

//--------------------------------------------------------------
void deathBox::keyReleased(int key) {
}

//--------------------------------------------------------------
void deathBox::mouseMoved(int x, int y ) {
	mouseX=x;
	mouseY=y;
}

//--------------------------------------------------------------
void deathBox::mouseDragged(int x, int y, int button) {
	mouseX=x;
	mouseY=y;
}

//--------------------------------------------------------------
void deathBox::mousePressed(int x, int y, int button) {
	
	
}

//--------------------------------------------------------------
void deathBox::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void deathBox::resized(int w, int h){
	
}

//--------------------------------------------------------------
void deathBox::generateParticles(int height,int x,int y){

	float r = ofRandom(3, 10);		// a random radius 4px - 20px
	CustomParticle *p = new CustomParticle();
	//p.setPhysics(0.4, 0.53, 0.31);
	p->setPhysics(10.9, 0.53, 0);
	
	
	p->setRadius();
	//p.setup(box2d.getWorld(), ofRandom(0,ofGetWidth()),2*(ofGetHeight()/3), r);
	p->setup(box2d.getWorld(), ofRandom(x,y),height, r);
	p->color.r = AppSettings::color1.r+ofRandom(20, 100);
	p->color.g = AppSettings::color1.g;
	p->color.b = AppSettings::color1.b+ofRandom(150, 255);
	customParticles.push_back(p);
	
}

//--------------------------------------------------------------
void deathBox::graph(int x, int y,int height){
	ofSetColor(10, 0, 200);	
	ofRect(x, y, 30, -1*height);	
}

//--------------------------------------------------------------
void deathBox::shrinkPop(float circX, float circY, float radiusBurst){
	
	//period in frames for animation to run
	int period=20;
	bool myBool;
	int circsize;
	int size;
	
	if(ofGetFrameNum()%period==0){
		myBool=true;
		circsize=0;
		size=0;		
	}
	
	if(myBool==true){
		startTime=ofGetFrameNum();
		circsize=1;
		myBool=false;
	}
	
	
	if(ofGetFrameNum()-startTime<=period){		
		ofFill();
		ofSetColor(255, 255, 255);		
		size++;		
	}
	
}

//--------------------------------------------------------------
void deathBox::popOnHighFreq(){

	if(ofGetFrameNum()>30){
		if(filteredInputLevel>filterThreshold){
			myBool=true;
			cout<<filteredInputLevel<<"FILTERED \n";		
		}
		
		
		if(myBool==true){
			for (int i=0; i<30; i++){
				customParticles[i]->explode();
				
			}
	
			counter++;
			if(counter==30){
				myBool=false;
				counter=0;
			}
		}
	}
	
	
}

//--------------------------------------------------------------
void deathBox::drawContainer(){
	//create invisible container walls
	container.setWorld(box2d.getWorld());
	container.clear();
	container.addPoint((screenW/3),screenH-(screenH/2));
	container.addPoint((screenW/3),screenH);
	container.addPoint(screenW-(screenW/3),screenH);
	container.addPoint(screenW-(screenW/3),screenH-(screenH/2));	
	container.createShape();
	
	container1.setWorld(box2d.getWorld());
	container1.clear();
	container1.addPoint((screenW/3),screenH-(screenH/2));
	container1.addPoint((screenW/3),screenH);
	container1.addPoint(0,screenH);
	container1.createShape();
	
	container2.setWorld(box2d.getWorld());
	container2.clear();
	container2.addPoint(screenW-(screenW/3),screenH-(screenH/2));	
	container2.addPoint(screenW-(screenW/3),screenH);
	container2.addPoint(screenW,screenH);	
	container2.createShape();
	
}

//--------------------------------------------------------------
void deathBox::drawWalls(){
	
	ofPushStyle();
	ofSetColor(255-red_colour, 255-green_colour, 255-blue_colour);
	ofSetLineWidth(10);
	//test line... will it appear
	ofBeginShape();
	ofNoFill();
	ofVertex((screenW/3),screenH-(screenH/2));
	ofVertex((screenW/3),screenH);
	ofVertex(screenW-(screenW/3),screenH);
	ofVertex(screenW-(screenW/3),screenH-(screenH/2));	
		
	ofEndShape();
	
	ofPopStyle();
	
}
