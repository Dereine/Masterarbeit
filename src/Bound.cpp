/*
 * Bound.cpp
 *
 *  Created on: 23.07.2015
 *      Author: johannes
 */

#include "Bound.h"

Bound::Bound() {
	// TODO Auto-generated constructor stub

}


Bound::Bound(Constant constant, Variable variable, LowOrUp lowerOrUpper) {
	_constant = constant;
	_variable = variable;
	_lowerOrUpper = lowerOrUpper;
}

Bound::~Bound() {
	// TODO Auto-generated destructor stub
}

const Constant& Bound::getConstant() const {
	return _constant;
}

void Bound::setConstant(const Constant& constant) {
	_constant = constant;
}

Bound::LowOrUp Bound::getLowerOrUpper() const {
	return _lowerOrUpper;
}

void Bound::setLowerOrUpper(LowOrUp lowerOrUpper) {
	_lowerOrUpper = lowerOrUpper;
}

const Variable Bound::getVariable() const {
//Variable Bound::getVariable() const {
	return _variable;
}

void Bound::setVariable(const Variable& variable) {
	_variable = variable;
}
