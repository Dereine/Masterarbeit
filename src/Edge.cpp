/*
 * Edge.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Edge.h"

Edge::Edge() {
}


Edge::Edge(const Location& source, const Location& destination, const Guard& guard,
		const Assignment assignment, const string name) {
	_source = source;
	_destination = destination;
	_guard = guard;
	_assignment = assignment;
	_name = name;
	_hasAssignment = true;
	_hasGuard = true;
}

Edge::Edge(const Location& source, const Location& destination, const Guard& guard,
		const string name) {
	_source = source;
	_destination = destination;
	_guard = guard;
	_name = name;
	_hasAssignment = false;
	_hasGuard = true;
}

Edge::Edge(const Location& source, const Location& destination, const Assignment& assignment,
		const string name) {
	_source = source;
	_destination = destination;
	_assignment = assignment;
	_name = name;
	_hasAssignment = true;
	_hasGuard = false;
}

Edge::Edge(const Location& source, const Location& destination, const string& name) {
	_source = source;
	_destination = destination;
	_name = name;
	_hasAssignment = false;
	_hasGuard = false;
}



Edge::~Edge() {
	// TODO Auto-generated destructor stub
}

void Edge::setSource(const Location& source) {
	_source = source;
}
const Location& Edge::getSource() {
	return _source;
}

void Edge::setDestination(const Location& destination) {
	_destination = destination;
}

const Location& Edge::getDestination() {
	return _destination;
}

void Edge::setGuard(const Guard& guard) {
	_guard = guard;
}

const Guard& Edge::getGuard() {
	return _guard;
}

void Edge::setAssignment(const Assignment& assignment) {
	_assignment = assignment;
}
const Assignment& Edge::getAssignment() {
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
