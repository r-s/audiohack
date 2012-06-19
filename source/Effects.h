//
//  Effects.h
//  audiohack
//

#ifndef audiohack_Effects_h
#define audiohack_Effects_h

#include "Superclass.h"

class Effects:public Superclass {
public:
    //////////////////////////////// Marcel + Marvin
    void delay(double delayTime, double feedback, double mix);      // feedback & mix between 0 and 1
};

#endif