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


//Bound::Bound(Constant constant, Variable variable, LowOrUp lowerOrUpper) {
//	_constant = constant;
//	_variable = variable;
//	_lowerOrUpper = lowerOrUpper;
//}

Bound::~Bound() {
	// TODO Auto-generated destructor stub
}

const Constant& Bound::getConstantUp() const {
	return _constantUp;
}

const Constant& Bound::getConstantLow() const {
	return _constantLow;
}

void Bound::setConstantUp(const Constant& constant) {
	_constantUp = constant;
}

void Bound::setConstantLow(const Constant& constant) {
	_constantLow = constant;
}

//Bound::LowOrUp Bound::getLowerOrUpper() const {
//	return _lowerOrUpper;
//}
//
//void Bound::setLowerOrUpper(LowOrUp lowerOrUpper) {
//	_lowerOrUpper = lowerOrUpper;
//}

const Variable Bound::getVariable() const {
//Variable Bound::getVariable() const {
	return _variable;
}

Bound::Bound(Variable variable, Constant constantUp, Constant constantLow) {
	_variable = variable;
	_constantLow = constantLow;
	_constantUp = constantUp;
}

void Bound::setVariable(const Variable& variable) {
	_variable = variable;
}
