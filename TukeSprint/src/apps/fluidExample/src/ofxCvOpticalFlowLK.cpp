//
// ofxCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#include "ofxCvOpticalFlowLK.h"

ofxCvOpticalFlowLK::ofxCvOpticalFlowLK(void)
{
	captureWidth = DEFAULT_CAPTURE_WIDTH;
	captureHeight = DEFAULT_CAPTURE_HEIGHT;

	captureColsStep = DEFAULT_CAPTURE_COLS_STEP;
	captureRowsStep = DEFAULT_CAPTURE_ROWS_STEP;
}

ofxCvOpticalFlowLK::~ofxCvOpticalFlowLK(void)
{
	cvReleaseImage(&vel_x);
	cvReleaseImage(&vel_y);
}
	
void ofxCvOpticalFlowLK::allocate(int _w, int _h){
	captureWidth = _w;
	captureHeight = _h;

	vel_x = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	vel_y = cvCreateImage( cvSize( captureWidth ,captureHeight ), IPL_DEPTH_32F, 1  );
	
    cvSetZero(vel_x);
    cvSetZero(vel_y);
}

void ofxCvOpticalFlowLK::setCalcStep(int _cols, int _rows){
	captureColsStep = _cols;
	captureRowsStep = _rows;
}

void ofxCvOpticalFlowLK::calc( ofxCvGrayscaleImage & pastImage, ofxCvGrayscaleImage & currentImage, int size) {
	cvCalcOpticalFlowLK( pastImage.getCvImage(), currentImage.getCvImage(),	cvSize( size, size), vel_x, vel_y );

}

void ofxCvOpticalFlowLK::filter(int flowBlur) {
	cvSmooth(vel_x, vel_x, CV_BLUR , flowBlur*2+1);
	cvSmooth(vel_y, vel_y, CV_BLUR , flowBlur*2+1);
}

void ofxCvOpticalFlowLK::draw(float x,float y,float w, float h) {

	glLineWidth(1);
	ofSetColor(0xffffff);
	ofNoFill();
	float speed;

	int xx, yy, dx, dy;
	float multW = w/captureWidth;
	float multH = h/captureHeight;
	glPushMatrix();
	{
		glTranslatef(x, y, 0);
		glScalef(multW, multH, 1);
		for ( yy = 0; yy < captureHeight; yy+=captureRowsStep ){
			for ( xx = 0; xx < captureWidth; xx+=captureColsStep ){

				dx = (int)cvGetReal2D( vel_x, yy, xx );
				dy = (int)cvGetReal2D( vel_y, yy, xx );
				//speed = dx * dx + dy * dy;
				ofLine(xx, yy, xx+dx * 2, yy+dy * 2);

			}
		}
	}
	glPopMatrix();
}
