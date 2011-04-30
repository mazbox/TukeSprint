/*
 *  Blob.h
 *  emptyExample
 *
 *  Created by Jean Baptiste Thiebaut on 27/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofMain.h"

class blob
{
public:
	blob(int x, int y, float xacc, float yacc, int s,string sound, ofColor col);
	~blob();
	
	float x,y,xacc,yacc;
	float transparency;
	int size;
	ofColor color;
	ofSoundPlayer beats;
	
	
	void update();

};