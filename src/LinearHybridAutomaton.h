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
//#include <sstream>
#include <iostream>
#include <cstdio>

class LinearHybridAutomaton {
public:
	LinearHybridAutomaton();
	virtual ~LinearHybridAutomaton();

	void setLocations(std::vector<Location> locations);
	void addLocation(Location location);
	std::vector<Location> getLocations();

	void setEdges(std::vector<Edge> locations);
	void addEdge(Edge edge);
	std::vector<Edge> getEdges();

	void setVariables(std::vector<Variable> variables);
	std::vector<Variable> getVariables();
	void addVariable(Variable variable);

	const std::vector<Constant>& getConstants() const;
	void setConstants(const std::vector<Constant>& constants);
	void addConstant(Constant constant);

	void toIsat3BMC();
	void setUpIsat3();
	string setUpVariables();
	void setUpConstants();
	string setUpInitial();
	string setTarget(LinearPredicate target);
	string setTarget(string target);
	const LinearPredicate& getInitialPredicate() const;
	void solveBMCIsat();
	string printBMCResultIsat(unsigned int numberOfTimeframes);
	void writeToFile();
	void toHysFile(LinearPredicate target);
	void toHysFile(string target);

	string toString(double value);
	string toString(unsigned int value);

private:
	std::vector<Location>	_locations;
	std::vector<Edge>		_edges;
	std::vector<Variable>	_variables;
	std::vector<Constant>	_constants;

	FILE * _hysFile;
	bool _hysFileActive;


	/*
	 * iSat3 Stuff
	 */
	bool _isatReady;
	i3_type_t _tframe;

	struct isat3* 					_isatInstance;
	std::vector<struct isat3_node*> _isatConstants;
	std::vector<struct isat3_node*> _isatConstantsDefines;
	std::vector<struct isat3_node*> _isatVariables;
	std::map<string, struct isat3_node*> _variableNodeMap;
	std::map<string, struct isat3_node*> _locatioNodeMap;
	std::map<string, struct isat3_node*> _edgeNodeMap;
	i3_type_t _result;

	struct isat3_node* _init;
	struct isat3_node* _bmcFormula;
	struct isat3_node* _target;

	/*
	 * Fränzle encoding 1 paper
	 */
	struct isat3_node* exactlyOneState();
	struct isat3_node* exactlyOneLocation(string &hysString);
	struct isat3_node* exactlyOneTransition();
	struct isat3_node* asMostOneTransition();
	struct isat3_node* continuousStateComponents();
	struct isat3_node* jumpsNoTime();
	struct isat3_node* jumpsNoTime2();
	struct isat3_node* assignmentAndNoTime();
	struct isat3_node* invariantHoldsEntry();
	struct isat3_node* invariantHoldsExit() ;
	struct isat3_node* transitionStateChange();
	struct isat3_node* transitionGuard();
	struct isat3_node* transitionAssignment();
	struct isat3_node* stayInLocation();
	struct isat3_node* notFlowVariablesStayConstant();

	/*
	 * Fränzle encoding presentation
	 */
	struct isat3_node* jumps(string &hysString);
	struct isat3_node* flows(string &hysString);
	struct isat3_node* alternationAndSucessor(string &hysString);
	string printIntervalOfVariableISat(string variableName,
			unsigned int tframe);
	string printTruthValueOfVariable(string variableName, unsigned int tframe);
	void modifiedFraenzle();
	string setUpLocationVariables();
	string setUpEdgeVariables();
};

#endif /* LINEARHYBRIDAUTOMATON_H_ */
