#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxConsole.h";

struct myCircle{
	float x, y, radius;
	ofColor col;
};

/* IF YOU DON'T HAVE BOOST, GO TO ofxConsoleBase.h
 AND COMMENT #define USE_BOOST, IF YOU HAVE BOOST
 DON't FORGET TO ADD IT'S PATH TO THE HEADER SEARCH PATHS
 IN YOUR PROJECT SETTINGS!*/

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void randomBgColor();
	void randomBackground(const std::vector<std::string> & args);
	void setBackground(const std::vector<std::string> & args);
	void setBackground(int r, int g, int b);
	void quit(const std::vector<std::string> & args);
	void doubleFunc(double c);
	void addCircle();

    ofxConsole * console;
	int testInt;
	bool bAlphaBlending;
	
	//just a vector of test circles
	std::vector<myCircle> circles;
	
};

#endif
