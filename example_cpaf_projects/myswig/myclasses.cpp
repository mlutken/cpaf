#include "myclasses.h"
#include <iostream>

Vector::Vector( double  iX, double iY, double iZ )
	: x(iX), y(iY), z(iZ)
{
	std::cout << "Vector CONSTRUCTOR: " << x << ", " << y << ", " << z << std::endl;
}

Vector::~Vector()
{
	std::cout << "Vector DESTRUCTOR" << std::endl;
}

double
Vector::len2() 
{ 
	return x*x + y*y + z*z; 
}


