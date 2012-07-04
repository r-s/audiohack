//
//  main.cpp
//  audiohack
//

#include <iostream>
#include "Dynamics.h"
#include "Time.h"
#include "Effects.h"

int main (int argc, const char * argv[])
{
	Superclass sound_1("/Users/Macbookpro/Desktop/InputFile.aif");
	
	for (int i = 100; i < 800; i++) {
		std::cout << sound_1.readItem(i, 0) << endl;
	}

    return 0;
}
