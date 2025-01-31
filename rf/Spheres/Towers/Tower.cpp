#include "Tower.h"
using namespace std;

Tower::Tower(double x = 0, double y = 0) {
	setXY(x, y);
}

void Tower::setXY(double x, double y) {
	towerX = x;
	towerY = y;
}

double Tower::measurePower() {
	//No idea how this will work yet.
	//Return 0.5 as a default.
	towerPower = 0.5;
	return towerPower;
}

double Tower::measureMic() {
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