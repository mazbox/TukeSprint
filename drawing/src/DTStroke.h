/*
 *  DTStroke.h
 *  emptyExample
 *
 *  Created by Jean Baptiste Thiebaut on 27/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class DTStroke
{
	
public:
	DTStroke();
	
	
	vector<ofPoint> point;    
	vector<float> width;
	vector<ofColor> color;
	
	void addPoint(ofPoint pt,double w, ofColor c);
};