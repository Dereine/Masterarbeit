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
	Constant timeDelta("deltaT", DELTANR);

	Constant one("one", 1.0f);
	bicycle.addConstant(one);

	Constant zero("zero", 0.0f);
	bicycle.addConstant(zero);

	Constant twenty("twenty", 20.0f);
	bicycle.addConstant(twenty);

	Constant ten("twenty", 10.0f);
	bicycle.addConstant(ten);

	Constant targetPlus("targetPlus", TARGET + DEVIATION);
	bicycle.addConstant(targetPlus);

	Constant targetMinus("targetMinus", TARGET - DEVIATION);
	bicycle.addConstant(targetMinus);

	Constant targetPlusGuard("targetPlusGuard", TARGET + (DEVIATION / 2));
	//Constant targetPlusGuard("targetPlusGuard", TARGET + DEVIATION);
	bicycle.addConstant(targetPlusGuard);

	Constant targetMinusGuard("targetMinusGuard", TARGET - (DEVIATION / 2));
	//Constant targetMinusGuard("targetMinusGuard", TARGET - DEVIATION);
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
	Constant ratioOneFour("ratio_one_four", (-1 * TEETHR4 / TEETHF1));
	bicycle.addConstant(ratioOneFour);
	Constant ratioOneFive("ratio_one_five", (-1 * TEETHR5 / TEETHF1));
	bicycle.addConstant(ratioOneFive);
	Constant ratioOneSix("ratio_one_six", (-1 * TEETHR6 / TEETHF1));
	bicycle.addConstant(ratioOneSix);
	Constant ratioOneSeven("ratio_one_seven", (-1 * TEETHR7 / TEETHF1));
	bicycle.addConstant(ratioOneSeven);
	Constant ratioOneEight("ratio_one_eight", (-1 * TEETHR8 / TEETHF1));
	bicycle.addConstant(ratioOneEight);
	Constant ratioTwoFour("ratio_two_four", (-1 * TEETHR4 / TEETHF2));
	bicycle.addConstant(ratioTwoFour);
	Constant ratioTwoFive("ratio_two_five", (-1 * TEETHR5 / TEETHF2));
	bicycle.addConstant(ratioTwoFive);
	Constant ratioTwoSix("ratio_two_six", (-1 * TEETHR6 / TEETHF2));
	bicycle.addConstant(ratioTwoSix);
	Constant ratioTwoSeven("ratio_two_seven", (-1 * TEETHR7 / TEETHF2));
	bicycle.addConstant(ratioTwoSeven);
	Constant ratioTwoEight("ratio_two_eight", (-1 * TEETHR8 / TEETHF2));
	bicycle.addConstant(ratioTwoEight);
	Constant ratioTwoNine("ratio_two_nine", (-1 * TEETHR9 / TEETHF2));
	bicycle.addConstant(ratioTwoNine);
	Constant ratioTwoTen("ratio_two_ten", (-1 * TEETHR10 / TEETHF2));
	bicycle.addConstant(ratioTwoTen);

	/*
	 * ************************************************************************
	 * Variables
	 * ************************************************************************
	 */
	Variable omegaWheel(Variable::REAL, "omegaWheel", 0, 150, 0.0, true);
	bicycle.addVariable(omegaWheel);

	Variable omegaCrank(Variable::REAL, "omegaCrank", 0, 150, 0.0, true);
	bicycle.addVariable(omegaCrank);

	Variable t(Variable::REAL, "t", 0, 500, 0.0, true);
	bicycle.addVariable(t);

	Variable bigJump(Variable::INTEGER, "bigJump", 0, 1, 0, false);
	bicycle.addVariable(bigJump);

	/*
	 * ************************************************************************
	 * Linear Predicates
	 * For Invariants, Guards, Assignments
	 * ************************************************************************
	 */
	LinearTerm oneTimesT(one, t);


	//LinearTerm oneTimesOmegaWheel(one, omegaWheel);
	LinearTerm oneTimesOmegaCrank(one, omegaCrank);

	vector<LinearTerm> linTermsInv;
	//linTermsInv.push_back(oneTimesOmegaWheel);
	linTermsInv.push_back(oneTimesOmegaCrank);

	LinearTerm oneTimesBigJump(one, bigJump);
	vector<LinearTerm> linTermsBigJump;
	linTermsBigJump.push_back(oneTimesBigJump);

	// omegaWheel <= target + deviation
	LinearPredicate upLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetPlus);
	// omegaWheel >= target - deviation
	LinearPredicate downLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetMinus);

	// omegaWheel >= target + deviation / 2
	LinearPredicate upLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetPlusGuard);
	// omegaWheel >= target + deviation
	LinearPredicate upLinPredGeq2(linTermsInv, LinearPredicate::GEQ, targetPlus);

	// omegaWheel <= target - deviation / 2
	LinearPredicate downLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetMinusGuard);

	vector<LinearTerm> linTermsAssign;
	linTermsAssign.push_back(oneTimesBigJump);
	LinearPredicate bigJumpEqualsOne(linTermsAssign, LinearPredicate::EQUAL,
			one);
	LinearPredicate bigJumpEqualsZero(linTermsAssign, LinearPredicate::EQUAL,
			zero);

	vector<LinearTerm> linTermsTarget;
	//linTermsTarget.push_back(oneTimesOmegaWheel);
	linTermsTarget.push_back(oneTimesOmegaCrank);
	LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, twenty);
	//LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, ten);

	LinearTerm ratio11TimesOmegaWheel(ratioOneOne, omegaWheel),
			ratio12TimesOmegaWheel(ratioOneTwo, omegaWheel),
			ratio13TimesOmegaWheel(ratioOneThree, omegaWheel),
			ratio14TimesOmegaWheel(ratioOneFour, omegaWheel),
			ratio15TimesOmegaWheel(ratioOneFive, omegaWheel),
			ratio16TimesOmegaWheel(ratioOneSix, omegaWheel),
			ratio17TimesOmegaWheel(ratioOneSeven, omegaWheel),
			ratio18TimesOmegaWheel(ratioOneEight, omegaWheel),
			ratio24TimesOmegaWheel(ratioTwoFour, omegaWheel),
			ratio25TimesOmegaWheel(ratioTwoFive, omegaWheel),
			ratio26TimesOmegaWheel(ratioTwoSix, omegaWheel),
			ratio27TimesOmegaWheel(ratioTwoSeven, omegaWheel),
			ratio28TimesOmegaWheel(ratioTwoEight, omegaWheel),
			ratio29TimesOmegaWheel(ratioTwoNine, omegaWheel),
			ratio210TimesOmegaWheel(ratioTwoTen, omegaWheel);


	vector<LinearTerm> linTermsRatio11;
	linTermsRatio11.push_back(oneTimesOmegaCrank);
	linTermsRatio11.push_back(ratio11TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio12;
	linTermsRatio12.push_back(oneTimesOmegaCrank);
	linTermsRatio12.push_back(ratio12TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio13;
	linTermsRatio13.push_back(oneTimesOmegaCrank);
	linTermsRatio13.push_back(ratio13TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio14;
	linTermsRatio14.push_back(oneTimesOmegaCrank);
	linTermsRatio14.push_back(ratio14TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio15;
	linTermsRatio15.push_back(oneTimesOmegaCrank);
	linTermsRatio15.push_back(ratio15TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio16;
	linTermsRatio16.push_back(oneTimesOmegaCrank);
	linTermsRatio16.push_back(ratio16TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio17;
	linTermsRatio17.push_back(oneTimesOmegaCrank);
	linTermsRatio17.push_back(ratio17TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio18;
	linTermsRatio18.push_back(oneTimesOmegaCrank);
	linTermsRatio18.push_back(ratio18TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio24;
	linTermsRatio24.push_back(oneTimesOmegaCrank);
	linTermsRatio24.push_back(ratio24TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio25;
	linTermsRatio25.push_back(oneTimesOmegaCrank);
	linTermsRatio25.push_back(ratio25TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio26;
	linTermsRatio26.push_back(oneTimesOmegaCrank);
	linTermsRatio26.push_back(ratio26TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio27;
	linTermsRatio27.push_back(oneTimesOmegaCrank);
	linTermsRatio27.push_back(ratio27TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio28;
	linTermsRatio28.push_back(oneTimesOmegaCrank);
	linTermsRatio28.push_back(ratio28TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio29;
	linTermsRatio29.push_back(oneTimesOmegaCrank);
	linTermsRatio29.push_back(ratio29TimesOmegaWheel);

	vector<LinearTerm> linTermsRatio210;
	linTermsRatio210.push_back(oneTimesOmegaCrank);
	linTermsRatio210.push_back(ratio210TimesOmegaWheel);


	LinearPredicate linPredRatio11(linTermsRatio11, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio12(linTermsRatio12, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio13(linTermsRatio13, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio14(linTermsRatio14, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio15(linTermsRatio15, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio16(linTermsRatio16, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio17(linTermsRatio17, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio18(linTermsRatio18, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio24(linTermsRatio24, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio25(linTermsRatio25, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio26(linTermsRatio26, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio27(linTermsRatio27, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio28(linTermsRatio28, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio29(linTermsRatio29, LinearPredicate::EQUAL, zero);
	LinearPredicate linPredRatio210(linTermsRatio210, LinearPredicate::EQUAL, zero);


	/*
	 * ************************************************************************
	 * Invariants
	 * ************************************************************************
	 */
	Invariant invUpAndDown11;
	invUpAndDown11.addLinPred(upLinPredLeq);

	Invariant invUpAndDown12;
	invUpAndDown12.addLinPred(upLinPredLeq);
	invUpAndDown12.addLinPred(downLinPredGeq);
	invUpAndDown12.addLinPred(linPredRatio12);

	Invariant invUpAndDown13;
	invUpAndDown13.addLinPred(upLinPredLeq);
	invUpAndDown13.addLinPred(downLinPredGeq);
	invUpAndDown13.addLinPred(linPredRatio13);

	Invariant invUpAndDown14;
	invUpAndDown14.addLinPred(upLinPredLeq);
	invUpAndDown14.addLinPred(downLinPredGeq);
	invUpAndDown14.addLinPred(linPredRatio14);

	Invariant invUpAndDown15;
	invUpAndDown15.addLinPred(upLinPredLeq);
	invUpAndDown15.addLinPred(downLinPredGeq);
	invUpAndDown15.addLinPred(linPredRatio15);

	Invariant invUpAndDown16;
	invUpAndDown16.addLinPred(upLinPredLeq);
	invUpAndDown16.addLinPred(downLinPredGeq);
	invUpAndDown16.addLinPred(linPredRatio16);

	Invariant invUpAndDown17;
	invUpAndDown17.addLinPred(upLinPredLeq);
	invUpAndDown17.addLinPred(downLinPredGeq);
	invUpAndDown17.addLinPred(linPredRatio17);

	Invariant invUpAndDown18;
	invUpAndDown18.addLinPred(upLinPredLeq);
	invUpAndDown18.addLinPred(downLinPredGeq);
	invUpAndDown18.addLinPred(linPredRatio18);

	Invariant invBigJump17;
	invBigJump17.addLinPred(linPredRatio17);

	Invariant invBigJump16;
	invBigJump16.addLinPred(linPredRatio16);

	Invariant invBigJump15;
	invBigJump15.addLinPred(linPredRatio15);

	Invariant invBigJump14;
	invBigJump14.addLinPred(linPredRatio14);

	Invariant invUpAndDown24;
	invUpAndDown24.addLinPred(upLinPredLeq);
	invUpAndDown24.addLinPred(downLinPredGeq);
	invUpAndDown24.addLinPred(linPredRatio24);

	Invariant invUpAndDown25;
	invUpAndDown25.addLinPred(upLinPredLeq);
	invUpAndDown25.addLinPred(downLinPredGeq);
	invUpAndDown25.addLinPred(linPredRatio25);

	Invariant invUpAndDown26;
	invUpAndDown26.addLinPred(upLinPredLeq);
	invUpAndDown26.addLinPred(downLinPredGeq);
	invUpAndDown26.addLinPred(linPredRatio26);

	Invariant invUpAndDown27;
	invUpAndDown27.addLinPred(upLinPredLeq);
	invUpAndDown27.addLinPred(downLinPredGeq);
	invUpAndDown27.addLinPred(linPredRatio27);

	Invariant invUpAndDown28;
	invUpAndDown28.addLinPred(upLinPredLeq);
	invUpAndDown28.addLinPred(downLinPredGeq);
	invUpAndDown28.addLinPred(linPredRatio28);

	Invariant invUpAndDown29;
	invUpAndDown29.addLinPred(upLinPredLeq);
	invUpAndDown29.addLinPred(downLinPredGeq);
	invUpAndDown29.addLinPred(linPredRatio29);

	Invariant invUpAndDown210;
	invUpAndDown210.addLinPred(upLinPredLeq);
	invUpAndDown210.addLinPred(downLinPredGeq);
	invUpAndDown210.addLinPred(linPredRatio210);


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
	//upNoBigJump.addLinPred(bigJumpEqualsZero);
	upNoBigJump.addLinPred(upLinPredGeq);

	Guard upNoBigJump2;
	//upNoBigJump2.addLinPred(bigJumpEqualsZero);
	upNoBigJump2.addLinPred(upLinPredGeq2);

	Guard upBigJump;
	upBigJump.addLinPred(upLinPredGeq);
	//upBigJump.addLinPred(bigJumpEqualsOne);

	Guard downNoBigJump;
	//downNoBigJump.addLinPred(bigJumpEqualsZero);
	downNoBigJump.addLinPred(downLinPredLeq);

	Guard downBigJump;
	downBigJump.addLinPred(downLinPredGeq);
	//downBigJump.addLinPred(bigJumpEqualsOne);

	Guard emptyGuard;

	/*
	 * ************************************************************************
	 * Assignments
	 * ************************************************************************
	 */
	Assignment bigJumpOne;
	bigJumpOne.addLinPred(bigJumpEqualsOne);

	Assignment bigJumpZero;
	bigJumpZero.addLinPred(bigJumpEqualsZero);

	Assignment emptyAssignment;

	/*
	 * ************************************************************************
	 * Bounds
	 * ************************************************************************
	 */
	Bound timeBound(t, timeDelta, timeDelta);

	Bound l11Omega(omegaWheel, cDrive11, cDrive11);
	Bound l12Omega(omegaWheel, cDrive12, cDrive12);
	Bound l13Omega(omegaWheel, cDrive13, cDrive13);
	Bound l14Omega(omegaWheel, cDrive14, cDrive14);
	Bound l15Omega(omegaWheel, cDrive15, cDrive15);
	Bound l16Omega(omegaWheel, cDrive16, cDrive16);
	Bound l17Omega(omegaWheel, cDrive17, cDrive17);
	Bound l18Omega(omegaWheel, cDrive18, cDrive18);
	Bound l24Omega(omegaWheel, cDrive24, cDrive24);
	Bound l25Omega(omegaWheel, cDrive25, cDrive25);
	Bound l26Omega(omegaWheel, cDrive26, cDrive26);
	Bound l27Omega(omegaWheel, cDrive27, cDrive27);
	Bound l28Omega(omegaWheel, cDrive28, cDrive28);
	Bound l29Omega(omegaWheel, cDrive29, cDrive29);
	Bound l210Omega(omegaWheel, cDrive210, cDrive210);

	Bound bigJumpBound(bigJump, zero, zero);

	Bound bigJumpBoundsOmega(omegaWheel, zero, zero);
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
	l11Bounds.push_back(timeBound);
	//l11Bounds.push_back(bigJumpBound);
	Location loc11(11, "one_one", invUp11, l11Bounds, true);
	bicycle.addLocation(loc11);

	vector <Bound> l12Bounds;
	l12Bounds.push_back(l12Omega);
	l12Bounds.push_back(timeBound);
	//l12Bounds.push_back(bigJumpBound);
	Location loc12(12, "one_two", invUpAndDown12, l12Bounds, false);
	//Location loc12(12, "one_two", invUp11, l11Bounds, false);
	bicycle.addLocation(loc12);


	vector <Bound> l13Bounds;
	l13Bounds.push_back(l13Omega);
	//l13Bounds.push_back(bigJumpBound);
	Location loc13(13, "one_three", invUpAndDown13, l13Bounds, false);
	bicycle.addLocation(loc13);


	vector <Bound> l14Bounds;
	l14Bounds.push_back(l14Omega);
	//l14Bounds.push_back(bigJumpBound);
	Location loc14(14, "one_four", invUpAndDown14, l14Bounds, false);
	bicycle.addLocation(loc14);

	vector <Bound> l15Bounds;
	l15Bounds.push_back(l15Omega);
	//l15Bounds.push_back(bigJumpBound);
	Location loc15(15, "one_five", invUpAndDown15, l15Bounds, false);
	bicycle.addLocation(loc15);

	vector <Bound> l16Bounds;
	l16Bounds.push_back(l16Omega);
	//l16Bounds.push_back(bigJumpBound);
	Location loc16(16, "one_six", invUpAndDown16, l16Bounds, false);
	bicycle.addLocation(loc16);

	vector <Bound> l17Bounds;
	l17Bounds.push_back(l17Omega);
	//l17Bounds.push_back(bigJumpBound);
	Location loc17(17, "one_seven", invUpAndDown17, l17Bounds, false);
	bicycle.addLocation(loc17);

	vector <Bound> l18Bounds;
	l18Bounds.push_back(l18Omega);
	//l18Bounds.push_back(bigJumpBound);
	Location loc18(18, "one_eight", invUpAndDown18, l18Bounds, false);
	bicycle.addLocation(loc18);

	vector <Bound> l17BoundsBigJump;
	l17BoundsBigJump.push_back(bigJumpBoundsOmega);
	//l17Bounds.push_back(bigJumpBound);
	Location loc17BigJump(172, "one_seven_big",
			invBigJump17, l17BoundsBigJump, false);
	bicycle.addLocation(loc17BigJump);

	vector <Bound> l16BoundsBigJump;
	l16BoundsBigJump.push_back(bigJumpBoundsOmega);
	//l17Bounds.push_back(bigJumpBound);
	Location loc16BigJump(162, "one_six_big",
			invBigJump16, l16BoundsBigJump, false);
	bicycle.addLocation(loc16BigJump);

	vector <Bound> l15BoundsBigJump;
	l15BoundsBigJump.push_back(bigJumpBoundsOmega);
	//l17Bounds.push_back(bigJumpBound);
	Location loc15BigJump(152, "one_five_big",
			invBigJump15, l15BoundsBigJump, false);
	bicycle.addLocation(loc15BigJump);

	vector <Bound> l14BoundsBigJump;
	l14BoundsBigJump.push_back(bigJumpBoundsOmega);
	//l17Bounds.push_back(bigJumpBound);
	Location loc14BigJump(142, "one_four_big",
			invBigJump14, l14BoundsBigJump, false);
	bicycle.addLocation(loc14BigJump);

	vector <Bound> l24Bounds;
	l24Bounds.push_back(l24Omega);
	l24Bounds.push_back(bigJumpBound);
	Location loc24(24, "two_four", invUpAndDown24, l24Bounds, false);
	bicycle.addLocation(loc24);

	vector <Bound> l25Bounds;
	l25Bounds.push_back(l25Omega);
	l25Bounds.push_back(bigJumpBound);
	Location loc25(25, "two_five", invUpAndDown25, l25Bounds, false);
	bicycle.addLocation(loc25);

	vector <Bound> l26Bounds;
	l26Bounds.push_back(l26Omega);
	l26Bounds.push_back(bigJumpBound);
	Location loc26(26, "two_six", invUpAndDown26, l26Bounds, false);
	bicycle.addLocation(loc26);

	vector <Bound> l27Bounds;
	l27Bounds.push_back(l27Omega);
	l27Bounds.push_back(bigJumpBound);
	Location loc27(27, "two_seven", invUpAndDown27, l27Bounds, false);
	bicycle.addLocation(loc27);

	vector <Bound> l28Bounds;
	l28Bounds.push_back(l28Omega);
	l28Bounds.push_back(bigJumpBound);
	Location loc28(28, "two_eight", invUpAndDown28, l28Bounds, false);
	bicycle.addLocation(loc28);

	vector <Bound> l29Bounds;
	l29Bounds.push_back(l29Omega);
	l29Bounds.push_back(bigJumpBound);
	Location loc29(29, "two_nine", invUpAndDown29, l29Bounds, false);
	bicycle.addLocation(loc29);

	vector <Bound> l210Bounds;
	l210Bounds.push_back(l210Omega);
	l210Bounds.push_back(bigJumpBound);
	Location loc210(210, "two_ten", invUpAndDown210, l210Bounds, false);
	bicycle.addLocation(loc210);

	/*
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

	Edge one_two_one_one(loc12, loc11, downNoBigJump, bigJumpZero,
			"one_two_one_one");
	bicycle.addEdge(one_two_one_one);

	Edge one_three_one_four(loc13, loc14, upNoBigJump, bigJumpZero,
			"one_three_one_four");
	bicycle.addEdge(one_three_one_four);
	Edge one_three_one_two(loc13, loc12, downNoBigJump, bigJumpZero,
			"one_three_one_two");
	//bicycle.addEdge(one_three_one_two);

	Edge one_four_one_five(loc14, loc15, upNoBigJump, bigJumpZero,
			"one_four_one_five");
	bicycle.addEdge(one_four_one_five);
	Edge one_four_one_three(loc14, loc13, downNoBigJump, bigJumpZero,
			"one_four_one_three");
	//bicycle.addEdge(one_four_one_three);

	Edge one_five_one_six(loc15, loc16, upNoBigJump, bigJumpZero,
			"one_five_one_six");
	bicycle.addEdge(one_five_one_six);
	Edge one_five_one_four(loc15, loc14, downNoBigJump, bigJumpZero,
			"one_five_one_four");
	//bicycle.addEdge(one_five_one_four);

	Edge one_six_one_seven(loc16, loc17, upNoBigJump, bigJumpZero,
			"one_six_one_seven");
	bicycle.addEdge(one_six_one_seven);
	Edge one_six_one_five(loc16, loc15, downNoBigJump, bigJumpZero,
			"one_six_one_five");
	//bicycle.addEdge(one_six_one_five);

	Edge one_seven_one_eight(loc17, loc18, upNoBigJump, bigJumpZero,
			"one_seven_one_eight");
	bicycle.addEdge(one_seven_one_eight);
	Edge one_seven_one_six(loc17, loc16, downNoBigJump, bigJumpZero,
			"one_seven_one_six");
	//bicycle.addEdge(one_seven_one_six);

	Edge one_eight_one_seven_big(loc18, loc17BigJump, emptyGuard, emptyAssignment,
			"one_eight_one_seven_big");
	bicycle.addEdge(one_eight_one_seven_big);
//	Edge one_eight_one_seven(loc18, loc17, downNoBigJump, bigJumpZero,
//			"one_eight_one_seven");
	//bicycle.addEdge(one_eight_one_seven);

	Edge one_seven_one_six_big(loc17BigJump, loc16BigJump, emptyGuard, emptyAssignment,
				"one_seven_one_six_big");
	bicycle.addEdge(one_seven_one_six_big);

	Edge one_six_one_five_big(loc16BigJump, loc15BigJump, emptyGuard, emptyAssignment,
				"one_six_one_five_big");
	bicycle.addEdge(one_six_one_five_big);

	Edge one_five_one_four_big(loc15BigJump, loc14BigJump, emptyGuard, emptyAssignment,
				"one_five_one_four_big");
	bicycle.addEdge(one_five_one_four_big);

	Edge one_four_two_four_big(loc14BigJump, loc24, emptyGuard, emptyAssignment,
				"one_four_two_four_big");
	bicycle.addEdge(one_four_two_four_big);

	Edge two_four_two_five(loc24, loc25, upNoBigJump, bigJumpZero,
			"two_four_two_five");
	bicycle.addEdge(two_four_two_five);

	Edge two_five_two_six(loc25, loc26, upNoBigJump, bigJumpZero,
			"two_five_two_six");
	bicycle.addEdge(two_five_two_six);

	Edge two_six_two_seven(loc26, loc27, upNoBigJump, bigJumpZero,
			"two_six_two_seven");
	bicycle.addEdge(two_six_two_seven);

	Edge two_seven_two_eight(loc27, loc28, upNoBigJump, bigJumpZero,
			"two_seven_two_eight");
	bicycle.addEdge(two_seven_two_eight);

	Edge two_eight_two_nine(loc28, loc29, upNoBigJump, bigJumpZero,
			"two_eight_two_nine");
	bicycle.addEdge(two_eight_two_nine);

	Edge two_nine_two_ten(loc29, loc210, upNoBigJump, bigJumpZero,
			"two_nine_two_ten");
	bicycle.addEdge(two_nine_two_ten);

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
