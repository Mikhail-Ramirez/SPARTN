#include "RFDetection.h"
#include "Tower.h"
#include "Sphere.h"
#include <cmath>
#include <tuple>
#include <iostream>

using namespace std;

RFDetection::RFDetection(Tower* t1, Tower* t2) {
	tN = t1;
	tM = t2;
	update();

	//Because the main process created the Tower objects, RFDetection is not responsible for destroying them.
}

double RFDetection::getX() {
	return shell.getX();
}

double RFDetection::getY() {
	return shell.getY();
}

double RFDetection::getZ() {
	return shell.getZ();
}

double RFDetection::getR2() {
	return shell.getR2();
}

double RFDetection::getIntersection(double x, double y, double z, char axis) {
	return shell.getIntersection(x, y, z, axis);
}

void RFDetection::update() {
	//This function uses the big equation to convert tower data into a sphere of possibilities.
	//This equation has never been tested.  This will probably be a huge mistake.
	
	//Get constants needed.
	double Pn = tN->getPow();
	double Xn = tN->getX();
	double Yn = tN->getY();
	double Xn2 = Xn * Xn;
	double Yn2 = Yn * Yn;

	double Pm = tM->getPow();
	double Xm = tM->getX();
	double Ym = tM->getY();
	double Xm2 = Xm * Xm;
	double Ym2 = Ym * Ym;

	double PnPmDiv = (Pn / Pm);
	double denom = 1 - PnPmDiv;

	//Solving r2
	//Part 1
	double r2 = (PnPmDiv * (Xn2 + Yn2) - (Xm2 + Ym2)) / (denom);
	//Part 2
	double temp = (Xm + PnPmDiv * Xn) / (denom);
	temp = temp * temp;
	r2 = r2 + temp;
	//Part 3
	temp = (Ym + PnPmDiv * Yn) / (denom);
	temp = temp * temp;
	r2 = r2 + temp;
	//Finally, write to Sphere shell.
	shell.setRadius(r2);

	//Solving for the X-Offset
	double XOff = (Xm + PnPmDiv * Xn) / denom;
	
	//Solving for the Y-Offset
	double YOff = (Ym + PnPmDiv * Yn) / denom;

	//Setting the XYZ offset.
	shell.setOrigin(XOff, YOff, 0);
	//Note!  Z-Off is 0 because the towers should be on the same plane.
	//Fixing this will require the equation to be rewritten.
}

tuple<double, double, double> getIntersection(RFDetection* RF1, RFDetection* RF2, RFDetection* RF3) {
	//From https://stackoverflow.com/questions/1406375/finding-intersection-points-between-3-spheres
	//Sadly, this needs to be translated from Python.

	//Note!  The sphere numbers here are independent of the tower numbers.
	
	//Step 1:  Load the data into local variables.
	tuple <double, double, double> P1( RF1->getX(), RF1->getY(), RF1->getZ() );
	double R21 = RF1->getR2();

	tuple <double, double, double> P2( RF2->getX(), RF2->getY(), RF2->getZ() );
	double R22 = RF2->getR2();

	tuple <double, double, double> P3( RF3->getX(), RF3->getY(), RF3->getZ() );
	double R23 = RF3->getR2();

	//Step 2:  Calculate!
	tuple<double, double, double> temp1(get<0>(P2) - get<0>(P1), get<1>(P2) - get<1>(P1), get<2>(P2) - get<2>(P1));

	double normalized1 = norm(temp1);
	tuple <double, double, double> e_x(get<0>(temp1) / normalized1, get<1>(temp1) / normalized1, get<2>(temp1) / normalized1);

	tuple<double, double, double> temp2(get<0>(P3) - get<0>(P1), get<1>(P3) - get<1>(P1), get<2>(P3) - get<2>(P1));

	double i = dot(e_x, temp2);

	tuple<double, double, double> temp3(get<0>(temp2) - i * get<0>(e_x), get<1>(temp2) - i * get<1>(e_x), get<2>(temp2) - i * get<2>(e_x));

	double normalized3 = norm(temp3);
	tuple <double, double, double> e_y(get<0>(temp3) / normalized3, get<1>(temp3) / normalized3, get<2>(temp3) / normalized3);
	
	tuple <double, double, double> e_z = cross(e_x, e_y);

	double d = norm(temp1);

	double j = dot(e_y, temp2);

	double x = (R21 - R22 + d * d) / (2 * d);

	double y = (R21 - R23 - 2 * i * x + i * i + j * j) / (2 * j);

	double temp4 = R21 - x * x - y * y;

	if (temp4 < 0) {
		cout << "Error!  Spheres do not intercept." << endl;
		return { 0, 0, 0 };
	}

	double z = sqrt(temp4);

	//p_12_a = P1 + x*e_x + y*e_y + z*e_z
	return make_tuple(get<0>(P1) + x * get<0>(e_x) + y*get<0>(e_y) + z*get<0>(e_z),
		get<1>(P1) + x * get<1>(e_x) + y * get<1>(e_y) + z * get<1>(e_z),
		get<2>(P1) + x * get<2>(e_x) + y * get<2>(e_y) + z * get<2>(e_z));
}

double norm(tuple<double, double, double> var) {
	double X = get<0>(var);
	double Y = get<1>(var);
	double Z = get<2>(var);
	return sqrt(X * X + Y * Y + Z * Z);
}

tuple<double, double, double> cross(tuple<double, double, double> P1, tuple<double, double, double> P2) {
	double X1 = get<0>(P1);
	double Y1 = get<1>(P1);
	double Z1 = get<2>(P1);

	double X2 = get<0>(P2);
	double Y2 = get<1>(P2);
	double Z2 = get<2>(P2);
	return tuple<double, double, double>(Y1*Z2 - Z1*Y2, Z1*X2 - X1*Z2, X1*Y2 - Y1*X2);
}

double dot(tuple<double, double, double> P1, tuple<double, double, double> P2) {
	return get<0>(P1) * get<0>(P2) + get<1>(P1) * get<1>(P2) + get<2>(P1) * get<2>(P2);
}