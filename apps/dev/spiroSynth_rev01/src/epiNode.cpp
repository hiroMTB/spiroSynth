#include "epiNode.h"
#include "sphereSystem.h"

float epiNode::dotPointSize = 1.0f;
float epiNode::nodePointSize = 1.0f;


epiNode::epiNode(float _length, float _freq, float _phase, epiNode * _child){
	length = _length;
	freq = _freq;
	phase = _phase;		
	child = _child;

	bShowDot = false;

	yAxis.set(0,0, -1.0f);
	setTargetAxis(0, 0, -1);
	p.set(0, 0, 0);
	p2.set(0, 0, 0);

	int sampleRate = sphereSystem::sampleRate;

	phaseAdder 			= 0.0f;
	phaseAdderTarget = (freq / (float) sampleRate) * TWO_PI;
	//dotColor.set(ofRandom(0, 1.0f), ofRandom(0,1.0f), ofRandom(0,1.0f));
	

	dotPointSize = ofGetHeight() * 0.15f;
	nodePointSize = ofGetHeight() * 0.003f;

	//wavedata = new ofPoint()[dotNumMax];
	head = 0;
}

void epiNode::setTargetAxis(float x, float y, float z){
	tAxis.set(x, y, z);
	tAxis = tAxis.normalize();

	rotAngle = yAxis.angle(tAxis); 		//angle between z and t
	cross = yAxis.crossed(tAxis);		// cross of yAxis and targetAxis
}

void epiNode::step(ofPoint * ret){

	while(TWO_PI < phase){
		phase -= TWO_PI;
	}
	
	phaseAdder = 0.5f*phaseAdder + 0.5f*phaseAdderTarget;
	phase += phaseAdder;
	p.set(length*cos(phase), length*sin(phase), 0);	// rotate p
	p2 = p.getRotated(rotAngle, cross);	// rotate p around cross axis

//    float l = p2.length();
//    float logl = log10(l*100);
//    
//    p2 *= (logl/l) * 0.4;
    
	
	ret->x += p2.x;
	ret->y += p2.y;
	ret->z += p2.z;
	
	if(child){
		child->step(ret);
	}else{
		pushWavedata(ret);
	}
}

void  epiNode::draw(float scale){

// log
//    float l = p2.length();
//    float logl = log10(l*100);
//    
//    ofVec3f logp = p2 * (logl/l) * 0.5;
//    
//	float x = logp.x*scale;
//	float y = logp.y*scale;
//	float z = logp.z*scale;

    	float x = p2.x*scale;
    	float y = p2.y*scale;
    	float z = p2.z*scale;

	{

        if(length!=0){
            glLineWidth(4.0);

            glBegin(GL_LINES);
            
            // target axis
            float s = scale*0.1f;			
            glVertex3f(0, 0, 0); glVertex3f(tAxis.x*s, tAxis.y*s, tAxis.z*s);

            glColor3f(dotColor.x, dotColor.y ,dotColor.z);

            glVertex3f(0, 0, 0); glVertex3f(x, y, z);
            glEnd();
        }
	}		

	// point2
	glPushMatrix();
	glTranslatef(x, y, z);

    if(length!=0){

        ofFill();
        glColor3f(dotColor.x, dotColor.y ,dotColor.z);

        glBegin(GL_POINT);

        glPointSize(nodePointSize);
        glVertex3f(0,0,0);
        glEnd();
    }
    
/*
	if(bShowDot){
		//ofSetColor(255,255,255);
		ofDrawBitmapString("node: "+ofToString(phase, 3), 10, 15);
	}
*/

	if(child){
		child->draw(scale);
	}
	glPopMatrix();
}

void epiNode::drawCircularGraph(float scale){
	if(!bShowDot) return;

	//list<ofPoint*>::iterator itr = wavedata.begin();
    glPointSize(2);
    
	//if(length!=0){
        glColor3f(dotColor.x, dotColor.y, dotColor.z);
        
        glBegin(GL_POINTS);
    
        //for(; itr != wavedata.end(); itr++){
        for(int i=0; i<dotNumMax; i++){
            int index = head+i;
            if(dotNumMax <= index)
                index -= dotNumMax;
            ofPoint p = wavedata[index];

            glVertex3f(p.x*scale, p.y*scale, p.z*scale);
        }

        glEnd();		
    //}
}

void epiNode::pushWavedata(ofPoint * p){
	/*if(wavedata.size() == dotNumMax){
		ofPoint * t = wavedata.back();
		wavedata.pop_back();
		//delete t;
	}*/
	//wavedata.push_front(p);

	wavedata[head].set(p->x, p->y, p->z);

	head ++;
	if(head==dotNumMax){
		head = 0;
	}

}

void epiNode::popWavedata(){
/*		
	while(wavedata.size() > dotNumMax){
		ofPoint * p = wavedata.back();
		wavedata.pop_back();
		delete p;
	}
*/
}

//	
// getter, setter
//
void epiNode::setChild(epiNode * ep){
	child = ep;
}

ofPoint* epiNode::getWavedata(){
	if(child)
		return child->getWavedata();
	else
		return wavedata;
}

// length, freq, color is 0 < 1
void epiNode::setRandom(){ 
	setLength( ofRandom(0,1) );
	setFreq( ofRandom(0, 20000) );
}

void epiNode::setShowDot(bool b){ bShowDot = b; }
void epiNode::setLength(float l){ length = l;}

void epiNode::setFreq(float f){
	int sampleRate = sphereSystem::sampleRate;
	freq = f;
	phaseAdderTarget = (freq / (float) sampleRate) * TWO_PI;
}

void epiNode::setColorRed(float r){ dotColor.x = r; }
void epiNode::setColorGreen(float g){ dotColor.y=g; }
void epiNode::setColorBlue(float b){ dotColor.z=b; }
void epiNode::setColor(float r, float g, float b){ dotColor.x=r; dotColor.y=g; dotColor.z=b; }
void epiNode::setAxisFromXY(float x, float y){ 
	float r = sqrt(pow(x-0.5f,2)+pow(y-0.5f,2));
	x = (x-0.5) * 2.0f; 
	y = (y-0.5) * 2.0f;
	setTargetAxis(y, -x, r*2.0f);
}

// getter
float epiNode::getLength(){ return length; }
float epiNode::getFreq(){ return freq; }


