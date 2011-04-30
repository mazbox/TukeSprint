/*
 *  Blob.cpp
 *  emptyExample
 *
 *  Created by Jean Baptiste Thiebaut on 27/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Blob.h"

blob::blob(int a, int b, float aacc, float bacc, int s, string sound,ofColor col)
{
	x = a;
	y= b;
	xacc = aacc;
	yacc = bacc;
	transparency = 180;
	size = s;
	
	color = col;

	beats.loadSound(sound,true);
	beats.setSpeed(pow(ABS(xacc)+ABS(yacc),0.3));
	//cout << beats.getSpeed() << endl;
	beats.setPan(x/ofGetWidth());
	beats.setVolume(180./255.);
	beats.play();
	

}

blob::~blob()
{
	beats.unloadSound();
	
}

void blob::update()
{
	x+=xacc;
	y+=yacc;
	
	transparency= MAX(transparency-2.,3);
	beats.setVolume(transparency/255.);
	beats.setPan(x/ofGetWidth());
	if (x>(ofGetWidth()-size/2) || x<size/2)
	{
		xacc=-xacc;
		beats.play();
		beats.setVolume(transparency/255.);
		beats.setPan((float)(x/ofGetWidth()));
	}
	if (y>(ofGetHeight()-size/2) || y<size/2)
	{
		yacc=-yacc;
		beats.play();
		beats.setVolume(transparency/255.);
		beats.setPan(x/ofGetWidth());
	}
		
	
	ofSetColor(color.r,color.g,color.b,transparency);
	ofFill();
	ofEllipse(x, y, size, size);
	
}

