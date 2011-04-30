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
#include "ofxDirList.h"

class HelpPane: public GuiListener {
public:
	void setup(ofVideoGrabber &video) {
		soundId = 0;
		imageId = 0;
		imagePreview.loadImage("resources/no-image.png");
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
		
		listFilesIntoVector("./../images", imageFiles);
		listFilesIntoVector("./../sounds", soundFiles);
		gui.addList("image", imageId, imageFiles);
		gui.addImage("img", imagePreview);
		gui.addList("sound", soundId, soundFiles);
		gui.addButton("play sound");
		
		gui.addButton("back");
		
		gui.addListener(this);
	}
	
	void listFilesIntoVector(string dir, vector<string> &dest) {
		ofxDirList DIR;
		if(dir.find("images")!=-1) {
			DIR.allowExt("jpeg");
			DIR.allowExt("jpg");
			DIR.allowExt("png");
			DIR.allowExt("gif");
			DIR.allowExt("JPEG");
			DIR.allowExt("JPG");
			DIR.allowExt("PNG");
			DIR.allowExt("GIF");
			
		} else {
			DIR.allowExt("WAV");
			DIR.allowExt("wav");
		}
		int numEntries = DIR.listDir(dir);
		for(int i = 0; i < numEntries; i++) {
			dest.push_back(DIR.getName(i));
		}
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
		} else if(control->name=="image") {
			imagePreview.loadImage(string("./../images/")+imageFiles[imageId]);
		} else if(control->name=="sound") {
			soundPreview.loadSound(string("./../sounds/")+soundFiles[soundId]);
		} else if(control->name=="play sound") {
			soundPreview.play();
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
	
	vector<string> imageFiles;
	vector<string> soundFiles;
	int imageId;
	int soundId;
	ofImage imagePreview;
	ofSoundPlayer soundPreview;
};

