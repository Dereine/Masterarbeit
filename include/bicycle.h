/*
 * bicycle.h
 *
 *  Created on: 08.06.2015
 *      Author: johannes
 */

#ifndef BICYCLE_H_
#define BICYCLE_H_

#include <isat3.h>
#include <isat3types.h>

struct isat3 *isatInstance;
struct isat3_node *omegaWheel;
struct isat3_node *omegaCrank;
struct isat3_node *omegaWheel;
struct isat3_node *omegaWheelDot;
struct isat3_node *mode;
struct isat3_node *target;
struct isat3_node *deviation;
struct isat3_node *rF1;
struct isat3_node *rF2;
struct isat3_node *rR1;
struct isat3_node *rR2;
struct isat3_node *rR3;
struct isat3_node *rR4;
struct isat3_node *rR5;
struct isat3_node *rR6;
struct isat3_node *rR7;
struct isat3_node *rR8;
struct isat3_node *rR9;
struct isat3_node *rR10;

/*
 * Defines used in the problem.
 */
struct isat3_node *rF1Define;
struct isat3_node *rF2Define;
struct isat3_node *rR1Define;
struct isat3_node *rR2Define;
struct isat3_node *rR3Define;
struct isat3_node *rR4Define;
struct isat3_node *rR5Define;
struct isat3_node *rR6Define;
struct isat3_node *rR7Define;
struct isat3_node *rR8Define;
struct isat3_node *rR9Define;
struct isat3_node *rR10Define;

// Variables
struct isat3_node *gearFront;
struct isat3_node *gearRear;
struct isat3_node* bigJump;


struct isat3_node *c1;
struct isat3_node *c2;
struct isat3_node *cInv;
struct isat3_node *cInvDefine;
struct isat3_node *c1Define;
struct isat3_node *c2Define;

/* Not used at the moment because they are regarded constant
struct isat3_node *fPed;
struct isat3_node *rPed;
struct isat3_node *m;
struct isat3_node *mu;
struct isat3_node *g;
struct isat3_node *rWheel;

struct isat3_node *fPedDefine;
struct isat3_node *rPedDefine;
struct isat3_node *mDefine;
struct isat3_node *muDefine;
struct isat3_node *gDefine;
struct isat3_node *rWheelDefine;
*/

struct isat3_node *init;
struct isat3_node *trans;

struct isat3_node *tS;
struct isat3_node *tSDefine;

struct isat3_node *t;
struct isat3_node *tDefine;

struct isat3_node *targetUpper;
struct isat3_node *targetLower;

struct isat3_node *targetUpperDefine;
struct isat3_node *targetLowerDefine;

struct isat3_node *constantZero;
struct isat3_node *constantZeroDefine;

struct isat3_node *jumpUp;
struct isat3_node *jumpDown;
//struct isat3_node *jumpFail;

struct isat3_node *state;

//struct mode {int gearFront; int gearRear;}

#define FPEDAL (float)70
#define RPEDAL (float)0.015
#define MU (float)0.015
#define RWHEEL (float)0.622
#define M (float)70
#define G (float)9.81
#define TS (float) 1.0
#define CINV (float)0.8
#define TARGET 10
#define DEVIATION 5
#define TIMEFRAMES 200

enum upOrDown {UP = 1, DOWN = 0};
enum bigOrNot {BIGJUMP = 1, NOBIGJUMP = 0};

void setUpVariablesAndConstants();
struct isat3_node* addTransition(int gearFrontSource, int gearRearSource, int gearFrontDest, int gearRearDest, int bigJumpPrem, int bigJumpConc, int upOrDown, struct isat3 *isatInstance);


#endif /* BICYCLE_H_ */
