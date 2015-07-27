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

class LinearPredicate {
public:
	enum Relation {LESS, GREATER, GEQ, LEQ, EQUAL};
	LinearPredicate();
	LinearPredicate(const std::vector<LinearTerm>& linTerms,
			LinearPredicate::Relation relation, Constant constant);
	virtual ~LinearPredicate();

	const std::vector<LinearTerm>& getLinTerms() const;
	void setLinTerms(const std::vector<LinearTerm>& linTerms);

	Relation getRelation() const;
	void setRelation(Relation relation);

	const Constant& getConstant() const;
	void setConstant(const Constant& constant);

	// Returns the linear predicate in human readable form.
	const std::string toString(bool prime) const;
	const std::string relationToString(Relation relation) const;


private:
	std::vector<LinearTerm> _linTerms;
	Relation 				_relation;
	Constant 				_constant;
};

#endif /* LINEARPREDICATE_H_ */
