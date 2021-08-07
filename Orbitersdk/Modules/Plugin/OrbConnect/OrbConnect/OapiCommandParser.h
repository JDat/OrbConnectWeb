#ifndef _OAPI_COMMAND_PARSER_H
#define _OAPI_COMMAND_PARSER_H

#include "CommandParser.h"
#include "Gbody.h"

using namespace std;

extern Gbody* sun;

class OapiCommandParser : public CommandParser {

public:
	string parse(long lparam, string command);
private:
	OBJHANDLE getObject(string nameOrIndex);
	string doCamera(long lParam, string command, vector<string> tokens);
	string getGBodyCount();
	string getSimTime();
	string getSimStep();
	string getSysTime();
	string getSysStep();
	string getSimMJD();
	string setSimMJD(vector<string> tokens);
	string getsysMJD();
	string getTimeAsMJD(vector<string> tokens);
	string getTimeAcceleration();
	string setTimeAcceleration(vector<string> tokens);
	string getPause();
	string setPause(vector<string> tokens);
	string getFramerate();
	string isCameraInternal();
	string getCameraMode();
	string getCockpitMode();
	string getCameraTarget();
	string getCameraGlobalPos();
	string getCameraGlobalDir();
	string getCameraTargetDist();
	string getCameraAzimuth();
	string getCameraPolar();
	string getCameraAperture();
	string setCameraAperture(vector<string> tokens);
	string setCameraScaleDist(vector<string> tokens);
	string setCameraRotationAzimuth(vector<string> tokens);
	string setCameraRotationPolar(vector<string> tokens);
	string setCockpitCameraDir(vector<string> tokens);
	string getHudMode();
	string setHudMode(vector<string> tokens);
	string setHudBrightness(vector<string> tokens);
	string toggleHudColor();
	string incrHudIntensity();
	string decrHudIntensity();
	string mouseClick(vector<string> tokens);
	string openMFD(vector<string> tokens);
	string getMFDMode(vector<string> tokens);
	string sendMFDKey(vector<string> tokens);
	string processMFDButton(vector<string> tokens);
	string getMFDButtonLabel(vector<string> tokens);
	string getMFDButtonLabels(vector<string> tokens);
	string getVirtualMFDs();
	string setPanel(vector<string> tokens);
	string switchPanel(vector<string> tokens);
    string getGbodies();
	string clearDebugString();
	string sendDebugString(string command);

};
#endif