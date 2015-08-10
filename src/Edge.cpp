/*
 * Edge.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Edge.h"

Edge::Edge() {
	// TODO Auto-generated constructor stub
}


Edge::Edge(Location source, Location destination, Guard guard,
		Assignment assignment, string name) {
	_source = source;
	_destination = destination;
	_guard = guard;
	_assignment = assignment;
	_name = name;
	_hasAssignment = true;
	_hasGuard = true;
}

Edge::Edge(Location source, Location destination, Guard guard,
		string name) {
	_source = source;
	_destination = destination;
	_guard = guard;
	_name = name;
	_hasAssignment = false;
	_hasGuard = true;
}

Edge::Edge(Location source, Location destination, Assignment assignment,
		string name) {
	_source = source;
	_destination = destination;
	_assignment = assignment;
	_name = name;
	_hasAssignment = true;
	_hasGuard = false;
}

Edge::Edge(Location source, Location destination, string name) {
	_source = source;
	_destination = destination;
	_name = name;
	_hasAssignment = false;
	_hasGuard = false;
}



Edge::~Edge() {
	// TODO Auto-generated destructor stub
}

void Edge::setSource(Location source) {
	_source = source;
}
Location Edge::getSource() {
	return _source;
}

void Edge::setDestination(Location destination) {
	_destination = destination;
}
Location Edge::getDestination() {
	return _destination;
}

void Edge::setGuard(Guard guard) {
	_guard = guard;
}
Guard Edge::getGuard() {
	return _guard;
}

void Edge::setAssignment(Assignment assignment) {
	_assignment = assignment;
}
Assignment Edge::getAssignment() {
	return _assignment;
}

const string& Edge::getName() const {
	return _name;
}

void Edge::setName(const string& name) {
	_name = name;
}

bool Edge::isHasAssignment() const {
	return _hasAssignment;
}

void Edge::setHasAssignment(bool hasAssignment) {
	_hasAssignment = hasAssignment;
}

bool Edge::isHasGuard() const {
	return _hasGuard;
}

void Edge::setHasGuard(bool hasGuard) {
	_hasGuard = hasGuard;
}
