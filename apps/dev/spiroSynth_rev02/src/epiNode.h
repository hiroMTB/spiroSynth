#pragma once

#include "ofMain.h"

class epiNode{

public:
	
	static const int dotNumMax = 1024*4;
	static float dotPointSize;
	static float nodePointSize;

	ofPoint dotColor;

	bool bShowDot;

	float length;
	float freq;
	float phase, phaseAdder, phaseAdderTarget;

	epiNode * child;

	int head;
	ofPoint wavedata[dotNumMax];

	// axis
	ofVec3f yAxis;		// y axis
	ofVec3f tAxis;		// target axis
	ofVec3f cross;		// cross axis

	ofVec3f p;		// point to be rotated
	ofVec3f p2;	// point after rotation

	float rotAngle;

	epiNode(float _length, float _freq, float _phase=0, epiNode * _child=NULL);


	void step(ofPoint * ret);

	void  draw(float scale);
	void drawCircularGraph(float scale);

	void pushWavedata(ofPoint *data);
	//void pushWavedata(float x, float y, float z);

	void popWavedata();

	// setter
	void setTargetAxis(float x, float y, float z);

	void setChild(epiNode * ep);
	void setRandom();
	void setShowDot(bool b);
	void setLength(float l);
	void setFreq(float f);
	void setColorRed(float r);
	void setColorGreen(float g);
	void setColorBlue(float b);
	void setColor(float r, float g, float b);
	void setColor(ofPoint& c);
	void setAxisFromXY(float x, float y);
	
	// getter
	ofPoint * getWavedata();


	float getLength();
	float getFreq();
	ofVec3f getTargetAxis(){ return tAxis; }
	ofPoint getColor(){
		return dotColor;
	}

};
