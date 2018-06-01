#include "Structures.h"

//----------CONSTRUCTORS----------

MembershipFunction::MembershipFunction(double a1, double b1, double c1, double d1)
{
	a = a1;
	b = b1;
	c = c1;
	d = d1;
}

//----------FUNCTIONS----------
double MembershipFunction::membership(double x)
{
	if (x <= a) return 0;
	else if (x <= b) return (x - a) / (b - a);
	else if (x <= c) return 1;
	else if (x <= d) return (d - x) / (d - c);
	return 0;
}