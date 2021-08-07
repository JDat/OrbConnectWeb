#ifndef _ORB_CONNECT_H
#define _ORB_CONNECT_H

#include <vector>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "windows.h"
#include "OapiCommandParser.h"
#include "ObjectCommandParser.h"
#include "PlanetCommandParser.h"
#include "XRCtlCommandParser.h"
#include "NavigationCommandParser.h"
#include "SubscriptionParser.h"
 
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "37777"

using namespace std;

typedef struct {
	SOCKET TCPClient;
	sockaddr * CLIaddr;
	HANDLE ClientReceiveThread,CommandProcessor;
	HANDLE mutex;
	string ReceiveBuffer;
	vector<string> CommandQueue;
} CLIENT;

// *** GLOBALS ***
extern enum apSettings {
    AP_PITCH = 0,
    AP_BANK,
    AP_ALT,
  };

extern std::vector<bool> vesselAxesSet;
extern std::vector<double> vesselAxesValues;

extern char port[8];
extern HINSTANCE g_hDLL; 
extern HANDLE StartupThread;
extern HANDLE clientListenThread;
extern HANDLE pauseThread;
extern HANDLE startupThread;
extern HANDLE clientMutex;
extern DWORD dwID[];
extern SOCKET listener;
extern vector<CLIENT> clients;
extern WSADATA wsaData;
extern SOCKET listenSocket;
extern SOCKET clientSocket;
extern struct addrinfo *result,
                 hints;
extern int  iResult;
extern int  recvbuflen;

extern Gbody* sun;
extern OapiCommandParser oapiParser;
extern VesselCommandParser vesselParser;
extern XRCtlCommandParser xrCtrlParser;
extern NavigationCommandParser navParser;
extern ObjectCommandParser objectParser;
extern PlanetCommandParser planetParser;

long WINAPI StartupThreadFunction(long lParam);
long WINAPI clientListenThreadFunction(long lParam);
long WINAPI ClientReceiveThreadFunction(long lParam);
long WINAPI CommandProcessorThreadFunction();

void parseCommand(long lParam, string Command);
void parseCommandID(long lParam, string Command, int ID);

bool startup();
void gatherNavmodeData();
void processCommands();
void processClient(long clientIdx);
void clearGbodies();
void initializeSolarSystem();
void deleteGBody(Gbody* body);

vector<string> stringSplit(string str, string delim);

void SendText(long lParam, string Command, string Message);
extern int SendText_Intercept;
extern string SendText_Buf;
void parseCommand(long lParam, string Command);

string uCase(string InString);

#endif