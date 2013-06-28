#ifndef _TEST_APP
#define _TEST_APP

#define OSC_PORT_RECEIVE 8000
#define OSC_PORT_RECEIVE_D 8888

#include "ofMain.h"
#include "ofxOsc.h"
#include "sphereEpi.h"
#include "ofx3DUtils.h"
#include "ofxTimeline.h"

//#include <sndfile.h>


class sphereSystem : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void manualUpdate(ofEventArgs &arg);
		void manualDraw(ofEventArgs &args);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void disableCallbacks();

		static const int row = 1;
		static const int col = 3;
		static const int nEpi = row*col;
        static const int epiDepth = 3;
    
		static int oscTargetSlot;

		sphereEpi epi[nEpi];
		ofxEasyCam cam;
		bool bGlobalMute;

		ofxOscReceiver	oscR;
        ofxOscReceiver	oscRd;
        void recieveOsc();
        void recieveOscD();

		bool bZoom;
		float touchX, touchY, pTouchX, pTouchY;
		ofColor bgColor;

		void setBgColorR(float r){ bgColor.r = r; }
		void setBgColorG(float g){ bgColor.g = g; }
		void setBgColorB(float b){ bgColor.b = b; }


		static int getOscTargetSlot(){ return oscTargetSlot; }

		// timeline
		ofxTimeline timeline;
		void receiveTrigger(ofxTLTriggerEventArgs& trigger);
        void changeEpi(int slot, string spiroName);

		void	audioRequested(float * output, int bufferSize, int nChannels){
			ofPoint p;
			
			for (int i = 0; i < bufferSize; i++){
		
				int index = i * nChannels;

				for(int j=0; j<nEpi; j++){
					p.set(0, 0, 0);
					epi[j].step(&p);
					if(&p){
						output[index] +=p.x;
						output[index +1] +=p.y;
					}
				}
				//output[index] = output[index];
				//output[index+1]  = output[index+1];
				//cout << output[index] << endl;
			}
		}

		static const int sampleRate = 44100*4;

		// test
		//static GLfloat distance[3];
    
    
    
    map<string, spiroData> spiroLib;
    void setupSpiroLib();
    
    static int getNEpi(){return nEpi; }
    static int getEpiDepth(){return epiDepth; }
    
    ofSoundStream stream;
};


#endif
