/*
 * Inequality.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Inequality.h"

Inequality::Inequality() {
	// TODO Auto-generated constructor stub

}

Inequality::Inequality(Constant constant, Inequality::Relation relation,
		Variable variable) {
	_relation = relation;
	_constant = constant;
	_variable = variable;
}

Inequality::~Inequality() {
	// TODO Auto-generated destructor stub
}

Inequality::Relation Inequality::getRelation() const {
	return _relation;
}

void Inequality::setRelation(Relation relation) {
	_relation = relation;
}

Variable Inequality::getVariable() const {
	return _variable;
}

void Inequality::setVariable(const Variable& variable) {
	_variable = variable;
}

//const Constant& Inequality::getConstant() const {
Constant Inequality::getConstant() const {
	return _constant;
}

void Inequality::setConstant(const Constant& constant) {
	_constant = constant;
}
