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
	
	sound_1.reverse();
	
    return 0;
}
