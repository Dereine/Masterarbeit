/*
 * LinearPredicateConjunct.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef LINEARPREDICATECONJUNCT_H_
#define LINEARPREDICATECONJUNCT_H_

#include "LinearPredicate.h"

using namespace std;

class LinearPredicateConjunct {
public:
	LinearPredicateConjunct();
	LinearPredicateConjunct(vector<LinearPredicate> linearPredicates);
	virtual ~LinearPredicateConjunct();
	const vector<LinearPredicate>& getLinPreds() const;
	void setLinPreds(const vector<LinearPredicate>& linPreds);
	void addLinPred(LinearPredicate linPred);
	string toStringISat(bool prime);

private:
	vector<LinearPredicate> _linPreds;
};

#endif /* LINEARPREDICATECONJUNCT_H_ */
