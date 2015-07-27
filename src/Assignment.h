/*
 * Assignment.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "LinearPredicateConjunct.h"

using namespace std;

class Assignment : public LinearPredicateConjunct{
public:
	Assignment();
	Assignment(vector<LinearPredicate> linPreds);
	virtual ~Assignment();
};

#endif /* ASSIGNMENT_H_ */
