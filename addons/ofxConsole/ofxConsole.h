/*
 Text Console for c++/OpenFrameworks 
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*Unless you are a boost/c++ pro and can improve things you should not change anything
 of these files. If you want to have a custom console using OF, simply use ofxConsole.h
 as your base class and build your own thing ontop of it. If you wan't to use it within
 any other c++ environment use ofxConsoleBase.h as your base class.*/

#ifndef _OFXCONSOLE
#define _OFXCONSOLE

#include "ofxConsoleBase.h"
#include "ofMain.h"

class ofxConsole : public ofxConsoleBase
{

//private:
	//static float consolePos_x, consolePos_y;

	
public:

	//static float& getConsolePos_x(){ return consolePos_x; }
	//static float& getConsolePos_y(){ return consolePos_y; }

public:

	int bw, sp;
	int boxW, boxH;

	ofxConsole();	
	virtual ~ofxConsole();

	//void resize(ofResizeEventArgs &e);
    
    void setFont(int size);
	void defaultFunction(const std::vector<std::string> & args);
	void close(const std::vector<std::string> & args);
	void close();
	void setToggleKey(int key);
	void enableCallbacks();
	void disableCallbacks();
	void setFixConsolePos(bool b);
	void sortItem();

	void toggleActive();
	
	void drawBitmapString(const std::string str, float x, float y, void *font);
	void drawBitmapString12(const std::string str, float x, float y);
	void drawBitmapString18(const std::string str, float x, float y);
	bool isActive();

	virtual void render();
    void    renderHistory();
    void    renderSuggest();
    void    renderCommandLine();
	void    renderStackCommand();
    

	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e){}
	
	void defaultFunctionPre(const std::vector<std::string> & args);
    void checkTextBufferSize();
    
protected:

	//settings
	bool bActive, bFixConsolePos;
	int toggleKey;
	//ofPoint consolePos;

	ofColor copyTextColor, messageTextColor, commandlineTextColor, errorTextColor;
	ofColor commandlineColor, backgroundColor;
	ofColor sugestColor;

	int fontSize_big;
	int fontSize_mid;
	int fontSize_small;

	ofTrueTypeFont  franklinBook_big;
	ofTrueTypeFont  franklinBook_mid;
	ofTrueTypeFont  franklinBook_small;
	//	void setConsolePosition();
    
public:
    void	inputKey(char _key){ passKey(_key); }
    void	inputEnterKey(){ passIntro(); }
//    void    setCommandLineString(string command){
//        commandLine = command;
//        passIntro();
//    }
    
};

#endif
