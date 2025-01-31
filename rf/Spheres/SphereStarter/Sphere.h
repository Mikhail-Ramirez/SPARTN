#pragma once

class Sphere
{
public:
	Sphere();
	void setOrigin(double x, double y, double z);
	void setRadius(double r2);
	double getX();
	double getY();
	double getZ();
	double getR2();
	double getIntersection(double x, double y, double z, char axis);

private:
	double xSphere = 0;
	double ySphere = 0;
	double zSphere = 0;
	double r2Sphere = 0;
};