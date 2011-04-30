/**
 * TestTApp.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#include "TukeApp.h"

class TestTApp: public TukeApp {
public:
	string getName() { return "Test T App!"; } 
	string getScreenshotFileName() { return "icons/test.png"; }
};
