/*
 * LinearHybridAutomaton.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "LinearHybridAutomaton.h"

using namespace std;

#define DEBUG 1
#define delta "1"

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
	for (size_t i = 0; i < _constants.size(); i++) {
		value = _constants[i].getValue();
		isat3_node_create_define(_isatInstance,
				_constants[i].getName().c_str(),
				isat3_node_create_constant_float(_isatInstance, value));
		//		_isatConstantsDefines.push_back(isat3_node_create_define(
		//				_isatInstance, _constants[i].getName().c_str(), _isatConstants[i]));
	}

}

void LinearHybridAutomaton::setUpVariables() {
	Variable variable;
	for (size_t i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		struct isat3_node* node = isat3_node_create_variable_float(_isatInstance,
				variable.getName().c_str(),
				variable.getLowerBound(),
				variable.getUpperBound());
		// Write the node in the list.
		_isatVariables.push_back(node);
		_variableNodeMap[variable.getName()] = node;
	}
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
	struct isat3_node* node;
	string atLeastOne;
	string atMostOne;
	string atLeastOneTmp;
	string atMostOneTmp;
	struct isat3_node* tmp1 = isat3_node_create_variable_integer(_isatInstance,
			_locations[0].getName().c_str(), 0, 1);
	_variableNodeMap[_locations[0].getName()] = tmp1;
	atLeastOne = "(   1 * " + _locations[0].getName() + "'";
	atMostOne = "   1 * abs(" + _locations[0].getName() + "' - 1)";
	cout << "Creating 'Exactly One Location Constraint' : " << endl;
	cout << atLeastOne << "   |   "+ atMostOne << endl;
	for (size_t i = 1; i < _locations.size(); i++) {
		struct isat3_node* tmp2 = isat3_node_create_variable_integer(_isatInstance,
				_locations[i].getName().c_str(), 0, 1);
		_variableNodeMap[_locations[i].getName()] = tmp2;
		atLeastOneTmp = " + 1 * " + _locations[i].getName() + "'";
		atMostOneTmp = " + 1 * abs(" + _locations[i].getName() + "' - 1)";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1) and (";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%u", _locations.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ");";
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
	struct isat3_node* node;
	string atLeastOne;
	string atMostOne;
	string atLeastOneTmp;
	string atMostOneTmp;
	string name = _edges[0].getName();
	struct isat3_node* tmp1 = isat3_node_create_variable_integer(_isatInstance,
			_edges[0].getName().c_str(), 0, 1);
	_variableNodeMap[_edges[0].getName()] = tmp1;
	atLeastOne = "(  1 * " + _edges[0].getName() + "'";
	atMostOne = "   1 * abs(" + _edges[0].getName() + "' - 1)";
	cout << "Exactly One Transition Constraint: " << endl;
	cout << atLeastOne << "   |   "+ atMostOne << endl;
	for (size_t i = 1; i < _edges.size(); i++) {
		struct isat3_node* tmp2 = isat3_node_create_variable_integer(_isatInstance,
				_edges[i].getName().c_str(), 0, 1);
		_variableNodeMap[_edges[i].getName()] = tmp2;
		atLeastOneTmp = " + 1 * " + _edges[i].getName()+ "'";
		atMostOneTmp = " + 1 * abs(" + _edges[i].getName() + "' - 1)";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1) and (";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%u", _edges.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ");";
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

struct isat3_node* LinearHybridAutomaton::asMostOneTransition() {
	char tmp[10];
	struct isat3_node* node;
	string atMostOne = "";
	string atMostOneTmp;
	string name = _edges[0].getName();
	struct isat3_node* tmp1 = isat3_node_create_variable_integer(_isatInstance,
			_edges[0].getName().c_str(), 0, 1);
	_variableNodeMap[_edges[0].getName()] = tmp1;
	atMostOne = "(1 * abs(" + _edges[0].getName() + "' - 1)";
	cout << "At most one transition Constraint: " << endl;
	for (size_t i = 1; i < _edges.size(); i++) {
		struct isat3_node* tmp2 = isat3_node_create_variable_integer(_isatInstance,
				_edges[i].getName().c_str(), 0, 1);
		_variableNodeMap[_edges[i].getName()] = tmp2;
		atMostOneTmp = " + 1 * abs(" + _edges[i].getName() + "' - 1)";
		atMostOne += atMostOneTmp;
		cout << atMostOneTmp << endl;
	}
	sprintf(tmp, "%u", _edges.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ");";
	atMostOne += atMostOneTmp;
	cout << atMostOne << endl;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, atMostOne.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::stayInLocation() {
	string constraint = "";
	Edge edge;
	Location source, destination;
	struct isat3_node* node;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		source = edge.getSource();
		destination = edge.getDestination();
		constraint += "1 * " + edge.getName() + " + 1 * abs(" +
				source.getName() + " - 1) + 1 * " + source.getName() +
				"' >= 1;\n";
	}
	cout << "Stay in location:" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
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
	size_t numberOfBounds;
	Edge edge;
	Constant lower, upper;
	Variable variable;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		constraint += "(";
		constraint += location.getName() + " = 1 -> (";
		bounds = location.getBounds();
		numberOfBounds = bounds.size();
		for (size_t i = 0; i < numberOfBounds; i++) {
			bound = location.getBounds()[i];
			variable = bound.getVariable();
			variableName = variable.getName();
			upper = bound.getConstantUp();
			lower = bound.getConstantLow();
			constraint += variableName + "' >= " + variableName + " + " +
					lower.getValueString() + " * " + delta + " and " +
					variableName + "' <= " + variableName + " + " +
					upper.getValueString() + " * " + delta +
					(i < numberOfBounds - 1 ? " and " : "));\n");
		}
	}
	cout << "Continuous State Components:" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

void LinearHybridAutomaton::modifiedFraenzle() {
	_bmcFormula = exactlyOneState();
//		_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//				ISAT3_NODE_BOP_AND, _bmcFormula, exactlyOneTransition());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, asMostOneTransition());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, continuousStateComponents());
		_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
				ISAT3_NODE_BOP_AND, _bmcFormula, invariantHoldsEntry());
		_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
				ISAT3_NODE_BOP_AND, _bmcFormula, invariantHoldsExit());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, transitionStateChange());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, stayInLocation());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, transitionGuard());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, transitionAssignment());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//	ISAT3_NODE_BOP_AND, _bmcFormula, notFlowVariablesStayConstant());
}

void LinearHybridAutomaton::toIsat3BMC() {
	modifiedFraenzle();
//	setUpLocationVariables();
//	_bmcFormula = jumps();
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//			ISAT3_NODE_BOP_AND, _bmcFormula, flows());
}

void LinearHybridAutomaton::setUpLocationVariables() {
	Location location;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		struct isat3_node* node = isat3_node_create_variable_integer(_isatInstance,
				location.getName().c_str(), 0, 1);
		// Write the node in the list.
//		_isatVariables.push_back(node);
//		_variableNodeMap[variable.getName()] = node;
	}
}
struct isat3_node* LinearHybridAutomaton::jumps() {
	string constraint = "";
	Edge edge;
	Location source;
	Location destination;
	Guard guard;
	Assignment assignment;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		source = edge.getSource();
		destination = edge.getDestination();
		guard = edge.getGuard();
		assignment = edge.getAssignment();
		constraint += "(" + source.getName() + " = 1 and " + destination.getName() +
				"' = 1 -> " + guard.toStringISat(false) + " and " +
				assignment.toStringISat(true) + ");\n";
	}
	cout << "Jumps : " << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}
struct isat3_node* LinearHybridAutomaton::flows() {
	string constraint = "";
	Location location;
	Invariant invariant;
	Bound bound;
	Variable variable;
	Constant upper;
	Constant lower;
	LinearPredicate linPred;
	LinearPredicateConjunct linPreds;
	string locationName;
	string variableName;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		locationName = location.getName();
		constraint += "( "+ locationName + " = 1 and " + locationName + "' = 1"
				+ " -> ";
		// Upper and lower bounds.
		for (size_t k = 0; k < location.getBounds().size(); k++) {
			bound = location.getBounds()[k];
			variable = bound.getVariable();
			variableName = variable.getName();
			upper = bound.getConstantUp();
			lower = bound.getConstantLow();
			constraint += variableName + "' >= " + variableName + " + " +
					lower.getValueString() + " * " + delta + " and " +
					variableName + "' <= " + variableName + " + " +
					upper.getValueString() + " * " + delta + " and ";
		}
		invariant = location.getInvariant();
		linPreds = invariant.getLinPreds();
		// Invariants
		for (size_t k = 0; k < linPreds.getLinPreds().size(); k++) {
			linPred = linPreds.getLinPreds()[k];
			constraint += linPred.toString(true) +
					(k < linPreds.getLinPreds().size() - 1 ? " and " : ");\n");
		}
	}
	cout << "Flows : " << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::invariantHoldsEntry() {
	string constraint = "";
	string variableName;
	string relation;
	string constant = "";
	Location location;
	Invariant invariant;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		constraint += "(" + location.getName() + " = 1 -> (";
		invariant = _locations[i].getInvariant();
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
	Location location;
	Invariant invariant;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		constraint += "(" + location.getName() + " = 1 -> (";
		invariant = _locations[i].getInvariant();
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
	for (size_t i = 1; i < _edges.size(); i++) {
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
	isat3_node* node;
	string constraint = "";
	string transitionName;
	string sourceName;
	string guard;
	Edge edge;
	edge = _edges[0];
	transitionName = edge.getName();
	sourceName = edge.getSource().getName();
	guard = edge.getGuard().toStringISat(true);
	constraint += transitionName + "' = 1 <-> (" + guard + " and " +
			sourceName + "' = 1);\n";
	for (size_t i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		sourceName = edge.getSource().getName();
		guard = edge.getGuard().toStringISat(true);
		constraint += transitionName + "' = 1 <-> (" + guard + " and " +
				sourceName + "' = 1);\n";
	}
	cout << "Transition Enabled <-> Guard Holds" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::transitionAssignment() {
	isat3_node* node;
	string constraint = "";
	string transitionName;
	string assignment;
	Edge edge;
	edge = _edges[0];
	transitionName = edge.getName();
	assignment = edge.getAssignment().toStringISat(true);
	constraint = transitionName + " = 1 -> (" + assignment + ");\n";
	for (size_t i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		assignment = edge.getGuard().toStringISat(true);
		constraint += transitionName + "= 1 -> (" + assignment + ");\n";
	}
	cout << "Transition -> Assignment" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::notFlowVariablesStayConstant() {
	struct isat3_node* node;
	string constraint = "";
	string variableName;
	Variable variable;
	Edge edge;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		variable = edge.getAssignment().getLinPreds()[i].getLinTerms()[0].
				getVariable();
		if (!variable.isFlowVariable()) {
			variableName = variable.getName();
			constraint += "(" + edge.getName() + " = 0 -> (" + variableName +
					"' = " + variableName + "));";
		}
	}
	cout << "Not - flow - variables stay constant in locations :" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

void LinearHybridAutomaton::setUpInitial() {
	string initCondition = "";
	Variable variable;
	for (size_t i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		initCondition += variable.getName() + " = " + variable.getInitialValueAsString() + ";\n";
	}
	Edge edge;
//	for (size_t i = 0; i < _edges.size(); i++) {
//		edge = _edges[i];
//		initCondition += edge.getName() + " = 0;\n";
//	}
	Location location;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		initCondition += location.getName() + " = " +
				(location.isInitial() ? "1" : "0") + ";\n";
	}
	cout << initCondition;
	cout << "---------------------------" << endl;
	_init = isat3_node_create_from_string(_isatInstance, initCondition.c_str());
}

void LinearHybridAutomaton::setTarget(LinearPredicate target) {
	cout << "Creating Target Condition:" << endl;
	cout << target.toString(false)  << endl;
	cout << "---------------------------" << endl;
	std::string targetString = target.toString(false) + ";\n";
	_target = isat3_node_create_from_string(_isatInstance,
			targetString.c_str());
}

void LinearHybridAutomaton::solveBMCIsat() {
	i3_type_t _result = isat3_solve_bmc(_isatInstance, _init, _bmcFormula, _target, 0, TIMEFRAMES, 3140000);
}

string LinearHybridAutomaton::printIntervalOfVariableISat(string variableName, unsigned int tframe) {
	char interval[1000];
	struct isat3_node* variable = _variableNodeMap[variableName];
	if (variable != NULL) {
		printf("tframe %d: %s %s%1.5f, %1.5f%s\n",
				tframe,
				isat3_node_get_variable_name(_isatInstance, variable),
				isat3_is_lower_bound_strict(_isatInstance, variable, tframe) ? "(" : "[",
				isat3_get_lower_bound(_isatInstance, variable, tframe),
				isat3_get_upper_bound(_isatInstance, variable, tframe),
				isat3_is_upper_bound_strict(_isatInstance, variable, tframe) ? ")" : "]");
		sprintf(interval, "tframe %d: %s %s%1.5f, %1.5f%s\n",
				tframe,
				isat3_node_get_variable_name(_isatInstance, variable),
				isat3_is_lower_bound_strict(_isatInstance, variable, tframe) ? "(" : "[",
				isat3_get_lower_bound(_isatInstance, variable, tframe),
				isat3_get_upper_bound(_isatInstance, variable, tframe),
				isat3_is_upper_bound_strict(_isatInstance, variable, tframe) ? ")" : "]");
//		sprintf(interval, "%1.5f;%1.5f;",
//				isat3_get_lower_bound(_isatInstance, variable, tframe),
//				isat3_get_upper_bound(_isatInstance, variable, tframe));
	}
	string returnString;
	returnString.assign(interval);
	return returnString;
}

string LinearHybridAutomaton::printBMCResultIsat(unsigned int numberOfTimeframes) {
	string returnString = "";
	i3_type_t tframe = isat3_get_tframe(_isatInstance);
	cout << "In Timeframe " << tframe << ": " << isat3_get_result_string(_result) << endl;
	for (unsigned int i = 0; i < numberOfTimeframes; i++) {
		cout << "***************************************************************" << endl;
		cout << "                          Timeframe" << i << "                 " << endl;
		for (size_t k = 0; k < _variables.size(); k++) {
			returnString += printIntervalOfVariableISat(
					_variables[k].getName(), i);
		}
		for (size_t k = 0; k < _locations.size(); k++) {
			returnString += printIntervalOfVariableISat(
					_locations[k].getName(), i);
		}
		for (size_t k = 0; k < _edges.size(); k++) {
			returnString += printIntervalOfVariableISat(
					_edges[k].getName(), i);
		}

	}
	return returnString;
}
