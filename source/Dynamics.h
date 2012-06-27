//
//  Dynamics.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

#include "Superclass.h"

class Dynamics:public Superclass {
public:
    ///////////////////////////////// Philipp
    void gain(double gain);          // gain in dB (+/-)
    void normalize();
    ///////////////////////////////// Jann
    void gatePipe(bool gateOrPipe, double threshold, int attack, int release); // threshold in dB
    //attack- und release-zeit in ms. gateOrPipe wählt zwischen Gate oder Pipe
};

#endif