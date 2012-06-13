//
//  Time.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

#include "Superclass.h"

class Time:public Superclass {
public: 
    ///////////////////////////////// Simon + Tobias
    void cutAtTime(double second);
    bool cutAtContent(double second, double tolerance); // tolerance in seconds; if tolerance > second -> return false
};

#endif