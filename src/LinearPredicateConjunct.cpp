/*
 * LinearPredicateConjunct.cpp
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#include "LinearPredicateConjunct.h"

LinearPredicateConjunct::LinearPredicateConjunct() {
	// TODO Auto-generated constructor stub

}

LinearPredicateConjunct::LinearPredicateConjunct(
		vector<LinearPredicate> linearPredicates) {
	_linPreds = linearPredicates;
}

LinearPredicateConjunct::~LinearPredicateConjunct() {
	// TODO Auto-generated destructor stub
}

const vector<LinearPredicate>& LinearPredicateConjunct::getLinPreds() const {
	return _linPreds;
}

void LinearPredicateConjunct::setLinPreds(
		const vector<LinearPredicate>& linPreds) {
	_linPreds = linPreds;
}

void LinearPredicateConjunct::addLinPred(LinearPredicate linPred) {
	_linPreds.push_back(linPred);
}

std::string LinearPredicateConjunct::toStringISat(bool prime) {
	std::string returnString = "";
	LinearPredicate linPred;
	if (_linPreds.size() == 0)
		return "true";
	linPred = _linPreds[0];
	returnString += linPred.toString(prime);
	for (int i = 1; i < _linPreds.size(); i++) {
		linPred = _linPreds[i];
		returnString += " and " + linPred.toString(prime);
	}
	return returnString;
}
