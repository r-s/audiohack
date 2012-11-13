//
//  Time.cpp
//  audiohack
//


#include "Time.h"

Time::Time(string filePath) : Superclass(filePath){}

int Time::cutAtTime(double second) {
	double maxTime = sfInfo.frames / (double)sfInfo.samplerate;

	if (second <= maxTime and second > 0) {
		sf_count_t frameToCut = sfInfo.samplerate * second;

		int framesToFade = 100;
		sf_count_t frames = sfInfoOut.frames;
		// sfInfoOut.frames backup

		double fadeTime = 1.0 / sfInfo.samplerate * framesToFade;

		this->fadeOut(fadeTime, frameToCut-framesToFade);
		this->writeFile("_cut01", 0, frameToCut-1, sfInfo.channels);

		sfInfoOut.frames = frames;
		// sfInfoOut wieder herstellen
		// writeFile() verändert sfInfoOut.frames.
		// wir müssen sie wiederherstellen, damit die zweite geschrieben wird.

		this->fadeIn(fadeTime, frameToCut);
		this->writeFile("_cut02", frameToCut, sfInfo.frames, sfInfo.channels);

		return 0;

	}
	else {
		if(second <= maxTime)
			return 1;
		if(second <= 0)
			return 2;
	}
	return 0;

}


// unused
bool Time::cutAtContent(double second, double tolerance) {
	// todo:
	// - everything

	return false;
}
