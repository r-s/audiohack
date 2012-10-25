//
//  Effects.cpp
//  audiohack
//


#include "Effects.h"


Effects::Effects(string filePath) : Superclass(filePath){}

int Effects::delay(double delayTime, int feedback, double mix) {
    
	// Mit ReadItem die Daten lesen
	double newLength = sfInfo.frames/(double)sfInfo.samplerate + (feedback * delayTime);
	
	
	this->reallocateOutputData(sfInfo.channels, newLength); // Laenge des Ausgangsfiles + Feedback * delayTime
	
	
	double delayedSamples = delayTime * sfInfo.samplerate;  //Schrittgroesse der Samples
    
    // Feedbackeingabe begrenzen damit nicht z.b. 1000 mal eingegeben werden kann.
	if (feedback > 100) {
            feedback = 100;
            cout << "Error: Maximal mögliche Eingabe beträgt 100! Die Eingabe wurde auf 100 reduziert." << endl;
    }
    
    if (feedback < 0) {
            feedback = 0;
            cout << "Error: Eingabewert ist negativ! Die Eingabe wurde auf 0 gesetzt." << endl;
    }
    
   
	for (int channel = 0; channel == sfInfo.channels; channel++)
	{
		for (int echoCounter = 0; echoCounter < feedback; echoCounter++) {
            
			
			//Jeden Wert für mix vor der innersten Schleife neu berechnen
			mix = pow(mix, echoCounter);
			
			
            
			for (int inputFrame = 0; inputFrame < sfInfo.frames; inputFrame++) {
                
                addItem(inputFrame + delayedSamples, channel, readItem(inputFrame, channel) * mix);
				
			}
            
		}
		
	} 
	return newLength;
}