//
//  Dynamics.cpp
//  AudioTool
//

#include <iostream>
#include "Dynamics.h"
#include "math.h"


void Dynamics::gatePipe(bool pipe, double threshold, int attack, int release) {
    
    int attackLength = sfInfo.samplerate / attack;
    int releaseLength = sfInfo.samplerate / release;
    double threshValue = pow(10, (threshold / 10));
    
    int windowSize = (int)((attack + release) * 2);
	bool silence = false;
	
	
	for (int windowBegin = 0; windowBegin < sfInfo.frames; windowBegin += windowSize) {
		
		// die Größe des letzten Fensters muss neu kalkuliert werden beim letzten Durchgang
		if (windowBegin + windowSize >= sfInfo.frames) windowSize = sfInfo.frames - windowBegin;
        
		double currentRMS = rms(windowBegin, windowBegin + windowSize - 1);        		
		
		// die if-Bedingung ergibt true für die beiden Fälle, in denen Klang geschrieben werden soll
		if ((currentRMS < threshValue && pipe) || (currentRMS > threshValue && !pipe)) {
            
			for (int channel = 0; channel < sfInfo.channels; channel++) {
                if (!silence)
                {
                    //ab hier findet das attack bzw. fadeIn statt
                    for (int readToWrite = 0; readToWrite<attackLength; readToWrite++)
                    {
                        writeItem(readToWrite+windowBegin, channel, readItem(readToWrite+windowBegin, channel) * (readToWrite/attackLength));
                    }
                    //hier hört das attack bzw. fadeIn auf
                    
                    for (int readToWrite = 0; readToWrite<windowSize-attackLength; readToWrite++)
                    {
                        writeItem(readToWrite+windowBegin+attackLength, channel, readItem(readToWrite+windowBegin+attackLength, channel));
                    }
                    silence = false;//jetzt hört man alles wieder & kein infade mehr
                    
                }
                else
                {
                    for (int readToWrite = 0; readToWrite<windowSize; readToWrite++)
                    {
                        writeItem(readToWrite+windowBegin, channel, readItem(readToWrite+windowBegin, channel));
                    }   //man hört sowieso etwas, von anfang bis ende des fensters
                }
                
			}
		}
		else {
            
			
			for (int channel = 0; channel < sfInfo.channels; channel++) {
                
                if (silence)
                {
                    //ab hier findet das release bzw. fadeOut statt
                    for (int readToWrite; readToWrite<releaseLength; readToWrite++)
                    {
                        writeItem(readToWrite+windowBegin, channel, readItem(readToWrite+windowBegin, channel) * (-1*(readToWrite/releaseLength)+1));
                    }
                    //hier hört das release bzw. fadeOut auf
                    
                    for (int writeToZero = windowBegin+releaseLength; writeToZero < (windowBegin+windowSize); writeToZero++)
                    {
                        writeItem(writeToZero, channel, 0);
                    };
                    silence = true; //ab jetzt ist alles leise & wird alles nicht mehr ausgefadet
                }
                
                
                else
                {
                    for (int writeToZero = windowBegin; writeToZero < (windowBegin+windowSize); writeToZero++)
                    {
                        writeItem(writeToZero, channel, 0);
                    };//man hört sowieso nichts, von anfang bis ende des fensters
                }
                
            }
		}
	}
}