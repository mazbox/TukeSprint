/**
 * HelpPane.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */

#include "ofxXmlGui.h"
#include "AppSettings.h"
#include "ColorScheme.h"

class HelpPane: public GuiListener {
public:
	void setup(ofVideoGrabber &video) {
		gui.setup(10, 10, 200);
		gui.addDrawable("camera", video);
		gui.addMeter("Mic level", AppSettings::micLevel);
		gui.addSlider("microphone volume", AppSettings::micVolume, 0, 1);
		colorSchemes.push_back(new ColorScheme("Warm"));
		colorSchemes.push_back(new ColorScheme("Cool"));
		colorSchemes.push_back(new ColorScheme("Multicoloured"));
		colorSchemes.push_back(new ColorScheme("Other"));
		colorSchemes.push_back(new ColorScheme("Another"));
		
		setColorScheme(0);
		
		for(int i = 0; i < colorSchemes.size(); i++) {
			gui.addToggle(colorSchemes[i]->name, colorSchemes[i]->enabled);
		}
		gui.addButton("back");
		gui.addListener(this);
	}
	
	
	void controlChanged(GuiControl *control) {
		for(int i = 0; i < colorSchemes.size(); i++) {
			if(colorSchemes[i]->name==control->name) {
				setColorScheme(i);
				return;
			}
		}
		
		if(control->name=="back") {
			disable();
		}
	}
	
	void setColorScheme(int scheme) {
		for(int i = 0; i < colorSchemes.size(); i++) {
			if(i==scheme) colorSchemes[i]->enabled = true;
			else colorSchemes[i]->enabled = false;
		}
	}
	vector<ColorScheme*> colorSchemes;
	void enable() {
		gui.enable();
	}
	void disable() {
		gui.disable();
	}
	
	bool isEnabled() {
		return gui.isEnabled();
	}
	ofxXmlGui gui;
};

