/*
 *  customDrawer.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "guiCustomImpl.h"
#include "guiBaseObject.h" //theo added for isInsideRect()

class customDrawer : public guiCustomImpl {
	
private:
	
	vector < vector <ofPoint> > pts;
	
public:
	
	bool mouseIsRelative;
	ofImage colorwheel;
	ofColor col;
	
	int markerX, markerY;
	
	customDrawer(){
		colorwheel.loadImage("colorwheel.png");

	}
	
	// -------------------------------------
	void saveSettings(string filename){
	}
	
	void reloadSettings(){
	
	}
	
	void saveSettings(){
	
	}
	
	void loadSettings(string filename) {
	
	}
	
	// -------------------------------------
	float getWidth() {
		return boundingRect.width;
	}
	float getHeight() {
		return boundingRect.height;
	}
	
	// -------------------------------------
	void clearPoints() {
	
	}
	
	// -------------------------------------
	void addPoint(float x, float y) {

	}
	
	void draw(float x, float y, float w, float h) {
		
		colorwheel.draw(x, y);
		ofNoFill();
		ofSetColor(0, 0, 0);
		ofRect(x+markerX, y+markerY, 5, 5);
		ofFill();
		ofSetColor(col.r, col.g, col.b);
		ofRect(x+markerX, y+markerY, 5, 5);

	}
	
	// -------------------------------------
	void mousePressed(float x, float y) {

		if( isInsideRect(x, y, ofRectangle(0, 0, boundingRect.width, boundingRect.height) ) ){
			//cout << x << " " << y << endl;
			changeColor(x, y);	

		}
	}

	void changeColor(float x, float y){
		
		
		unsigned char* pixels = colorwheel.getPixels();
		int pos = (((int)y * colorwheel.width) + (int)x)*4;
		col.r = pixels[pos];
		col.g = pixels[pos+1];
		col.b = pixels[pos+2];
		
		markerX = (int)x;
		markerY = (int)y;

	}
	
	//these values come in with 0,0 being the top left of your bounding rect 
	// -------------------------------------
	void mouseDragged(float x, float y){

		//theo changed
		
		if( isInsideRect(x, y, ofRectangle(0, 0, boundingRect.width, boundingRect.height) ) ){
			changeColor(x, y); 
		}
	}
	
	void mouseReleased(float x, float y){
		if( isInsideRect(x, y, ofRectangle(0, 0, boundingRect.width, boundingRect.height) ) ){
			changeColor(x, y); 
		}
	}
	
};