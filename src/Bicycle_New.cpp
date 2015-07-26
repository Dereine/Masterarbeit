//============================================================================
// Name        : Bicycle_New.cpp
// Author      : Johannes Scherle
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "Location.h"
#include "LinearHybridAutomaton.h"
using namespace std;

Location test;

int main() {

	Constant zero("zero", 0.0);
	Constant five("five", 5.0);
	Constant one("one", 1.0);
	Constant ten("ten", 10.0);
	Constant twenty("twenty", 20.0);
	Constant fifty("fifty", 50);
	Variable x(Variable::REAL, "x", 0, 10, 0);
	Variable y(Variable::REAL, "y", 0, 10, 0);
	//Inequality ieq1 (five, Inequality:: LEQ, x);
//	std::vector<Inequality> lineq;
//	lineq.push_back(ieq1);

	LinearTerm linTerm1(five, x), linTerm2(one, y);
	std::vector<LinearTerm> linearTerms1, linearTerms2;
	linearTerms1.push_back(linTerm1);
	linearTerms2.push_back(linTerm2);

	LinearPredicate linPred1(linearTerms1, LinearPredicate::EQUAL, ten);
	LinearPredicate linPred2(linearTerms2, LinearPredicate::EQUAL, twenty);

	vector<LinearPredicate> linPreds1;
	linPreds1.push_back(linPred1);

	Invariant inv1(linPreds1);
	Constant upperBound("upperBound", 10), lowerBound("lowerBound", 3);

	Bound up1(one, x, Bound::LOW), low1(five, x, Bound::UP),
			up2(one, x, Bound::LOW), low2(five, x, Bound::UP);

	std::vector<Bound> loc1, loc2;
	loc1.push_back(up1); loc1.push_back(low1);
	loc2.push_back(up2); loc2.push_back(low2);
	Location test1(1, "test1", inv1, loc1, true);
	Location test2(2, "test2", inv1, loc2, false);
	vector<Location> locations;
	locations.push_back(test1);
	locations.push_back(test2);
	Edge testEdge1(test1, test2, linPred1, linPred1, "blubb"),
			testEdge2(test1, test2, linPred1, linPred1, "blabb");
	vector<Edge> edges;
	edges.push_back(testEdge1); edges.push_back(testEdge2);
	LinearHybridAutomaton blubb;
	blubb.setLocations(locations);
	blubb.setEdges(edges);
	blubb.addVariable(x);
	blubb.addVariable(y);
	blubb.addConstant(five);
	blubb.addConstant(one);
	blubb.addConstant(zero);
	blubb.addConstant(twenty);
	blubb.addConstant(fifty);
	blubb.addConstant(upperBound);
	blubb.addConstant(lowerBound);

	LinearTerm initTerm(one, x);
	vector<LinearTerm> initPred;
	initPred.push_back(initTerm);
	LinearPredicate initPreds(initPred, LinearPredicate::EQUAL, zero);

	LinearTerm targetTerm(one, x);
	vector<LinearTerm> targetPred;
	targetPred.push_back(targetTerm);
	LinearPredicate targetPreds(targetPred, LinearPredicate::GEQ, fifty);

	blubb.setUpIsat3();
	blubb.setUpVariables();
	//blubb.setUpConstants();
	blubb.toIsat3BMC();
	blubb.setUpInitial();
	blubb.setTarget(targetPreds);

	cout << test.getId();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
