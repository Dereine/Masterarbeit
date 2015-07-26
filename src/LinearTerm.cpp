/*
 * LinearTerm.cpp
 *
 *  Created on: 26.07.2015
 *      Author: johannes
 */

#include "LinearTerm.h"

LinearTerm::LinearTerm() {
	// TODO Auto-generated constructor stub

}

LinearTerm::LinearTerm(Constant constant, Variable variable) {
	_constant = constant;
	_variable = variable;
}

LinearTerm::~LinearTerm() {
	// TODO Auto-generated destructor stub
}


Variable LinearTerm::getVariable() const {
	return _variable;
}

void LinearTerm::setVariable(const Variable& variable) {
	_variable = variable;
}

//const Constant& LinearTerm::getConstant() const {
Constant LinearTerm::getConstant() const {
	return _constant;
}

void LinearTerm::setConstant(const Constant& constant) {
	_constant = constant;
}
