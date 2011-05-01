#include "baroque.h"


//--------------------------------------------------------------
void baroque::init(){

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
	
	//lets load in our face xml file
 	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");
	
	//randomiseAllPoints();
	
	//now lets load in the XML if it exists
//	cout << "loading faces.xml"<< endl;
	
	//we load our settings file
	if( XML.loadFile("faces.xml") ){
//		cout << "faces.xml loaded!"<< endl;
	}else{
		cerr << "unable to load faces.xml check data/ folder"<< endl;
	}
	
	//we initalize some of our variables
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
	
	int numIDTags = XML.getNumTags("COMPUTERID");
//	cout << "Number of ID tags: " << numIDTags << endl;
	
	if(numIDTags == 1)
	{
		std::ostringstream startOss;
		startOss << XML.getValue("COMPUTERID", "NONE");
		computerID = startOss.str();
//		cout << "Computer ID " << computerID << endl;
	}else{
		cerr << "No ID  found!" << endl;
	}	
	
	int numSaveTags = XML.getNumTags("SAVELOCATION");
	cout << "Number of save tags: " << numSaveTags << endl;
	
	if(numSaveTags == 1)
	{
		std::ostringstream startOss;
		startOss << XML.getValue("SAVELOCATION", "NONE");
		saveLocation = startOss.str();
//		cout << "Save location " << saveLocation << endl;
	}else{
		cerr << "No save location found!" << endl;
	}
	
	//lets see how many <FACE> </FACE> tags there are in the xml file
	int numFaceTags = XML.getNumTags("FACELOCATION");
	
//	cout << "Number of faces: " << numFaceTags << endl;
	
	//resize the vector to the number of strokes we have from the XML file
	
	faces.resize(numFaceTags);
	
	for(int i = 0; i < numFaceTags; i++){
		std::ostringstream faceXMLLocationOSS;
		faceXMLLocationOSS << XML.getValue("FACELOCATION", "faces/strokesArchitecture.xml", i);
		string faceXMLLocation = faceXMLLocationOSS.str();
		faces[i].XMLLocation = faceXMLLocation;
//		cout << "Face" << i << " of " << numFaceTags << ", is at " << faceXMLLocation << endl;
	}
	
	for(int i = 0; i < numFaceTags; i++)
	{
		XML.clear();
		
		//now for all the faces, load in all their strokes
		//now lets load in the XML if it exists
		cout << "loading: " << faces[i].XMLLocation << endl;
		
		//we load our settings file
		if( XML.loadFile(faces[i].XMLLocation) ){
//			cout << faces[i].XMLLocation << "loaded!"<< endl;
		}else{
			cerr << "unable to load " << faces[i].XMLLocation << "check data/ folder"<< endl;
		}
		
		//we initalize some of our variables
		lastTagNumber	= 0;
		pointCount		= 0;
		lineCount		= 0;
		
		//lets see how many <STROKE> </STROKE> tags there are in the xml file
		int numStrokeTags = XML.getNumTags("STROKE");
		
//		cout << "Number of strokes: " << numStrokeTags << ", in " << faces[i].XMLLocation << endl;
		
		//resize the vector to the number of strokes we have from the XML file
		
		faces[i].strokes.resize(numStrokeTags);
		
		//if there is at least one <STROKE> tag we can read the list of points
		//and then try and draw it as a line on the screen
		for(int j = 0; j < numStrokeTags; j++){
			//we push into the ith STROKE tag
			//this temporarirly treats the tag as
			//the document root.
			XML.pushTag("STROKE", j);
			
			int numStartTags = XML.getNumTags("STARTIMAGE");
			
//			cout << "Number start tags " << numStartTags << endl;
			
			std::ostringstream startOss;
			startOss << XML.getValue("STARTIMAGE", "elements/Elements_Architecture/A_1.png");
			string startImageLocation = startOss.str();
			
//			cout << "Start image location " << startImageLocation << endl;
			
			int numMiddleTags = XML.getNumTags("MIDDLEIMAGE");
			
//			cout << "Number middle tags " << numMiddleTags << endl;
			
			std::ostringstream middleOss;
			middleOss << XML.getValue("MIDDLEIMAGE", "elements/Elements_Architecture/A_1.png");
			string middleImageLocation = middleOss.str();
			
//			cout << "Middle image location " << middleImageLocation << endl;
			
			int numEndTags = XML.getNumTags("ENDIMAGE");
			
//			cout << "Number end tags " << numEndTags << endl;
			
			std::ostringstream endOss;
			endOss << XML.getValue("ENDIMAGE", "elements/Elements_Architecture/A_1.png");
			string endImageLocation = endOss.str();
			
//			cout << "End image location " << endImageLocation << endl;
			
			//load in all the images
			faces[i].strokes[j].startImageLocation = startImageLocation;
			faces[i].strokes[j].startImage.loadImage(startImageLocation);
			faces[i].strokes[j].startImage.setAnchorPercent(0.5f,0.5f);
			faces[i].strokes[j].middleImageLocation = middleImageLocation;
			faces[i].strokes[j].middleImage.loadImage(middleImageLocation);
			faces[i].strokes[j].middleImage.setAnchorPercent(0.5f,0.5f);
			faces[i].strokes[j].endImageLocation = endImageLocation;
			faces[i].strokes[j].endImage.loadImage(endImageLocation);
			faces[i].strokes[j].endImage.setAnchorPercent(0.5f,0.5f);	
			
			//now mirror the images and save them to the right places
			//was going to do it with opencv, just added to ofimage very naughty
			//http://www.openframeworks.cc/forum/viewtopic.php?f=6&t=1716&view=unread#unread
			
			faces[i].strokes[j].startImageMirrored.loadImage(startImageLocation);
			faces[i].strokes[j].startImageMirrored.mirror(true, false);
			faces[i].strokes[j].startImageMirrored.setAnchorPercent(0.5f,0.5f);
			faces[i].strokes[j].middleImageMirrored.loadImage(middleImageLocation);
			faces[i].strokes[j].middleImageMirrored.mirror(true, false);
			faces[i].strokes[j].middleImageMirrored.setAnchorPercent(0.5f,0.5f);
			faces[i].strokes[j].endImageMirrored.loadImage(endImageLocation);
			faces[i].strokes[j].endImageMirrored.mirror(true, false);
			faces[i].strokes[j].endImageMirrored.setAnchorPercent(0.5f,0.5f);			
			
			faces[i].strokes[j].points.resize(0);
			
			//we see how many points we have stored in <PT> tags
			int numPtTags = XML.getNumTags("PT");			
			
			for(int k = 0; k < numPtTags; k++){
				
				//We then read those x y values into our
				//vector - so that we can then draw the points as
				//a line on the screen
				
				//the last argument of getValue can be used to specify
				//which tag out of multiple tags you are refering to.
				int x = XML.getValue("PT:X", 0, k);
				int y = XML.getValue("PT:Y", 0, k);
				
				faces[i].strokes[j].points.push_back(ofPoint());
				int last = faces[i].strokes[j].points.size()-1;
				
				faces[i].strokes[j].points[last].x = x;
				faces[i].strokes[j].points[last].y = y;
			}
			
			//this pops us out of the STROKE tag
			//sets the root back to the xml document
			XML.popTag();
		}			
	}
	
	strokeDrawList.resize(NUM_STROKES_IN_FACE);
	
	// do an initial random selection of strokes here
	
	makeRandomMask();
	
	drawCentre.x = ofGetWidth()/2.f;
	drawCentre.y = ofGetHeight()/2.f;
	
	faceFound = false;
	
	faceTimeElapsed = 0.f;
	timeFirstFace = 0.f;
	gracePeriod = 0.1f; // one tenth of a second is the graceperiod
	graceActivated = 0.f;
	howMuchGrace = 0.f;
	currentTime = 0.f;
	oldNumFaces = 0;
	growTime = 0.8f;	//one second to grow a face
	
	pleaseSaveBackground = false; //save the background this frame
	runFlash = false; //should flash be decreasing now?
	flashPeriod = 0.2f;//duration of a flash
	flashTimeRemaining = 0.f; //flash time remaining	
	
	deltaFromLastFrame = 0.f;
	lastFrameTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void baroque::update(){
	ofBackground(100,100,100);
	
	currentTime  = ofGetElapsedTimef();
	howMuchGrace = currentTime-graceActivated;
	deltaFromLastFrame = currentTime - lastFrameTime;	
    
	colorImg.setFromPixels(video->getPixels(), CAM_WIDTH,CAM_HEIGHT);

	grayImage = colorImg;
	if (bLearnBakground == true){
		grayBg = grayImage;		
		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
		bLearnBakground = false;
	}
	
	haarFinder.findHaarObjects(grayImage, 10, 99999999, 10);
	
	// take the abs value of the difference between background and incoming and then threshold:
	grayDiff.absDiff(grayBg, grayImage);
	grayDiff.threshold(threshold);
	
	// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
	// also, find holes is set to true so we will get interior contours as well....
	contourFinder.findContours(grayDiff, 20, (CAM_WIDTH*CAM_HEIGHT)/3, 10, true);	// find holes
	
	
	int numFace = haarFinder.blobs.size();
	
	if(numFace > 0)
	{
		graceActivated = 0.f; // if we have faces graceactivated should be 0.f, so we can work out
								// the transition to graceperiod next time
		if(faceFound) //already found a face on the previous frame, or in graceperiod
		{
			//just let the timer keep going
			faceTimeElapsed = currentTime - timeFirstFace;
		}else{
			//start the timer;
			timeFirstFace = currentTime;
			faceTimeElapsed = 0.f;
			faceFound = true; //then we have some faces, so set faceFound to true
		}
	}else
	{
		if(faceFound){ //so we had a face last frame, but not this one, or we are in grace
			//just let the timer keep going
			faceTimeElapsed = currentTime - timeFirstFace;
			
			if(howMuchGrace == currentTime){
				//then we need to activate grace on this update, as we know graceactivated is 0 from 
				//a pre
				graceActivated = currentTime;
				//facefound stays true			
			}else{
				if(howMuchGrace > gracePeriod){ // run out of grace
					faceFound = false;
					faceTimeElapsed = 0.f; // zero this immediately, not on next frame
				}else{
					//stay in the graceperiod, do nothing
				}
			}
		}else{
			faceTimeElapsed = 0.f;
			graceActivated = 0.f;
		}
	}
	
	if(runFlash){
		flashTimeRemaining -= deltaFromLastFrame;
		
		if(flashTimeRemaining < 0.f)
		{
			runFlash = false;
		}
		
	}
	
	lastFrameTime = ofGetElapsedTimef();
}

void baroque::draw(){
	ofSetColor(0xffffff);
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CORNER); //make sure!	
	
	colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());

	int numFace = haarFinder.blobs.size(); // as we know it's sorted in order of biggest, only draw 1
	
	if(numFace > 0){
		drawMasks(false); //draw faces with live data
	}else{
		//no faces found! , do grace drawing here, bust the big draw loop into a call with a bool
		// to set if it should be drawn with what alpha
		
		if(faceFound){
			//we are still in graceperiod, so draw with the old data
			drawMasks(true);
		}else{
			makeRandomMask();			
		}

	}
	
	// finally, a report:
	
//	ofSetColor(0xffffff);
//	char reportStr[1024];
//	sprintf(reportStr, "SPACE to save image, press 'b' to capture bg\nthreshold %i (press: +/-)", threshold);
//	ofDrawBitmapString(reportStr, 20, 600);
//	
//	ofFill();
//	ofSetColor(0x000000);
//	ofRect(60, 630, 400,100);
	
	//some text to explain whats what
	
//	std::ostringstream localOSS;
//	string strokeDrawOrderInfo;
//	int currentStrokeToDraw = strokeDrawList.back();
//	
//	localOSS << "Selected:" << currentStrokeToDraw 
//	<< ", Start " << strokes[currentStrokeToDraw].startImageLocation
//	<< "\n Middle " << strokes[currentStrokeToDraw].middleImageLocation
//	<< "\n End " << strokes[currentStrokeToDraw].endImageLocation;
//	
//	strokeDrawOrderInfo = localOSS.str();
//	
//	ofDrawBitmapString(strokeDrawOrderInfo, 75, 670);
	
//	string faceTimeElapsedStr = "FaceTime:" + ofToString(faceTimeElapsed)+ "HowMuchGraceTime:" + ofToString(howMuchGrace);
//	ofSetColor(0xff0000);
//	ofDrawBitmapString(faceTimeElapsedStr, 75, 760);
}

//--------------------------------------------------------------
void baroque::keyPressed  (int key){

	switch (key){
		case ' ':
		{
			pleaseSaveBackground = true;
			break;				
		}
		
		case 'b':{
			bLearnBakground = true;
			break;			
		}
		case '+':{
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;			
		}
		case '-':{
			threshold --;
			if (threshold < 0) threshold = 0;
			break;			
		}
		case 'f':{
			ofToggleFullscreen();
			break;				
		}
		default:{
			break;
		}
	}
}

//--------------------------------------------------------------
void baroque::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void baroque::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void baroque::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void baroque::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void baroque::resized(int w, int h){

}

void baroque::drawCross(float centreX, float centreY, float crossWidth, float crossHeight){
	ofSetColor(0xFF0000);
	
	ofPoint left, right, top, bottom;
	
	left.x = centreX - crossWidth/2.f;
	left.y = centreY;
	
	right.x = centreX + crossWidth/2.f;
	right.y = centreY;
	
	top.x = centreX;
	top.y = centreY - crossHeight/2.f;
	
	bottom.x = centreX;
	bottom.y = centreY + crossHeight/2.f;
	
	ofLine(left.x, left.y, right.x, right.y);
	ofLine(top.x, top.y, bottom.x, bottom.y);	
}

void baroque::saveBackground()
{
	if(saveLocation != "NONE" && computerID != "NONE"){
		ofImage screen;
		screen.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
		screen.grabScreen(0,0,ofGetWidth(), ofGetHeight());
		
		std::ostringstream stampOSS;
		stampOSS << ofGetYear() << "_" << ofGetMonth() << "_" << ofGetDay() << 	"_" 
			<< ofGetHours() << "_" << ofGetMinutes() << "_" << ofGetSeconds() << "_" << ofGetFrameNum();
		
		std::ostringstream finalOSS;
		finalOSS << saveLocation << computerID << stampOSS.str() << ".jpg";
		string completeSaveLocation = finalOSS.str();
		
		//cout << "Trying to save a grab to: " << completeSaveLocation << endl;
		
		ofDisableDataPath();
		screen.saveImage(completeSaveLocation);
		ofEnableDataPath();	
		
		pleaseSaveBackground = false; //save the background this frame
		runFlash = true; //should flash be decreasing now?
		flashTimeRemaining = flashPeriod; //flash time remaining		
	}else{
		cerr << "Invalid save path or path not loaded or invalid computer ID" << endl;
	}		
}

void baroque::makeRandomMask()
{
	int totalNumberOfStrokesLoaded = 0;
	
	for(int i=0; i<faces.size(); i++){
		totalNumberOfStrokesLoaded += faces[i].strokes.size();
	}
	
	for(int i=0; i< strokeDrawList.size(); i++)
	{
		int strokeIndexToUse = (int)ofRandom(0, totalNumberOfStrokesLoaded);
		
		strokeDrawList[i] = strokeIndexToUse;
		//cout << "Stroke selected:" << strokeIndexToUse << endl;
	}
	
	//cout << "Random mask made " << endl;
}

int baroque::getRelativeFaceIndex(int absoluteStrokeIndex){
	int indexCounter = 0;
	
	for(int i=0; i < faces.size(); i++)
	{
		for(int j=0; j < faces[i].strokes.size(); j++){
			if(indexCounter == absoluteStrokeIndex){
				return i;
			}
			indexCounter++;
		}
	}	
	
	return 0;
}
int baroque::getRelativeStrokeIndex(int absoluteStrokeIndex)
{
	int indexCounter = 0;

	for(int i=0; i < faces.size(); i++)
	{
		for(int j=0; j < faces[i].strokes.size(); j++){
			if(indexCounter == absoluteStrokeIndex){
				return j;
			}
			indexCounter++;
		}
	}	
	
	return 0;
}

void baroque::drawFlash(){
	//draw the flash
	
	float alphaValue = (flashTimeRemaining/flashPeriod)*255.f;
	ofEnableAlphaBlending();
	
	//cout << "Alpha value:"<< alphaValue << endl;
	
	ofSetColor(255,255,255, (int) alphaValue);
	ofFill();
	ofRect(0,0,ofGetWidth(), ofGetHeight());
	ofNoFill();
	
	ofDisableAlphaBlending();
	
}

void baroque::drawMasks(bool useOldData){
	
	if(!useOldData){
		//copy in the livedata to the old vector for convenience
		int numFace = haarFinder.blobs.size(); // as we know it's sorted in order of biggest, only draw 1
		
		if(numFace > 0){
			lastGoodTrack.resize(numFace);
			
			for(int i = 0; i < numFace; i++){
				//take a copy, if we lose the face on the next update cycle, 
				//so we can draw it in grace, fade out, etc
				lastGoodTrack[i].rect.x = haarFinder.blobs[i].boundingRect.x;
				lastGoodTrack[i].rect.y = haarFinder.blobs[i].boundingRect.y;
				lastGoodTrack[i].rect.width = haarFinder.blobs[i].boundingRect.width;
				lastGoodTrack[i].rect.height = haarFinder.blobs[i].boundingRect.height;
				lastGoodTrack[i].centroid.x = haarFinder.blobs[i].centroid.x;
				lastGoodTrack[i].centroid.y = haarFinder.blobs[i].centroid.y;
			}
			
			oldNumFaces = numFace;
		}else{
			//there must be faces!
			cerr << "draw masks called with useOldData set to false, but there are no faces! Error!" << endl;
		}
	}
	
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	
	//do the alpha if we are in grace
	if(useOldData){
		int graceAlpha = (int)min((howMuchGrace/ gracePeriod)*255.f, 255.f);
		
		ofSetColor(255,255,255,0.1f);
	}
	
	for(int i = 0; i < oldNumFaces; i++){
		float x = lastGoodTrack[i].rect.x;
		float y = lastGoodTrack[i].rect.y;
		float w = lastGoodTrack[i].rect.width;
		float h = lastGoodTrack[i].rect.height;
		
		//cout << "x,y :: width, height : " << x << ", " << y << "::" << w << ", " << h << endl;
		
		float cx = lastGoodTrack[i].centroid.x;
		float cy = lastGoodTrack[i].centroid.y;
		
		float scaleXRatio = (w/CAM_WIDTH)/ ORIG_FACE_SIDE;
		scaleXRatio *= ofGetWidth();
		
		float scaleYRatio = (h/CAM_HEIGHT)/ ORIG_FACE_SIDE;
		scaleYRatio *= ofGetHeight();
		
		float blobX = x/CAM_WIDTH;
		blobX *= ofGetWidth();
		
		float blobY = y/CAM_HEIGHT;
		blobY *= ofGetHeight();
		
		float blobW = w/CAM_WIDTH;
		blobW *= ofGetWidth();
		
		float blobH = h/CAM_HEIGHT;
		blobH *= ofGetHeight();
		
		float blobCX = cx/CAM_WIDTH;
		blobCX *= ofGetWidth();
		
		float blobCY = cy/CAM_HEIGHT;
		blobCY *= ofGetHeight();
		
		float percentageOfStrokeToDraw = faceTimeElapsed/growTime; //we need to take account of grace,
																	//but just get it working for now
		percentageOfStrokeToDraw = min(percentageOfStrokeToDraw, 1.f);
		
		for(int j=0; j < strokeDrawList.size(); j++)
		{
			int absoluteStrokeIndex = strokeDrawList[j];
			int relativeFaceIndex = getRelativeFaceIndex(absoluteStrokeIndex);
			int relativeStrokeIndex = getRelativeStrokeIndex(absoluteStrokeIndex);
			
			int numPts = faces[relativeFaceIndex].strokes[relativeStrokeIndex].points.size();
			
			numPts = (int)((float)numPts * percentageOfStrokeToDraw);
			
			//catmull curves create nice smooth curves between points
			//so actually a lower resolution creates a smoother line
			//and reduce the number of points in our eps file.
			int rescaleRes = 6;
			
			float angleOfRotation = 0.f;
			
			for(int k = 0; k < numPts; k++){
				//taking account of offset now, and scale
				int x = blobCX + faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].x * scaleXRatio; 				
				int y = blobCY + faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].y * scaleYRatio;				
				
				if(k != numPts -1){
					//then it's not the last point, so the maths below will work
					ofPoint p0 = 
					ofPoint(faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].x, 
							faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].y);
					ofPoint p1 = 
					ofPoint(faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k+1].x,
							faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k+1].y);
					
					float adjacent = p1.x - p0.x;
					float opposite = p1.y - p0.y;
					
					angleOfRotation = atan(opposite/adjacent);
				}
				
				ofPushMatrix();
				ofTranslate(x,y,0);
				ofRotateZ(ofRadToDeg(angleOfRotation));
				// draw the image
				ofSetColor(0xFFFFFF); // for safety
				if(k==0){
					// start by drawing the first image
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImage.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImage.height * scaleYRatio;
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImage.draw(0, 0, scaledWidth, scaledHeight);
				}else if(k == (numPts -1)){
					// then its the end image
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImage.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImage.height * scaleYRatio;					
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImage.draw(0, 0, scaledWidth, scaledHeight);
				}else{
					//otherwise draw the middle
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImage.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImage.height * scaleYRatio;					
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImage.draw(0, 0, scaledWidth, scaledHeight);
				}
				
				
				ofPopMatrix();
				
				// now draw on the opposite side
				//y remains the same, x changes
				
				//offset and scale and mirror
				float mirroredX = blobCX - (faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].x * scaleXRatio); 
				
				if(k != numPts -1){
					//then it's not the last point, so the maths below will work
					
					float mirroredNextX = blobCX - (faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k+1].x * scaleXRatio);
					
					ofPoint p0 = ofPoint(mirroredX, faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k].y);
					ofPoint p1 = ofPoint(mirroredNextX, faces[relativeFaceIndex].strokes[relativeStrokeIndex].points[k+1].y);
					
					float adjacent = p1.x - p0.x;
					float opposite = p1.y - p0.y;
					
					angleOfRotation = atan(opposite/adjacent);
				}
				
				ofPushMatrix();
				ofTranslate(mirroredX,y,0);
				ofRotateZ(ofRadToDeg(angleOfRotation));
				// draw the image
				if(k==0){
					// start by drawing the first image
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImageMirrored.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImageMirrored.height * scaleYRatio;					
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].startImageMirrored.draw(0, 0, scaledWidth, scaledHeight);
				}else if(k == (numPts -1)){
					// then its the end image
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImageMirrored.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImageMirrored.height * scaleYRatio;
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].endImageMirrored.draw(0, 0, scaledWidth, scaledHeight);
				}else{
					//otherwise draw the middle
					float scaledWidth = faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImageMirrored.width * scaleXRatio;
					float scaledHeight = faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImageMirrored.height * scaleYRatio;						
					faces[relativeFaceIndex].strokes[relativeStrokeIndex].middleImageMirrored.draw(0, 0, scaledWidth, scaledHeight);
				}
				
				ofPopMatrix();				
			}
		}		
		
		//			ofSetColor(0xFF0000);
		//			ofRect(blobX, blobY, blobW, blobH);
		//			
		//			ofSetColor(0xFFFFFF);
		//			ofDrawBitmapString("face "+ofToString(i), blobCX, blobCY);
		//			drawCross(blobCX, blobCY, 200.f, 200.f);
	}	
	ofDisableAlphaBlending();	
	
	if(pleaseSaveBackground)
		saveBackground();
	
	if(runFlash)
		drawFlash();
}