#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	console = new ofxConsole();
	testInt = 0;
	
	ofSetCircleResolution(45);

	//set key to open and close the console (default is '+')
	console->setToggleKey('#');
	
    //just a welcome message :)
	console->print("Welcome to the alpha version of ofxConsole! YEAAAH DUUDE!");
	
	//We can add pointers to variables, which can be changed through the console.
	console->addItem("testInt", &testInt, CTYPE_INT);
	
	//We can add pointers to variables, which can be changed through the console.
	console->addItem("alphaBlending", &bAlphaBlending, CTYPE_BOOL);
	
	console->addFunction("randomBackground", this, &testApp::randomBackground);
	console->addFunction("setBackground", this, &testApp::setBackground);
	console->addFunction("quit", this, &testApp::quit);

	/*this is just to show the difference between the syntax of adding a boost callback and a "normal" one. 
	 The boost part is some syntactic c++ madness but pretty straight forward to use and alot more powerful
	 than the default version since you can register almost any void function you
	 want! (you should keep in mind though that since it is a textconsole things other than long, int, float, double, bool
	 and string dont really make sense, the varying amount of arguments is the real plus!)*/
	
	/*EDIT: Actually you should be able to leave the type cast to boost::function out, but for some reason that did
	 not work for me*/
#ifdef USE_BOOST
	console->addFunction("setBackgroundBoostWay", (boost::function< void(int, int, int) >)
						 (boost::bind(&testApp::setBackground, this, _1, _2, _3)));
	console->addFunction("charFunc", (boost::function< void(double) >)
						 (boost::bind(&testApp::doubleFunc, this, _1)));
	console->addFunction("addCircle", (boost::function< void() >)
						 (boost::bind(&testApp::addCircle, this)));
#endif
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	//render the console!
	if(bAlphaBlending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	for(int i=0; i<circles.size(); i++){
		glColor4f(circles[i].col.r/255.0, circles[i].col.g/255.0, circles[i].col.b/255.0, circles[i].col.a/255.0);
		ofCircle(circles[i].x, circles[i].y, circles[i].radius);
	}
	if(bAlphaBlending) {
		glDisable(GL_BLEND);
	}
	console->render();
}

void testApp::addCircle(){
	ofColor col;
	col.r = ofRandom(0,255);
	col.g = ofRandom(0,255);
	col.b = ofRandom(0,255);
	col.a = ofRandom(100,255);
	//myCircle c = {ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), ofRandom(30, 400), col};
	myCircle c;
	c.x = ofRandom(0, ofGetWidth());
	c.y = ofRandom(0, ofGetHeight());
	c.radius = ofRandom(3, 300);
	c.col = col;
	circles.push_back(c);
}

void testApp::doubleFunc(double c){
}

void testApp::randomBackground(const std::vector<std::string> & args){
	ofBackground(ofRandom(0,255), ofRandom(0, 255), ofRandom(0,255));
}

void testApp::setBackground(const std::vector<std::string> & args){
	if(args.size()!=4){
		console->print("Wrong number of arguments for < "+args[0]+" >!", CTEXT_ERROR);
		return;
	}
	setBackground(atoi(args[1].c_str()), atoi(args[2].c_str()), atoi(args[3].c_str()));
}

void testApp::setBackground(int r, int g, int b){
	ofBackground(r, g, b);
}

void testApp::quit(const std::vector<std::string> & args){
	std::exit(0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

