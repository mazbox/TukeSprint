/**
 * ImageObject.h
 * TukeSprint
 *
 * Created by Marek Bereza on 01/05/2011.
 *
 */
#include "InteractiveObject.h"
#pragma once
class ImageObject: public InteractiveObject {
public:
	ofImage img;
	ImageObject(string imgUrl) {
		img.loadImage(imgUrl);
	}
	void draw() {
		ofSetColor(255, 255, 255);
		img.draw(x, y, width, height);
	}
};