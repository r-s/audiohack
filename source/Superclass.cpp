//
//  Superclass.cpp
//  audiohack
//

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

//sfInfo.frames aktualisieren falls processedData abweichend von rawData oder zweite Instanz?
Superclass::~Superclass() {  // Destruktor

	// Arrays löschen
    for (int channel = 0; channel < sfInfo.channels; channel++) {
        delete [] rawData[channel];
    }
    delete [] rawData;
    for (int channel = 0; channel < sfInfo.channels; channel++) {
    	delete [] processedData[channel];
    }
    delete [] processedData;
}

double Superclass::readItem(int frame, int chan) {
	bool error = false;  // Fehlerprüfung nur für uns, wird am Ende entfernt

	if (chan >= sfInfo.channels || chan < 0) {
	    cout << "Channel-Angabe falsch!\n";
	    error = true;
	}
	if (frame >= sfInfo.frames || frame < 0) {
	    cout << "Frame-Angabe falsch!\n";
	    error = true;
	}
	if (error) {
	    return 0;
	}
	return rawData[chan][frame];
}

void Superclass::writeItem(int frame, int chan, double value) {
	// Fehlerprüfung nur für uns, wird am Ende entfernt
	if(chan >= sfInfo.channels || chan < 0) {
		cout << "Channel-Angabe falsch!\n";
	}
	if (frame >= sfInfo.frames || frame < 0) {
		cout << "Frame-Angabe falsch!\n";
	}
	processedData[chan][frame] = value;
}

void Superclass::writeFile(string outputFilePath) {
		sf_count_t frameSum = sfInfo.frames;
		outFile = sf_open(outputFilePath.c_str(), SFM_WRITE, &sfInfo);

		double *processedDataInterleaved = new double[frameSum * sfInfo.channels];
		int item = 0;

		for (int frame = 0; frame < frameSum; frame++) {
			for (int channel = 0; channel < sfInfo.channels; channel++) {
				processedDataInterleaved[item++] = processedData[channel][frame];
			}
		}

		sf_writef_double(outFile, processedDataInterleaved, frameSum);
		sf_close(inFile);
		sf_close(outFile);
}

int Superclass::nextZeroPass(double seconds) {

	int totalNumFrames = sfInfo.frames; // korrigiert
	int zeroPass;
	int frameCount = (seconds * sfInfo.samplerate);  // korrigiert
	bool found = false;
	
	try {
		seconds * sfInfo.samplerate <= totalNumFrames;
	}
	catch ( ... ) {
		cout << "Soundfile is only " << totalNumFrames*sfInfo.samplerate << " seconds long. \n";
	}

	while (!found) {
		if (rawData[0][frameCount] <= 0 && rawData[0][frameCount+1] >= 0) {
			zeroPass = frameCount;
			found = true;
		}
		frameCount++;
	}
	    return zeroPass;
}

void Superclass::fadeIn(int length) {
	int frameCount = 0;
	int channelCount = 0;
	
	for (channelCount; channelCount == sfInfo.channels; channelCount++) {
		for (frameCount; frameCount == sfInfo.frames; frameCount++) {
			if (frameCount <= length) {
				processedData[channelCount][frameCount] = rawData[channelCount][frameCount] * (double)( frameCount / length );
			} else {
				processedData[channelCount][frameCount] = rawData[channelCount][frameCount];
			};
		}
	}
};

void Superclass::fadeOut(int length) {
	int channelCount = 0;
	int frameCount = 0;
	int fadeStart = sfInfo.frames - length;
	int fadeCount = 0;
	
	for (channelCount; channelCount == sfInfo.channels; channelCount++) {
		for (frameCount; frameCount == sfInfo.frames; frameCount++) {
			if (frameCount < fadeStart) {
				processedData[channelCount][frameCount] = rawData[channelCount][frameCount];
			} else if (frameCount >= fadeStart) {
				processedData[channelCount][frameCount] = rawData[channelCount][frameCount] * (double)( fadeCount / length );
				fadeCount++;
			}
		}
	}
	
};
