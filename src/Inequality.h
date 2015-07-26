/*
 * Inequality.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef INEQUALITY_H_
#define INEQUALITY_H_

#include "Variable.h"
#include "Constant.h"


class Inequality {
public:
	enum Relation {GEQ, LEQ};
	Inequality();
	Inequality(Constant constant, Inequality::Relation relation,
			Variable variable);
	virtual ~Inequality();
	Relation getRelation() const;
	void setRelation(Relation relation);

	Variable getVariable() const;
	void setVariable(const Variable& variable);

	//const Constant& getConstant() const;
	Constant getConstant() const;
	void setConstant(const Constant& constant);

private:
	Variable _variable;
	Constant _constant;
	Relation _relation;

};

#endif /* INEQUALITY_H_ */
