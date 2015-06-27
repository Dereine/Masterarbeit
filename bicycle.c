#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <bicycle.h>

// Taken from the isat3 Example.
char* printIntervalOfVariable(
		struct isat3			*is3,
		struct isat3_node		*variable,
		i3_tframe_t			tframe) {
	char interval[1000];
	if (variable != NULL) {
		//printf("tframe %d: %s %s%1.40f, %1.40f%s\n",
		printf("tframe %d: %s %s%1.5f, %1.5f%s\n",
				tframe,
				isat3_node_get_variable_name(is3, variable),
				isat3_is_lower_bound_strict(is3, variable, tframe) ? "(" : "[",
				isat3_get_lower_bound(is3, variable, tframe),
				isat3_get_upper_bound(is3, variable, tframe),
				isat3_is_upper_bound_strict(is3, variable, tframe) ? ")" : "]");
		sprintf(interval, "%1.5f;%1.5f;",
				isat3_get_lower_bound(is3, variable, tframe),
				isat3_get_upper_bound(is3, variable, tframe));
	}
	return interval;
}

char* printTruthValueOfVariable(struct isat3	*is3, struct isat3_node *variable, i3_tframe_t tframe) {
	char text[100];
	printf("tframe %d: %s %s\n", tframe,
			isat3_node_get_variable_name(is3, variable),
			isat3_get_truth_value(is3, variable, tframe) ? "true" : "false");
	sprintf(text, "%s;",isat3_get_truth_value(is3, variable, tframe) ? "1" : "0");
	return text;

}

void writeToFile() {
	int i = 0;
	FILE * theFile = fopen("trace.txt", "w");
	fprintf(theFile, "TF;[t;t];JU;JD;[wwheel;wwheel];[wweelDot;wweelDot];[wcrank;wcrank];[TU;TU];[TL;TL];[gR;gR];[gF;gF]\n");
	char dummy[100];
	for (i = 0; i < TIMEFRAMES; i++) {
		printIntervalOfVariable(isatInstance, t, i);
		if (i > 0)
			theFile = fopen("trace.txt", "a");
		sprintf(dummy, "%d;", i);
		fprintf(theFile, dummy);
		fprintf(theFile, printIntervalOfVariable(isatInstance, t, i));
		fprintf(theFile, printTruthValueOfVariable(isatInstance, jumpUp, i));
		fprintf(theFile, printTruthValueOfVariable(isatInstance, jumpDown, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, omegaWheel, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, omegaWheelDot, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, omegaCrank, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, targetUpper, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, targetLower, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, gearRear, i));
		fprintf(theFile, printIntervalOfVariable(isatInstance, gearFront, i));
		//printIntervalOfVariable(isatInstance, bigJump, i);
		fprintf(theFile, "\n");
		fclose(theFile);
	}

}

void setUpVariablesAndConstants() {
	omegaWheel = isat3_node_create_variable_float(isatInstance, "omegaWheel", 0, 100);
	omegaWheelDot = isat3_node_create_variable_float(isatInstance, "omegaWheelDot", 0, 100);

	omegaCrank = isat3_node_create_variable_float(isatInstance, "omegaCrank", 0, 100);
	rF1 = isat3_node_create_constant_float(isatInstance, 1 / 0.075); // 1 / because division not allowed
	rF2 = isat3_node_create_constant_float(isatInstance, 1 / 0.1);

	rF1Define = isat3_node_create_define(isatInstance, "rF1", rF1);
	rF2Define = isat3_node_create_define(isatInstance, "rF2", rF2);

	rR1 = isat3_node_create_constant_float(isatInstance, 0.1);
	rR2 = isat3_node_create_constant_float(isatInstance, 0.092);
	rR3 = isat3_node_create_constant_float(isatInstance, 0.084);
	rR4 = isat3_node_create_constant_float(isatInstance, 0.076);
	rR5 = isat3_node_create_constant_float(isatInstance, 0.068);
	rR6 = isat3_node_create_constant_float(isatInstance, 0.064);
	rR7 = isat3_node_create_constant_float(isatInstance, 0.06);
	rR8 = isat3_node_create_constant_float(isatInstance, 0.056);
	rR9 = isat3_node_create_constant_float(isatInstance, 0.052);
	rR10 = isat3_node_create_constant_float(isatInstance, 0.048);

	rR1Define = isat3_node_create_define(isatInstance, "rR1", rR1);
	rR2Define = isat3_node_create_define(isatInstance, "rR2", rR2);
	rR3Define = isat3_node_create_define(isatInstance, "rR3", rR3);
	rR4Define = isat3_node_create_define(isatInstance, "rR4", rR4);
	rR5Define = isat3_node_create_define(isatInstance, "rR5", rR5);
	rR6Define = isat3_node_create_define(isatInstance, "rR6", rR6);
	rR7Define = isat3_node_create_define(isatInstance, "rR7", rR7);
	rR8Define = isat3_node_create_define(isatInstance, "rR8", rR8);
	rR9Define = isat3_node_create_define(isatInstance, "rR9", rR9);
	rR10Define = isat3_node_create_define(isatInstance, "rR10", rR10);

	c1 = isat3_node_create_constant_float(isatInstance, FPEDAL * ((RPEDAL) / (pow(RWHEEL, 2) * M)));
	c1Define = isat3_node_create_define(isatInstance, "c1", c1);
	c2 = isat3_node_create_constant_float(isatInstance, (-1 * MU * G) / RWHEEL);
	c2Define = isat3_node_create_define(isatInstance, "c2", c2);
	cInv = isat3_node_create_constant_float(isatInstance, CINV);
	cInvDefine = isat3_node_create_define(isatInstance, "cInv", cInv);

	tS = isat3_node_create_constant_float(isatInstance, TS);
	tSDefine = isat3_node_create_define(isatInstance, "tS", tS);

	t = isat3_node_create_variable_float(isatInstance, "t", 0, 1000000); // TODO: find a meaningful value here.

	/*
	targetUpper = isat3_node_create_constant_integer(isatInstance, TARGET + DEVIATION);
	targetUpperDefine = isat3_node_create_define(isatInstance, "targetUpper", targetUpper);
	targetLower = isat3_node_create_constant_integer(isatInstance, TARGET - DEVIATION);
	targetLowerDefine = isat3_node_create_define(isatInstance, "targetLower", targetLower);
*/
	targetUpper = isat3_node_create_variable_integer(isatInstance, "targetUpper", TARGET + DEVIATION, TARGET + DEVIATION);
	targetLower = isat3_node_create_variable_integer(isatInstance, "targetLower", TARGET - DEVIATION, TARGET - DEVIATION);

	//constantZero = isat3_node_create_constant_integer(isatInstance, 0);
	//constantZeroDefine = isat3_node_create_define(isatInstance, "0", constantZero);

	jumpUp = isat3_node_create_variable_boole(isatInstance, "jumpUp");
	jumpDown = isat3_node_create_variable_boole(isatInstance, "jumpDown");
	bigJump = isat3_node_create_variable_boole(isatInstance, "bigJump");

	mode = isat3_node_create_variable_integer(isatInstance,"mode", 0, 21);

	gearRear = isat3_node_create_variable_integer(isatInstance,"gearRear", 1, 10);
	gearFront= isat3_node_create_variable_integer(isatInstance,"gearFront", 1, 2);
}

struct isat3_node* addFlow(int gearFront, int gearRear,
						   struct isat3 *isatInstance,
						   int up, int down, int gearFrontDestUp,
						   int gearRearDestUp, int gearFrontDestDown,
						   int gearRearDestDown) {
	char*  transition[1000];
	printf("*******************************************************************\n");
	printf("Building flow \n");
	sprintf(transition, "gearFront = %d and \n"
			"gearRear = %d	and \n"
			"!jumpDown and !jumpUp	 -> 	omegaCrank' = omegaWheel * (rR%d * rF%d) and\n"
			"						   	    omegaWheelDot' = (c1 * (rR%d * rF%d) - c2);\n",
			gearFront,
			gearRear, gearRear, gearFront,
			gearRear, gearFront);
	struct isat3_node* node = isat3_node_create_from_string(isatInstance, transition);
	printf(transition);
	if (up) {
		sprintf(transition, "gearFront = %d and \n"
				"gearRear = %d	and jumpUp  ->  omegaCrank' = omegaWheel * (rR%d * rF%d) and\n"
				"						   	    omegaWheelDot' = (c1 * (rR%d * rF%d) - c2);\n",
				gearFront,
				gearRear, gearRearDestUp, gearFrontDestUp,
				gearRearDestUp, gearFrontDestUp);
		printf(transition);
		node = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, node,
				isat3_node_create_from_string(isatInstance, transition));
	}
	if (down) {
		sprintf(transition, "gearFront = %d and \n"
				"gearRear = %d	and jumpDown -> omegaCrank' = omegaWheel * (rR%d * rF%d) and\n"
				"						   	    omegaWheelDot' = (c1 * (rR%d * rF%d) - c2);\n",
				gearFront,
				gearRear, gearRearDestDown, gearFrontDestDown,
				gearRearDestDown, gearFrontDestDown);
		printf(transition);
		node = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, node,
				isat3_node_create_from_string(isatInstance, transition));
	}
	return node;
}


struct isat3_node* buildFlows() {
	int iFront = 0;
	int iRear = 0;
	struct isat3_node* flow;
	flow = addFlow(1, 1, isatInstance, 1, 1, 1, 2, 1, 1);
	//flow = addFlow(1, 1, isatInstance, 1, 1, 1, 2, 1, 1);
	for (iFront = 1; iFront< 3; iFront++) {
		for (iRear = 1; iRear < 11; iRear++) {
			if (!(iRear == 1 && iFront == 1)) {
				// Conjunct the new node with the rest.
				flow = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, flow,
						addFlow(iFront, iRear, isatInstance, 1, 1, iFront,
							    iRear < 10 ? iRear + 1 : iRear, iFront,
							    iRear > 1 ? iRear- 1 : iRear));
			}
		}
	}
	return flow;
}

// TODO: Reminder bigJump or Not could also be solved with ITE
// 0 for Up, 1 for down
struct isat3_node* addTransition(int gearFrontSource, int gearRearSource, int gearFrontDest, int gearRearDest, int bigJumpPrem, int bigJumpConc, int upOrDown, struct isat3 *isatInstance) {
	char*  transition[2000];
	if (!bigJumpPrem && !bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear = %d and gearFront = %d -> gearRear' = %d and gearFront' = %d;\n",
								gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear = %d and gearFront = %d -> gearRear' = %d and gearFront' = %d;\n",
								gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		}
	} else if (bigJumpPrem && !bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear = %d and gearFront = %d and bigJump ->"
								"gearRear' = %d and gearFront' = %d and bigJump' = !bigJump;\n",
					gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear = %d and gearFront = %d and bigJump ->"
								"gearRear' = %d and gearFront' = %d and bigJump' = !bigJump;\n",
								gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		}
	} else if (!bigJumpPrem && bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear = %d and gearFront = %d and !bigJump->"
								"gearRear' = %d and gearFront' = %d and bigJump' = !bigJump;\n",
					gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear = %d and gearFront = %d and !bigJump->"
								"gearRear' = %d and gearFront' = %d and bigJump' = !bigJump;\n",
					gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		}
	} else if (bigJumpPrem && bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear  = %d and gearFront = %d and bigJump ->"
								"gearRear' = %d and bigJump' = bigJump and gearFront' = %d;\n",
					gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear  = %d and gearFront = %d and bigJump ->"
								"gearRear' = %d and bigJump' = bigJump and gearFront' = %d;\n",
					gearRearSource, gearFrontSource, gearRearDest, gearFrontDest);
		}
	}
	printf("*******************************************************************************************************\n");
	printf("Building Transition %d and %d for %s\n", gearFrontSource, gearRearSource, upOrDown == UP ? "UP" : "DOWN");
	printf(transition);
	struct isat3_node* node = isat3_node_create_from_string(isatInstance, transition);
	return node;
}
struct isat3_node* buildTransition() {
	int iFront = 0;
	int iRear = 0;
	struct isat3_node* jump;
	// Initial node jump
	jump = addTransition(1, 1, 1, 2, NOBIGJUMP, NOBIGJUMP, UP, isatInstance);
	jump = isat3_node_create_binary_operation(isatInstance,
			ISAT3_NODE_BOP_AND, jump, addTransition(1, 2, 1, 1, NOBIGJUMP, NOBIGJUMP, DOWN, isatInstance));
	for (iFront = 1; iFront < 3; iFront++) {
		for (iRear = 1; iRear < 11; iRear++) {
			// Special case big jump
			if (iRear == 8 && iFront == 1) {
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 8,
								1, 7, NOBIGJUMP, BIGJUMP, DOWN,
								isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 7,
								1, 6, BIGJUMP, BIGJUMP, DOWN,
								isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 6,
								1, 5, BIGJUMP, BIGJUMP, DOWN,
								isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 5,
								1, 4, BIGJUMP, BIGJUMP, DOWN,
								isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 4,
								2, 4, BIGJUMP, NOBIGJUMP, DOWN,
								isatInstance));
				// Normal Jump from 1-9 to 1-8
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 9,
								1, 8, NOBIGJUMP, NOBIGJUMP, DOWN,
								isatInstance));


			} else if (iRear == 3 && iFront == 2) {
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(2, 3, 1, 3, NOBIGJUMP, BIGJUMP, DOWN, isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 3, 1, 4, BIGJUMP, BIGJUMP, UP, isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 4, 1, 5, BIGJUMP, BIGJUMP, UP, isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 5, 1, 6, BIGJUMP, BIGJUMP, UP, isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 6, 1, 7, BIGJUMP, BIGJUMP, UP, isatInstance));
				// Normal Jump from 2-3 to 2-4
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(2, 3, 2, 4, NOBIGJUMP, NOBIGJUMP, UP, isatInstance));
			} else if (iRear == 1 && iFront == 1){
				// We created this node already as our dummy node.
				;
			} else {
				// small jump
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(iFront, iRear, iFront, iRear < 10 ?  iRear + 1 : iRear, NOBIGJUMP, NOBIGJUMP, UP, isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(iFront, iRear, iFront, iRear > 1 ?  iRear - 1 : iRear, NOBIGJUMP, NOBIGJUMP, DOWN, isatInstance));
			}
		}
	}
	return jump;
}

/*
struct isat3_node* addInvariant(int gearFront, int gearRear, struct isat3 *isatInstance) {
	char* invariant[1000];
	//sprintf(invariant, "")
}
 */

int main(void) {
	isat3_setup();
	isatInstance = isat3_init(NULL);
	setUpVariablesAndConstants();

	// Init condition
	init = isat3_node_create_from_string(isatInstance, "omegaWheel = 0;\n"
													   "omegaWheelDot = (c1 * (rR1 * rF1)) - c2;\n"
													   "omegaCrank = 0;\n"
													   "gearRear = 1;\n"
													   "gearFront = 1;\n"
													   "t = 0;\n"
													   "!jumpUp;\n"
													   "!jumpDown;\n");
	// Basic edge labels
	trans = isat3_node_create_from_string(isatInstance, "!jumpUp and !jumpDown	<-> (omegaWheel' = omegaWheel + omegaWheelDot * tS and\n"
														"							 t' = t + tS) and \n"
														"							 gearRear' = gearRear and gearFront' = gearFront;\n"
														"jumpUp or jumpDown		-> omegaWheel' = omegaWheel and t' = t;\n"														"							"
														"jumpUp 				<-> omegaCrank > targetUpper and !(gearRear = 10 and gearFront = 2);\n"
														"jumpDown				<-> omegaCrank < targetLower and !(gearRear = 1 and gearFront = 1);\n");

	/**
	 * Transitions
	 */
	printf("******************************************************************\n"
		   "		Building transitions				  \n");

	trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, buildTransition());
	//trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addTransition(1, 1, 1, 2, NOBIGJUMP, NOBIGJUMP, UP, isatInstance));
	//trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addTransition(1, 2, 1, 3, NOBIGJUMP, NOBIGJUMP, UP, isatInstance));

	/*
	 * Build the flows.
	 */
	trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, buildFlows());
	//trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addFlow(1, 1, isatInstance, 1, 0, 1, 2, 1, 1));
	//trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addFlow(1, 2, isatInstance, 1, 0, 1, 3, 1, 1));
	//trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addFlow(1, 3, isatInstance, 1, 0, 1, 4, 1, 2));

	/**
	 * The target
	 */
	target = isat3_node_create_from_string(isatInstance, "omegaWheel > targetUpper;");

	i3_type_t result = isat3_solve_bmc(isatInstance, init, trans, target, 0, TIMEFRAMES, 3140000);

	i3_type_t tframe = isat3_get_tframe(isatInstance);
	printf("%s (in tframe %d)\n", isat3_get_result_string(result), tframe);
	int i = 0;
	//writeToFile();
	for (i = 0; i < TIMEFRAMES; i++) {
		if(isat3_get_lower_bound(isatInstance, gearRear, i) == 8)
			printf("Stop!");
		printIntervalOfVariable(isatInstance, t, i);
		printTruthValueOfVariable(isatInstance, jumpUp, i);
		printTruthValueOfVariable(isatInstance, jumpDown, i);
		printIntervalOfVariable(isatInstance, omegaWheel, i);
		printIntervalOfVariable(isatInstance, omegaWheelDot, i);
		printIntervalOfVariable(isatInstance, omegaCrank, i);
		printIntervalOfVariable(isatInstance, targetUpper, i);
		printIntervalOfVariable(isatInstance, targetLower, i);
		printIntervalOfVariable(isatInstance, gearRear, i);
		printIntervalOfVariable(isatInstance, gearFront, i);
		printTruthValueOfVariable(isatInstance, bigJump, i);
		printf("-------------------------------------------------------------------------\n");
	}
	return 0;
}
