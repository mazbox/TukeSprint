#include "testApp.h"

testApp *testApp::instance = NULL;

//--------------------------------------------------------------
void testApp::setup(){
	instance = this;
	
	ofSetFrameRate(30);
	video.initGrabber(320, 240);
	
	mainMenu.init();
	currApp = NULL;
	showMainMenu();
	help.setup(video);

	ofSoundStreamSetup(2, 1, this, 44100, 1024, 1);
}


void testApp::showMainMenu() {
	if(currApp!=NULL) {
		currApp->stop();
	} 
	launchTukeApp(&mainMenu);
}

void testApp::launchTukeApp(TukeApp *app) {
	currApp = app;
	currApp->start();
}

void testApp::showHelp() {
	help.enable();
}

//--------------------------------------------------------------
void testApp::update() {
	video.grabFrame();
	if(currApp!=NULL) {
		currApp->video = &video;
		currApp->update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if(currApp!=NULL) {
		currApp->draw();
	}
	
	if(help.isEnabled()) {
		help.draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key) {
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
		default:
			keyQueue.push_back(key);
			while(keyQueue.size()>4) keyQueue.pop_front();
			checkKeyPresses();
			break;
	}
}

void testApp::checkKeyPresses() {
	if(keyQueue.size()==4) {
		if(keyQueue[0]=='h' && keyQueue[1]=='e' 
		   && keyQueue[2]=='l' && keyQueue[3]=='p') {
			showHelp();
		} else if(keyQueue[0]=='m' && keyQueue[1]=='e' 
				  && keyQueue[2]=='n' && keyQueue[3]=='u') {
			showMainMenu();
		} else if(keyQueue[0]=='q' && keyQueue[1]=='u' 
				  && keyQueue[2]=='i' && keyQueue[3]=='t') {
			OF_EXIT_APP(0);
		}
		//printf("%c %c %c %c\n", keyQueue[0], keyQueue[1], keyQueue[2], keyQueue[3]);
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if(help.isEnabled()) return;
	if(currApp!=NULL) {
		currApp->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(help.isEnabled()) return;
	if(currApp!=NULL) {
		currApp->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(help.isEnabled()) return;
	if(currApp!=NULL) {
		currApp->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(help.isEnabled()) return;
	if(currApp!=NULL) {
		currApp->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


void testApp::audioReceived( float * input, int bufferSize, int nChannels ) {
	float max = 0;
	float vol = AppSettings::micVolume;
	for(int i = 0; i < bufferSize*nChannels; i++) {
		input[i] *= vol;
		float inp = ABS(input[i]);
		if(inp>max) max = inp;
	}
	if(max>AppSettings::micLevel) AppSettings::micLevel = max;
	else AppSettings::micLevel *= 0.95;
	
	if(currApp!=NULL) {
		currApp->audioReceived(input, bufferSize, nChannels);
	}

}
void testApp::audioRequested( float * output, int bufferSize, int nChannels ) {
	if(currApp!=NULL) {
		currApp->audioRequested(output, bufferSize, nChannels);
		float vol = AppSettings::outputVolume;
		for(int i = 0; i < bufferSize*nChannels; i++) {
			output[i] *= vol;
		}
	} else {
		for(int i = 0; i < bufferSize*nChannels; i++) {
			output[i] = 0;
		}
	}
}