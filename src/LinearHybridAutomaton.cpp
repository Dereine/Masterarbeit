/*
 * LinearHybridAutomaton.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "LinearHybridAutomaton.h"

using namespace std;

#define DEBUG 1
#define delta "0.01"

LinearHybridAutomaton::LinearHybridAutomaton() {
	// TODO Auto-generated constructor stub

}

LinearHybridAutomaton::~LinearHybridAutomaton() {
	// TODO Auto-generated destructor stub
}

void LinearHybridAutomaton::setLocations(vector<Location> locations) {
	_locations = locations;
}
vector<Location> LinearHybridAutomaton::getLocations() {
	return _locations;
}

void LinearHybridAutomaton::setEdges(vector<Edge> edges) {
	_edges = edges;
}
vector<Edge> LinearHybridAutomaton::getEdges() {
	return _edges;
}

void LinearHybridAutomaton::setVariables(vector<Variable> variables) {
	_variables = variables;
}
vector<Variable> LinearHybridAutomaton::getVariables() {
	return _variables;
}

const vector<Constant>& LinearHybridAutomaton::getConstants() const {
	return _constants;
}

void LinearHybridAutomaton::setConstants(
		const vector<Constant>& constants) {
	_constants = constants;
}

void LinearHybridAutomaton::addLocation(Location location) {
	_locations.push_back(location);
}

void LinearHybridAutomaton::addEdge(Edge edge) {
	_edges.push_back(edge);
}

void LinearHybridAutomaton::addVariable(Variable variable) {
	_variables.push_back(variable);
}

void LinearHybridAutomaton::addConstant(Constant constant) {
	_constants.push_back(constant);
}

void LinearHybridAutomaton::setUpConstants() {
	double value;
	char* name;
	for (int i = 0; i < _constants.size(); i++) {
		value = _constants[i].getValue();
//		name = _constants[i].getName().c_str();
		// Write the node in the list.
		_isatConstants.push_back(isat3_node_create_constant_float(_isatInstance, value));
		// And create the define.
		_isatConstantsDefines.push_back(isat3_node_create_define(
				_isatInstance, _constants[i].getName().c_str(), _isatConstants[i]));
	}

}

void LinearHybridAutomaton::setUpVariables() {
	for (int i = 0; i < _variables.size(); i++)
		// Write the node in the list.
		_isatConstants.push_back(isat3_node_create_variable_float(_isatInstance,
				_variables[i].getName().c_str(),
				_variables[i].getLowerBound(),
				_variables[i].getUpperBound()));
}

void LinearHybridAutomaton::setUpIsat3() {
	isat3_setup();
	_isatInstance = isat3_init(NULL);
	_isatReady = true;
}

/*
 * Create a sigma variable for each location and build up the
 * exactly one state constraint.
 */
struct isat3_node* LinearHybridAutomaton::exactlyOneState() {
	char tmp[10];
	isat3_node* node;
	string atLeastOne;
	string atMostOne;
	string atLeastOneTmp;
	string atMostOneTmp;
	isat3_node_create_variable_integer(_isatInstance,
					_locations[0].getName().c_str(), 0, 1);
	atLeastOne = "   1 * " + _locations[0].getName();
	atMostOne = "   1 * abs(" + _locations[0].getName() + " - 1)";
	cout << "Creating 'Exactly One State Constraint' : " << endl;
	cout << atLeastOne << "   |   "+ atMostOne << endl;
	for (int i = 1; i < _locations.size(); i++) {
		isat3_node_create_variable_integer(_isatInstance,
				_locations[i].getName().c_str(), 0, 1);
		atLeastOneTmp = " + 1 * " + _locations[i].getName();
		atMostOneTmp = " + 1 * abs(" + _locations[i].getName() + " - 1)";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1 and ";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%d", _locations.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ";";
	atMostOne += atMostOneTmp;
	atLeastOne += atMostOne;
	cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	if (DEBUG)
		cout << atLeastOne << endl;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, atLeastOne.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::exactlyOneTransition() {
	char tmp[10];
	isat3_node* node;
	string atLeastOne;
	string atMostOne;
	string atLeastOneTmp;
	string atMostOneTmp;
	string name = _edges[0].getName();
	isat3_node_create_variable_integer(_isatInstance,
					_edges[0].getName().c_str(), 0, 1);
	atLeastOne = "   1 * " + _edges[0].getName();
	atMostOne = "   1 * abs(" + _edges[0].getName() + " - 1)";
	cout << "Creating 'Exactly One Transition Constraint' : " << endl;
	cout << atLeastOne << "   |   "+ atMostOne << endl;
	for (int i = 1; i < _edges.size(); i++) {
		isat3_node_create_variable_integer(_isatInstance,
				_edges[i].getName().c_str(), 0, 1);
		atLeastOneTmp = " + 1 * " + _edges[i].getName();
		atMostOneTmp = " + 1 * abs(" + _edges[i].getName() + " - 1)";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1 and ";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%d", _edges.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ";";
	atMostOne += atMostOneTmp;
	atLeastOne += atMostOne;
	cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	if (DEBUG)
		cout << atLeastOne << endl;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, atLeastOne.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::continuousStateComponents() {
	string constraint = "";
	string variableName;
	string relation;
	Location location;
	string slope;
	vector <Bound> bounds;
	Bound bound;
	int numberOfBounds;
	for (int i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		constraint += "(";
		constraint += location.getName() + " = 1" + " -> (";
		bounds = location.getBounds();
		numberOfBounds = bounds.size();
		for (int i = 0; i < numberOfBounds; i++) {
			bound = bounds[i];
			variableName = bound.getVariable().getName();
			relation = bound.getLowerOrUpper() == Bound::LOW ? ">=" : "<=";
			slope = bound.getConstant().getValueString();
			constraint += variableName + "'"; 						// Variable prime
			constraint += relation;
			constraint += variableName + "+ (";
			constraint += slope + " * " + delta + "))";
			if (i < numberOfBounds - 1)
				constraint += " and (";
			else
				constraint += ");\n";
		}
	}
	cout << constraint << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

void LinearHybridAutomaton::toIsat3BMC() {
	struct isat3_node* bmcFormular = exactlyOneState();
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, exactlyOneTransition());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, continuousStateComponents());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, invariantHoldsEntry());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, invariantHoldsExit());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, transitionStateChange());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, transitionGuard());
	bmcFormular = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, bmcFormular, transitionAssignment());
}

struct isat3_node* LinearHybridAutomaton::invariantHoldsEntry() {
	string constraint = "";
	string variableName;
	string relation;
	string constant = "";
	int numberOfIneqs;
	for (int i = 0; i < _locations.size(); i++) {
		Location location = _locations[i];
		constraint += "(" + location.getName() + " = 1 -> (";
		Invariant invariant = _locations[i].getInvariant();
		constraint += invariant.toStringISat(false) + "));\n";
	}
	cout << "Invariant holds upon entry: " << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::invariantHoldsExit() {
	string constraint = "";
	string variableName;
	string relation;
	string constant = "";
	int numberOfIneqs;
	for (int i = 0; i < _locations.size(); i++) {
		Location location = _locations[i];
		constraint += "(" + location.getName() + " = 1 -> (";
		Invariant invariant = _locations[i].getInvariant();
		constraint += invariant.toStringISat(true) + "));\n";
	}
	cout << "Invariant holds upon exit: " << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::transitionStateChange() {
	isat3_node* node;
	string constraint = "";
	string transitionName;
	string sourceName;
	string destinationName;
	Edge edge;
	edge = _edges[0];
	transitionName = edge.getName();
	sourceName = edge.getSource().getName();
	destinationName = edge.getDestination().getName();
	constraint = "2 * abs(" + transitionName + " - 1) + 1 * "; // 2 * /edge + 1 *
	constraint += sourceName + " + 1 * " + destinationName + "' >= 2;\n"; // state + 1 * state' >= 2
	for (int i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		sourceName = edge.getSource().getName();
		destinationName = edge.getDestination().getName();
		constraint += "2 * abs(" + transitionName + " - 1) + 1 * "; // 2 * /edge + 1 *
		constraint += sourceName + " + 1 * " + destinationName + "' >= 2;\n"; // state + 1 * state' >= 2
	}
	cout << "Transition -> Change state" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::transitionGuard() {
	char tmp[10];
	isat3_node* node;
	string constraint = "";
	string transitionName;
	string guard;
	Edge edge;
	edge = _edges[0];
	transitionName = edge.getName();
	// TODO: Maybe here prime?
	guard = edge.getGuard().toString(true);
	constraint = transitionName + " = 1 -> (" + guard + ");\n";
	for (int i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		guard = edge.getGuard().toString(true);
		constraint += transitionName + "= 1 -> (" + guard + ");\n";
	}
	cout << "Transition -> Guard Holds" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}


struct isat3_node* LinearHybridAutomaton::transitionAssignment() {
	char tmp[10];
	isat3_node* node;
	string constraint = "";
	string transitionName;
	string assignment;
	Edge edge;
	edge = _edges[0];
	transitionName = edge.getName();
	assignment = edge.getAssignment().toString(false);
	constraint = transitionName + " = 1 -> (" + assignment + ");\n";
	for (int i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		assignment = edge.getGuard().toString(false);
		constraint += transitionName + "= 1 -> (" + assignment + ");\n";
	}
	cout << "Transition -> Guard Holds" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

void LinearHybridAutomaton::setUpInitial() {
	string initCondition = "";
	Variable variable;
	for (int i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		initCondition += variable.getName() + " = " + variable.getInitialValueAsString() + ";\n";
	}
	Edge edge;
	for (int i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		initCondition += edge.getName() + " = 0;\n";
	}
	Location location;
	for (int i = 0; i < _edges.size(); i++) {
		location = _locations[i];
		initCondition += location.getName() + " = " +
				(location.isInitial() ? "1" : "0") + ";\n";
	}
	cout << "Creating Initial Condition:" << endl;
	cout << initCondition;
	cout << "---------------------------" << endl;
	_init = isat3_node_create_from_string(_isatInstance, initCondition.c_str());
}

void LinearHybridAutomaton::setTarget(LinearPredicate target) {
	cout << "Creating Target Condition:" << endl;
	cout << target.toString(false);
	cout << "---------------------------" << endl;
	std::string targetString = target.toString(false) + ";\n";
	_target = isat3_node_create_from_string(_isatInstance,
			targetString.c_str());
}
