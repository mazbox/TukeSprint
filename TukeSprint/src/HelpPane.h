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
	ofTrueTypeFont font;
	ofTrueTypeFont bigFont;
	
	void setup(ofVideoGrabber &video) {
		bigFont.loadFont("resources/Arial Bold.ttf", 32);
		font.loadFont("resources/Arial Bold.ttf", 22);
		soundId = 0;
		imageId = 0;
		imagePreview.loadImage("resources/no-image.png");
		gui.setup(10, 10, 200);

		gui.addTitle("Settings", bigFont, 0)->position(45, 85);
		
		gui.addTitle("Video", font, 0)->position(45, 160);
		gui.addDrawable("camera", video)->position(45, 195);
		
		gui.addTitle("Volume", font, 0)->position(45, 370);
		gui.addSlider("volume", AppSettings::outputVolume, 0, 1)->position(45, 390);
		
		gui.addTitle("Microphone", font, 0)->position(45, 480);
		gui.addSlider("microphone volume", AppSettings::micVolume, 0, 1)->position(45, 506);
		gui.addMeter("Mic level", AppSettings::micLevel)->position(45, 537);
		
		
		gui.addTitle("Colour", font, 0)->position(272, 160);
		colorSchemes.push_back(new ColorScheme("Warm"));
		colorSchemes.push_back(new ColorScheme("Cool"));
		colorSchemes.push_back(new ColorScheme("Multicoloured"));
		colorSchemes.push_back(new ColorScheme("Other"));
		colorSchemes.push_back(new ColorScheme("Another"));
		
		setColorScheme(0);
		/*
		for(int i = 0; i < colorSchemes.size(); i++) {
			gui.addToggle(colorSchemes[i]->name, colorSchemes[i]->enabled);
		}
		
		
		
		
		
		listFilesIntoVector("./../images", imageFiles);
		listFilesIntoVector("./../sounds", soundFiles);
		
		gui.addTitle("Custom Picture", font, 0);
		gui.addImage("img", imagePreview);
		gui.addList("image", imageId, imageFiles);

		
		gui.addTitle("Custom Sound", font, 0);
		gui.addButton("play sound");
		gui.addList("sound", soundId, soundFiles);

		
		gui.addButton("back");
		*/
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
	
	void draw() {
		
		// draw background
		ofSetColor(255, 255, 255, 220);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		
		
		
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

