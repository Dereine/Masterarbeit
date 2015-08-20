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

class Location {

public:
	Location();
	Location(int id, std::string name, Invariant invariant,
			std::vector<Bound> bounds, bool initial);
	virtual ~Location();

	unsigned int getId();
	void setId(int id);

	Constant getMinSlope();
	void setMinSlope(Constant minSlope);

	Constant getMaxSlope();
	void setMaxSlope(Constant maxSlope);

	const std::string& getName() const;
	void setName(const std::string& name);

	const Invariant& getInvariant() const;
	void setInvariant(const Invariant& invariant);

	const std::vector<Bound>& getBounds() const;
	void setBounds(const std::vector<Bound>& bounds);

	bool isInitial() const;
	void setInitial(bool initial);

	void flowToSpaceExXML(string& flowString);

private:
	unsigned int		_id;
	std::string 		_name;
	Invariant 			_invariant;
	std::vector<Bound> 	_bounds;
	bool 				_initial;
	
};

#endif /* LOCATION_H_ */
