#include "Tower.h"
using namespace std;

Tower::Tower() {} //Default constructor

Tower::Tower(double x, double y) {
	setXY(x, y);
}

void Tower::setXY(double x, double y) {
	towerX = x;
	towerY = y;
}

double Tower::measurePow() {
	//No idea how this will work yet.
	//Return 0.5 as a default.
	towerPower = 0.5;
	return towerPower;
}

double Tower::measureVol() {
	//No idea how this will work yet.
	//Return 0.4 as a default.
	towerVolume = 0.4;
	return towerVolume;
}

double Tower::getPow() {
	return towerPower;
}

double Tower::getVol() {
	return towerVolume;
}

double Tower::getX() {
	return towerX;
}

double Tower::getY() {
	return towerY;
}