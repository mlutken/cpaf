
/* File : myclasses.i */
%module myclasses

%{
#include "myclasses.h"
%}

class Vector {
public:
        double x,y,z;
        Vector( double iX, double iY, double iZ );
        ~Vector();
        double len2();
};



