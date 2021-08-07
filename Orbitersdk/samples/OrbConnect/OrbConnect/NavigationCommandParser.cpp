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
#include "NavigationCommandParser.h"
#include <algorithm>

string NavigationCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    VESSEL2* pVessel = 0;
    vector<string> tokens = stringSplit(command,":");

    string ldMsg = loadVessel(&pVessel, tokens);
    if (0 == pVessel) {
        return ldMsg;
    }

    string cmd = uCase(tokens.at(2));

    if ("ELEMENTS1" == cmd)
    {
        message = getElements1(pVessel);
    }
    else if ("ELEMENTS2" == cmd)
    {
        message = getElements2(pVessel, tokens);
    }
    else if ("EQUPOS" == cmd)
    {
        message = getEquPos(pVessel);
    }
    else if ("GLOBALPOS" == cmd)
    {
        message = getGlobalPos(pVessel);
    }
    else if ("GLOBALVEL" == cmd)
    {
        message = getGlobalVel(pVessel);
    }
    else if ("RELPOS" == cmd && tokens.size() >= 4 && !tokens.at(3).empty())
    {
        tokens.erase(tokens.begin());   // remove the 'ship' token
        message = getVesselRelativePos(pVessel, tokens);
    }
    else if ("RELVEL" == cmd && tokens.size() >= 4 && !tokens.at(3).empty())
    {
        tokens.erase(tokens.begin());   // remove the 'ship' token
        message = getVesselRelativeVel(pVessel, tokens);
    }
    else if ("RELPOSVEL" == cmd && tokens.size() >= 4 && !tokens.at(3).empty())
    {
        tokens.erase(tokens.begin());   // remove the 'ship' token
        message = getVesselRelativePosAndVel(pVessel, tokens);
    }
    else if ("GRAVCONTRIB" == cmd)
    {
        message = getGravContribution(pVessel, tokens);
    }
    else if ("GRAVREF" == cmd)
    {
        message = getGravReference(pVessel);
    }
    else if ("NAVAIDS" == cmd)
    {
        message = getNavaids(pVessel, tokens);
    }
    else if ("NAVAIDRELPOS" == cmd)
    {
        message = getNavaidRelPos(pVessel, tokens);
    }
    else if ("NAVAIDRELVEL" == cmd)
    {
        message = getNavaidRelVel(pVessel, tokens);
    }

    return message;
}

string NavigationCommandParser::getElements1(VESSEL2* pVessel) {
    MessageStream msg;
    ELEMENTS el;
    double MJD;
    OBJHANDLE refH = pVessel->GetElements(el, MJD);
    if (refH == NULL) {
        return "ERR13";
    }

    char buffer[256];
    oapiGetObjectName(refH, buffer, 256);
    msg << buffer << ","
        << el.a << "," 
        << el.e << "," 
        << el.i << "," 
        << el.theta << "," 
        << el.omegab << "," 
        << el.L << "," 
        << MJD;

    return msg.str();
}

string NavigationCommandParser::getElements2(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    ELEMENTS el;
    ORBITPARAM prm;
    bool converted = true;
    double MJD = 0.0;
    int frame = FRAME_ECL;
    OBJHANDLE refH;

    if (4 <= tokens.size() && converted)
    {
        if (tokens.at(3).empty()) {
            refH = pVessel->GetGravityRef();
        } else {
            refH = oapiGetObjectByName(const_cast<char*> (tokens.at(3).c_str()));
        }
    } else {
        refH = pVessel->GetGravityRef();
    }
    if (refH == NULL || (OBJTP_STAR != oapiGetObjectType(refH) && OBJTP_PLANET != oapiGetObjectType(refH)))
    {
        return "ERR11";
    }

    if (5 <= tokens.size() && converted && !tokens.at(4).empty())
    {
        converted = CommandParser::doubleFromString(tokens.at(4), MJD);
    }
    if (6 <= tokens.size() && converted && !tokens.at(5).empty())
    {
        converted = CommandParser::intFromString(tokens.at(5), frame);
    }

    if (!converted)
    {
        return "ERR04";
    }

    pVessel->GetElements(refH, el, &prm, MJD, frame);
    char buffer[256];
    oapiGetObjectName(refH, buffer, 256);
    msg << buffer << ","
        << el.a << "," 
        << el.e << "," 
        << el.i << "," 
        << el.theta << "," 
        << el.omegab << "," 
        << el.L << "," 
        << MJD << "," 
        << frame;

    // do ORBITPARAM
    if (&prm != NULL) {
        msg << ","
            << prm.SMi << "," 
            << prm.PeD << "," 
            << prm.ApD << "," 
            << prm.MnA << "," 
            << prm.TrA << "," 
            << prm.MnL << "," 
            << prm.TrL << "," 
            << prm.EcA << "," 
            << prm.Lec << "," 
            << prm.T << "," 
            << prm.PeT << "," 
            << prm.ApT;
    }

    return msg.str();
}

string NavigationCommandParser::getEquPos(VESSEL2* pVessel) {
    VECTOR3 pos;
    pVessel->GetEquPos(pos.x, pos.y, pos.z);
    char buffer[256];
    sprintf_s(buffer, "%f,%f,%f", pos.x, pos.y, pos.z);

    return buffer;
}

string NavigationCommandParser::getGlobalPos(VESSEL2* pVessel) {
    VECTOR3 pos;
    pVessel->GetGlobalPos(pos);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f",pos.x,pos.y,pos.z);

    return buffer;
}

string NavigationCommandParser::getGlobalVel(VESSEL2* pVessel) {
    VECTOR3 pos;
    pVessel->GetGlobalVel(pos);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f",pos.x,pos.y,pos.z);

    return buffer;
}

/**
* 0 - global reference frame
* 1 - local vessel reference frame
* 2 - local horizon reference frame
* 3 - vessel-relative horizon reference frame
*/
string NavigationCommandParser::getVesselRelativePos(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    bool converted = true;
    int referenceFrame = 0;
    VECTOR3 outPos;
    VECTOR3 refPos;
    OBJHANDLE refH = getReferenceObject(tokens.at(2));
    if (refH == NULL)
    {
        return "ERR11";
    }

    if (tokens.size() > 3 && converted) {
        converted = converted && CommandParser::intFromString(tokens.at(3), referenceFrame);
    }
    if (converted) {
        oapiGetGlobalPos(refH, &refPos);
        outPos = getVesselRelativePosition(pVessel, refPos, referenceFrame);
        msg << outPos.x << "," << outPos.y << "," << outPos.z;
    } else {
        msg << "ERR09";
    }

    return msg.str();
}

/**
* 0 - global reference frame
* 1 - local vessel reference frame
* 2 - local horizon reference frame
* 3 - vessel-relative horizon reference frame
*/
VECTOR3 NavigationCommandParser::getVesselRelativePosition(VESSEL2* pVessel, VECTOR3 refGlobalPos, int refFrame)
{
    VECTOR3 relVesPos;
    VECTOR3 globalVesPos;
    VECTOR3 outPos;
    VECTOR3 tmpPos;
    
    pVessel->GetGlobalPos(globalVesPos);
    relVesPos = globalVesPos - refGlobalPos;
    switch (refFrame) {
        case 1:
            pVessel->Global2Local(relVesPos + globalVesPos, outPos);
            break;
        case 2:
            pVessel->Global2Local(relVesPos + globalVesPos, tmpPos);
            pVessel->HorizonRot(tmpPos, outPos);
            break;
        case 3:
            double heading; 
            // Horizon frame of reference, rotated to vessel's direction
            oapiGetHeading(pVessel->GetHandle(), &heading);
            heading = -heading;
            pVessel->Global2Local(relVesPos + globalVesPos, tmpPos);
            pVessel->HorizonRot(tmpPos, tmpPos);
            outPos.z = cos(heading) * tmpPos.z - sin(heading) * tmpPos.x;
            outPos.x = sin(heading) * tmpPos.z + cos(heading) * tmpPos.x;
            outPos.y = tmpPos.y;
            break;
        default:
            outPos = relVesPos;
            break;
    }
    
    return outPos;
}


/**
* 0 - global reference frame
* 1 - local vessel reference frame
* 2 - local horizon reference frame
* 3 - vessel-relative horizon reference frame
*/
string NavigationCommandParser::getVesselRelativeVel(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    bool converted = true;
    int referenceFrame = 0;
    VECTOR3 relVesVel;
    VECTOR3 globalVesPos;
    VECTOR3 outVel;
    OBJHANDLE refH = getReferenceObject(tokens.at(2));
    if (refH == NULL)
    {
        return "ERR11";
    }

    if (tokens.size() > 3 && converted) {
        converted = converted && CommandParser::intFromString(tokens.at(3), referenceFrame);
    }

    if (converted) {
        pVessel->GetRelativeVel(refH, relVesVel);
        pVessel->GetGlobalPos(globalVesPos);
        switch (referenceFrame) {
            VECTOR3 tmpVel;
            case 1:
                pVessel->Global2Local(relVesVel + globalVesPos, outVel);
                break;
            case 2:
                pVessel->Global2Local(relVesVel + globalVesPos, tmpVel);
                pVessel->HorizonRot(tmpVel, outVel);
                break;
            case 3:
                double heading; 
                // Horizon frame of reference, rotated to vessel's direction
                oapiGetHeading(pVessel->GetHandle(), &heading);
                heading = -heading;
                pVessel->Global2Local(relVesVel + globalVesPos, tmpVel);
                pVessel->HorizonRot(tmpVel, tmpVel);
                outVel.z = cos(heading) * tmpVel.z - sin(heading) * tmpVel.x;
                outVel.x = sin(heading) * tmpVel.z + cos(heading) * tmpVel.x;
                outVel.y = tmpVel.y;
                break;
            default:
                outVel = relVesVel;
                break;
        }
        msg << outVel.x << "," << outVel.y << "," << outVel.z;
    } else {
        msg << "ERR09";
    }

    return msg.str();
}

/**
* 0 - global reference frame
* 1 - local vessel reference frame
* 2 - local horizon reference frame
* 3 - vessel-relative horizon reference frame
*/
string NavigationCommandParser::getVesselRelativePosAndVel(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    string msg2;
    bool error = false;
    msg << getVesselRelativePos(pVessel, tokens);
    if (string::npos == msg.str().find("ERR")) {
        msg2 = getVesselRelativeVel(pVessel, tokens);
    } else {
        oapiWriteLog("Error from relpos");
    }
    if (string::npos != msg2.find("ERR")) {
        error = true;
        oapiWriteLog("Error from relvel");
    } else {
        msg << "," << msg2;
    }

    return error ? msg2 : msg.str();
}

string NavigationCommandParser::getGravContribution(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    OBJHANDLE refH;
    VECTOR3 relPos;
    double gRef;
    double gTot;
    unsigned int numGbodies;

    if (4 <= tokens.size())
    {
        if (tokens.at(3).empty()) {
            refH = pVessel->GetGravityRef();
        } else {
            refH = oapiGetObjectByName(const_cast<char*> (tokens.at(3).c_str()));
        }
    } else {
        refH = pVessel->GetGravityRef();
    }
    if (refH == NULL || (OBJTP_STAR != oapiGetObjectType(refH) && OBJTP_PLANET != oapiGetObjectType(refH)))
    {
        return "ERR11";
    }

    pVessel->GetRelativePos(refH, relPos);
    // Thanks to CJP for the following method from the Free Orbit MFD:
    // dotp(a, a) == length(a)^2
    gRef = oapiGetMass(refH) / dotp(relPos, relPos);
    gTot = 0.0;

    numGbodies = oapiGetGbodyCount();
    for(unsigned int idx = 0; idx < numGbodies; idx++) {
        refH = oapiGetGbodyByIndex(idx);
        pVessel->GetRelativePos(refH, relPos);
        gTot += oapiGetMass(refH) / dotp(relPos, relPos);
    }

    msg << gRef / gTot;

    return msg.str();
}

string NavigationCommandParser::getGravReference(VESSEL2* pVessel) {
    MessageStream msg;
    OBJHANDLE refH;
    char buffer[256];

    refH = pVessel->GetGravityRef();
    oapiGetObjectName(refH, buffer, 256);

    msg << buffer;

    return msg.str();
}

bool sortByDist(XMTR_T nav1, XMTR_T nav2) { return nav1.dist < nav2.dist; }

bool sortByName(XMTR_T nav1, XMTR_T nav2) { return nav1.descr.compare(nav2.descr) < 0; }

string NavigationCommandParser::getNavaids(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    if(!pVessel->SetNavRecv(0, 0)) {
        return "ERR14";
    }
    
    bool converted = true;
    int navType = 0;
    int limit = 12;
    bool alphaSort = false;
    if (4 <= tokens.size() && !tokens.at(3).empty())
    {
        converted = CommandParser::intFromString(tokens.at(3), navType);
    }
    if (5 <= tokens.size() && converted && !tokens.at(4).empty())
    {
        converted = CommandParser::intFromString(tokens.at(4), limit);
    }
    if (6 <= tokens.size() && converted && !tokens.at(5).empty())
    {
        converted = CommandParser::boolFromString(tokens.at(5), alphaSort);
    }

    if (!converted)
    {
        return "ERR04";
    }

    vector<XMTR_T> navaids = findXmtrs(pVessel);
    bool delimit = false;
  
    if (alphaSort) {
        sort(navaids.begin(), navaids.end(), sortByName);
    } else {
        sort(navaids.begin(), navaids.end(), sortByDist);
    }

  vector<XMTR_T>::iterator iter;
    for (iter = navaids.begin(); iter != navaids.end(); ++iter) {
        XMTR_T navaid = *iter;
        if (navType > 0 && navType != navaid.type) {
            continue;
        }
        if (delimit) {
            msg << ";";
        } else {
            delimit = true;
        }
        msg << navaid.type << "," 
            << navaid.descr << "," 
            << (navaid.channel * 0.05 + 108.0) << "," 
            << navaid.inRange;
        if (--limit < 0) {
            break;
        }
    }

    return msg.str();
}

MATRIX3 invertMatrix3(const MATRIX3 &mat)
{
    double det = mat.m11 * (mat.m33 * mat.m22 - mat.m32 * mat.m23)
               - mat.m21 * (mat.m33 * mat.m12 - mat.m32 * mat.m13)
               + mat.m31 * (mat.m23 * mat.m12 - mat.m22 * mat.m13);

    if (det != 0.0)
        det = 1.0 / det;

    MATRIX3 inv;
    inv.m11 = det * (mat.m33 * mat.m22 - mat.m32 * mat.m23);
    inv.m12 = -det * (mat.m33 * mat.m12 - mat.m32 * mat.m13);
    inv.m13 = det * (mat.m23 * mat.m12 - mat.m22 * mat.m13);

    inv.m21 = -det * (mat.m33 * mat.m21 - mat.m31 * mat.m23);
    inv.m22 = det * (mat.m33 * mat.m11 - mat.m31 * mat.m13);
    inv.m23 = -det * (mat.m23 * mat.m11 - mat.m21 * mat.m13);

    inv.m31 = det * (mat.m32 * mat.m21 - mat.m31 * mat.m22);
    inv.m32 = -det * (mat.m32 * mat.m11 - mat.m31 * mat.m12);
    inv.m33 = det * (mat.m22 * mat.m11 - mat.m21 * mat.m12);

    return inv;
}

/**
* 0 - global reference frame
* 1 - local vessel reference frame
* 2 - local horizon reference frame
* 3 - vessel-relative horizon reference frame
*/
string NavigationCommandParser::getNavaidRelVel(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    
    bool converted = true;
    int rcvrIdx = 0;
    int referenceFrame = 0;
    VECTOR3	xmtrPos;
    VECTOR3 shipPos;
    VECTOR3 globalRelVel = _V(0,0,0);
    double freq;
    string str;
    int xmtrType;

    if (4 <= tokens.size() && !tokens.at(3).empty())
    {
        converted = CommandParser::intFromString(tokens.at(3), rcvrIdx);
    }
    if (5 <= tokens.size() && converted && !tokens.at(4).empty())
    {
        converted = CommandParser::intFromString(tokens.at(4), referenceFrame);
    }

    if (!converted) {
        return "ERR04";
    }
    
    freq = pVessel->GetNavRecvFreq(rcvrIdx);
    if (freq == 0.0) {
        return "ERR14";
    }
    
    NAVHANDLE hNav = pVessel->GetNavSource(rcvrIdx);
    if (!hNav)  {
        return "0,0,0";
    }

    oapiGetNavPos(hNav, &xmtrPos);
    pVessel->GetGlobalPos(shipPos);
    
    if (!oapiNavInRange(hNav, shipPos)) {
        return "0,0,0";
    }

    xmtrType = oapiGetNavType(hNav);

    switch (xmtrType) {
        case TRANSMITTER_VOR:
            // same as ILS
        case TRANSMITTER_VTOL:
            // same as ILS
        case TRANSMITTER_ILS:
            {
                // Version 2.0 - ground-stationary (based on groundspd math)
                OBJHANDLE planet = pVessel->GetGravityRef();
                VECTOR3 g_pos, horRelVel;
                VECTOR3 relVel;
                double period = oapiGetPlanetPeriod(planet);
                double alt = pVessel->GetAltitude();
                pVessel->GetRelativeVel(planet, relVel);

                // Get the relative velocity in the horizon frame
                pVessel->GetGlobalPos(g_pos);
                //pVessel->Global2Local((relVel + g_pos),relVel);
                //pVessel->HorizonRot(relVel, horRelVel);
                horRelVel = _V(0, 0, 0);

                // Determine the altitude-adjusted ground rotational velocity
                double lon, lat, rad;
                pVessel->GetEquPos(lon, lat, rad);
                rad = rad * cos(lat);
                double circ = 2 * PI * rad;
                double spinVel = circ / period;
                horRelVel.x += spinVel;

                // Convert back to global (for now)
                pVessel->HorizonInvRot(horRelVel, horRelVel);
                pVessel->GlobalRot(horRelVel, horRelVel);
                globalRelVel = relVel - horRelVel;
            }
            break;
        case TRANSMITTER_IDS:
            // Determine the owning vessel
            // get velocity of owning vessel
            // calculate offset from center of owning vessel
            // based on rotation of owning vessel, calculate resulting abs vel for
            // the IDS
            {
                VECTOR3 angularVel;
                VECTOR3 dockPos, dockDir, dockRot;
                char descr[128] = {'\0'};
                char vesName[128] = {'\0'};
                int dockNum;
     
                // Get the description and parse out the dock number and vessel name
                oapiGetNavDescr(hNav, descr, 128);
                sscanf_s(descr, "IDS D-%d %[^\n\r]", &dockNum, vesName);
                OBJHANDLE refVes = oapiGetVesselByName(vesName);
                if (refVes == NULL) {
                    break;
                }
                VESSEL * pRefVes = oapiGetVesselInterface(refVes);
                if (pRefVes == NULL) {
                    break;
                }
                pRefVes->GetAngularVel(angularVel);

                // In the description, dock numbers start at 1
                dockNum--;
                DOCKHANDLE dock = pRefVes->GetDockHandle(dockNum);
                if (dock == NULL) {
                    break;
                }
                pRefVes->GetDockParams(dock, dockPos, dockDir, dockRot);

                VECTOR3 accumVector = _V(0,0,0);
                double cylRad, radSpd;
                // Handle each axis individually
                for (int i = 0; i < 3; i++) {
                    VECTOR3 tmpVector = dockPos;
                    tmpVector.data[i] = 0;
                    cylRad = length ( tmpVector );
                    // speed relative to planet is velocity around that axis
                    // times the radius
                    radSpd = cylRad * angularVel.data[i];

                    // obtain unit vector in radial direction
                    tmpVector = tmpVector / cylRad;
                    
                    // obtain unit vector in velocity direction (perp. to radial)
                    double tmpDouble = -tmpVector.data[i < 2 ? i + 1 : i - 2];
                    tmpVector.data[i < 2 ? i + 1 : i - 2] = tmpVector.data[i < 1 ? i + 2 : i - 1];
                    tmpVector.data[i < 1 ? i + 2 : i - 1] = tmpDouble;
                    // multiply by speed to get velocity vector
                    tmpVector = tmpVector * radSpd;
                    accumVector += tmpVector;
                }
                pRefVes->GlobalRot(accumVector, accumVector);
                pVessel->GetRelativeVel(refVes, globalRelVel);
                globalRelVel += accumVector;
                break;
            }
            break;
        case TRANSMITTER_XPDR:
            {
                char descr[128] = {'\0'};
                char vesName[128] = {'\0'};
                // same as above, without rotation
                oapiGetNavDescr(hNav, descr, 128);
                sscanf_s(descr, "XPDR %[^\n\r]", vesName);
                OBJHANDLE refVes = oapiGetVesselByName(vesName);
                if (refVes != NULL) {
                    pVessel->GetRelativeVel(refVes, globalRelVel);
                }
                break;
            }
    }
    
    if (converted) {
        VECTOR3 outVel;
        switch (referenceFrame) {
            VECTOR3 tmpVel;
            case 1:
                pVessel->Global2Local(globalRelVel + shipPos, outVel);
                break;
            case 2:
                pVessel->Global2Local(globalRelVel + shipPos, tmpVel);
                pVessel->HorizonRot(tmpVel, outVel);
                break;
            case 3:
                double heading; 
                // Horizon frame of reference, rotated to vessel's direction
                oapiGetHeading(pVessel->GetHandle(), &heading);
                heading = -heading;
                pVessel->Global2Local(globalRelVel + shipPos, tmpVel);
                pVessel->HorizonRot(tmpVel, tmpVel);
                outVel.z = cos(heading) * tmpVel.z - sin(heading) * tmpVel.x;
                outVel.x = sin(heading) * tmpVel.z + cos(heading) * tmpVel.x;
                outVel.y = tmpVel.y;
                break;
            default:
                outVel = globalRelVel;
                break;
        }
        msg << outVel.x << "," << outVel.y << "," << outVel.z;
    }
    
    
    return msg.str();
}

string NavigationCommandParser::getNavaidRelPos(VESSEL2* pVessel, vector<string> tokens) {
    MessageStream msg;
    
    bool converted = true;
    int rcvrIdx = 0;
    int referenceFrame = 0;
    double dist;
    VECTOR3	xmtrPos;
    VECTOR3 shipPos;
    VECTOR3 relPos;
    bool inRange;
    double freq;
    string str;
    char descr[64];
    int cutAt;

    if (4 <= tokens.size() && !tokens.at(3).empty())
    {
        converted = CommandParser::intFromString(tokens.at(3), rcvrIdx);
    }
    if (5 <= tokens.size() && converted && !tokens.at(4).empty())
    {
        converted = CommandParser::intFromString(tokens.at(4), referenceFrame);
    }

    if (!converted) {
        return "ERR04";
    }
    
    freq = pVessel->GetNavRecvFreq(rcvrIdx);
    if (freq == 0.0) {
        return "ERR14";
    }
    
    NAVHANDLE hNav = pVessel->GetNavSource(rcvrIdx);
    
    if (!hNav)  {
        return "0,,0,0,0,0,0,0,0";
    }
    
    oapiGetNavPos(hNav, &xmtrPos);
    pVessel->GetGlobalPos(shipPos);
    inRange = oapiNavInRange(hNav, shipPos) ;    
    dist = length(xmtrPos - shipPos);
    oapiGetNavDescr(hNav, descr, 64);
    str = descr;
    cutAt = str.find_first_of(" ");
    msg << oapiGetNavType(hNav) << "," 
        << str.substr(cutAt + 1) << "," 
        << pVessel->GetNavRecv(rcvrIdx) << "," 
        << freq << "," 
        << inRange;
    if (inRange) {
        relPos = getVesselRelativePosition(pVessel, xmtrPos, referenceFrame);
        msg << "," << dist
            << "," << relPos.x
            << "," << relPos.y
            << "," << relPos.z;
    } else {
        msg << ",0,0,0,0";
    }
    
    return msg.str();
}

vector<XMTR_T> NavigationCommandParser::findXmtrs(VESSEL2* pVessel)
{
    int	origChannel;
    VECTOR3	xmtrPos;
    VECTOR3 shipPos;
    vector<XMTR_T> xmtrs;
    NAVHANDLE hNav;
    char descr[64];
    int cutAt;
    string str;

    origChannel = pVessel->GetNavRecv(0);
    for(int ch = 0; ch <= 640; ch++) {
        pVessel->SetNavRecv(0, ch);
        hNav = pVessel->GetNavSource(0);
        if(!hNav) {
            continue;
        }
        XMTR_T entry;
        // found one, do range check first
        oapiGetNavPos(hNav, &xmtrPos);
        oapiGetGlobalPos(pVessel->GetHandle(), &shipPos);
        entry.dist = length(xmtrPos - shipPos);

        entry.channel = ch;
        oapiGetNavDescr(hNav, descr, 64);
        str = descr;
        cutAt = str.find_first_of(" ");
        entry.descr = str.substr(cutAt + 1);
        entry.type = oapiGetNavType(hNav);
        entry.inRange = oapiNavInRange(hNav, shipPos) ;
        xmtrs.push_back(entry);
        oapiWriteLog(descr);
    }
    pVessel->SetNavRecv(0, origChannel) ;
    
    return xmtrs;
};

