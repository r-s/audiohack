//
//  Dynamics.h
//  audiohack
//

#ifndef audiohack_Dynamics_h
#define audiohack_Dynamics_h

class Dynamics:public Superclass {
public:
    /////////////////////////////////
    gain(double gain);          // gain in dB (+/-)
    normalize();
    ///////////////////////////////// Jann
    gatePipe(double threshold); // threshold in dB (+ = pipe, - = gate)
};

#endif
