/*
 * Variable.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Variable.h"

Variable::Variable() {
}

Variable::Variable(Variable::type variableType, std::string name,
		double lowerBound, double upperBound, double initialValue) {
	_initialValue = initialValue;
	_variableType = variableType;
	_name = name;
	_lowerBound = lowerBound;
	_upperBound = upperBound;
	_initialized = true;
}

Variable::Variable(Variable::type variableType, std::string name,
		double lowerBound, double upperBound) {
	_initialValue = -1;
	_variableType = variableType;
	_name = name;
	_lowerBound = lowerBound;
	_upperBound = upperBound;
	_initialized = false;
}

Variable::~Variable() {
	// TODO Auto-generated destructor stub
}

void Variable::setType(type varType) {
	_variableType = varType;
}

Variable::type Variable::getType() {
	return _variableType;
}

std::string Variable::getName() const {
	return _name;
}
void Variable::setName(std::string name) {
	_name = name;
}

double Variable::getLowerBound() {
	return _lowerBound;
}


void Variable::setLowerBound(double lowerBound) {
	_lowerBound = lowerBound;
}

double Variable::getUpperBound() const {
	return _upperBound;
}

void Variable::setUpperBound(double upperBound) {
	_upperBound = upperBound;
}

double Variable::getInitialValue() const {
	return _initialValue;
}

std::string Variable::getInitialValueAsString() const {
	char tmp[10];
	std::string tmpString;
	sprintf(tmp, "%1.5f", _initialValue);
	tmpString.assign(tmp);
	return tmpString;
}

void Variable::setInitialValue(double initialValue) {
	_initialValue = initialValue;
}

std::string Variable::stayConstantAsStringIsat() {
	std::string returnString = "";
	returnString = _name + "' = " + _name;
	return returnString;
}

bool Variable::isInitialized() const {
	return _initialized;
}

void Variable::setInitialized(bool initialized) {
	_initialized = initialized;
}

//type Variable::getVariableType() const {
//	return _variableType;
//}

void Variable::setVariableType(type variableType) {
	_variableType = variableType;
}
