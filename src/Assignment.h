/*
 * Assignment.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "LinearPredicateConjunct.h"
#include <map>

using namespace std;

class Assignment : public LinearPredicateConjunct{
public:
	Assignment();
	Assignment(vector<LinearPredicate> linPreds);
	virtual ~Assignment();
	const map<string, Variable>& getAssignedVariables() const;
	void setAssignedVariables(const vector<Variable>& assignedVariables);
	void addAssignedVariable(Variable assignedVariable);
	bool isAssignedVariable(const string& variableName) const;
	void toStringSpaceExXML(string& assignmentString,
			const vector<Variable>& variables) const;

private:
	map<string, Variable> _assignedVariables;
};

#endif /* ASSIGNMENT_H_ */
