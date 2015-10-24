/*
 * LinearPredicate.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef LINEARPREDICATE_H_
#define LINEARPREDICATE_H_

//#include "Inequality.h"
#include "LinearTerm.h"
#include "Constant.h"
#include <vector>

using namespace std;

class LinearPredicate {
public:
	enum Relation {LESS, GREATER, GEQ, LEQ, EQUAL};
	LinearPredicate();
	LinearPredicate(const vector<LinearTerm>& linTerms,
			LinearPredicate::Relation relation, Constant constant);
	virtual ~LinearPredicate();

	/*
	 * Setters and getters
	 */
	const vector<LinearTerm>& getLinTerms() const;
	void setLinTerms(const vector<LinearTerm>& linTerms);

	Relation getRelation() const;
	void setRelation(Relation relation);

	const Constant& getConstant() const;
	void setConstant(const Constant& constant);

	bool isFlow() const;
	void setFlow(bool flow);

	/*
	 * Returns the linear predicate in String form.
	 * One Version for iSat and one for SpaceEx.
	 */
	const string toString(bool prime) const;
	const string toStringSpaceExXML(bool prime, bool assignment) const;
	const string relationToString(Relation relation) const;
	const string relationToStringSpaceExXML() const;

private:
	vector<LinearTerm> 	_linTerms;
	Relation 			_relation;
	Constant 			_constant;
	bool 				_flow;
};

#endif /* LINEARPREDICATE_H_ */
