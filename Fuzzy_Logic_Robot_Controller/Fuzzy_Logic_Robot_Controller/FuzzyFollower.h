#ifndef FUZZYFOLLOWER_H_
#define FUZZYFOLLOWER_H_

#include <vector>
using namespace std;

class FuzzyFollower {
public:
	vector<double> follow(double, double);

private:
	vector<double> fuzzifyR(double);
	vector<double> fuzzifyF(double);

	vector<double> infer(vector<double>, vector<double>);
	double minimum(double, double);

	vector<double> defuzzify(vector<double>);
	vector<double> ifThenRules(vector<double>, vector<double>);
	double fuzzyOutput(vector<double>, vector<double>);
};

#endif
