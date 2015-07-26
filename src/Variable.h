/*
 * Variable.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <string>
#include <cstdio>

class Variable {

public:
	enum type {DUMMY, BOOL, REAL, INTEGER};
	Variable();
	Variable(Variable::type variableType, std::string name,
			double lowerBound, double upperBound, double initialValue) ;
	virtual ~Variable();

	type getType();
	void setType(type varType);

	std::string getName() const;
	void setName(std::string name);

	double getLowerBound();
	void setLowerBound(double lowerBound);

	double getUpperBound() const;
	void setUpperBound(double upperBound);

	double getInitialValue() const;

	std::string getInitialValueAsString() const;

	void setInitialValue(double initialValue);



private:
	type 		_variableType;
	std::string _name;
	double		_lowerBound;
	double		_upperBound;
	double		_initialValue;
};

#endif /* VARIABLE_H_ */
