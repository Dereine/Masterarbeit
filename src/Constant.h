/*
 * Constant.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <string>
#include <cstdio>

#define CONSTANT_DIGITS 100

using namespace std;

class Constant {
public:
	Constant();
	Constant(string name, double value);
	virtual ~Constant();

	string getName();
	void setName(string name);

	double getValue();
	const string getValueString(const unsigned int digits) const;
	void setValue(double value);

private:
	string _name;
	double _value;
};

#endif /* CONSTANT_H_ */
