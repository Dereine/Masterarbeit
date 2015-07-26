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


Edge::Edge(Location source, Location destination, LinearPredicate guard,
		LinearPredicate assignment, string name) {
	_source = source;
	_destination = destination;
	_guard = guard;
	_assignment = assignment;
	_name = name;
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

void Edge::setGuard(LinearPredicate guard) {
	_guard = guard;
}
LinearPredicate Edge::getGuard() {
	return _guard;
}

void Edge::setAssignment(LinearPredicate assignment) {
	_assignment = assignment;
}
LinearPredicate Edge::getAssignment() {
	return _assignment;
}

const string& Edge::getName() const {
	return _name;
}

void Edge::setName(const string& name) {
	_name = name;
}
