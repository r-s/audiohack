//
//  main.cpp
//  audiohack
//

#include <iostream> // wird eigentlich in Superclass.h aufgerufen
#include "Dynamics.h"
#include "Time.h"
#include "Effects.h"
//#include "Superclass.h" // ?

using namespace std;

int main (int argc, char* argv[])
{
    /*
     //spezielle ueberpruefungs-main für gatepipe
    Dynamics pipe("/Users/Jann/Desktop/InputFile.aif");
    pipe.gatePipe(false, 40, 50, 50);
    */
    
    if (argc < 3) {
        cout << "Use: audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
        return 1;
    }
    
    string inputfilepath = argv[1];
    char* function = argv[2];
	
    
    //cout << filepath << endl << function << endl;
    
     if (!strcmp(function,"-rev")){
         Superclass rev(inputfilepath);
         rev.reverse();
     }
     else if (!strcmp(function,"-phase")){
         Superclass phase(inputfilepath);
         phase.invertPhase();
     }
     else if (!strcmp(function,"-norm")){
         Dynamics norm(inputfilepath);
         norm.normalize();
     }
     else if (!strcmp(function,"-gain")){
         Dynamics gain(inputfilepath);
         gain.gain(atof(argv[3]));
     }
     else if (!strcmp(function,"-fadein")){
         Time fadeIn(inputfilepath);
         fadeIn.fadeIn(atof(argv[3]));
     }
     else if (!strcmp(function,"-fadeout")){
         Time fadeOut(inputfilepath);
         fadeOut.fadeOut(atof(argv[3]));
     }
     else if (!strcmp(function,"-cut")){
         Time cut(inputfilepath);
         cut.cutAtTime(atof(argv[3]));
     }
     else if (!strcmp(function,"-gate")){
         Dynamics gate(inputfilepath);
         gate.gatePipe(true, atof(argv[3]), atof(argv[4]), atof(argv[5]));
     }
     else if (!strcmp(function,"-pipe")){
         Dynamics pipe(inputfilepath);
         pipe.gatePipe(false, atof(argv[3]), atof(argv[4]), atof(argv[5]));
     }
     else if (!strcmp(function,"-delay")){
         Effects delay(inputfilepath);
         delay.delay(atof(argv[3]), atof(argv[4]), atof(argv[5]));
     }
     else {
     cout << "Use: ./audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
     return 1;
     }
     
	
    return 0;
}
