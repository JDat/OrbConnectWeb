#pragma once

#include "OrbConnect.h"
#include "CommandParser.h"

#include <stack>
#include <set>
#include <math.h>

#define NUM_BUCKETS 20

using namespace std;

struct Subscription
{
	Subscription(void) : 
		ID(nextID++) {}

	Subscription(long client, double delayVal, string command) : 
		ID(nextID++), clientNum(client), delay(delayVal), request(command), remainder(0) {}

	long clientNum; // client to reply to
	static int nextID;
	int ID; // ID for this transaction
	string request; // Character string of the request
	//int length; // Length of the character string, cached for easy dropping purpose
	double delay; // Delay of reporting in # of buckets
	double remainder;
};


class SubscriptionParser : public CommandParser
{
public:
	string parse(long lparam, string command);

	SubscriptionParser(void);

	// processes events in the current time slot
	int ProcessItems();
	string AddRequest(Subscription* req);
	string RemoveRequest(string id);

private:
	vector<stack<Subscription*>> timeslot;
	set<int> removedRequests;
	int lastTimeslot;
	HANDLE mutex;
};

extern SubscriptionParser subscripParser;