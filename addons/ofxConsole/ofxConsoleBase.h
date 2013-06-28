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

#ifndef _OFXCONSOLEBASE
#define _OFXCONSOLEBASE

#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <iostream>

//THIS LINE IS THE ONLY THING YOU SHOULD CHANGE IF YOU DON'T HAVE BOOST
//#define USE_BOOST

#include "ofxConsoleFunctor.h"


//different message types
enum consoleTextType{
	CTEXT_COPY,
	CTEXT_MESSAGE,
	CTEXT_COMMANDLINE,
	CTEXT_ERROR
};

/*
 * extend this class if you want to add more features
 * to consoleText, right now I only added enums to differ
 * between various message/string types. this information
 * can be used to draw the text for example in different colours
 */
class consoleText : public std::string{
public:
	consoleTextType textType;
	virtual void render(){};
};

enum consoleType
{
	CTYPE_UCHAR,
	CTYPE_CHAR,
	CTYPE_CHARP,
	CTYPE_UINT,
	CTYPE_INT,
	CTYPE_FLOAT,
	CTYPE_STRING,
	CTYPE_BOOL,
	CTYPE_FUNCTION
};

typedef ofxConsoleFunctor* consoleFunction;

typedef struct ConsoleItem
{	
	std::string name;			// item name
	consoleType type;			// item type
	consoleTextType textType;
	
	// function callback or pointer to the value
	union{   
		consoleFunction function;
		void *variable_pointer;
	};

	// for sort
	bool operator <(const ConsoleItem &rth)
	{
		// myself = rth  0
		// myself < rth  -
		// myself > rth  +
		int result = name.compare(rth.name);

		return result < 0;
	}

} consoleItem;


class ofxConsoleBase
{
public:
	ofxConsoleBase();
	virtual ~ofxConsoleBase();
	
	void addItem(const std::string & strName, void *pointer, consoleType type);
	
	//adds a function to the console (non boost way), this might look confusing because of the template
	//but it's actually pretty straightforward to use. the template is needed to be able
	//to call functions from any object (not predefined).
	//due to compiler issues with templates in .cpp files it's all put in here.
	template <class T>
	void addFunction( const std::string & strName, 
					  T * objpointer, 
					  void(T::*fctpointer)(const std::vector<std::string> & args) 
					 )
	{
		consoleItem it;
		it.name = strName;
		it.type = CTYPE_FUNCTION;
		
		ofxConsoleFunctorNb<T> * eventHolder = new ofxConsoleFunctorNb<T>();
		eventHolder->set(objpointer, fctpointer);
		it.function = (ofxConsoleFunctor*) eventHolder;
		
		itemList.push_back(it);
	}
	
	// sets the command that is to be executed when no commandline match is found (non boost way)
	// this can currently only be done the non boost way :)
	template <class T>
	void setDefaultCommand(T * objpointer, void(T::*fctpointer)(const std::vector<std::string> & args)) { 
		ofxConsoleFunctorNb<T> * eventHolder = new ofxConsoleFunctorNb<T>();
		eventHolder->set(objpointer, fctpointer);
		defaultCommand = (ofxConsoleFunctor*) eventHolder;
	}

#ifdef USE_BOOST
	//addFunction for a boost callback with no parameter
	void addFunction(const std::string & strName, const boost::function<void()> & f){
		consoleItem it;
		it.name = strName;
		it.type = CTYPE_FUNCTION;
		
		ofxConsoleFunctor0 * eventHolder = new ofxConsoleFunctor0(f);
		it.function = (ofxConsoleFunctor*) eventHolder;
		
		itemList.push_back(it);
	}
/*this generates 9 more addFunction templates so you can add void functions with
 up to 9 parameters!*/
#define BOOST_PP_ITERATION_LIMITS ( 1, 9)
	
//link to the file which gets included for each iteration!
#define BOOST_PP_FILENAME_1 "ofxConsoleItemGenerator.h"
#include BOOST_PP_ITERATE()
	
#endif

	void deleteItem(const std::string & strName);
	void print(const std::string & strText);
	void print(const std::string & strText, consoleTextType textType);
	void setCommandBufferSize(int size);
	void clearCommandBuffer();
	void setTextBufferSize(int size);
	void clearTextBuffer();	
	void historyGo(int where);	
	void passKey(char key);
	void passBackspace();
	void passIntro();
	virtual void render() = 0;
	void parseCommandLine(string command, bool echo=true);
    void setCommandLineString(string command);
	
protected:
	
	std::string commandLine;			// holds the command line
	consoleFunction defaultCommand;		// command to be executed by default	
	consoleFunction defaultCommandPre;

	int lineIndex;		// actual line index (in history)
	int commandBufferSize;
	int textBufferSize;
	
	std::list<consoleItem>   itemList;		// all registered console elements;

	std::list<consoleText>	 textBuffer;	// holds the text buffer
	std::vector<consoleText> commandBuffer;	// holds the buffer for previously entered commands
	std::vector<string> suggestList;	// for sugestion

	bool bCommandEcho;
	bool bStackCommandMode;

	// Sugestion
	bool bSugestMode;
	int  sugestIndex;

	void sugestGo(int num);
	void doSugest();
	void searchSugest(string text);
	void clearSuggest();

	// stack command, do not excute but stack command and excute all at once
	std::list<consoleText>	 stackCommandBuffer;

	bool isStackCommandMode(){ return bStackCommandMode; }

	void stackCommand(string _command);

    int commandLineClearCount;
public:
	void excuteAllStackCommand();
	void setStackCommandMode(bool _mode){ bStackCommandMode = _mode; };
	void decStackCommandBuffer();


};

#endif
