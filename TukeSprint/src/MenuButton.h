/*
 *  Button.h
 *
 *  Created by Marek Bereza on 16/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ImageCache.h"
#include "InteractiveObject.h"
#include "AppSettings.h"

class MenuButtonListener {
public:
	virtual void buttonPressed(string buttonName) = 0;
};

/**
 * Really simple button class
 */
class MenuButton: public InteractiveObject {
private:
	ofImage *upImg;
	ofImage *dnImg;

	
	
protected:
	int currTouchId;
	string name;
	MenuButtonListener *listener;
	bool down;
	bool over;
	float alpha;
	static ofTrueTypeFont *font;
public:
	
	
	
	void setup(string name, ofPoint centre, string upImgUrl, string dnImgUrl = "") ;
	
	
	void setAlpha(float alpha) ;
	bool touchOver(float xx, float yy) ;
	bool touchDown(float xx, float yy, int tid) ;
	
	bool touchMoved(float xx, float yy, int tid) ;
	
	virtual bool touchUp(float xx, float yy, int tid) ;
	
	void setListener(MenuButtonListener *listener) ;
		
	
	virtual void draw() ;
};
