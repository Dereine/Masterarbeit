#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <bicycle.h>

// Taken from the isat3 Example.
void printIntervalOfVariable(
		struct isat3			*is3,
		struct isat3_node		*variable,
		i3_tframe_t			tframe) {
	if (variable != NULL) {
		//printf("tframe %d: %s %s%1.40f, %1.40f%s\n",
		printf("tframe %d: %s %s%1.5f, %1.5f%s\n",
				tframe,
				isat3_node_get_variable_name(is3, variable),
				isat3_is_lower_bound_strict(is3, variable, tframe) ? "(" : "[",
						isat3_get_lower_bound(is3, variable, tframe),
						isat3_get_upper_bound(is3, variable, tframe),
						isat3_is_upper_bound_strict(is3, variable, tframe) ? ")" : "]");
	}
}

void printTruthValueOfVariable(struct isat3	*is3, struct isat3_node *variable, i3_tframe_t tframe) {
	printf("tframe %d: %s %s\n", tframe,
			isat3_node_get_variable_name(is3, variable),
			isat3_get_truth_value(is3, variable, tframe) ? "true" : "false");
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


	targetUpper = isat3_node_create_constant_integer(isatInstance, TARGET + DEVIATION);
	targetUpperDefine = isat3_node_create_define(isatInstance, "targetUpper", targetUpper);
	targetLower = isat3_node_create_constant_integer(isatInstance, TARGET - DEVIATION);
	targetLowerDefine = isat3_node_create_define(isatInstance, "targetLower", targetLower);

	//constantZero = isat3_node_create_constant_integer(isatInstance, 0);
	//constantZeroDefine = isat3_node_create_define(isatInstance, "0", constantZero);

	jumpUp = isat3_node_create_variable_boole(isatInstance, "jumpUp");
	jumpDown = isat3_node_create_variable_boole(isatInstance, "jumpDown");
	bigJump = isat3_node_create_variable_boole(isatInstance, "bigJump");

	mode = isat3_node_create_variable_integer(isatInstance,"mode", 0, 21);

	gearRear = isat3_node_create_variable_integer(isatInstance,"gearRear", 1, 10);
	gearFront= isat3_node_create_variable_integer(isatInstance,"gearFront", 1, 2);
}

struct isat3_node* addFlow(int gearFront, int gearRear, struct isat3 *isatInstance) {
	char*  transition[1000];
	sprintf(transition, "!jumpUp and !jumpDown and\n"
			"gearFront = %d and \n"
			"gearRear = %d			-> omegaCrank' = omegaWheel * (rR%d * rF%d) and\n"
			"						   omegaWheelDot' = (c1 * (rR%d * rF%d) - c2);\n",
			gearFront,
			gearRear, gearRear, gearFront,
			gearRear, gearFront);
	struct isat3_node* node = isat3_node_create_from_string(isatInstance, transition);
	printf("*******************************************************************\n");
	printf("Building flow \n");
	printf(transition);
	return node;
}

struct isat3_node* buildFlows() {
	int iFront = 0;
	int iRear = 0;
	struct isat3_node* flow;
	flow = addFlow(1, 1, isatInstance);
	for (iFront = 1; iFront< 3; iFront++) {
		for (iRear = 1; iRear < 11; iRear++) {
			if (!(iRear == 1 && iFront == 1))
				// Conjunct the new node with the rest.
				flow = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, flow, addFlow(iFront, iRear, isatInstance));
		}
	}
}

// TODO: Reminder bigJump or Not could also be solved with ITE
// 0 for Up, 1 for down
struct isat3_node* addTransition(int gearFrontSource, int gearRearSource, int gearFrontDest, int gearRearDest, int bigJumpPrem, int bigJumpConc, int upOrDown, struct isat3 *isatInstance) {
	char*  transition[2000];
	if (!bigJumpPrem && !bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear = %d and !bigJump -> gearRear' = %d and bigJump' = false and\n"
								"jumpUp and gearFront = %d and !bigJump -> gearFront' = %d and bigJump' = false;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear  = %d and !bigJump -> gearRear' = %d and bigJump' = false and\n"
								"jumpDown and gearFront = %d and !bigJump -> gearFront' = %d and bigJump' = false;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		}
	} else if (bigJumpPrem && !bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear  = %d and bigJump	-> gearRear' = %d and bigJump = false and\n"
								"jumpUp and gearFront = %d and bigJump	-> gearFront' = %d and bigJump = false;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear  = %d and bigJump -> gearRear' = %d and bigJump = false and\n"
								"jumpDown and gearFront = %d and bigJump -> gearFront' = %d and bigJump = false;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		}
	} else if (!bigJumpPrem && bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear  = %d and !bigJump	-> gearRear' = %d and bigJump = true and\n"
								"jumpUp and gearFront = %d and !bigJump	-> gearFront' = %d and bigJump = true;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear  = %d and !bigJump -> gearRear' = %d and bigJump = true and\n"
								"jumpDown and gearFront = %d and !bigJump -> gearFront' = %d and bigJump = true;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		}
	} else if (bigJumpPrem && bigJumpConc) {
		if (upOrDown == UP) {
			sprintf(transition, "jumpUp and gearRear  = %d and bigJump	-> gearRear' = %d and bigJump = true and\n"
								"jumpUp and gearFront = %d and bigJump	-> gearFront' = %d and bigJump = true;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
		} else if (upOrDown == DOWN) {
			sprintf(transition, "jumpDown and gearRear  = %d and bigJump -> gearRear' = %d and bigJump = true and\n"
								"jumpDown and gearFront = %d and bigJump -> gearFront' = %d and bigJump = true;\n",
					gearRearSource, gearRearDest,
					gearFrontSource, gearFrontDest);
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
			if (iRear == 9 && iFront == 1) {
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 9,
								1, 8, NOBIGJUMP, BIGJUMP, DOWN,
								isatInstance));
				jump = isat3_node_create_binary_operation(isatInstance,
						ISAT3_NODE_BOP_AND, jump, addTransition(1, 8,
								1, 7, BIGJUMP, BIGJUMP, DOWN,
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
			"t = 0;\n");

	// Basic edge labels
	trans = isat3_node_create_from_string(isatInstance, "!jumpUp and !jumpDown	-> (omegaWheel' = omegaWheel + omegaWheelDot * tS and\n"
			"		   					t' = t + tS);\n"
			"jumpUp 				<-> omegaCrank > targetUpper;\n"
			"jumpDown				<-> omegaCrank < targetLower;\n");

	/**
	 * Transitions
	 */
	printf("******************************************************************\n"
		   "		Building transitions				  \n");
	//trans = isat3_node_create_binary_operation(isatInstance,
							//ISAT3_NODE_BOP_AND, trans, buildTransition());
	trans = isat3_node_create_binary_operation(isatInstance, ISAT3_NODE_BOP_AND, trans, addTransition(1, 1, 1, 2, NOBIGJUMP, NOBIGJUMP, UP, isatInstance));

	/*
	 * Build the flows.
	 */
	//buildFlows();

	/**
	 * The target
	 */
	target = isat3_node_create_from_string(isatInstance, "omegaWheel = 20;");

	trans = isat3_node_create_from_string(isatInstance, "jumpUp and gearRear = 1 and !bigJump -> gearRear' = 2 and\n"// !bigJump' and\n"
														"jumpUp and gearFront = 1 and !bigJump -> gearFront' = 1;");// and bigJump' = false;");

	//i3_type_t result = isat3_solve_bmc(isatInstance, init, trans, target, TIMEFRAMES, TIMEFRAMES, 3140000);
	i3_type_t result = isat3_solve_bmc(isatInstance, init, trans, target, TIMEFRAMES, TIMEFRAMES, 3140000);
	int i = 0;
	for (i = 0; i < TIMEFRAMES; i++) {
		printIntervalOfVariable(isatInstance, t, i);
		printTruthValueOfVariable(isatInstance, jumpUp, i);
		printIntervalOfVariable(isatInstance, omegaWheel, i);
		printIntervalOfVariable(isatInstance, omegaWheelDot, i);
		printIntervalOfVariable(isatInstance, omegaCrank, i);
		printIntervalOfVariable(isatInstance, mode, i);
		printf("-------------------------------------------------------------------------\n");
	}
	i3_type_t tframe = isat3_get_tframe(isatInstance);
	printf("%s (in tframe %d)\n", isat3_get_result_string(result), tframe);
	return 0;
}
