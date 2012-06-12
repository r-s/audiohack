//
//  Superclass.h
//  audiohack
//

#ifndef audiohack_Superclass_h
#define audiohack_Superclass_h

using namespace std;

class Superclass {
    
private:
    double** rawData;
    double** processedData;
    
public:
    
    /////////////////////////////////
    Superclass(string inputFilePath);                       // open File and write to rawData
    ~Superclass();                                          // write to File and close
    double readItem(int frame, int chan);                   // reads from rawData
    void writeItem(int frame, int chan, double value);      // writes to processedData
    
    ///////////////////////////////// Magnus
    void fadeIn(int length);                                // ramp-Tool
    void fadeOut(int length);                               // ramp-Tool
    
    ///////////////////////////////// Michael
    double rms(int startFrame, int endFrame);
    void invertPhase();                                     // inverts Phase
    void reverse();                                         // time-reverts Data
};

#endif