#include "drawing.h"
#include "AppSettings.h"

//--------------------------------------------------------------
void drawing::init(){
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
	
	
	LOWPASS = 45; //must be the same as the size of the array smoothPos

	ofxDirList DIR;
	DIR.allowExt("aif");
	DIR.allowExt("wav");
	DIR.allowExt("mp3");
	
	numFiles = DIR.listDir("./");
//	
	for(int i = 0; i < numFiles; i++){
		files.push_back( DIR.getPath(i));
		printf("name is %s - path is %s \n", DIR.getName(i).c_str(), DIR.getPath(i).c_str() );
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

	//blobColor = AppSettings::color1;
//	crossColor = AppSettings::color2;
//	backgroundColor =AppSettings::color3;

}
//--------------------------------------------------------------
void drawing::update(){
	// update the sound playing system:
	ofSoundUpdate();
	
	//OpenCV stuff
	if (!usingMouseInput)
	{
		
		colorImg.setFromPixels(video->getPixels(), 320,240);
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

	if (!explode && ofDist(_x, _y, targetPos.x, targetPos.y)<80) //Hit the target!
	{
		explode = true;
		explosionTimer = ofGetElapsedTimeMillis();
		
		transparency = 180;
		
		sample.play();
//		if(sample.getIsPlaying()==true){
//			sample.
//		}
		
		//change the sound randomly
		int i = (int)ofRandom(0,files.size());
		currentSound = files[i];
		//cout <<numFiles<< " " << i << " sound changed to:" << currentSound << endl;
		//change the position of the target
		blobColor.b = (int)ofRandom(40,205);
	}
	if (!usingMouseInput)
	{
		float time = (float)(ofGetElapsedTimeMillis() - timeElapsed);
		timeElapsed = ofGetElapsedTimeMillis();
		
		ofPoint lastPos = smoothPos[(indexSmoothPos+LOWPASS+1)%LOWPASS];
		float dist  = ofDist(_x, _y, lastPos.x,lastPos.y);
		
		float speed = powf(dist/time,0.3);
		float newwidth = MAX(0.5,1/speed);

		if (speed > 0.1 && speed < 1.5)
		{
			float xacc = ofSign(blobPosition.x-lastBlobPosition.x)*(pow(abs((int)(_x-lastBlobPosition.x)),0.3));
			float yacc = ofSign(blobPosition.y-lastBlobPosition.y)*(pow(abs((int)(_y-lastBlobPosition.y)),0.3));
		//	cout << "xacc, yacc " << xacc << " " << yacc << endl;
			blobs.push_back(blob(_x,_y,xacc,yacc,BLOB_SIZE+(int)(10/(speed)), currentSound, blobColor));
		}
	
	}
	
	
	if (bLearnBakground == true){
		grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
	//	bLearnBakground = false;
	}
	
}

//--------------------------------------------------------------
void drawing::draw(){
	
	transparency = MAX(transparency-5,0);
	ofBackground(AppSettings::color3.r, AppSettings::color3.g, AppSettings::color3.b);
//	ofSetColor(blobColor.r,blobColor.g,blobColor.b,80);
	ofSetColor(AppSettings::color1.r, AppSettings::color1.g, AppSettings::color1.b,80);
	float variation = sin(ofGetElapsedTimeMillis()/5*PI/360);
	//draw the cross
	ofRect(_x-30, _y-75, 30, 120);
	ofRect(_x-75, _y-30, 120, 30);

	ofSetColor(AppSettings::color2.r, AppSettings::color2.g, AppSettings::color2.b,80);
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
		ofSetColor(AppSettings::color2.r, AppSettings::color2.g, AppSettings::color2.b,140);
		ofCircle(targetPos.x, targetPos.y, 35);
		//ofSetColor(200, 200, 0,80);
		ofSetColor(AppSettings::color2.r, AppSettings::color2.g, AppSettings::color2.b,80);
		ofCircle(targetPos.x,targetPos.y,55+20*cos(ofGetElapsedTimeMillis()/5*PI/360));
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
			ofSetColor(AppSettings::color2.r, AppSettings::color2.g, AppSettings::color2.b,140);
			ofCircle(targetPos.x+variation*(ofGetWidth()-targetPos.x), targetPos.y, 15);
			ofCircle(targetPos.x+variation*(-targetPos.x), targetPos.y, 15);
			ofCircle(targetPos.x, targetPos.y+variation*(ofGetHeight()-targetPos.y), 15);
			ofCircle(targetPos.x, targetPos.y+variation*(-targetPos.y), 15);
			//ofSetColor(200, 200, 0,80);
			ofSetColor(AppSettings::color2.r, AppSettings::color2.g, AppSettings::color2.b,80);
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
	
	float time = (float)(ofGetElapsedTimeMillis() - timeElapsed);
	timeElapsed = ofGetElapsedTimeMillis();
	float dist = ofDist(currMouse.x, currMouse.y, prevMouse.x, prevMouse.y);
	
	float speed = dist/time;
	
	//cout << "speed " << speed << endl;
	float newwidth = MAX(0.5,1/speed);
	
	indexSmoothPos = (indexSmoothPos+1)%LOWPASS;
	smoothPos[indexSmoothPos].x = x;
	smoothPos[indexSmoothPos].y = y;
	_x=x;
	_y=y;
	
	
	if (ofDist(x, y, targetPos.x, targetPos.y)<80) //Hit the target!
	{
		//change the sound randomly
		int i = (int)ofRandom(0,files.size());
		currentSound = files[i];
		sample.loadSound(currentSound, true);
		//change the position of the target
		targetPos.x  =(int)ofRandom(0,ofGetWidth());
		targetPos.y = (int)ofRandom(0, ofGetHeight());
		explode=true;
	//	blobColor.b = (int)ofRandom(0,255);
	}
	if (speed > 0.1 && speed<1.5)
	{
		float xacc = ofSign(x-prevMouse.x)*(pow(abs((int)(_x-lastBlobPosition.x)),0.3));
		float yacc = ofSign(y-prevMouse.y)*(pow(abs((int)(_y-lastBlobPosition.y)),0.3));
		
		blobs.push_back(blob(x,y,xacc,yacc,BLOB_SIZE, currentSound,AppSettings::color1));
		
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

