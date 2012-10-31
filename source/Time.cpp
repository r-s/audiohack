//
//  Time.cpp
//  audiohack
//


#include "Time.h"

Time::Time(string filePath) : Superclass(filePath){}

int Time::cutAtTime(double second) {
	// todo:
	// - fadeOut and fadeIn at cutframe (100 frames)
	// - return cutframe

	double maxTime = sfInfo.frames / sfInfo.samplerate;

	if (second <= maxTime and second > 0) {
		sf_count_t frameToCut = sfInfo.samplerate * second;

		int fadeTime = 100;

		this->fadeOut(fadeTime, frameToCut-fadeTime);
		this->fadeIn(fadeTime, frameToCut);

		// writeFile(string insertion, int start, int stop, int channels)
		// for both channels

		this->writeFile("_cut01", 0, frameToCut-1, sfInfo.channels);

		this->writeFile("_cut02", frameToCut, sfInfo.frames, sfInfo.channels);
		return 0;

	}
	else {
		//		cout << "Falscher Wert: Die Schnittzeit übersteigt die Gesamtlänge der Audiofile!" << endl;
		// nicht mehr nötig, da fehlermeldungen via interface
		//Error = true;
		//+ Errormessage

		if(second <= maxTime)
			return 1;
		// return errorcode 1
		if(second <= 0)
			return 2;
		// return errorcode 2

		// TODO: fehlercode überprüfen und ggf anpassen
	}
	return 0;

}


// unused
bool Time::cutAtContent(double second, double tolerance) {
	// todo:
	// - everything

	return false;
}
