/*
 * LinearPredicate.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "LinearPredicate.h"

LinearPredicate::LinearPredicate() {
	// TODO Auto-generated constructor stub
}

//LinearPredicate::LinearPredicate(const std::vector<Inequality>& ineqs) {
//	_ineqs = ineqs;
//}

LinearPredicate::LinearPredicate(const std::vector<LinearTerm>& linTerms,
		LinearPredicate::Relation relation, Constant constant) {
	_linTerms = linTerms;
	_relation = relation;
	_constant = constant;
}

LinearPredicate::~LinearPredicate() {
	// TODO Auto-generated destructor stub
}

//const std::vector<Inequality>& LinearPredicate::getIneqs() const {
//	return _ineqs;
//}
//
//
//void LinearPredicate::setIneqs(const std::vector<Inequality>& ineqs) {
//	_ineqs = ineqs;
//}

const std::vector<LinearTerm>& LinearPredicate::getLinTerms() const {
	return _linTerms;
}

void LinearPredicate::setLinTerms(const std::vector<LinearTerm>& linTerms) {
	_linTerms = linTerms;
}

const std::string LinearPredicate::toString(bool prime) const {
	std::string returnString = "(";
	LinearTerm term;
	term = _linTerms[0];
	if (term.getConstant().getValue() != 1.0f) {
		returnString += term.getConstant().getValueString() + " * " +
				term.getVariable().getName();
	} else {
		returnString += term.getVariable().getName();
	}
	if (prime)
		returnString += "'";
	for (size_t i = 1; i < _linTerms.size(); i++) {
		term = _linTerms[i];
		if (term.getConstant().getValue() != 1.0f) {
			returnString += " + " + term.getConstant().getValueString() + " * " +
					term.getVariable().getName();
		} else {
			returnString += " + " + term.getVariable().getName();
		}

		if (prime)
			returnString += "'";
	}
	returnString += " " + relationToString(_relation) + " " + _constant.getValueString() + ")";
	return returnString;
}

const std::string LinearPredicate::toStringSpaceExXML(bool prime) const {
	std::string returnString = "(";
	LinearTerm term;
	term = _linTerms[0];
	if (term.getConstant().getValue() != 1.0f) {
		returnString += term.getConstant().getValueString() + " * " +
				term.getVariable().getName();
	} else {
		returnString += term.getVariable().getName();
	}
	for (int i = 1; i < _linTerms.size(); i++) {
		term = _linTerms[i];
		if (term.getConstant().getValue() != 1.0f) {
			returnString += " + " + term.getConstant().getValueString() + " * " +
					term.getVariable().getName();
		} else {
			returnString += " + " + term.getVariable().getName();
		}
	}

	returnString += " " + this->relationToStringSpaceExXML() + " " + _constant.getValueString() + ")";
	return returnString;
}

LinearPredicate::Relation LinearPredicate::getRelation() const {
	return _relation;
}

void LinearPredicate::setRelation(Relation relation) {
	_relation = relation;
}

const std::string LinearPredicate::relationToString(Relation relation) const {
	std::string returnString;
	switch(relation) {
	case (LESS):
			returnString = "<";
			break;
	case (GREATER):
			returnString = ">";
			break;
	case (LEQ):
			returnString = "<=";
			break;
	case (GEQ):
			returnString = ">=";
			break;
	case (EQUAL):
			returnString = "=";
			break;
	}
	return returnString;
}

const std::string LinearPredicate::relationToStringSpaceExXML() const {
	std::string returnString;
	switch(_relation) {
	case (LESS):
			returnString = " &l;= ";
			break;
	case (GREATER):
			returnString = " &g;= ";
			break;
	case (LEQ):
			returnString = " &lt;= ";
			break;
	case (GEQ):
			returnString = " &gt;= ";
			break;
	case (EQUAL):
			returnString = "==";
			break;
	}
	return returnString;
}

const Constant& LinearPredicate::getConstant() const {
return _constant;
}

void LinearPredicate::setConstant(const Constant& constant) {
_constant = constant;
}

bool LinearPredicate::isFlow() const {
	return _flow;
}

void LinearPredicate::setFlow(bool flow) {
	_flow = flow;
}
