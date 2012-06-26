//
//  Dynamics.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

#include "Superclass.h"

class Dynamics : public Superclass {
public:
    ///////////////////////////////// Philipp
    void gain(double gain);          // gain in dB (+/-)
    void normalize();
    ///////////////////////////////// Jann
    void gatePipe(double threshold); // threshold in dB (+ = pipe, - = gate)
};

#endif