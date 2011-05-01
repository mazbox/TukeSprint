/**
 * MainMenu.cpp
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */

#include "MainMenu.h"
#include "SimpleButton.h"
#include "TestTApp.h"
#include "testApp.h"
#include "Sampler.h"
#include "drawing.h"
#include "fluid001.h"

void MainMenu::init() {

	apps.push_back(new TestTApp());
	apps.push_back(new Sampler());
	apps.push_back(new drawing());
	apps.push_back(new fluid001("Fluid 1", "icons/test.png", 119, 0.00028, 0.00006, 0.003, 15, 3, 0));
	apps.push_back(new fluid001("Fluid 2", "icons/test.png", 182, 0.00017, 0.00011, 0.006, 14, 0, 0));
	
	int itemsPerRow = 3;
	int PADDING = 10;
	for(int i = 0; i < apps.size(); i++) {
		apps[i]->init();
		
		SimpleButton *appButton = new SimpleButton();


		appButton->setup(apps[i]->getName(), ofPoint(), apps[i]->getScreenshotFileName());
		appButton->setListener(this);
		
		items.push_back(appButton);
		
		int row = i%itemsPerRow;
		int col = i/itemsPerRow;
		
		items[i]->x = PADDING + row*(items[i]->width + PADDING);
		items[i]->y = PADDING + col*(items[i]->height + PADDING);
		
	}
	
	
	
}

void MainMenu::draw() {
	for(int i = 0; i < items.size(); i++) {
		items[i]->draw();
	}
}

/** mouse events */
void MainMenu::mouseMoved(int x, int y ) { 
	for(int i = 0; i < items.size(); i++) {
		items[i]->touchOver(x, y);
	}
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