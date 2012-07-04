//
// Time.cpp
// audiohack
//

// todo:
// - cutAtTime
// - cutAtContent

#include "Time.h"

using namespace std;

void Time::cutAtTime(double second) {
	// todo:
	// - Write both parts to File.


	//Parameter:
	// - SampleRate (sfInfo.samplerate)
	// - Schnittzeit(seconds)
	// - rawData, (**rawData)
	// - frameSum (sfInfo.frames)
	double maxTime = sfInfo.frames / sfInfo.samplerate;

	if (second <= maxTime) {

		double **part1 = new double *[sfInfo.channels], **part2 = new double *[sfInfo.channels];
		sf_count_t frameToCut = sfInfo.samplerate * second;

		for (int channel = 0; channel < sfInfo.channels; channel++){
			part1[channel] = new double[(int)frameToCut];
			part2[channel] = new double[sfInfo.frames - (int)frameToCut];
		}

		for (int channel = 0;  channel < sfInfo.channels; channel++) {
			for (int frame = 0; frame < frameToCut; frame++){
				//part1[channel][frame] = rawData[channel][frame];
				part1[channel][frame] = this->readItem(frame, channel);

			}
		}

		for (int channel = 0;  channel < sfInfo.channels; channel++) {
			for (int frame = 0; frame < frameToCut; frame++){
				//part2[channel][frame] = rawData[channel][frame + frameToCut];
				part2[channel][frame] = this->readItem(frame + frameToCut, channel);

			}
		}

	}
	else {
		cout << "Falscher Wert: Die Schnittzeit �bersteigt die Gesamtl�nge der Audiofile!" << endl;
		//Error = true;
		//+ Errormessage
	}

}


// unused
bool Time::cutAtContent(double second, double tolerance) {
	// todo:
	// - everything

	return false;
}
