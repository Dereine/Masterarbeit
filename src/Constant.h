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

class Constant {
public:
	Constant();
	Constant(std::string name, double value);
	virtual ~Constant();

	std::string getName();
	void setName(std::string name);

	double getValue();
	const std::string getValueString() const;
	void setValue(double value);

private:
	std::string _name;
	double _value;
};

#endif /* CONSTANT_H_ */
