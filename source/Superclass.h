//
//  Superclass.h
//  audiohack
//

#ifndef audiohack_Superclass_h
#define audiohack_Superclass_h

#include <iostream>
#include <string>
#include "sndfile.h"

#define BUFFER_SIZE 512

using namespace std;


class Superclass {
    
private:
    double** rawData;
    double** processedData;
    
    SNDFILE *inFile, *outFile;
protected:
    SF_INFO sfInfo;											// input; Do not change!
    SF_INFO sfInfoOut;										// output; frames and channels are set in writeFile
    string inputFilepath;									// path
    string suffix;											// FileSuffix (e.g. '.wav')

    void reallocateOutputData(int channels, double length); // deallocate and reallocate processedData

public:
    
    ///////////////////////////////// Daniel
    Superclass(string inputFilePath);                       // open File and write to rawData
    ~Superclass();                                          // write to File and close
    double readItem(int frame, int chan);                   // reads from rawData
    void writeItem(int frame, int chan, double value);      // writes to processedData
    void writeFile(int start, int stop, int channels, string outputFilePath);	// writes to a File, time in frames
    
    ///////////////////////////////// Magnus
    int nextZeroPass(double second);                        // returns Frame; - -> +
    void fadeIn(int length);                                // ramp-Tool, length in frames
    void fadeOut(int length);                               // ramp-Tool, length in frames
    void fadeIn(int length, int frame);
    void fadeOut(int length, int frame);
    
    ///////////////////////////////// Michael
    double rms(int startFrame, int endFrame, int channel);
    void invertPhase();                                     // inverts Phase
    void reverse();                                         // time-reverts whole Data
    void reverse(int startFrame, int endFrame);             // time-reverts specific Data
};

#endif
