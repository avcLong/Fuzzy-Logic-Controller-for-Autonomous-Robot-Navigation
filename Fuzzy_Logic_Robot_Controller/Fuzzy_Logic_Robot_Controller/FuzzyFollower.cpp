#include "FuzzyFollower.h"
#include "Structures.h"
#include <vector>
#include <iostream>
using namespace std;

//----------GLOBAL VARIABLES----------

//speed fuzzy sets, the triangles have same width, so only the centre is required
const double slow = 50;
const double modest = 100;
const double fast = 150;

//membership functions for the sonar readings
MembershipFunction CloseFollowR = MembershipFunction(0, 0, 100, 350);
MembershipFunction MediumFollowR = MembershipFunction(100, 325, 375, 600);
MembershipFunction FarFollowR = MembershipFunction(350, 600, 5000, 5000);

MembershipFunction CloseFollowF = MembershipFunction(0, 0, 100, 350);
MembershipFunction MediumFollowF = MembershipFunction(100, 325, 375, 600);
MembershipFunction FarFollowF = MembershipFunction(350, 600, 5000, 5000);


//the speed outputs for the if-then rules in the order for the following mapping with the sonar membership values:
//cc cm cf mc mm mf fc fm ff
const vector<double> leftFuzzy = { slow, slow, slow, modest, modest, slow, modest, fast, fast };
const vector<double> rightFuzzy = { fast, modest, fast, slow, modest, modest, slow, modest, slow };
//const vector<double> rightFuzzy = { fast, fast, fast, modest, modest, modest, slow, slow, slow };

//----------PUBLIC FUNCTIONS----------

vector<double> FuzzyFollower::follow(double sonar6, double sonar7)
{
	vector<double> fuzzySonar6 = fuzzifyF(sonar6);
	vector<double> fuzzySonar7 = fuzzifyR(sonar7);
	vector<double> firingStrength = infer(fuzzySonar6, fuzzySonar7);
	vector<double> speed = defuzzify(firingStrength);
	return speed;
}

//----------PRIVATE FUNCTIONS----------

//-----fuzzification-----
//-core-
vector<double> FuzzyFollower::fuzzifyR(double x)
{
	return{ CloseFollowR.membership(x), MediumFollowR.membership(x), FarFollowR.membership(x) };
}

vector<double> FuzzyFollower::fuzzifyF(double x)
{
	return{ CloseFollowF.membership(x), MediumFollowF.membership(x), FarFollowF.membership(x) };
}

//-----inference-----
//-core-
vector<double> FuzzyFollower::infer(vector<double> sonar6, vector<double> sonar7)
{
	vector<double> fireStr;
	for (int i = 0; i < sonar6.size(); i++) {
		for (int j = 0; j < sonar7.size(); j++) {
			fireStr.push_back(minimum(sonar6[i], sonar7[j]));
		}
	}
	return fireStr;
}

//-auxiliary-
double FuzzyFollower::minimum(double x, double y)
{
	if (x < y) return x;
	return y;
}


//-----defuzzification-----
//-core-
vector<double> FuzzyFollower::defuzzify(vector<double> firingStr)
{
	vector<double> outputSpeed;
	vector<double> leftPartialOutput = ifThenRules(firingStr, leftFuzzy);
	vector<double> rightPartialOutput = ifThenRules(firingStr, rightFuzzy);
	outputSpeed.push_back(fuzzyOutput(leftPartialOutput, firingStr));
	outputSpeed.push_back(fuzzyOutput(rightPartialOutput, firingStr));
	return outputSpeed;
}

//-auxiliary-
vector<double> FuzzyFollower::ifThenRules(vector<double> fireStr, vector<double> fuzzySpeed)
{
	vector<double> partialOutputs;
	for (int i = 0; i < fireStr.size(); i++) {
		partialOutputs.push_back(fireStr[i] * fuzzySpeed[i]);
	}
	return partialOutputs;
}

double FuzzyFollower::fuzzyOutput(vector<double> partials, vector<double> fireStr)
{
	double numerator = 0;
	double denominator = 0;
	for (int i = 0; i < partials.size(); i++) {
		numerator = numerator + partials[i];
		denominator = denominator + fireStr[i];
	}
	return numerator / denominator;
}
