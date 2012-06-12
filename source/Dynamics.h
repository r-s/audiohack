//
//  Dynamics.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

class Dynamics:public Superclass {
public:
    /////////////////////////////////
    void gain(double gain);          // gain in dB (+/-)
    void normalize();
    ///////////////////////////////// Jann
    void gatePipe(double threshold); // threshold in dB (+ = pipe, - = gate)
};

#endif
