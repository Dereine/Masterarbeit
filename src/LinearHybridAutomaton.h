/*
 * LinearHybridAutomaton.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef LINEARHYBRIDAUTOMATON_H_
#define LINEARHYBRIDAUTOMATON_H_

#define TIMEFRAMES 		220
#define DEBUG 			1
#define DELTASTRING 	"1"
#define DELTANR			1

#include <vector>
#include <map>
#include "Location.h"
#include "Edge.h"
#include "Variable.h"
#include "Constant.h"

#include <isat3.h>
#include <isat3types.h>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

class LinearHybridAutomaton {
public:
	LinearHybridAutomaton();
	LinearHybridAutomaton(string name);
	virtual ~LinearHybridAutomaton();

	/*
	 * Getter and Setter methods.
	 */
	void setLocations(const vector<Location>& locations);
	void addLocation(const Location& location);
	vector<Location> getLocations();

	void setEdges(const vector<Edge>& locations);
	void addEdge(const Edge& edge);
	vector<Edge> getEdges();

	void setVariables(const vector<Variable>& variables);
	vector<Variable> getVariables();
	void addVariable(const Variable& variable);

	const vector<Constant>& getConstants() const;
	void setConstants(const vector<Constant>& constants);
	void addConstant(Constant constant);

	/*
	 * iSat3 Methods
	 */
	void setUpIsat3();

	string setUpInitial();
	string setTarget(LinearPredicate target);
	string setTarget(string target);
	const LinearPredicate& getInitialPredicate() const;
	void solveBMCIsat();
	string printBMCResultIsat(unsigned int numberOfTimeframes);
	void writeToFile();
	void toHysFile(const LinearPredicate& target);
	void toHysFile(const string& target);

	/*
	 * SpaceEx Methods
	 */
	void toSpaceExXML(const string& target);
	void xmlWriteParams(string& params);
	void xmlWriteLocations(string& locations);
	void xmlWriteEdges(string& edges);

	const string toString(const double value, const unsigned int digits);
	const string toString(const unsigned int value);

private:
	vector<Location>	_locations;
	vector<Edge>		_edges;
	vector<Variable>	_variables;
	vector<Constant>	_constants;
	string 				_name;

	FILE * 	_hysFile;
	bool 	_hysFileActive;
	FILE * 	_spaceExXMLFile;

	/*
	 * iSat3 Stuff
	 */
	bool _isatReady;
	i3_type_t _tframe;

	struct isat3* 					_isatInstance;
	vector<struct isat3_node*>		_isatConstants;
	vector<struct isat3_node*> 		_isatConstantsDefines;
	vector<struct isat3_node*> 		_isatVariables;
	map<string, struct isat3_node*> _variableNodeMap;
	map<string, struct isat3_node*> _locatioNodeMap;
	map<string, struct isat3_node*> _edgeNodeMap;
	i3_type_t 						_result;

	struct isat3_node* _init;
	struct isat3_node* _bmcFormula;
	struct isat3_node* _target;

	/*
     * iSat3 Methods
	 */
	string setUpVariables();
	void setUpConstants();
	struct isat3_node* exactlyOneLocation(string &hysString);
	struct isat3_node* jumps(string &hysString);
	struct isat3_node* flows(string &hysString);
	struct isat3_node* alternationAndSucessor(string &hysString);
	string printIntervalOfVariableISat(string variableName,
			unsigned int tframe);
	string printTruthValueOfVariable(string variableName, unsigned int tframe);
	string setUpLocationVariables();
	string setUpEdgeVariables();
};

#endif /* LINEARHYBRIDAUTOMATON_H_ */
