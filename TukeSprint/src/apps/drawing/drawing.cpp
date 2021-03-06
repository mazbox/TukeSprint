#include "drawing.h"

//--------------------------------------------------------------
void drawing::init(){
	selImage=NULL;
	AppSettings::addListener(this);
	current = -1;
	timeElapsed = ofGetElapsedTimeMillis();

	
	BLOB_SIZE = 15;
	//beats.loadSound("vib-ntch.st-B3-f.AIF");

	// OpenCV stuff
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
	
	 
	bLearnBakground = true;
	threshold = 40;
	
	
	LOWPASS = 25; //must be the same as the size of the array smoothPos

	ofxDirList DIR;
	DIR.allowExt("aif");
	DIR.allowExt("wav");
	DIR.allowExt("mp3");
	
	numFiles = DIR.listDir("./");
//	
	for(int i = 0; i < numFiles; i++){
		files.push_back( DIR.getPath(i));
		//printf("name is %s - path is %s \n", DIR.getName(i).c_str(), DIR.getPath(i).c_str() );
	}
		
	if (numFiles==0)
	{
		cout << "no sounds..." << endl;
	}

	blobColor.r = 116;
	blobColor.g= 80;
	blobColor.b =116;
	currentSound = (files[(int)ofRandom(0, files.size())]);	
	numFiles = files.size();

	start();
	
}
//--------------------------------------------------------------

void drawing::start()
{
	sample.loadSound("1.wav", false);
	sample.setVolume(1.);
	targetPos.x = ofGetWidth()/2;
	_y = targetPos.y = ofGetHeight()/2;
	_x = 100;	
	transparency = 0;
	explode = false;
	usingMouseInput=false;
	ofEnableAlphaBlending();
	for (int i=0;i<LOWPASS;i++)
	{
		smoothPos[i].x=_x;
		smoothPos[i].y=_y;
	}

	pulsatingCircle=35;
	colorChanged();

}
//--------------------------------------------------------------
void drawing::stop()
{
	sample.stop();
	for (int i=0;i<blobs.size();i++)
		blobs.erase(blobs.begin());
}
//--------------------------------------------------------------
void drawing::update(){
	// update the sound playing system:
	ofSoundUpdate();
	
	//OpenCV stuff
	if (!usingMouseInput)
	{
		
		colorImg.setFromPixels(video->getPixels(), 320,240);
		colorImg.mirror(false, !(AppSettings::mirrorCamera));
		grayImage = colorImg;

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, 320*240/3, 10, true);	// find holes
		
		if (contourFinder.nBlobs>0)
		{
			
			lastBlobPosition = blobPosition;
			float tmp_x, tmp_y;
			if (_x<ofGetWidth()/2)
				tmp_x = blobPosition.x = ofGetWidth()-contourFinder.blobs[0].boundingRect.x*ofGetWidth()/320;
			else
				tmp_x = blobPosition.x = ofGetWidth()-(contourFinder.blobs[0].boundingRect.x+contourFinder.blobs[0].boundingRect.width)*ofGetWidth()/320;
			
			tmp_y = blobPosition.y = contourFinder.blobs[0].boundingRect.y*ofGetHeight()/240;
				
			indexSmoothPos = (indexSmoothPos+1)%LOWPASS;
			
			smoothPos[indexSmoothPos].x = tmp_x;
			smoothPos[indexSmoothPos].y = tmp_y;

			for (int i = 0 ; i<LOWPASS ; i++)
			{
				tmp_x+=smoothPos[i].x;
				tmp_y+=smoothPos[i].y;
			}
			
			_x = tmp_x/LOWPASS;
			_y = tmp_y/LOWPASS;
		}
	}
	
	float hitCircleX,hitCircleY;
	if (selImage)
	{
		hitCircleX = 35+pulsatingCircle;
		hitCircleY = 35*selImage->getHeight()/selImage->getWidth()+pulsatingCircle;
	}
	else 
	{
		hitCircleX = pulsatingCircle + 35;
		hitCircleY = pulsatingCircle + 35;
	}
	float dist = ofDist(_x, _y, targetPos.x, targetPos.y);
	if (!explode && (dist<=hitCircleX || dist<=hitCircleY)) //Hit the target!
	{
		explode = true;
		explosionTimer = ofGetElapsedTimeMillis();
		
		transparency = 180;
		//change the sound randomly
		int i = (int)ofRandom(0,files.size());
		currentSound = files[i];
		sample.loadSound(currentSound, false);
		sample.play();
		for (int j = 0;j<35;j++)
			blobs.push_back(blob(targetPos.x,targetPos.y,ofRandom(-6,6),ofRandom(-4,4),BLOB_SIZE, currentSound, blobColor));

		blobColor.b = (int)ofRandom(40,205);
	}
	
	if (bLearnBakground == true){
		grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
	//	bLearnBakground = false;
	}
	
}

//--------------------------------------------------------------
void drawing::draw(){
	
	transparency = MAX(transparency-5,0);
	ofBackground(backgroundColor.r, backgroundColor.g, backgroundColor.b);
//	ofSetColor(blobColor.r,blobColor.g,blobColor.b,80);
	ofSetColor(blobColor.r, blobColor.g, blobColor.b,80);
	float variation = sin(ofGetElapsedTimeMillis()/5*PI/360);
	//draw the image
	if (selImage) {
		ofSetColor(255,255,255,255);
		selImage->setAnchorPercent(0.5, 0.5);
		selImage->draw(_x,_y,70,(selImage->getHeight()*70)/selImage->getWidth());
		selImage->setAnchorPercent(0, 0);
	} else //or draw the cross
	{
		ofRect(_x-30, _y-75, 30, 120);
		ofRect(_x-75, _y-30, 120, 30);
	}
	ofSetColor(crossColor.r, crossColor.g, crossColor.b,80);
//	ofSetColor(200, 200, 0, transparency);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	for (int i = 0 ; i<blobs.size() ; i++)
	{
		if (blobs[i].transparency <= 3)
		{
			blobs.erase(blobs.begin()+i);

		}
		else
			blobs[i].update();
	}

 
	if (!explode)
	{
		//ofSetColor(200, 200, 0,140);
		ofSetColor(crossColor.r, crossColor.g, crossColor.b,140);
		ofCircle(targetPos.x, targetPos.y, 35);
		//ofSetColor(200, 200, 0,80);
		ofSetColor(crossColor.r, crossColor.g, crossColor.b,80);
		pulsatingCircle=55+20*cos(ofGetElapsedTimeMillis()/5*PI/360);
		ofCircle(targetPos.x,targetPos.y,pulsatingCircle);
	}
	else
	{
		if (ofGetElapsedTimeMillis()>(explosionTimer+2500))
		{
			explode = false;
			targetPos.x  =(int)ofRandom(ofGetWidth()/4,ofGetWidth()*3/4);
			targetPos.y = (int)ofRandom(ofGetHeight()/4, ofGetHeight()*3/4);			
		}
		else //draw the explosion animation
		{
			float variation = (ofGetElapsedTimeMillis()-explosionTimer)/2500.;
			//ofSetColor(200, 200, 0,140);
			ofSetColor(crossColor.r, crossColor.g, crossColor.b,140);
			ofCircle(targetPos.x+variation*(ofGetWidth()-targetPos.x), targetPos.y, 15);
			ofCircle(targetPos.x+variation*(-targetPos.x), targetPos.y, 15);
			ofCircle(targetPos.x, targetPos.y+variation*(ofGetHeight()-targetPos.y), 15);
			ofCircle(targetPos.x, targetPos.y+variation*(-targetPos.y), 15);
			//ofSetColor(200, 200, 0,80);
			ofSetColor(crossColor.r, crossColor.g, crossColor.b,80);
			ofCircle(targetPos.x+variation*(ofGetWidth()-targetPos.x),targetPos.y,25+10*cos(ofGetElapsedTimeMillis()/5*PI/360));			
			ofCircle(targetPos.x+variation*(-targetPos.x),targetPos.y,25+10*cos(ofGetElapsedTimeMillis()/5*PI/360));			
			ofCircle(targetPos.x,targetPos.y+variation*(ofGetHeight()-targetPos.y),25+10*cos(ofGetElapsedTimeMillis()/5*PI/360));			
			ofCircle(targetPos.x,targetPos.y+variation*(-targetPos.y),25+10*cos(ofGetElapsedTimeMillis()/5*PI/360));			
		}
	}
}

//--------------------------------------------------------------
void drawing::keyPressed(int key){
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}

}

//--------------------------------------------------------------
void drawing::keyReleased(int key){

}

//--------------------------------------------------------------
void drawing::mouseMoved(int x, int y ){
	prevMouse = currMouse;
	currMouse = ofPoint(x,y);
}

//--------------------------------------------------------------
void drawing::mouseDragged(int x, int y, int button){
	prevMouse = currMouse;
	currMouse = ofPoint(x,y);
	
	indexSmoothPos = (indexSmoothPos+1)%LOWPASS;
	smoothPos[indexSmoothPos].x = x;
	smoothPos[indexSmoothPos].y = y;
	_x=x;
	_y=y;
	
	float hitCircleX,hitCircleY;
	if (selImage)
	{
		hitCircleX = 35+pulsatingCircle;
		hitCircleY = 35*selImage->getHeight()/selImage->getWidth()+pulsatingCircle;
	}
	else 
	{
		hitCircleX = pulsatingCircle + 35;
		hitCircleY = pulsatingCircle + 35;
	}		

	float dist = ofDist(_x, _y, targetPos.x, targetPos.y);

	if (dist<hitCircleX || dist<hitCircleY) //Hit the target!
	{
		//change the sound randomly
		int i = (int)ofRandom(0,files.size());
		currentSound = files[i];
		sample.loadSound(currentSound, false);
		sample.play();
		for (int j = 0;j<30;j++)
			blobs.push_back(blob(targetPos.x,targetPos.y,ofRandom(-4,4),ofRandom(-4,4),BLOB_SIZE, currentSound, blobColor));

		//change the position of the target
		targetPos.x  =(int)ofRandom(0, ofGetWidth());
		targetPos.y = (int)ofRandom(0, ofGetHeight());
		explode=true;
		
		blobColor.b = (int)ofRandom(0,255);
	}
}

//--------------------------------------------------------------
void drawing::mousePressed(int x, int y, int button){
	prevMouse = currMouse;
	currMouse = ofPoint(x,y);
	current++;
	usingMouseInput=true;
}


//--------------------------------------------------------------
void drawing::mouseReleased(int x, int y, int button){
	prevMouse = currMouse;
	currMouse = ofPoint(x,y);
	_x=x;
	_y=y;
	usingMouseInput=false;
}

//--------------------------------------------------------------
void drawing::windowResized(int w, int h){

}

