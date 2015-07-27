/*
 * Guard.cpp
 *
 *  Created on: 27.07.2015
 *      Author: johannes
 */

#include "Guard.h"

Guard::Guard() {
	// TODO Auto-generated constructor stub

}

Guard::Guard(vector<LinearPredicate> linPreds) :
		LinearPredicateConjunct(linPreds){}

Guard::~Guard() {
	// TODO Auto-generated destructor stub
}

