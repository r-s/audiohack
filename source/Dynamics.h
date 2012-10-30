//
//  Dynamics.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

#include "Superclass.h"

class Dynamics:public Superclass {
private:
    double from_dB(double value);
    void gainIntern(double gain);
    
public:
    Dynamics(string filePath);
    
    void gain(double gain_dB);
    void normalize();
    ///////////////////////////////// Jann
    void gatePipe(bool gateOrPipe, double threshold, int attack, int release); // threshold in dB
    //attack- und release-zeit in ms. gateOrPipe wählt zwischen Gate oder Pipe
};

#endif