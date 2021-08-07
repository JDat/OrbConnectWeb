#ifndef _PLANET_COMMAND_PARSER_H
#define _PLANET_COMMAND_PARSER_H

#include "CommandParser.h"
#include "Orbitersdk.h"

using namespace std;

class PlanetCommandParser : public CommandParser {
public:
	string parse(long lparam, string command);

private:
	OBJHANDLE getPlanet(string objId);
	string getPeriod(OBJHANDLE planet);
	string getObliquity(OBJHANDLE planet);
	string getTheta(OBJHANDLE planet);
	string getObliquityMatrix(OBJHANDLE planet);
	string getCurrentRotation(OBJHANDLE planet);
	string getAtmConstants(OBJHANDLE planet);
	string getAtmParams(OBJHANDLE planet, vector<string> tokens);
	string getJCoeffCount(OBJHANDLE planet);
	string getJCoeff(OBJHANDLE planet, vector<string> tokens);
	string getJCoeffs(OBJHANDLE planet);
	string getBaseCount(OBJHANDLE planet);

	OBJHANDLE getBase(OBJHANDLE planet, vector<string> tokens);
	NAVHANDLE getNavHandle(OBJHANDLE base, vector<string> tokens);
	string getBaseName(OBJHANDLE base);
	string getBaseEquPos(OBJHANDLE base);
	string getBasePadCount(OBJHANDLE base);
	string getBasePadEquPos(OBJHANDLE base, vector<string> tokens);
	string getBasePadStatus(OBJHANDLE base, vector<string> tokens);
	string getNavChannel(OBJHANDLE base, vector<string> tokens);
	string getNavFreq(OBJHANDLE base, vector<string> tokens);
	string getNavPos(OBJHANDLE base, vector<string> tokens);
};
#endif