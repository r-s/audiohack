//
//  Superclass.cpp
//  audiohack
//

#include <iostream>
#include "Superclass.h"

using namespace std;


Superclass::Superclass(string inputFilePath) {  // Konstruktor

	inFile = sf_open(inputFilePath.c_str(), SFM_READ, &sfInfo); //sf_open verlangt c-string

    if(!inFile) {
        cout << "Not able to open " << inputFilePath << endl;
        sf_perror(NULL);
    }
    rawData = new double*[sfInfo.channels];
    processedData = new double*[sfInfo.channels];

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

// File wird noch nicht beschrieben!
Superclass::~Superclass() {  // Destruktor
    for (int channel = 0; channel < sfInfo.channels; channel++) { // Arrays löschen
        delete [] rawData[channel];
    }
    delete [] rawData;
    for (int channel = 0; channel < sfInfo.channels; channel++) {
    	delete [] processedData[channel];
    }
    delete [] processedData;
}

double Superclass::readItem(int frame, int chan) {
	bool error = false;  // Fehlerprüfung nur für uns, wird noch entfernt

	if (chan >= sfInfo.channels || chan < 0) {
	    cout << "Channel-Angabe falsch!\n";
	    error = true;
	}
	if (frame >= sfInfo.frames || frame < 0) {
	    cout << "Frame-Angabe falsch!\n";
	    error = true;
	}
	if (error == 1) {
	    return 0;
	}
	return rawData[chan][frame];
}

void Superclass::writeItem(int frame, int chan, double value) {
	// Fehlerprüfung nur für uns, wird noch entfernt
	if(chan >= sfInfo.channels || chan < 0) {
		cout << "Channel-Angabe falsch!\n";
	}
	if (frame >= sfInfo.frames || frame < 0) {
		cout << "Frame-Angabe falsch!\n";
	}
	processedData[chan][frame] = value;
}
