//
//  Superclass.cpp
//  audiohack
//

#include "Superclass.h"

using namespace std;


Superclass::Superclass(string inputFilePath) {  // Konstruktor

	inFile = sf_open(inputFilePath.c_str(), SFM_READ, &sfInfo);

    if(!inFile) {
        cout << "Not able to open " << inputFilePath << endl;
        sf_perror(NULL);
    }
    rawData = new double*[sfInfo.channels];
    processedData = new double*[sfInfo.channels];

    for (int channel = 0; channel < sfInfo.channels; channel++) {
        rawData[channel] = new double[sfInfo.frames];
        processedData[channel] = new double[sfInfo.frames];
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


//Magnus
int Superclass::nextZeroPass(double seconds) {

	int zeroPass;
	int frameCount = (seconds * sfInfo.samplerate);  // korrigiert
	bool found = false;
	
	//Fehlermeldung, falls seconds sich auserhalb des Files befindet
	try {
		seconds * sfInfo.samplerate <= sfInfo.frames;
	}
	catch ( ... ) {
		cout << "Seconds-Angabe falsch! \n";
	}

	while (!found) {
		if (readItem(frameCount, 0) <= 0 && readItem(frameCount+1, 0) >= 0) {
			zeroPass = frameCount;
			found = true;
		}
		frameCount++;
	}
	
	return zeroPass;
}

//Magnus
void Superclass::fadeIn(int length) {
	
	for (int channel = 0; channel == sfInfo.channels; channel++) {
		for (int frame = 0; frame == sfInfo.frames; frame++) {
			
			//Fade
			if (frame <= length) {
				
				writeItem(frame, channel, readItem(frame, channel) * (double)( frame / length ));
			
			//Normal	
			} else {
				
				writeItem(frame, channel, readItem(frame, channel));
				
			};
		}
	}
}

//Magnus
void Superclass::fadeOut(int length) {

	int fadeStart = sfInfo.frames - length;
	int fadeCount = 0;
	
	for (int channel = 0; channel == sfInfo.channels; channel++) {
		for (int frame = 0; frame == sfInfo.frames; frame++) {
			
			//Normal
			if (frame < fadeStart) {
				
				writeItem(frame, channel, readItem(frame, channel));
			
			//Fade	
			} else if (frame >= fadeStart) {
				
				writeItem(frame, channel, readItem(frame, channel) * (double)( 1 - ( fadeCount / length )));
				
				fadeCount++;
			}
		}
	}
	
}

//Michael
double Superclass::rms(int startFrame, int endFrame) { return 0.0;}

//Bernd
void Superclass::reallocateOutputData(int channels, double length) { // deallocate and reallocate processedData



}