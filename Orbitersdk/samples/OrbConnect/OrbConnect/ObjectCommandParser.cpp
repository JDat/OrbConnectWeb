#include "ObjectCommandParser.h"

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

string ObjectCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    bool found = false;
    vector<string> tokens = stringSplit(command, ":");
    if (2 > tokens.size())
    {
        return message;
    }

    string cmd = uCase(tokens.at(1));

    if ("COUNT" == cmd)
    {
        return getObjectCount();
    }
    else if ("BYTYPE" == cmd)
    {
        return getByType(tokens);
    }

    if (tokens.size() < 2) return "ERR03";

    cmd = uCase(tokens.at(2));

    OBJHANDLE objH = getObject(tokens.at(1));
    if (objH == NULL)
    {
        message = "ERR10";
    }
    else if ("NAME" == cmd)
    {
        message = getObjectName(objH);
    }
    else if ("TYPE" == cmd)
    {
        message = getObjectType(objH);
    }
    else if ("ISVESSEL" == cmd)
    {
        message = isVessel(objH);
    }
    else if ("SIZE" == cmd)
    {
        message = getSize(objH);
    }
    else if ("MASS" == cmd)
    {
        message = getMass(objH);
    }
    else if ("GLOBALPOS" == cmd)
    {
        message = getGlobalPos(objH);
    }
    else if ("GLOBALVEL" == cmd)
    {
        message = getGlobalVel(objH);
    }
    else if ("RELPOSVEL" == cmd)
    {
        message = getRelativePosAndVel(objH, tokens);
    }
    else if ("RELPOS" == cmd)
    {
        message = getRelativePos(objH, tokens);
    }
    else if ("RELVEL" == cmd)
    {
        message = getRelativeVel(objH, tokens);
    }
    else if ("ATTACHCAMERA" == cmd)
    {
        message = attachCamera(objH, tokens);
    }

    return message;
}

string ObjectCommandParser::getObjectCount() {
    char buffer[8];
    sprintf_s(buffer, "%d", oapiGetObjectCount());

    return buffer;
}

OBJHANDLE ObjectCommandParser::getObject(string objId) {
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

string ObjectCommandParser::getObjectName(OBJHANDLE objH) {
    char buffer[256];
    oapiGetObjectName(objH, buffer, 256);

    return buffer;
}

string ObjectCommandParser::getObjectType(OBJHANDLE objH) {
    char buffer3[8];
    sprintf_s(buffer3, "%d", oapiGetObjectType(objH));

    return buffer3;
}

string ObjectCommandParser::isVessel(OBJHANDLE objH) {

    return oapiIsVessel(objH) ? "1" : "0";
}

string ObjectCommandParser::getSize(OBJHANDLE objH) {
    MessageStream response;
    response << oapiGetSize(objH);

    return response.str();
}

string ObjectCommandParser::getMass(OBJHANDLE objH) {
    MessageStream response;
    response << oapiGetMass(objH);

    return response.str();
}

string ObjectCommandParser::getGlobalPos(OBJHANDLE objH) {
    MessageStream response;
    VECTOR3 pos;
    oapiGetGlobalPos(objH, &pos);
    response << pos.x << "," << pos.y << "," << pos.z;

    return response.str();
}

string ObjectCommandParser::getGlobalVel(OBJHANDLE objH) {
    MessageStream response;
    VECTOR3 vel;
    oapiGetGlobalVel(objH, &vel);
    response << vel.x << "," << vel.y << "," << vel.z;

    return response.str();
}

string ObjectCommandParser::getRelativePos(OBJHANDLE objH, vector<string> tokens) {
    if (4 != tokens.size()) return "ERR03";

    OBJHANDLE objH2 = getObject(tokens.at(3));
    if (NULL == objH2)
    {
        return "ERR11";
    }

    MessageStream response;
    response.precision(12);
    VECTOR3 pos;
    oapiGetRelativePos(objH, objH2, &pos);
    response << pos.x << "," << pos.y << "," << pos.z;

    return response.str();
}

string ObjectCommandParser::getRelativeVel(OBJHANDLE objH, vector<string> tokens) {
    if (4 != tokens.size()) return "ERR03";

    OBJHANDLE objH2 = getObject(tokens.at(3));
    if (NULL == objH2)
    {
        return "ERR11";
    }

    MessageStream response;
    response.precision(12);
    VECTOR3 pos;
    oapiGetRelativeVel(objH, objH2, &pos);
    response << pos.x << "," << pos.y << "," << pos.z;

    return response.str();
}


string ObjectCommandParser::getRelativePosAndVel(OBJHANDLE objH, vector<string> tokens) {
    MessageStream msg;
    string msg2;
    bool error = false;
    msg << getRelativePos(objH, tokens);
    if (string::npos == msg.str().find("ERR")) {
        msg2 = getRelativeVel(objH, tokens);
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

string ObjectCommandParser::getByType(vector<string> tokens) {
    if (3 != tokens.size()) return "ERR03";

    MessageStream response;
    int type;
    char name[64];
    bool prependComma = false;
    bool converted = CommandParser::intFromString(tokens.at(2), type);
    if (converted)
    {
        for (unsigned int idx = 0; idx < oapiGetObjectCount(); idx++) {
            OBJHANDLE objH = oapiGetObjectByIndex(idx);
            if (type == oapiGetObjectType(objH)) {
                oapiGetObjectName(objH, name, 64);
                if (prependComma) {
                    response << ",";
                } else {
                    prependComma = true;
                }
                response << name;
            }
        }
    }

    return converted ? response.str() : "ERR04";
}

string ObjectCommandParser::attachCamera(OBJHANDLE objH, vector<string> tokens) {
    if (4 != tokens.size()) return "ERR03";

    string response = "ERR04";
    int mode;
    bool converted = CommandParser::intFromString(tokens.at(3), mode);
    if (converted)
    {
        oapiCameraAttach(objH, mode);
        response = "OK";
    }

    return response;
}
