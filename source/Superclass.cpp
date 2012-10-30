//
//  Superclass.cpp
//  audiohack
//

#include "Superclass.h"

using namespace std;

// Daniel
Superclass::Superclass(string filePath) {  // Konstruktor

	(*this).inputFilePath = filePath;  // set Class variable inputFilePath
	inFile = sf_open(filePath.c_str(), SFM_READ, &sfInfo);
    if(!inFile) {
        cout << "Not able to open " << filePath << endl;
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
    // Bernd
	sfInfoOut.frames = sfInfo.frames;
	sfInfoOut.samplerate = sfInfo.samplerate;
	sfInfoOut.channels = sfInfo.channels;
	sfInfoOut.format = sfInfo.format;
	sfInfoOut.sections= sfInfo.sections;
	sfInfoOut.seekable = sfInfo.seekable;	
}

Superclass::~Superclass() {  // Destruktor
    for (int channel = 0; channel < sfInfo.channels; channel++) { // Arrays löschen
        delete [] rawData[channel];
    }
    delete [] rawData;
    for (int channel = 0; channel < sfInfoOut.channels; channel++) {
    	delete [] processedData[channel];
    }
    delete [] processedData;
}

double Superclass::readItem(int frame, int chan) {
	bool error = false;  // Debug für uns

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
	// Debug für uns
	if(chan >= sfInfoOut.channels || chan < 0) {
		cout << "Channel-Angabe falsch!\n";
	}
	if (frame >= sfInfoOut.frames || frame < 0) {
		cout << "Frame-Angabe falsch!\n";
	}
	processedData[chan][frame] = value;
}
// Bernd
void Superclass::addItem(int frame, int chan, double value) {
	// Debug für uns
	if(chan >= sfInfoOut.channels || chan < 0) {
		cout << "Channel-Angabe falsch!\n";
	}
	if (frame >= sfInfoOut.frames || frame < 0) {
		cout << "Frame-Angabe falsch!\n";
	}
	processedData[chan][frame] += value;
}

// Daniel
void Superclass::writeFile(string insertion, int start, int stop, int channels) {
	size_t pos = inputFilePath.find_last_of(".");
	string outputFilePath = inputFilePath.insert(pos, insertion);

	sf_count_t frameSum = stop - start;
	sfInfoOut.frames = frameSum;
	sfInfoOut.channels = channels;
	SF_INFO copy = sfInfoOut;
	outFile = sf_open(outputFilePath.c_str(), SFM_WRITE, &copy);

	double *processedDataInterleaved = new double[frameSum * sfInfoOut.channels];
	int item = 0;
	for (int frame = 0; frame < frameSum; frame++) {
		for (int channel = 0; channel < sfInfoOut.channels; channel++) {
			processedDataInterleaved[item++] = processedData[channel][start + frame];
		}
	}

	sf_writef_double(outFile, processedDataInterleaved, frameSum);
	sf_close(inFile);
	sf_close(outFile);
    delete [] processedDataInterleaved;
}

void Superclass::writeFile(string insertion) {
	size_t pos = inputFilePath.find_last_of(".");
	string outputFilePath = inputFilePath.insert(pos, insertion);
	SF_INFO copy = sfInfoOut;
	outFile = sf_open(outputFilePath.c_str(), SFM_WRITE, &copy);

	double *processedDataInterleaved = new double[sfInfoOut.frames * sfInfoOut.channels];
	int item = 0;
	for (int frame = 0; frame < sfInfoOut.frames; frame++) {
		for (int channel = 0; channel < sfInfoOut.channels; channel++) {
			processedDataInterleaved[item++] = processedData[channel][frame];
		}
	}

	sf_writef_double(outFile, processedDataInterleaved, sfInfoOut.frames);
	sf_close(inFile);
	sf_close(outFile);
    delete [] processedDataInterleaved;
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
	
	this->fadeIn(length, 0);
	
}

//Magnus
void Superclass::fadeOut(int length) {
	
	this->fadeOut(length, (sfInfo.frames - length));
	
}

//Magnus
void Superclass::fadeIn(int length, int frame) {
	
	for (int channel = 0; channel < sfInfo.channels; channel++) {
		for (int frameCount = 0; frameCount < sfInfo.frames; frameCount++) {
			
			//Fade
			if (frame <= frameCount && frameCount <= length + frame) {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel) * (frameCount-frame) / (double)length);
				
				//Normal	
			} else {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel));
				
			};
		}
	}
	this->writeFile("_fadeIn");
}

//Magnus
void Superclass::fadeOut(int length, int frame) {
	
	for (int channel = 0; channel < sfInfo.channels; channel++) {
		for (int frameCount = 0; frameCount < sfInfo.frames; frameCount++) {
			
			//Fade
			if (frame <= frameCount && frameCount <= length + frame) {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel) * (1 - (frameCount-frame) / (double)length));
				
				//Normal	
			} else {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel));
				
			};
		}
	}
	this->writeFile("_fadeOut");
}

//Michael
//rawData wird rückwärts in processedData geschrieben
void Superclass::reverse()
{		
    int num = (int)sfInfo.frames;
    
    int chan = sfInfo.channels;
	
    for ( int channel = 0; channel <= chan; channel++) {
        
        for (int item = 0; item <= num; item++) {
			processedData[channel][item] = rawData[channel][num - item];	// Fehler!
		}
		
    }
}

//rawData wird rückwärts in processedData mit bestimmten Start und Endwerten geschrieben
void Superclass::reverse(int startFrame, int endFrame)
{
    int chan = sfInfo.channels;
    
    if (startFrame > endFrame){
        cout << "startFrame larger than endFrame" << endl;
    } else {
		
        for ( int channel = 0; channel <= chan; channel++) {
			
            for (int startframe = 0; startframe <= endFrame; startFrame++) {
                processedData[channel][startFrame] = rawData[channel][endFrame - startFrame];
            }
        }
    }
}

void Superclass::invertPhase()
{		
    int num = (int)sfInfo.frames;
    
    int chan = sfInfo.channels;
    
    for ( int i = 0; i <= chan; i++) {
        
        for (int item = 0; item <= num; item++) {
			processedData[i][item] = rawData[i][item] * -1;
		}
		
    }
}

//rms-wert: √( ( 1 / anzahlsamples ) * ( alle samples^2 addiert) )
double Superclass::rms(int startFrame, int endFrame, int channel)
{
    int counter = 0;
    int framecounter = 0;
    double rms;
    if (channel >= sfInfo.channels){
        return 0;
    } else {
		
		for (startFrame = 0; startFrame <= endFrame; startFrame++) {
			counter = counter + (pow((rawData[channel][startFrame]),2));
			framecounter++;
		}
    }
	
    rms = sqrt((1 / framecounter) * counter);
    return rms;
}


//Bernd
void Superclass::reallocateOutputData(int channels, double length) { // deallocate and reallocate processedData

	for (int channel = 0; channel < sfInfo.channels; channel++) {
    	delete [] processedData[channel];
    }
    delete [] processedData;
	
	sfInfoOut.channels = channels;
	sfInfoOut.frames = (sf_count_t)(length * sfInfo.samplerate);
	
    processedData = new double*[channels];

    for (int channel = 0; channel < channels; channel++) {
        processedData[channel] = new double[sfInfoOut.frames];
    }
}
