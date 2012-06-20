//
//  Superclass.cpp
//  audiohack
//

#include <iostream>
#include "Superclass.h"

using namespace std;


Superclass::Superclass(string inputFilePath) {

	inFile = sf_open(inputFilePath.c_str(), SFM_READ, &sfInfo);

    if(!inFile) {
        cout << "Not able to open " << inputFilePath << endl;
        sf_perror(NULL);
    }
    rawData = new double*[sfInfo.channels];

    for (int channel = 0; channel < sfInfo.channels; channel++) {
        rawData[channel] = new double[sfInfo.frames];
    }

    double data[BUFFER_SIZE * sfInfo.channels];
    int framesRead;
    sf_count_t currentFrame = 0;

    while (framesRead = sf_readf_double(inFile, data, BUFFER_SIZE)) {
         for (int frame = 0; frame < framesRead; frame++) {
            for(int channel = 0; channel < sfInfo.channels; channel++) {
                rawData[channel][currentFrame] = data[frame * sfInfo.channels + channel];
            }
            currentFrame++;
        }
    }
}

// processedData wird nicht gelöscht, File wird nicht beschrieben
Superclass::~Superclass() {
    for (int channel = 0; channel < sfInfo.channels; channel++) {
        delete [] rawData[channel];
    }

    delete [] rawData;
}

// double Superclass::readItem(int frame, int chan) {}
// void Superclass::writeItem(int frame, int chan, double value) {}

int Superclass::nextZeroPass(double seconds) {
	
	int totalNumFrames = rawData.getFrames;
	int zeroPass;
	int frameCount = (seconds * rawData.getSamplerate);
	bool found = false;
	
	while (!found) {
		if (rawData[0][frameCount] <= 0 && rawData[0][frameCount+1] >= 0) {
			zeroPass = frameCount;
			found = true;
		}
		
		frameCount++;
		
	}
	
	return zeroPass;
	
}
