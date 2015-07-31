//============================================================================
// Name        : Bicycle_New.cpp
// Author      : Johannes Scherle
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include "Bicycle.h"
#include "Location.h"
#include "LinearHybridAutomaton.h"
using namespace std;

//void test() {
//	Constant zero("zero", 0.0);
//	Constant five("five", 5.0);
//	Constant one("one", 1.0);
//	Constant ten("ten", 10.0);
//	Constant twenty("twenty", 20.0);
//	Constant fifty("fifty", 50);
//	Variable x(Variable::REAL, "x", 0, 10, 0);
//	Variable y(Variable::REAL, "y", 0, 10, 0);
//	Inequality ieq1 (five, Inequality:: LEQ, x);
//		std::vector<Inequality> lineq;
//		lineq.push_back(ieq1);
//	LinearTerm linTerm1(five, x), linTerm2(one, y);
//	std::vector<LinearTerm> linearTerms1, linearTerms2;
//	linearTerms1.push_back(linTerm1);
//	linearTerms2.push_back(linTerm2);
//	LinearPredicate linPred1(linearTerms1, LinearPredicate::EQUAL, ten);
//	LinearPredicate linPred2(linearTerms2, LinearPredicate::EQUAL, twenty);
//	vector<LinearPredicate> linPreds1;
//	linPreds1.push_back(linPred1);
//	Invariant inv1(linPreds1);
//	Constant upperBound("upperBound", 10), lowerBound("lowerBound", 3);
//	Bound up1(one, x, Bound::LOW), low1(five, x, Bound::UP), up2(one, x,
//			Bound::LOW), low2(five, x, Bound::UP);
//	std::vector<Bound> loc1, loc2;
//	loc1.push_back(up1);
//	loc1.push_back(low1);
//	loc2.push_back(up2);
//	loc2.push_back(low2);
//	Location test1(1, "test1", inv1, loc1, true);
//	Location test2(2, "test2", inv1, loc2, false);
//	vector<Location> locations;
//	locations.push_back(test1);
//	locations.push_back(test2);
//	Edge testEdge1(test1, test2, linPred1, linPred1, "blubb"), testEdge2(test1,
//			test2, linPred1, linPred1, "blabb");
//	vector<Edge> edges;
//	edges.push_back(testEdge1);
//	edges.push_back(testEdge2);
//	LinearHybridAutomaton blubb;
//	blubb.setLocations(locations);
//	blubb.setEdges(edges);
//	blubb.addVariable(x);
//	blubb.addVariable(y);
//	blubb.addConstant(five);
//	blubb.addConstant(one);
//	blubb.addConstant(zero);
//	blubb.addConstant(twenty);
//	blubb.addConstant(fifty);
//	blubb.addConstant(upperBound);
//	blubb.addConstant(lowerBound);
//	LinearTerm initTerm(one, x);
//	vector<LinearTerm> initPred;
//	initPred.push_back(initTerm);
//	LinearPredicate initPreds(initPred, LinearPredicate::EQUAL, zero);
//	LinearTerm targetTerm(one, x);
//	vector<LinearTerm> targetPred;
//	targetPred.push_back(targetTerm);
//	LinearPredicate targetPreds(targetPred, LinearPredicate::GEQ, fifty);
//	blubb.setUpIsat3();
//	blubb.setUpVariables();
//	blubb.setUpConstants();
//	blubb.toIsat3BMC();
//	blubb.setUpInitial();
//	blubb.setTarget(targetPreds);
//}

int main() {
	LinearHybridAutomaton bicycle;

	/*
	 * ************************************************************************
	 * Constants
	 * ************************************************************************
	 */
	Constant one("one", 1.0f);
	bicycle.addConstant(one);

	Constant zero("zero", 0.0f);
	bicycle.addConstant(zero);

	Constant twenty("twenty", 20.0f);
	bicycle.addConstant(twenty);

	Constant targetPlus("targetPlus", TARGET + DEVIATION);
	bicycle.addConstant(targetPlus);

	Constant targetMinus("targetMinus", TARGET - DEVIATION);
	bicycle.addConstant(targetMinus);

	Constant targetPlusGuard("targetPlusGuard", TARGET + (DEVIATION / 2));
	bicycle.addConstant(targetPlusGuard);

	Constant targetMinusGuard("targetMinusGuard", TARGET - (DEVIATION / 2));
	bicycle.addConstant(targetMinusGuard);

	Constant cFric("cFric", (-1 * MU * G) / RWHEEL);
	bicycle.addConstant(cFric);

	Constant cDrive11("cDrive11", C1 * (TEETHR1 / TEETHF1) - C2);
	bicycle.addConstant(cDrive11);
	Constant cDrive12("cDrive12", C1 * (TEETHR2 / TEETHF1) - C2);
	bicycle.addConstant(cDrive12);
	Constant cDrive13("cDrive13", C1 * (TEETHR3 / TEETHF1) - C2);
	bicycle.addConstant(cDrive13);
	Constant cDrive14("cDrive14", C1 * (TEETHR4 / TEETHF1) - C2);
	bicycle.addConstant(cDrive14);
	Constant cDrive15("cDrive15", C1 * (TEETHR5 / TEETHF1) - C2);
	bicycle.addConstant(cDrive15);
	Constant cDrive16("cDrive16", C1 * (TEETHR6 / TEETHF1) - C2);
	bicycle.addConstant(cDrive16);
	Constant cDrive17("cDrive17", C1 * (TEETHR7 / TEETHF1) - C2);
	bicycle.addConstant(cDrive17);
	Constant cDrive18("cDrive18", C1 * (TEETHR8 / TEETHF1) - C2);
	bicycle.addConstant(cDrive18);
	Constant cDrive24("cDrive24", C1 * (TEETHR4 / TEETHF2) - C2);
	bicycle.addConstant(cDrive24);
	Constant cDrive25("cDrive25", C1 * (TEETHR5 / TEETHF2) - C2);
	bicycle.addConstant(cDrive25);
	Constant cDrive26("cDrive26", C1 * (TEETHR6 / TEETHF2) - C2);
	bicycle.addConstant(cDrive26);
	Constant cDrive27("cDrive27", C1 * (TEETHR7 / TEETHF2) - C2);
	bicycle.addConstant(cDrive27);
	Constant cDrive28("cDrive28", C1 * (TEETHR8 / TEETHF2) - C2);
	bicycle.addConstant(cDrive28);
	Constant cDrive29("cDrive29", C1 * (TEETHR9 / TEETHF2) - C2);
	bicycle.addConstant(cDrive29);
	Constant cDrive210("cDrive210", C1 * (TEETHR10 / TEETHF2) - C2);
	bicycle.addConstant(cDrive210);


	Constant ratioOneOne("ratio_one_one", (-1 * TEETHR1 / TEETHF1));
	bicycle.addConstant(ratioOneOne);
	Constant ratioOneTwo("ratio_one_two", (-1 * TEETHR2 / TEETHF1));
	bicycle.addConstant(ratioOneTwo);
	Constant ratioOneThree("ratio_one_three", (-1 * TEETHR3 / TEETHF1));
	bicycle.addConstant(ratioOneThree);

	/*
	 * ************************************************************************
	 * Variables
	 * ************************************************************************
	 */
	Variable omegaWheel(Variable::REAL, "omegaWheel", 0, 150, 0.0, true);
	bicycle.addVariable(omegaWheel);

	Variable omegaCrank(Variable::REAL, "omegaCrank", 0, 150, 0.0, true);
	bicycle.addVariable(omegaCrank);

	Variable bigJump(Variable::INTEGER, "bigJump", 0, 1, 0, false);
	bicycle.addVariable(bigJump);

	/*
	 * ************************************************************************
	 * Linear Predicates
	 * For Invariants, Guards, Assignments
	 * ************************************************************************
	 */
	LinearTerm oneTimesOmegaWheel(one, omegaWheel);
	vector<LinearTerm> linTermsInv;
	linTermsInv.push_back(oneTimesOmegaWheel);

	LinearTerm oneTimesBigJump(one, bigJump);
	vector<LinearTerm> linTermsBigJump;
	linTermsBigJump.push_back(oneTimesBigJump);

	// omegaWheel <= target + deviation
	LinearPredicate upLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetPlus);
	// omegaWheel >= target - deviation
	LinearPredicate downLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetMinus);
	// omegaWheel >= target + deviation
	LinearPredicate upLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetPlusGuard);
	// omegaWheel <= target - deviation
	LinearPredicate downLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetMinusGuard);

	vector<LinearTerm> linTermsAssign;
	linTermsAssign.push_back(oneTimesBigJump);
	LinearPredicate bigJumpEqualsOne(linTermsAssign, LinearPredicate::EQUAL,
			one);
	LinearPredicate bigJumpEqualsZero(linTermsAssign, LinearPredicate::EQUAL,
			zero);

	vector<LinearTerm> linTermsTarget;
	linTermsTarget.push_back(oneTimesOmegaWheel);
	LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, twenty);


	LinearTerm oneTimesOmegaCrank(one,omegaCrank);
	LinearTerm ratio11TimesOmegaWheel(ratioOneOne, omegaWheel),
			ratio12TimesOmegaWheel(ratioOneTwo, omegaWheel),
			ratio13TimesOmegaWheel(ratioOneThree, omegaWheel);

	vector<LinearTerm> linTermsRatio11;
	linTermsRatio11.push_back(oneTimesOmegaCrank);
	linTermsRatio11.push_back(ratio11TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio12;
	linTermsRatio12.push_back(oneTimesOmegaCrank);
	linTermsRatio12.push_back(ratio12TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio13;
	linTermsRatio13.push_back(oneTimesOmegaCrank);
	linTermsRatio13.push_back(ratio13TimesOmegaWheel);

	LinearPredicate linPredRatio11(linTermsRatio11, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio12(linTermsRatio12, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio13(linTermsRatio13, LinearPredicate::EQUAL, zero);


	/*
	 * ************************************************************************
	 * Invariants
	 * ************************************************************************
	 */
	Invariant invUpAndDown12;
	invUpAndDown12.addLinPred(upLinPredLeq);
	invUpAndDown12.addLinPred(downLinPredGeq);
	invUpAndDown12.addLinPred(linPredRatio12);

	Invariant invUpAndDown13;
	invUpAndDown13.addLinPred(upLinPredLeq);
	invUpAndDown13.addLinPred(downLinPredGeq);
	invUpAndDown13.addLinPred(linPredRatio13);

	Invariant invUp11;
	invUp11.addLinPred(upLinPredLeq);
	invUp11.addLinPred(linPredRatio11);

	Invariant invDown;
	invDown.addLinPred(downLinPredGeq);

	/*
	 * ************************************************************************
	 * Guards
	 * ************************************************************************
	 */
	Guard upNoBigJump;
	upNoBigJump.addLinPred(bigJumpEqualsZero);
	upNoBigJump.addLinPred(upLinPredGeq);

	Guard upBigJump;
	upBigJump.addLinPred(bigJumpEqualsOne);

	Guard downNoBigJump;
	downNoBigJump.addLinPred(bigJumpEqualsZero);
	downNoBigJump.addLinPred(downLinPredGeq);

	Guard downBigJump;
	downBigJump.addLinPred(bigJumpEqualsOne);

	/*
	 * ************************************************************************
	 * Assignments
	 * ************************************************************************
	 */
	Assignment bigJumpOne;
	bigJumpOne.addLinPred(bigJumpEqualsOne);

	Assignment bigJumpZero;
	bigJumpZero.addLinPred(bigJumpEqualsZero);

	/*
	 * ************************************************************************
	 * Bounds
	 * ************************************************************************
	 */
	Bound l11Omega(omegaWheel, cDrive11, cDrive11);
	Bound l11BigJump(bigJump, zero, zero);
//	Bound l11Up(cDrive11, omegaWheel, Bound::UP);
//	Bound l11Low(cDrive11, omegaWheel, Bound::LOW);
//	Bound l12Up(cDrive12, omegaWheel, Bound::UP);
//	Bound l12Low(cDrive12, omegaWheel, Bound::LOW);
//	Bound l13Up(cDrive13, omegaWheel, Bound::UP);
//	Bound l13Low(cDrive13, omegaWheel, Bound::LOW);
//	Bound l14Up(cDrive14, omegaWheel, Bound::UP);
//	Bound l14Low(cDrive14, omegaWheel, Bound::LOW);
//	Bound l15Up(cDrive15, omegaWheel, Bound::UP);
//	Bound l15Low(cDrive15, omegaWheel, Bound::LOW);
//	Bound l16Up(cDrive16, omegaWheel, Bound::UP);
//	Bound l16Low(cDrive16, omegaWheel, Bound::LOW);
//	Bound l17Up(cDrive17, omegaWheel, Bound::UP);
//	Bound l17Low(cDrive17, omegaWheel, Bound::LOW);
//	Bound l18Up(cDrive18, omegaWheel, Bound::UP);
//	Bound l18Low(cDrive18, omegaWheel, Bound::LOW);
//	Bound l24Up(cDrive24, omegaWheel, Bound::UP);
//	Bound l24Low(cDrive24, omegaWheel, Bound::LOW);
//	Bound l25Up(cDrive25, omegaWheel, Bound::UP);
//	Bound l25Low(cDrive25, omegaWheel, Bound::LOW);
//	Bound l26Up(cDrive26, omegaWheel, Bound::UP);
//	Bound l26Low(cDrive26, omegaWheel, Bound::LOW);
//	Bound l27Up(cDrive27, omegaWheel, Bound::UP);
//	Bound l27Low(cDrive27, omegaWheel, Bound::LOW);
//	Bound l28Up(cDrive28, omegaWheel, Bound::UP);
//	Bound l28Low(cDrive28, omegaWheel, Bound::LOW);
//	Bound l29Up(cDrive29, omegaWheel, Bound::UP);
//	Bound l29Low(cDrive29, omegaWheel, Bound::LOW);
//	Bound l210Up(cDrive210, omegaWheel, Bound::UP);
//	Bound l210Low(cDrive210, omegaWheel, Bound::LOW);

	/*
	 * ************************************************************************
	 * Locations
	 * ************************************************************************
	 */
	vector <Bound> l11Bounds;
	l11Bounds.push_back(l11Omega);
	l11Bounds.push_back(l11BigJump);
	Location loc11(11, "one_one", invUp11, l11Bounds, true);
	bicycle.addLocation(loc11);

//	vector <Bound> l12Bounds;
//	l12Bounds.push_back(l11Omega);
//	l12Bounds.push_back(l11BigJump);
	Location loc12(12, "one_two", invUpAndDown12, l11Bounds, false);
	bicycle.addLocation(loc12);


//	vector <Bound> l13Bounds;
//	l13Bounds.push_back(l13Up);
//	l13Bounds.push_back(l13Low);
	Location loc13(13, "one_three", invUpAndDown13, l11Bounds, false);
	bicycle.addLocation(loc13);

/*
	vector <Bound> l14Bounds;
	l14Bounds.push_back(l14Up);
	l14Bounds.push_back(l14Low);
	Location loc14(14, "one_four", invUpAndDown, l14Bounds, false);
	bicycle.addLocation(loc14);

	vector <Bound> l15Bounds;
	l15Bounds.push_back(l15Up);
	l15Bounds.push_back(l15Low);
	Location loc15(15, "one_five", invUpAndDown, l15Bounds, false);
	bicycle.addLocation(loc15);

	vector <Bound> l16Bounds;
	l16Bounds.push_back(l16Up);
	l16Bounds.push_back(l16Low);
	Location loc16(16, "one_six", invUpAndDown, l16Bounds, false);
	bicycle.addLocation(loc16);

	vector <Bound> l17Bounds;
	l17Bounds.push_back(l17Up);
	l17Bounds.push_back(l17Low);
	Location loc17(17, "one_seven", invUpAndDown, l17Bounds, false);
	bicycle.addLocation(loc17);

	vector <Bound> l18Bounds;
	l18Bounds.push_back(l18Up);
	l18Bounds.push_back(l18Low);
	Location loc18(18, "one_eight", invUpAndDown, l18Bounds, false);
	bicycle.addLocation(loc18);

	vector <Bound> l24Bounds;
	l24Bounds.push_back(l24Up);
	l24Bounds.push_back(l24Low);
	Location loc24(24, "two_four", invUpAndDown, l24Bounds, false);
	bicycle.addLocation(loc24);

	vector <Bound> l25Bounds;
	l25Bounds.push_back(l25Up);
	l25Bounds.push_back(l25Low);
	Location loc25(25, "two_five", invUpAndDown, l25Bounds, false);
	bicycle.addLocation(loc25);

	vector <Bound> l26Bounds;
	l26Bounds.push_back(l26Up);
	l26Bounds.push_back(l26Low);
	Location loc26(26, "two_six", invUpAndDown, l26Bounds, false);
	bicycle.addLocation(loc26);

	vector <Bound> l27Bounds;
	l27Bounds.push_back(l27Up);
	l27Bounds.push_back(l27Low);
	Location loc27(27, "two_seven", invUpAndDown, l27Bounds, false);
	bicycle.addLocation(loc27);

	vector <Bound> l28Bounds;
	l28Bounds.push_back(l28Up);
	l28Bounds.push_back(l28Low);
	Location loc28(28, "two_eight", invUpAndDown, l28Bounds, false);
	bicycle.addLocation(loc28);

	vector <Bound> l29Bounds;
	l29Bounds.push_back(l29Up);
	l29Bounds.push_back(l29Low);
	Location loc29(29, "two_nine", invUpAndDown, l29Bounds, false);
	bicycle.addLocation(loc29);

	vector <Bound> l210Bounds;
	l210Bounds.push_back(l210Up);
	l210Bounds.push_back(l210Low);
	Location loc210(210, "two_ten", invUp, l210Bounds, false);
	bicycle.addLocation(loc210);

	/*
	 * ************************************************************************
	 * Edges
	 * ************************************************************************
	 */
	Edge one_one_one_two(loc11, loc12, upNoBigJump, bigJumpZero,
			"one_one_one_two");
	bicycle.addEdge(one_one_one_two);

	Edge one_two_one_three(loc12, loc13, upNoBigJump, bigJumpZero,
			"one_two_one_three");
	bicycle.addEdge(one_two_one_three);
	/*
	Edge one_two_one_one(loc12, loc11, downNoBigJump, bigJumpZero,
			"one_two_one_one");
	bicycle.addEdge(one_two_one_one);

	Edge one_three_one_four(loc13, loc14, upNoBigJump, bigJumpZero,
			"one_three_one_four");
	bicycle.addEdge(one_three_one_four);
	Edge one_three_one_two(loc13, loc12, downNoBigJump, bigJumpZero,
			"one_three_one_two");
	bicycle.addEdge(one_three_one_two);

	Edge one_four_one_five(loc14, loc15, upNoBigJump, bigJumpZero,
			"one_four_one_five");
	bicycle.addEdge(one_four_one_five);
	Edge one_four_one_three(loc14, loc13, downNoBigJump, bigJumpZero,
			"one_four_one_three");
	bicycle.addEdge(one_four_one_three);

	Edge one_five_one_six(loc15, loc16, upNoBigJump, bigJumpZero,
			"one_five_one_six");
	bicycle.addEdge(one_five_one_six);
	Edge one_five_one_four(loc15, loc14, downNoBigJump, bigJumpZero,
			"one_five_one_four");
	bicycle.addEdge(one_five_one_four);

	Edge one_six_one_seven(loc16, loc17, upNoBigJump, bigJumpZero,
			"one_six_one_seven");
	bicycle.addEdge(one_six_one_seven);
	Edge one_six_one_five(loc16, loc15, downNoBigJump, bigJumpZero,
			"one_six_one_five");
	bicycle.addEdge(one_six_one_five);

	Edge one_seven_one_eight(loc17, loc18, upNoBigJump, bigJumpZero,
			"one_seven_one_eight");
	bicycle.addEdge(one_seven_one_eight);
	Edge one_seven_one_six(loc17, loc16, downNoBigJump, bigJumpZero,
			"one_seven_one_six");
	bicycle.addEdge(one_seven_one_six);

	Edge one_eight_one_seven_big(loc18, loc17, upNoBigJump, bigJumpOne,
			"one_eight_one_seven_big");
	bicycle.addEdge(one_eight_one_seven_big);
	Edge one_eight_one_seven(loc18, loc17, downNoBigJump, bigJumpZero,
			"one_eight_one_seven");
	bicycle.addEdge(one_eight_one_seven);

	Edge one_seven_one_six_big(loc17, loc16, downBigJump, bigJumpOne,
				"one_seven_one_six_big");
	bicycle.addEdge(one_seven_one_six_big);
	*/

	bicycle.setUpIsat3();
	bicycle.setUpVariables();
	//bicycle.setUpConstants();
	bicycle.toIsat3BMC();
	bicycle.setUpInitial();
	bicycle.setTarget(target);
	bicycle.solveBMCIsat();
	bicycle.printBMCResultIsat(TIMEFRAMES);


	//test();
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}