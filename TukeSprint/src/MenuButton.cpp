/**
 * MenuButton.cpp
 * TukeSprint
 *
 * Created by Marek Bereza on 01/05/2011.
 *
 */


#include "MenuButton.h"

ofTrueTypeFont *MenuButton::font = NULL;


	

void MenuButton::setup(string name, ofPoint centre, string upImgUrl, string dnImgUrl) {
	if(font==NULL) {
		font = new ofTrueTypeFont();
		font->loadFont("resources/Arial Bold.ttf", 14);
	}
	centre.x = (int)centre.x;
	centre.y = (int)centre.y;
	listener = NULL;
	down = false;
	over = false;
	this->name = name;
	currTouchId = -1;
	alpha = 1;
	upImg = ImageCache::getImage(upImgUrl);
	
	if(dnImgUrl=="") {
		dnImg = NULL;
	} else {
		dnImg = ImageCache::getImage(dnImgUrl);
	}
	
	
	setFromCenter(ofPoint(centre.x, centre.y), upImg->getWidth(), upImg->getHeight());
	
}


void MenuButton::setAlpha(float alpha) {
	this->alpha = alpha;
}
bool MenuButton::touchOver(float xx, float yy) {
	if(inside(xx, yy)) {
		over = true;
		return true;
	} else {
		over = false;
		return false;
	}
}
bool MenuButton::touchDown(float xx, float yy, int tid) {
	
	if(inside(xx, yy)) {
		currTouchId = tid;
		down = true;
	} else {
		down = false;
		over = false;
	}
	
	return down;
}

bool MenuButton::touchMoved(float xx, float yy, int tid) {
	if(inside(xx, yy) && tid==currTouchId) {
		down = true;
		over = true;
	} else {
		down = false;
		over = false;
	}
	return down;
}

bool MenuButton::touchUp(float xx, float yy, int tid) {
	
	if(currTouchId==tid) {
		currTouchId = -1;
		down = false;
		over = false;
		if(inside(xx, yy)) {
			if(listener!=NULL) listener->buttonPressed(name);
		}
		
	}
	return down;
	
}

void MenuButton::setListener(MenuButtonListener *listener) {
	this->listener = listener;
}


void MenuButton::draw() {
	x = (int)x;
	y = (int)y;
	ofFill();
	if(over) {
		int BORDER = 8;
		ofSetColor(255, 0, 0);
		ofRect(x-BORDER, y-BORDER, width+BORDER*2, height+BORDER*2);
		
	}
	ofSetColor(255, 255, 255, alpha*255.f);
	if(down) {
		if(dnImg!=NULL) {
			dnImg->draw(x, y, width, height);
		} else {
			ofSetColor(0xFFCCCC);
			upImg->draw(x, y, width, height);
		}
	} else {
		upImg->draw(x, y, width, height);
	}
	
	ofSetColor(0, 182, 252);
	int LABEL_HEIGHT = 40;
	ofRect(x, y+height-LABEL_HEIGHT, width, LABEL_HEIGHT);
	ofSetColor(255, 255, 255);
	
	
	float textX = x + (width - font->stringWidth(name))/2;
	textX = (int)textX;
	float textY = y+height-16;
	font->drawString(name, textX, textY);
	// black outline
	if(over) {
		ofSetColor(0, 0, 0);
		ofNoFill();
		ofRect(x, y, width, height);
		ofFill();
	}
}
