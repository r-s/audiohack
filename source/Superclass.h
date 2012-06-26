//
//  Superclass.h
//  audiohack
//

#ifndef audiohack_Superclass_h
#define audiohack_Superclass_h

#include <string>
#include "sndfile.h"

#define BUFFER_SIZE 512

using namespace std;


class Superclass {
    
private:
    double** rawData;
    double** processedData;
    
    SNDFILE *inFile, *outFile;
    SF_INFO sfInfo;

public:
    
    ///////////////////////////////// Daniel
    Superclass(string inputFilePath);                       // open File and write to rawData
    ~Superclass();                                          // write to File and close
    double readItem(int frame, int chan);                   // reads from rawData
    void writeItem(int frame, int chan, double value);      // writes to processedData
    
    ///////////////////////////////// Magnus
    int nextZeroPass(double second);                        // returns Frame; - -> +
    void fadeIn(int length);                                // ramp-Tool, length in frames
    void fadeOut(int length);                               // ramp-Tool, length in frames
    
    ///////////////////////////////// Michael
    double rms(int startFrame, int endFrame);
    void invertPhase();                                     // inverts Phase
    void reverse();                                         // time-reverts whole Data
    void reverse(int startFrame, int endFrame);             // time-reverts specific Data
};

#endif
