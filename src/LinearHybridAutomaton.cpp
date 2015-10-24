/*
 * LinearHybridAutomaton.cpp
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#include "LinearHybridAutomaton.h"

using namespace std;

LinearHybridAutomaton::LinearHybridAutomaton() {
}

LinearHybridAutomaton::LinearHybridAutomaton(string name) {
	_name = name;
}

LinearHybridAutomaton::~LinearHybridAutomaton() {
	// TODO Auto-generated destructor stub
}

void LinearHybridAutomaton::setLocations(const vector<Location>& locations) {
	_locations = locations;
}
vector<Location> LinearHybridAutomaton::getLocations() {
	return _locations;
}

void LinearHybridAutomaton::setEdges(const vector<Edge>& edges) {
	_edges = edges;
}
vector<Edge> LinearHybridAutomaton::getEdges() {
	return _edges;
}

void LinearHybridAutomaton::setVariables(const vector<Variable>& variables) {
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

void LinearHybridAutomaton::addLocation(const Location& location) {
	_locations.push_back(location);
}

void LinearHybridAutomaton::addEdge(const Edge& edge) {
	_edges.push_back(edge);
}

void LinearHybridAutomaton::addVariable(const Variable& variable) {
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

struct isat3_node* LinearHybridAutomaton::exactlyOneLocation(string &hysString) {
	string constraint = "";
	hysString += "\t";
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
	/* Create the node in the isatinstance */
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance, constraint.c_str());
	return node;
}

string LinearHybridAutomaton::setUpLocationVariables() {
	Location location;
	string hysString = "";
	for (size_t i = 0; i < _locations.size(); i++) {
		location = _locations[i];
		struct isat3_node* node = isat3_node_create_variable_boole(
				_isatInstance, location.getName().c_str());
		/* Write the node in the list. */
		_variableNodeMap[location.getName()] = node;
		/* Write to hys-String */
		hysString += "\t boole " +  location.getName() + ";\n";
	}
	return hysString;
}

string LinearHybridAutomaton::setUpEdgeVariables() {
	Edge edge;
	string hysString = "";
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		struct isat3_node* node = isat3_node_create_variable_boole(
				_isatInstance, edge.getName().c_str());
		/* Write the node in the list. */
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
	Invariant destInvariant;
	Variable variable;
	for (size_t i = 0; i < _edges.size(); i++) {
		edge = _edges[i];
		source = edge.getSource();
		destination = edge.getDestination();
		guard = edge.getGuard();
		assignment = edge.getAssignment();
		destInvariant = destination.getInvariant();
		/*
		 * Assignment and guard.
		 */
		constraint += source.getName() + " and " + destination.getName()
				+ "' -> " + guard.toStringISat(false) + " and "
				+ assignment.toStringISat(true) + " and "
				+ destInvariant.toStringISat(true);
		hysString += "\t" + source.getName() + " and " + destination.getName()
				+ "' -> " + guard.toStringISat(false) + " and\n\t\t\t"
				+ assignment.toStringISat(true) + " and\n\t\t\t"
				+ destInvariant.toStringISat(true) + "\n\t\t\t";

		/*
		 * Variables that are not assigned stay constant.
		 */
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
		/*
		 * Jumps take no time.
		 */
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
		/* Upper and lower bounds. */
		for (size_t k = 0; k < location.getBounds().size(); k++) {
			bound = location.getBounds()[k];
			variable = bound.getVariable();
			variableName = variable.getName();
			upper = bound.getConstantUp();
			lower = bound.getConstantLow();
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
	cout << "Alternation and successor:" << endl;
	cout << constraint;
	cout << "--------------------------------------------------------" << endl;
	struct isat3_node* node = isat3_node_create_from_string(_isatInstance,
			constraint.c_str());
	return node;
}

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
		printTruthValueOfVariable("flow", i);
		for (size_t k = 0; k < _variables.size(); k++) {
			returnString += printIntervalOfVariableISat(_variables[k].getName(),
					i);
		}
		for (size_t k = 0; k < _locations.size(); k++) {
			returnString += printTruthValueOfVariable(_locations[k].getName(),
					i);
		}
		for (size_t k = 0; k < _edges.size(); k++) {
			returnString += printTruthValueOfVariable(_edges[k].getName(), i);
		}

	}
	return returnString;
}

void LinearHybridAutomaton::toHysFile(const LinearPredicate& target) {
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

void LinearHybridAutomaton::toHysFile(const string& target) {
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

const string LinearHybridAutomaton::toString(const double value, const unsigned int digits) {
	char tmp[150];
	string format = "%1." + toString(digits) + "f";
	sprintf(tmp, format.c_str(), value);
	string string;
	string.assign(tmp);
	return string;
}

const string LinearHybridAutomaton::toString(const unsigned int value) {
	char tmp[100];
	sprintf(tmp, "%u", value);
	string string;
	string.assign(tmp);
	return string;
}
