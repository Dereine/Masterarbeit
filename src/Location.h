/*
 * Location.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef LOCATION_H_
#define LOCATION_H_

#include "Invariant.h"
#include "Bound.h"
#include <string>
#include <vector>

using namespace std;

class Location {

public:
	Location();
	Location(int id, string name, Invariant invariant,
			vector<Bound> bounds, bool initial);
	virtual ~Location();

	/*
	 * Setters and getters
	 */
	const unsigned int getId() const;
	void setId(int id);

	const string& getName() const;
	void setName(const string& name);

	const Invariant& getInvariant() const;
	void setInvariant(const Invariant& invariant);

	const vector<Bound>& getBounds() const;
	void setBounds(const vector<Bound>& bounds);

	bool isInitial() const;
	void setInitial(bool initial);

	/*
	 * Creates a string of the flow for SpaceEx.
	 */
	void flowToSpaceExXML(string& flowString);

private:
	unsigned int	_id;
	string 			_name;
	Invariant 		_invariant;
	vector<Bound> 	_bounds;
	bool 			_initial;
	
};

#endif /* LOCATION_H_ */
