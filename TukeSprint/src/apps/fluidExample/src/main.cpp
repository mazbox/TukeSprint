
#include "testApp.h"

testApp *myApp;
int main( ){
	//ofSetupOpenGL(1440, 900, OF_FULLSCREEN);
	ofSetupOpenGL(1024, 700, OF_WINDOW);// <-------- setup the GL context
	myApp = new testApp;
	ofRunApp(myApp);
}
