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
* @Author Brandon Bolling
*/

#include "SubscriptionParser.h"

int Subscription::nextID = 1000;

extern void parseCommandID(long lParam, string Command, int ID);
extern vector<CLIENT> clients;

SubscriptionParser::SubscriptionParser(void) :
mutex(CreateMutex(NULL, false, NULL)), timeslot(NUM_BUCKETS)
{
    double st = oapiGetSysTime();
    double intpart;

    // Will be in the range 0 - 19
    lastTimeslot = (int)(modf(st, &intpart) * NUM_BUCKETS);
}

string SubscriptionParser::parse(long lparam, string command)
{
    string message = "ERR02";
    vector<string> tokens = stringSplit(uCase(command), ":");

    if ("UNSUBSCRIBE" == uCase(tokens.at(0))) {
        return RemoveRequest(tokens.at(1));
    }

    // Cannot subscribe to a subscription
    if (command.find("SUBSCRIBE", 9) != string::npos) {
        return "ERR04";
    }

    command = command.substr(command.find(':', command.find(':') + 1) + 1);

    double freq;
    if (doubleFromString(tokens.at(1), freq))
    {
        double delay = NUM_BUCKETS / freq;
        if (delay > NUM_BUCKETS)
        {
            delay = NUM_BUCKETS;
        }

        if (delay < 1)
        {
            delay = 1;
        }

        Subscription* sub = new Subscription(lparam, delay, command);
        message = AddRequest(sub);
    }

    return message;
}

int SubscriptionParser::ProcessItems(void)
{
    // Get timeslot
    double st = oapiGetSysTime();
    double intpart;
    int time = (int)(modf(st, &intpart) * NUM_BUCKETS);

    WaitForSingleObject(mutex,INFINITE);
    if (lastTimeslot == time) { // don't re-process a timeslot until we've processed something else
		ReleaseMutex(mutex);
        return -1;
    }

    int newtime;
    std::set<int>::iterator itr;
    Subscription *req;
    vector<stack<Subscription*>> newState(timeslot);

    // loop on the stack in that slot
    for (int slot = time; slot != lastTimeslot; slot = (slot + NUM_BUCKETS - 1) % NUM_BUCKETS)
    {
        while (!newState[slot].empty())
        {
            newState[slot].pop();
        }
        while(!timeslot[slot].empty())
        {
            req = timeslot[slot].top(); // Get the first event to handle
            (timeslot.at(slot)).pop(); 

            itr = removedRequests.find(req->ID);
            if (itr != removedRequests.end() || (clients.at(req->clientNum)).TCPClient == INVALID_SOCKET )
            { 
                delete req; // Ignore it if it was removed

                if (itr != removedRequests.end())
                {
                    removedRequests.erase(itr); // remove the deletion report
                }
                continue;
            }

            parseCommandID(req->clientNum, req->request, req->ID);
            //oapiWriteLog("OrbConnect:: Processing subscription");
            req->remainder += req->delay;
            int thisdelay = (int)(req->remainder);
            req->remainder = req->remainder - (double)thisdelay;
            newtime = (time + thisdelay) % NUM_BUCKETS;
            newState[newtime].push(req);
        }
    }

    timeslot = newState;

    lastTimeslot = time;

    ReleaseMutex(mutex);

    return 0;
}

string SubscriptionParser::AddRequest(Subscription* req)
{
    double st = oapiGetSysTime();
    double intpart;
    int time = (int)(modf(st, &intpart) * NUM_BUCKETS) + 1;
    if (time >= NUM_BUCKETS)
        time = 0;

    MessageOutStream o;

    if (!(o << req->ID)) 
    {
        delete req;
        return "ERR02";
    }

    WaitForSingleObject(mutex, INFINITE);
    timeslot[time].push(req);
    ReleaseMutex(mutex);

    return o.str();
}

string SubscriptionParser::RemoveRequest(string id)
{
    int result;
    if (intFromString(id, result))
    {
        removedRequests.insert(result);
        return "OK";
    }

    return "ERR04";
}