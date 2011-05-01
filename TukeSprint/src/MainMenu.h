/**
 * MainMenu.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#pragma once
#include "ofMain.h"
#include "TukeApp.h"
#include "MenuButton.h"

class MainMenu: public TukeApp, public MenuButtonListener {
public:
	void init();
	
	void draw();

	vector<InteractiveObject*> items;
	vector<TukeApp*> apps;
	
	/** mouse events */
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void buttonPressed(string name);
	
	string getName() { return ""; }
	string getScreenshotFileName() { return ""; }
	
	InteractiveObject *rolledOverItem;
	float boxAlpha;
	bool rollingOverItem;
	void drawBoxAroundItem(InteractiveObject *item);
};