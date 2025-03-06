#include "Sphere.h"
#include <cmath>
#include <iostream>

using namespace std;

Sphere::Sphere() {}; //Default constructor

void Sphere::setOrigin(double x, double y, double z) {
	xSphere = x;
	ySphere = y;
	zSphere = z;
}

void Sphere::setRadius(double r2) {
	r2Sphere = r2;
}

double Sphere::getX() {
	return xSphere;
}

double Sphere::getY() {
	return ySphere;
}

double Sphere::getZ() {
	return zSphere;
}

double Sphere::getR2() {
	return r2Sphere;
}

//This function accepts a sphere, XYZ coordinates, and an axis to look along.
//It returns the largest positive number along that axis that is on the shell.
double Sphere::getIntersection(double x, double y, double z, char axis) {
	//This starts from the equation of the sphere:  r2 = (x - x_1)^2 + (y - y_1)^2 + (z - z_1)^2
	//Because the sphere is symmetrical, any variable can be switched out with any other.
	//Solving for z, it becomes sqrt(r2 - (x - x_1)^2 - (y - y_1)^2) + z_1 = z

	//Generalizing, z = result, x = axis1, y = axis2

	double axis1;
	double axis2;
	double axis3;
	double sphere1;
	double sphere2;
	double sphere3;

	double xSphere = getX();
	double ySphere = getY();
	double zSphere = getZ();
	double r2Sphere = getR2();

	switch (axis) {
	case 'x':
		axis1 = y;
		axis2 = z;
		axis3 = x;
		sphere1 = ySphere;
		sphere2 = zSphere;
		sphere3 = xSphere;
		break;
	case 'y':
		axis1 = x;
		axis2 = z;
		axis3 = y;
		sphere1 = xSphere;
		sphere2 = zSphere;
		sphere3 = ySphere;
		break;
	case 'z':
		axis1 = x;
		axis2 = y;
		axis3 = z;
		sphere1 = xSphere;
		sphere2 = ySphere;
		sphere3 = zSphere;
		break;
	default:
		//cout << "Error!  Axis character not recognized.";
		return -2;
	}
	

	double core1 = axis1 - sphere1;
	double core2 = axis2 - sphere2;

	//Check to see if the given coordinates lie outside the circle.
	if (core1 * core1 + core2 * core2 > r2Sphere) {
		//cout << "Error!  Given point is outside the circle" << endl;
		return -1;
	}

	//cout << "Point detected inside circle!" << endl;
	double result = sqrt(r2Sphere - (core1 * core1) - (core2 * core2)) + sphere3;
	return result;
}