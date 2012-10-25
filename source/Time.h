//
//  Time.h
//  audiohack
//

#ifndef audiohack_Time_h
#define audiohack_Time_h

#include "Superclass.h"

class Time : public Superclass {
public:
    Time(string filePath);
    ///////////////////////////////// Simon + Tobias
    int cutAtTime(double second);
    bool cutAtContent(double second, double tolerance); // tolerance in seconds; if tolerance > second -> return false
    	// fällt vorerst weg
};

#endif
