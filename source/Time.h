//
//  Time.h
//  audiohack
//

#ifndef audiohack_Time_h
#define audiohack_Time_h

#include "Superclass.h"

class Time:public Superclass {
public: 
    ///////////////////////////////// Simon + Tobias
    void cutAtTime(double second);
    bool cutAtContent(double second, double tolerance); // tolerance in seconds; if tolerance > second -> return false
};

#endif