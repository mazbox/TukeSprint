/**
 * TukeApp.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */
#pragma once

class TukeApp {
public:
	/** 
	 * Return a name that will be used for the title on the main page
	 */
	virtual string getName() = 0;
	
	/** The name of the screenshot */
	virtual string getScreenshotFileName() = 0;
	
	ofVideoGrabber *video;
	
	
	/**
	 * this gets called right at the beginning of the program.
	 */
	virtual void init() {}
	
	
	/**
	 * this gets called when this individual TukeApp is started.
	 */
	virtual void start() {}
	
	/**
	 * Gets called when the user goes back to the main menu.
	 */
	virtual void stop() {}
	
	
	virtual void update() {}
	virtual void draw() {}
	
	/** mouse events */
	virtual void mouseMoved(int x, int y ) {}
	virtual void mouseDragged(int x, int y, int button) {}
	virtual void mousePressed(int x, int y, int button) {}
	virtual void mouseReleased(int x, int y, int button) {}
	
	/** audio stuff */
	virtual void audioReceived( float * input, int bufferSize, int nChannels ){}
	virtual void audioRequested( float * output, int bufferSize, int nChannels ){}
	
	
};