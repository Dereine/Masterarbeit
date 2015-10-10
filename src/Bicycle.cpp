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

#define USERINPUT

void scenarioSelection(ShiftSequence& shiftSequence,
		bool& modelTimeBehavior, double& fPedMin, double& fPedMax,
		double& targetRPM, double& deviation, double& error, string& target) {
	Scenario scenario1{FOUR_ONE, true, 35, 200, 100, 10, 30, "omegaCrank > upperLimit;"},
	scenario2{TWO_ONE_TWO, true, 150, 200, 100, 10, 30, "omegaCrank > upperLimit;"};


	std::vector<Scenario> Scenarios;
	Scenarios.push_back(scenario1);
	Scenarios.push_back(scenario2);

	int input;
	cout << "Please select a scenario" << endl;
	for (size_t i = 1; i < Scenarios.size() + 1; i++) {
		cout << i << ": " << endl;
		cout << "fPedMin: " << Scenarios[i - 1].fPedMin << endl;
		cout << "fPedMax: " << Scenarios[i - 1].fPedMax << endl;
		cout << "Target: " << Scenarios[i - 1].targetRPM << endl;
		cout << "Deviation: " << Scenarios[i - 1].deviation << endl;
		cout << "Error: " << Scenarios[i - 1].error << endl;
		cout << "Shift-Sequence: " << (Scenarios[i - 1].shiftSequence ==
				FOUR_ONE ? "FOUR-ONE" : "TWO_ONE_TWO") << endl;
		cout << "Time Behavior: " << (Scenarios[i - 1].modelTimeBehavior ?
				"Active" : "Not active") << endl;
		cout << "Target: " << Scenarios[i - 1].target << endl;
		cout << "----------------------------------------------------" << endl;
	}
	cin >> input;
	Scenario chosenScenario = Scenarios[input - 1];
	shiftSequence = chosenScenario.shiftSequence;
	modelTimeBehavior = chosenScenario.modelTimeBehavior;
	fPedMin = chosenScenario.fPedMin;
	fPedMax = chosenScenario.fPedMax;
	targetRPM = chosenScenario.targetRPM;
	deviation = chosenScenario.deviation;
	error = chosenScenario.error;
	target = chosenScenario.target;
}

int main() {
	double targetUpper;
	double targetLower;
	double upperLimit;
	double lowerLimit;
	double targetRPM;
	double deviation;
	double error;
	bool switchTimeModel = false;
	double fPedMax;
	double fPedMin;
	ShiftSequence shiftSequence;
	int shiftSequenceInt;
	string targetString;
	bool spaceEx = false;
	int input;
#ifdef USERINPUT
	cout << "Select scenario? (1/0):";
	cin >> input;
	if (input) {
		scenarioSelection(shiftSequence, switchTimeModel, fPedMin, fPedMax,
				targetRPM, deviation, error, targetString);
	} else {
		cout << "Please enter a max F_ped" << endl;
		cin >> fPedMax;
		cout << "Please enter a min F_ped" << endl;
		cin >> fPedMin;
		cout << "Please enter target in RPM:" << endl;
		cin >> targetRPM;
		cout << "Please enter deviation in RPM:" << endl;
		cin >> deviation;
		cout << "Please enter maximal allowed error in [%]:" << endl;
		cin >> error;
		cout << "Please enter a shift sequence for the big jumps (0: FOUR_ONE, 1 : TWO_ONE_TWO):" << endl;
		cin >> shiftSequenceInt;
		if (shiftSequenceInt == 0)
			shiftSequence = FOUR_ONE;
		else if (shiftSequenceInt == 1) {
			shiftSequence = TWO_ONE_TWO;
		}
		cout << "Please enter Property to check." << endl;
		cin >> targetString;
		cout << "Consider time behavior? (0: Gear shift in 0-time, 1: Gear shift takes 600 ms)" << endl;
		cin >>input;
		input == 1 ? switchTimeModel == true : switchTimeModel == false;
	}
	targetUpper = ((targetRPM + deviation) * 6.28f) / 60.0f;
	targetLower = ((targetRPM - deviation) * 6.28f) / 60.0f;
	upperLimit = targetUpper * (1 + (error / 100.0f));
	lowerLimit = targetUpper * (1 - (error / 100.0f));
#endif

#ifndef USERINPUT
	targetUpper = TARGETUPPEROMEGA;
	targetLower = TARGETLOWEROMEGA;
	upperLimit = UPPERLIMIT;
	lowerLimit = LOWERLIMIT;
	fPedMax = FPEDALHIGH;
	fPedMin = FPEDALLOW;
	shiftSequence = TWO_ONE_TWO;
#endif

	LinearHybridAutomaton bicycle("bicycle");

	/*
	 * Create the strings for the target.
	 */
	if (targetString.compare("omegaCrank < lowerLimit;") == 0)
		targetString = "(omegaCrank < " + bicycle.toString(lowerLimit) + ") and !one_one;";
	else if (targetString.compare("omegaCrank > upperLimit;") == 0)
		targetString = "(omegaCrank > " + bicycle.toString(upperLimit) + ") and !one_one;";

	/*
	 * ************************************************************************
	 * Constants
	 * ************************************************************************
	 */

	Constant timeDelta("deltaT", DELTANR);

	Constant switchTimeThreshold("switchTime", 0.400f);

	Constant one("one", 1.0f);
	bicycle.addConstant(one);

	Constant zero("zero", 0.0f);
	bicycle.addConstant(zero);

	Constant twenty("twenty", 20.0f);
	bicycle.addConstant(twenty);

	Constant ten("ten", 10.0f);
	bicycle.addConstant(ten);

	Constant upperLimitConstant("upperLimit", upperLimit),
			lowerLimitConstant("lowerLimit", lowerLimit);
	bicycle.addConstant(upperLimitConstant);
	bicycle.addConstant(lowerLimitConstant);

	//Constant targetPlus("targetPlus", TARGET + DEVIATION);
	Constant targetPlus("targetPlus", targetUpper);
	bicycle.addConstant(targetPlus);

	//Constant targetMinus("targetMinus", TARGET - DEVIATION);
	Constant targetMinus("targetMinus", targetLower);
	bicycle.addConstant(targetMinus);

	//Constant targetPlusGuard("targetPlusGuard", TARGET + (DEVIATION / 2));
	//Constant targetPlusGuard("targetPlusGuard", TARGET + DEVIATION);
	Constant targetPlusGuard("targetPlusGuard", targetUpper);
	bicycle.addConstant(targetPlusGuard);

	//Constant targetMinusGuard("targetMinusGuard", TARGET - (DEVIATION / 2));
	//Constant targetMinusGuard("targetMinusGuard", TARGET - DEVIATION);
	Constant targetMinusGuard("targetMinusGuard", targetLower);
	bicycle.addConstant(targetMinusGuard);

	Constant cFric("cFric", (-1 * MU * G) / RWHEEL);
	bicycle.addConstant(cFric);

	Constant cDrive11("cDrive11", C1 * (TEETHR1 / TEETHF1) - C2);
	bicycle.addConstant(cDrive11);
	Constant cDrive11Up("cDrive11Up", fPedMax * C1_1 * (TEETHR1 / TEETHF1) - C2);
	bicycle.addConstant(cDrive11Up);
	Constant cDrive11Low("cDrive11Low", fPedMin * C1_1 * (TEETHR1 / TEETHF1) - C2);
	bicycle.addConstant(cDrive11Low);

	Constant cDrive12("cDrive12", C1 * (TEETHR2 / TEETHF1) - C2);
	bicycle.addConstant(cDrive12);
	Constant cDrive12Up("cDrive12Up", fPedMax * C1_1 * (TEETHR2 / TEETHF1) - C2);
	bicycle.addConstant(cDrive12Up);
	Constant cDrive12Low("cDrive12Low", fPedMin * C1_1 * (TEETHR2 / TEETHF1) - C2);
	bicycle.addConstant(cDrive12Low);

	Constant cDrive13("cDrive13", C1 * (TEETHR3 / TEETHF1) - C2);
	bicycle.addConstant(cDrive13);
	Constant cDrive13Up("cDrive13Up", fPedMax * C1_1 * (TEETHR3 / TEETHF1) - C2);
	bicycle.addConstant(cDrive13Up);
	Constant cDrive13Low("cDrive13Low", fPedMin * C1_1 * (TEETHR3 / TEETHF1) - C2);
	bicycle.addConstant(cDrive13Low);

	Constant cDrive14("cDrive14", C1 * (TEETHR4 / TEETHF1) - C2);
	bicycle.addConstant(cDrive14);
	Constant cDrive14Up("cDrive14Up", fPedMax * C1_1 * (TEETHR4 / TEETHF1) - C2);
	bicycle.addConstant(cDrive14Up);
	Constant cDrive14Low("cDrive14Low", fPedMin * C1_1 * (TEETHR4 / TEETHF1) - C2);
	bicycle.addConstant(cDrive14Low);

	Constant cDrive15("cDrive15", C1 * (TEETHR5 / TEETHF1) - C2);
	bicycle.addConstant(cDrive15);
	Constant cDrive15Up("cDrive15Up", fPedMax * C1_1 * (TEETHR5 / TEETHF1) - C2);
	bicycle.addConstant(cDrive15Up);
	Constant cDrive15Low("cDrive15Low", fPedMin * C1_1 * (TEETHR5 / TEETHF1) - C2);
	bicycle.addConstant(cDrive15Low);

	Constant cDrive16("cDrive16", C1 * (TEETHR6 / TEETHF1) - C2);
	bicycle.addConstant(cDrive16);
	Constant cDrive16Up("cDrive16Up", fPedMax * C1_1 * (TEETHR6 / TEETHF1) - C2);
	bicycle.addConstant(cDrive16Up);
	Constant cDrive16Low("cDrive16Low", fPedMin * C1_1 * (TEETHR6 / TEETHF1) - C2);
	bicycle.addConstant(cDrive16Low);

	Constant cDrive17("cDrive17", C1 * (TEETHR7 / TEETHF1) - C2);
	bicycle.addConstant(cDrive17);
	Constant cDrive17Up("cDrive17Up", fPedMax * C1_1 * (TEETHR7 / TEETHF1) - C2);
	bicycle.addConstant(cDrive17Up);
	Constant cDrive17Low("cDrive17Low", fPedMin * C1_1 * (TEETHR7 / TEETHF1) - C2);
	bicycle.addConstant(cDrive17Low);

	Constant cDrive18("cDrive18", C1 * (TEETHR8 / TEETHF1) - C2);
	bicycle.addConstant(cDrive18);
	Constant cDrive18Up("cDrive18Up", fPedMax * C1_1 * (TEETHR8 / TEETHF1) - C2);
	bicycle.addConstant(cDrive18Up);
	Constant cDrive18Low("cDrive18Low", fPedMin * C1_1 * (TEETHR8 / TEETHF1) - C2);
	bicycle.addConstant(cDrive18Low);

	Constant cDrive24("cDrive24", C1 * (TEETHR4 / TEETHF2) - C2);
	bicycle.addConstant(cDrive24);
	Constant cDrive24Up("cDrive24Up", fPedMax * C1_1 * (TEETHR4 / TEETHF2) - C2);
	bicycle.addConstant(cDrive24Up);
	Constant cDrive24Low("cDrive24Low", fPedMin * C1_1 * (TEETHR4 / TEETHF2) - C2);
	bicycle.addConstant(cDrive24Low);

	Constant cDrive25("cDrive25", C1 * (TEETHR5 / TEETHF2) - C2);
	bicycle.addConstant(cDrive25);
	Constant cDrive25Up("cDrive25Up", fPedMax * C1_1 * (TEETHR5 / TEETHF2) - C2);
	bicycle.addConstant(cDrive25Up);
	Constant cDrive25Low("cDrive25Low", fPedMin * C1_1 * (TEETHR5 / TEETHF2) - C2);
	bicycle.addConstant(cDrive25Low);

	Constant cDrive26("cDrive26", C1 * (TEETHR6 / TEETHF2) - C2);
	bicycle.addConstant(cDrive26);
	Constant cDrive26Up("cDrive26Up", fPedMax * C1_1 * (TEETHR6 / TEETHF2) - C2);
	bicycle.addConstant(cDrive26Up);
	Constant cDrive26Low("cDrive26Low", fPedMin * C1_1 * (TEETHR6 / TEETHF2) - C2);
	bicycle.addConstant(cDrive26Low);

	Constant cDrive27("cDrive27", C1 * (TEETHR7 / TEETHF2) - C2);
	bicycle.addConstant(cDrive27);
	Constant cDrive27Up("cDrive27Up", fPedMax * C1_1 * (TEETHR7 / TEETHF2) - C2);
	bicycle.addConstant(cDrive27Up);
	Constant cDrive27Low("cDrive27Low", fPedMin * C1_1 * (TEETHR7 / TEETHF2) - C2);
	bicycle.addConstant(cDrive27Low);

	Constant cDrive28("cDrive28", C1 * (TEETHR8 / TEETHF2) - C2);
	bicycle.addConstant(cDrive28);
	Constant cDrive28Up("cDrive28Up", fPedMax * C1_1 * (TEETHR8 / TEETHF2) - C2);
	bicycle.addConstant(cDrive28Up);
	Constant cDrive28Low("cDrive28Low", fPedMin * C1_1 * (TEETHR8 / TEETHF2) - C2);
	bicycle.addConstant(cDrive28Low);

	Constant cDrive29("cDrive29", C1 * (TEETHR9 / TEETHF2) - C2);
	bicycle.addConstant(cDrive29);
	Constant cDrive29Up("cDrive29Up", fPedMax * C1_1 * (TEETHR9 / TEETHF2) - C2);
	bicycle.addConstant(cDrive29Up);
	Constant cDrive29Low("cDrive29Low", fPedMin * C1_1 * (TEETHR9 / TEETHF2) - C2);
	bicycle.addConstant(cDrive29Low);

	Constant cDrive210("cDrive210", C1 * (TEETHR10 / TEETHF2) - C2);
	bicycle.addConstant(cDrive210);
	Constant cDrive210Up("cDrive210Up", fPedMax * C1_1 * (TEETHR10 / TEETHF2) - C2);
	bicycle.addConstant(cDrive210Up);
	Constant cDrive210Low("cDrive210Low", fPedMin * C1_1 * (TEETHR10 / TEETHF2) - C2);
	bicycle.addConstant(cDrive210Low);

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
	Variable omegaWheel(Variable::REAL, "omegaWheel", 0, 150, 0.0);
	bicycle.addVariable(omegaWheel);

	Variable omegaCrank(Variable::REAL, "omegaCrank", 0, 150, 0.0);
	bicycle.addVariable(omegaCrank);

	Variable t(Variable::REAL, "t", 0, 500);
	bicycle.addVariable(t);

	Variable tSum(Variable::REAL, "tSum", 0, 500);
	bicycle.addVariable(tSum);

	Variable tSwitch(Variable::REAL, "tSwitch", 0, 5000);
	bicycle.addVariable(tSwitch);

	Variable bigJump(Variable::INTEGER, "bigJump", 0, 1, 0);
	bicycle.addVariable(bigJump);

	/*
	 * ************************************************************************
	 * Linear Predicates
	 * For Invariants, Guards, Assignments
	 * ************************************************************************
	 */
	LinearTerm oneTimesT(one, t);
	//LinearTerm oneTimesTSum(one, tSum);
	vector<LinearTerm> linTermsTimeThreshold;
	linTermsTimeThreshold.push_back(oneTimesT);

	LinearTerm oneTimesTSwitch(one, tSwitch);
	vector<LinearTerm> linTermsTimeSwitchThreshold;
	linTermsTimeSwitchThreshold.push_back(oneTimesTSwitch);

	LinearPredicate tSwitchGeqThreshold(linTermsTimeSwitchThreshold,
			LinearPredicate::GEQ, switchTimeThreshold);
	LinearPredicate tSwitchLeqThreshold(linTermsTimeSwitchThreshold,
			LinearPredicate::LEQ, switchTimeThreshold);
	LinearPredicate tSwitchEqualZero(linTermsTimeSwitchThreshold,
				LinearPredicate::EQUAL, zero);

	LinearTerm oneTimesOmegaWheel(one, omegaWheel);
	LinearTerm oneTimesOmegaCrank(one, omegaCrank);

	vector<LinearTerm> linTermsInv;
	//linTermsInv.push_back(oneTimesOmegaWheel);
	linTermsInv.push_back(oneTimesOmegaCrank);

	LinearTerm oneTimesBigJump(one, bigJump);
	vector<LinearTerm> linTermsBigJump;
	linTermsBigJump.push_back(oneTimesBigJump);

	// t > Threshold.
	LinearPredicate tGreaterThreshold(linTermsTimeThreshold,
			LinearPredicate::GREATER, switchTimeThreshold);

	// t = Threshold.
	LinearPredicate tEqualThreshold(linTermsTimeThreshold,
			LinearPredicate::EQUAL, switchTimeThreshold);

	// omegaWheel <= target + deviation
	LinearPredicate upLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetPlus);
	// omegaWheel >= target - deviation
	LinearPredicate downLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetMinus);

	// omegaWheel >= target + deviation / 2
	LinearPredicate upLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetPlusGuard);
	//LinearPredicate upLinPredGeq(linTermsInv, LinearPredicate::GEQ, targetPlus);
	// omegaWheel >= target + deviation
	LinearPredicate upLinPredGeq2(linTermsInv, LinearPredicate::GEQ, targetPlus);

	// omegaWheel <= target - deviation / 2
	LinearPredicate downLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetMinusGuard);
	//LinearPredicate downLinPredLeq(linTermsInv, LinearPredicate::LEQ, targetMinus);

	vector<LinearTerm> linTermsAssign;
	linTermsAssign.push_back(oneTimesBigJump);
	LinearPredicate bigJumpEqualsOne(linTermsAssign, LinearPredicate::EQUAL,
			one);
	LinearPredicate bigJumpEqualsZero(linTermsAssign, LinearPredicate::EQUAL,
			zero);

	vector<LinearTerm> linTermsTime;
	linTermsTime.push_back(oneTimesT);
	LinearPredicate tEqualsZero(linTermsTime, LinearPredicate::EQUAL,
			zero);

	vector<LinearTerm> linTermsTime2;
	linTermsTime2.push_back(oneTimesT);
	LinearPredicate tGreaterZero(linTermsTime2, LinearPredicate::GREATER,
				zero);

	vector<LinearTerm> linTermsTarget;
	linTermsTarget.push_back(oneTimesOmegaCrank);
	//linTermsTarget.push_back(oneTimesOmegaWheel);
	//LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, twenty);
	//LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, ten);
	//LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, twenty);
	LinearPredicate target(linTermsTarget, LinearPredicate::GEQ, upperLimitConstant);

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
	if (spaceEx)
		linPredRatio11.setFlow(true);
	LinearPredicate linPredRatio12(linTermsRatio12, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio12.setFlow(true);
	LinearPredicate linPredRatio13(linTermsRatio13, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio13.setFlow(true);
	LinearPredicate linPredRatio14(linTermsRatio14, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio14.setFlow(true);
	LinearPredicate linPredRatio15(linTermsRatio15, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio15.setFlow(true);
	LinearPredicate linPredRatio16(linTermsRatio16, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio16.setFlow(true);
	LinearPredicate linPredRatio17(linTermsRatio17, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio17.setFlow(true);
	LinearPredicate linPredRatio18(linTermsRatio18, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio18.setFlow(true);
	LinearPredicate linPredRatio24(linTermsRatio24, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio24.setFlow(true);
	LinearPredicate linPredRatio25(linTermsRatio25, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio25.setFlow(true);
	LinearPredicate linPredRatio26(linTermsRatio26, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio26.setFlow(true);
	LinearPredicate linPredRatio27(linTermsRatio27, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio27.setFlow(true);
	LinearPredicate linPredRatio28(linTermsRatio28, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio28.setFlow(true);
	LinearPredicate linPredRatio29(linTermsRatio29, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio29.setFlow(true);
	LinearPredicate linPredRatio210(linTermsRatio210, LinearPredicate::EQUAL, zero);
	if (spaceEx)
		linPredRatio210.setFlow(true);

	/*
	 * ************************************************************************
	 * Invariants
	 * ************************************************************************
	 */
	Invariant invUp11;
	invUp11.addLinPred(upLinPredLeq);
	invUp11.addLinPred(linPredRatio11);

	Invariant invUpAndDown11;
	invUpAndDown11.addLinPred(upLinPredLeq);

	Invariant invUpAndDown12;
	invUpAndDown12.addLinPred(upLinPredLeq);
	invUpAndDown12.addLinPred(downLinPredGeq);
	invUpAndDown12.addLinPred(linPredRatio12);
	//invUpAndDown12.addLinPred(tGreaterZero);

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
	invBigJump17.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump16;
	invBigJump16.addLinPred(linPredRatio16);
	invBigJump16.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump15;
	invBigJump15.addLinPred(linPredRatio15);
	invBigJump15.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump14;
	invBigJump14.addLinPred(linPredRatio14);
	invBigJump14.addLinPred(tSwitchLeqThreshold);

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

	Invariant invBigJump25;
	invBigJump25.addLinPred(linPredRatio25);
	invBigJump25.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump26;
	invBigJump26.addLinPred(linPredRatio26);
	invBigJump26.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump27;
	invBigJump27.addLinPred(linPredRatio27);
	invBigJump27.addLinPred(tSwitchLeqThreshold);

	Invariant invBigJump28;
	invBigJump28.addLinPred(linPredRatio28);
	invBigJump28.addLinPred(tSwitchLeqThreshold);

	Invariant invDown;
	invDown.addLinPred(downLinPredGeq);

	/*
	 * ************************************************************************
	 * Guards
	 * ************************************************************************
	 */

	Guard upNoBigJump;
	upNoBigJump.addLinPred(upLinPredGeq);
	if (switchTimeModel && spaceEx)
		upNoBigJump.addLinPred(tSwitchGeqThreshold);

	Guard upNoBigJump2;
	upNoBigJump2.addLinPred(upLinPredGeq2);

	Guard upBigJump;
	upBigJump.addLinPred(bigJumpEqualsOne);
	if (switchTimeModel && spaceEx)
		upBigJump.addLinPred(tSwitchGeqThreshold);

	Guard downBigJump;
	downBigJump.addLinPred(bigJumpEqualsZero);
	if (switchTimeModel && spaceEx)
		downBigJump.addLinPred(tSwitchGeqThreshold);

	Guard downNoBigJump;
	downNoBigJump.addLinPred(downLinPredLeq);
	if (switchTimeModel && spaceEx)
		downNoBigJump.addLinPred(tSwitchGeqThreshold);

	Guard emptyGuard;

	/*
	 * ************************************************************************
	 * Assignments
	 * ************************************************************************
	 */
	Assignment bigJumpToOne;
	bigJumpToOne.addLinPred(bigJumpEqualsOne);
	bigJumpToOne.addAssignedVariable(bigJump);

	Assignment bigJumpToZero;
	bigJumpToZero.addLinPred(bigJumpEqualsZero);
	bigJumpToZero.addAssignedVariable(bigJump);

	Assignment tZero;
	tZero.addLinPred(tEqualsZero);
	tZero.addAssignedVariable(t);

	Assignment bigJumpOne;
	bigJumpOne.addLinPred(bigJumpEqualsOne);
	bigJumpOne.addAssignedVariable(bigJump);

	Assignment assign11;
	assign11.addLinPred(linPredRatio11);
	assign11.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign11.addLinPred(tGreaterThreshold);
		assign11.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign11.addLinPred(tSwitchEqualZero);
		assign11.addAssignedVariable(tSwitch);
	}

	Assignment assign12;
	assign12.addLinPred(linPredRatio12);
	//assign12.addLinPred(tEqualsZero);
	assign12.addAssignedVariable(omegaCrank);
	//assign12.addAssignedVariable(t);
	if (switchTimeModel && !spaceEx) {
		assign12.addLinPred(tGreaterThreshold);
		assign12.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign12.addLinPred(tSwitchEqualZero);
		assign12.addAssignedVariable(tSwitch);
	}

	Assignment assign13;
	assign13.addLinPred(linPredRatio13);
	assign13.addAssignedVariable(omegaCrank);
	//assign13.addLinPred(tEqualsZero);
	//assign13.addAssignedVariable(omegaCrank);
	//assign13.addAssignedVariable(t);
	if (switchTimeModel && !spaceEx) {
		assign13.addLinPred(tGreaterThreshold);
		assign13.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign13.addLinPred(tSwitchEqualZero);
		assign13.addAssignedVariable(tSwitch);
	}

	Assignment assign14;
	assign14.addLinPred(linPredRatio14);
	assign14.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign14.addLinPred(tGreaterThreshold);
		assign14.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign14.addLinPred(tSwitchEqualZero);
		assign14.addAssignedVariable(tSwitch);
	}

	Assignment assign15;
	assign15.addLinPred(linPredRatio15);
	assign15.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign15.addLinPred(tGreaterThreshold);
		assign15.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign15.addLinPred(tSwitchEqualZero);
		assign15.addAssignedVariable(tSwitch);
	}

	Assignment assign16;
	assign16.addLinPred(linPredRatio16);
	assign16.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign16.addLinPred(tGreaterThreshold);
		assign16.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign16.addLinPred(tSwitchEqualZero);
		assign16.addAssignedVariable(tSwitch);
	}

	Assignment assign17;
	assign17.addLinPred(linPredRatio17);
	assign17.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign17.addLinPred(tGreaterThreshold);
		assign17.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign17.addLinPred(tSwitchEqualZero);
		assign17.addAssignedVariable(tSwitch);
	}

	Assignment assign18;
	assign18.addLinPred(linPredRatio18);
	assign18.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign18.addLinPred(tGreaterThreshold);
		assign18.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign18.addLinPred(tSwitchEqualZero);
		assign18.addAssignedVariable(tSwitch);
	}

	Assignment assign17BigUp;
	assign17BigUp.addLinPred(linPredRatio17);
	assign17BigUp.addAssignedVariable(omegaCrank);
	assign17BigUp.addLinPred(bigJumpEqualsOne);
	assign17BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign17BigUp.addLinPred(tEqualThreshold);
		assign17BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign17BigUp.addLinPred(tSwitchEqualZero);
		assign17BigUp.addAssignedVariable(tSwitch);
	}

	Assignment assign16BigUp;
	assign16BigUp.addLinPred(linPredRatio16);
	assign16BigUp.addAssignedVariable(omegaCrank);
	assign16BigUp.addLinPred(bigJumpEqualsOne);
	assign16BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign16BigUp.addLinPred(tEqualThreshold);
		assign16BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign16BigUp.addLinPred(tSwitchEqualZero);
		assign16BigUp.addAssignedVariable(tSwitch);
	}

	Assignment assign15BigUp;
	assign15BigUp.addLinPred(linPredRatio15);
	assign15BigUp.addAssignedVariable(omegaCrank);
	assign15BigUp.addLinPred(bigJumpEqualsOne);
	assign15BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign15BigUp.addLinPred(tEqualThreshold);
		assign15BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign15BigUp.addLinPred(tSwitchEqualZero);
		assign15BigUp.addAssignedVariable(tSwitch);
	}

	Assignment assign14BigUp;
	assign14BigUp.addLinPred(linPredRatio14);
	assign14BigUp.addAssignedVariable(omegaCrank);
	assign14BigUp.addLinPred(bigJumpEqualsOne);
	assign14BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign14BigUp.addLinPred(tEqualThreshold);
		assign14BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign14BigUp.addLinPred(tSwitchEqualZero);
		assign14BigUp.addAssignedVariable(tSwitch);
	}

//	Assignment assign24BigUp;
//	assign24BigUp.addLinPred(linPredRatio24);
//	assign24BigUp.addAssignedVariable(omegaCrank);
//	assign24BigUp.addLinPred(bigJumpEqualsOne);
//	assign24BigUp.addAssignedVariable(bigJump);
//	if (switchTimeModel && !spaceEx) {
//		assign24BigUp.addLinPred(tGreaterThreshold);
//		assign24BigUp.addAssignedVariable(t);
//	}

	Assignment assign25BigUp;
	assign25BigUp.addLinPred(linPredRatio25);
	assign25BigUp.addAssignedVariable(omegaCrank);
	assign25BigUp.addLinPred(bigJumpEqualsOne);
	assign25BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign25BigUp.addLinPred(tEqualThreshold);
		assign25BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign25BigUp.addLinPred(tSwitchEqualZero);
		assign25BigUp.addAssignedVariable(tSwitch);
	}

	Assignment assign26BigUp;
	assign26BigUp.addLinPred(linPredRatio26);
	assign26BigUp.addAssignedVariable(omegaCrank);
	assign26BigUp.addLinPred(bigJumpEqualsOne);
	assign26BigUp.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign26BigUp.addLinPred(tEqualThreshold);
		assign26BigUp.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign26BigUp.addLinPred(tSwitchEqualZero);
		assign26BigUp.addAssignedVariable(tSwitch);
	}

	Assignment assign24;
	assign24.addLinPred(linPredRatio24);
	assign24.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign24.addLinPred(tGreaterThreshold);
		assign24.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign24.addLinPred(tSwitchEqualZero);
		assign24.addAssignedVariable(tSwitch);
	}

	Assignment assign25;
	assign25.addLinPred(linPredRatio25);
	assign25.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign25.addLinPred(tGreaterThreshold);
		assign25.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign25.addLinPred(tSwitchEqualZero);
		assign25.addAssignedVariable(tSwitch);
	}

	Assignment assign25BigDown;
	assign25BigDown.addLinPred(linPredRatio25);
	assign25BigDown.addAssignedVariable(omegaCrank);
	assign25BigDown.addLinPred(bigJumpEqualsZero);
	assign25BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign25BigDown.addLinPred(tEqualThreshold);
		assign25BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign25BigDown.addLinPred(tSwitchEqualZero);
		assign25BigDown.addAssignedVariable(tSwitch);
	}

	Assignment assign26BigDown;
	assign26BigDown.addLinPred(linPredRatio26);
	assign26BigDown.addAssignedVariable(omegaCrank);
	assign26BigDown.addLinPred(bigJumpEqualsZero);
	assign26BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign26BigDown.addLinPred(tEqualThreshold);
		assign26BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign26BigDown.addLinPred(tSwitchEqualZero);
		assign26BigDown.addAssignedVariable(tSwitch);
	}

	Assignment assign27BigDown;
	assign27BigDown.addLinPred(linPredRatio27);
	assign27BigDown.addAssignedVariable(omegaCrank);
	assign27BigDown.addLinPred(bigJumpEqualsZero);
	assign27BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign27BigDown.addLinPred(tEqualThreshold);
		assign27BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign27BigDown.addLinPred(tSwitchEqualZero);
		assign27BigDown.addAssignedVariable(tSwitch);
	}

	Assignment assign28BigDown;
	assign28BigDown.addLinPred(linPredRatio28);
	assign28BigDown.addAssignedVariable(omegaCrank);
	assign28BigDown.addLinPred(bigJumpEqualsZero);
	assign28BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign28BigDown.addLinPred(tEqualThreshold);
		assign28BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign28BigDown.addLinPred(tSwitchEqualZero);
		assign28BigDown.addAssignedVariable(tSwitch);
	}

//	Assignment assign18BigDown;
//	assign18BigDown.addLinPred(linPredRatio18);
//	assign18BigDown.addAssignedVariable(omegaCrank);
//	assign18BigDown.addLinPred(bigJumpEqualsZero);
//	assign18BigDown.addAssignedVariable(bigJump);
//	if (switchTimeModel && !spaceEx) {
//		assign18BigDown.addLinPred(tGreaterThreshold);
//		assign18BigDown.addAssignedVariable(t);
//	}

	Assignment assign16BigDown;
	assign16BigDown.addLinPred(linPredRatio16);
	assign16BigDown.addAssignedVariable(omegaCrank);
	assign16BigDown.addLinPred(bigJumpEqualsZero);
	assign16BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign16BigDown.addLinPred(tEqualThreshold);
		assign16BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign16BigDown.addLinPred(tSwitchEqualZero);
		assign16BigDown.addAssignedVariable(tSwitch);
	}

	Assignment assign17BigDown;
	assign17BigDown.addLinPred(linPredRatio17);
	assign17BigDown.addAssignedVariable(omegaCrank);
	assign17BigDown.addLinPred(bigJumpEqualsZero);
	assign17BigDown.addAssignedVariable(bigJump);
	if (switchTimeModel && !spaceEx) {
		assign17BigDown.addLinPred(tEqualThreshold);
		assign17BigDown.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign17BigDown.addLinPred(tSwitchEqualZero);
		assign17BigDown.addAssignedVariable(tSwitch);
	}

	Assignment assign26;
	assign26.addLinPred(linPredRatio26);
	assign26.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign26.addLinPred(tGreaterThreshold);
		assign26.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign26.addLinPred(tSwitchEqualZero);
		assign26.addAssignedVariable(tSwitch);
	}

	Assignment assign27;
	assign27.addLinPred(linPredRatio27);
	assign27.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign27.addLinPred(tGreaterThreshold);
		assign27.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign27.addLinPred(tSwitchEqualZero);
		assign27.addAssignedVariable(tSwitch);
	}

	Assignment assign28;
	assign28.addLinPred(linPredRatio28);
	assign28.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign28.addLinPred(tGreaterThreshold);
		assign28.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign28.addLinPred(tSwitchEqualZero);
		assign28.addAssignedVariable(tSwitch);
	}

	Assignment assign29;
	assign29.addLinPred(linPredRatio29);
	assign29.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign29.addLinPred(tGreaterThreshold);
		assign29.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign29.addLinPred(tSwitchEqualZero);
		assign29.addAssignedVariable(tSwitch);
	}

	Assignment assign210;
	assign210.addLinPred(linPredRatio210);
	assign210.addAssignedVariable(omegaCrank);
	if (switchTimeModel && !spaceEx) {
		assign210.addLinPred(tGreaterThreshold);
		assign210.addAssignedVariable(t);
	} else if (switchTimeModel && spaceEx) {
		assign210.addLinPred(tSwitchEqualZero);
		assign210.addAssignedVariable(tSwitch);
	}

	Assignment bigJumpZero;
	bigJumpZero.addLinPred(bigJumpEqualsZero);

	Assignment emptyAssignment;

	/*
	 * ************************************************************************
	 * Bounds
	 * ************************************************************************
	 */
	Bound timeDerivative(t, one, one);

	Bound tSwitchDerivative(tSwitch, one, one);

	//Bound l11Omega(omegaWheel, cDrive11, cDrive11);
	Bound l11Omega(omegaWheel, cDrive11Up, cDrive11Low);
	//Bound l12Omega(omegaWheel, cDrive12, cDrive12);
	Bound l12Omega(omegaWheel, cDrive12Up, cDrive12Low);
	//Bound l13Omega(omegaWheel, cDrive13, cDrive13);
	Bound l13Omega(omegaWheel, cDrive13Up, cDrive13Low);
	//Bound l14Omega(omegaWheel, cDrive14, cDrive14);
	Bound l14Omega(omegaWheel, cDrive14Up, cDrive14Low);
	//Bound l15Omega(omegaWheel, cDrive15, cDrive15);
	Bound l15Omega(omegaWheel, cDrive15Up, cDrive15Low);
	//Bound l16Omega(omegaWheel, cDrive16, cDrive16);
	Bound l16Omega(omegaWheel, cDrive16Up, cDrive16Low);
	//Bound l17Omega(omegaWheel, cDrive17, cDrive17);
	Bound l17Omega(omegaWheel, cDrive17Up, cDrive17Low);
	//Bound l18Omega(omegaWheel, cDrive18, cDrive18);
	Bound l18Omega(omegaWheel, cDrive18Up, cDrive18Low);
	//Bound l24Omega(omegaWheel, cDrive24, cDrive24);
	Bound l24Omega(omegaWheel, cDrive24Up, cDrive24Low);
	//Bound l25Omega(omegaWheel, cDrive25, cDrive25);
	Bound l25Omega(omegaWheel, cDrive25Up, cDrive25Low);
	//Bound l26Omega(omegaWheel, cDrive26, cDrive26);
	Bound l26Omega(omegaWheel, cDrive26Up, cDrive26Low);
	//Bound l27Omega(omegaWheel, cDrive27, cDrive27);
	Bound l27Omega(omegaWheel, cDrive27Up, cDrive27Low);
	//Bound l28Omega(omegaWheel, cDrive28, cDrive28);
	Bound l28Omega(omegaWheel, cDrive28Up, cDrive28Low);
	//Bound l29Omega(omegaWheel, cDrive29, cDrive29);
	Bound l29Omega(omegaWheel, cDrive29Up, cDrive29Low);
	//Bound l210Omega(omegaWheel, cDrive210, cDrive210);
	Bound l210Omega(omegaWheel, cDrive210Up, cDrive210Low);

	Bound bigJumpBound(bigJump, zero, zero);

	Bound bigJumpBoundsOmega(omegaWheel, zero, zero);

	/*
	 * ************************************************************************
	 * Locations
	 * ************************************************************************
	 */
	vector <Bound> l11Bounds;
	l11Bounds.push_back(l11Omega);
	l11Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l11Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l11Bounds.push_back(tSwitchDerivative);
	}
	//l11Bounds.push_back(timeBound);
	Location loc11(11, "one_one", invUp11, l11Bounds, true);
	bicycle.addLocation(loc11);

	vector <Bound> l12Bounds;
	l12Bounds.push_back(l12Omega);
	//l12Bounds.push_back(timeBound);
	l12Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l12Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l12Bounds.push_back(tSwitchDerivative);
	}
	Location loc12(12, "one_two", invUpAndDown12, l12Bounds, false);
	bicycle.addLocation(loc12);

	vector <Bound> l13Bounds;
	l13Bounds.push_back(l13Omega);
	l13Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l13Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l13Bounds.push_back(tSwitchDerivative);
	}
	Location loc13(13, "one_three", invUpAndDown13, l13Bounds, false);
	bicycle.addLocation(loc13);

	vector <Bound> l14Bounds;
	l14Bounds.push_back(l14Omega);
	l14Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l14Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l14Bounds.push_back(tSwitchDerivative);
	}
	Location loc14(14, "one_four", invUpAndDown14, l14Bounds, false);
	bicycle.addLocation(loc14);

	vector <Bound> l15Bounds;
	l15Bounds.push_back(l15Omega);
	l15Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l15Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l15Bounds.push_back(tSwitchDerivative);
	}
	Location loc15(15, "one_five", invUpAndDown15, l15Bounds, false);
	bicycle.addLocation(loc15);

	vector <Bound> l16Bounds;
	l16Bounds.push_back(l16Omega);
	l16Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l16Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l16Bounds.push_back(tSwitchDerivative);
	}
	Location loc16(16, "one_six", invUpAndDown16, l16Bounds, false);
	bicycle.addLocation(loc16);

	vector <Bound> l17Bounds;
	l17Bounds.push_back(l17Omega);
	l17Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l17Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l17Bounds.push_back(tSwitchDerivative);
	}
	Location loc17(17, "one_seven", invUpAndDown17, l17Bounds, false);
	bicycle.addLocation(loc17);

	vector <Bound> l18Bounds;
	l18Bounds.push_back(l18Omega);
	l18Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l18Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l18Bounds.push_back(tSwitchDerivative);
	}
	Location loc18(18, "one_eight", invUpAndDown18, l18Bounds, false);
	bicycle.addLocation(loc18);

	vector <Bound> l17BoundsBigJump;
	//l17BoundsBigJump.push_back(bigJumpBoundsOmega);
	l17BoundsBigJump.push_back(l17Omega);
	l17BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l17BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l17BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc17BigJump(172, "one_seven_big",
			invBigJump17, l17BoundsBigJump, false);
	bicycle.addLocation(loc17BigJump);

	vector <Bound> l16BoundsBigJump;
	//l16BoundsBigJump.push_back(bigJumpBoundsOmega);
	l16BoundsBigJump.push_back(l16Omega);
	l16BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l16BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l16BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc16BigJump(162, "one_six_big",
			invBigJump16, l16BoundsBigJump, false);
	bicycle.addLocation(loc16BigJump);

	vector <Bound> l15BoundsBigJump;
	//l15BoundsBigJump.push_back(bigJumpBoundsOmega);
	l15BoundsBigJump.push_back(l15Omega);
	l15BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l15BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l15BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc15BigJump(152, "one_five_big",
			invBigJump15, l15BoundsBigJump, false);
	bicycle.addLocation(loc15BigJump);

	vector <Bound> l14BoundsBigJump;
	//l14BoundsBigJump.push_back(bigJumpBoundsOmega);
	l14BoundsBigJump.push_back(l14Omega);
	l14BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l14BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l14BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc14BigJump(142, "one_four_big",
			invBigJump14, l14BoundsBigJump, false);
	bicycle.addLocation(loc14BigJump);

	vector <Bound> l24Bounds;
	l24Bounds.push_back(l24Omega);
	l24Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l24Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l24Bounds.push_back(tSwitchDerivative);
	}
	Location loc24(24, "two_four", invUpAndDown24, l24Bounds, false);
	bicycle.addLocation(loc24);

	vector <Bound> l25Bounds;
	l25Bounds.push_back(l25Omega);
	l25Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l25Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l25Bounds.push_back(tSwitchDerivative);
	}
	Location loc25(25, "two_five", invUpAndDown25, l25Bounds, false);
	bicycle.addLocation(loc25);

	vector <Bound> l26Bounds;
	l26Bounds.push_back(l26Omega);
	l26Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l26Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l26Bounds.push_back(tSwitchDerivative);
	}
	Location loc26(26, "two_six", invUpAndDown26, l26Bounds, false);
	bicycle.addLocation(loc26);

	vector <Bound> l27Bounds;
	l27Bounds.push_back(l27Omega);
	l27Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l27Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l27Bounds.push_back(tSwitchDerivative);
	}
	Location loc27(27, "two_seven", invUpAndDown27, l27Bounds, false);
	bicycle.addLocation(loc27);

	vector <Bound> l28Bounds;
	l28Bounds.push_back(l28Omega);
	l28Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l28Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l28Bounds.push_back(tSwitchDerivative);
	}
	Location loc28(28, "two_eight", invUpAndDown28, l28Bounds, false);
	bicycle.addLocation(loc28);

	vector <Bound> l29Bounds;
	l29Bounds.push_back(l29Omega);
	l29Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l29Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l29Bounds.push_back(tSwitchDerivative);
	}
	Location loc29(29, "two_nine", invUpAndDown29, l29Bounds, false);
	bicycle.addLocation(loc29);

	vector <Bound> l210Bounds;
	l210Bounds.push_back(l210Omega);
	l210Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l210Bounds.push_back(timeDerivative);
		if (switchTimeModel)
			l210Bounds.push_back(tSwitchDerivative);
	}
	Location loc210(210, "two_ten", invUpAndDown210, l210Bounds, false);
	bicycle.addLocation(loc210);

	vector <Bound> l25BoundsBigJump;
	//l25BoundsBigJump.push_back(bigJumpBoundsOmega);
	l25BoundsBigJump.push_back(l25Omega);
	l25Bounds.push_back(bigJumpBound);
	if (spaceEx) {
		l25BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l25BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc25BigJump(252, "two_five_big",
			invBigJump25, l25BoundsBigJump, false);
	bicycle.addLocation(loc25BigJump);

	vector <Bound> l26BoundsBigJump;
	//l26BoundsBigJump.push_back(bigJumpBoundsOmega);
	l26BoundsBigJump.push_back(l26Omega);
	l26BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l26BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l26BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc26BigJump(662, "two_six_big",
			invBigJump26, l26BoundsBigJump, false);
	bicycle.addLocation(loc26BigJump);

	vector <Bound> l27BoundsBigJump;
//	l27BoundsBigJump.push_back(bigJumpBoundsOmega);
	l27BoundsBigJump.push_back(l27Omega);
	l27BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l27BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l27BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc27BigJump(272, "two_seven_big",
			invBigJump27, l27BoundsBigJump, false);
	bicycle.addLocation(loc27BigJump);

	vector <Bound> l28BoundsBigJump;
//	l28BoundsBigJump.push_back(bigJumpBoundsOmega);
	l28BoundsBigJump.push_back(l28Omega);
	l28BoundsBigJump.push_back(bigJumpBound);
	if (spaceEx) {
		l28BoundsBigJump.push_back(timeDerivative);
		if (switchTimeModel)
			l28BoundsBigJump.push_back(tSwitchDerivative);
	}
	Location loc28BigJump(282, "two_eight_big",
			invBigJump28, l28BoundsBigJump, false);
	bicycle.addLocation(loc28BigJump);

	/*
	 * ************************************************************************
	 * Edges
	 * ************************************************************************
	 */
	Edge one_one_one_two(loc11, loc12, upNoBigJump, assign12, "one_one_one_two");
	//Edge one_one_one_two(loc11, loc12, upNoBigJump, "one_one_one_two");
	//Edge one_one_one_two(loc11, loc12, "one_one_one_two");
	bicycle.addEdge(one_one_one_two);

	Edge one_two_one_three(loc12, loc13, upNoBigJump, assign13, "one_two_one_three");
	//Edge one_two_one_three(loc12, loc13, "one_two_one_three");
	bicycle.addEdge(one_two_one_three);

	Edge one_two_one_one(loc12, loc11, downNoBigJump, assign11, "one_two_one_one");
	bicycle.addEdge(one_two_one_one);

	Edge one_three_one_four(loc13, loc14, upNoBigJump, assign14, "one_three_one_four");
	bicycle.addEdge(one_three_one_four);

	Edge one_three_one_two(loc13, loc12, downNoBigJump, assign12, "one_three_one_two");
	bicycle.addEdge(one_three_one_two);

	Edge one_four_one_five(loc14, loc15, upNoBigJump, assign15, "one_four_one_five");
	bicycle.addEdge(one_four_one_five);
	Edge one_four_one_three(loc14, loc13, downNoBigJump, assign13, "one_four_one_three");
	bicycle.addEdge(one_four_one_three);

	Edge one_five_one_six(loc15, loc16, upNoBigJump, assign16, "one_five_one_six");
	bicycle.addEdge(one_five_one_six);
	Edge one_five_one_four(loc15, loc14, downNoBigJump, assign14, "one_five_one_four");
	bicycle.addEdge(one_five_one_four);

	Edge one_six_one_seven(loc16, loc17, upNoBigJump, assign17, "one_six_one_seven");
	bicycle.addEdge(one_six_one_seven);
	Edge one_six_one_five(loc16, loc15, downNoBigJump, assign15, "one_six_one_five");
	bicycle.addEdge(one_six_one_five);

	Edge one_seven_one_eight(loc17, loc18, upNoBigJump, assign18, "one_seven_one_eight");
	bicycle.addEdge(one_seven_one_eight);
	Edge one_seven_one_six(loc17, loc16, downNoBigJump, assign16, "one_seven_one_six");
	bicycle.addEdge(one_seven_one_six);

	Edge one_eight_one_seven(loc18, loc17, downNoBigJump, assign17, "one_eight_one_seven");
	bicycle.addEdge(one_eight_one_seven);

	/*
	 * Shift sequence for the big jump.
	 */
	Edge one_eight_one_seven_big(loc18, loc17BigJump, upNoBigJump, assign17BigUp, "one_eight_one_seven_big");
	bicycle.addEdge(one_eight_one_seven_big);

	Edge one_seven_one_six_big(loc17BigJump, loc16BigJump, upBigJump, assign16BigUp, "one_seven_one_six_big");
	bicycle.addEdge(one_seven_one_six_big);

	if (shiftSequence == FOUR_ONE) {
		Edge one_six_one_five_big(loc16BigJump, loc15BigJump, upBigJump, assign15BigUp, "one_six_one_five_big");
		bicycle.addEdge(one_six_one_five_big);
		Edge one_five_one_four_big(loc15BigJump, loc14BigJump, upBigJump, assign14BigUp, "one_five_one_four_big");
		bicycle.addEdge(one_five_one_four_big);
		Edge one_four_two_four_big(loc14BigJump, loc24, upBigJump, assign24, "one_four_two_four_big");
		bicycle.addEdge(one_four_two_four_big);
	} else if (shiftSequence == TWO_ONE_TWO) {
		Edge one_six_two_six_big(loc16BigJump, loc26BigJump, upBigJump, assign26BigUp, "one_six_two_six_big");
		bicycle.addEdge(one_six_two_six_big);
		Edge two_six_two_five_big(loc26BigJump, loc25BigJump, upBigJump, assign25BigUp, "two_six_two_five_big");
		bicycle.addEdge(two_six_two_five_big);
		Edge two_five_two_four_big(loc25BigJump, loc24, upBigJump, assign24, "two_five_two_four_big");
		bicycle.addEdge(two_five_two_four_big);
	}

	/*
	 * End of shift sequence for the big jump.
	 */

	Edge two_four_two_five(loc24, loc25, upNoBigJump, assign25, "two_four_two_five");
	bicycle.addEdge(two_four_two_five);

	Edge two_five_two_four(loc25, loc24, downNoBigJump, assign24, "two_five_two_four");
	bicycle.addEdge(two_five_two_four);

	Edge two_five_two_six(loc25, loc26, upNoBigJump, assign26, "two_five_two_six");
	bicycle.addEdge(two_five_two_six);

	Edge two_six_two_five(loc26, loc25, downNoBigJump, assign25, "two_six_two_five");
	bicycle.addEdge(two_six_two_five);

	Edge two_six_two_seven(loc26, loc27, upNoBigJump, assign27, "two_six_two_seven");
	bicycle.addEdge(two_six_two_seven);

	Edge two_seven_two_six(loc27, loc26, downNoBigJump, assign26, "two_seven_two_six");
	bicycle.addEdge(two_seven_two_six);

	Edge two_seven_two_eight(loc27, loc28, upNoBigJump, assign28, "two_seven_two_eight");
	bicycle.addEdge(two_seven_two_eight);

	Edge two_eight_two_seven(loc28, loc27, downNoBigJump, assign28, "two_eight_two_seven");
	bicycle.addEdge(two_eight_two_seven);

	Edge two_eight_two_nine(loc28, loc29, upNoBigJump, assign29, "two_eight_two_nine");
	bicycle.addEdge(two_eight_two_nine);

	Edge two_nine_two_eight(loc29, loc28, downNoBigJump, assign28, "two_nine_two_eight");
	bicycle.addEdge(two_nine_two_eight);

	Edge two_nine_two_ten(loc29, loc210, upNoBigJump, assign210, "two_nine_two_ten");
	bicycle.addEdge(two_nine_two_ten);

	Edge two_ten_two_nine(loc210, loc29, downNoBigJump, assign210, "two_ten_two_nine");
	bicycle.addEdge(two_ten_two_nine);

	/*
	 * Shift sequence for the big down jump.
	 */
	Edge two_four_two_five_big(loc24, loc25BigJump, downNoBigJump,
			assign25BigDown, "two_four_two_five_big");
	bicycle.addEdge(two_four_two_five_big);

	Edge two_five_two_six_big(loc25BigJump, loc26BigJump, downBigJump, assign26BigDown, "two_five_two_six_big");
	bicycle.addEdge(two_five_two_six_big);

	/*
	 * Set up the edges that are determined by the shift sequence.
	 */
	if (shiftSequence == FOUR_ONE) {
		Edge two_six_two_seven_big(loc26BigJump, loc27BigJump, downBigJump, assign27BigDown, "two_six_two_seven_big");
		bicycle.addEdge(two_six_two_seven_big);
		Edge two_seven_two_eight_big(loc27BigJump, loc28BigJump, downBigJump, assign28BigDown, "two_seven_two_eight_big");
		bicycle.addEdge(two_seven_two_eight_big);
		Edge two_eight_one_eight_big(loc28BigJump, loc18, downBigJump, assign18, "two_eight_one_eight_big");
		bicycle.addEdge(two_eight_one_eight_big);
	} else if (shiftSequence == TWO_ONE_TWO) {
		Edge two_six_one_six_big(loc26BigJump, loc16BigJump, downBigJump, assign16BigDown, "two_six_one_six_big");
		bicycle.addEdge(two_six_one_six_big);
		Edge one_six_one_seven_big(loc16BigJump, loc17BigJump, downBigJump, assign17BigDown, "one_six_one_seven_big");
		bicycle.addEdge(one_six_one_seven_big);
		Edge one_seven_one_eight_big(loc17BigJump, loc18, downBigJump, assign18, "one_seven_one_eight_big");
		bicycle.addEdge(one_seven_one_eight_big);
	}

	/*
	 * Generate the SpaceEx XML-File.
	 */
	if (spaceEx) {
		bicycle.toSpaceExXML("(omegaCrank > " + bicycle.toString(upperLimit) + ") and !one_one;");
		return 0;
	}

	/*
	 * Generate the iSat3 .hys File and start the isat solver.
	 */
	if (!spaceEx) {
		bicycle.setUpIsat3();
		//bicycle.toHysFile(target);
		//bicycle.toHysFile("two_nine;");
		//bicycle.toHysFile("(omegaCrank > " + bicycle.toString(upperLimit) + ") and !one_one;");
		bicycle.toHysFile(targetString);
		system("./isat3 -I -v -v --start-depth 0 --max-depth 150 LHA.hys");
		return 0;
	}
	bicycle.setUpVariables();
	bicycle.toIsat3BMC();
	bicycle.setUpInitial();
	bicycle.setTarget(target);
	bicycle.solveBMCIsat();
	//bicycle.writeToFile();
	bicycle.printBMCResultIsat(TIMEFRAMES);
}
