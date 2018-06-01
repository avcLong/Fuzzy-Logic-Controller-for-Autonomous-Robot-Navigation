#ifndef FUZZYAVOIDER_H_
#define FUZZYAVOIDER_H_


#include<vector>
using namespace std;

class FuzzyAvoider
{
public:
	vector<double> avoid(double, double);

private:
	vector<double> fuzzify(double);

	vector<double> infer(vector<double>, vector<double>);
	double minimum(double, double);

	vector<double> defuzzify(vector<double>);
	vector<double> ifThenRules(vector<double>, vector<double>);
	double fuzzyOutput(vector<double>, vector<double>);
};



#endif
