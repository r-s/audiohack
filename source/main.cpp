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
    
    
    if (argc < 3) {
        cout << "Use: audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
        return 1;
    }
    
    char* inputfilepath = argv[1];
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
         gain.gain((double)*argv[3]);
     }
     else if (!strcmp(function,"-fadein")){
         Time fadeIn(inputfilepath);
         fadeIn.fadeIn((int)*argv[3]);
     }
     else if (!strcmp(function,"-fadeout")){
         Time fadeOut(inputfilepath);
         fadeOut.fadeOut((int)*argv[3]);
     }
     else if (!strcmp(function,"-cut")){
         Time cut(inputfilepath);
         cut.cutAtTime((double)*argv[3]);
     }
     else if (!strcmp(function,"-gate")){
         Dynamics gate(inputfilepath);
         gate.gatePipe(true, (double)*argv[3], (double)*argv[4], (double)*argv[5]);
     }
     else if (!strcmp(function,"-pipe")){
         Dynamics pipe(inputfilepath);
         pipe.gatePipe(false, (double)*argv[3], (double)*argv[4], (double)*argv[5]);
     }
     else if (!strcmp(function,"-delay")){
         Effects delay(inputfilepath);
         delay.delay((double)*argv[3], (double)*argv[4], (double)*argv[5]);
     }
     else {
     cout << "Use: ./audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
     return 1;
     }
     
	
    return 0;
}
