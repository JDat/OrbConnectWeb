#ifndef _NAVIGATION_COMMAND_PARSER_H
#define _NAVIGATION_COMMAND_PARSER_H

#include "CommandParser.h"
#include "Orbitersdk.h"

using namespace std;

// Navaid types
#define	FLAG_VOR	0x001
#define	FLAG_VTOL	0x002
#define	FLAG_ILS	0x004
#define	FLAG_IDS	0x008
#define	FLAG_XPDR	0x010
#define	FLAG_ALL	(FLAG_VOR|FLAG_VTOL|FLAG_ILS|FLAG_IDS|FLAG_XPDR)

typedef struct {
    string id;
    DWORD type;
    string descr;
    int channel;
    VECTOR3 gpos;
    double dist;
    bool inRange;
} XMTR_T;


// In OrbConnect.cpp
double magnitude(const VECTOR3 &vec);

// In VesselCommandParser.cpp
string loadVessel(VESSEL2** pVessel, vector<string> tokens);

class NavigationCommandParser : public VesselCommandParser {
public:
	string parse(long lparam, string command);

protected:
	string getElements1(VESSEL2* pVessel);
	string getElements2(VESSEL2* pVessel, vector<string> tokens);
	string getEquPos(VESSEL2* pVessel);
	string getGlobalPos(VESSEL2* pVessel);
	string getGlobalVel(VESSEL2* pVessel);
	string getVesselRelativePos(VESSEL2* pVessel, vector<string> tokens);
	string getVesselRelativeVel(VESSEL2* pVessel, vector<string> tokens);
	string getVesselRelativePosAndVel(VESSEL2* pVessel, vector<string> tokens);
	string getGravContribution(VESSEL2* pVessel, vector<string> tokens);
	string getGravReference(VESSEL2* pVessel);
	string getNavaids(VESSEL2* pVessel, vector<string> tokens);
	string getNavaidRelPos(VESSEL2* pVessel, vector<string> tokens);
    string getNavaidRelVel(VESSEL2* pVessel, vector<string> tokens);

	vector<XMTR_T> findXmtrs(VESSEL2* pVessel);
	VECTOR3 getVesselRelativePosition(VESSEL2* pVessel, VECTOR3 refGlobalPos, int refFrame);
};
#endif