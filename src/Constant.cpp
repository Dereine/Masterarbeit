/*
 * Constant.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "Constant.h"

Constant::Constant() {
	// TODO Auto-generated constructor stub

}

Constant::Constant(std::string name, double value) {
	_name = name;
	_value = value;
}

Constant::~Constant() {
	// TODO Auto-generated destructor stub
}

std::string Constant::getName() {
	return _name;
}
void Constant::setName(std::string name) {
	_name = name;
}

double Constant::getValue() {
	return _value;
}

const std::string Constant::getValueString() const {
	char tmp[10];
	std::string tmpString;
	sprintf(tmp, "%1.5f", _value);
	tmpString.assign(tmp);
	return tmpString;
}

void Constant::setValue(double value) {
	_value = value;
}

