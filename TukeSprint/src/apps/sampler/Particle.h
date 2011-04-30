/**
 * Particle.h
 * TukeSprint
 *
 * Created by Marek Bereza on 30/04/2011.
 *
 */


class Particle {
public:
	ofPoint pos, vel;
	float age;
	bool alive;
	static ofImage particle;
	//	static ofxControlPanel *gui;
	static ofColor *color;
	float maxSize;
	float maxAge;
	Particle(float x = 0, float y = 0, float speed = 0, float angle = 0) {
		if(particle.width==0) {
			particle.loadImage("particle.png");
			particle.setAnchorPercent(0.5, 0.5);
		}
		alive = true;
		age = 0;
		pos.x = x;
		pos.y = y;
		speed *= 2;// this is the speed of the particle
		vel.x = speed*cos(angle);
		vel.y = speed*sin(angle);
		maxSize = 20;
		maxAge = 5;
	}
	void update() {
		age++;
		pos += vel;
	}
	
	void draw() {
		float radius = ofMap(age, 0, maxAge, maxSize, 0);
		float alpha = ofMap(age, 0, maxAge, 255, 0);
		if(radius<0) {
			alive = false;
			alpha = 0;
			radius = 0;
		}
		ofSetColor(color->r, color->g, color->b, alpha);
		particle.draw(pos.x, pos.y, radius, radius);
	}
};