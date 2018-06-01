#include "FuzzyAvoider.h"
#include "Structures.h"
#include <vector>
#include <iostream>

//----------GLOBAL VARIABLES----------
double slow = 0;
double modest = 150;
double fast = 300;
MembershipFunction CloseAvoid = MembershipFunction(0, 0, 100, 350);
MembershipFunction MediumAvoid = MembershipFunction(100, 325, 375, 600);
MembershipFunction FarAvoid = MembershipFunction(350, 600, 10000, 10000);

//cc cm cf mc mm mf fc fm ff
vector<double> leftFuzzy = { slow, slow, slow, slow, slow, slow, slow, slow, modest };
vector<double> rightFuzzy = { fast, fast, fast, fast, modest, modest, fast, modest, modest };

//----------PUBLIC FUNCTIONS----------
vector<double> FuzzyAvoider::avoid(double sonar2, double sonar5)
{
	vector<double> fuzzySonar2 = fuzzify(sonar2);
	vector<double> fuzzySonar5 = fuzzify(sonar5);
	vector<double> firingStrength = infer(fuzzySonar2, fuzzySonar5);
	vector<double> speed = defuzzify(firingStrength);
	return speed;
}

//----------PRIVATE FUNCTIONS----------

//-----fuzzification-----
vector<double> FuzzyAvoider::fuzzify(double x)
{
	return{ CloseAvoid.membership(x), MediumAvoid.membership(x), FarAvoid.membership(x) };
}

//-----inference-----
//-core-
vector<double> FuzzyAvoider::infer(vector<double> sonar2, vector<double> sonar5)
{
	vector<double> fireStr;
	for (int i = 0; i < sonar2.size(); i++) {
		for (int j = 0; j < sonar5.size(); j++) {
			double minVal = minimum(sonar2[i], sonar5[j]);
			fireStr.push_back(minVal);
		}
	}
	return fireStr;
}

//-auxiliary-
double FuzzyAvoider::minimum(double x, double y)
{
	if (x < y) return x;
	return y;
}

//-----defuzzification-----

vector<double> FuzzyAvoider::defuzzify(vector<double> firingStr)
{
	vector<double> outputSpeed;
	vector<double> leftPartialOutput = ifThenRules(firingStr, leftFuzzy);
	vector<double> rightPartialOutput = ifThenRules(firingStr, rightFuzzy);
	outputSpeed.push_back(fuzzyOutput(leftPartialOutput, firingStr));
	outputSpeed.push_back(fuzzyOutput(rightPartialOutput, firingStr));
	return outputSpeed;
}

vector<double> FuzzyAvoider::ifThenRules(vector<double> fireStr, vector<double> fuzzySpeed)
{
	vector<double> partialOutputs;
	for (int i = 0; i < fireStr.size(); i++) {
		partialOutputs.push_back(fireStr[i] * fuzzySpeed[i]);
	}
	return partialOutputs;
}

double FuzzyAvoider::fuzzyOutput(vector<double> partials, vector<double> fireStr)
{
	double numerator = 0;
	double denominator = 0;
	for (int i = 0; i < partials.size(); i++) {
		numerator = numerator + partials[i];
		denominator = denominator + fireStr[i];
	}
	return numerator / denominator;
}