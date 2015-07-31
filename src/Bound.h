/*
 * Bound.h
 *
 *  Created on: 23.07.2015
 *      Author: johannes
 */

#ifndef BOUND_H_
#define BOUND_H_

#include "Variable.h"
#include "Constant.h"


class Bound {
public:
	enum LowOrUp {LOW, UP};

	Bound();
	//Bound(Constant constant, Variable variable, LowOrUp lowerOrUpper);
	Bound(Variable variable, Constant constantUp, Constant constantLow);
	virtual ~Bound();

	//const Constant& getConstant() const;
	const Constant& getConstantUp() const;
	const Constant& getConstantLow() const;

	void setConstant(const Constant& constant);

//	LowOrUp getLowerOrUpper() const;
//	void setLowerOrUpper(LowOrUp lowerOrUpper);

	void setConstantLow(const Constant& constant);
	void setConstantUp(const Constant& constant);

	//const Variable getVariable() const;
	const Variable getVariable() const;
	void setVariable(const Variable& variable);

private:
	Constant _constantUp;
	Constant _constantLow;
	Variable _variable;
	//LowOrUp _lowerOrUpper;

};

#endif /* BOUND_H_ */