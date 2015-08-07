/*
 * Bicycle.h
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#ifndef BICYCLE_H_
#define BICYCLE_H_

#include "math.h"

#define FPEDAL		(double)70
#define RPEDAL		(double)0.015
#define MU			(double)0.015
#define RWHEEL		(double)0.622
#define M			(double)70
#define G			(double)9.81
#define TS			(double) 1.0
#define CINV		(double)0.8
#define C1			(FPEDAL * ((RPEDAL) / (pow(RWHEEL, 2) * M)))
#define C2			(-1 * MU * G) / RWHEEL

#define TARGET 		10
#define DEVIATION 	8


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


#endif /* BICYCLE_H_ */
