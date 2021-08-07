
#define _WIN32_WINNT 0x0501
#include "OapiCommandParser.h"
#include "windows.h"

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


/* ====== VNCMFD-related stuff ====== */
#define REQUIRED_VNCMFD_API_VERSION 32

bool CheckVncMfdApiVersion(void)
{
	HMODULE hMod = GetModuleHandle("VNCMFD.dll");
	if ( ! hMod ) return FALSE;

	FARPROC lpfn = GetProcAddress(hMod, "GetAPIVersion");
	if ( ! lpfn ) goto too_old;

	int ver = (int) lpfn();
	if ( ver >= REQUIRED_VNCMFD_API_VERSION )
		return TRUE;

too_old:
	char buf[1024];
	sprintf(buf, "VNCMFD.dll revision %d or later is required. Please upgrade!", REQUIRED_VNCMFD_API_VERSION);

	MessageBox(NULL, buf, "VNCMFD.dll too old", MB_OK);

	return FALSE;
}

int *GetVncMfdPorts(void) {
	HMODULE hMod = GetModuleHandle("VNCMFD.dll");
	if ( ! hMod ) return NULL;

	if ( ! CheckVncMfdApiVersion() ) return NULL;

    FARPROC lpfnGetMfdPorts = GetProcAddress(hMod, "GetMfdPorts");
	if (lpfnGetMfdPorts ) {
		return (int*) lpfnGetMfdPorts();
	}

	return NULL;
}

typedef ExternMFD* (*GetMfdByPort_t)(int port);


ExternMFD *GetVncMfd(int port) {
	oapiWriteLog("GetVncMfd");

	HMODULE hMod = GetModuleHandle("VNCMFD.dll");
	if ( ! hMod ) return NULL;

	if ( ! CheckVncMfdApiVersion() ) return NULL;

    GetMfdByPort_t lpfn = (GetMfdByPort_t) GetProcAddress(hMod, "GetMfdByPort");
	if ( ! lpfn ) return NULL;
	
	return (ExternMFD*) lpfn(port);
}

string OapiCommandParser::getVirtualMFDs() {
    MessageStream msg;
    boolean prependComma = false;
	int *ids = GetVncMfdPorts();
	int *id = ids;
	while (*id) {
	    if (prependComma) {
	        msg << ",";
	    } else {
	        prependComma = true;
	    }
		msg << *id;
		id++;
	}    
	return msg.str();
}

#define MFD_BAD 0xFFFFFFFF

UINT GetRealMfdId(int id)
{
	ExternMFD *ext;

	/* Built-in MFD */
	if ( id <= MAXMFD )
		return id;

	/* VNC port */
	if ( (id >= 5900) && ( id <=5999 ) )
		ext = GetVncMfd(id);
	else {
#ifdef YES_I_AM_AN_IDIOT_AND_WANT_TO_ALLOW_ARBITRARY_CODE_INJECTION
		/* ExternMFD instance */
		ext = (ExternMFD *) id;
#else
		/* Calls to arbitrary pointers are disallowed for security reasons */
		return NULL;
#endif
	} // else

	if ( ! ext )
		return MFD_BAD;

	return ext->Id();
}


void my_oapiOpenMFD(int mode, int mfd)
{
	int realMfd = GetRealMfdId(mfd);
	if ( realMfd == MFD_BAD )
		return ;  /* FIXME: Error message? */

	oapiOpenMFD(mode, realMfd);
}



int my_oapiGetMFDMode(int mfd)
{
	int realMfd = GetRealMfdId(mfd);
	if ( realMfd == MFD_BAD )
		return -1;  /* FIXME: Error message? */

	return oapiGetMFDMode(realMfd);
}

int my_oapiSendMFDKey(int mfd, DWORD key)
{
	int realMfd = GetRealMfdId(mfd);
	if ( realMfd == MFD_BAD )
		return 0;  /* FIXME: Error message? */

	// As of Orbiter 100830, oapiSendMFDKey() has a bug when dealing with
	// MFDs derived from ExternMFD
	if ( realMfd <= MAXMFD )
		return oapiSendMFDKey(realMfd, key);
	else
		return ((ExternMFD*) realMfd)->SendKey(key);
}


bool my_oapiProcessMFDButton(int mfd, int bt, int event)
{
	int realMfd = GetRealMfdId(mfd);
	if ( realMfd == MFD_BAD )
		return FALSE;  /* FIXME: Error message? */

	return oapiProcessMFDButton(realMfd, bt, event);
}

const char *my_oapiMFDButtonLabel(int mfd, int bt)
{
	int realMfd = GetRealMfdId(mfd);
	if ( realMfd == MFD_BAD )
		return NULL;  /* FIXME: Error message? */

	return oapiMFDButtonLabel(realMfd, bt);
}

/* ====== end of VNCMFD-related stuff ====== */

string OapiCommandParser::parse(long lParam, string command) {
    string message = "ERR02";
    vector<string> tokens = stringSplit(uCase(command),":");

    if ("CAMERA" == uCase(tokens.at(0))) {
        return doCamera(lParam, command, tokens);
    }

    string cmd = uCase(tokens.at(1));

    if ("GBODYCOUNT" == cmd)
    {
        message = getGBodyCount();
    }
    else if ("GBODIES" == cmd)
    {
        message = getGbodies();
    }
    else if ("SIMTIME" == cmd)
    {
        message = getSimTime();
    }
    else if ("SIMSTEP" == cmd)
    {
        message = getSimStep();
    }
    else if ("SYSTIME" == cmd)
    {
        message = getSysTime();
    }
    else if ("SYSSTEP" == cmd)
    {
        message = getSysStep();
    }
    else if ("SIMMJD" == cmd)
    {
        message = getSimMJD();
    }
    else if ("SYSMJD" == cmd)
    {
        message = getsysMJD();
    }
    else if ("SETSIMMJD" == cmd)
    {
        message = setSimMJD(tokens);
    }
    else if ("TIME2MJD" == cmd && tokens.size() == 3)
    {
        message = getTimeAsMJD(tokens);
    }
    else if ("TIMEACCEL" == cmd)
    {
        message = getTimeAcceleration();
    }
    else if ("SETTIMEACCEL" == cmd && tokens.size() == 3)
    {
        message = setTimeAcceleration(tokens);
    }
    else if ("PAUSE" == cmd)
    {
        message = getPause();
    }
    else if ("SETPAUSE" == cmd && tokens.size() == 3)
    {
        message = setPause(tokens);
    }
    else if ("FRAMERATE" == cmd)
    {
        message = getFramerate();
    }
    else if ("SETHUDMODE" == cmd && tokens.size() == 3)
    {
        message = setHudMode(tokens);
    }
    else if ("HUDMODE" == cmd)
    {
        message = getHudMode();
    }
    else if ("SETHUDBRIGHTNESS" == cmd)
    {
        message = setHudBrightness(tokens);
    }
    else if ("TOGGLEHUDCOLOR" == cmd)
    {
        message = toggleHudColor();
    }
    else if ("INCHUDINTENSITY" == cmd)
    {
        message = incrHudIntensity();
    }
    else if ("DECHUDINTENSITY" == cmd)
    {
        message = decrHudIntensity();
    }
    else if ("MOUSECLICK" == cmd)
    {
        message = mouseClick(tokens);
    }
    else if ("OPENMFD" == cmd && tokens.size() == 4)
    {
        message = openMFD(tokens);
    }
    else if ("MFDMODE" == cmd && tokens.size() == 3)
    {
        message = getMFDMode(tokens);
    }
    else if ("SENDMFDKEY" == cmd)
    {
        message = sendMFDKey(tokens);
    }
    else if ("PROCESSMFDBUTTON" == cmd && tokens.size() == 6)
    {
        message = processMFDButton(tokens);
    }
    else if ("MFDBUTTONLABEL" == cmd)
    {
        message = getMFDButtonLabel(tokens);
    }
    else if ("MFDBUTTONLABELS" == cmd)
    {
        message = getMFDButtonLabels(tokens);
    }
    else if ("VIRTUALMFDS" == cmd)
    {
        message = getVirtualMFDs();
    }
    else if ("SETPANEL" == cmd && tokens.size() == 3)
    {
        message = setPanel(tokens);
    }
    else if ("SWITCHPANEL" == cmd && tokens.size() == 3)
    {
        message = switchPanel(tokens);
    }
    else if ("DEBUGSTRING" == cmd && tokens.size() > 2)
    {
        if (tokens.at(2) == "CLEAR")
        {
            message = clearDebugString();
        }
        else
        {
            message = sendDebugString(command);
        }
    }

    return message;
}


string OapiCommandParser::doCamera(long lParam, string command, vector<string> tokens) {
    string message = "ERR02";

    string cmd = uCase(tokens.at(1));

    if ("ISINTERNAL" == cmd)
    {
        message = isCameraInternal();
    }
    else if ("MODE" == cmd)
    {
        message = getCameraMode();
    }
    else if ("COCKPITMODE" == cmd)
    {
        message = getCockpitMode();
    }
    else if ("TARGET" == cmd)
    {
        message = getCameraTarget();
    }
    else if ("GLOBALPOS" == cmd)
    {
        message = getCameraGlobalPos();
    }
    else if ("GLOBALDIR" == cmd)
    {
        message = getCameraGlobalDir();
    }
    else if ("TARGETDIST" == cmd)
    {
        message = getCameraTargetDist();
    }
    else if ("AZIMUTH" == cmd)
    {
        message = getCameraAzimuth();
    }
    else if ("POLAR" == cmd)
    {
        message = getCameraPolar();
    }
    else if ("APERTURE" == cmd)
    {
        message = getCameraAperture();
    }
    else if ("SETAPERTURE" == cmd && tokens.size() == 3)
    {		
        message = setCameraAperture(tokens);
    }
    else if ("SETSCALEDIST" == cmd && tokens.size() == 3)
    {		
        message = setCameraScaleDist(tokens);
    }
    else if ("ROTAZIMUTH" == cmd && tokens.size() == 3)
    {		
        message = setCameraRotationAzimuth(tokens);
    }
    else if ("ROTPOLAR" == cmd && tokens.size() == 3)
    {		
        message = setCameraRotationPolar(tokens);
    }
    else if ("SETCOCKPITDIR" == cmd && tokens.size() == 4)
    {		
        message = setCockpitCameraDir(tokens);
    }

    return message;
}

OBJHANDLE OapiCommandParser::getObject(string objId) {
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

string OapiCommandParser::getGBodyCount() {
    char buffer[8];
    sprintf_s(buffer,"%d",oapiGetGbodyCount());

    return buffer;
}

string OapiCommandParser::getSimTime() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetSimTime());

    return buffer;
}

string OapiCommandParser::getSimStep() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetSimStep());

    return buffer;
}

string OapiCommandParser::getSysTime() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetSysTime());

    return buffer;
}

string OapiCommandParser::getSysStep() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetSysStep());

    return buffer;
}

string OapiCommandParser::getSimMJD() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetSimMJD());

    return buffer;
}

string OapiCommandParser::setSimMJD(vector<string> tokens) {
    if (tokens.size() != 5) return "ERR03";

    string msg = "ERR04";
    double MJD;
    int oprop;
    int soprop;
    bool converted = CommandParser::doubleFromString(tokens.at(2), MJD);
    if (converted) {
        converted = CommandParser::intFromString(tokens.at(3), oprop);
    }
    if (converted) {
        converted = CommandParser::intFromString(tokens.at(4), soprop);
    }

    if (converted && oprop >= 0 && soprop >= 0) 
    {
        msg = oapiSetSimMJD(MJD,oprop | soprop) ? "1" : "0";
    }

    return msg;

}

string OapiCommandParser::getsysMJD() {
    char buffer[32];
    sprintf_s(buffer, "%f", oapiGetSysMJD());

    return buffer;
}

string OapiCommandParser::getTimeAsMJD(vector<string> tokens) {
    double simt = atof(const_cast<char*> (tokens.at(2).c_str()));
    char respBuffer[32];
    sprintf_s(respBuffer, "%f", oapiTime2MJD(simt));

    return respBuffer;
}

string OapiCommandParser::getTimeAcceleration() {
    char buffer[32];
    sprintf_s(buffer,"%f",oapiGetTimeAcceleration());

    return buffer;
}

string OapiCommandParser::setTimeAcceleration(vector<string> tokens) {
    string msg = "ERR04";
    double warp;
    bool converted = CommandParser::doubleFromString(tokens.at(2), warp);
    if (converted)
    {
        oapiSetTimeAcceleration(warp);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::getPause() {
    return oapiGetPause() ? "1" : "0";
}

string OapiCommandParser::setPause(vector<string> tokens) {
    string msg = "ERR04";
    int x = -1;
    bool converted = CommandParser::intFromString(tokens.at(2), x);
    if (converted && (x == 0 || x == 1))
    {
        oapiSetPause(x == 1);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::getFramerate() {
    char buffer[32];
    sprintf_s(buffer,"%f", oapiGetFrameRate());

    return buffer;
}

string OapiCommandParser::isCameraInternal() {
    return oapiCameraInternal() ? "1" : "0";
}

string OapiCommandParser::getCameraMode() {
    char buffer[8];
    sprintf_s(buffer,"%d", oapiCameraMode());

    return buffer;
}

string OapiCommandParser::getCockpitMode() {
    char buffer[8];
    sprintf_s(buffer,"%d", oapiCockpitMode());

    return buffer;
}

string OapiCommandParser::getCameraTarget() {
    char buffer[256];
    oapiGetObjectName(oapiCameraTarget(), buffer, 256);

    return buffer;
}

string OapiCommandParser::getCameraGlobalPos() {
    VECTOR3 pos;
    oapiCameraGlobalPos(&pos);
    char buffer[256];
    sprintf_s(buffer, "%f,%f,%f", pos.x, pos.y, pos.z);

    return buffer;
}

string OapiCommandParser::getCameraGlobalDir() {
    VECTOR3 pos;
    oapiCameraGlobalDir(&pos);
    char buffer[256];
    sprintf_s(buffer, "%f,%f,%f", pos.x, pos.y, pos.z);

    return buffer;
}

string OapiCommandParser::getCameraTargetDist() {
    char buffer[32];
    sprintf_s(buffer, "%f", oapiCameraTargetDist());

    return buffer;
}

string OapiCommandParser::getCameraAzimuth() {
    char buffer[32];
    sprintf_s(buffer, "%f", oapiCameraAzimuth());

    return buffer;
}

string OapiCommandParser::getCameraPolar() {
    char buffer[32];
    sprintf_s(buffer,"%f",oapiCameraPolar());

    return buffer;
}

string OapiCommandParser::getCameraAperture() {
    char buffer[32];
    sprintf_s(buffer,"%f",oapiCameraAperture());

    return buffer;
}

string OapiCommandParser::setCameraAperture(vector<string> tokens) {
    string msg = "ERR04";
    double aperture;
    bool converted = CommandParser::doubleFromString(tokens.at(2), aperture);
    if (converted)
    {
        oapiCameraSetAperture(aperture);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::setCameraScaleDist(vector<string> tokens) {
    string msg = "ERR04";
    double dist;
    bool converted = CommandParser::doubleFromString(tokens.at(2), dist);
    if (converted)
    {
        oapiCameraScaleDist(dist);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::setCameraRotationAzimuth(vector<string> tokens) {
    string msg = "ERR04";
    double azimuth;
    bool converted = CommandParser::doubleFromString(tokens.at(2), azimuth);
    if (converted)
    {
        oapiCameraRotAzimuth(azimuth);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::setCameraRotationPolar(vector<string> tokens) {
    string msg = "ERR04";
    double polar;
    bool converted = CommandParser::doubleFromString(tokens.at(2), polar);
    if (converted)
    {
        oapiCameraRotPolar(polar);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::setCockpitCameraDir(vector<string> tokens) {
    string msg = "ERR04";
    double polar;
    double azimuth;
    bool converted = CommandParser::doubleFromString(tokens.at(2), polar);
    if (converted)
    {
        converted = CommandParser::doubleFromString(tokens.at(3), azimuth);
    }
    if (converted)
    {
        oapiCameraSetCockpitDir(polar, azimuth, false);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::getHudMode() {
    char buffer[8];
    sprintf_s(buffer,"%d",oapiGetHUDMode());

    return buffer;
}

string OapiCommandParser::setHudMode(vector<string> tokens) {
    string msg = "ERR04";
    int mode;
    bool converted = CommandParser::intFromString(tokens.at(2), mode);

    if (converted && mode >= 0 && mode <= 3 )
    {
        oapiSetHUDMode(mode);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::setHudBrightness(vector<string> tokens) {
    string msg = "ERR04";
    int pct;
    bool converted = CommandParser::intFromString(tokens.at(2), pct);

    if (converted)
    {
        for (int cnt = 0; cnt < 2048; cnt++)
        {
            oapiDecHUDIntensity();
        }
        for (int cnt = 0; cnt < pct * .768; cnt++)
        {
            oapiIncHUDIntensity();
        }

        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::toggleHudColor() {
    oapiToggleHUDColour();

    return "OK";
}

string OapiCommandParser::incrHudIntensity() {
    oapiIncHUDIntensity();

    return "OK";
}

string OapiCommandParser::decrHudIntensity() {
    oapiDecHUDIntensity();

    return "OK";
}

string OapiCommandParser::mouseClick(vector<string> tokens) {
    if (4 > tokens.size()) return "ERR03";

    int x;
    int y;

    bool converted = CommandParser::intFromString(tokens.at(2), x);
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(3), y);
    }

    if (converted && SetCursorPos(x, y))
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
        return "OK";
    }

    return "ERR04";
}

string OapiCommandParser::openMFD(vector<string> tokens) {
    if (4 > tokens.size()) return "ERR03";

    string msg = "ERR04";
    int mode;
    int mfd;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(3), mode);
    }

    if (converted)
    {
        my_oapiOpenMFD(mode, mfd);
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::getMFDMode(vector<string> tokens) {
    char buffer[256];
    int mfd;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);

    sprintf_s(buffer,"%d", my_oapiGetMFDMode(mfd));

    return buffer;
}

string OapiCommandParser::sendMFDKey(vector<string> tokens) {
    if (tokens.size() != 4) return "ERR03";

    string msg = "ERR04";
    int mfd;
    int key;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(3), key);
    }

    if (converted && 0 != my_oapiSendMFDKey(mfd, key))
    {
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::processMFDButton(vector<string> tokens) {
    if (tokens.size() != 6) return "ERR02";

    string msg = "ERR04";
    int mfd;
    int btn;
    int evt1;
    int evt2;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(3), btn);
    }
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(4), evt1);
    }
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(5), evt2);
    }

    if (converted && my_oapiProcessMFDButton(mfd, btn, evt1 | evt2))
    {
        msg = "OK";
    }

    return msg;
}

string OapiCommandParser::getMFDButtonLabel(vector<string> tokens) {
    if (4 != tokens.size()) return "ERR03";

    string msg = "ERR04";
    int mfd;
    int btn;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);
    if (converted) 
    {
        converted = CommandParser::intFromString(tokens.at(3), btn);
    }

    const char* label = my_oapiMFDButtonLabel(mfd, btn);

    return NULL == label ? "" : label;
}

string OapiCommandParser::getMFDButtonLabels(vector<string> tokens) {
    if (3 != tokens.size()) return "ERR03";

    MessageStream msg;
    int mfd;
    bool converted = CommandParser::intFromString(tokens.at(2), mfd);

    if (converted ) {
        boolean prependComma = false;
        for (int btn = 0; btn < 16; btn++) {
	        if (prependComma) {
	            msg << ",";
	        } else {
	            prependComma = true;
	        }
            const char* label = my_oapiMFDButtonLabel(mfd, btn);
            msg << ((NULL == label) ? "" : label);
        }
    } else {
        msg << "ERR04";
    }
        
    return msg.str();
}

string OapiCommandParser::setPanel(vector<string> tokens) {
    MessageStream msg;
    int idx;
    bool converted = CommandParser::intFromString(tokens.at(2), idx);
    if (converted) {
        msg << oapiSetPanel(idx);
    }
    else{
        msg << "ERR04";
    }

    return msg.str();
}

string OapiCommandParser::switchPanel(vector<string> tokens) {
    MessageStream msg;
    int dir;
    bool converted = CommandParser::intFromString(tokens.at(2), dir);
    if (converted) {
        msg << oapiSwitchPanel(dir);
    }
    else{
        msg << "ERR04";
    }

    return msg.str();
}

string OapiCommandParser::getGbodies() {
    vector<Gbody*>::iterator iter;
    return sun == NULL ? "ERR15" : sun->toCsvString("");
}

string OapiCommandParser::clearDebugString() {
    sprintf_s(oapiDebugString(),256,"");

    return "OK";
}

string OapiCommandParser::sendDebugString(string command) {
    string msg = "ERR98";
    int x = uCase(command).find("DEBUGSTRING") + 12;
    char buffer[256];
    if (command.substr(x).length() < 256)
    {
        strcpy_s(buffer, command.substr(x).c_str());
        sprintf_s(oapiDebugString(), 256, buffer);
        msg = "OK";
    }

    return msg;
}

