/*
 * Edge.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef EDGE_H_
#define EDGE_H_

#include "Location.h"
#include "Guard.h"
#include "Assignment.h"
#include "LinearPredicate.h"
#include <string>

using namespace std;

class Edge {
public:
	Edge();
	Edge(Location source, Location destination, Guard guard,
			Assignment assignment, string name);
	virtual ~Edge();

	void setSource(Location source);
	Location getSource();

	void setDestination(Location destination);
	Location getDestination();

	void setGuard(Guard guard);
	Guard getGuard();

	void setAssignment(Assignment assignment);
	Assignment getAssignment();

	const string& getName() const;
	void setName(const string& name);

private:
	string _name;
	Location _source;
	Location _destination;
	Guard _guard;
	Assignment _assignment;

};

#endif /* EDGE_H_ */
