/*
 * Invariant.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef INVARIANT_H_
#define INVARIANT_H_

#include "Variable.h"
//#include "LinearPredicate.h"
#include "LinearPredicateConjunct.h"

class Invariant : public LinearPredicateConjunct{
public:
	Invariant();
	Invariant(std::vector <LinearPredicate> linPreds);
	virtual ~Invariant();

//	std::vector<LinearPredicate> getLinPreds();
//	void setLinPreds(std::vector<LinearPredicate> linearPredicate);
//	void addLinPred(LinearPredicate linPred);
//
//	std::string toStringISat(bool prime);
//
//private:
//	std::vector <LinearPredicate> _linPreds;
};

#endif /* INVARIANT_H_ */
