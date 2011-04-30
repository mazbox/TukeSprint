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

void MainMenu::init() {
	
	apps.push_back(new TestTApp());
	apps.push_back(new TestTApp());
	
	for(int i = 0; i < apps.size(); i++) {
		apps[i]->init();
		
		SimpleButton *appButton = new SimpleButton();
		appButton->setup(apps[i]->getName(), ofPoint(), apps[i]->getScreenshotFileName());
		items.push_back(appButton);
		
	}
	
}

void MainMenu::draw() {

}