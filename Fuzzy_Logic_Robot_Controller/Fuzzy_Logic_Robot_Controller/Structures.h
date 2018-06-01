#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include<vector>
using namespace std;

class MembershipFunction
{
public:
	MembershipFunction(double a, double b, double c, double d);

	double membership(double);

private:
	double a, b, c, d;
};

#endif

