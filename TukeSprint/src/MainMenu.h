/**
 * MainMenu.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#include "ofMain.h"
#include "TukeApp.h"
#include "InteractiveObject.h"

class MainMenu: public TukeApp {
public:
	void init();
	
	void draw();

	vector<InteractiveObject*> items;
	vector<TukeApp*> apps;
	
	
	
	string getName() { return ""; }
	string getScreenshotFileName() { return ""; }
};