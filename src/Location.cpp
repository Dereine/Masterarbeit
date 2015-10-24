/*
 * Location.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Location.h"

Location::Location() {
}

Location::Location(int id, string name, Invariant invariant,
		vector<Bound> bounds, bool initial) {
	_id = id;
	_bounds = bounds;
	_invariant = invariant;
	_name = name;
	_initial = initial;
}

Location::~Location() {
	// TODO Auto-generated destructor stub
}

const unsigned int Location::getId() const {
	return _id;
}

void Location::setId(int id) {
	this->_id = id;
}

const string& Location::getName() const {
	return _name;
}

void Location::setName(const string& name) {
	_name = name;
}

const Invariant& Location::getInvariant() const {
	return _invariant;
}

void Location::setInvariant(const Invariant& invariant) {
	_invariant = invariant;
}

const vector<Bound>& Location::getBounds() const {
	return _bounds;
}

void Location::setBounds(const vector<Bound>& bounds) {
	_bounds = bounds;
}

bool Location::isInitial() const {
	return _initial;
}

void Location::setInitial(bool initial) {
	_initial = initial;
}

void Location::flowToSpaceExXML(string& flowString) {
	flowString += "      <flow>";
	Bound bound;
	for (size_t i = 0; i < _bounds.size(); i++) {
		bound = _bounds[i];
		if (i > 0)
			flowString += "&amp;";
		//else
			//flowString += "&amp; "
		flowString += bound.getVariable().getName() + "' &gt;= "  +
				bound.getConstantLow().getValueString() + "\n&amp;" +
				bound.getVariable().getName() + "' &lt;= "  +
				bound.getConstantUp().getValueString() + "\n";
	}
	LinearPredicate linPred;
	for (size_t i = 0; i < _invariant.getLinPreds().size(); i++) {
		linPred = _invariant.getLinPreds()[i];
		if (linPred.isFlow())
			flowString += "&amp;" +
			linPred.toStringSpaceExXML(true, false) + "\n";
	}
	flowString += "      </flow>\n";
}
