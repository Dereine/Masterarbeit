/*
 * Guard.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef GUARD_H_
#define GUARD_H_

#include "LinearPredicateConjunct.h"

using namespace std;

class Guard : public LinearPredicateConjunct {
public:
	Guard();
	Guard(vector<LinearPredicate> linPreds);
	virtual ~Guard();
	void toStringSpaceExXML(string& guardString);
};

#endif /* GUARD_H_ */
