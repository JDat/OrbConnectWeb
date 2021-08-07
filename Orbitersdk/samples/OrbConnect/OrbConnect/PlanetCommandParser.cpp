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

#include "PlanetCommandParser.h"


string PlanetCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    bool found = false;
    vector<string> tokens = stringSplit(command, ":");
    string cmd = uCase(tokens.at(2));
    OBJHANDLE planet = getPlanet(tokens.at(1));
    if (planet == NULL)
    {
        return "ERR10";
    }
    if (oapiGetObjectType(planet) != 4)
    {
        return "ERR10";
    }
    if ("PERIOD" == cmd)
    {
        message = getPeriod(planet);
    }
    else if ("OBLIQUITY" == cmd)
    {
        message = getObliquity(planet);
    }
    else if ("THETA" == cmd)
    {
        message = getTheta(planet);
    }
    else if ("OBLIQUITYMATRIX" == cmd)
    {
        message = getObliquityMatrix(planet);
    }
    else if ("CURRROTATION" == cmd)
    {
        message = getCurrentRotation(planet);
    }
    else if ("HASATM" == cmd)
    {
        message = (oapiPlanetHasAtmosphere(planet) ? "1" : "0");
    }
    else if ("ATMCONSTS" == cmd)
    {
        message = getAtmConstants(planet);
    }
    else if ("ATMPARAMS" == cmd)
    {
        message = getAtmParams(planet, tokens);
    }
    else if ("JCOEFFCOUNT" == cmd)
    {
        message = getJCoeffCount(planet);
    }
    else if ("JCOEFF" == cmd)
    {
        message = getJCoeff(planet, tokens);
    }
    else if ("JCOEFFS" == cmd)
    {
        message = getJCoeffs(planet);
    }
    else if ("BASECOUNT" == cmd)
    {
        message = getBaseCount(planet);
    }
    else if ("BASENAME" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getBaseName(base);
    }
    else if ("BASEEQUPOS" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getBaseEquPos(base);
    }
    else if ("BASEPADCOUNT" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getBasePadCount(base);
    }
    else if ("BASEPADEQUPOS" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getBasePadEquPos(base, tokens);
    }
    else if ("BASEPADSTATUS" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getBasePadStatus(base, tokens);
    }
    else if ("NAVCHANNEL" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getNavChannel(base, tokens);
    }
    else if ("NAVFREQ" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getNavFreq(base, tokens);
    }
    else if ("NAVPOS" == cmd)
    {
        OBJHANDLE base = getBase(planet, tokens);
        message = getNavPos(base, tokens);
    }

    return message;
}

OBJHANDLE PlanetCommandParser::getPlanet(string objId) {
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

string PlanetCommandParser::getPeriod(OBJHANDLE planet) {
    char buffer[64];
    sprintf_s(buffer,"%f",oapiGetPlanetPeriod(planet));
    string response (buffer);

    return response;
}

string PlanetCommandParser::getObliquity(OBJHANDLE planet) {
    char buffer[64];
    sprintf_s(buffer,"%f",oapiGetPlanetObliquity(planet));
    string response (buffer);

    return response;
}

string PlanetCommandParser::getTheta(OBJHANDLE planet) {
    char buffer[64];
    sprintf_s(buffer,"%f",oapiGetPlanetTheta(planet));
    string response (buffer);

    return response;
}

string PlanetCommandParser::getObliquityMatrix(OBJHANDLE planet) {
    MATRIX3 mat;
    oapiGetPlanetObliquityMatrix(planet, &mat);
    char buffer[256];
    sprintf_s(buffer,"%f,%f,%f,%f,%f,%f,%f,%f,%f",mat.m11,mat.m12,mat.m13,mat.m21,mat.m22,mat.m23,mat.m31,mat.m32,mat.m33);
    string response (buffer);

    return response;
}
string PlanetCommandParser::getCurrentRotation(OBJHANDLE planet) {
    char buffer[64];
    sprintf_s(buffer,"%f",oapiGetPlanetCurrentRotation(planet));
    string response (buffer);

    return response;
}

string PlanetCommandParser::getAtmConstants(OBJHANDLE planet) {
    char buffer[512];
    const ATMCONST * atm = oapiGetPlanetAtmConstants(planet);
    if (atm == NULL)
    {
        return "ERR12";
    }

    sprintf_s(buffer,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",atm->p0,atm->rho0,atm->R,atm->gamma,atm->C,atm->O2pp,atm->altlimit,atm->radlimit,atm->horizonalt,atm->color0.x,atm->color0.y,atm->color0.z);
    string response (buffer);

    return response;
}

string PlanetCommandParser::getAtmParams(OBJHANDLE planet, vector<string> tokens) {
    if (tokens.size() < 4) return "ERR03";

    double radius;
    bool converted = CommandParser::doubleFromString(tokens.at(3), radius);

    if (!converted)
    {
        return "ERR04.";
    }

    char buffer2[512];
    ATMPARAM atm;
    oapiGetPlanetAtmParams(planet, radius, &atm);
    sprintf_s(buffer2,"%f,%f,%f", atm.T, atm.p, atm.rho);
    string response (buffer2);

    return response;
}

string PlanetCommandParser::getJCoeffCount(OBJHANDLE planet) {
    MessageStream response;
    response << oapiGetPlanetJCoeffCount(planet);

    return response.str();
}

string PlanetCommandParser::getJCoeff(OBJHANDLE planet, vector<string> tokens) {
    if (tokens.size() != 4) return "ERR03";

    string message = "ERR04";
    int idx;
    bool converted = CommandParser::intFromString(tokens.at(3), idx);
    if (converted && idx < (int)oapiGetPlanetJCoeffCount(planet) && idx >= 0)
    {
        char buffer2[32];
        sprintf_s(buffer2,"%f",oapiGetPlanetJCoeff(planet, idx));
        string response (buffer2);
        message = response;
    }

    return message;
}

string PlanetCommandParser::getJCoeffs(OBJHANDLE planet) {
    MessageStream result;
    bool prependComma = false;
    unsigned int idx = 0;
    for (idx = 0; idx < oapiGetPlanetJCoeffCount(planet); idx++)
    {
        if (prependComma)
        {
            result << ",";
        }
        else
        {
            prependComma = true;
        }

        result << oapiGetPlanetJCoeff(planet, idx);
    }

    return result.str();
}

string PlanetCommandParser::getBaseCount(OBJHANDLE planet) {
    MessageStream response;
    response << oapiGetBaseCount(planet);

    return response.str();
}

OBJHANDLE PlanetCommandParser::getBase(OBJHANDLE planet, vector<string> tokens) {
    if (tokens.size() < 4) return "ERR03";

    OBJHANDLE base = oapiGetBaseByName(planet, const_cast<char*> (tokens.at(3).c_str()));
    if (base == NULL)
    {
        int idx;
        bool converted = CommandParser::intFromString(tokens.at(3), idx);
        if (converted && idx < (int)oapiGetBaseCount(planet) && idx >= 0)
        {
            base = oapiGetBaseByIndex(planet, idx);
        }
    }

    return base;
}

string PlanetCommandParser::getBaseName(OBJHANDLE base) {
    if (NULL == base) return "ERR10";

    char buffer[256];
    oapiGetObjectName(base, buffer, 256);

    return buffer;
}

string PlanetCommandParser::getBaseEquPos(OBJHANDLE base) {
    if (NULL == base) return "ERR10";

    char buffer[256];
    double lng;
    double lat;
    double rad;

    oapiGetBaseEquPos(base, &lng, &lat, &rad);
    sprintf_s(buffer,"%f,%f,%f", lng, lat, rad);

    return buffer;
}

string PlanetCommandParser::getBasePadCount(OBJHANDLE base) {
    if (NULL == base) return "ERR10";

    MessageStream response;
    response << oapiGetBasePadCount(base);

    return response.str();
}

string PlanetCommandParser::getBasePadEquPos(OBJHANDLE base, vector<string> tokens) {
    if (NULL == base) return "ERR10";
    if (5 > tokens.size()) return "ERR03";

    MessageStream response;
    double lng;
    double lat;
    double rad;
    int idx;
    bool converted = CommandParser::intFromString(tokens.at(4), idx);
    if (converted && idx < (int)oapiGetBasePadCount(base) && idx >= 0)
    {
        oapiGetBasePadEquPos(base, idx, &lng, &lat, &rad);
        response << lng << "," << lat << "," << rad;
    }
    else
    {
        response << "ERR04";
    }					

    return response.str();
}

string PlanetCommandParser::getBasePadStatus(OBJHANDLE base, vector<string> tokens) {
    if (NULL == base) return "ERR10";
    if (5 > tokens.size()) return "ERR03";

    MessageStream response;
    int status;
    int idx;
    bool converted = CommandParser::intFromString(tokens.at(4), idx);
    if (converted && idx < (int)oapiGetBasePadCount(base) && idx >= 0)
    {
        oapiGetBasePadStatus(base, idx, &status);
        response << status;
    }
    else
    {
        response << "ERR04";
    }					

    return response.str();
}

NAVHANDLE PlanetCommandParser::getNavHandle(OBJHANDLE base, vector<string> tokens) {
    int idx;
    NAVHANDLE nav = NULL;
    bool converted = CommandParser::intFromString(tokens.at(4), idx);
    if (converted && idx < (int)oapiGetBasePadCount(base) && idx >= 0)
    {
        nav = oapiGetBasePadNav(base, idx);
    }

    return nav;
}

string PlanetCommandParser::getNavChannel(OBJHANDLE base, vector<string> tokens) {
    if (NULL == base) return "ERR10";
    if (5 > tokens.size()) return "ERR03";

    MessageStream response;
    NAVHANDLE nav = getNavHandle(base, tokens);
    if (NULL != nav)
    {
        response << oapiGetNavChannel(nav);
    }
    else
    {
        response << "ERR04";
    }					

    return response.str();
}

string PlanetCommandParser::getNavFreq(OBJHANDLE base, vector<string> tokens) {
    if (NULL == base) return "ERR10";

    MessageStream response;

    NAVHANDLE nav = getNavHandle(base, tokens);
    if (NULL != nav)
    {
        response << oapiGetNavFreq(nav);
    }
    else
    {
        response << "ERR04";
    }					

    return response.str();
}

string PlanetCommandParser::getNavPos(OBJHANDLE base, vector<string> tokens) {
    if (NULL == base) return "ERR10";

    MessageStream response;
    VECTOR3 gpos;

    NAVHANDLE nav = getNavHandle(base, tokens);
    if (NULL != nav)
    {
        oapiGetNavPos(nav, &gpos);
        response << gpos.x << "," << gpos.y << "," << gpos.z;
    }
    else
    {
        response << "ERR04";
    }					

    return response.str();
}

