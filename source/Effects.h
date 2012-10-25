//
//  Effects.h
//  audiohack
//

#ifndef audiohack_Effects_h
#define audiohack_Effects_h

#include "Superclass.h"

class Effects:public Superclass {
public:
    Effects(string filePath);
    //////////////////////////////// Marcel + Marvin
    int delay(double delayTime, int feedback, double mix);      // feedback & mix between 0 and 1
};

#endif