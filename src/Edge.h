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
	/*
	 * Constructors
	 */
	Edge(const Location& source, const Location& destination, const Guard& guard,
			const Assignment assignment, const string name);

	Edge(const Location& source, const Location& destination, const Guard& guard,
			const string name);

	Edge(const Location& source, const Location& destination, const Assignment& assignment,
			const string name);

	Edge(const Location& source, const Location& destination, const string& name);

	virtual ~Edge();

	/*
	 * Setters and getters
	 */
	void setSource(const Location& source);
	const Location& getSource();

	void setDestination(const Location& destination);
	const Location& getDestination();

	void setGuard(const Guard& guard);
	const Guard& getGuard();

	void setAssignment(const Assignment& assignment);
	const Assignment& getAssignment();

	void setName(const string& name);
	const string& getName() const;

	bool isHasAssignment() const;
	void setHasAssignment(bool hasAssignment);
	bool isHasGuard() const;
	void setHasGuard(bool hasGuard);

private:
	string _name;
	Location _source;
	Location _destination;
	Guard _guard;
	Assignment _assignment;
	bool _hasAssignment;
	bool _hasGuard;

};

#endif /* EDGE_H_ */
