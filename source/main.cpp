//
//  main.cpp
//  audiohack
//

#include <iostream>
#include "Dynamics.h"
#include "Time.h"
#include "Effects.h"

using namespace std;

int main (int argc, char* argv[])
{
    
    /*
    if (argc < 3) {
        cout << "Use: audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
        return 1;
    }
    
    char* inputfilepath = argv[1];
    char* function = argv[2];
    
    //cout << filepath << endl << function << endl;
    
     if (function == "-rev"){
         Superclass rev(inputfilepath);
         rev.reverse();
     }
     else if (function == "-phase"){
         Superclass phase(inputfilepath);
         phase.invertPhase();
     }
     else if (function == "-norm"){
         Dynamics norm(inputfilepath);
         norm.normalize();
     }
     else if (function == "-gain"){
         Dynamics gain(inputfilepath);
         gain.gain(*argv[3]);
     }
     else if (function == "-fadein"){
         Time fadeIn(inputfilepath);
         fadeIn.fadeIn(*argv[3]);
     }
     else if (function == "-fadeout"){
         Time fadeOut(inputfilepath);
         fadeOut.fadeOut(*argv[3]);
     }
     else if (function == "-cut"){
         Time cut(inputfilepath);
         cut.cutAtTime(*argv[3]);
     }
     else if (function == "-gate"){
         Dynamics gate(inputfilepath);
         gate.gatePipe(true, *argv[3], *argv[4], *argv[5]);
     }
     else if (function == "-pipe"){
         Dynamics pipe(inputfilepath);
         pipe.gatePipe(false, *argv[3], *argv[4], *argv[5]);
     }
     else if (function == "-delay"){
         Effects delay(inputfilepath);
         delay.delay(*argv[3], *argv[4], *argv[5]);
     }
     else {
     cout << "Use: ./audiohack inputfilepath function [parameter 1] [parameter 2])" << endl << "For further documentation see user_interface.pdf" << endl;
     return 1;
     }
     */
	
    return 0;
}