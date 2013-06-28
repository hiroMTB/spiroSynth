#include "ofxConsole.h"
#include "btBulletDynamicsCommon.h"
#include "ofxMtb.h"
#include "ConsoleFunction.h"

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


//float ofxConsole::consolePos_x = 10;
//float ofxConsole::consolePos_y = 10;

ofxConsole::ofxConsole()
{
	enableCallbacks();
	bActive = true;
	setDefaultCommand(this, &ofxConsole::defaultFunction);
	//setDefaultCommandPre(this, &ofxConsole::defaultFunctionPre);
		
	/*
	 * this is just to show the difference between the syntax. The boost part is
	 * some syntactic c++ madness but pretty straight forward to use and alot more powerful
	 * than the default version since you can register almost any void function you want!
	 */

#ifndef USE_BOOST
	addFunction("close", this, &ofxConsole::close);
#else
	//adds the close function to the console the boost way (boost::function< void(argument types (eg: int, float)) >)
	addFunction("close", (boost::function< void() >)(boost::bind(&ofxConsole::close, this)));
#endif


	backgroundColor.set(255, 255, 255);		// history background
	copyTextColor.set(130, 130, 130);       // history text
	commandlineColor.set(0, 0, 0);          // current background
	commandlineTextColor.set(80, 80, 80);	// current text
	messageTextColor.set(100, 100, 102);	// message like testInt
	sugestColor.set(255, 0, 0);
	errorTextColor.set(200, 0, 100);	// error

	setToggleKey('+');
	
	bw = 13; /// band width
	sp = 100; /// space before string
		
	boxW = 300;
	boxH = bw * (textBufferSize+1);

	bSugestMode = false;
	bFixConsolePos = true;

    commandLineClearCount = 0;

}


void ofxConsole::setFont(int size)
{    
	fontSize_big   = (int) size * 1.3;
	fontSize_mid   = (int) size * 0.85;
	fontSize_small = (int) size * 0.8;
    
	franklinBook_big.loadFont("fonts/MyriadPro-LightSemiExtIt.otf", fontSize_big);		// commandLine
	franklinBook_mid.loadFont("fonts/MyriadPro-LightSemiExtIt.otf", fontSize_mid);		// history, suggest
	franklinBook_small.loadFont("fonts/MyriadPro-LightSemiExtIt.otf", fontSize_small);
    
	textBufferSize = ofGetHeight() / fontSize_small+1;    
}


//void ofxConsole::resize(ofResizeEventArgs &e){}



ofxConsole::~ofxConsole(){
	disableCallbacks();
}


/*
void ofxConsole::setConsolePosition()
{
	if(bFixConsolePos)	
	{
		consolePos.set( ofGetWidth()/3 + 50, ofGetHeight()/2, 0);

	}else{
		btVector3 targetPos;
		targetPos = SelectionController::getSelectedPlanet()->getBtPosition();
		consolePos.set(targetPos.x()+100, targetPos.y(), targetPos.z()); 
		
		// draw line between object and console
		glDisable(GL_LINE_STIPPLE);		// stop dot line
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(1);
		ofSetColor(0,0,0,255);
		ofLine(targetPos.x(), targetPos.y(), consolePos.x, consolePos.y);
	}
}
*/

void ofxConsole::renderStackCommand()
{
	BT_PROFILE("Console::renderConsole");

	int j=0;
	int posx = ofGetWidth() - 800;
	int posy = 0;

	int maxNum = (ofGetHeight()-fontSize_big) / fontSize_mid;

	std::list<consoleText>::iterator itr = stackCommandBuffer.begin();
	
	for(; itr!=stackCommandBuffer.end(); itr++)
	{
		posy += fontSize_mid;
		   
		int row = j / maxNum;
		if( (j % maxNum) == 0 && row !=0){
			posy = fontSize_mid + 5;
			posx = (fontSize_mid * 17) * row;
		}

		string str = static_cast<string>(*itr);
		glColor3f(0, 0.1, 1.0);
		franklinBook_mid.drawString(str, posx, posy);

		j++;
	}
}



//simple render function


void    ofxConsole::renderHistory(){
    if(bActive)    
	{
		// history
		std::list<consoleText>::iterator itr;
        
		int i = 0;
		int posy;
        
		for(itr = textBuffer.begin(); itr!=textBuffer.end(); ++itr)
		{
			posy = (fontSize_small+1) * (textBuffer.size() - i) ;
            
			consoleText str = *(itr);
			str.insert(0, "> ");
			switch(str.textType){
                case CTEXT_MESSAGE:
                    ofSetColor(messageTextColor);
                    franklinBook_small.drawString(str, 0, posy);
                    break;
                case CTEXT_COPY:
                    ofSetColor(copyTextColor);
                    franklinBook_small.drawString(str, 0, posy);
                    break;
                case CTEXT_ERROR:
                    ofSetColor(errorTextColor);
                    franklinBook_small.drawString(str, 0, posy);				
                    break;
			}			
			i++;
		}
	}
}

void    ofxConsole::renderSuggest(){
    if(bActive && bSugestMode)
    {
        int j=0;
        int posx = 0;
        int posy = 0;
        
        int maxNum = (ofGetHeight()-fontSize_big) / fontSize_mid;
        
        std::vector<string>::iterator iter;
        for(iter = suggestList.begin(); iter!=suggestList.end(); iter++)
        {
            posy += fontSize_mid;
            
            int row = j / maxNum;
            if( (j % maxNum) == 0 && row !=0){
                posy = fontSize_mid;
                posx = (fontSize_mid * 17) * row;
            }
            
            string str = *(iter);
            str.insert(0, "-> ");
            glColor3f(sugestColor.r, sugestColor.g, sugestColor.b);
            
            franklinBook_mid.drawString(str, posx,posy);
            
            if(j == sugestIndex){
                glColor3f(0, 0, 0);
                franklinBook_mid.drawString(str, posx, posy);
            }
            
            j++;
        }
    }
}

void    ofxConsole::renderCommandLine(){
    if(bActive){
        ofSetColor(commandlineTextColor);
		franklinBook_big.drawString(commandLine, 3, -5);
    }
}

void ofxConsole::checkTextBufferSize(){
    // if we are out of bounds
	if( (int) textBuffer.size() > textBufferSize)
	{
		std::list<consoleText>::iterator itr = textBuffer.begin();
		for(int i=0; i< textBuffer.size()-textBufferSize; i++){
			itr++;
		}
        
		textBuffer.erase(textBuffer.begin(), itr);
	}
    
    if(commandLineClearCount >= 0){
        if(commandLineClearCount == 0){
            commandLine = "";
            commandLineClearCount = -123;
        }else {
            commandLineClearCount--;
        }
    }

}

void ofxConsole::render(){}


//process key events
void ofxConsole::keyPressed(ofKeyEventArgs &e){

	int key = e.key;

	if(key==toggleKey) {
		toggleActive();
		return;
	}

	if(!bActive) return;
	switch(key){
		case OF_KEY_RIGHT : case OF_KEY_LEFT:
			break;

		case OF_KEY_BACKSPACE:
			passBackspace();
			break;

		case OF_KEY_RETURN:

			if( bSugestMode )
			{
				doSugest();
				clearSuggest();
			}else{
				passIntro();
			}
			break;

		case OF_KEY_UP:
			if( bSugestMode ){
				sugestGo(-1);
			}else{
				historyGo(-1);
			}
			break;

		case OF_KEY_DOWN:	
			if(bSugestMode){
				sugestGo(1);
			}else{
				historyGo(1);
			}
			break;

			// TAB
		case 9:
			if(bSugestMode){
				sugestGo(1);
			}else{
				bSugestMode = true;
				searchSugest(commandLine);
				sugestGo(0);
			}
			break;

		case OF_KEY_ESC:
			clearSuggest();
			commandLine.clear();
			break;

		case ' ':
			// using in ShortCytKey.cpp so do nothing
			if( ofKeyControl() ) 
				break;

			// else FOLLW THROUGH 

		default:
			passKey(key);
			if(bSugestMode) 
				clearSuggest();

			//passIntroPre();
			break;
	}
		
}
	

void ofxConsole::sortItem()
{
	itemList.sort();
}




//customText
void ofxConsole::drawBitmapString(const std::string str, float x, float y, void *font){
	const char * cstr = str.c_str();
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(cstr);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, cstr[i]);
	}
}
	
void ofxConsole::drawBitmapString12(const std::string str, float x, float y){
	const char * cstr = str.c_str();
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(cstr);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, cstr[i]);
	}	
}

void ofxConsole::drawBitmapString18(const std::string str, float x, float y){
	const char * cstr = str.c_str();
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(cstr);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, cstr[i]);
	}
}


void ofxConsole::defaultFunction(const std::vector<std::string> & args){
	print("Not a registered command!", CTEXT_ERROR);

	/// DOOOOO SOMETHING LIKE insert false to some flags!!!
}

	
//close non boost way
void ofxConsole::close(const std::vector<std::string> & args){
	bActive = false;
}
	
//close boost way
void ofxConsole::close(){
	bActive = false;
}
	
void ofxConsole::setToggleKey(int key){
	toggleKey = key;
}
	
//enable OF keyEvents
void ofxConsole::enableCallbacks(){
	//ofAddListener(ofEvents.keyPressed, this, &ofxConsole::keyPressed);
	//ofAddListener(ofEvents.keyReleased, this, &ofxConsole::keyReleased);
	//ofAddListener(ofEvents.windowResized, this, &ofxConsole::resize);
}
	
void ofxConsole::disableCallbacks(){
	//ofRemoveListener(ofEvents.keyPressed, this, &ofxConsole::keyPressed);
	//ofRemoveListener(ofEvents.keyReleased, this, &ofxConsole::keyReleased);

}

	
//activate (show) deactivate (hide) console
void ofxConsole::toggleActive(){
	bActive = !bActive;
}


bool ofxConsole::isActive() 
{
	return bActive;
};



void ofxConsole::setFixConsolePos(bool b)
{
	bFixConsolePos = b;
}


