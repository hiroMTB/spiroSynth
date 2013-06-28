#include "sphereEpi.h"
#include "sphereSystem.h"
#include "ofxXmlSettings.h"

const string sphereEpi::EPI_PANNEL = "/epi";
const string sphereEpi::EPI_RANDOM = "/epi/random";

const string sphereEpi::EPI_SHOW_NODE = "/epi/showNode";

const string sphereEpi::EPI_SHOW_ALL_PUSH = "/epi/showAllPush";
const string sphereEpi::EPI_SHOW_ALL_TOGGLE = "/epi/showAllToggle";

const string sphereEpi::EPI_NODE_HEIGHT = "/epi/height";
const string sphereEpi::EPI_NODE_FREQ  = "/epi/freq";
const string sphereEpi::EPI_NODE_RED = "/epi/red";
const string sphereEpi::EPI_NODE_GREEN = "/epi/green";
const string sphereEpi::EPI_NODE_BLUE = "/epi/blue";

const string sphereEpi::EPI_NODE_AXIS = "/epi/axis";

sphereEpi::sphereEpi(){ mute = false; }

sphereEpi::~sphereEpi(){}

void sphereEpi::setup(){
	freq = 400;
	nodeNum = 3;
	
	nodes = new epiNode*[nodeNum];
	float r = 0.2f;

	nodes[0] = 	new epiNode(r, freq);
	nodes[1] = 	new epiNode(r, freq);
	nodes[2] = 	new epiNode(r, freq);
	//nodes[3] = 	new epiNode(r, freq);

	for(int i=0; i<nodeNum-1; i++){
		nodes[i]->setChild(nodes[i+1]);
	}

	nodes[nodeNum-1]->setShowDot(true);

	name = "default.xml";
	spiroData s = loadFromXml(name);
    setSpiroData(s);

	sOsc.setup(OSC_SEND_HOST, OSC_SEND_PORT);

}


void sphereEpi::draw(float x, float y, float z, float scale){
	if(mute) return;

    float alllength=0;
    for(int i=0; i<nodeNum; i++){
		alllength += nodes[i]->getLength();
    }
    
    float logLength = log10(0.5+alllength*99);
    if(logLength<=0) logLength=alllength;
    
    float logRate = (logLength / alllength) *0.6;
    scale *= logRate;
    

	glPushMatrix();
	glTranslatef(x, y, z);
		
//	nodes[0]->draw(scale);

	for(int i=0; i<nodeNum; i++){
		nodes[i]->drawCircularGraph(scale);
		nodes[i]->popWavedata();
	}

	glPopMatrix();
}



void sphereEpi::rOsc(ofxOscMessage m){

	string address = m.getAddress();

	cout << address << endl;

	//vector<string> prop = ofSplitString(address, "_");
	//int n = prop.size();
	const char c = address[address.size()-1];
	int node = atoi(&c);

	/// NODE CTLR
	if(0<= node && node < nodeNum){
		address = address.substr(0, address.size()-1);

		if(address == EPI_NODE_HEIGHT){
			float v = m.getArgAsFloat(0) * maxLength;
			nodes[node]->setLength(v);
		}else if(address == EPI_NODE_FREQ){
			float v = m.getArgAsFloat(0) * maxFreq;
			nodes[node]->setFreq(v);
		}else if(address == EPI_NODE_RED){
			float v = m.getArgAsFloat(0);
			nodes[node]->setColorRed(v);
		}else if(address == EPI_NODE_GREEN){
			float v = m.getArgAsFloat(0);
			nodes[node]->setColorGreen(v);
		}else if(address == EPI_NODE_BLUE){
			float v = m.getArgAsFloat(0);
			nodes[node]->setColorBlue(v);
		}else if(address == EPI_RANDOM){
			nodes[node]->setRandom();
		}else if(address == EPI_SHOW_NODE){
			bool v = m.getArgAsInt32(0);
			nodes[node]->setShowDot(v);
		}else if(address == EPI_NODE_AXIS){
			float x = m.getArgAsFloat(0);
			float y = m.getArgAsFloat(1);
			nodes[node]->setAxisFromXY(x, y);
		}
	}else{

		if (address == EPI_SHOW_ALL_PUSH){
		
		}else if(address == EPI_SHOW_ALL_TOGGLE){
		}
	}

/*else{
		cout << "recieve strange OSC value in EpiCyclicSphere" << endl; 
	}
*/

/*
	float padx = m.getArgAsFloat( 0 ) * ofGetWidth()/2.0f;
	float pady = m.getArgAsFloat( 1 ) * ofGetHeight()/2.0f;

	ofVec3f relx = cam.getDir().getCrossed(cam.getUp()).normalize();
	ofVec3f rely = cam.getDir().getCrossed(relx).normalize();	   
	ofVec3f rot = relx * padx + rely * -pady;
	cam.orbitAround(cam.getEye(), rot, rot.length() * cam.orbitSpeed);
*/	
}

void sphereEpi::keyPressed(int key){}


void sphereEpi::mouseMoved(int x, int y ){}


epiNode * sphereEpi::getNodeEpi(int n){
	if(nodeNum < n) return NULL;
	return nodes[n];
}



// XML format
/*
<spiro>
	<name>TEST SPIRO</name>

	<node>
		<amp>0.3</amp>
		<freq>800</freq>
		<color>1234</color>
		<axis>
			<x>10</x>
			<y>25</y>
			<z>78</z>
		</axis>
	</node>

	<node></node>
</spiro>

*/

void sphereEpi::saveToXml(string name){

	ofxXmlSettings xml;

	xml.addTag("spiro");
	xml.pushTag("spiro");
		xml.setValue("name", name); // same with file name so does not need.

		for(int i=0; i<nodeNum; i++){
			//string node = "node_"+ofToString(i);
			xml.addTag("node");
			xml.pushTag("node", i);
				xml.setValue("amp", nodes[i]->getLength());
				xml.setValue("freq", nodes[i]->getFreq());
			
				xml.addTag("color");
				xml.pushTag("color");
					ofPoint c = nodes[i]->getColor();
					xml.setValue("r", c.x);
					xml.setValue("g", c.y);
					xml.setValue("b", c.z);
				xml.popTag();

				xml.addTag("axis");
				xml.pushTag("axis");
					ofVec3f v = nodes[i]->getTargetAxis();
					xml.setValue("x", v.x);
					xml.setValue("y", v.y);
					xml.setValue("z", v.z);
				xml.popTag();
			xml.popTag();
		}
	xml.popTag();

	xml.saveFile("spiroData/" + name + ".xml");
}

// name = "mySuperSpiro.xml"
spiroData sphereEpi::loadFromXml(string name){

    spiroData sd;
    
	ofxXmlSettings xml;
	string file = "spiroData/" + name;
	bool ok = xml.loadFile(file);
	
	if(!ok){
		cout << "fail to load " << file << endl;
	}else{
		
		xml.pushTag("spiro");

		int n = xml.getNumTags("node");
	
		for(int i=0; i<n; i++){
			xml.pushTag("node", i);
				float amp = xml.getValue("amp", 12345.0f);
				float freq = xml.getValue("freq", 12345.0f);

				xml.pushTag("color");
					float r = xml.getValue("r", -123.0f);
					float g = xml.getValue("g", -123.0f);
					float b = xml.getValue("b", -123.0f);
				xml.popTag();

				xml.pushTag("axis");
					float x = xml.getValue("x", -123.0f);
					float y = xml.getValue("y", -123.0f);
					float z = xml.getValue("z", -123.0f);
				xml.popTag();
			xml.popTag();

            sd.length[i] = amp;
            sd.freq[i]  = freq;
            sd.color[i].x = r;
            sd.color[i].y = g;
            sd.color[i].z = b;
            sd.axis[i].x = x;
            sd.axis[i].y = y;
            sd.axis[i].z = z;
        }
        
		xml.popTag();
    }
    
    return sd;
}

void sphereEpi::setSpiroData(spiroData s){

    for(int i=0; i<sphereSystem::getEpiDepth(); i++){
        float amp = s.length[i];
        float freq = s.freq[i];
        ofPoint& c = s.color[i];
        ofVec3f& v = s.axis[i];
        float r = c.x;
        float g = c.y;
        float b = c.z;
        float x = v.x;
        float y = v.y;
        float z = v.z;
        
        nodes[i]->setLength(amp);
        nodes[i]->setFreq(freq);
        nodes[i]->setColor(c);
        nodes[i]->setTargetAxis(x, y, z);
        
        //if(sendOsc)
        // send setting data to oscTargetSlot
        if(slot == sphereSystem::getOscTargetSlot()){

            ofxOscBundle bundle;
            ofxOscMessage mAmp, mFreq, mR, mG, mB;
            string node = ofToString(i);

            mAmp.setAddress(EPI_NODE_HEIGHT+node);
            mAmp.addFloatArg(amp);

            mFreq.setAddress(EPI_NODE_FREQ+node);
            mFreq.addFloatArg(freq);

            mR.setAddress(EPI_NODE_RED+node);
            mR.addFloatArg(r);
            
            mG.setAddress(EPI_NODE_GREEN+node);
            mG.addFloatArg(g);
            
            mB.setAddress(EPI_NODE_BLUE+node);
            mB.addFloatArg(b);
            
            bundle.addMessage(mAmp);
            bundle.addMessage(mFreq);
            bundle.addMessage(mR);
            bundle.addMessage(mG);
            bundle.addMessage(mB);
            sOsc.sendBundle(bundle);

            // axis... mendokusai
        }
		
		
	}
}