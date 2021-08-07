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
*    This code is modified from code originally written for the transx MFD
*    by Duncan Sharpe, Steve Arch
* @copyright 2008 Randy Stearns
* @Author Randy Stearns, Duncan Sharpe, Steve Arch
*/

#include "MessageStream.h"
#include "Gbody.h"

Gbody::Gbody(char* nm) {
    name = nm;
    parent = NULL;
    next = NULL;
    previous = NULL;
    children = new vector<Gbody*>();
}

Gbody::~Gbody() {
    if (children == NULL) return;

    vector<Gbody*>::iterator iter;
    for(iter = children->begin(); iter != children->end(); iter++) {
        delete *iter;
    }
    delete children;
    
    children = NULL;
}
void Gbody::addChild(Gbody* child) {
    children->push_back(child);
}

vector<Gbody*>* Gbody::getChildren() {
    return children;
}

/**
 * Primary planets are identified the same as the Star.
 */
string Gbody::toCsvString(string lastName) {
    vector<Gbody*>::iterator iter;
    MessageStream ss;
    string dotName;
    if (parent == NULL || parent->getParent() == NULL ) {
        dotName = name;
    } else {
        dotName = lastName + "." + name;
    }
    ss << dotName << ",";
    if (children != NULL && children->size() > 0) {
        for(iter = children->begin(); iter != children->end(); iter++) {
            Gbody* child = *iter;
            ss << child->toCsvString(dotName);
        }
    }
    
    return ss.str();
}

	string Gbody::getName() {
	    return name;
	}
	void Gbody::setName(string value) {
	    name = value;
	}
	OBJHANDLE Gbody::getHandle() {
	    return bodyhandle;
	}
	void Gbody::setHandle(OBJHANDLE hObj) {
	    bodyhandle = hObj;
	}
	Gbody* Gbody::getParent() {
	    return parent;
	}
	void Gbody::setParent(Gbody* value) {
	    parent = value;
	}
	Gbody* Gbody::getNext() {
	    return next;
	}
	void Gbody::setNext(Gbody* value) {
	    next = value;
	}
	Gbody* Gbody::getPrevious() {
	    return previous;
	}
	void Gbody::setPrevious(Gbody* value) {
	    previous = value;
	}
	double Gbody::getSoiSize2() {
	    return soisize2;
	}
	void Gbody::setSoiSize2(double value) {
	    soisize2 = value;
	}
	double Gbody::getGravBodyRatio2() {
	    return gravbodyratio2;
	}
	void Gbody::setGravBodyRatio2(double value) {
	    gravbodyratio2 = value;
	}
	double Gbody::getMass() {
	    return mass;
	}
	void Gbody::setMass(double value) {
	    mass = value;
	}
