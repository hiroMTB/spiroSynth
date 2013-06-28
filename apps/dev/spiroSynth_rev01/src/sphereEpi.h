/*
 *	ssphereEpi.h
 *  2011.12.19
 *	hiroshi matoba
 *
 */

#pragma once


#define OSC_SEND_PORT 9000
#define OSC_SEND_HOST "169.254.240.60"

#include "ofMain.h"
#include "sphereBase.h"
#include "ofCamera.h"
#include "epiNode.h"
#include "ofxOsc.h"

class sphereEpi : public sphereBase{

private:
	static const int maxLength = 1;
	static const int maxFreq = 1000;

	static const string EPI_PANNEL;

	static const string EPI_RANDOM;
	static const string EPI_SHOW_NODE;

	static const string EPI_SHOW_ALL_PUSH;
	static const string EPI_SHOW_ALL_TOGGLE;


	static const string EPI_NODE_HEIGHT;
	static const string EPI_NODE_FREQ;
	static const string EPI_NODE_RED;
	static const string EPI_NODE_GREEN;
	static const string EPI_NODE_BLUE;
	static const string EPI_NODE_AXIS;

	epiNode **nodes;
	int nodeNum;
	float freq;

	string name;
	int slot;

public:

	sphereEpi();
	~sphereEpi();

	virtual void setup();

	virtual void draw(float x, float y, float z, float scale);
	virtual void rOsc(ofxOscMessage m);


	virtual void keyPressed  (int key);
	virtual void mouseMoved(int x, int y );

	epiNode * getNodeEpi(int n);

	// save, load
	void saveToXml(string name);
	void loadFromXml(string name);

	//void register(){}

	inline void step(ofPoint * ret){
		if(!mute){
			nodes[0]->step(ret);
		}
	}


	void setSlot(int s){ slot = s; }

	// should be moved
	ofxOscSender sOsc;

};
