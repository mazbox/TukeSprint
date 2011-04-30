#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	mainMenu.init();
	currApp = NULL;
	showMainMenu();
	ofSoundStreamSetup(2, 1, this, 44100, 1024, 1);
}


void testApp::showMainMenu() {
	if(currApp!=NULL) {
		currApp->stop();
	} 
	currApp = &mainMenu;
	currApp->start();
}

void testApp::showHelp() {
	
}

//--------------------------------------------------------------
void testApp::update(){
	if(currApp!=NULL) {
		currApp->update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if(currApp!=NULL) {
		currApp->draw();
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
		}
		//printf("%c %c %c %c\n", keyQueue[0], keyQueue[1], keyQueue[2], keyQueue[3]);
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	if(currApp!=NULL) {
		currApp->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(currApp!=NULL) {
		currApp->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(currApp!=NULL) {
		currApp->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(currApp!=NULL) {
		currApp->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


void testApp::audioReceived( float * input, int bufferSize, int nChannels ) {
	if(currApp!=NULL) {
		currApp->audioReceived(input, bufferSize, nChannels);
	}

}
void testApp::audioRequested( float * output, int bufferSize, int nChannels ) {

}