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
* @Author Brandon Bolling
*/

#include "OrbConnect.h"
#include "VesselCommandParser.h"
#include "XRCtlCommandParser.h"
#include <algorithm>

string VesselCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    VESSEL2* pVessel = 0;
    vector<string> tokens = stringSplit(command,":");
    string cmd = uCase(tokens.at(0));

    if ("FOCUS" == cmd)
    {
        return doFocus(lParam, command, tokens);
    }

    cmd = uCase(tokens.at(1));
    if ("COUNT" == cmd)
    {
        return getVesselCount();
    }
    else if ("COCKPITMODE" == cmd)
    {
        return getCockpitMode();
    }

    string ldMsg = loadVessel(&pVessel, tokens);
    if (0 == pVessel) {
        return ldMsg;
    }

    cmd = uCase(tokens.at(2));

    if ("NAME" == cmd)
    {
        message = pVessel->GetName();
    }
    else if ("CLASSNAME" == cmd)
    {
        message = getClassName(pVessel);
    }
    else if ("VERSION" == cmd)
    {
        message = getVersion(pVessel);
    }
    else if ("STATUS" == cmd)
    {
        message = getStatus(pVessel);
    }
    else if ("STATUS2" == cmd)
    {
        message = getStatus2(pVessel, tokens);
    }
    else if ("MASS" == cmd)
    {
        message = getMass(pVessel);
    }
    else if ("EMPTYMASS" == cmd)
    {
        message = getEmptyMass(pVessel);
    }
    else if ("PROPMASS" == cmd)
    {
        message = getPropMass(pVessel);
    }
    else if ("PROPFLOWRATE" == cmd)
    {
        message = getPropFlowRate(pVessel);
    }
    else if ("DFLTFUELMASS" == cmd)
    {
        message = getFuelMass(pVessel);
    }
    else if ("DFLTMAXFUELMASS" == cmd)
    {
        message = getMaxFuelMass(pVessel);
    }
    else if ("DFLTFUELFLOWRATE" == cmd)
    {
        message = getFuelFlowRate(pVessel);
    }
    else if ("ALT" == cmd)
    {
        message = getAltitude(pVessel);
    }
    else if ("NAVMODESTATES" == cmd && tokens.size() == 3)
    {
        message = getNavModeStates(pVessel);
    }
    else if ("SETNAVMODE" == cmd && tokens.size() == 5)
    {
        message = setNavMode(pVessel, tokens);
    }
    else if ("TOGGLENAVMODE" == cmd && tokens.size() == 4)
    {
        message = toggleNavModeState(pVessel, tokens);
    }
    else if ("APSETTINGS" == cmd)
    {
        message = getAPHoldSettings(pVessel);
    }
    else if ("SETATTITUDEMODE" == cmd && tokens.size() == 4)
    {
        message = setAttitudeMode(pVessel, tokens);
    }
    else if ("TOGGLEATTITUDEMODE" == cmd)
    {
        message = toggleAttitudeMode(pVessel);
    }
    else if ("ADCTRLMODE" == cmd)
    {
        message = getADCtrlMode(pVessel);
    }
    else if ("SETADCTRLMODE" == cmd && tokens.size() == 4)
    {
        message = setADCtrlMode(pVessel, tokens);
    }
    else if ("ENGINEGRPLEVELS" == cmd)
    {
        message = getEngineLevels(pVessel);
    }
    else if ("SETENGINEGRPLEVEL" == cmd && tokens.size() == 5)
    {
        message = setEngineLevel(pVessel, tokens);
    }
    else if ("CHGENGINEGRPLEVEL" == cmd && tokens.size() == 5)
    {
        message = changeEngineLevel(pVessel, tokens);
    }
    else if ("FLTSTATUS" == cmd)
    {
        message = getFlightStatus(pVessel);
    }
    else if ("AIRSPD" == cmd)
    {
        message = getAirspeed(pVessel);
    }
    else if ("TRUSPD" == cmd)
    {
        message = getAirspeed(pVessel);
    }
    else if ("SHIPAIRSPDVECTOR" == cmd)
    {
        message = getShipAirspeedVector(pVessel);
    }
    else if ("HORIZONAIRSPDVECTOR" == cmd)
    {
        message = getHorizonAirspeedVector(pVessel);
    }
    else if ("ATTITUDE" == cmd)
    {
        message = getAttitude(pVessel);
    }
    else if ("ATMCONDITIONS" == cmd)
    {
        message = getAtmConditions(pVessel);
    }
    else if ("ACCEL" == cmd)
    {
        message = getAccel(pVessel);
    }
    else if ("VACCEL" == cmd)
    {
        message = getVAccel(pVessel);
    }
    else if ("INDSPD" == cmd)
    {
        message = getIndSpd(pVessel);
    }
    else if ("ORBSPD" == cmd)
    {
        message = getOrbSpd(pVessel);
    }
    else if ("GNDSPD" == cmd)
    {
        message = getGndSpd(pVessel);
    }
    else if ("EQUSPD" == cmd)
    {
        message = getEquSpd(pVessel);
    }
    else if ("RADIOS" == cmd)
    {
        message = getRadios(pVessel);
    }
    else if ("SETRADIO" == cmd)
    {
        message = setRadio(pVessel, tokens);
    }
    else if ("SETXPDR" == cmd)
    {
        message = setTransponder(pVessel, tokens);
    }
    else if ("SETIDS" == cmd)
    {
        message = setIDS(pVessel, tokens);
    }
    else if ("SENDBUFFEREDKEY" == cmd)
    {
        message = SendBufferedKey(pVessel, tokens);
    }

    return message;
}

string VesselCommandParser::doFocus(long lParam, string command, vector<string> tokens) {
    string message = "ERR02";
    string cmd = uCase(tokens.at(1));

    if ("NAME" == cmd)
    {
        message = getFocusObjectName();
    }
    else if ("SETBYINDEX" == cmd && tokens.size() == 3 && !tokens.at(2).empty())
    {
        message = setFocusByIndex(tokens);
    }
    else if ("SETBYNAME" == cmd && tokens.size() == 3 && !tokens.at(2).empty())
    {
        message = setFocusByName(tokens);
    }
    else if ("ALT" == cmd)
    {
        message = getFocusAltitude();
    }
    else if ("PITCH" == cmd)
    {
        message = getFocusPitch();
    }
    else if ("BANK" == cmd)
    {
        message = getFocusBank();
    }
    else if ("HEADING" == cmd)
    {
        message = getFocusHeading();
    }
    else if ("EQUPOS" == cmd)
    {
        message = getFocusEquPos();
    }
    else if ("AIRSPD" == cmd)
    {
        message = getFocusAirspeed();
    }
    else if ("AIRSPDVECTOR" == cmd)
    {
        message = getFocusAirspeedVector();
    }
    else if ("SHIPAIRSPDVECTOR" == cmd)
    {
        message = getFocusShipAirspeedVector();
    }
    else if ("ATMDENSITY" == cmd)
    {
        message = getFocusAtmPressureDensity();
    }
    else if ("ENGINESTATUS" == cmd)
    {
        message = getFocusEngineStatus();
    }
    else if ("ATTITUDEMODE" == cmd)
    {
        message = getFocusAttitudeMode();
    }
    else if ("TOGGLEATTITUDEMODE" == cmd)
    {
        message = toggleFocusAttitudeMode();
    }
    else if ("SETATTITUDEMODE" == cmd && tokens.size() == 3 && !tokens.at(2).empty())
    {
        message = setFocusAttitudeMode(tokens);
    }

    return message;
}

string VesselCommandParser::loadVessel(VESSEL2** pVessel, vector<string> tokens) {
    string msg = "ERR09";
    OBJHANDLE objH = NULL;
    if ("FOCUS" == uCase(tokens.at(1)))
    {
        objH = oapiGetFocusObject();
    }
    else
    {
        char Buffer[256];
        strcpy_s(Buffer,tokens.at(1).c_str());
        objH = oapiGetVesselByName(Buffer);
        if (objH == NULL)
        {
            int idx = -1;
            bool converted = CommandParser::intFromString(tokens.at(1), idx);
            if (converted && idx < (int)oapiGetVesselCount() && idx >= 0) {
                objH = oapiGetVesselByIndex(idx);
            }
        }
    }
    if (objH == NULL)
    {
        msg = "ERR10";
    } else if (!oapiIsVessel(objH) )
    {
        msg = "ERR05";
    } else 
    {
        *pVessel = (VESSEL2*) oapiGetVesselInterface(objH);
    }

    return msg;
}

OBJHANDLE VesselCommandParser::getReferenceObject(string objId) {
    OBJHANDLE objH = oapiGetObjectByName(const_cast<char*> (objId.c_str()));
    if (objH == NULL)
    {
        int idx;
        bool converted = CommandParser::intFromString(objId, idx);
        if (converted && idx < (int)oapiGetObjectCount() && idx >= 0)
        {
            objH = oapiGetObjectByIndex(idx);
        }
    }

    return objH;
}

string VesselCommandParser::getVesselCount() {
    char buffer[8];
    sprintf_s(buffer, "%d", oapiGetVesselCount());

    return buffer;
}

string VesselCommandParser::getFocusObjectName() {
    char buffer[256];
    oapiGetObjectName(oapiGetFocusObject(),buffer,256);

    return buffer;
}

string VesselCommandParser::setFocusByIndex(vector<string> tokens) {
    string msg = "Index out of bounds";

    int idx = -1;
    bool converted = CommandParser::intFromString(tokens.at(2), idx);
    OBJHANDLE vessel = NULL;
    if (converted && idx < (int)oapiGetVesselCount() && idx >= 0)
    {
        vessel = oapiGetVesselByIndex(idx);
        if (NULL != vessel)
        {
            oapiSetFocusObject(vessel);
            msg = "OK";
        }
        else
        {
            msg = "ERR04";
        }
    }

    return msg;
}

string VesselCommandParser::setFocusByName(vector<string> tokens) {
    string msg = "ERR05";
    OBJHANDLE hVessel = oapiGetVesselByName(const_cast<char*> (tokens.at(2).c_str()));
    if (hVessel != NULL && oapiIsVessel(hVessel))
    {
        oapiSetFocusObject(hVessel);
        msg = "OK";
    }

    return msg;
}


string VesselCommandParser::getCockpitMode() {
    char buffer[8];
    sprintf_s(buffer,"%d", oapiCockpitMode());

    return buffer;
}

string VesselCommandParser::getFocusAltitude() {
    double alt;
    oapiGetFocusAltitude(&alt);
    char buffer[256];
    sprintf_s(buffer, "%f", alt);

    return buffer;
}

string VesselCommandParser::getFocusPitch() {
    double pitch;
    oapiGetFocusPitch(&pitch);
    char buffer[256];
    sprintf_s(buffer, "%f", pitch);

    return buffer;
}

string VesselCommandParser::getFocusBank() {
    double bank;
    oapiGetFocusBank(&bank);
    char buffer[256];
    sprintf_s(buffer, "%f", bank);

    return buffer;
}

string VesselCommandParser::getFocusHeading() {
    double val;
    oapiGetFocusHeading(&val);
    char buffer[256];
    sprintf_s(buffer, "%f", val);

    return buffer;
}

string VesselCommandParser::getFocusEquPos() {
    VECTOR3 pos;
    oapiGetFocusEquPos(&pos.x,&pos.y,&pos.z);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f",pos.x,pos.y,pos.z);

    return buffer;
} 

string VesselCommandParser::getFocusAirspeed() {
    OBJHANDLE objH = oapiGetFocusObject();
    if (!oapiIsVessel(objH)) {
        return "ERR05";
    }
    VESSEL* pVessel = oapiGetVesselInterface(objH);
    double staticPressure = pVessel->GetAtmPressure();
    if (staticPressure  < 0.0001) {    // Not in an atmosphere
        return "ERR12";
    }

    double val;
    oapiGetFocusAirspeed(&val);
    char buffer[256];
    sprintf_s(buffer, "%f", val);

    return buffer;
}

string VesselCommandParser::getFocusAirspeedVector() {
    VECTOR3 pos;
    oapiGetFocusAirspeedVector(&pos);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f",pos.x,pos.y,pos.z);

    return buffer;
}

string VesselCommandParser::getFocusShipAirspeedVector() {
    VECTOR3 pos;
    oapiGetFocusShipAirspeedVector(&pos);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f",pos.x,pos.y,pos.z);

    return buffer;
}

string VesselCommandParser::getFocusAtmPressureDensity() {
    VECTOR3 pos;
    oapiGetFocusAtmPressureDensity(&pos.x,&pos.y);
    char buffer[256];
    sprintf_s(buffer,"%f,%f",pos.x,pos.y);

    return buffer;
}

string VesselCommandParser::getFocusEngineStatus() {
    ENGINESTATUS eng;
    oapiGetFocusEngineStatus(&eng);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%d",eng.main,eng.hover,eng.attmode);

    return buffer;
}

string VesselCommandParser::getFocusAttitudeMode() {
    char buffer[2];
    sprintf_s(buffer,"%d",oapiGetFocusAttitudeMode());

    return buffer;
}

string VesselCommandParser::setFocusAttitudeMode(vector<string> tokens) {
    string msg = "ERR04";
    int mode;
    bool converted = CommandParser::intFromString(tokens.at(2), mode);
    if (converted && mode >= 0 && mode <= 2)
    {
        oapiSetFocusAttitudeMode(mode);
        msg = "OK";
    }

    return msg;
}

string VesselCommandParser::toggleFocusAttitudeMode() {
    char buffer[2];
    sprintf_s(buffer,"%d", oapiToggleFocusAttitudeMode());

    return buffer;
}

string VesselCommandParser::getVersion(VESSEL2* pVessel) {
    char buffer[2];
    sprintf_s(buffer,"%d",pVessel->Version());

    return buffer;
}

string VesselCommandParser::getName(VESSEL2* pVessel) {
    return pVessel->GetName();
}

string VesselCommandParser::getClassName(VESSEL2* pVessel) {
    return pVessel->GetClassName();
}

string VesselCommandParser::getMass(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetMass());

    return buffer;
}

string VesselCommandParser::getEmptyMass(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetEmptyMass());

    return buffer;
}

string VesselCommandParser::getPropMass(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetTotalPropellantMass());

    return buffer;
}

string VesselCommandParser::getPropFlowRate(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetTotalPropellantFlowrate());

    return buffer;
}

string VesselCommandParser::getFuelMass(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetFuelMass());

    return buffer;
}

string VesselCommandParser::getMaxFuelMass(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetMaxFuelMass());

    return buffer;
}

string VesselCommandParser::getFuelFlowRate(VESSEL2* pVessel) {
    char buffer[32];
    sprintf_s(buffer,"%f",pVessel->GetFuelRate());

    return buffer;
}

string VesselCommandParser::getAltitude(VESSEL2* pVessel) {
    double alt = pVessel->GetAltitude();
    char buffer[256];
    sprintf_s(buffer, "%f", alt);

    return buffer;
}

string VesselCommandParser::getNavModeStates(VESSEL2* pVessel) {

    MessageStream msg;
    XRVesselCtrl* pXRVessel;
    XRAttitudeHoldState attState;
    XRDescentHoldState dscntState;
    XRAirspeedHoldState aspdState;
    int pos = 0;
    for (; pos < 8; pos++)
    {
        bool state = pVessel->GetNavmodeState(pos);
        msg << (state ? "1" : "0");
    }

    if (isXRVessel(pVessel)) {
        pXRVessel = (XRVesselCtrl*) pVessel;
        if (0 == pXRVessel) {
            return "ERR07";
        }
        pXRVessel->GetAttitudeHoldAP(attState);
        msg << (attState.on ? "1" : "0");
        pXRVessel->GetDescentHoldAP(dscntState);
        msg << (dscntState.on ? "1" : "0");
        pXRVessel->GetAirspeedHoldAP(aspdState);
        msg << (aspdState.on ? "1" : "0");
    } else {
        msg << "000";
    }

    return msg.str();
}

string VesselCommandParser::setNavMode(VESSEL2* pVessel, vector<string> tokens) {
    string msg = "ERR04";
    int mode = -1;
    bool newState;
    bool converted = CommandParser::intFromString(tokens.at(3), mode);
    if (converted)
    {
        converted = CommandParser::boolFromString(tokens.at(4), newState);
    }
    if ( converted && mode >= 0 && mode <= 7 )
    {
        bool state = (newState 
            ? pVessel->ActivateNavmode(mode) 
            : pVessel->DeactivateNavmode(mode));
        msg = (state ? "1" : "0");
    }

    return msg;
}

string VesselCommandParser::toggleNavModeState(VESSEL2* pVessel, vector<string> tokens) {
    string msg = "ERR04";
    int mode = -1;
    bool converted = CommandParser::intFromString(tokens.at(3), mode);
    if ( converted && mode >= 0 && mode <= 7 )
    {
        bool state = pVessel->ToggleNavmode(mode);
        msg = (state ? "1" : "0");
    }

    return msg;
}

int VesselCommandParser::findVesselIndex(VESSEL2* pVessel) {
    MessageStream result;
    VESSEL* ship = NULL;
    int pos = -1;
    for (int idx = 0; idx < (int) oapiGetVesselCount; idx++) {
        ship = oapiGetVesselInterface(oapiGetVesselByIndex(idx));
        if (strcmp(ship->GetName(), pVessel->GetName()) == 0) {
            pos = idx;
            break;
        }
    }

    return pos;
}

string VesselCommandParser::getAPHoldSettings(VESSEL2* pVessel) {
    XRAttitudeHoldState attState;
    XRAirspeedHoldState spdState;
    XRDescentHoldState dscntState;
    XRVesselCtrl* pXRVessel = NULL;
    MessageStream result;
    int idx = findVesselIndex(pVessel);
    if (idx < 0) return "ERR10";

    if (isXRVessel(pVessel)) {
        pXRVessel = (XRVesselCtrl*) pVessel;
        if (0 == pXRVessel) {
            return "ERR07";
        }
        pXRVessel->GetAttitudeHoldAP(attState);
        pXRVessel->GetAirspeedHoldAP(spdState);
        pXRVessel->GetDescentHoldAP(dscntState);
    }
    // do bank
    if (pXRVessel == NULL) {
        result << (vesselAxesSet[3 * idx + AP_BANK] ? "1" : "0") << ",";
        result << vesselAxesValues[3 * idx + AP_BANK];
    } else {
        result << ((attState.on) ? "1" : "0") << ",";
        result << ((attState.on) ? attState.TargetBank : 0);
    }
    result << ",";
    // do pitch
    if (pXRVessel == NULL) {
        result << (vesselAxesSet[3 * idx + AP_PITCH] ? "1" : "0") << ",";
        result << vesselAxesValues[3 * idx + AP_PITCH];
    } else {
        result << ((attState.on && attState.mode == 0) ? "1" : "0") << ",";
        result << ((attState.on && attState.mode == 0) ? attState.TargetPitch : 0);
    }
    result << ",";
    // do AoA
    if (pXRVessel == NULL) {
        result << "0,0";
    } else {
        result << ((attState.on && attState.mode == 1) ? "1" : "0") << ",";
        result << ((attState.on && attState.mode == 1) ? attState.TargetPitch : 0);
    }
    result << ",";
    // do altitude
    if (pXRVessel == NULL) {
        result << (vesselAxesSet[3 * idx + AP_ALT] ? "1" : "0") << ",";
        result << vesselAxesValues[3 * idx + AP_ALT];
    } else {
        result << "0,0";
    }
    result << ",";
    // do airspeed
    if (pXRVessel == NULL) {
        result << "0,0";
    } else {
        result << ((spdState.on) ? "1" : "0") << ",";
        result << ((spdState.on) ? spdState.TargetAirspeed : 0);
    }
    result << ",";
    // do vspeed
    if (pXRVessel == NULL) {
        result << "0,0";
    } else {
        result << ((dscntState.on && dscntState.AutoLandMode == 0) ? "1" : "0") << ",";
        result << ((dscntState.on && dscntState.AutoLandMode == 0) ? dscntState.TargetDescentRate : 0);
    }

    return result.str();
}

string VesselCommandParser::getAttitudeMode(VESSEL2* pVessel) {
    char buffer[8];
    sprintf_s(buffer,"%d",pVessel->GetAttitudeMode());

    return buffer;
}

string VesselCommandParser::setAttitudeMode(VESSEL2* pVessel, vector<string> tokens) {
    string msg = "ERR04";
    int mode = -1;
    bool converted = CommandParser::intFromString(tokens.at(3), mode);
    if ( converted && mode >= 0 && mode <= 2 )
    {
        bool state = pVessel->SetAttitudeMode(mode);
        msg = (state ? "1" : "0");
    }

    return msg;
}

string VesselCommandParser::toggleAttitudeMode(VESSEL2* pVessel) {
    char buffer[8];
    sprintf_s(buffer,"%d",pVessel->ToggleAttitudeMode());

    return buffer;
}

string VesselCommandParser::getADCtrlMode(VESSEL2* pVessel) {
    char buffer[8];
    sprintf_s(buffer,"%d",pVessel->GetADCtrlMode());

    return buffer;
}

string VesselCommandParser::setADCtrlMode(VESSEL2* pVessel, vector<string> tokens) {
    string msg = "ERR04";
    int mode = -1;
    bool converted = CommandParser::intFromString(tokens.at(3), mode);
    if ( converted && mode >= 0 && mode <= 2 )
    {
        pVessel->SetADCtrlMode(mode);
        msg = "OK";
    }

    return msg;
}

string VesselCommandParser::getStatus(VESSEL2* pVessel) {
    VESSELSTATUS status;
    VECTOR3 vec;
    OBJHANDLE obj = 0;
    char buffer[256];
    char buffer2[256];
    MessageStream msg;
    pVessel->GetStatus(status);
    vec = status.rpos;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.rvel;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.vrot;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.arot;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    msg << status.fuel << "," << status.eng_main << "," << status.eng_hovr << ",";
    obj = status.rbody;
    oapiGetObjectName(obj, buffer, 256);
    msg << buffer << ",";
    obj = status.base;
    oapiGetObjectName(obj, buffer2, 256);
    msg << buffer2 << ",";
    msg << status.port << "," << status.status;

    return msg.str();
}

string VesselCommandParser::getStatus2(VESSEL2* pVessel, vector<string> tokens) {
    if (3 > tokens.size()) return "ERRO3";

    VECTOR3 vec;
    OBJHANDLE obj = 0;
    char buffer[256];
    char buffer2[256];
    MessageStream msg;
    VESSELSTATUS2 status;
    OBJHANDLE vesselHandle;
    VESSEL2* pDockedVessel;

    bool getFuel = false;
    bool getThrusters = false;
    bool getDockInfo = false;
    bool converted = true;
    unsigned int idx;
    if (4 <= tokens.size() && !tokens.at(3).empty())
    {
        converted = CommandParser::boolFromString(tokens.at(3), getFuel);
    }
    if (5 <= tokens.size() && converted && !tokens.at(4).empty())
    {
        converted = CommandParser::boolFromString(tokens.at(4), getThrusters);
    }
    if (6 <= tokens.size() && converted && !tokens.at(5).empty())
    {
        converted = CommandParser::boolFromString(tokens.at(5), getDockInfo);
    }

    if (!converted)
    {
        return "ERR04";
    }


    memset (&status, 0, sizeof(status));
    status.version = 2;
    status.flag = 0;
    if (getFuel) status.flag = status.flag | VS_FUELLIST;
    if (getThrusters) status.flag = status.flag | VS_THRUSTLIST;
    if (getDockInfo) status.flag = status.flag | VS_DOCKINFOLIST;

	if ( getFuel ) {
		status.fuel = new VESSELSTATUS2::FUELSPEC[pVessel->GetPropellantCount()];
	}

	if ( getThrusters ) {
		status.thruster = new VESSELSTATUS2::THRUSTSPEC[pVessel->GetThrusterCount()];
	}

	if ( getDockInfo ) {
		status.dockinfo = new VESSELSTATUS2::DOCKINFOSPEC[pVessel->DockCount()];
	}


    pVessel->GetStatusEx (&status);

    msg << status.version << "," << status.flag << ",";
    obj = status.rbody;
    oapiGetObjectName(obj, buffer, 256);
    msg << buffer << ",";
    obj = status.base;
    oapiGetObjectName(obj, buffer2, 256);
    msg << buffer2 << ",";
    msg << status.port << "," << status.status << ",";
    vec = status.rpos;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.rvel;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.vrot;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    vec = status.arot;
    msg << vec.x << "," << vec.y << "," << vec.z << ",";
    msg << status.surf_lng << "," << status.surf_lat << "," << status.surf_hdg;

    msg << ";";
    if (getFuel)
    {
        msg << status.nfuel;
        for (idx = 0; idx < status.nfuel; idx++)
        {
            msg << "," << status.fuel[idx].level;
        }
        delete[] status.fuel;
    }
    msg << ";";
    if (getThrusters)
    {
        msg << status.nthruster;
        for (idx = 0; idx < status.nthruster; idx++)
        {
            msg << "," << status.thruster[idx].level;
        }
        delete[] status.thruster;
    }
    msg << ";";
    if (getDockInfo)
    {
        msg << status.ndockinfo;
        for (idx = 0; idx < status.ndockinfo; idx++)
        {
            vesselHandle = status.dockinfo[idx].rvessel;
            msg << "," << status.dockinfo[idx].ridx << ",";
            if (NULL == vesselHandle)
            {
                msg << "Empty";
            }
            else 
            {
                pDockedVessel = (VESSEL2*) oapiGetVesselInterface(vesselHandle);
                msg << pDockedVessel->GetName();
            }
        }
        delete[] status.dockinfo;
    }

    return msg.str();
}

string VesselCommandParser::getFlightStatus(VESSEL2* pVessel) {
    char buffer[8];
    sprintf_s(buffer,"%d",pVessel->GetFlightStatus());

    return buffer;
}

string VesselCommandParser::getEngineLevels(VESSEL2* pVessel) {

    MessageStream msg;
    int pos = 0;
    bool addComma = false;

	for (pos=0; pos<14; pos++) {
		msg << pVessel->GetThrusterGroupLevel((THGROUP_TYPE) pos);
		if ( pos != 13 )
			msg << ",";
	}

	return msg.str();
}

string VesselCommandParser::setEngineLevel(VESSEL2* pVessel, vector<string> tokens) {
    int engId;
    double level;
    bool converted = CommandParser::intFromString(tokens.at(3), engId);
    if (converted)
    {
        converted = CommandParser::doubleFromString(tokens.at(4), level);
    }
    if (!converted)
    {
        return "ERR04";
    }

	pVessel->SetThrusterGroupLevel((THGROUP_TYPE) engId, level);

    return "OK";
}

string VesselCommandParser::changeEngineLevel(VESSEL2* pVessel, vector<string> tokens) {
    int engId;
    double level;
    bool converted = CommandParser::intFromString(tokens.at(3), engId);
    if (converted)
    {
        converted = CommandParser::doubleFromString(tokens.at(4), level);
    }
    if (!converted)
    {
        return "ERR04";
    }

	pVessel->IncThrusterGroupLevel((THGROUP_TYPE) engId, level);

    return "OK";
}

string VesselCommandParser::getAirspeed(VESSEL2* pVessel) {
    double staticPressure = pVessel->GetAtmPressure();
    if (staticPressure  < 0.0001) {    // Not in an atmosphere
        return "ERR12";
    }

    double val = pVessel->GetAirspeed();
    char buffer[256];
    sprintf_s(buffer, "%f", val);

    return buffer;
}

string VesselCommandParser::getShipAirspeedVector(VESSEL2* pVessel) {
    VECTOR3 vec;
    bool state = pVessel->GetShipAirspeedVector(vec);
    char buffer[256];
    if (state) {
        sprintf_s(buffer, "%f,%f,%f", vec.x, vec.y, vec.z);
    }

    return (state ? buffer : "ERR08");
}

string VesselCommandParser::getHorizonAirspeedVector(VESSEL2* pVessel) {
    VECTOR3 vec;
    bool state = pVessel->GetHorizonAirspeedVector(vec);
    char buffer[256];
    if (state) {
        sprintf_s(buffer, "%f,%f,%f", vec.x, vec.y, vec.z);
    }

    return (state ? buffer : "ERR08");
}

/*
AOA, SlipAngle, Pitch, Bank
*/
string VesselCommandParser::getAttitude(VESSEL2* pVessel) {
    MessageStream msg;

    msg << pVessel->GetAOA() << ",";
    msg << pVessel->GetSlipAngle() << ",";
    msg << pVessel->GetPitch() << ",";
    msg << pVessel->GetBank();

    return msg.str();
}

/*
AtmTemp, AtmDensity, AtmPressure, DynPressure, MachNumber
*/
string VesselCommandParser::getAtmConditions(VESSEL2* pVessel) {
    MessageStream msg;
    OBJHANDLE atmRef = pVessel->GetAtmRef();
    if (atmRef == NULL) {               // Not in an atmosphere
        return "ERR12";
    }
    double staticPressure = pVessel->GetAtmPressure();
    if (staticPressure  < 0.0001) {    // Not in an atmosphere
        return "ERR12";
    }
    msg << pVessel->GetAtmTemperature() << ",";
    msg << pVessel->GetAtmDensity() << ",";
    msg << staticPressure << ",";
    msg << pVessel->GetDynPressure() << ",";
    msg << pVessel->GetMachNumber();

    return msg.str();
}

// Delta-TAS
string VesselCommandParser::getAccel(VESSEL2* pVessel) {
    MessageStream msg;
    double acc, mag;
    VECTOR3 force_vec, acc_vec, spd_vec;

    // Get the vectors we need
    pVessel->GetShipAirspeedVector(spd_vec);
    pVessel->GetForceVector(force_vec);

    // Normalize the speed vector
    mag = sqrt((spd_vec.x * spd_vec.x) + (spd_vec.y * spd_vec.y) + (spd_vec.z * spd_vec.z));
    if (mag != 0)
    {
        spd_vec = spd_vec / mag;

        // Calculate the acceleration vector
        acc_vec = force_vec / pVessel->GetMass();

        // Take the dot product
        acc = acc_vec.x * spd_vec.x + acc_vec.y * spd_vec.y + acc_vec.z * spd_vec.z;

        msg << acc;
    } else {
        msg << 0;
    }

    return msg.str();
}

// Delta-Vspeed
string VesselCommandParser::getVAccel(VESSEL2* pVessel) {
    MessageStream msg;
    double vacc, lon, lat, radius, mag;
    VECTOR3 force_vec, acc_vec, spd_vec;
    VECTOR3 g_vpos, g_rvel;
    OBJHANDLE planet;

    // Get the vectors we need
    pVessel->GetForceVector(force_vec);

    // Calculate the acceleration vector
    acc_vec = force_vec / pVessel->GetMass();

    // VACC now
    pVessel->HorizonRot(acc_vec,acc_vec);
    vacc = acc_vec.y;

    // Account for "centrifugal acceleration"
    planet = pVessel->GetSurfaceRef();
    pVessel->GetGlobalPos(g_vpos);
    pVessel->GetRelativeVel(planet, g_rvel);
    pVessel->Global2Local((g_rvel + g_vpos),g_rvel);
    pVessel->HorizonRot(g_rvel, spd_vec);
    pVessel->GetEquPos(lon, lat, radius);

    mag = sqrt((spd_vec.x * spd_vec.x) + (spd_vec.z * spd_vec.z));
    vacc += mag * mag / radius;

    if (pVessel->GetFlightStatus() == 1)
        vacc = 0;
    msg << vacc;

    return msg.str();
}

// Indicated Airspeed
string VesselCommandParser::getIndSpd(VESSEL2* pVessel) {
    MessageStream msg;

    // IAS calculations
    // speed of sound at sea level
    double speedOfSound = 340.29;
    // use Orbiter's constant for earth sea level pressure
    double seaLevelPres = ATMP;
    double ias = 0;
    double statPres;
    double dynPres;
    double mach;
    const ATMCONST *atmConst;

    OBJHANDLE atmRef = pVessel->GetAtmRef();
    if (atmRef == NULL) {            // Not in an atmosphere
        return "ERR12";
    } 
    // Freestream static pressure
    statPres = pVessel->GetAtmPressure();
    if (statPres  < 0.0001) {    // Not in an atmosphere
        return "ERR12";
    }

    // Retrieve/cache the ratio of specific heats
    atmConst = oapiGetPlanetAtmConstants(atmRef);
    double gamma = atmConst->gamma;

    // Mach number
    mach = pVessel->GetMachNumber();

    // Pressure ratio is stagnation pressure / static pressure
    if (mach < 1.0) {
        // Subsonic
        // Use the thermal definition for pressure ratio
        dynPres = (gamma - 1) * pow(mach, 2.0) / 2 + 1;
        dynPres = pow(dynPres, gamma / (gamma - 1));
    } else {
        // Supersonic
        // Use the supersonic compressible flow calculation for
        // pressure ratio
        mach = pow(mach, 2.0);
        dynPres = pow(gamma + 1, 2) * mach;
        dynPres = dynPres / (4 * gamma * mach - 2 * (gamma - 1));
        dynPres = pow(dynPres, 1 / (gamma - 1));
        dynPres = ((gamma + 1) / 2) * mach * dynPres;
    }
    // Convert pressure ratio to dynamic pressure
    dynPres = dynPres * statPres - statPres;

    // Following is the equation from the Orbiter manual, page 62
    ias = dynPres / seaLevelPres + 1;
    ias = pow(ias, ((gamma - 1)/gamma)) - 1.0;
    ias = ias * 2 / (gamma - 1);
    ias = sqrt(ias) * speedOfSound;

    msg << ias;

    return msg.str();
}

// Orbital Speed
string VesselCommandParser::getOrbSpd(VESSEL2* pVessel) {
    MessageStream msg;

    OBJHANDLE surfRef = pVessel->GetSurfaceRef();
    VECTOR3 relVel;
    pVessel->GetRelativeVel(surfRef, relVel);
    msg << magnitude(relVel);

    return msg.str();
}

// Ground Speed
string VesselCommandParser::getGndSpd(VESSEL2* pVessel) {
    MessageStream msg;

    OBJHANDLE surfRef = pVessel->GetSurfaceRef();
    VECTOR3 g_pos, horRelVel;
    VECTOR3 relVel;
    double period = oapiGetPlanetPeriod(surfRef);
    double alt = pVessel->GetAltitude();
    pVessel->GetRelativeVel(surfRef, relVel);

    // Get the relative velocity in the horizon frame
    pVessel->GetGlobalPos(g_pos);
    pVessel->Global2Local((relVel + g_pos),relVel);
    pVessel->HorizonRot(relVel, horRelVel);

    // Determine the altitude-adjusted ground rotational velocity
    double lon, lat, rad;
    pVessel->GetEquPos(lon, lat, rad);
    rad = rad * cos(lat);
    double circ = 2 * PI * rad;
    double spinVel = circ / period;
    horRelVel.x -= spinVel;
    msg << magnitude(horRelVel);
    return msg.str();
}

// Equivalent Airspeed
string VesselCommandParser::getEquSpd(VESSEL2* pVessel) {
    MessageStream msg;

    // speed of sound at sea level
    double speedOfSound = 340.29;
    // use Orbiter's constant for earth sea level pressure
    double seaLevelPres = ATMP;
    double statPres;
    // Freestream static pressure
    statPres = pVessel->GetAtmPressure();
    if (statPres  < 0.0001) {    // Not in an atmosphere
        return "ERR12";
    }
    msg << pVessel->GetMachNumber() * speedOfSound * sqrt(statPres / seaLevelPres);

    return msg.str();
}

string VesselCommandParser::getRadios(VESSEL2* pVessel) {
    MessageStream msg;
    bool delimit = false;
    double freq = 0.0;
    NAVHANDLE hNav = NULL;
    DOCKHANDLE hDock = NULL;
    char descr[64];

    for (int idx = 0; idx < 10; idx++) {
        freq = pVessel->GetNavRecvFreq(idx);
        if (freq == 0.0) {
            break;
        }
        if (delimit) {
            msg << ";";
        } else {
            delimit = true;
        }
        msg << "0," << idx << "," << freq << ",";
        hNav = pVessel->GetNavSource(idx);
        if (NULL == hNav) {
            continue;
        }
        oapiGetNavDescr(hNav, descr, 64);
        msg << descr;
    }
    msg << ";1,0,";
    hNav = pVessel->GetTransponder();
    if (NULL == hNav) {
        msg << "OFF";
    } else {
        msg << oapiGetNavFreq(hNav);
    }
    
    // IDS transmitters
    for (unsigned int idx = 0; idx < pVessel->DockCount(); idx++) {
        hDock = pVessel->GetDockHandle(idx);
        if (NULL == hDock) {
            continue;
        }
        hNav = pVessel->GetIDS(hDock);
        if (NULL == hNav) {
            continue;
        }
        freq = oapiGetNavFreq(hNav);
        msg << ";2," << idx << "," << freq << ",";
        oapiGetNavDescr(hNav, descr, 64);
        msg << descr;
    }
    //oapiWriteLog(const_cast<char*> (msg.str().c_str()));
    
    return msg.str();
}

string VesselCommandParser::setRadio(VESSEL2* pVessel, vector<string> tokens) {
    bool converted = false;
    int rcvrIdx;
    int channel;
    double freq;
    if (5 < tokens.size()) {
        return "ERR03";
    }
    converted = CommandParser::intFromString(tokens.at(3), rcvrIdx);
    converted = converted && CommandParser::intFromString(tokens.at(4), channel);

    if (!converted) {
        return "ERR04";
    }
    
    freq = pVessel->GetNavRecvFreq(rcvrIdx);
    if (freq == 0.0) {
        return "ERR04";
    }
    
    if (! pVessel->SetNavRecv(rcvrIdx, channel)) {
        return "ERR04";
    }
    
    return "OK";
}

string VesselCommandParser::setTransponder(VESSEL2* pVessel, vector<string> tokens) {

    bool converted = false;
    int channel;
    if (4 < tokens.size()) {
        return "ERR03";
    }
    converted = CommandParser::intFromString(tokens.at(3), channel);

    if (!converted) {
        return "ERR04";
    }
    
    if (! pVessel->SetTransponderChannel(channel)) {
        return "ERR04";
    }
    
    return "OK";
}

string VesselCommandParser::setIDS(VESSEL2* pVessel, vector<string> tokens) {

    bool converted = false;
    int dockIdx;
    int channel;
    if (5 < tokens.size()) {
        return "ERR03";
    }
    converted = CommandParser::intFromString(tokens.at(3), dockIdx);
    converted = converted && CommandParser::intFromString(tokens.at(4), channel);

    if (!converted) {
        return "ERR04";
    }
    
    DOCKHANDLE dock = pVessel->GetDockHandle(dockIdx);
    if (NULL == dock) {
        return "ERR04";
    }

    if (! pVessel->SetIDSChannel(dock, channel)) {
        return "ERR04";
    }
    
    return "OK";
}

string VesselCommandParser::SendBufferedKey(VESSEL2* pVessel, vector<string> tokens) {

    bool converted = false;
    int key;
    int modifier;
	char kstate[256];

	if (tokens.size() >= 4 && !tokens.at(3).empty()) {
		converted = CommandParser::intFromString(tokens.at(3), key);
    }
    if (converted && tokens.size() >= 5 && !tokens.at(4).empty()) {
		converted = converted && CommandParser::intFromString(tokens.at(4), modifier);
		for(int i=0;i<256;i++) kstate[i]=0x00;
		kstate[key]=0x80u;
		if (converted) {
			kstate[modifier]=0x80u;
		}
	}
    if (converted && tokens.size() >= 6 && !tokens.at(5).empty()) {
		converted = converted && CommandParser::intFromString(tokens.at(5), modifier);
		if (converted) {
			kstate[modifier]=0x80u;
		}
    }
    if (converted && tokens.size() >= 7 && !tokens.at(6).empty()) {
		converted = converted && CommandParser::intFromString(tokens.at(6), modifier);
		if (converted) {
			kstate[modifier]=0x80u;
		}
    }

    if (!converted)
    {
        return "ERR04";
    }

    if (7 < tokens.size()) {
        return "ERR03";
    }
	
	int process;
	if (tokens.size() >4) {
		process = pVessel-> SendBufferedKey(key, true, kstate);
	} else {
		process = pVessel-> SendBufferedKey(key);
	}

    if (0 == process) {
        return "ERR08";
    }
   
    return "OK";
}
       
