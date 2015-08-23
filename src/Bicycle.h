/*
 * Bicycle.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef BICYCLE_H_
#define BICYCLE_H_

#include "math.h"
#include <cstdlib>

using namespace std;

#define FPEDAL		(double)200
#define FPEDALLOW	(double)100
#define FPEDALHIGH	(double)200
#define RPEDAL		(double)0.09
#define MU			(double)0.008
#define RWHEEL		(double)0.305
#define M			(double)70
#define G			(double)9.81
#define CINV		(double)0.8
#define C1			(FPEDAL * ((RPEDAL) / (pow(RWHEEL, 2) * M)))
#define C1_1		(RPEDAL) / (pow(RWHEEL, 2) * M)
#define C2			(MU * G) / RWHEEL

#define TARGET 		(double)100
#define DEVIATION 	(double)10
#define ERROR		(double)10

#define TARGETUPPEROMEGA 	((TARGET + DEVIATION) * 2 * M_PI) / 60.0f
#define TARGETLOWEROMEGA 	((TARGET - DEVIATION) * 2 * M_PI) / 60.0f
#define UPPERLIMIT			TARGETUPPEROMEGA * (1 + ERROR / 100.0f)
#define LOWERLIMIT			TARGETLOWEROMEGA * (1 - ERROR / 100.0f)


#define TEETHF1 	(double) 34
#define TEETHF2 	(double) 50

#define TEETHR1 	(double) 25
#define TEETHR2 	(double) 23
#define TEETHR3 	(double) 21
#define TEETHR4 	(double) 19
#define TEETHR5 	(double) 17
#define TEETHR6 	(double) 16
#define TEETHR7 	(double) 15
#define TEETHR8 	(double) 14
#define TEETHR9 	(double) 13
#define TEETHR10 	(double) 12

enum ShiftSequence {FOUR_ONE, THREE_TWO, TWO_ONE_TWO};
struct Scenario {ShiftSequence shiftSequence;
				 bool 	modelTimeBehavior;
				 double fPedMin;
				 double fPedMax;
				 double targetRPM;
				 double deviation;
				 double error;
				 string target;};

#endif /* BICYCLE_H_ */
