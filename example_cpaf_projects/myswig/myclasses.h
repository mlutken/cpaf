#ifndef _MY_CLASSES_H_
#define _MY_CLASSES_H_


class Vector {
public:
	double x,y,z;
	Vector( double  iX, double iY, double iZ );
	~Vector();
	double	len2();
//		void	print() {}
};

#endif //_MY_CLASSES_H_


