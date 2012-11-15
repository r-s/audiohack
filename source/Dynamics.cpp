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
    
    if ((release < 146) and (attack < 146)) {
        
    int attackLength = (attack * sfInfo.samplerate) / 1000;
    int releaseLength = (release * sfInfo.samplerate) / 1000;
    double threshValue = 1.0 * (pow(10.0, (threshold / 10)));
        int windowSize = attackLength+releaseLength+1000;//für den fall, dass sowohl attack als auch release 1 oder sogar 0 sind
        
    bool silence = true;    
        
	for (int windowBegin = 0; windowBegin < sfInfo.frames; windowBegin = windowBegin + windowSize) {
		
		// die Größe des letzten Fensters muss neu kalkuliert werden beim letzten Durchgang
		if (windowBegin + windowSize >= sfInfo.frames) {windowSize = (int)sfInfo.frames - windowBegin;}
        
        for (int channel = 0; channel < sfInfo.channels; channel++) {
            
		double currentRMS = this->rms(windowBegin, windowBegin + windowSize - 1, channel);

            
		if ((currentRMS < threshValue && !pipe) || (currentRMS > threshValue && pipe)) {
            
			
                if (silence==true)
                {
                    //ab hier findet das attack bzw. fadeIn statt
                    for (int readToFadeIn = 0; readToFadeIn<attackLength; readToFadeIn++)
                    {
                        this->writeItem(readToFadeIn+windowBegin, channel, this->readItem(readToFadeIn+windowBegin, channel) * ((double)(readToFadeIn)/(double)(attackLength)));
                    }
                    //hier hört das attack bzw. fadeIn auf
                    
                    for (int writeToCopy = attackLength+1; writeToCopy < windowSize; writeToCopy++)
                    {
                        this->writeItem(writeToCopy+windowBegin, channel, this->readItem(writeToCopy+windowBegin, channel));
                    }
                    silence = false;//jetzt hört man alles wieder & kein fadeIn mehr
                    //@@@@@@@@EINZIGES RESTPROBLEM AN DIESER STELLE!!! Die Veränderung des Bools wird nicht gespeichert!!!@@@@@@@@
                }
            
                else
                {
                    for (int writeToCopy = windowBegin; writeToCopy < (windowBegin+windowSize); writeToCopy++)
                    {
                        this->writeItem(writeToCopy, channel, this->readItem(writeToCopy, channel));
                    }   //man hört sowieso etwas, von anfang bis ende des fensters
                }
            
            
			}
		
		else {
            
            
                if (silence==false)
                {
                    //ab hier findet das release bzw. fadeOut statt
                    for (int readToFadeOut = 0; readToFadeOut<releaseLength; readToFadeOut++)
                    {
                        this->writeItem(readToFadeOut+windowBegin, channel, this->readItem(readToFadeOut+windowBegin, channel) * (-1*((double)(readToFadeOut)/(double)(releaseLength))+1));
                    }
                    //hier hört das release bzw. fadeOut auf
                    
                    for (int writeToZero = releaseLength+1 + windowBegin; writeToZero < windowSize; writeToZero++)
                    {
                        this->writeItem(writeToZero, channel, 0);
                    }
                    silence = true; //ab jetzt ist alles leise & wird alles nicht mehr ausgefadet
                    //@@@@@@@@EINZIGES RESTPROBLEM AN DIESER STELLE!!! Die Veränderung des Bools wird nicht gespeichert!!!@@@@@@@@
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
    if (pipe == true) {this->writeFile("_gate");}
    else if (pipe == false) {this->writeFile("_pipe");}
    }
    else {cout << "Sorry! You need to choose values which are equal or less 145 for the parameters of attack and release!" << "\n";}
    //cout <<"sfinfo.frames = " << sfInfo.frames << "\n";
}
