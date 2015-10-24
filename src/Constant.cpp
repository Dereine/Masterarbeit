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

const std::string Constant::getValueString(const unsigned int digits) const {
	char tmp2[7];
	sprintf(tmp2, "%u", digits);
	string string;
	string.assign(tmp2);
	char tmp[150];
	std::string tmpString = "%1." + string + "f";
	sprintf(tmp, tmpString.c_str(), _value);
	tmpString = "";
	tmpString.assign(tmp);
	return tmpString;
}

void Constant::setValue(double value) {
	_value = value;
}

