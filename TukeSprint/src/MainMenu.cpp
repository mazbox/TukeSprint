/**
 * MainMenu.cpp
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */

#include "MainMenu.h"
#include "MenuButton.h"
#include "TestTApp.h"
#include "testApp.h"
#include "Sampler.h"
#include "drawing.h"
#include "fluid001.h"
#include "ImageObject.h"

void MainMenu::init() {

	apps.push_back(new TestTApp());
	apps.push_back(new Sampler());
	apps.push_back(new drawing());
	apps.push_back(new fluid001());
	apps.push_back(new TestTApp());
	apps.push_back(new Sampler());
	apps.push_back(new drawing());
	apps.push_back(new fluid001());

	
	int itemsPerRow = 3;
	int ITEM_PADDING = 20;
	int SCREEN_PADDING = 50;
	float buttonWidth = ((float)(ofGetWidth() - 2*ITEM_PADDING - 2*SCREEN_PADDING))/3.f;
	float buttonHeight = ((float)(ofGetHeight() - 2*ITEM_PADDING - 2*SCREEN_PADDING))/3.f;
	for(int i = 0; i < apps.size(); i++) {
		apps[i]->init();
		
		MenuButton *appButton = new MenuButton();


		appButton->setup(apps[i]->getName(), ofPoint(), apps[i]->getScreenshotFileName());
		appButton->setListener(this);
		
		items.push_back(appButton);
		
		int row = i%itemsPerRow;
		int col = i/itemsPerRow;
		
		items[i]->width = buttonWidth;
		items[i]->height = buttonHeight;
		
		items[i]->x = SCREEN_PADDING + row*(items[i]->width + ITEM_PADDING);
		items[i]->y = SCREEN_PADDING + col*(items[i]->height + ITEM_PADDING);
	}
	items.push_back(new ImageObject("resources/instructions.png"));
	items.back()->width = buttonWidth;
	items.back()->height = buttonHeight;
	items.back()->x = SCREEN_PADDING + 2*(items.back()->width + ITEM_PADDING);
	items.back()->y = SCREEN_PADDING + 2*(items.back()->height + ITEM_PADDING);
	boxAlpha = 0;
	rolledOverItem = NULL;
	rollingOverItem = false;
}

void MainMenu::draw() {
	ofBackground(255, 255, 255);
	for(int i = 0; i < items.size(); i++) {
		items[i]->draw();
	}
	/*
	////////////////////////////////
	// The following code dims out
	// the rest of the screen except
	// for the currently rolled over
	// item. Not ideal for touch
	// screens so I'm leaving it out.
	if(rollingOverItem) {
		boxAlpha+= 0.05;
		if(boxAlpha>1) boxAlpha = 1;
	} else {
		boxAlpha -= 0.05;
		if(boxAlpha<0) {
			boxAlpha = 0;
			rolledOverItem = NULL;
		}
	}
	
	if(rolledOverItem!=NULL) {
		ofSetColor(0, 0, 0, 100*boxAlpha);
		ofRect(0, 0, ofGetWidth(), rolledOverItem->y);
		ofRect(0, rolledOverItem->y, rolledOverItem->x, rolledOverItem->height);
		ofRect(rolledOverItem->x+rolledOverItem->width, rolledOverItem->y, ofGetWidth() - (rolledOverItem->x+rolledOverItem->width), rolledOverItem->height);
		ofRect(0, rolledOverItem->y+rolledOverItem->height, ofGetWidth(), ofGetHeight() - (rolledOverItem->y+rolledOverItem->height));
	}
	*/
}

void MainMenu::drawBoxAroundItem(InteractiveObject *item) {
	if(item==NULL) {
		rollingOverItem = false;
	} else {
		rollingOverItem = true;
		rolledOverItem = item;
	}
	
}
/** mouse events */
void MainMenu::mouseMoved(int x, int y ) { 
	for(int i = 0; i < items.size(); i++) {
		items[i]->touchOver(x, y);
	}
	
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->inside(x, y)) {
			drawBoxAroundItem(items[i]);
			return;
		}
	}
	drawBoxAroundItem(NULL);
	
	
}
void MainMenu::mouseDragged(int x, int y, int button) { 
	for(int i = 0; i < items.size(); i++) {
		items[i]->touchMoved(x, y, button);
	}
}
void MainMenu::mousePressed(int x, int y, int button) { 
	for(int i = 0; i < items.size(); i++) {
		items[i]->touchDown(x, y, button);
	}
}
void MainMenu::mouseReleased(int x, int y, int button) {
	for(int i = 0; i < items.size(); i++) {
		items[i]->touchUp(x, y, button);
	}
}




void MainMenu::buttonPressed(string name) {
	for(int i = 0; i < apps.size(); i++) {
		if(apps[i]->getName()==name) {
			testApp::getInstance()->launchTukeApp(apps[i]);
		}
	}
}