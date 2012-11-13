//
// Effects.cpp
// audiohack
//


#include "Effects.h"


Effects::Effects(string filePath) : Superclass(filePath){}

int Effects::delay(double delayTime, int feedback, double mix) {
    
	// Mit ReadItem die Daten lesen
	double newLength = sfInfo.frames/(double)sfInfo.samplerate + (feedback * delayTime);
	
	
	this->reallocateOutputData(sfInfo.channels, newLength); // Laenge des Ausgangsfiles + Feedback * delayTime
	
	
	double delayedSamples = delayTime * sfInfo.samplerate; //Schrittgroesse der Samples
    
    int feedbackCopy;
    double mixCopy;
    
    // Feedbackeingabe begrenzen damit nicht z.b. 1000 mal eingegeben werden kann.
	if (feedback > 100) {
        feedbackCopy = feedback; //Übergabe an neue Variable für Errormessage. s.u.
		feedback = 100;
    }
    
    if (feedback < 0) {
		return 2;
    }
    
    if (delayTime < 0) {
        return 2;
    }
    
    if (mix > 1) {
        mixCopy = mix; //Übergabe an neue Variable für Errormessage. s.u.
        mix = 1;
    }
    
    if (mix < 0) {
        return 2;
    }
    
	
	for (int channel = 0; channel < sfInfo.channels; channel++)
	{
		for (int echoCounter = 0; echoCounter <= feedback; echoCounter++) {
            
			
			//Jeden Wert für mix vor der innersten Schleife neu berechnen
			mix = pow(mix, 2);
			
            
			for (int inputFrame = 0; inputFrame < sfInfo.frames; inputFrame++) {
				
                
                this->addItem(inputFrame + delayedSamples * echoCounter, channel, this->readItem(inputFrame, channel) * mix);
				
			}
            
		}
		
	}
	
    
    
    
	this->writeFile("_delay");
    
    
    if (feedbackCopy > 100) { return 4; }
    if (mixCopy > 1) { return 4; }
    
    
	return 0; // NewLength einfach weglassen
	
	
}