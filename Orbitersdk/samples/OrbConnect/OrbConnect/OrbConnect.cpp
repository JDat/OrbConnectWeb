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
*    Solar system initialization code is modified from code originally 
*    written for the transx MFD by Duncan Sharpe, Steve Arch.
** @copyright 2008 Randy Stearns
* @Author Randy Stearns
* @Author Russ Purinton
*/

#define ORBITER_MODULE 

#include "OrbConnect.h"

#include "http.h"

bool terminatePauseThread = false;
std::vector<bool> vesselAxesSet;
std::vector<double> vesselAxesValues;

char port[8];
HINSTANCE g_hDLL; 
HANDLE StartupThread;
HANDLE clientListenThread;
HANDLE pauseThread;
HANDLE startupThread;
HANDLE clientMutex;
DWORD dwID[3];
SOCKET listener;
vector<CLIENT> clients;
WSADATA wsaData;
SOCKET listenSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;
struct addrinfo *result = NULL,
    hints;
int  iResult;
int  recvbuflen = DEFAULT_BUFLEN;

OapiCommandParser oapiParser;
VesselCommandParser vesselParser;
XRCtlCommandParser xrCtrlParser;
NavigationCommandParser navParser;
ObjectCommandParser objectParser;
PlanetCommandParser planetParser;
SubscriptionParser subscripParser;

Gbody* sun;

int SendText_Intercept = 0;
string SendText_Buf;

// Just like parseCommand, but sends ID instead of command
// back to the client
void parseCommandID(long lParam, string Command, int ID)
{
    MessageStream ss;
    ss << ID;
    string IDstring = ss.str();

    if (Command.empty())
    {
        SendText(lParam, IDstring, "ERR00");
        return;
    }

    vector<string> tokens = stringSplit(Command, ":");
    if (2 > tokens.size() || tokens.at(0).empty())
    {
        SendText(lParam, IDstring, "ERR01");
        return;
    }

    string area = uCase(tokens.at(0));

    if ("SHIP" == area || "FOCUS" == area )
    {
        SendText(lParam, IDstring, vesselParser.parse(lParam, Command));
    }
    else if ("XCTL" == area)
    {
        SendText(lParam, IDstring, xrCtrlParser.parse(lParam, Command));
    }
    else if ("NAV" == area)
    {
        SendText(lParam, IDstring, navParser.parse(lParam, Command));
    }
    else if ("ORB" == area || "CAMERA" == area)
    {
        SendText(lParam, IDstring, oapiParser.parse(lParam, Command));
    }
    else if ("BODY" == area)
    {
        SendText(lParam, IDstring, planetParser.parse(lParam, Command));
    }
    else if ("OBJ" == area)
    {
        SendText(lParam, IDstring, objectParser.parse(lParam, Command));
    }
    else 
    {
        SendText(lParam, IDstring, "ERR02");
    }

}

void parseCommand(long lParam, string Command)
{
    if (Command.empty())
    {
        SendText(lParam, Command, "ERR00");
        return;
    }

    vector<string> tokens = stringSplit(Command, ":");
    if (2 > tokens.size() || tokens.at(0).empty())
    {
        SendText(lParam, Command, "ERR01");
        return;
    }

    string area = uCase(tokens.at(0));

    if ("SHIP" == area || "FOCUS" == area )
    {
        SendText(lParam, Command, vesselParser.parse(lParam, Command));
    }
    else if ("SUBSCRIBE" == area || "UNSUBSCRIBE" == area )
    {
        SendText(lParam, Command, subscripParser.parse(lParam, Command));
    }
    else if ("XCTL" == area)
    {
        SendText(lParam, Command, xrCtrlParser.parse(lParam, Command));
    }
    else if ("NAV" == area)
    {
        SendText(lParam, Command, navParser.parse(lParam, Command));
    }
    else if ("ORB" == area || "CAMERA" == area)
    {
        SendText(lParam, Command, oapiParser.parse(lParam, Command));
    }
    else if ("BODY" == area)
    {
        SendText(lParam, Command, planetParser.parse(lParam, Command));
    }
    else if ("OBJ" == area)
    {
        SendText(lParam, Command, objectParser.parse(lParam, Command));
    }
    else 
    {
        SendText(lParam, Command, "ERR02");
    }
}
void readConfig() {
    bool foundParam = false;
    char prt[8];
    strcpy_s(port, 8, DEFAULT_PORT);
    FILEHANDLE f = oapiOpenFile("OrbConnect.cfg", FILE_IN, CONFIG);
    if (f) {
        oapiWriteLog("OrbConnect::Found Config File");
        foundParam = oapiReadItem_string(f, "PORT", prt);
        if (foundParam) {
            oapiWriteLog("OrbConnect::Found Port entry");
            strcpy_s(port, 8, prt);
        }
        oapiCloseFile(f, FILE_IN);
    }
}

DLLCLBK void opcOpenRenderViewport (HWND renderWnd,DWORD width,DWORD height,BOOL fullscreen)
{ 
    if (startup())
    {
        oapiWriteLog("OrbConnect::Module Initialized");
    }

	http_init();
}

DLLCLBK void opcCloseRenderViewport (void)
{
	http_finish();

    unsigned int idx;
    for(idx = 0; idx < clients.size(); idx++)
    {
        send(clients.at(idx).TCPClient,"Simulation Closed.\r\n",20,0);
        closesocket(clients.at(idx).TCPClient);
    }
    // terminate all threads
    WSACleanup();

    vesselAxesSet.resize(0);
    vesselAxesValues.resize(0);
    delete sun;
    
    oapiWriteLog("OrbConnect::Module Shutdown");
}


bool startup()
{
    int sz = oapiGetVesselCount() * 3;
	vesselAxesSet.resize(sz,false);
	vesselAxesValues.resize(sz,0.0);

    initializeSolarSystem();

    char msgBuf[DEFAULT_BUFLEN];
    readConfig();
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        sprintf_s(msgBuf, "OrbConnect::WSAStartup failed with accepted socket: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
        return false;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 )
    {
        sprintf_s(msgBuf, "OrbConnect::getaddrinfo with accepted socket: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listener = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listener == INVALID_SOCKET)
    {
        sprintf_s(msgBuf, "OrbConnect::socket failed with accepted socket: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    // Setup the TCP listening socket
    iResult = bind( listener, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        sprintf_s(msgBuf, "OrbConnect::bind failed with accepted socket: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
        freeaddrinfo(result);
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(listener, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        sprintf_s(msgBuf, "OrbConnect::listen failed with accepted socket: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
        closesocket(listener);
        WSACleanup();
        return 1;
    }

    clientListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientListenThreadFunction, NULL, 0, &dwID[0]);

    return true;
}


long WINAPI clientListenThreadFunction(long lParam)
{
    char msgBuf[DEFAULT_BUFLEN];
    CLIENT newClient;
    // Accept a client socket
    while ((clientSocket = accept(listener, NULL, NULL)) != INVALID_SOCKET)
    {
        memset(&newClient, 0, sizeof(CLIENT));
        newClient.TCPClient = clientSocket;
        clientSocket = INVALID_SOCKET;

        if (newClient.TCPClient == SOCKET_ERROR)
        {
            sprintf_s(msgBuf, "OrbConnect::Error with accepted socket: %d", WSAGetLastError());
            oapiWriteLog(msgBuf);

            return 1;
        }

        char mutexId[32];
        sprintf_s(mutexId, "client%d", clients.size());
        newClient.mutex = CreateMutex(NULL, false, mutexId);
        clients.push_back(newClient);
        clients.at(clients.size() - 1).ClientReceiveThread = 
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientReceiveThreadFunction, LPVOID(clients.size() - 1), 0, &dwID[1]);
    }

    if (clientSocket == INVALID_SOCKET) {
        sprintf_s(msgBuf, "OrbConnect::accept failed: %d", WSAGetLastError());
        oapiWriteLog(msgBuf);
//        oapiWriteLog("OrbConnect::accept failed");
        closesocket(listener);
        WSACleanup();

        return 1;
    }

    return 0;
}


/* This function handles a single client for the lifetime of that client */
long WINAPI ClientReceiveThreadFunction(long lParam)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    char msgBuf[DEFAULT_BUFLEN];
    //	sprintf_s(msgBuf, "OrbConnect::Listening to client %d", lParam);
    //	oapiWriteLog(msgBuf);

    // Process incoming messages while there are incoming messages available
    while ((iResult = recv(clients.at(lParam).TCPClient, recvbuf, recvbuflen, 0)) > 0)
    {
        //		sprintf_s(msgBuf, "OrbConnect::Received message from client %d. iResult: %d", lParam, iResult);
        //		oapiWriteLog(msgBuf);
        WaitForSingleObject(clients.at(lParam).mutex, INFINITE);

        string message(recvbuf);
        message = message.substr(0, iResult);
        //		sprintf_s(msgBuf, "OrbConnect::Adding message from client %d. Msg: %s", lParam, message);
        //		oapiWriteLog(msgBuf);
        clients.at(lParam).ReceiveBuffer.append(message);
        //		bool PendingCommands = 0;

        // Transfer complete commands from buffer to queue
        while (clients.at(lParam).ReceiveBuffer.find("\r\n") != string::npos)
        {
            //			PendingCommands = 1;
            clients.at(lParam).CommandQueue.push_back(clients.at(lParam).ReceiveBuffer.substr(0,clients.at(lParam).ReceiveBuffer.find("\r\n")));
            clients.at(lParam).ReceiveBuffer = clients.at(lParam).ReceiveBuffer.substr(clients.at(lParam).ReceiveBuffer.find("\r\n") + 2);
        }
        //		if (PendingCommands)
        //		{
        //			clients.at(lParam).CommandProcessor = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CommandProcessorThreadFunction,(LPVOID)lParam,0,&dwID[2]);
        //		}

        ReleaseMutex(clients.at(lParam).mutex);

        memset(recvbuf,0,DEFAULT_BUFLEN);
    }
    if (iResult == 0)
    {
        oapiWriteLog("OrbConnect::recv returned 0; closing");
        closesocket(clients.at(lParam).TCPClient);
        clients.at(lParam).TCPClient = INVALID_SOCKET;
        return 1;
    }
    else
    {
        sprintf_s(msgBuf, "OrbConnect::recv failed for client %d with error %d; closing", lParam, WSAGetLastError());
        oapiWriteLog(msgBuf);
        closesocket(clients.at(lParam).TCPClient);
        clients.at(lParam).TCPClient = INVALID_SOCKET;
        return 1;
    }

    return 0;
}

long WINAPI CommandProcessorThreadFunction()
{
    unsigned int clientIdx = 0;

    while(!terminatePauseThread)
    {
        for(clientIdx = 0; clientIdx < clients.size(); clientIdx++)
        {
            processClient(clientIdx);
            if (terminatePauseThread)
            {
                break;
            }
        }
        subscripParser.ProcessItems();
    }

    return 0;
}

void processClient(long clientIdx)
{
    unsigned int cmdIdx = 0;
    char msgBuf[1024];
    WaitForSingleObject(clients.at(clientIdx).mutex,INFINITE);
    for (cmdIdx = 0; cmdIdx < clients.at(clientIdx).CommandQueue.size(); cmdIdx++)
    {
        sprintf_s(msgBuf, "Processing message %d.%d", clientIdx, cmdIdx);
        //		oapiWriteLog(msgBuf);
        parseCommand(clientIdx, clients.at(clientIdx).CommandQueue.at(cmdIdx));
    }
    //	oapiWriteLog("OrbConnect:: Client complete");
    clients.at(clientIdx).CommandQueue.erase(clients.at(clientIdx)
        .CommandQueue.begin(), clients.at(clientIdx).CommandQueue.end());
    //	oapiWriteLog("OrbConnect:: Commands cleared");
    //	closesocket(clients.at(clientIdx).TCPClient);
    //	oapiWriteLog("OrbConnect:: Socket Released");
    ReleaseMutex(clients.at(clientIdx).mutex);
}

DLLCLBK void opcPause(bool paused)
{
    if (paused)
    {
        terminatePauseThread = false;
        pauseThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CommandProcessorThreadFunction, NULL, 0, &dwID[2]);
    }
    else
    {
        terminatePauseThread = true;
    }
}

double magnitude(const VECTOR3 &vec)
{
    return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

DLLCLBK void opcPostStep (double SimT, double SimDT, double mjd)
{
    gatherNavmodeData();
    processCommands();
    subscripParser.ProcessItems();

	http_postStep();
}

/**
* PITCH = 0,
* BANK = 1,
* ALT = 2,
*/
void gatherNavmodeData()
{
    int idx = 0;
    bool set = false;
    OBJHANDLE obj = NULL;
    VESSEL* ship = NULL;
    if (vesselAxesSet.size() == 0) return;
	int nbrVessels = (int) oapiGetVesselCount();

	if (nbrVessels * 3 > (int)vesselAxesSet.max_size()) {
		vesselAxesSet.resize(nbrVessels * 3);
		vesselAxesValues.resize(nbrVessels * 3);
	}

    for (idx = 0; idx < nbrVessels; idx++) {
        obj = oapiGetVesselByIndex(idx);
        if (obj == NULL) {
            continue;
        }
        ship = oapiGetVesselInterface(obj);
        set = ship->GetNavmodeState(NAVMODE_HLEVEL);
        if (set != vesselAxesSet[3 * idx + AP_PITCH]) {
            vesselAxesSet[3 * idx + AP_PITCH] = set;
            if (set) {
                vesselAxesValues[3 * idx + AP_PITCH] = 0;
            }
        }
        if (set != vesselAxesSet[3 * idx + AP_BANK]) {
            vesselAxesSet[3 * idx + AP_BANK] = set;
            if (set) {
                vesselAxesValues[3 * idx + AP_BANK] = 0;
            }
        }
        set = ship->GetNavmodeState(NAVMODE_HOLDALT);
        if (set != vesselAxesSet[3 * idx + AP_ALT]) {
            vesselAxesSet[3 * idx + AP_ALT] = set;
            vesselAxesValues[3 * idx + AP_ALT] = set ? ship->GetAltitude() : 0;
        }
    }
}

void processCommands()
{
    unsigned int clientIdx = 0;

    for(clientIdx = 0; clientIdx < clients.size(); clientIdx++)
    {
        processClient(clientIdx);
    }
}

void SendText(long lParam, string Command, string Message)
{
	if ( SendText_Intercept ) {
		SendText_Buf = Message;
		return ;
	}

	int length = Message.length() + Command.length() + 3;
	Command.append("=");
	Command.append(Message);
	Command.append("\r\n");
	char SendText[1024];
	strcpy_s(SendText, Command.c_str());
	//	oapiWriteLog(SendText);
	send(clients.at(lParam).TCPClient, SendText, length, 0);
}

vector<string> stringSplit(string str, string delim)
{
    int cutAt;
    vector<string> results;
    while( (cutAt = str.find_first_of(delim)) != str.npos )
    {
        if(cutAt > 0)
        {
            results.push_back(str.substr(0,cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        results.push_back(str);
    }

    return results;
}

string uCase(string InString)
{
    for (unsigned int j=0; j < InString.length(); j++)
    {
        InString[j]=toupper(InString[j]);
    }

    return InString;
}

void initializeSolarSystem()
{
    char buf[128];
    char msgBuf[DEFAULT_BUFLEN];
    int totalbodies=oapiGetGbodyCount();
    sprintf_s(msgBuf, "OrbConnect::Initializing solar system with %d bodies", totalbodies);
    oapiWriteLog(msgBuf);
    vector<Gbody*> templist; // used for easy accesss to the bodies rather than traversing the tree
    for (int idx = 0; idx < totalbodies; idx++)
    {
        OBJHANDLE hObj = oapiGetGbodyByIndex(idx);
        oapiGetObjectName(hObj, buf, 128);
        
        Gbody* body = new Gbody(buf);
        body->setHandle(hObj);
        body->setMass(oapiGetMass(hObj));
        if(idx == 0)
        {
            // The central star
            body->setSoiSize2(1e100);//Infinite SOI
            sun = body;
        }
        else
        {
            // gravbodyratio2 and soisize
            // Find the parent body (what it is orbiting)
            vector<Gbody*>::iterator iter = templist.begin();
            Gbody* currparent;
            double currdistance2;
            while(iter != templist.end())
            {
                Gbody* entry = (*iter);

                VECTOR3 pos;
                oapiGetRelativePos(body->getHandle(), entry->getHandle(), &pos);
                double distance2 = dotp(pos, pos);
                if(distance2 < entry->getSoiSize2())
                {
                    currparent = entry;
                    currdistance2 = distance2;
                }

                iter++;
            }
            body->setParent(currparent);
            if(body->getParent()->getChildren()->size() > 0)
            {
                body->setPrevious(body->getParent()->getChildren()->back());
                body->getPrevious()->setNext(body);
            }
            body->getParent()->getChildren()->push_back(body);
            body->setGravBodyRatio2(pow(body->getMass() / body->getParent()->getMass(), 0.8));
            body->setSoiSize2(currdistance2 * body->getGravBodyRatio2());//Internal soi size
        }
        templist.push_back(body); // add it to the temp list
    }
}
