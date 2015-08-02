/*
 * LinearHybridAutomaton.h
 *
 *  Created on: 20.07.2015
 *      Author: johannes
 */

#ifndef LINEARHYBRIDAUTOMATON_H_
#define LINEARHYBRIDAUTOMATON_H_

#define TIMEFRAMES 250

#include <vector>
#include <map>
#include "Location.h"
#include "Edge.h"
#include "Variable.h"
#include "Constant.h"

#include <isat3.h>
#include <isat3types.h>
#include <string.h>
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
	void setUpVariables();
	void setUpConstants();
	void setUpInitial();
	void setTarget(LinearPredicate target);
	const LinearPredicate& getInitialPredicate() const;
	void solveBMCIsat();
	string printBMCResultIsat(unsigned int numberOfTimeframes);

private:
	std::vector<Location>	_locations;
	std::vector<Edge>		_edges;
	std::vector<Variable>	_variables;
	std::vector<Constant>	_constants;


	/*
	 * iSat3 Stuff
	 */
	bool _isatReady;

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
	struct isat3_node* exactlyOneTransition();
	struct isat3_node* asMostOneTransition();
	struct isat3_node* continuousStateComponents();
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
	struct isat3_node* jumps();
	struct isat3_node* flows();
	string printIntervalOfVariableISat(
			string variableName, unsigned int tframe);
	string printTruthValueOfVariable(string variableName, unsigned int tframe);
	void modifiedFraenzle();
	void setUpLocationVariables();
	void setUpEdgeVariables();
};

#endif /* LINEARHYBRIDAUTOMATON_H_ */
