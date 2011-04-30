/*
 *  ParticleSystem.h
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */
#pragma once

#include "Particle.h"

#define MAX_PARTICLES		50000


class ParticleSystem {
public:	
	
    float posArray[MAX_PARTICLES * 2 * 2];
    float colArray[MAX_PARTICLES * 3 * 2];
	
    int curIndex;
	
	int numberOfParticleBirths;
    Particle particles[MAX_PARTICLES];
	
    ParticleSystem();
	
	int particleType;
	
    void updateAndDraw();
	void addParticles(float x, float y, int count );
	void addParticle(float x, float y);
	
	void setParticleType(int typer);
};




