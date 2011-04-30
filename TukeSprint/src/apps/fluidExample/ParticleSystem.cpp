/*
 *  ParticleSystem.cpp
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "fluid001.h"
#include "ParticleSystem.h"



ParticleSystem::ParticleSystem() {
	curIndex = 0;
	numberOfParticleBirths = 0;
	particleType = 0;
}


void ParticleSystem::setParticleType(int typer){
	particleType = typer;
}


void ParticleSystem::updateAndDraw(){
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(0.3);
	
	if(myApp->renderUsingVA) {
		for(int i=0; i<MAX_PARTICLES; i++) {
			if(particles[i].alpha > 0) {
				particles[i].update();
				particles[i].updateVertexArrays(i, posArray, colArray);
			}
		}    
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, posArray);

		
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colArray);
		
	//	cout << particleType << endl;
		
		switch(particleType) {
				 
			case 0:
				glDrawArrays(GL_LINES, 0, MAX_PARTICLES * 2);
				break;
			case 1:
				glDrawArrays(GL_LINE_LOOP, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			case 2:
				glDrawArrays(GL_POLYGON, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			case 3:
				glDrawArrays(GL_TRIANGLE_STRIP, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			case 4:
				glDrawArrays(GL_LINES_ADJACENCY_EXT, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			case 5:
				glDrawArrays(GL_TRIANGLES_ADJACENCY_EXT, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			case 6:
				glDrawArrays(GL_TRIANGLE_FAN, 0, MIN(numberOfParticleBirths*2, MAX_PARTICLES * 2));
				break;
			default: 
				glDrawArrays(GL_LINES, 0, MAX_PARTICLES * 2);
			
		}
	
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		
		
	} else {
		glBegin(GL_LINES);               // start drawing points
		for(int i=0; i<MAX_PARTICLES; i++) {
			if(particles[i].alpha > 0) {
				particles[i].update();
				particles[i].drawOldSchool();    // use oldschool renderng
			
			}
		}
		glEnd();
	
		
		
	}
	
	glDisable(GL_BLEND);
}


void ParticleSystem::addParticles(float x, float y, int count ){
	for(int i=0; i<count; i++) addParticle(x + ofRandom(-15, 15), y + ofRandom(-15, 15));
}


void ParticleSystem::addParticle(float x, float y) {
	particles[curIndex].init(x, y);
	curIndex++;
	numberOfParticleBirths++;
	if(curIndex >= MAX_PARTICLES) curIndex = 0;
}





