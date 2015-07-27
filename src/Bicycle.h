/*
 * Bicycle.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef BICYCLE_H_
#define BICYCLE_H_

#include "math.h"

#define FPEDAL		(float)70
#define RPEDAL		(float)0.015
#define MU			(float)0.015
#define RWHEEL		(float)0.622
#define M			(float)70
#define G			(float)9.81
#define TS			(float) 1.0
#define CINV		(float)0.8
#define C1			(FPEDAL * ((RPEDAL) / (pow(RWHEEL, 2) * M)))
#define C2			(-1 * MU * G) / RWHEEL

#define TARGET 		10
#define DEVIATION 	5
#define TIMEFRAMES 	200

#define TEETHF1 	34
#define TEETHF2 	50

#define TEETHR1 	25
#define TEETHR2 	23
#define TEETHR3 	21
#define TEETHR4 	19
#define TEETHR5 	17
#define TEETHR6 	16
#define TEETHR7 	15
#define TEETHR8 	14
#define TEETHR9 	13
#define TEETHR10 	12


#endif /* BICYCLE_H_ */
