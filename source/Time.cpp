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

		double fadeTime = 1.0/sfInfo.samplerate*100;
		cout << fadeTime << endl;

		this->fadeOut(fadeTime, frameToCut-fadeTime);
		this->fadeIn(fadeTime, frameToCut);

		this->writeFile("_cut01", 0, frameToCut-1, sfInfo.channels);

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
