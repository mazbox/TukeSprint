#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include <deque.h>
#include "MainMenu.h"
#include "TukeApp.h"
#include "HelpPane.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void audioReceived( float * input, int bufferSize, int nChannels );
	void audioRequested( float * output, int bufferSize, int nChannels );
	
	static testApp *getInstance() { return instance; }
	
	void launchTukeApp(TukeApp *app);
	
	
private:
	HelpPane help;
	ofVideoGrabber video;
	static testApp *instance;
	// for storing the last 4 keys pressed.
	deque<int> keyQueue;
	// looks for if you keyed 'menu' or 'help'
	void checkKeyPresses();
	
	void showMainMenu();
	
	void showHelp();
	
	MainMenu mainMenu;
	
	TukeApp *currApp;
};

#endif
