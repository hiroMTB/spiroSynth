/*
 *	sphereBase.h
 *  2011.12.19
 *	hiroshi matoba
 *
 */


#pragma once
#include "ofxOsc.h"

class sphereBase{

public:
	
	bool mute;
	ofPoint pos;
	float	radius;

	virtual void setup(){}
	virtual void uodate(){}	
	virtual void draw(){}

	void  toggleMute(){ mute = !mute; }
	void  setMute(bool m){ mute = m; }

	virtual void keyPressed  (int key){}
	virtual void mouseMoved(int x, int y ){}

	virtual void keyReleased(int key){}

	virtual void rOsc(ofxOscMessage m){}
	

	//virtual void mouseDragged(int x, int y, int button);
	//virtual void mousePressed(int x, int y, int button);
	//virtual void mouseReleased(int x, int y, int button);
	//virtual void windowResized(int w, int h);	

};