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

typedef struct spiroData{
    
    spiroData(){
        for(int i=0; i<4; i++){
            length[i] = 0;
            freq[i] = 0;
        }
    }
    spiroData(const spiroData& s){
        for(int i=0; i<4; i++){
            length[i] = s.length[i];
            freq[i] = s.freq[i];
            color[i] = s.color[i];
            axis[i] = s.axis[i];
        }
    }
    
    float length[4];
    float freq[4];
    ofPoint color[4];
    ofVec3f axis[4];
    
    void print(){
        printf("length[0]= %0.3f, length[1]= %0.3f, length[2]= %0.3f, length[3]= %0.3f\n", length[0], length[1], length[2],length[3]);
        printf("freq[0]= %0.3f, freq[1]= %0.3f, freq[2]= %0.3f, freq[3]= %0.3f\n", freq[0], freq[1], freq[2],freq[3]);
    }
} spiroData;

class sphereEpi : public sphereBase{

public:


    
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
	static spiroData loadFromXml(string name);
    void setSpiroData(spiroData s);
    
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
