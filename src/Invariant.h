/*
 * Invariant.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef INVARIANT_H_
#define INVARIANT_H_

#include "Variable.h"
#include "LinearPredicate.h"

class Invariant {
public:
	Invariant();
	Invariant(std::vector <LinearPredicate> linearPredicate);
	virtual ~Invariant();

	std::vector<LinearPredicate> getLinPreds();
	void setLinPreds(std::vector<LinearPredicate> linearPredicate);

	std::string toStringISat(bool prime);

private:
	std::vector <LinearPredicate> _linPreds;
};

#endif /* INVARIANT_H_ */
