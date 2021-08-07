/**
* This file is part of Orb:Connect.
*
*    Orb:Connect is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    Orb:Connect is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with Orb:Connect.  If not, see <http://www.gnu.org/licenses/>.
*
* @copyright 2008 Randy Stearns
* @Author Randy Stearns
* @Author Russ Purinton
*/

#include "XRCtlCommandParser.h"

const char *XRVessels[] = { "DeltaGliderXR1", "XR2Ravenstar", "XR5Vanguard" };
bool testMWSPressed = false;

// 	park log command oapiWriteLog(const_cast<char*> (result.str().c_str()));

string XRCtlCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    bool found = false;
    VESSEL2* pVessel = 0;
    XRVesselCtrl* pXRVessel;
    try
    {
        vector<string> tokens = stringSplit(command, ":");

        message = loadVessel(&pVessel, tokens);
        if (0 == pVessel) {
            return "ERR05";
        } else if (!isXRVessel(pVessel)) {
            return "ERR06";
        }

        pXRVessel = (XRVesselCtrl*) pVessel;
        if (0 == pXRVessel) {
            return "ERR07";
        }

        string cmd = uCase(tokens.at(2));

        if ("XRVERSION" == cmd)
        {
            message = getXRVersion(pXRVessel);
        }
        else if ("SETENGINE" == cmd && tokens.size() >= 5)
        {
            message = setEngineState(pXRVessel, tokens);
        }
        else if ("ENGINE" == cmd)
        {
            message = getSingleEngineState(pXRVessel, tokens);
        }
        else if ("ENGINES" == cmd)
        {
            message = getEngineStates(pXRVessel);
        }
        else if ("DOORS" == cmd)
        {
            message = getDoors(pXRVessel);
        }
        else if ("DOORSPOS" == cmd)
        {
            message = getDoorPositions(pXRVessel);
        }
        else if ("SETDOOR" == cmd && tokens.size() == 5)
        {
            message = setDoorState(pXRVessel, tokens);
        }
        else if ("TOGGLEDOOR" == cmd && tokens.size() == 4)
        {
            message = toggleDoorState(pXRVessel, tokens);
        }
        else if ("FLTCTRLS" == cmd)
        {
            message = getFlightControls(pXRVessel);
        }
        else if ("KILLAPILOTS" == cmd)
        {
            message = killAutoPilots(pXRVessel);
        }
        else if ("SETFREEDRIFT" == cmd)
        {
            message = setFreeDrift(pXRVessel);
        }
        else if ("STDAPS" == cmd)
        {
            message = getStandardAPs(pXRVessel);
        }
        else if ("EXPAPS" == cmd)
        {
            message = getExpandedAPs(pXRVessel);
        }
        else if ("SETSTDAP" == cmd && tokens.size() == 5)
        {
            message = setStandardAP(pXRVessel, tokens);
        }
        else if ("ATTHLDAP" == cmd)
        {
            message = getAttitudeHoldAP(pXRVessel);
        }
        else if ("SETATTHLDAP" == cmd && tokens.size() > 3)
        {
            message = setAttitudeHoldAP(pXRVessel, tokens);
        }
        else if ("TOGGLEATTHLDAP" == cmd)
        {
            message = toggleAttitudeHoldAP(pXRVessel);
        }
        else if ("TOGGLELVLHAP" == cmd)
        {
            message = toggleLVLHAP(pXRVessel);
        }
        else if ("TOGGLECURRATTHLDAP" == cmd)
        {
            message = toggleCurrentAttitudeHoldAP(pXRVessel);
        }
        else if ("DSCNTHLDAP" == cmd)
        {
            message = getDescentHoldAP(pXRVessel);
        }
        else if ("SETDSCNTHLDAP" == cmd)
        {
            message = setDescentHoldAP(pXRVessel, tokens);
        }
        else if ("AIRSPDAP" == cmd)
        {
            message = getAirspeedHoldAP(pXRVessel);
        }
        else if ("SETAIRSPDAP" == cmd)
        {
            message = setAirspeedHoldAP(pXRVessel, tokens);
        }
        else if ("SYSSTATUS" == cmd)
        {
            message = getSystemStatus(pXRVessel);
        }
        else if ("MWS" == cmd)
        {
            message = getMWS(pXRVessel);
        }
        else if ("RESETMWS" == cmd)
        {
            message = resetMWS(pXRVessel);
        }
        else if ("EXTLIGHTS" == cmd)
        {
            message = getExteriorLights(pXRVessel);
        }
        else if ("SETEXTLIGHT" == cmd && tokens.size() == 5)
        {
            message = setExteriorLight(pXRVessel, tokens);
        }
        else if ("HUDMODES" == cmd)
        {
            message = getHudModes(pXRVessel);
        }
        else if ("SETHUDMODE" == cmd && tokens.size() == 5)
        {
            message = setHudMode(pXRVessel, tokens);
        }
        else if ("TESTMWS" == cmd)
        {
            message = testMWS(pXRVessel);
        }
        else if ("COG" == cmd)
        {
            message = getCenterOfGravity(pXRVessel);
        }
        else if ("CHGCOG" == cmd)
        {
            message = changeCenterOfGravity(pXRVessel, tokens);
        }
        else if ("RCSDOCKMODE" == cmd)
        {
            message = getRCSDockingMode(pXRVessel);
        }
        else if ("SETRCSDOCKMODE" == cmd)
        {
            message = setRCSDockingMode(pXRVessel, tokens);
        }
        else if ("ELEVEVA" == cmd)
        {
            message = getElevatorEVAPortActive(pXRVessel);
        }
        else if ("SETELEVEVA" == cmd)
        {
            message = setElevatorEVAPortActive(pXRVessel, tokens);
        }
        else if ("STATUSMSGS" == cmd)
        {
            message = getStatusMessages(pXRVessel, tokens);
        }
    }
    catch (...) 
    {
        message = "ERR99";
    }

    return message;
}

bool isXRVessel(VESSEL2* pVessel) {
    // NOTE: this list could be read from an external config file as well
    // XR1: if (Version() >= 2) bXRVesselCtrlEnabled = true;
    // XR5: bXRVesselCtrlEnabled = true;

    char *vesselClassName = pVessel->GetClassName();    

    bool result = false;   // assume not an XR vessel
    for (int i = 0; i < (sizeof(XRVessels) / sizeof(char *)); i++)
    {
        if (_stricmp(vesselClassName, XRVessels[i]) == 0)   // ignore case here
        {
            result = true;
            if (vesselClassName != 0) {
                string myString(vesselClassName); 
                if (myString.find("XR1",0) == 0 && pVessel->Version() < 2) {
                    result = false;		// oops, version too old
                }
            }
            break;
        }
    }

    return result;
}

string getXRVersion(XRVesselCtrl* pVessel) {
    stringstream msg;

    msg << pVessel->GetCtrlAPIVersion();
    return msg.str();
}

/*
* Engine ID ( 0 <= n <= 8 )
* Throttle level ( 0.0 <= n <= 1.0 )
* Gimbal X ( -1.0 <= n <= 1.0 )
* Gimbal Y ( -1.0 <= n <= 1.0 )
* Balance ( -1.0 <= n <= 1.0 )
* Centering Mode X (Pitch. 1 = on, 0 = off)
* Centering Mode Y (Yaw. 1 = on, 0 = off)
* Centering Mode Balance (1 = on, 0 = off)
* Auto Mode (1 = on, 0 = off)
* Divergent Mode (1 = on, 0 = off)
*
* Non-supplied values do not update existing state
*/
string setEngineState(XRVesselCtrl* pVessel, vector<string> tokens) {
    XREngineStateRead state;
    int intValue;
    bool converted = CommandParser::intFromString(tokens.at(3), intValue);
    if (! converted)
    {
        return "ERR04";
    }

    XREngineID engineId = (XREngineID) intValue;
    pVessel->GetEngineState(engineId, state);

    if (converted && !tokens.at(4).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(4), state.ThrottleLevel);
    }
    if (converted && tokens.size() >= 6 && !tokens.at(5).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(5), state.GimbalX);
    }
    if (converted && tokens.size() >= 7 && !tokens.at(6).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(6), state.GimbalY);
    }
    if (converted && tokens.size() >= 8 && !tokens.at(7).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(7), state.Balance);
    }
    if (converted && tokens.size() >= 9 && !tokens.at(8).empty()) {
        converted = CommandParser::boolFromString(tokens.at(8), state.CenteringModeX);
    }
    if (converted && tokens.size() >= 10 && !tokens.at(9).empty()) {
        converted = CommandParser::boolFromString(tokens.at(9), state.CenteringModeY);
    }
    if (converted && tokens.size() >= 11 && !tokens.at(10).empty()) {
        converted = CommandParser::boolFromString(tokens.at(10), state.CenteringModeBalance);
    }
    if (converted && tokens.size() >= 12 && !tokens.at(11).empty()) {
        converted = CommandParser::boolFromString(tokens.at(11), state.AutoMode);
    }
    if (converted && tokens.size() >= 13 && !tokens.at(12).empty()) {
        converted = CommandParser::boolFromString(tokens.at(12), state.DivergentMode);
    }

    if (!converted)
    {
        return "ERR04";
    }

    return (pVessel->SetEngineState(engineId, state) ? "OK" : "ERR04");
}

string getEngineStates(XRVesselCtrl* pVessel) {
    string states;
    states.append(getEngineState(pVessel, XRE_MainLeft));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_MainRight));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_HoverFore));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_HoverAft));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_ScramLeft));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_ScramRight));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_RetroLeft));
    states.append(";");
    states.append(getEngineState(pVessel, XRE_RetroRight));

    return states;
}

string getSingleEngineState(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (4 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";

    int engineId;
    bool success = CommandParser::intFromString(tokens.at(3), engineId);

    return success ? getEngineState(pVessel, (XREngineID) engineId)
        : "ERR04";
}

string getDoors(XRVesselCtrl* pVessel) {
    string states;
    states.append(getDoorState(pVessel, XRD_DockingPort));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_ScramDoors));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_HoverDoors));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_Ladder));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_Gear));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_RetroDoors));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_OuterAirlock));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_InnerAirlock));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_AirlockChamber));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_CrewHatch));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_Radiator));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_Speedbrake));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_APU));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_CrewElevator));
    states.append(";");
    states.append(getDoorState(pVessel, XRD_PayloadBayDoors));
    states.append(";");
    states.append(getElevatorEVAPortActive(pVessel));

    return states;
}

string getDoorPositions(XRVesselCtrl* pVessel) {
    string states;
    states.append(getDoorStateAndPosition(pVessel, XRD_DockingPort));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_ScramDoors));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_HoverDoors));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_Ladder));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_Gear));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_RetroDoors));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_OuterAirlock));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_InnerAirlock));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_AirlockChamber));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_CrewHatch));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_Radiator));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_Speedbrake));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_APU));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_CrewElevator));
    states.append(";");
    states.append(getDoorStateAndPosition(pVessel, XRD_PayloadBayDoors));

    return states;
}

string setDoorState(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (5 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";
    if (tokens.at(4).empty()) return "ERR03";

    stringstream msg;
    int doorId;
    int doorState;
    bool success = CommandParser::intFromString(tokens.at(3), doorId);
    if (!success)
    {
        return "ERR04";
    }
    success = CommandParser::intFromString(tokens.at(4), doorState);
    if (!success)
    {
        return "ERR04";
    }

    bool result = pVessel->SetDoorState((XRDoorID) doorId, (XRDoorState) doorState);

    return (result ? "OK" : "ERR08");
}

string toggleDoorState(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (4 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";

    stringstream msg;
    int doorId;
    XRDoorState newDoorState;
    bool success = CommandParser::intFromString(tokens.at(3), doorId);
    if (!success)
    {
        return "ERR04";
    }

    XRDoorState doorState = pVessel->GetDoorState((XRDoorID) doorId);
    switch (doorState)
    {
      case XRDS_Opening:
         newDoorState = XRDS_Closing;
         break;
      case XRDS_Open:
         newDoorState = XRDS_Closing;
         break;
      case XRDS_Closing:
         newDoorState = XRDS_Opening;
         break;
      case XRDS_Closed:
         newDoorState = XRDS_Opening;
         break;
      default:
        newDoorState = XRDS_Failed;
         break;
    }
    bool result = pVessel->SetDoorState((XRDoorID) doorId, (XRDoorState) newDoorState);

    return (result ? "OK" : "ERR08");
}

string getFlightControls(XRVesselCtrl* pVessel) {
    char buffer [sizeof(long)*8+1];

    string states;
    _ltoa_s(pVessel->GetADCtrlMode(), buffer, 10);
    states.append(buffer);
    states.append(";");
    _ltoa_s(pVessel->GetAttitudeMode(), buffer, 10);
    states.append(buffer);
    states.append(";");
    states.append(pVessel->IsRCSDockingMode() ? "1" : "0");
    states.append(";0");  // Future CoG Mode

    return states;
}

string killAutoPilots(XRVesselCtrl* pVessel) {
    pVessel->KillAutopilots();

    return "OK";
}

string setFreeDrift(XRVesselCtrl* pVessel) {
    pVessel->KillAutopilots();
    pVessel->SetAttitudeMode(0);
    pVessel->SetADCtrlMode(0);

    return "OK";
}

string getStandardAPs(XRVesselCtrl* pVessel) {
    stringstream result;

    result << pVessel->GetStandardAP(XRSAP_KillRot);
    result << pVessel->GetStandardAP(XRSAP_LevelHorizon);
    result << pVessel->GetStandardAP(XRSAP_Prograde);
    result << pVessel->GetStandardAP(XRSAP_Retrograde);
    result << pVessel->GetStandardAP(XRSAP_Normal);
    result << pVessel->GetStandardAP(XRSAP_AntiNormal);
    result << pVessel->GetStandardAP(XRSAP_Hover);

    return result.str();
}

string getExpandedAPs(XRVesselCtrl* pVessel) {
    stringstream result;
    XRAttitudeHoldState attHldState;
    XRDescentHoldState dscHldState;
    XRAirspeedHoldState aspdHldState;

    pVessel->GetAttitudeHoldAP(attHldState);
    pVessel->GetDescentHoldAP(dscHldState);
    pVessel->GetAirspeedHoldAP(aspdHldState);
    
    result << pVessel->GetStandardAP(XRSAP_KillRot);
    // LVLH
    result << (attHldState.on 
                    && 0.01 > abs(attHldState.TargetPitch) 
                    && (0.01 > abs(attHldState.TargetBank) 
                    || 0.01 > 180.0 - abs(attHldState.TargetBank) 
                    || 0.01 > 360.0 - abs(attHldState.TargetBank))
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    result << pVessel->GetStandardAP(XRSAP_Prograde);
    result << pVessel->GetStandardAP(XRSAP_Retrograde);
    result << pVessel->GetStandardAP(XRSAP_Normal);
    result << pVessel->GetStandardAP(XRSAP_AntiNormal);
    // Descent Hld
    result << (dscHldState.on 
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    // Hover
    result << (dscHldState.on && !dscHldState.AutoLandMode && 0.05 > abs(dscHldState.TargetDescentRate)
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    // Autoland
    result << (dscHldState.on && dscHldState.AutoLandMode
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    // Inertial (Hold Attitude)
    result << (attHldState.on 
                    && ((abs(attHldState.TargetPitch) > 0.05) 
                    || (abs(attHldState.TargetBank) > 0.05))
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    // Airspeed Hld
    result << (aspdHldState.on 
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);
    // Free Drift)
    result << (!attHldState.on && !dscHldState.on & !aspdHldState.on 
                    && 0 == pVessel->GetFlightStatus() && 0 == pVessel->GetADCtrlMode() && 0 == pVessel->GetAttitudeMode()
                    && XRAPSTATE_Disengaged == pVessel->GetStandardAP(XRSAP_KillRot)
                    && XRAPSTATE_Disengaged == pVessel->GetStandardAP(XRSAP_Prograde)
                    && XRAPSTATE_Disengaged == pVessel->GetStandardAP(XRSAP_Retrograde)
                    && XRAPSTATE_Disengaged == pVessel->GetStandardAP(XRSAP_Normal)
                    && XRAPSTATE_Disengaged == pVessel->GetStandardAP(XRSAP_AntiNormal)
                ? XRAPSTATE_Engaged 
                : XRAPSTATE_Disengaged);

    return result.str();
}

string setStandardAP(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (5 > tokens.size()) return "ERR02";
    if (tokens.at(3).empty()) return "ERR02";
    if (tokens.at(4).empty()) return "ERR02";

    int id;
    bool on;
    bool success = CommandParser::intFromString(tokens.at(3), id);
    if (!success) 
    {
        return "ERR04";
    }

    success = CommandParser::boolFromString(tokens.at(4), on);
    if (!success) 
    {
        return "ERR04";
    }

    char buffer[8];
    sprintf_s(buffer, "%d", pVessel->SetStandardAP((XRStdAutopilot) id, on));

    return buffer;
}

string getAttitudeHoldAP(XRVesselCtrl* pVessel) {
    XRAttitudeHoldState state;
    stringstream result;

    pVessel->GetAttitudeHoldAP(state);
    result << (state.on ? "1" : "0") << ",";
    result << (unsigned int) state.mode << ",";
    result << state.TargetPitch << "," << state.TargetBank;

    return result.str();
}

/*
 * Returns XRAutoPilotState
 */
string setAttitudeHoldAP(XRVesselCtrl* pVessel, vector<string> tokens) {
    XRAttitudeHoldState state;
    pVessel->GetAttitudeHoldAP(state);
    bool converted = true;
    int value = -1;

    if (!tokens.at(3).empty()) {
        converted = CommandParser::boolFromString(tokens.at(3), state.on);
    }
    if (converted && tokens.size() >= 5 && !tokens.at(4).empty()) {
        converted = CommandParser::intFromString(tokens.at(4), value);
        if (converted)
        {
            state.mode = (XRAttitudeHoldMode) value;
        }
    }
    if (converted && tokens.size() >= 6 && !tokens.at(5).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(5), state.TargetPitch);
    }
    if (converted && tokens.size() >= 7 && !tokens.at(6).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(6), state.TargetBank);
    }

    if (!converted) 
    {
        return "ERR04";
    }
    stringstream result;
    result << (unsigned int) pVessel->SetAttitudeHoldAP(state);

    return result.str();
}

/*
 * Returns XRAutoPilotState
 */
string toggleLVLHAP(XRVesselCtrl* pVessel) {
    XRAttitudeHoldState state;
    pVessel->GetAttitudeHoldAP(state);

    if (state.on) {
        state.on = false;
    } else {
        state.on = true;
        state.TargetBank = 0.0;
        state.TargetPitch = 0.0;
    }
    stringstream result;
    result << (unsigned int) pVessel->SetAttitudeHoldAP(state);

    return result.str();
}

/*
 * Returns XRAutoPilotState
 */
string toggleCurrentAttitudeHoldAP(XRVesselCtrl* pVessel) {
    XRAttitudeHoldState state;
    pVessel->GetAttitudeHoldAP(state);

    if (state.on) {
        state.on = false;
    } else {
        state.on = true;
        state.TargetBank = pVessel->GetBank();
        if (XRAH_HoldAOA == state.mode) {
            state.TargetPitch = pVessel->GetAOA();
                    } else {
            state.TargetPitch = pVessel->GetPitch();
        }
    }
    stringstream result;
    result << (unsigned int) pVessel->SetAttitudeHoldAP(state);

    return result.str();
}

/*
 * Returns XRAutoPilotState
 */
string toggleAttitudeHoldAP(XRVesselCtrl* pVessel) {
    XRAttitudeHoldState state;
    pVessel->GetAttitudeHoldAP(state);

    if (state.on) {
        state.on = false;
    } else {
        state.on = true;
    }
    stringstream result;
    result << (unsigned int) pVessel->SetAttitudeHoldAP(state);

    return result.str();
}

string getDescentHoldAP(XRVesselCtrl* pVessel) {
    XRDescentHoldState state;
    stringstream result;

    pVessel->GetDescentHoldAP(state);
    result << (state.on ? "1" : "0") << ",";
    result << state.TargetDescentRate << ",";
    result << (state.AutoLandMode ? "1" : "0");

    return result.str();
}

string setDescentHoldAP(XRVesselCtrl* pVessel, vector<string> tokens) {
    XRDescentHoldState state;
    bool converted = false;

    pVessel->GetDescentHoldAP(state);
    if (tokens.size() >= 4 && !tokens.at(3).empty()) {
        converted = CommandParser::boolFromString(tokens.at(3), state.on);
    }
    if (converted && tokens.size() >= 5 && !tokens.at(4).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(4), state.TargetDescentRate);
    }
    if (converted && tokens.size() >= 6 && !tokens.at(5).empty()) {
        converted = CommandParser::boolFromString(tokens.at(5), state.AutoLandMode);
    }

    if (!converted)
    {
        return "ERR04";
    }

    stringstream result;
    result << (unsigned int) pVessel->SetDescentHoldAP(state);

    return result.str();
}

string getAirspeedHoldAP(XRVesselCtrl* pVessel) {
    XRAirspeedHoldState state;
    stringstream result;

    pVessel->GetAirspeedHoldAP(state);
    result << (state.on ? "1" : "0") << ",";
    result << state.TargetAirspeed;

    return result.str();
}

string setAirspeedHoldAP(XRVesselCtrl* pVessel, vector<string> tokens) {
    XRAirspeedHoldState state;
    bool converted = false;

    pVessel->GetAirspeedHoldAP(state);
    if (tokens.size() >= 4 && !tokens.at(3).empty()) {
        converted = CommandParser::boolFromString(tokens.at(3), state.on);
    }
    if (converted && tokens.size() >= 5 && !tokens.at(4).empty()) {
        converted = CommandParser::doubleFromString(tokens.at(4), state.TargetAirspeed);
    }

    if (!converted)
    {
        return "ERR04";
    }

    stringstream result;
    result << (unsigned int) pVessel->SetAirspeedHoldAP(state);

    return result.str();
}

string getSystemStatus(XRVesselCtrl* pVessel) {
    stringstream msg;
    XRSystemStatusRead status;
    pVessel->GetXRSystemStatus(status);

    msg << status.MasterWarning << ",";
    msg << status.LeftWing << ",";
    msg << status.RightWing << ",";
    msg << status.LeftMainEngine << ",";
    msg << status.RightMainEngine << ",";
    msg << status.LeftSCRAMEngine << ",";
    msg << status.RightSCRAMEngine << ",";
    msg << status.ForeHoverEngine << ",";
    msg << status.AftHoverEngine << ",";
    msg << status.LeftRetroEngine << ",";
    msg << status.RightRetroEngine << ",";
    msg << status.ForwardLowerRCS << ",";
    msg << status.AftUpperRCS << ",";
    msg << status.ForwardUpperRCS << ",";
    msg << status.AftLowerRCS << ",";
    msg << status.ForwardStarboardRCS << ",";
    msg << status.AftPortRCS << ",";
    msg << status.ForwardPortRCS << ",";
    msg << status.AftStarboardRCS << ",";
    msg << status.OutboardUpperPortRCS << ",";
    msg << status.OutboardLowerStarboardRCS << ",";
    msg << status.OutboardUpperStarboardRCS << ",";
    msg << status.OutboardLowerPortRCS << ",";
    msg << status.AftRCS << ",";
    msg << status.ForwardRCS << ",";
    msg << (int) status.LeftAileron << ",";
    msg << (int) status.RightAileron << ",";
    msg << (int) status.LandingGear << ",";
    msg << (int) status.DockingPort << ",";
    msg << (int) status.RetroDoors << ",";
    msg << (int) status.TopHatch << ",";
    msg << (int) status.Radiator << ",";
    msg << (int) status.Speedbrake << ",";
    msg << (int) status.PayloadBayDoors << ",";
    msg << (int) status.CrewElevator << ",";
    msg << (int) status.HullTemperatureWarning << ",";
    msg << (int) status.RCSFuelWarning << ",";
    msg << (int) status.APUFuelWarning << ",";
    msg << (int) status.LOXWarning << ",";
    msg << (int) status.DynamicPressureWarning << ",";
    msg << (int) status.CoolantWarning << ",";
    msg << (int) status.MasterWarning << ",";
    msg << status.RCSFuelLevel << ",";
    msg << status.RCSMaxFuelMass << ",";
    msg << status.APUFuelLevel << ",";
    msg << status.APUMaxFuelMass << ",";
    msg << status.LOXLevel << ",";
    msg << status.LOXMaxMass << ",";
    msg << status.BayLOXMass;

	
	if ( pVessel->GetCtrlAPIVersion() >= 30.0f ) {
		// Fields added in API version 3.0
		msg << (int) status.MWSAlarmState;
		msg << (int) status.COGAutoMode;
		msg << (int) status.InternalSystemsFailure;
		msg << status.CenterOfGravity;
		msg << status.CabinO2Level;
		msg << status.CoolantTemp;
		msg << status.NoseconeTemp;
		msg << status.LeftWingTemp;
		msg << status.RightWingTemp;
		msg << status.CockpitTemp;
		msg << status.TopHullTemp;
		msg << status.MaxSafeNoseconeTemp;
		msg << status.MaxSafeWingTemp;
		msg << status.MaxSafeCockpitTemp;
		msg << status.MaxSafeTopHullTemp;
	}
    return msg.str();
}

string getMWS(XRVesselCtrl* pVessel) {
    string msg = "0000000000000000000000000000000000000000000";
    string on = "1";
    XRSystemStatusRead status;
    pVessel->GetXRSystemStatus(status);

    if (XRDMG_offline == status.MasterWarning) {
        msg.replace(0, 1, on);
    }
    if (status.LeftWing < 1.0) {
        msg.replace(1, 1, on);
    }
    if (status.RightWing < 1.0) {
        msg.replace(2, 1, on);
    }
    if (status.LeftMainEngine < 1.0) {
        msg.replace(3, 1, on);
    }
    if (status.RightMainEngine < 1.0) {
        msg.replace(4, 1, on);
    }
    if (status.LeftSCRAMEngine < 1.0) {
        msg.replace(5, 1, on);
    }
    if (status.RightSCRAMEngine < 1.0) {
        msg.replace(6, 1, on);
    }
    if (status.ForeHoverEngine < 1.0) {
        msg.replace(7, 1, on);
    }
    if (status.AftHoverEngine < 1.0) {
        msg.replace(8, 1, on);
    }
    if (status.LeftRetroEngine < 1.0) {
        msg.replace(9, 1, on);
    }
    if (status.RightRetroEngine < 1.0) {
        msg.replace(10, 1, on);
    }
    if (status.ForwardLowerRCS < 1.0) {
        msg.replace(11, 1, on);
    }
    if (status.AftUpperRCS < 1.0) {
        msg.replace(12, 1, on);
    }
    if (status.ForwardUpperRCS < 1.0) {
        msg.replace(13, 1, on);
    }
    if (status.AftLowerRCS < 1.0) {
        msg.replace(14, 1, on);
    }
    if (status.ForwardStarboardRCS < 1.0) {
        msg.replace(15, 1, on);
    }
    if (status.AftPortRCS < 1.0) {
        msg.replace(16, 1, on);
    }
    if (status.ForwardPortRCS < 1.0) {
        msg.replace(17, 1, on);
    }
    if (status.AftStarboardRCS < 1.0) {
        msg.replace(18, 1, on);
    }
    if (status.OutboardUpperPortRCS < 1.0) {
        msg.replace(19, 1, on);
    }
    if (status.OutboardLowerStarboardRCS < 1.0) {
        msg.replace(20, 1, on);
    }
    if (status.OutboardUpperStarboardRCS < 1.0) {
        msg.replace(21, 1, on);
    }
    if (status.OutboardLowerPortRCS < 1.0) {
        msg.replace(22, 1, on);
    }
    if (status.AftRCS < 1.0) {
        msg.replace(23, 1, on);
    }
    if (status.ForwardRCS < 1.0) {
        msg.replace(24, 1, on);
    }
    if (XRDMG_offline == status.LeftAileron) {
        msg.replace(25, 1, on);
    }
    if (XRDMG_offline == status.RightAileron) {
        msg.replace(26, 1, on);
    }
    if (XRDMG_offline == status.LandingGear) {
        msg.replace(27, 1, on);
    }
    if (XRDMG_offline == status.DockingPort) {
        msg.replace(28, 1, on);
    }
    if (XRDMG_offline == status.RetroDoors) {
        msg.replace(29, 1, on);
    }
    if (XRDMG_offline == status.TopHatch) {
        msg.replace(30, 1, on);
    }
    if (XRDMG_offline == status.Radiator) {
        msg.replace(31, 1, on);
    }
    if (XRDMG_offline == status.Speedbrake) {
        msg.replace(32, 1, on);
    }
    if (XRDMG_offline == status.PayloadBayDoors) {
        msg.replace(33, 1, on);
    }
    if (XRDMG_offline == status.CrewElevator) {
        msg.replace(34, 1, on);
    }
    if (XRW_warningActive == status.HullTemperatureWarning) {
        msg.replace(35, 1, on);
    }
    if (XRW_warningActive == status.MainFuelWarning) {
        msg.replace(36, 1, on);
    }
    if (XRW_warningActive == status.RCSFuelWarning) {
        msg.replace(37, 1, on);
    }
    if (XRW_warningActive == status.APUFuelWarning) {
        msg.replace(38, 1, on);
    }
    // Uncomment below if scram fuel warning is added.
    //    if (XRW_warningActive == status.ScramFuelWarning) {
    //        msg.replace(39, 1, on);
    //    }
    if (XRW_warningActive == status.LOXWarning) {
        msg.replace(40, 1, on);
    }
    if (XRW_warningActive == status.DynamicPressureWarning) {
        msg.replace(41, 1, on);
    }
    if (XRW_warningActive == status.CoolantWarning) {
        msg.replace(42, 1, on);
    }

    return msg;
}

string resetMWS(XRVesselCtrl* pVessel) {
    bool result = pVessel->ResetMasterWarningAlarm();

    return (result ? "1" : "0");
}

string getExteriorLights(XRVesselCtrl* pVessel) {
    stringstream status;
    status << pVessel->GetExteriorLight(XRL_Nav);
    status << pVessel->GetExteriorLight(XRL_Beacon);
    status << pVessel->GetExteriorLight(XRL_Strobe);

    return status.str();
}

string setExteriorLight(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (5 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";
    if (tokens.at(4).empty()) return "ERR03";

    bool success = true;
    int id;
    bool on;
    success = CommandParser::intFromString(tokens.at(3), id);
    if (success)
    {
        success = CommandParser::boolFromString(tokens.at(4), on);
    }

    if (!success)
    {
        return "ERR04";
    }

    bool result = pVessel->SetExteriorLight((XRLight) id, on);

    return (result ? "1" : "0");
}

string getHudModes(XRVesselCtrl* pVessel) {
    stringstream result;
    if (oapiGetVesselInterface(oapiGetFocusObject()) == pVessel) {
        result << oapiGetHUDMode();
    }
    result << ",";
    result << pVessel->GetSecondaryHUDMode() << ",";
    result << pVessel->GetTertiaryHUDState();

    return result.str();
}

string setHudMode(XRVesselCtrl* pVessel, vector<string> tokens) {
    if (5 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";
    if (tokens.at(4).empty()) return "ERR03";

    bool converted = false;
    int id = -1;
    bool mode = false;
    converted = CommandParser::intFromString(tokens.at(3), id);
    if (converted)
    {
        converted = CommandParser::boolFromString(tokens.at(4), mode);
    }
    if (!converted)
    {
        return "ERR04";
    }

    bool result = false;
    switch (id) {
        case 1:
            if (oapiGetVesselInterface(oapiGetFocusObject()) == pVessel) {
                result = oapiSetHUDMode(mode);
            }
            break;
        case 2:
            result = pVessel->SetSecondaryHUDMode(mode);
            break;
        case 3:
            result = pVessel->SetTertiaryHUDState(mode);
            break;
        default:
            break;
    }

    return result ? "1" : "0";
}

string testMWS(XRVesselCtrl* pVessel)
{
    XRSystemStatusRead status;
    status.MWSLightState = true;
    pVessel->GetXRSystemStatus(status);

    return "OK";
}

string getCenterOfGravity(XRVesselCtrl* pVessel)
{
    char buffer[32];
    sprintf_s(buffer,"%f", pVessel->GetCenterOfGravity());

    return buffer;
}

string changeCenterOfGravity(XRVesselCtrl* pVessel, vector<string> tokens)
{
    if (4 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";

    bool converted;
    double value;
    converted = CommandParser::doubleFromString(tokens.at(3), value);

    return CommandParser::boolToString(pVessel->ShiftCenterOfGravity(value));
}

string getRCSDockingMode(XRVesselCtrl* pVessel)
{
    return CommandParser::boolToString(pVessel->IsRCSDockingMode());
}

string setRCSDockingMode(XRVesselCtrl* pVessel, vector<string> tokens)
{
    if (4 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";

    bool state;
    bool converted = false;
    bool result = false;
    converted = CommandParser::boolFromString(tokens.at(3), state);

    if (converted)
    {
        result = pVessel->SetRCSDockingMode(state);
    }

    return CommandParser::boolToString(result);
}

string getElevatorEVAPortActive(XRVesselCtrl* pVessel)
{
    return CommandParser::boolToString(pVessel->IsElevatorEVAPortActive());
}

string setElevatorEVAPortActive(XRVesselCtrl* pVessel, vector<string> tokens)
{
    if (4 > tokens.size()) return "ERR03";
    if (tokens.at(3).empty()) return "ERR03";

    bool state;
    bool converted = false;
    bool result = false;
    converted = CommandParser::boolFromString(tokens.at(3), state);

    if (converted)
    {
        result = pVessel->SetElevatorEVAPortActive(state);
    }

    return CommandParser::boolToString(result);
}

string getStatusMessages(XRVesselCtrl* pVessel, vector<string> tokens)
{
    int idx = 0;
    int startIdx = 0;
    stringstream msg;
    int nbrRequested = 7;
    int nbrMessages;
    bool converted = true;
    bool prependDelim = false;

    if (tokens.size() ==4 && !tokens.at(3).empty())
    {
        converted = CommandParser::intFromString(tokens.at(3), nbrRequested);
    }

    if (!converted)
    {
        return "ERR04";
    }

    char* messages = new char[50 * nbrRequested + 1];
    nbrMessages = pVessel->GetStatusScreenText(messages, nbrRequested);
//    nbrMessages = messages.size();
    if (0 == nbrRequested || nbrMessages < nbrRequested)
    {
        nbrRequested = nbrMessages;
    }

    startIdx = nbrMessages - nbrRequested;
    if (startIdx < 0) startIdx = 0;
//    for (idx = startIdx; idx < nbrMessages; idx++) 
//    {
//        if (prependDelim)
//        {
//            msg << ";";
//        }
//        else 
//        {
//            prependDelim = true;
//        }
//        msg << messages.at(idx);
//    }
    msg << messages;

    return msg.str();
}

string getEngineState(XRVesselCtrl* pVessel, XREngineID id) {
    XREngineStateRead state;
    stringstream out;
    pVessel->GetEngineState(id, state);
    out << id << ",";
    out << state.ThrottleLevel << ",";
    out << state.GimbalX << ",";
    out << state.GimbalY << ",";
    out << state.Balance << ",";
    out << state.CenteringModeX << ",";
    out << state.CenteringModeY << ",";
    out << state.CenteringModeBalance << ",";
    out << state.AutoMode << ",";
    out << state.DivergentMode << ",";
    out << state.TSFC << ",";
    out << state.FlowRate << ",";
    out << state.Thrust << ",";
    out << state.FuelLevel << ",";
    out << state.MaxFuelMass << ",";
    out << state.DiffuserTemp << ",";
    out << state.BurnerTemp << ",";
    out << state.ExhaustTemp << ",";
    out << state.BayFuelMass;

    return out.str();
}

string getDoorState(XRVesselCtrl* pVessel, XRDoorID id) {
    stringstream out;
    out << (unsigned int) pVessel->GetDoorState(id);

    return out.str();
}

string getDoorStateAndPosition(XRVesselCtrl* pVessel, XRDoorID id) {
    stringstream out;
    double pos = 0;
    unsigned int state = pVessel->GetDoorState(id, &pos);
    out << state << "," << pos;

    return out.str();
}
