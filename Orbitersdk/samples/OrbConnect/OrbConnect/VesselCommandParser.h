#ifndef _VESSEL_COMMAND_PARSER_H
#define _VESSEL_COMMAND_PARSER_H

#include "CommandParser.h"
#include "Orbitersdk.h"

using namespace std;

// Here to fix typo bug in Orbiter
void oapiGelFocusGlobalPos(VECTOR3 *pos);

// In OrbConnect.cpp
double magnitude(const VECTOR3 &vec);

class VesselCommandParser : public CommandParser {
public:
	string parse(long lparam, string command);

protected:
	string loadVessel(VESSEL2** pVessel, vector<string> tokens);
	int findVesselIndex(VESSEL2* pVessel);
	OBJHANDLE getReferenceObject(string id);
	string getVesselCount();
	string getFocusObjectName();
	string setFocusByIndex(vector<string> tokens);
	string setFocusByName(vector<string> tokens);
	string getCockpitMode();
	string getFocusAltitude();
	string getFocusPitch();
	string getFocusBank();
	string getFocusHeading();
	string getFocusEquPos();
	string getFocusAirspeed();
	string getFocusAirspeedVector();
	string getFocusShipAirspeedVector();
	string getFocusAtmPressureDensity();
	string getFocusEngineStatus();
	string getFocusAttitudeMode();
	string setFocusAttitudeMode(vector<string> tokens);
	string toggleFocusAttitudeMode();
	string getVersion(VESSEL2* pVessel);
	string getName(VESSEL2* pVessel);
	string getClassName(VESSEL2* pVessel);
	string getStatus(VESSEL2* pVessel);
	string getStatus2(VESSEL2* pVessel, vector<string> tokens);
	string getFlightStatus(VESSEL2* pVessel);
	string getMass(VESSEL2* pVessel);
	string getEmptyMass(VESSEL2* pVessel);
	string getFuelMass(VESSEL2* pVessel);
	string getFuelFlowRate(VESSEL2* pVessel);
	string getMaxFuelMass(VESSEL2* pVessel);
	string getPropMass(VESSEL2* pVessel);
	string getPropFlowRate(VESSEL2* pVessel);
	string getNavModeStates(VESSEL2* pVessel);
	string setNavMode(VESSEL2* pVessel, vector<string> tokens);
	string toggleNavModeState(VESSEL2* pVessel, vector<string> tokens);
	string getAPHoldSettings(VESSEL2* pVessel);
	string getAttitudeMode(VESSEL2* pVessel);
	string setAttitudeMode(VESSEL2* pVessel, vector<string> tokens);
	string toggleAttitudeMode(VESSEL2* pVessel);
	string getADCtrlMode(VESSEL2* pVessel);
	string setADCtrlMode(VESSEL2* pVessel, vector<string> tokens);
	string getMaxThrust(VESSEL2* pVessel);
	string getEngineLevels(VESSEL2* pVessel);
	string setEngineLevel(VESSEL2* pVessel, vector<string> tokens);
	string changeEngineLevel(VESSEL2* pVessel, vector<string> tokens);
	string getAltitude(VESSEL2* pVessel);
	string getAirspeed(VESSEL2* pVessel);
	string getShipAirspeedVector(VESSEL2* pVessel);
	string getHorizonAirspeedVector(VESSEL2* pVessel);
	string getAttitude(VESSEL2* pVessel);
	string getAtmConditions(VESSEL2* pVessel);
	string getAccel(VESSEL2* pVessel);
	string getVAccel(VESSEL2* pVessel);
	string getIndSpd(VESSEL2* pVessel);
	string getOrbSpd(VESSEL2* pVessel);
	string getGndSpd(VESSEL2* pVessel);
	string getEquSpd(VESSEL2* pVessel);
	string getRadios(VESSEL2* pVessel);
	string setRadio(VESSEL2* pVessel, vector<string> tokens);
	string setTransponder(VESSEL2* pVessel, vector<string> tokens);
	string setIDS(VESSEL2* pVessel, vector<string> tokens);

	string doFocus(long lparam, string command, vector<string> tokens);
	string SendBufferedKey(VESSEL2* pVessel, vector<string> tokens);

};
#endif