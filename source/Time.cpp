//
// Time.cpp
// audiohack
//

// todo:
// - cutAtTime
// - cutAtContent

#include "Time.h"

using namespace std;

int Time::cutAtTime(double second) {
	// todo:
	// - fadeOut and fadeIn at cutframe (100 frames)
	// - return cutframe


	double maxTime = sfInfo.frames / sfInfo.samplerate;

	if (second <= maxTime) {

		//double **part1 = new double *[sfInfo.channels], **part2 = new double *[sfInfo.channels];
		sf_count_t frameToCut = sfInfo.samplerate * second;

		int fadeTime = 100;

		this->fadeOut(fadeTime, frameToCut-fadeTime);
		this->fadeIn(fadeTime, frameToCut);

		return frameToCut;

	}
	else {
		cout << "Falscher Wert: Die Schnittzeit übersteigt die Gesamtlänge der Audiofile!" << endl;
		//Error = true;
		//+ Errormessage
		return 0;
	}

}


// unused
bool Time::cutAtContent(double second, double tolerance) {
	// todo:
	// - everything

	return false;
}
