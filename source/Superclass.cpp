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
	bool error = false;  // ausfuehrliche Debug-Meldungen für uns

	if (chan >= sfInfo.channels) {
	    cout << "readItem: Channel-Angabe zu hoch: " << chan << endl;
	    error = true;
	}
	if (chan < 0) {
		cout << "readItem: Channel-Angabe negativ: " << chan << endl;
		error = true;
	}
	if (frame >= sfInfo.frames) {
	    cout << "readItem: Frame-Angabe zu hoch: " << frame << endl;
	    error = true;
	}
	if (frame < 0) {
		cout << "readItem: Frame-Angabe negativ: " << frame << endl;
		error = true;
	}
	if (error == 1) {
	    return 0;
	}
	//cout << "readItem: value: " << rawData[chan][frame] << endl; // moegliche zusaetzliche Ausgabe
	return rawData[chan][frame];
}

void Superclass::writeItem(int frame, int chan, double value) {
	// ausfuehrliche Debug-Meldungen für uns

	if (chan >= sfInfo.channels) {
	    cout << "writeItem: Channel-Angabe zu hoch: " << chan << endl;
	}
	if (chan < 0) {
		cout << "writeItem: Channel-Angabe negativ: " << chan << endl;
	}
	if (frame >= sfInfo.frames) {
	    cout << "writeItem: Frame-Angabe zu hoch: " << frame << endl;
	}
	if (frame < 0) {
		cout << "writeItem: Frame-Angabe negativ: " << frame << endl;
	}
	//cout << "writeItem: value: " << value << endl; // moegliche zusaetzliche Ausgabe

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

sf_count_t Superclass::getFrames() {
	return sfInfo.frames;
}

int Superclass::getSampleRate() {
	return sfInfo.samplerate;
}

int Superclass::getChannels() {
	return sfInfo.channels;
}


// Daniel
void Superclass::writeFile(string insertion, int start, int stop, int channels) {
	size_t pos = inputFilePath.find_last_of(".");
	string outputFilePath = inputFilePath.c_str();
	outputFilePath.insert(pos, insertion);

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
    cout << "Your file has been succesfully written to " << outputFilePath << endl;
}

void Superclass::writeFile(string insertion) {
	this->writeFile(insertion, 0, sfInfoOut.frames, sfInfoOut.channels);
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
void Superclass::fadeIn(double length) {
	
	this->fadeIn(length, 0);
    this->writeFile("_fadeIn");
	
}

//Magnus
void Superclass::fadeOut(double length) {
    
    int lengthInFrames = (int)(length * sfInfo.samplerate);
	
	this->fadeOut(length, (sfInfo.frames - lengthInFrames));
    this->writeFile("_fadeOut");
	
}

//Magnus
void Superclass::fadeIn(double length, int frame) {
	
	int lengthInFrames = (int)(length * sfInfo.samplerate);
	
	for (int channel = 0; channel < sfInfo.channels; channel++) {
		for (int frameCount = 0; frameCount < sfInfo.frames; frameCount++) {
			
			//Fade
			if (frame <= frameCount && frameCount <= lengthInFrames + frame) {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel) * (frameCount-frame) / (double)lengthInFrames);
				
				//Normal	
			} else {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel));
				
			};
		}
	}
}

//Magnus
void Superclass::fadeOut(double length, int frame) {
    
    int lengthInFrames = (int)(length * sfInfo.samplerate);
	
	for (int channel = 0; channel < sfInfo.channels; channel++) {
		for (int frameCount = 0; frameCount < sfInfo.frames; frameCount++) {
			
			//Fade
			if (frame <= frameCount && frameCount <= lengthInFrames + frame) {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel) * (1 - (frameCount-frame) / (double)lengthInFrames));
				
				//Normal	
			} else {
				
				this->writeItem(frameCount, channel, this->readItem(frameCount, channel));
				
			};
		}
	}
}

//Michael
//rawData wird rückwärts in processedData mit bestimmten Start und Endwerten geschrieben
void Superclass::reverse(int startFrame, int endFrame)
{
    
    int chan = sfInfo.channels;
    if (startFrame > endFrame){
        cout << "startFrame larger than endFrame" << endl;
    } else {
		
        for ( int channel = 0; channel < chan; channel++) {
			
            for (int start = startFrame; start < endFrame; start++) {
                this->writeItem(start, channel, this->readItem(endFrame - start, channel));
            }
        }
    }
}

//rawData wird rückwärts in processedData geschrieben
void Superclass::reverse()
{	
	int num = (int)sfInfo.frames; 
    this->reverse(0, num - 1);
    
    this->writeFile("_rev");
}




void Superclass::invertPhase()
{		
    int num = (int)sfInfo.frames;
    
    int chan = sfInfo.channels;
    
    for ( int channel = 0; channel < chan; channel++) {
        
        for (int item = 0; item < num; item++) {
			this->writeItem(item, channel, this->readItem(item, channel) * -1);
		}
		
    }
	this->writeFile("_phase");
}

//rms-wert: √( ( 1 / anzahlsamples ) * ( alle samples^2 addiert) )
double Superclass::rms(int startFrame, int endFrame, int channel)
{
    double counter = 0;
    if (channel >= sfInfo.channels or channel < 0 or endFrame <= startFrame){
        return 0;
    } else {
		
		for (int frame = 0;frame < endFrame; frame++) {
			counter += pow(this->readItem(startFrame + frame, channel),2);
		}
    }
	
    double rms = sqrt((1.0 / (endFrame-startFrame)) * counter);
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
