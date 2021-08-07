#ifndef _GBODY_H
#define _GBODY_H

#include <vector>
#include <string>
#include <sstream>
#include "Orbitersdk.h"

using namespace std;

class Gbody {
private:
	OBJHANDLE bodyhandle;
	string name;
	Gbody *parent, *next, *previous; // next and previous bodies orbiting same body
	vector<Gbody*>* children;
	double soisize2;
	double gravbodyratio2;
	double mass;

public:
    Gbody(char* nm);
    virtual ~Gbody();
	void addChild(Gbody* child);
	vector<Gbody*>* getChildren();
	OBJHANDLE getHandle();
	void setHandle(OBJHANDLE hObj);
	string getName();
	void setName(string);
	Gbody* getParent();
	void setParent(Gbody* parent);
	Gbody* getNext();
	void setNext(Gbody* parent);
	Gbody* getPrevious();
	void setPrevious(Gbody* parent);
	double getSoiSize2();
	void setSoiSize2(double value);
	double getGravBodyRatio2();
	void setGravBodyRatio2(double value);
	double getMass();
	void setMass(double value);
	string toCsvString(string parent);
};
#endif