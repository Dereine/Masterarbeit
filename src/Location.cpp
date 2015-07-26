/*
 * Location.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Location.h"

Location::Location() {
}

Location::Location(int id, std::string name, Invariant invariant,
		std::vector<Bound> bounds, bool initial) {
	_id = id;
	_bounds = bounds;
	_invariant = invariant;
	_name = name;
	_initial = initial;
}

Location::~Location() {
	// TODO Auto-generated destructor stub
}

int Location::getId() {
	return _id;
}

void Location::setId(int id) {
	this->_id = id;
}

const std::string& Location::getName() const {
	return _name;
}

void Location::setName(const std::string& name) {
	_name = name;
}

const Invariant& Location::getInvariant() const {
	return _invariant;
}

void Location::setInvariant(const Invariant& invariant) {
	_invariant = invariant;
}

const std::vector<Bound>& Location::getBounds() const {
	return _bounds;
}

void Location::setBounds(const std::vector<Bound>& bounds) {
	_bounds = bounds;
}

bool Location::isInitial() const {
	return _initial;
}

void Location::setInitial(bool initial) {
	_initial = initial;
}
