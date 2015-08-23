/*
 * LinearHybridAutomaton.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "LinearHybridAutomaton.h"

//#ifndef
//#define

using namespace std;

LinearHybridAutomaton::LinearHybridAutomaton() {
	// TODO Auto-generated constructor stub
}

LinearHybridAutomaton::LinearHybridAutomaton(string name) {
	_name = name;
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

void LinearHybridAutomaton::setConstants(const vector<Constant>& constants) {
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
		isat3_node_create_define(_isatInstance, _constants[i].getName().c_str(),
				isat3_node_create_constant_float(_isatInstance, value));
		//		_isatConstantsDefines.push_back(isat3_node_create_define(
		//				_isatInstance, _constants[i].getName().c_str(), _isatConstants[i]));
	}

}

string LinearHybridAutomaton::setUpVariables() {
	Variable variable;
	string hysString = "";
	// TODO Modification here
	struct isat3_node* node = isat3_node_create_variable_boole(_isatInstance, "flow");
	hysString = "\tboole flow;\n";
	_isatVariables.push_back(node);
	_variableNodeMap["flow"] = node;
	for (size_t i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		struct isat3_node* node = isat3_node_create_variable_float(
				_isatInstance, variable.getName().c_str(),
				variable.getLowerBound(), variable.getUpperBound());
		// Write the node in the list.
		_isatVariables.push_back(node);
		hysString += "\t float [" + toString(variable.getLowerBound()) + "," +
				toString(variable.getUpperBound()) + "] " + variable.getName() +
				";\n";
		_variableNodeMap[variable.getName()] = node;
	}
	return hysString;
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
//	struct isat3_node* tmp1 = isat3_node_create_variable_integer(_isatInstance,
//			_locations[0].getName().c_str(), 0, 1);
	struct isat3_node* tmp1 = isat3_node_create_variable_boole(_isatInstance,
			_locations[0].getName().c_str());
	_variableNodeMap[_locations[0].getName()] = tmp1;
	atLeastOne = "(" + _locations[0].getName() + "'";
	//atMostOne = "   1 * abs(" + _locations[0].getName() + "' - 1)";
	//atMostOne = "!" + _locations[0].getName() + "'";
	atMostOne = _locations[0].getName() + "'";
	cout << "Creating 'Exactly One Location Constraint' : " << endl;
	cout << atLeastOne << "   |   " + atMostOne << endl;
	for (size_t i = 1; i < _locations.size(); i++) {
//		struct isat3_node* tmp2 = isat3_node_create_variable_integer(_isatInstance,
//				_locations[i].getName().c_str(), 0, 1);
		struct isat3_node* tmp2 = isat3_node_create_variable_boole(
				_isatInstance, _locations[i].getName().c_str());
		_variableNodeMap[_locations[i].getName()] = tmp2;
		atLeastOneTmp = " + " + _locations[i].getName() + "'";
		//atMostOneTmp = " + !" + _locations[i].getName() + "'";
		atMostOneTmp = " + " + _locations[i].getName() + "'";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		//cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1) and (";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%u", _locations.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	//atMostOneTmp = " >= " + tmpString + ");";
	atMostOneTmp = " <= 1);";
	atMostOne += atMostOneTmp;
	atLeastOne += atMostOne;
	//cout << atLeastOneTmp << "   |   "+ atMostOneTmp << endl;
	if (DEBUG)
		cout << atLeastOne << endl;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, atLeastOne.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::exactlyOneLocation(string &hysString) {
	string constraint = "";
	hysString += "\t";
	//setUpLocationVariables();
	for (size_t i = 0; i < _locations.size(); i++) {
		if (i < _locations.size() - 1) {
			constraint += _locations[i].getName() + "' + ";
			hysString += _locations[i].getName() + "' + ";
		}
		else {
			constraint += _locations[i].getName() + "' = 1;\n";
			hysString += _locations[i].getName() + "' = 1;\n";
		}
	}
	cout << "Exactly one Location: " << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
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
	atLeastOne = "(" + _edges[0].getName() + "'";
	atMostOne = "!" + _edges[0].getName() + "'";
	cout << "Exactly One Transition Constraint: " << endl;
	cout << atLeastOne << "   |   " + atMostOne << endl;
	for (size_t i = 1; i < _edges.size(); i++) {
		struct isat3_node* tmp2 = isat3_node_create_variable_integer(
				_isatInstance, _edges[i].getName().c_str(), 0, 1);
		_variableNodeMap[_edges[i].getName()] = tmp2;
		atLeastOneTmp = " + " + _edges[i].getName() + "'";
		atMostOneTmp = " + !" + _edges[i].getName() + "'";
		atLeastOne += atLeastOneTmp;
		atMostOne += atMostOneTmp;
		cout << atLeastOneTmp << "   |   " + atMostOneTmp << endl;
	}
	atLeastOneTmp = " >= 1) and (";
	atLeastOne += atLeastOneTmp;
	sprintf(tmp, "%u", _edges.size() - 1);
	string tmpString;
	tmpString.assign(tmp);
	atMostOneTmp = " >= " + tmpString + ");";
	atMostOne += atMostOneTmp;
	atLeastOne += atMostOne;
	cout << atLeastOneTmp << "   |   " + atMostOneTmp << endl;
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
//	struct isat3_node* tmp1 = isat3_node_create_variable_integer(_isatInstance,
//			_edges[0].getName().c_str(), 0, 1);
	struct isat3_node* tmp1 = isat3_node_create_variable_boole(_isatInstance,
			_edges[0].getName().c_str());
	_variableNodeMap[_edges[0].getName()] = tmp1;
	atMostOne = "(!" + _edges[0].getName() + "'";
	cout << "At most one transition Constraint: " << endl;
	for (size_t i = 1; i < _edges.size(); i++) {
//		struct isat3_node* tmp2 = isat3_node_create_variable_integer(_isatInstance,
//				_edges[i].getName().c_str(), 0, 1);
		struct isat3_node* tmp2 = isat3_node_create_variable_boole(
				_isatInstance, _edges[i].getName().c_str());
		_variableNodeMap[_edges[i].getName()] = tmp2;
		atMostOneTmp = " + !" + _edges[i].getName() + "'";
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
	Location source, destination, location;
	struct isat3_node* node;
	for (size_t k = 0; k < _locations.size(); k++) {
		location = _locations[k];
		constraint += location.getName();
		for (size_t i = 0; i < _edges.size(); i++) {
			edge = _edges[i];
			source = edge.getSource();
			if (source.getId() == location.getId())
				constraint += " and !" + edge.getName();// + " and ";// + source.getName()
					//+ " -> " + source.getName() + "';\n";// = " + source.getName() + ";\n";
		}
		constraint += " -> " + location.getName() + "';\n";
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
	vector<Bound> bounds;
	Bound bound;
	size_t numberOfBounds;
	Edge edge;
	Constant lower, upper;
	Variable variable;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		constraint += "(" + location.getName();	// + "'";
		for (size_t i = 0; i < _edges.size(); i++) {
			edge = _edges[i];
			if (edge.getSource().getId() == location.getId())
				constraint += " and !" + edge.getName();	// + "'";
		}
		constraint += " -> (";
		bounds = location.getBounds();
		numberOfBounds = bounds.size();
		for (size_t i = 0; i < numberOfBounds; i++) {
			bound = location.getBounds()[i];
			variable = bound.getVariable();
			variableName = variable.getName();
			upper = bound.getConstantUp();
			lower = bound.getConstantLow();
			constraint += variableName + "' >= " + variableName + " + "
					+ lower.getValueString() + " * " + DELTASTRING + " and "
					+ variableName + "' <= " + variableName + " + "
					+ upper.getValueString() + " * " + DELTASTRING
					+ (i < numberOfBounds - 1 ? " and " : "));\n");
//			constraint += variableName + "' >= " + variableName + " + "
//					+ lower.getValueString() + " * " + "t" + " and "
//					+ variableName + "' <= " + variableName + " + "
//					+ upper.getValueString() + " * " + "t"
//					+ (i < numberOfBounds - 1 ? " and " : "));\n");
		}
	}
	cout << "Continuous State Components:" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::jumpsNoTime() {
	string constraint = "";
	string variableName;
	string relation;
	Location location;
	string slope;
	vector<Bound> bounds;
	Bound bound;
	size_t numberOfBounds;
	Edge edge;
	Constant lower, upper;
	Variable variable;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		vector<Edge> edges;
		// Find all edges that have the current location as their source.
		for (size_t i = 0; i < _edges.size(); i++) {
			edge = _edges[i];
			if (edge.getSource().getId() == location.getId())
				edges.push_back(edge);
		}
		if (edges.size() > 0) {
			constraint += "(" + location.getName() + " and (";	// + "'";
			constraint += edges[0].getName();
			// And add them to the constraint.
			for (size_t i = 1; i < edges.size(); i++)
				constraint += " or " + edges[i].getName();	// + "'";
			constraint += ") -> (";
			bounds = location.getBounds();
			numberOfBounds = bounds.size();
			for (size_t i = 0; i < numberOfBounds; i++) {
				bound = location.getBounds()[i];
				variable = bound.getVariable();
				variableName = variable.getName();
				// Check for assignments of variables. If on a edge the
				// variable is assigned to a new value, do the assignment,
				// else stay constant.
//				for (size_t k = 0; k < edges.size(); k++) {
//					if (edges[k].getAssignment().getLinPreds()[0].
//							getLinTerms()[0].getVariable().getName() ==
//									variableName)
//						constraint += edges[k].getAssignment().
//						toStringISat(true) +
//						(i < numberOfBounds - 1 ? " and " : "));\n");
//				}
				constraint += variableName + "' = " + variableName
				+ (i < numberOfBounds - 1 ? " and " : "));\n");
			}
		}
	}
	cout << "Jumps consume no time:" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::jumpsNoTime2() {
	string constraint1 = "";
	string constraint2 = "";
	string assignedVariable;
	string variableName;
	Location location;
	string slope;
	vector<Bound> bounds;
	Bound bound;
	Edge edge;
	Constant lower, upper;
	Variable variable;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		if (!edge.isHasAssignment()) {
			if (i == 0)
				constraint1 = "(" + edge.getName();
			else
				constraint1 += " or " + edge.getName();
		} else {
				constraint2 += "(" + edge.getName() + " -> (" +
						edge.getAssignment().toStringISat(true);//);//;\n";
				assignedVariable = edge.getAssignment().getLinPreds()[0].
						getLinTerms()[0].getVariable().getName();
				for (size_t k = 0; k < _variables.size(); k++) {
					if (_variables[k].getName() != assignedVariable)
						constraint2  += " and " + _variables[k].
						stayConstantAsStringIsat();
				}
				constraint2 += "));\n";
		}
	}
	constraint1 += ") -> (" + _variables[0].stayConstantAsStringIsat();
	for (size_t k = 1; k < _variables.size(); k++) {
			constraint1  += " and " + _variables[k].
			stayConstantAsStringIsat();
	}
	constraint1 += ");\n";
	//constraint1 += constraint2;
	cout << "Jumps consume no time:" << endl;
	cout << constraint1;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint1.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::assignmentAndNoTime() {
	Assignment assignment;
	string constraint = "";
	string assignedVariable;
	string variableName;
	Location location;
	string slope;
	vector<Bound> bounds;
	Bound bound;
	Edge edge;
	Constant lower, upper;
	Variable variable;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		assignment = edge.getAssignment();
		constraint += "(" + edge.getName() + " -> ";
		for (size_t k = 0; k < _variables.size(); k++) {
			variable = _variables[k];
			if (k > 0)
				constraint += " and ";
			if (assignment.isAssignedVariable(variable.getName()))
				constraint += assignment.toStringISat(true);
			else
				constraint += variable.stayConstantAsStringIsat();
		}
		constraint += ");\n";
	}
	cout << "Assignments, and jumps consume no time:" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

void LinearHybridAutomaton::modifiedFraenzle() {
	setUpEdgeVariables();
	_bmcFormula = exactlyOneState();
//		_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//				ISAT3_NODE_BOP_AND, _bmcFormula, exactlyOneTransition());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//			ISAT3_NODE_BOP_AND, _bmcFormula, asMostOneTransition());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, continuousStateComponents());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
			ISAT3_NODE_BOP_AND, _bmcFormula, assignmentAndNoTime());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//			ISAT3_NODE_BOP_AND, _bmcFormula, invariantHoldsEntry());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, invariantHoldsExit());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, transitionStateChange());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, stayInLocation());
	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
	ISAT3_NODE_BOP_AND, _bmcFormula, transitionGuard());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//	ISAT3_NODE_BOP_AND, _bmcFormula, transitionAssignment());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//	ISAT3_NODE_BOP_AND, _bmcFormula, notFlowVariablesStayConstant());
}

void LinearHybridAutomaton::toIsat3BMC() {
	modifiedFraenzle();
//	string hysdummy;
//	setUpEdgeVariables();
//	_bmcFormula = exactlyOneLocation(hysdummy);
////	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
////			ISAT3_NODE_BOP_AND, _bmcFormula, jumps());
////	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
////			ISAT3_NODE_BOP_AND, _bmcFormula, flows());
//	_bmcFormula = isat3_node_create_binary_operation(_isatInstance,
//			ISAT3_NODE_BOP_AND, _bmcFormula, alternationAndSucessor(hysdummy));
}

string LinearHybridAutomaton::setUpLocationVariables() {
	Location location;
	string hysString = "";
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		struct isat3_node* node = isat3_node_create_variable_boole(
				_isatInstance, location.getName().c_str());
		// Write the node in the list.
//		_isatVariables.push_back(node);
		_variableNodeMap[location.getName()] = node;
		// Write to hys-String
		hysString += "\t boole " +  location.getName() + ";\n";
	}
	return hysString;
}

string LinearHybridAutomaton::setUpEdgeVariables() {
	Edge edge;
	string hysString = "";
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		//struct isat3_node* node = isat3_node_create_variable_integer(_isatInstance,
		//	location.getName().c_str(), 0, 1);
		struct isat3_node* node = isat3_node_create_variable_boole(
				_isatInstance, edge.getName().c_str());
		//Write the node in the list.
		_isatVariables.push_back(node);
		_variableNodeMap[edge.getName()] = node;
		hysString += "\t boole" +  edge.getName() + ";\n";
	}
	return hysString;
}
struct isat3_node* LinearHybridAutomaton::jumps(string &hysString) {
	string constraint = "";
	Edge edge;
	Location source;
	Location destination;
	Guard guard;
	Assignment assignment;
	Variable variable;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		source = edge.getSource();
		destination = edge.getDestination();
		guard = edge.getGuard();
		assignment = edge.getAssignment();
		constraint += source.getName() + " and " + destination.getName()
				+ "' -> " + guard.toStringISat(false) + " and "
				+ assignment.toStringISat(true);
		hysString += "\t" + source.getName() + " and " + destination.getName()
				+ "' -> " + guard.toStringISat(false) + " and\n\t\t\t"
				+ assignment.toStringISat(true) + + "\n\t\t\t";
		for (size_t k = 0; k < _variables.size(); k++) {
			variable = _variables[k];
			if (!assignment.isAssignedVariable(variable.getName()) &&
					variable.getName() != "t") {
				constraint += " and " + variable.getName() + "' = " +
						variable.getName() + "\n\t\t\t";
				hysString += " and " + variable.getName() + "' = " +
						variable.getName() + "\n\t\t\t";
			}
		}
		constraint += " and (t = 0.00000);\n";
		hysString += " and (t = 0.00000);\n";
	}
	cout << "Jumps : " << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}
struct isat3_node* LinearHybridAutomaton::flows(string &hysString) {
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
		constraint += locationName + " and " + locationName + "'"
				+ " -> ";
		hysString += "\t" + locationName + " and " + locationName + "'"
				+ " -> ";
		constraint += "(t > 0.00000) and\n\t\t\t";
		hysString += "(t > 0.00000) and\n\t\t\t";
//		constraint += "(t > 0.40000) and ";
//		hysString += "(t > 0.40000) and\n\t\t\t";
		// Upper and lower bounds.
		for (size_t k = 0; k < location.getBounds().size(); k++) {
			bound = location.getBounds()[k];
			variable = bound.getVariable();
			variableName = variable.getName();
			upper = bound.getConstantUp();
			lower = bound.getConstantLow();
//			constraint += variableName + "' >= " + variableName + " + "
//					+ lower.getValueString() + " * " + DELTASTRING + " and "
//					+ variableName + "' <= " + variableName + " + "
//					+ upper.getValueString() + " * " + DELTASTRING + " and ";
			constraint += variableName + "' >= " + variableName + " + "
					+ lower.getValueString() + " * " + "t" + " and\n\t\t\t"
					+ variableName + "' <= " + variableName + " + "
					+ upper.getValueString() + " * " + "t" + " and\n\t\t\t";
			hysString += variableName + "' >= " + variableName + " + "
					+ lower.getValueString() + " * " + "t" + " and\n\t\t\t"
					+ variableName + "' <= " + variableName + " + "
					+ upper.getValueString() + " * " + "t" + " and\n\t\t\t";
		}
		invariant = location.getInvariant();
		linPreds = invariant.getLinPreds();
		constraint += invariant.toStringISat(true) + ";\n";
		hysString += invariant.toStringISat(true) + ";\n";
//		// Invariants
//		for (size_t k = 0; k < linPreds.getLinPreds().size(); k++) {
//			linPred = linPreds.getLinPreds()[k];
//			constraint += linPred.toString(true)// + " and ";
//							+ (k < linPreds.getLinPreds().size() - 1 ?
//									" and " : ");\n");
//			hysString += linPred.toString(true)// + " and ";
//							+ (k < linPreds.getLinPreds().size() - 1 ?
//									" and " : ");\n");
//		}
//		constraint += ";\n";
//		hysString += ";\n";
	}
	cout << "Flows : " << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

struct isat3_node* LinearHybridAutomaton::alternationAndSucessor(string &hysString) {
	string constraint = "";
	Edge edge;
	Location source, destination;

	constraint += "flow <-> !flow';\n";
	hysString += "\t flow <-> !flow';\n";
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		source = edge.getSource();
		destination = edge.getDestination();
		constraint += "(flow and " + source.getName() + ") -> " +
				source.getName() + "';\n";
		hysString += "\t (flow and " + source.getName() + ") -> " +
						source.getName() + "';\n";
	}
	bool locationFound = false;
	for (size_t k = 0; k < _locations.size(); k++) {
		source = _locations[k];
		locationFound = false;
		for (size_t i = 0; i < _edges.size(); i++) {
			edge = _edges[i];
			if (edge.getSource().getId() == source.getId()) {
				if (!locationFound) {
					constraint += "!flow and " + source.getName() + " -> " +
							edge.getDestination().getName() + "'";
					hysString += "!flow and " + source.getName() + " -> " +
							edge.getDestination().getName() + "'";
					locationFound = true;
				} else {
					constraint += " or " + edge.getDestination().getName() + "'";
					hysString += " or " + edge.getDestination().getName() + "'";
				}
			}
		}
		if (locationFound) {
			constraint += ";\n";
			hysString += ";\n";
		}
	}
//	for (size_t i = 0; i < _edges.size(); i++) {
//		edge = _edges[i];
//		source = edge.getSource();
//		destination = edge.getDestination();
//		constraint += "(!flow and " + source.getName() + ") -> " +
//				destination.getName() + "';\n";
//		hysString += "\t (!flow and " + source.getName() + ") -> " +
//						destination.getName() + "';\n";
//	}
	cout << "Alternation and successor:" << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
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
		constraint += "(" + location.getName() + " -> (";
		invariant = _locations[i].getInvariant();
		constraint += invariant.toStringISat(false)
				+ (i < _locations.size() - 1 ? ")) and\n" : "));\n");
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
		constraint += "(" + location.getName() + "' -> (";
		invariant = _locations[i].getInvariant();
		constraint += invariant.toStringISat(true)
				+ (i < _locations.size() - 1 ? ")) and\n" : "));\n");
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
	//constraint = "2 * abs(" + transitionName + " - 1) + 1 * "; // 2 * /edge + 1 *
//	constraint = "2 * !" + transitionName + " + "; // 2 * /edge + 1 *
//	constraint += sourceName + " + " + destinationName + "' >= 2;\n"; // state + 1 * state' >= 2
	constraint += "(" + transitionName + " and " + sourceName + " -> "
			+ destinationName + "');\n";		// or " + sourceName + "');\n";
	for (size_t i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		sourceName = edge.getSource().getName();
		destinationName = edge.getDestination().getName();
		//constraint += "2 * abs(" + transitionName + " - 1) + 1 * "; // 2 * /edge + 1 *
//		constraint += "2 * !" + transitionName + " + "; // 2 * /edge + 1 *
//		constraint += sourceName + " + " + destinationName + "' >= 2;\n"; // state + 1 * state' >= 2
		constraint += "(" + transitionName + " and " + sourceName + " -> "
				+ destinationName + "');\n";
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
	constraint += transitionName + "' <-> (" + guard + " and " + sourceName
			+ "');\n";
	for (size_t i = 1; i < _edges.size(); i++) {
		edge = _edges[i];
		transitionName = edge.getName();
		sourceName = edge.getSource().getName();
		guard = edge.getGuard().toStringISat(true);
		constraint += transitionName + "' <-> (" + guard + " and " + sourceName
				+ "');\n";
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
//	edge = _edges[0];
//	transitionName = edge.getName();
	//assignment = edge.getAssignment().toStringISat(true);
	//constraint = transitionName + " -> (" + assignment + ");\n";
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		if (edge.isHasAssignment()) {
			transitionName = edge.getName();
			assignment = edge.getAssignment().toStringISat(true);
			constraint += transitionName + " -> (" + assignment + ");\n";
		}
	}
	if (constraint.length() == 0) {
		constraint = "true";
	}
	cout << "Transition -> Assignment" << endl;
	cout << constraint;
	cout << "----------------------------------------" << endl;
	// Create the node in the isatinstance
	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}
//struct isat3_node* LinearHybridAutomaton::notFlowVariablesStayConstant() {
//	struct isat3_node* node;
//	string constraint = "";
//	string variableName;
//	Variable variable;
//	Edge edge;
//	for (size_t i = 0; i < _edges.size(); i++) {
//		edge = _edges[i];
//		variable =
//				edge.getAssignment().getLinPreds()[i].getLinTerms()[0].getVariable();
//		if (!variable.isFlowVariable()) {
//			variableName = variable.getName();
//			constraint += "(" + edge.getName() + " = 0 -> (" + variableName
//					+ "' = " + variableName + "));";
//		}
//	}
//	cout << "Not - flow - variables stay constant in locations :" << endl;
//	cout << constraint;
//	cout << "----------------------------------------" << endl;
//	node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
//	return node;
//}

string LinearHybridAutomaton::setUpInitial() {
	string initCondition = "";
	string hysString = "";
	Variable variable;
	for (size_t i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		// TODO: Modification done here.
		if (variable.isInitialized()) {
			initCondition += variable.getName() + " = "
			+ variable.getInitialValueAsString() + ";\n";
			hysString += "\t" + variable.getName() + " = "
			+ variable.getInitialValueAsString() + ";\n";
		}
	}
#ifdef ENCODING1
	Edge edge;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		initCondition += "!" + edge.getName() + ";\n";
	}
#endif
	Location location;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		initCondition += (location.isInitial() ? "" : "!") + location.getName()
				+ ";\n";
		hysString += "\t";
		hysString += (location.isInitial() ? "" : "!") + location.getName()
				+ ";\n";
	}
	// TODO: Modification done here.
#ifdef ENCODING2
	initCondition += "flow;\n";
	hysString += "\tflow;\n";
#endif
	cout << "Initial condition:" << endl;
	cout << initCondition;
	cout << "---------------------------" << endl;
	_init = isat3_node_create_from_string(_isatInstance, initCondition.c_str());
	return hysString;
}

string LinearHybridAutomaton::setTarget(LinearPredicate target) {
	string hysString = "";
	cout << "Creating Target Condition:" << endl;
	cout << target.toString(false) << endl;
	cout << "---------------------------" << endl;
	string targetString = target.toString(false) + ";\n";
	hysString = "\t" + target.toString(false) + ";\n";
	_target = isat3_node_create_from_string(_isatInstance,
			targetString.c_str());
	//_target = isat3_node_create_from_string(_isatInstance, "one_three;\n");
	//hysString += "one_three;\n";
	return hysString;
}

string LinearHybridAutomaton::setTarget(string target) {
	string hysString = "";
	string targetString = target + "\n";
	cout << "Creating Target Condition:" << endl;
	cout << targetString << endl;
	cout << "---------------------------" << endl;
	hysString = "\t" + targetString;
	_target = isat3_node_create_from_string(_isatInstance,
			targetString.c_str());
	return hysString;
}

void LinearHybridAutomaton::solveBMCIsat() {
	_result = isat3_solve_bmc(_isatInstance, _init, _bmcFormula,
			_target, 0, TIMEFRAMES, 6280000);
}

string LinearHybridAutomaton::printIntervalOfVariableISat(string variableName,
		unsigned int tframe) {
	char interval[1000];
	struct isat3_node* variable = _variableNodeMap[variableName];
	if (variable != NULL) {
		printf("tframe %d: %s %s%1.5f, %1.5f%s\n", tframe,
				isat3_node_get_variable_name(_isatInstance, variable),
				isat3_is_lower_bound_strict(_isatInstance, variable, tframe) ?
						"(" : "[",
				isat3_get_lower_bound(_isatInstance, variable, tframe),
				isat3_get_upper_bound(_isatInstance, variable, tframe),
				isat3_is_upper_bound_strict(_isatInstance, variable, tframe) ?
						")" : "]");
//		sprintf(interval, "tframe %d: %s %s%1.5f, %1.5f%s\n", tframe,
//				isat3_node_get_variable_name(_isatInstance, variable),
//				isat3_is_lower_bound_strict(_isatInstance, variable, tframe) ?
//						"(" : "[",
//				isat3_get_lower_bound(_isatInstance, variable, tframe),
//				isat3_get_upper_bound(_isatInstance, variable, tframe),
//				isat3_is_upper_bound_strict(_isatInstance, variable, tframe) ?
//						")" : "]");
		sprintf(interval, "%1.5f;%1.5f;",
				isat3_get_lower_bound(_isatInstance, variable, tframe),
				isat3_get_upper_bound(_isatInstance, variable, tframe));
	}
	string returnString;
	returnString.assign(interval);
	return returnString;
}

string LinearHybridAutomaton::printTruthValueOfVariable(string variableName,
		unsigned int tframe) {
	char value[1000];
	struct isat3_node* variable = _variableNodeMap[variableName];
	cout << "tframe " << tframe << ": "
			<< (isat3_get_truth_value(_isatInstance, variable, tframe) ?
					"" : "!")
			<< isat3_node_get_variable_name(_isatInstance, variable) << endl;

//	printf("tframe %d: %s %s\n", tframe,
//			isat3_node_get_variable_name(_isatInstance, variable),
//			isat3_get_truth_value(_isatInstance, variable, tframe) ? "true" : "false");
	sprintf(value, "%s;",isat3_get_truth_value(_isatInstance, variable, tframe) ? "1" : "0");
	string returnString;
	returnString.assign(value);
	return returnString;
}

string LinearHybridAutomaton::printBMCResultIsat(
		unsigned int numberOfTimeframes) {
	string returnString = "";
	_tframe = isat3_get_tframe(_isatInstance);
	cout << "In Timeframe " << _tframe << ": "
			<< isat3_get_result_string(_result) << endl;
	for (unsigned int i = 0; i <= _tframe; i++) {
		cout
				<< "***************************************************************"
				<< endl;
		cout << "                          Timeframe" << i
				<< "                 " << endl;
		// TODO: Modification done here.
		printTruthValueOfVariable("flow", i);
		for (size_t k = 0; k < _variables.size(); k++) {
			returnString += printIntervalOfVariableISat(_variables[k].getName(),
					i);
		}
		for (size_t k = 0; k < _locations.size(); k++) {
			returnString += printTruthValueOfVariable(_locations[k].getName(),
					i);
		}
		// TODO: Modification done here.
		for (size_t k = 0; k < _edges.size(); k++) {
			returnString += printTruthValueOfVariable(_edges[k].getName(), i);
		}

	}
	return returnString;
}

void LinearHybridAutomaton::toHysFile(LinearPredicate target) {
	_hysFile = fopen("LHA.hys", "w");
	_hysFileActive = true;
	setUpConstants();
	fprintf(_hysFile, "DECL\n");
	fprintf(_hysFile, setUpVariables().c_str());
	fprintf(_hysFile, setUpLocationVariables().c_str());
	fprintf(_hysFile, "\nINIT\n");
	fprintf(_hysFile, setUpInitial().c_str());
	fprintf(_hysFile, "\nTRANS\n");
	string hysString = "";
	exactlyOneLocation(hysString);
	fprintf(_hysFile, hysString.c_str());
	hysString = "";
	alternationAndSucessor(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\n");
	hysString = "";
	jumps(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\n");
	hysString = "";
	flows(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\nTARGET\n");
	fprintf(_hysFile, setTarget(target).c_str());
	fclose(_hysFile);
}

void LinearHybridAutomaton::toHysFile(string target) {
	_hysFile = fopen("LHA.hys", "w");
	_hysFileActive = true;
	setUpConstants();
	fprintf(_hysFile, "DECL\n");
	fprintf(_hysFile, setUpVariables().c_str());
	fprintf(_hysFile, setUpLocationVariables().c_str());
	fprintf(_hysFile, "\nINIT\n");
	fprintf(_hysFile, setUpInitial().c_str());
	fprintf(_hysFile, "\nTRANS\n");
	string hysString = "";
	exactlyOneLocation(hysString);
	fprintf(_hysFile, hysString.c_str());
	hysString = "";
	alternationAndSucessor(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\n");
	hysString = "";
	jumps(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\n");
	hysString = "";
	flows(hysString);
	fprintf(_hysFile, hysString.c_str());
	fprintf(_hysFile, "\nTARGET\n");
	fprintf(_hysFile, setTarget(target).c_str());
	fclose(_hysFile);
}

void LinearHybridAutomaton::toSpaceExXML(const string& target) {
	_spaceExXMLFile = fopen("LHA.xml", "w");
	string xmlString = "<\?xml version=\"1.0\" encoding=\"iso-8859-1\"\?>\n  <sspaceex xmlns=\"http://www-verimag.imag.fr/xml-namespaces/sspaceex\" version=\"0.2\" math=\"SpaceEx\">""\n";
	fprintf(_spaceExXMLFile, xmlString.c_str());
	xmlString =  "<component id=\"" + _name + "\">\n";
	fprintf(_spaceExXMLFile, xmlString.c_str());
	xmlString = "";
	xmlWriteParams(xmlString);
	fprintf(_spaceExXMLFile, xmlString.c_str());
	xmlString = "";
	xmlWriteLocations(xmlString);
	fprintf(_spaceExXMLFile, xmlString.c_str());
	xmlString = "";
	xmlWriteEdges(xmlString);
	fprintf(_spaceExXMLFile, xmlString.c_str());
	xmlString = "  </component> \n </sspaceex>";
	fprintf(_spaceExXMLFile, xmlString.c_str());
	fclose(_spaceExXMLFile);
}

void LinearHybridAutomaton::xmlWriteParams(string& params) {
	Variable variable;
	for (size_t i = 0; i < _variables.size(); i++) {
		variable = _variables[i];
		params += "  <param name=\"" + variable.getName() + "\" " +
				"type=\"real\" local=\"false\" d1=\"1\" d2=\"1\" dynamics=\"any\" />\n";
	}
	Constant constant;
	for (size_t i = 0; i < _constants.size(); i++) {
		constant = _constants[i];
		params += "  <param name=\"" + constant.getName() + "\" " +
				"type=\"real\" local=\"false\" d1=\"1\" d2=\"1\" dynamics=\"const\" />\n";
	}
	Edge edge;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		params += "  <param name=\"" + edge.getName() + "\" " +
				"type=\"label\" local=\"false\"/>\n";
	}
}

void LinearHybridAutomaton::xmlWriteLocations(string& locations) {
	Location location;
	Invariant invariant;
	locations += "\n";
	unsigned int x = 200;
	unsigned int y = 200;
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		invariant = location.getInvariant();
		locations += "  <location id =\"" + toString(location.getId()) + "\"" +
				" name=\"" + location.getName() + "\"" +
				" x=\"" + toString(x) + "\"" +
				" y=\"" + toString(y) + "\"" +
				" width=\"" + toString((unsigned int)100) + "\"" +
				" height=\"" + toString((unsigned int)100) + "\">\n";
		invariant.toStringSpaceExXML(locations);
		location.flowToSpaceExXML(locations);
		locations += "  </location>\n";
		x += 150;
	}
}

void LinearHybridAutomaton::xmlWriteEdges(string& edges) {
	Edge edge;
	Guard guard;
	Assignment assignment;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		edges += "    <transition source=\"" +
				toString(edge.getSource().getId()) + "\" target=\"" +
				toString(edge.getDestination().getId()) + "\">\n";
		edges += "      <label>" + edge.getName() + "</label>\n";
		edge.getGuard().toStringSpaceExXML(edges);
		edges += "\n";
		edge.getAssignment().toStringSpaceExXML(edges, _variables);
		edges += "    </transition>\n";
	}
}

void LinearHybridAutomaton::writeToFile() {
	int i = 0;
	FILE * theFile = fopen("trace.txt", "w");
	string header = "TF;";
	for (size_t k = 0; k < _locations.size(); k++) {
		header += _locations[k].getName() + ";";
	}
	for (size_t k = 0; k < _edges.size(); k++) {
		header += _edges[k].getName() + ";";
	}
	for (size_t k = 0; k < _variables.size(); k++) {
		header += "[" + _variables[k].getName() + ";" +
				_variables[k].getName() + "];";
	}
	header += "\n";
	fprintf(theFile, header.c_str());
	char dummy[100];
	string data = "";
	for (unsigned int i = 0; i < _tframe; i++) {
		sprintf(dummy, "%u;", i);
		data = dummy;
		if (i > 0)
			theFile = fopen("trace.txt", "a");
		for (size_t k = 0; k < _locations.size(); k++) {
			data += printTruthValueOfVariable(_locations[k].getName(), i);
		}
		for (size_t k = 0; k < _edges.size(); k++) {
			data += printTruthValueOfVariable(_edges[k].getName(), i);
		}
		for (size_t k = 0; k < _variables.size(); k++) {
			data += printIntervalOfVariableISat(_variables[k].getName(),i);
		}
		data += "\n";
		fprintf(theFile, data.c_str());
		fclose(theFile);
	}
}

string LinearHybridAutomaton::toString(double value) {
	char tmp[100];
	sprintf(tmp, "%1.5f", value);
	string string;
	string.assign(tmp);
	return string;
}

string LinearHybridAutomaton::toString(unsigned int value) {
	char tmp[100];
	sprintf(tmp, "%u", value);
	string string;
	string.assign(tmp);
	return string;
}
