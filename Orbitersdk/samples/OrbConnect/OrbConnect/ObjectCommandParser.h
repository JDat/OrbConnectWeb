#ifndef _OBJECT_COMMAND_PARSER_H
#define _OBJECT_COMMAND_PARSER_H

#include "CommandParser.h"
#include "Orbitersdk.h"

using namespace std;

class ObjectCommandParser : public CommandParser {
public:
	string parse(long lparam, string command);

private:
	OBJHANDLE getObject(string objId);
	string getObjectCount();
	string getObjectName(OBJHANDLE objH);
	string getObjectType(OBJHANDLE objH);
	string isVessel(OBJHANDLE objH);
	string getSize(OBJHANDLE objH);
	string getMass(OBJHANDLE objH);
	string getGlobalPos(OBJHANDLE objH);
	string getGlobalVel(OBJHANDLE objH);
	string getRelativePos(OBJHANDLE objH, vector<string> tokens);
	string getRelativeVel(OBJHANDLE objH, vector<string> tokens);
    string getRelativePosAndVel(OBJHANDLE objH, vector<string> tokens);
	string getByType(vector<string> tokens);
	string attachCamera(OBJHANDLE objH, vector<string> tokens);
};
#endif