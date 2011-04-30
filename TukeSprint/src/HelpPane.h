/**
 * HelpPane.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */

#include "ofxXmlGui.h"
#include "AppSettings.h"

class HelpPane {
public:
	void setup(ofVideoGrabber &video) {
		gui.setup(10, 10, 200);
		gui.addDrawable("camera", video);
		gui.addMeter("Mic level", AppSettings::micLevel);
		gui.addSlider("microphone volume", AppSettings::micVolume, 0, 1);
	}
	void enable() {
		gui.enable();
	}
	void disable() {
		gui.disable();
	}
	
	ofxXmlGui gui;
};