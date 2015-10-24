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

LinearPredicate::LinearPredicate(const vector<LinearTerm>& linTerms,
		LinearPredicate::Relation relation, Constant constant) {
	_linTerms = linTerms;
	_relation = relation;
	_constant = constant;
	_flow = false;
}

LinearPredicate::~LinearPredicate() {
	// TODO Auto-generated destructor stub
}

const vector<LinearTerm>& LinearPredicate::getLinTerms() const {
	return _linTerms;
}

void LinearPredicate::setLinTerms(const vector<LinearTerm>& linTerms) {
	_linTerms = linTerms;
}

const string LinearPredicate::toString(bool prime) const {
	string returnString = "(";
	LinearTerm term;
	term = _linTerms[0];
	if (term.getConstant().getValue() != 1.0f) {
		returnString += term.getConstant().getValueString(CONSTANT_DIGITS) + " * " +
				term.getVariable().getName();
	} else {
		returnString += term.getVariable().getName();
	}
	if (prime)
		returnString += "'";
	for (size_t i = 1; i < _linTerms.size(); i++) {
		term = _linTerms[i];
		if (term.getConstant().getValue() != 1.0f) {
			returnString += " + " + term.getConstant().getValueString(CONSTANT_DIGITS) + " * " +
					term.getVariable().getName();
		} else {
			returnString += " + " + term.getVariable().getName();
		}

		if (prime)
			returnString += "'";
	}
	returnString += " " + relationToString(_relation) + " " + _constant.getValueString(CONSTANT_DIGITS) + ")";
	return returnString;
}

const string LinearPredicate::toStringSpaceExXML(bool prime, bool assignment) const {
	string returnString = "(";
	LinearTerm term;
	term = _linTerms[0];
	if (term.getConstant().getValue() != 1.0f) {
		returnString += term.getConstant().getValueString(CONSTANT_DIGITS) + " * " +
				term.getVariable().getName();
	} else {
		returnString += term.getVariable().getName();
	}
	if (!assignment) {
		if (prime)
			returnString += "'";
		for (int i = 1; i < _linTerms.size(); i++) {
			term = _linTerms[i];
			if (term.getConstant().getValue() != 1.0f) {
				returnString += " + " + term.getConstant().getValueString(CONSTANT_DIGITS) + " * " +
						term.getVariable().getName();
			} else {
				returnString += " + " + term.getVariable().getName();
			}
			if (prime)
				returnString += "'";
		}
		returnString += " " + this->relationToStringSpaceExXML() + " " + _constant.getValueString(CONSTANT_DIGITS) + ")";
	} else {
		returnString += " := ";
		for (int i = 1; i < _linTerms.size(); i++) {
			term = _linTerms[i];
			double constantValue = term.getConstant().getValue();
			if (constantValue != 1.0f) {
				returnString += "-1 * " +
						term.getConstant().getValueString(CONSTANT_DIGITS) + " * " +
						term.getVariable().getName();
			} else {
				returnString += " - " + term.getVariable().getName();
			}
			if (prime)
				returnString += "'";
		}
		returnString += (_linTerms.size() > 1 ? " + " : "") + _constant.getValueString(CONSTANT_DIGITS) + ")";
	}
	return returnString;
}

LinearPredicate::Relation LinearPredicate::getRelation() const {
	return _relation;
}

void LinearPredicate::setRelation(Relation relation) {
	_relation = relation;
}

const string LinearPredicate::relationToString(Relation relation) const {
	string returnString;
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

const string LinearPredicate::relationToStringSpaceExXML() const {
	string returnString;
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
