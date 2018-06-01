#include "aria.h"
#include "FuzzyFollower.h"
#include "FuzzyAvoider.h"
#include <vector>
#include <iostream>
using namespace std;

double minimum(double x, double y)
{
	if (x < y) return x;
	return y;
}

int main(int argc, char **argv) {

	Aria::init();
	ArRobot robot;
	ArPose pose;

	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();

	ArRobotConnector robotConnector(&argParser, &robot);
	if (robotConnector.connectRobot())
		std::cout << "Robot connected!" << std::endl;
	robot.runAsync(false);
	robot.lock();
	robot.enableMotors();
	robot.unlock();

	ArSensorReading *sonarSensor[8];


	ArUtil::sleep(1000);

	// storing sonar readings in array
	// just need the right side sonars for this assignment

	double sonarRange[8];
	double averageSonarRange[8];
	double rightBack;
	double rightFront;
	double frontRight;
	double frontLeft;
	double realRange;
	double front;
	double right;

	FuzzyFollower robotFollow = FuzzyFollower();
	FuzzyAvoider robotAvoid = FuzzyAvoider();

	while (true) {
		for (int t = 2; t <= 7; t++) {

			sonarSensor[t] = robot.getSonarReading(t);
			sonarRange[t] = sonarSensor[t]->getRange();
			ArUtil::sleep(100);
			cout << "--------------" << "\n";
			cout << "sonar 2 = " << sonarRange[2] << "\n";
			cout << "sonar 3 = " << sonarRange[3] << "\n";
			cout << "sonar 4 = " << sonarRange[4] << "\n";
			cout << "sonar 5 = " << sonarRange[5] << "\n";
			cout << "sonar 6 = " << sonarRange[6] << "\n";
			cout << "sonar 7 = " << sonarRange[7] << "\n";

		}

		if (sonarRange[6] < 250 || sonarRange[7] < 250) {
			sonarRange[6] = 10;
			sonarRange[7] = 10;
		}

		rightFront = minimum(sonarRange[6], sonarRange[7]);
		rightBack = minimum(sonarRange[6], sonarRange[7]);
		right = minimum(rightFront, rightBack);
		frontRight = minimum(sonarRange[4], sonarRange[5]);
		frontLeft = minimum(sonarRange[3], sonarRange[3]);
		front = minimum(frontRight, frontLeft);

		if (front < 400) {
			vector<double> avoiderOut = robotAvoid.avoid(front, front);
			robot.setVel2(avoiderOut[0], avoiderOut[1]);
		}
		else {
			vector<double> followerOut = robotFollow.follow(right, right);
			robot.setVel2(followerOut[0], followerOut[1]);
		}

		//cout << followerOut[0] << "\n" << followerOut[1] << "\n";
		//cout << avoiderOut[0] << "\n" << avoiderOut[1] << "\n";

	}
	system("PAUSE");
	return 0;
}