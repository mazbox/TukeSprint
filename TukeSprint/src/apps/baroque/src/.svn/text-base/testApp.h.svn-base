#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

#include "ofxCvHaarFinder.h"
#include "ofxXmlSettings.h"
#include "ofImageAdv.h"

#define NUM_PTS 800
#define NUM_LINES 10
#define CAM_WIDTH 320
#define CAM_HEIGHT 240
#define ORIG_FACE_SIDE 525
#define NUM_STROKES_IN_FACE 9

struct Stroke{
	string startImageLocation;
	string middleImageLocation;
	string endImageLocation;
	ofImage startImage;
	ofImage middleImage;
	ofImage endImage;
	ofImageAdv startImageMirrored;
	ofImageAdv middleImageMirrored;
	ofImageAdv endImageMirrored; // mirror them on construction for speed
	vector <ofPoint> points;
};

struct Face{ // a face has several strokes in it, 10 usually
	string XMLLocation;
	vector <Stroke> strokes;
};

struct OldTrackBlob { // for remembering the last frame with a face in it's information
	ofRectangle rect;
	ofPoint centroid;
};

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);

private:
	void drawCross(float centreX, float centreY, float crossWidth, float crossHeight);
	void saveBackground(); //saves the whole screen to a specified location, saveLocation with date stamp
	void makeRandomMask(); //populates the stroke draw list with a random series of strokes from different
							// faces
	int getRelativeFaceIndex(int absoluteStrokeIndex);
	int getRelativeStrokeIndex(int absoluteStrokeIndex);
	void drawFlash(); // flash with alpha so people know they have taken a shot
	void drawMasks(bool useOldData);	//draw the mask(s), using live data or, the old info
private:
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
#else
	ofVideoPlayer 		vidPlayer;
#endif

	ofxCvColorImage		colorImg;

	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;

	ofxCvContourFinder 	contourFinder;
	ofxCvHaarFinder			haarFinder;

	int 				threshold;
	bool				bLearnBakground;

	ofxXmlSettings XML;
	ofTrueTypeFont TTF;

	string xmlStructure;
	string message;

	int pointCount;
	int lineCount;
	int lastTagNumber;

	vector <Face> faces; //the faces

	vector <int> strokeDrawList; // a vector of ints all relating to the stroke index in question

	ofPoint drawCentre;

	string saveLocation;
	string computerID;

	float timeFirstFace; //moment in milliseconds from the start of the app that a face was detected continuously
	float faceTimeElapsed; //since a face was found
	bool faceFound; //was a face found this frame?
	float gracePeriod; //how long you can go without loosing a face in seconds
	float graceActivated; //time since grace activation
	float howMuchGrace; //how much grace has gone
	float currentTime; //the current time

	vector <OldTrackBlob> lastGoodTrack;
	int oldNumFaces;

	float growTime; //time taken to grow a face

	bool pleaseSaveBackground; //save the background this frame
	bool runFlash; //should flash be decreasing now?
	float flashPeriod;//duration of a flash
	float flashTimeRemaining; //flash time remaining
	float lastFrameTime;
	float deltaFromLastFrame;
};

#endif
