#include "sphereSystem.h"
#include "ofxMtb.h"

int sphereSystem::oscTargetSlot = 2;



void sphereSystem::setup(){

    int frameRate = 60;
	ofBackground(255,255,255);
	ofSetCircleResolution(100);
	ofSetFrameRate(frameRate);
	ofDisableAlphaBlending();

	oscTargetSlot = 1;

    ofSetVerticalSync(true);
    
    setupSpiroLib();
    
    
	for(int i=0; i<nEpi; i++){
		epi[i].setup();
		epi[i].setSlot(i);
	}

	//GLfloat distance[] = {0.0, 0.0, 1.0};
	//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);

	oscR.setup( OSC_PORT_RECEIVE );
	cout << "listening for osc messages on port " << OSC_PORT_RECEIVE << "\n";

    
    oscRd.setup( OSC_PORT_RECEIVE_D );
	cout << "listening from on port " << OSC_PORT_RECEIVE_D << "\n";

	bGlobalMute = false;
	bZoom = false;
	touchX = touchY = pTouchX =  pTouchY = 0;

	bgColor.r = 0; 	bgColor.g = 0; 	bgColor.b = 0; 

	// timeline
	timeline.setup();
	timeline.setDurationInFrames(60*60*3 * frameRate/60);
	timeline.setLoopType(OF_LOOP_NONE);
	timeline.getColors().loadColors("defaultColors.xml");
	timeline.setMovePlayheadOnDrag(true);



	// load XML file for timeline
	//
	// param name structure
	//
	// epi1/amp1, epi1/freq1, epi1_mute
	// epi1/amp2, epi1/freq2
	// epi1/amp3, epi1/freq3
	// epi1/amp4, epi1/freq4
	
	int n = nEpi;
		
	for(int i=0; i<n; i++){		
		timeline.addTriggers(ofToString(i), "triggers/" + ofToString(i) + ".xml");
		//timeline.addKeyframes("amp"+ofToString(i), "amps/" + ofToString(i) + ".xml",  ofRange(0, 1.0f));

		/*
		// KEYFRAMES
		for(int j=1; j<2; j++){
			string nodeNum = ofToString(j);
			string ampFile = epi + "/amp"+nodeNum;
			string freqFile = epi + "/freq"+nodeNum;
			timeline.addKeyframes(ampFile,  ampFile+".xml",  ofRange(0, 1));
			timeline.addKeyframes(freqFile, freqFile+".xml", ofRange(0, 1));
		}
		*/	
	}

	ofAddListener(ofxTLEvents.trigger, this, &sphereSystem::receiveTrigger);
	ofAddListener(ofEvents.update, this, &sphereSystem::manualUpdate);
	ofAddListener(ofEvents.draw, this, &sphereSystem::manualDraw);

    stream.stop();
    stream.listDevices();
    stream.setDeviceID(7);
    stream.setup(this, 2, 0, sampleRate, 256, 4);
    stream.start();
    
//    ofSoundStreamSetup(2, 0, this, sampleRate, 256, 4);
    
}

void sphereSystem::manualUpdate(ofEventArgs &arg){
	recieveOsc();
    
	recieveOscD();
    
	// keyframe
	//for(int i=0; i<nEpi; i++){
	//	float amp = timeline.getKeyframeValue("amp"+ofToString(i)); 
	//	epi[i].getNodeEpi(0)->setLength(amp);
	//}
}

void sphereSystem::update(){	//epi.update(60); //recieveOsc();
//	cout << "default update" << "\n";
}

void sphereSystem::draw(){
//	cout << "default draw" << "\n";
    
}

void sphereSystem::manualDraw(ofEventArgs &args){

	ofBackground(255, 255, 255);
	timeline.draw();

	int w = ofGetWidth();
	int h = ofGetHeight(); 

	if(!bGlobalMute){

		//cam.place();
		//glPopMatrix();

		int dw = w / col;
		int dh = h / row;

        glPushMatrix();
        
		glTranslatef(dw/2, dh/2, 0);
        
//        ofEnableAlphaBlending();
//        ofEnableSmoothing();
        
		for(int i=0; i<row; i++){
			for(int j=0; j<col; j++){
				epi[i*col+j].draw(j*dw, i*dh, 0, 180);
			}
		}
		glPopMatrix();
       
#ifdef _DEBUG
        ofSetColor(0, 0, 0);
		ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate(), 6), 10, 15);
#endif

	}
    
}

void sphereSystem::keyPressed(int key){

	switch(key){
		case '!':
			epi[oscTargetSlot].saveToXml("temp");
			break;

		case '#':
			ofToggleFullscreen();
			break;

		case '$':
			timeline.toggleShow();
			if(timeline.getIsShowing())
				ofShowCursor();
			else
				ofHideCursor();

			break;

		case OF_KEY_LEFT:
			break;

		case ' ':
			timeline.togglePlay();
			//bGlobalMute = false;
			break;

	}
}

void sphereSystem::keyReleased(int key){
	
	switch(key){
		case ' ': 			
			//bGlobalMute = true;
			break;
	}
}

void sphereSystem::disableCallbacks(){
	ofRemoveListener(ofEvents.update, this, &sphereSystem::manualUpdate);
	ofRemoveListener(ofEvents.draw, this, &sphereSystem::manualDraw);
}

void sphereSystem::mouseMoved(int x, int y ){

}

void sphereSystem::mouseDragged(int x, int y, int button){

}

void sphereSystem::mousePressed(int x, int y, int button){

}

void sphereSystem::mouseReleased(int x, int y, int button){

}

void sphereSystem::windowResized(int w, int h){
    cam.setOrigin(OF_ORIGIN);
	cam.perspective();
	cam.position();
	cam.eye();
	cam.up();

}

void sphereSystem::receiveTrigger(ofxTLTriggerEventArgs& t){

	//cout << t.triggerGroupName + " : " + t.triggerName<< endl;

	string slot = t.triggerGroupName;
	string spiro = t.triggerName;

	int slotNum = ofToInt(slot);
    
    changeEpi(slotNum, spiro);
}

void sphereSystem::changeEpi(int slotNum, string spiroName){
    
	// search spiroData
    if(spiroLib.find(spiroName) != spiroLib.end()){
        epi[slotNum].setSpiroData(spiroLib[spiroName]);
    
    }else{
        cout << "cant find any task for slot:" + ofToString(slotNum) + ", triggerName: " + spiroName << endl;
    }
}

void sphereSystem::recieveOscD(){
    
    /*
     
        /1/makeNote 23 12
     
    */
	while( oscRd.hasWaitingMessages() ){
		ofxOscMessage m;
		oscRd.getNextMessage( &m );
		int num = m.getNumArgs();
		string address = m.getAddress();
        
        vector<string> ads = ofSplitString(address, "/");
       
//        cout << address << endl;
        
        if(ads.size() <= 2) return;
        
        if (ads[2] == "makeNote") {
            int slot = ofToInt(ads[1]) -1;
            int noteNum = m.getArgAsInt32(0)  - 48;
            int velocity = m.getArgAsInt32(1);

            string name = (velocity==0) ? "0" : ofToString(noteNum);
            
            changeEpi(slot, name);
        }
    }
}

void sphereSystem::recieveOsc(){

	while( oscR.hasWaitingMessages() ){
		ofxOscMessage m;
		oscR.getNextMessage( &m );
		int num = m.getNumArgs();
		string address = m.getAddress();

		vector<string> ads = ofSplitString(address, "/");

		// this is just a change pannel message, e.g /global or /epi
		if(ads.size() == 1) return;

		string g = ads[0];
		if(g=="global"){ 		
			string ctrl = ads[1];

			if(ctrl == "xyz"){
				ofMouseEventArgs e;
				e.button = 0;
				e.y =  (1.0f - m.getArgAsFloat(0)) * 10000;
				e.x =  (1.0f - m.getArgAsFloat(1)) * 10000;
				//cam.mousePressed(e);
				
				cam.setMouseButton(0);
				cam.mouseDragged(e);
				//cam.mouseReleased(e);
				
			}else if(ctrl == "zoom"){
				ofMouseEventArgs e;
				e.button = 2;
				e.y =  (1.0f - m.getArgAsFloat(0)) * 20000;				
				cam.setMouseButton(2);
				cam.mouseDragged(e);

			}else if(ctrl == "mute"){
				bool mute = m.getArgAsInt32(0);
				bGlobalMute = !mute;
			}else if(ctrl=="pushShow"){
				bool mute = m.getArgAsInt32(0);
				bGlobalMute = !mute;		
			}else if(ctrl=="bgColorR"){
				float r =  m.getArgAsFloat(0) * 255.0f;				
				setBgColorR(r);
			}else if(ctrl=="bgColorG"){
				float g =  m.getArgAsFloat(0) * 255.0f;
				setBgColorG(g);
			}else if(ctrl=="bgColorB"){
				float b =  m.getArgAsFloat(0)* 255.0f;				
				setBgColorB(b);
			}
		}else{
			//for(int i=0; i<nEpi; i++){
				epi[oscTargetSlot].rOsc(m);
			//}
		}
	}
}

void sphereSystem::setupSpiroLib(){
    ofDirectory dir;
	dir.listDir("spiroData");
	dir.sort();
    
	int n = dir.size();
    
	for(int i=0; i<n; i++){
        string path = dir.getPath(i);
		string file = dir.getName(i);
		string name = ofFilePath::getBaseName(path);
        spiroData s = sphereEpi::loadFromXml(file);
        spiroLib.insert(pair<string, spiroData>(name, s));
        
        printf("load: file spiroData %s, name %s \n", file.c_str(), name.c_str());
	}
    
//    cout << " check" << endl;
//    map<string, spiroData>::iterator itr = spiroLib.begin();
//    
//    for(;itr != spiroLib.end(); itr++){
//        cout << (*itr).first << endl;
//    }
}