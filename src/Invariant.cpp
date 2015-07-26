/*
 * Invariant.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Invariant.h"

Invariant::Invariant() {
	// TODO Auto-generated constructor stub

}

Invariant::Invariant(std::vector<LinearPredicate> linearPredicate) {
	_linPreds = linearPredicate;
}

Invariant::~Invariant() {
	// TODO Auto-generated destructor stub
}

std::vector<LinearPredicate> Invariant::getLinPreds() {
	return _linPreds;
}

void Invariant::setLinPreds(std::vector<LinearPredicate> linearPredicate) {
	_linPreds = linearPredicate;
}

std::string Invariant::toStringISat(bool prime) {
	std::string returnString = "";
	LinearPredicate linPred;
	linPred = _linPreds[0];
	returnString += linPred.toString(prime);
	for (int i = 0; i < _linPreds.size(); i++) {
		linPred = _linPreds[i];
		returnString += " and " + linPred.toString(prime);
	}
	return returnString;
}
