#include <iostream>
#include "Tower.h"

using namespace std;

int main() {
	cout << "Starting!" << endl << endl;

	cout << "Test 1:  Default tower" << endl;
	Tower tower1;
	if (tower1.getX() == 0 && tower1.getY() == 0) {
		cout << "Sucess!" << endl;
	}
	else {
		cout << "Failure!" << endl;
	}
	cout << endl;

	cout << "Test 2:  Tower, X = 5, Y = 2.2" << endl;
	Tower tower2 = Tower(5, 2.2);
	if (tower2.getX() == 5 && tower2.getY() == 2.2) {
		cout << "Sucess!" << endl;
	}
	else {
		cout << "Failure!" << endl;
	}
	cout << endl;

	cout << "Test 3:  Tower mic = 0.4, Tower power = 0.5" << endl;
	Tower tower3;
	if (tower3.measurePow() == 0.5 && tower3.measureVol() == 0.4) {
		cout << "Sucess!" << endl;
	}
	else {
		cout << "Failure!" << endl;
	}
	cout << endl;

	return 0;
}