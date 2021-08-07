#ifndef _XRCTL_COMMAND_PARSER_H
#define _XRCTL_COMMAND_PARSER_H
#define VSEXPRESS 1

#include "VesselCommandParser.h"
#include "XRVesselCtrl.h"


using namespace std;

class XRCtlCommandParser : public VesselCommandParser {

public:
	string parse(long lparam, string command);
	bool boolFromString(string token);
	double doubleFromStrings(string token1, string token2);
	int intFromString(string token);

};
bool isXRVessel(VESSEL2* pVessel);
	string getXRVersion(XRVesselCtrl* pVessel);
	string setEngineState(XRVesselCtrl* pVessel, vector<string> tokens);
	string getEngineStates(XRVesselCtrl* pVessel);
	string getDoors(XRVesselCtrl* pVessel);
	string getDoorPositions(XRVesselCtrl* pVessel);
	string setDoorState(XRVesselCtrl* pVessel, vector<string> tokens);
	string toggleDoorState(XRVesselCtrl* pVessel, vector<string> tokens);
	string getFlightControls(XRVesselCtrl* pVessel);
	string killAutoPilots(XRVesselCtrl* pVessel);
	string setFreeDrift(XRVesselCtrl* pVessel);
	string getStandardAPs(XRVesselCtrl* pVessel);
	string getExpandedAPs(XRVesselCtrl* pVessel);
	string setStandardAP(XRVesselCtrl* pVessel, vector<string> tokens);
	string getAttitudeHoldAP(XRVesselCtrl* pVessel);
	string setAttitudeHoldAP(XRVesselCtrl* pVessel, vector<string> tokens);
	string toggleCurrentAttitudeHoldAP(XRVesselCtrl* pVessel);
	string toggleAttitudeHoldAP(XRVesselCtrl* pVessel);
	string toggleLVLHAP(XRVesselCtrl* pVessel);
	string getDescentHoldAP(XRVesselCtrl* pVessel);
	string setDescentHoldAP(XRVesselCtrl* pVessel, vector<string> tokens);
	string getAirspeedHoldAP(XRVesselCtrl* pVessel);
	string setAirspeedHoldAP(XRVesselCtrl* pVessel, vector<string> tokens);
	string getSystemStatus(XRVesselCtrl* pVessel);
	string getMWS(XRVesselCtrl* pVessel);
	string resetMWS(XRVesselCtrl* pVessel);
	string getExteriorLights(XRVesselCtrl* pVessel);
	string setExteriorLight(XRVesselCtrl* pVessel, vector<string> tokens);
	string getHudModes(XRVesselCtrl* pVessel);
	string setHudMode(XRVesselCtrl* pVessel, vector<string> tokens);
	string testMWS(XRVesselCtrl* pVessel);
	string getCenterOfGravity(XRVesselCtrl* pVessel);
	string changeCenterOfGravity(XRVesselCtrl* pVessel, vector<string> tokens);
	string getRCSDockingMode(XRVesselCtrl* pVessel);
	string setRCSDockingMode(XRVesselCtrl* pVessel, vector<string> tokens);
	string getElevatorEVAPortActive(XRVesselCtrl* pVessel);
	string setElevatorEVAPortActive(XRVesselCtrl* pVessel, vector<string> tokens);
	string getStatusMessages(XRVesselCtrl* pVessel, vector<string> tokens);
	

string getSingleEngineState(XRVesselCtrl* pVessel, vector<string> tokens);
string getEngineState(XRVesselCtrl* pVessel, XREngineID id);
string getDoorState(XRVesselCtrl* pVessel, XRDoorID id);
string getDoorStateAndPosition(XRVesselCtrl* pVessel, XRDoorID id);

#endif