//
//  Dynamics.cpp
//  audiohack
//

#include "Dynamics.h"


Dynamics::Dynamics(string filePath) : Superclass(filePath){}


double Dynamics::from_dB(double value)
{
    return pow(10.0, value/10.0);
}


void Dynamics::gainIntern(double gainFactor)
{
    for (int frame=0; frame<sfInfo.frames; frame++)
    {
        for (int channel=0; channel<sfInfo.channels; channel++)
        {
            writeItem(frame, 
                      channel,
                      readItem(frame,
                               channel)*gainFactor);
        }
    }
}


void Dynamics::gain(double gain_dB)
{
    double gainFactor = from_dB(gain_dB);
    gainIntern(gainFactor);
    this->writeFile("_gain");
}


void Dynamics::normalize()
{
    double max = 0.0;
    
    for (int frame=0; frame<sfInfo.frames; frame++)
    {
        for (int channel=0; channel<sfInfo.channels; channel++)
        {
            double value = abs(readItem(frame,
                                        channel));
            if (max < value)
            {
                max = value;
            }
        }
    }
    gainIntern(1.0/max);
    this->writeFile("_norm");
}



void Dynamics::gatePipe(bool pipe, double threshold, int attack, int release) {
    
    int attackLength = sfInfo.samplerate / (attack * 1000);
    int releaseLength = sfInfo.samplerate / (release * 1000);
    double threshValue = pow(10, (threshold / 10));
    
    double windowSize = (double)((attackLength + releaseLength) * 2);
	bool silence = false;
	
	
	for (int windowBegin = 0; windowBegin < sfInfo.frames; windowBegin = windowBegin + windowSize) {
		double currentRMS = (double)rand()/RAND_MAX; //nur zur ueberpruefung
		// die Größe des letzten Fensters muss neu kalkuliert werden beim letzten Durchgang
		if (windowBegin + windowSize >= sfInfo.frames) {windowSize = sfInfo.frames - windowBegin;}
        
		//double currentRMS = this->rms(windowBegin, windowBegin + windowSize - 1, 0);      // hier muss noch channel-spezifisch ausgewertet werden
		
        cout << windowBegin;
		// die if-Bedingung ergibt true für die beiden Fälle, in denen Klang geschrieben werden soll
		if ((currentRMS < threshValue && pipe) || (currentRMS > threshValue && !pipe)) {
            
			for (int channel = 0; channel < sfInfo.channels; channel++) {
                if (!silence)
                {
                    //ab hier findet das attack bzw. fadeIn statt
                    for (int readToWrite = 0; readToWrite<attackLength; readToWrite++)
                    {
                        this->writeItem(readToWrite+windowBegin, channel, this->readItem(readToWrite+windowBegin, channel) * (readToWrite/attackLength));
                    }
                    //hier hört das attack bzw. fadeIn auf
                    
                    for (int readToWrite = 0; readToWrite<windowSize-attackLength; readToWrite++)
                    {
                        this->writeItem(readToWrite+windowBegin+attackLength, channel, this->readItem(readToWrite+windowBegin+attackLength, channel));
                    }
                    silence = false;//jetzt hört man alles wieder & kein infade mehr
                    
                }
                else
                {
                    for (int readToWrite = 0; readToWrite<windowSize; readToWrite++)
                    {
                        this->writeItem(readToWrite+windowBegin, channel, this->readItem(readToWrite+windowBegin, channel));
                    }   //man hört sowieso etwas, von anfang bis ende des fensters
                }
                
			}
		}
		else {
            
			
			for (int channel = 0; channel < sfInfo.channels; channel++) {
                
                if (silence)
                {
                    //ab hier findet das release bzw. fadeOut statt
                    for (int readToWrite = 0; readToWrite<releaseLength; readToWrite++)
                    {
                        this->writeItem(readToWrite+windowBegin, channel, this->readItem(readToWrite+windowBegin, channel) * (-1*(readToWrite/releaseLength)+1));
                    }
                    //hier hört das release bzw. fadeOut auf
                    
                    for (int writeToZero = windowBegin+releaseLength; writeToZero < (windowBegin+windowSize); writeToZero++)
                    {
                        this->writeItem(writeToZero, channel, 0);
                    };
                    silence = true; //ab jetzt ist alles leise & wird alles nicht mehr ausgefadet
                }
                
                
                else
                {
                    for (int writeToZero = windowBegin; writeToZero < (windowBegin+windowSize); writeToZero++)
                    {
                        this->writeItem(writeToZero, channel, 0);
                    };//man hört sowieso nichts, von anfang bis ende des fensters
                }
                
            }
		}
	}
    this->writeFile("_gatepipe");
}