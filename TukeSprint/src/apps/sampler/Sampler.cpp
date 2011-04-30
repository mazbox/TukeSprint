#include "Sampler.h"
#include "stdio.h"
#include "ofxDirList.h"
#include "AppSettings.h"


pthread_mutex_t         controlMutex = PTHREAD_MUTEX_INITIALIZER;


#define PENTATONIC "pentatonic"
#define MAJOR "major"
#define MINOR "minor"
#define CHROMATIC "chromatic"

ofImage Particle::particle;

ofColor *Particle::color;
//--------------------------------------------------------------
void Sampler::init(){	 
	
	ofBackground(0,0,0);	
	movementThreshold = 0.5;
	Particle::color = &AppSettings::color3;
	
	recordBufferSize = SAMPLERATE*MAX_RECORD_SECONDS;
	recordBuffer = new float[recordBufferSize];
	recordPos = 0;
	recording = false;
	inputLevel = 0;
	playbackSpeed = 1;
	lastSound = -1;
	
	
	ofxDirList DIR;
	
	int numFiles = DIR.listDir("./../sounds");
	for(int i = 0; i < numFiles; i++) {
		sounds.push_back(DIR.getName(i));
	}
	
	
	// this is the last time that there was a note
	noteLastTime = -10;
	lastNote = 0;
	
	
	scales.push_back(PENTATONIC);
	scales.push_back(MAJOR);
	scales.push_back(MINOR);
	scales.push_back(CHROMATIC);

	
	memset(recordBuffer, 0, recordBufferSize*sizeof(float));
	// 1 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 1 num buffers (latency)

	

		
	//--------- PANEL 1

	
	vision.setup();
}



int lastMaxLevel = -1;
void Sampler::update() {
	vision.video = video;
	// the vision code works out how much average change there is in each of
	// either vertical or horizontal strips across the screen.
	// this bit of code finds out if the strip with the most change
	// is a different strip from the last strip, and then triggers that note.
	float max = 0;
	int currMaxLevel = -1;
	for(int i = 0; i < vision.levels.size(); i++) {

		if(max<vision.levels[i]) {
			max = vision.levels[i];
			currMaxLevel = i;
		}
	}

	if(lastMaxLevel!=currMaxLevel) {
		//printf("Playing note %d %f\n", currMaxLevel, max);
		float volume = ofMap(max, 0, 0.5, 0, 1);
		if(volume>1) volume = 1;
		if(volume>movementThreshold) { // some threshold
			lastMaxLevel = currMaxLevel;
			playSound(volume, 1.f - (float)currMaxLevel/vision.levels.size());
		} else {
			currMaxLevel = -1;
		}
	}
	
	lastMaxLevel = currMaxLevel;

	
	int soundIndex = 0; // this is where it chooses the sound
	if(soundIndex!=lastSound) {
		printf("loading %s\n", sounds[soundIndex].c_str());
		string file = "./../sounds/" + sounds[soundIndex];
		pthread_mutex_lock(&controlMutex);
		sample.loadFromFile(file);
		pthread_mutex_unlock(&controlMutex);
	}
	lastSound = soundIndex;
	
	vision.update();
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update();
		if(!particles[i].alive) {
			particles.erase(particles.begin()+i);
			i--;
		}
	}
}

//--------------------------------------------------------------
void Sampler::draw(){
	ofFill();
	ofEnableAlphaBlending();
	vision.draw();

	// fade out a note
	if(ofGetElapsedTimef() - noteLastTime < 1.5f) {
		float alpha = 0.5*ofMap(ofGetElapsedTimef() - noteLastTime, 0, 1.5, 255, 0);
		
		//if(vision.vertical) {
		float height = (float) ofGetHeight()/vision.levels.size();
		//ofRect(0, height*lastNote, ofGetWidth(), height);
		
		glBegin(GL_QUAD_STRIP);
		ofSetColor(AppSettings::color1.r, AppSettings::color1.g, AppSettings::color1.b, alpha/2);
		glVertex2f(0, height*lastNote);
		glVertex2f(ofGetWidth(), height*lastNote);
		
		ofSetColor(AppSettings::color1.r, AppSettings::color1.g, AppSettings::color1.b, alpha);
		glVertex2f(0, height*lastNote+height/2);
		glVertex2f(ofGetWidth(), height*lastNote + height/2);
		
		ofSetColor(AppSettings::color1.r, AppSettings::color1.g, AppSettings::color1.b, alpha/2);
		glVertex2f(0, height*lastNote+height);
		glVertex2f(ofGetWidth(), height*lastNote + height);
		
		glEnd();
		
	}
	

	

	ofSetColor(0xFFFFFF);

	for(int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
	ofDisableAlphaBlending();
	//gui.draw();
}






void Sampler::audioRequested (float * output, int bufferSize, int nChannels) {
	// if we're recording, we want silence!!
	if(recording) {
		memset(output, 0, bufferSize*nChannels*sizeof(float));
	} else {
		// otherwise, maybe we want playback
		for(int i = 0; i < bufferSize; i++) {
			float s = sample.getSample(playbackSpeed);
			for(int channel = 0; channel < nChannels; channel++) {
				output[i*nChannels + channel] = s;
			}
		}
	}
}
//--------------------------------------------------------------
void Sampler::audioReceived 	(float * input, int bufferSize, int nChannels){	


	pthread_mutex_lock(&controlMutex);
	for(int i = 0; i < bufferSize; i++) {
		
		float inp = input[i*nChannels];
		
		// do the recording
		if(recording && recordPos<recordBufferSize) {
			recordBuffer[recordPos++] = inp;
		}
		// do a level meter
		if(inputLevel<inp) {
			inputLevel = inp;
		} else {
			inputLevel *= 0.99995;
		}
	}
	pthread_mutex_unlock(&controlMutex);

}


//--------------------------------------------------------------
void Sampler::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void Sampler::mouseDragged(int x, int y, int button){

}

void Sampler::playSound(float volume, float pitch) {
	int note = valueToNote(pitch);
	playbackSpeed = noteToSpeed(note);
	sample.trigger(volume);
	noteLastTime = ofGetElapsedTimef();
	lastNote = vision.levels.size() - pitch*vision.levels.size();
	
	// do some stuff here
	ofPoint pos, force;
	if(vision.getBiggestFlowPoint(&pos, &force)) {
		// correct the y pos
		float y = (float)((float)lastMaxLevel+0.5)/vision.levels.size();
		pos.y = y;
		spawnParticle(pos, volume);
	}
}

void Sampler::spawnParticle(ofPoint pos, float volume) {
	int star = 12;
	for(int i = 0; i < star; i++) {
		float angle = (float)i/star;
		angle *= 2*PI;
		particles.push_back(Particle(pos.x*ofGetWidth(), pos.y*ofGetHeight(), volume, angle));
	}
}

//--------------------------------------------------------------
void Sampler::mousePressed(int x, int y, int button){
	int note = valueToNote(1.f-((float)y/ofGetHeight()));
	playbackSpeed = noteToSpeed(note);
	sample.trigger(1);
	
}

//--------------------------------------------------------------
void Sampler::mouseReleased(int x, int y, int button){

}




float Sampler::noteToSpeed(int note) {
	return pow(2, (float)note/12.f);
}

int Sampler::valueToNote(float value) {

	int scale = 1;
	int notesInScale = 5;
	if(scales[scale]==PENTATONIC) {
		notesInScale = 5;
	} else if(scales[scale]==MAJOR) {
		notesInScale = 7;
	} else if(scales[scale]==MINOR) {
		notesInScale = 7;
	} else if(scales[scale]==CHROMATIC) {
		notesInScale = 12;
	}
	int maxOctaves = 2;
	
	// how many octaves we want
	value *= maxOctaves;
	
	// how many notes in the scale
	value *= notesInScale;
	
	// this is the position in the scale
	int noteInScale = floor(value);
	
	// this is the chromatic position
	int noteNum = 0;
	
	// work out the octave offset
	noteNum = floor(noteInScale/notesInScale)*12;
	
	// add the note offset
	
	
	
	if(scales[scale]==PENTATONIC) {
		switch(noteInScale%notesInScale) {
			case 0: noteNum += 0;  break;
			case 1: noteNum += 3;  break;
			case 2: noteNum += 5;  break;
			case 3: noteNum += 7;  break;
			case 4: noteNum += 10; break;
		}
	} else if(scales[scale]==MAJOR) {
		switch(noteInScale%notesInScale) {
			case 0: noteNum += 0;  break;
			case 1: noteNum += 2;  break;
			case 2: noteNum += 4;  break;
			case 3: noteNum += 5;  break;
			case 4: noteNum += 7; break;
			case 5: noteNum += 9;  break;
			case 6: noteNum += 11;  break;

		}
	} else if(scales[scale]==MINOR) {
		switch(noteInScale%notesInScale) {
			case 0: noteNum += 0;  break;
			case 1: noteNum += 2;  break;
			case 2: noteNum += 3;  break;
			case 3: noteNum += 5;  break;
			case 4: noteNum += 7;  break;
			case 5: noteNum += 8;  break;
			case 6: noteNum += 11; break;
		}
	} else if(scales[scale]==CHROMATIC) {
		noteNum += noteInScale%notesInScale;
	}
	
	
	return noteNum + 12; // set the pitch here
			
}

