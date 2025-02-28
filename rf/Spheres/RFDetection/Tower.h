#pragma once

class Tower {

public:
	Tower();
	Tower(double, double);
	void setXY(double, double);
	
	double measureVol();
	double measurePow();

	double getVol();
	double getPow();
	double getX();
	double getY();

private:
	double towerX = 0;
	double towerY = 0;
	double towerPower = 0;
	double towerVolume = 0;
	//Does this need a microphone object?
	//Does this need a hackRF object?
};