/*
 * Assignment.cpp
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#include "Assignment.h"

Assignment::Assignment() {
	// TODO Auto-generated constructor stub

}

Assignment::Assignment(vector<LinearPredicate> linPreds) :
	LinearPredicateConjunct(linPreds){}

Assignment::~Assignment() {
	// TODO Auto-generated destructor stub
}

const map<string, Variable>& Assignment::getAssignedVariables() const {
	return _assignedVariables;
}

void Assignment::setAssignedVariables(
		const vector<Variable>& assignedVariables) {
	Variable variable;
	for (size_t i = 0; i < assignedVariables.size(); i++) {
		variable = assignedVariables[i];
		_assignedVariables[variable.getName()] = variable;
	}
}

void Assignment::addAssignedVariable(Variable assignedVariable) {
	_assignedVariables[assignedVariable.getName()] = (assignedVariable);
}

bool Assignment::isAssignedVariable(const string& variableName) const {
	map<string, Variable>::const_iterator it = _assignedVariables.find(variableName);
	return (it != _assignedVariables.end());
}

void Assignment::toStringSpaceExXML(string& assignmentString,
		const std::vector<Variable>& variables) const {
	assignmentString += "      <assignment> ";
	bool firstElement = true;
	LinearPredicate linPred;
	Variable variable;
	for (size_t k = 0; k < variables.size(); k++) {
		variable = variables[k];
		if (!isAssignedVariable(variable.getName())) {
			if (firstElement) {
				assignmentString += variable.getName() + ":=" +
				variable.getName() + "\n";
				firstElement = false;
			}
			else
				assignmentString += "&amp;" + variable.getName() + ":=" +
				variable.getName() + "\n";
		}
	}
	firstElement = true;
	for (size_t i = 0; i < _linPreds.size(); i++) {
		linPred = _linPreds[i];
		assignmentString +=  "&amp;" +
				linPred.toStringSpaceExXML(false, true) + "\n";
	}
	assignmentString += "      </assignment>\n";
}
