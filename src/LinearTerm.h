/*
 * LinearTerm.h
 *
 *  Created on: 26.07.2015
 *      Author: johannes
 */

#ifndef LINEARTERM_H_
#define LINEARTERM_H_

#include "Variable.h"
#include "Constant.h"

class LinearTerm {
public:
	LinearTerm();
	LinearTerm(Constant constant, Variable variable);
	virtual ~LinearTerm();

	Variable getVariable() const;
	void setVariable(const Variable& variable);

	Constant getConstant() const;
	void setConstant(const Constant& constant);

private:
	Variable _variable;
	Constant _constant;

};
#endif /* LINEARTERM_H_ */
