#pragma once
#include "Tower.h"
#include "Sphere.h"
#include <tuple>

using std::tuple;

class RFDetection {

public:
	RFDetection(Tower *, Tower *);
	void update();

	double getX();
	double getY();
	double getZ();
	double getR2();
	double getIntersection(double, double, double, char);

private:
	Sphere shell;
	Tower* tN;
	Tower* tM;
};

tuple<double, double, double> getIntersection(RFDetection*, RFDetection*, RFDetection*);

double norm(tuple<double, double, double>);
tuple<double, double, double> cross(tuple<double, double, double>, tuple<double, double, double>);
double dot(tuple<double, double, double>, tuple<double, double, double>);