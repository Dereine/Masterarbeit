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

void Guard::toStringSpaceExXML(string& guardString) {
	guardString += "      <guard> ";
	bool firstElement = true;
	LinearPredicate linPred;
	for (size_t i = 0; i < _linPreds.size(); i++) {
		linPred = _linPreds[i];
		if (!linPred.isFlow()) {
			if (firstElement) {
				guardString += linPred.toStringSpaceExXML(false, false) + "\n";
				firstElement = false;
			} else {
				guardString +=  "&amp;" +
				linPred.toStringSpaceExXML(false, false) + "\n";
			}
		}
	}
	guardString += "      </guard> ";
}
