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
#include "ofxCvColorImage.h"

class HelpPane: public GuiListener {
public:
	ofTrueTypeFont font;
	ofTrueTypeFont bigFont;
	ofImage bg;
	
	ofImage logo;
	void setup(ofxCvColorImage &video) {
		bg.loadImage("resources/guiBackground.png");
		logo.loadImage("resources/sam.png");
		logo.setAnchorPercent(0, 1);
		bigFont.loadFont("resources/Arial Bold.ttf", 32);
		font.loadFont("resources/Arial.ttf", 19);
		soundId = 0;
		imageId = 0;
		imagePreview.loadImage("resources/no-image.png");
		gui.setup(0,0, 200);
		int col1 = 45;
		gui.addTitle("Settings", bigFont, 0)->position(col1, 85);
	
		gui.addTitle("Video", font, 0)->position(col1, 160);
		gui.addDrawable("camera", video)->position(col1, 200)->size(200, 150);
		gui.addToggle("mirror camera", AppSettings::mirrorCamera, "resources/mirror.png", "resources/mirrorOn.png", "resources/mirrorDown.png")->position(col1, 360);
		
		
		gui.addTitle("Volume", font, 0)->position(col1, 410);
		gui.addSlider("volume", AppSettings::outputVolume, 0, 1)->position(col1, 430);
		
		gui.addTitle("Microphone", font, 0)->position(col1, 500);
		gui.addSlider("microphone volume", AppSettings::micVolume, 0, 1.5)->position(col1, 526);
		gui.addMeter("Mic level", AppSettings::micLevel)->position(col1, 557);
		
		int col2 = 320;
		
		gui.addTitle("Colour", font, 0)->position(col2, 160);
		colorSchemes.push_back(new ColorScheme("scheme0", "resources/colourSchemes/Palette1.png", 0x6a0100, 0xc09d4d, 0xfbe7c0));
		colorSchemes.push_back(new ColorScheme("scheme1", "resources/colourSchemes/Palette2.png", 0xe90000, 0x11d552, 0xaaffc3));
		colorSchemes.push_back(new ColorScheme("scheme2", "resources/colourSchemes/Palette3.png", 0x34d1c4, 0xb3d9b0, 0x4a453c));
		colorSchemes.push_back(new ColorScheme("scheme3", "resources/colourSchemes/Palette4.png", 0xf2071b, 0x106973, 0xf2cc0c));
		colorSchemes.push_back(new ColorScheme("scheme4", "resources/colourSchemes/Palette5.png", 0xff00ba, 0x00eaff, 0x00ff0c));
		colorSchemes.push_back(new ColorScheme("scheme5", "resources/colourSchemes/Palette6.png", 0x000000, 0x999999, 0xffffff));
		colorSchemes.push_back(new ColorScheme("scheme6", "resources/colourSchemes/Palette7.png", 0x000000, 0x00ff33, 0xff00ff));

		
		
		
		for(int i = 0; i < colorSchemes.size(); i++) {
			float xx = col2 + (i%2)*110;
			float yy = 200 + (i/2)*100;
			GuiControl *c = gui.addToggle(colorSchemes[i]->name, colorSchemes[i]->thumbnailUrl, colorSchemes[i]->enabled)->position(xx, yy);
			colorSchemes[i]->control = c;
		}
		
		setColorScheme(AppSettings::colorScheme);
		
		
		
		listFilesIntoVector("./../images", imageFiles);
		listFilesIntoVector("./../sounds", soundFiles);
		int col3 = 580;
		int col4 = col3 + 100;
		gui.addTitle("Custom Picture", font, 0)->position(col3, 160);
		gui.addImage("img", imagePreview)->position(col3, 200);
		gui.addList("image", imageId, imageFiles)->position(col4, 200)->size(284, 167);

		
		gui.addTitle("Custom Sound", font, 0)->position(col3, 450);
		gui.addButton("play sound", "resources/play.png", "resources/playDown.png")->position(col3, 480);
		gui.addList("sound", soundId, soundFiles)->position(col4, 480)->size(284, 167);

		
		gui.addButton("back", "resources/back.png", "resources/backDown.png")->position(778, 690);
		
		gui.addListener(this);
	}
	
	void listFilesIntoVector(string dir, vector<string> &dest) {
		dest.push_back("None (default)");
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
				for(int j = 0; j < AppSettings::listeners.size(); j++) {
					AppSettings::listeners[j]->colorChanged();
				}
				return;
			}
		}
		
		if(control->name=="back") {
			disable();
		} else if(control->name=="image") {
			if(imageId==0) {
				AppSettings::image = NULL;
				imagePreview.loadImage("resources/no-image.png");
			} else {
				AppSettings::image = &imagePreview;
				imagePreview.loadImage(string("./../images/")+imageFiles[imageId]);
			}
			for(int i = 0; i < AppSettings::listeners.size(); i++) {
				AppSettings::listeners[i]->imageChanged();
			}
		} else if(control->name=="sound") {
			if(soundId==0) {
				AppSettings::soundFile = "";
			} else {
				soundPreview.loadSound(string("./../sounds/")+soundFiles[soundId]);
				AppSettings::soundFile = string("./../sounds/")+soundFiles[soundId];
			}
			
			for(int i = 0; i < AppSettings::listeners.size(); i++) {
				AppSettings::listeners[i]->soundChanged();
			}
		} else if(control->name=="play sound") {
			soundPreview.play();
		}
	}
	
#ifndef hexValR
#define hexValR(A)  ((A >> 16) & 0xff)
#define hexValG(A) ((A >> 8) & 0xff)
#define hexValB(A) ((A >> 0) & 0xff)
#endif
	
	
	ofColor ofColorFromHex(int hex) {
		ofColor c;
		c.r = hexValR(hex);
		c.g = hexValG(hex);
		c.b = hexValB(hex);
		return c;
	}

	void setColorScheme(int scheme) {
		
		AppSettings::colorScheme = scheme;
		
		AppSettings::color1 = ofColorFromHex(colorSchemes[scheme]->color1);
		AppSettings::color2 = ofColorFromHex(colorSchemes[scheme]->color2);
		AppSettings::color3 = ofColorFromHex(colorSchemes[scheme]->color3);
		
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
		
		ofFill();
		// draw background
		ofSetColor(255, 255, 255);
		
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		bg.draw(0, 0);
		logo.draw(0, 768);
		// draw the highlight
		ofRectangle rect(colorSchemes[AppSettings::colorScheme]->control->x,
		colorSchemes[AppSettings::colorScheme]->control->y,
		colorSchemes[AppSettings::colorScheme]->control->width,
		colorSchemes[AppSettings::colorScheme]->control->height);
		rect.x -= 5;
		rect.y -= 5;
		rect.width += 10;
		rect.height += 10;
		ofSetColor(255, 0, 0);
		ofRect(rect.x, rect.y, rect.width, rect.height);
		ofNoFill();
		ofSetColor(0,0,0);
		ofRect(0, 0, 1024, 768);
		ofFill();
		
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

