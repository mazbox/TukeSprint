/*
 *  GuiIntSlider.h
 *
 *  Created by Mac Mini 1 on 09/03/2011.
 *
 */

#pragma once

#include "GuiControl.h"



class GuiIntSlider: public GuiControl {
public:
	
	bool vertical;
	float min;
	float max;
	int bgColor;
	int fgColor;
	bool stepped;
	bool showValue;
	
	string sliderBGUrl;
	string sliderFGUrl;
	string sliderHandleUrl;
	ofImage *sliderBG, *sliderFG, *sliderHandle;
	
	void setup() {
		vertical = false;
		height = 20;
		width = 100;
		stepped = false;
		value = new int[1];
		ival(value) = 0;
		min = 0;
		max = 255;
		bgColor = 0x505050;
		fgColor = 0x960000;
		sliderBGUrl = "";
		sliderFGUrl = "";
		sliderHandleUrl = "";
		showValue = false;
	}
	
	void load() {
		if(ival(value)<min) ival(value) = min;
		if(ival(value)>max) ival(value) = max;
		sliderBG = resources->getImage(sliderBGUrl);
		sliderFG = resources->getImage(sliderFGUrl);
		sliderHandle = resources->getImage(sliderHandleUrl);
		if(sliderBG!=NULL) {
			scalable = false;
			width = sliderBG->getWidth();
			height = sliderBG->getHeight();
		}
		
	}
	void draw() {
		
		if(sliderBG!=NULL) {
			ofSetHexColor(0xFFFFFF);
			sliderBG->draw(x, y);
		} else {
			ofSetHexColor(bgColor);
			ofRect(x, y, width, height);
		}
		
		float val = (ival(value)-min)/(max-min);
		
		if(sliderFG!=NULL) {
			ofSetHexColor(0xFFFFFF);
			ofPoint abs = getAbsoluteCoords();
			if(vertical) maskOn(abs.x, abs.y + height-height*val, width, height*val);
			else maskOn(abs.x, abs.y, width*val, height);
			sliderFG->draw(x, y);
			//ofRect(0, 0, ofGetWidth(), ofGetHeight());
			maskOff();
		} else {
			ofSetHexColor(fgColor);
			
			if(vertical) ofRect(x, y+height-height*val, width, height*val);
			else ofRect(x, y, width*val, height);
		}
		
		if(sliderHandle!=NULL) {
			ofSetHexColor(0xFFFFFF);
			if(vertical) {
				
				sliderHandle->draw(x, y + (1.f - val)*(height - sliderHandle->getHeight()));
			} else {
				sliderHandle->draw(x+val*(width-sliderHandle->getWidth()), y);
			}
		}
		ofSetColor(255, 255, 255);
		drawLabel();
	}
	
	void drawLabel() {
		if(showValue) {
			resources->getDefaultFont()->drawString(name + "   " + ofToString(ival(value)), x, y);
		} else {
			resources->getDefaultFont()->drawString(name, x, y);
		}
	}
	
	bool touchDown(int _x, int _y, int touchId) {
		
		
		if(vertical) {
			float val = 1 - (_y-y)/height;

			ival(value) = val*((float)max-min) + min;

			
		} else {
			float val = (_x-x)/width;

			ival(value) = val*(max-min) + min;
		}
		return true;
	}
	
	bool touchMoved(int _x, int _y, int touchId) {
		touchDown(_x, _y, touchId);
		return true;
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Vertical", "vertical", "toggle", &vertical));
		params.push_back(ParameterInfo("Min", "min", "intfield", &min));
		params.push_back(ParameterInfo("Max", "max", "intfield", &max));
		params.push_back(ParameterInfo("Stepped", "stepped", "toggle", &stepped));
		params.push_back(ParameterInfo("Value", "value", "intfield", value));
		params.push_back(ParameterInfo("Show Value", "showvalue", "toggle", &showValue));
		params.push_back(ParameterInfo("BG Color", "bgColor", "colorpicker", &bgColor));
		params.push_back(ParameterInfo("Slider Color", "fgColor", "colorpicker", &fgColor));
		
		params.push_back(ParameterInfo("Slider BG", "sliderBGUrl", "file", &sliderBGUrl));
		params.push_back(ParameterInfo("Slider FG", "sliderFGUrl", "file", &sliderFGUrl));
		params.push_back(ParameterInfo("Slider Handle", "sliderHandleUrl", "file", &sliderHandleUrl));
		
		return params;
	}
	
	string valueToString() {
		return ofToString(ival(value));
	}
	
	void valueFromString(string inp) {
		ival(value) = atoi(inp.c_str());
	}
};