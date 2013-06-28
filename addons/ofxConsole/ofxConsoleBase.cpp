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

#include "ofxConsoleBase.h"
#include "Autometer.h"

ofxConsoleBase::ofxConsoleBase(){
	lineIndex = 0;
	defaultCommand = NULL;
	defaultCommandPre = NULL;
	
	commandBufferSize = 20;
	textBufferSize = 40;

	sugestIndex = 0;
	
	//init bools
	bCommandEcho = true;
	bStackCommandMode = false;

}

ofxConsoleBase::~ofxConsoleBase()
{
	commandBuffer.clear();
	textBuffer.clear();
}


// prints text on the console
void ofxConsoleBase::print(const std::string & strTxt)
{
	// push it
	textBuffer.push_back((consoleText&)strTxt);
	textBuffer.back().textType = CTEXT_COPY;
	
	// if we are out of bounds
	//if((int)textBuffer.size() > textBufferSize)
	//	textBuffer.pop_front();
}

void ofxConsoleBase::print(const std::string & strTxt, consoleTextType textType)
{
	// push it
	textBuffer.push_back((consoleText&)strTxt);
	textBuffer.back().textType = textType;
	
	// if we are out of bounds
	//if((int)textBuffer.size() > textBufferSize)
	//	textBuffer.pop_front();
}


void ofxConsoleBase::stackCommand(string _command)
{
	stackCommandBuffer.push_back( (consoleText&)_command);
}

void ofxConsoleBase::excuteAllStackCommand()
{
/*	std::list<consoleText>::iterator itr = stackCommandBuffer.begin();

	Autometer * am = new Autometer();
	int time = 0;

	for(; itr!=stackCommandBuffer.end(); itr++){
		//am->addSmallTimeFirst((*itr), time, this, &ofxConsoleBase::decStackCommandBuffer);
		AutoCommand * com = new AutoCommand( (*itr), time);
		com->addCB(this, &ofxConsoleBase::decStackCommandBuffer);
		am->add(com);
		time += 5;
	}

	am->setAutoOn(true);
	//	stackCommandBuffer.clear();
 */
}

void ofxConsoleBase::decStackCommandBuffer()
{
	stackCommandBuffer.pop_back();
}

// parses command line
void ofxConsoleBase::parseCommandLine(string command, bool echo)
{
    if(echo){
		commandLine = command;
		commandLineClearCount = 8;
	}
    std::ostringstream out;
	std::string::size_type index = 0;
	std::vector<std::string> arguments;
	std::list<consoleItem>::const_iterator iter;
	bool bCommandFound = false;
	

	// add to text buffer
	if(bCommandEcho)
	{
		print(command);
	}
	

	// add to commandline buffer
	commandBuffer.push_back((consoleText&)command);
	
	if((int)commandBuffer.size() > commandBufferSize){
		commandBuffer.erase(commandBuffer.begin());
	}

	// stack mode
	if(bStackCommandMode && command.find("stackCommandEnd")){
		stackCommand(command);
		return;
	}

	// tokenize
	std::stringstream commandStream(command); 
	std::string temp;
	while(getline(commandStream, temp, ' '))
		arguments.push_back(temp);
	
	// arguments[0] = command name

	// serch and execute
	for(iter = itemList.begin(); iter != itemList.end(); ++iter)
	{
		if(iter->name == arguments[0] || "/"+iter->name == arguments[0])
		{
			bCommandFound = true;
			switch(iter->type)
			{
				case CTYPE_UCHAR:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((unsigned char *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
					}
					else if(arguments.size() == 2)
					{
						*((unsigned char *)(*iter).variable_pointer) = (unsigned char) atoi(arguments[1].c_str());
					}
					break;
					
				case CTYPE_CHAR:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((char *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					else if(arguments.size() == 2)
					{
						*((char *)(*iter).variable_pointer) = (char) atoi(arguments[1].c_str());
						
					}
					break;
					
				case CTYPE_UINT:
					if(arguments.size() > 2)
					{
						
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((unsigned int *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((unsigned int *)(*iter).variable_pointer) = (unsigned int) atoi(arguments[1].c_str());
						
					}
					break;
					
				case CTYPE_INT:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_INT >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((int *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((int *)(*iter).variable_pointer) = (int) atoi(arguments[1].c_str());
						
					}
					break;
				case CTYPE_BOOL:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_BOOL >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((bool *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((bool *)(*iter).variable_pointer) = (bool) atoi(arguments[1].c_str());
					}
					break;
				case CTYPE_FLOAT:
					if(arguments.size() > 2)
					{
						print("Too many arguments for < CTYPE_FLOAT >", CTEXT_ERROR);
					}
					else if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << *((float *)(*iter).variable_pointer);
						print(out.str(), CTEXT_MESSAGE);
						
					}
					if(arguments.size() == 2)
					{
						*((float *)(*iter).variable_pointer) = (float)atof(arguments[1].c_str());
					}
					break;
					
				case CTYPE_STRING:
					if(arguments.size() == 1)
					{
						out.str("");
						out << (*iter).name << " = " << (std::string *)(*iter).variable_pointer;
						print(out.str());
						
					}
					else if(arguments.size() > 1)
					{
						// reset variable_pointeriable
						*((std::string *)(*iter).variable_pointer) = "";
						
						// add new string
						for(int i = 0; i < (int)arguments.size(); ++i)
						{
							*((std::string *)(*iter).variable_pointer) += arguments[i];
						}
						
					}
					break;
					
				case CTYPE_FUNCTION:
					{
						ofxConsoleFProps * fProps = (*iter).function->callBack(arguments);

						/**/
						if(fProps->err.size()){
							std::string args;
							for(int i=0; i<fProps->numArgs; i++){
								args+=fProps->arguments[i];
								if(i<fProps->numArgs-1){
									args+=", ";
								}
							}
							for(int i=0; i< (int)fProps->err.size(); i++){
								print("< "+(*iter).name+"("+args+") > "+fProps->err[i], CTEXT_ERROR);
							}
							
							fProps->err.clear();
						}
						/**/
					}
					
					return;
					break;
					
				default:
					if(defaultCommand!=NULL) defaultCommand->callBack(arguments);
					break;
			}
		} 
	}
	
	if(!bCommandFound) {
		if(defaultCommand!=NULL) defaultCommand->callBack(arguments);
	}
}

// text key hit
void ofxConsoleBase::passKey(char key)
{
	// if we can print the char
	if(key > 31) commandLine += key;
}


// clears one character
void ofxConsoleBase::passBackspace()
{
	if(commandLine.length() > 0)
	{
		commandLine.erase(commandLine.end()-1);
	}
}

// executes
void ofxConsoleBase::passIntro()
{
	if(commandLine.length() > 0)
	{
		parseCommandLine(commandLine);
		commandLine.clear();
		lineIndex=commandBuffer.size();
	}
}

void ofxConsoleBase::addItem(const std::string & strName, void *pointer, consoleType type){
	consoleItem it;
	
	// fill item properties
	it.name = strName;
	it.type = type;
	
	// address
	if(type != CTYPE_FUNCTION)
	{
		it.variable_pointer = pointer;
	}
	
	// add item
	itemList.push_back(it);
}

// deletes an item/function from the console
void ofxConsoleBase::deleteItem(const std::string & strName)
{
	std::list<consoleItem>::iterator iter;
	
	// find item
	for (iter = itemList.begin(); iter != itemList.end(); ++iter)
	{
		if (iter->name == strName)
		{
			itemList.erase(iter);
			break;
		}
	}
}

// sets text buffer size
void ofxConsoleBase::setTextBufferSize(int size)
{
	textBufferSize = size;
	
	// remove any extra line
	if((int)textBuffer.size() > textBufferSize)
	{
		std::list<consoleText>::iterator it;
		it = textBuffer.end();
		advance(it, -textBufferSize);
		textBuffer.erase(textBuffer.begin(), it);
	}
}


// sets commandline buffer size
void ofxConsoleBase::setCommandBufferSize(int size)
{
	commandBufferSize = size;
	
	// remove any extra line
	if((int)commandBuffer.size() > commandBufferSize)
	{
		std::vector<consoleText>::iterator it;
		//advance(it, -commandBufferSize);
		it = commandBuffer.end() - commandBufferSize;
		commandBuffer.erase(commandBuffer.begin(), it);
	}
}

// clears the console's commandline text buffer
void ofxConsoleBase::clearCommandBuffer(){
	commandBuffer.clear();
}

// clears the console's text buffer
void ofxConsoleBase::clearTextBuffer() 
{ 
	textBuffer.clear(); 
}



// changes the commandline text to a previous one
void ofxConsoleBase::historyGo(int where)
{
	// increment line index
	lineIndex += where;
	
	// error correct line index
	if(lineIndex < 0)
	{
		lineIndex = 0;
	}
	else if(lineIndex >= (int)commandBuffer.size())
	{
		lineIndex = commandBuffer.size() - 1;
	}
	
	// set command line
	if(!commandBuffer.empty())
	{
		commandLine = commandBuffer[lineIndex];
	}
}

void ofxConsoleBase::sugestGo(int num)
{
	sugestIndex += num;
	int max = (int) suggestList.size() - 1;

	if(sugestIndex < 0)
	{
		sugestIndex = max;

	}else if(max < sugestIndex)
	{
		sugestIndex = sugestIndex - max - 1;
	}

	// set command line
	if(!suggestList.empty())
	{
		commandLine = suggestList[sugestIndex] + " ";
	}


}

void ofxConsoleBase::clearSuggest()
{
	suggestList.clear();
	bSugestMode = false;
	sugestIndex  = 0;
}

void ofxConsoleBase::doSugest()
{	
	// set command line
	if(!suggestList.empty())
	{
		commandLine = suggestList[sugestIndex] + " ";
	}

	// sugestMode = false;
}

// search
void ofxConsoleBase::searchSugest(string text)
{
	std::ostringstream						out;
	//std::string::size_type				index = 0;
	std::vector<std::string>				arguments;
	std::list<consoleItem>::const_iterator	iter;

	int bSugestFound = false;	

	if(text.size() == 0){
		// suggest all function
		for(iter = itemList.begin(); iter != itemList.end(); ++iter)
		{
			bSugestFound = true;
			suggestList.push_back( iter->name );
			cout << "add sugestList : " << iter->name << endl;
		}
	}else{

		// tokenize
		std::stringstream commandStream(text); 
		std::string temp;
		while(getline(commandStream, temp, ' '))
			arguments.push_back(temp);
	
		if(arguments.size() != 1 )
			return;

		int wordNum = arguments[0].size();

		suggestList.clear();

		// serch and make list
		for(iter = itemList.begin(); iter != itemList.end(); ++iter)
		{
			string head = iter->name.substr(0, wordNum);
	   
			if(head == arguments[0] || "/"+head == arguments[0])
			{
			bSugestFound = true;
			suggestList.push_back( iter->name );
			cout << "add sugestList : " << iter->name << endl;
			}
		}
	}
}