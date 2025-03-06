#include <iostream>
#include "Sphere.h"
#include <cmath>

using namespace std;

int main() {
	cout << "Starting!" << endl;

	//Test 1:  Default sphere case.  Sphere should be all 0s.
	cout << "Test 1:  Create a sphere with default parameters. -- ";
	Sphere sphereTest1;
	if (sphereTest1.getX() == 0 && sphereTest1.getY() == 0 && sphereTest1.getZ() == 0 && sphereTest1.getR2() == 0) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Fail!***" << endl << endl;
	}

	//Test 2: Reshaping the sphere to new parameters.
	cout << "Test 2:  Create a sphere, then reshape it to a new size. -- ";
	Sphere sphereTest2;
	double x2 = 3;
	double y2 = 8;
	double z2 = 97;
	double r2_2 = 5;
	sphereTest2.setOrigin(x2, y2, z2);
	sphereTest2.setRadius(r2_2);
	if (sphereTest2.getX() == x2 && sphereTest2.getY() == y2 && sphereTest2.getZ() == z2 && sphereTest2.getR2() == r2_2) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Fail!***" << endl << endl;
	}
	

	//Test 3a:  Does the getIntersection function work on a default sphere?
	cout << "Test 3a:  getIntersection with a default sphere, point = (0, 0, 0), all directions -- ";
	Sphere sphereTest3;
	if (sphereTest3.getIntersection(0, 0, 0, 'x') == 0 && sphereTest3.getIntersection(0, 0, 0, 'y') == 0 && sphereTest3.getIntersection(0, 0, 0, 'z') == 0) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Fail!***" << endl << endl;
	}


	//Test 3b:  Does the getIntersection function work on a default sphere?
	cout << "Test 3b:  getIntersection with a default sphere, point = (1, 1, 1), all directions -- ";
	if (sphereTest3.getIntersection(1, 1, 1, 'x') == -1 && sphereTest3.getIntersection(1, 1, 1, 'y') == -1 && sphereTest3.getIntersection(1, 1, 1, 'z') == -1) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Fail!***" << endl;
		cout << "x: " << sphereTest3.getIntersection(1, 1, 1, 'x') << endl;
		cout << "y: " << sphereTest3.getIntersection(1, 1, 1, 'y') << endl;
		cout << "z: " << sphereTest3.getIntersection(1, 1, 1, 'z') << endl << endl;
	}



	//Test 4a:  Does the getIntersection function work on a default sphere?
	cout << "Test 4a:  getIntersection with a modified sphere, point at origin, all directions -- ";
	Sphere sphereTest4;
	double x4 = 3;
	double y4 = 8;
	double z4 = 97;
	double r2_4 = 25;
	sphereTest4.setOrigin(x4, y4, z4);
	sphereTest4.setRadius(r2_4);
	if (sphereTest4.getIntersection(x4, y4, z4, 'x') == x4 + sqrt(r2_4) && sphereTest4.getIntersection(x4, y4, z4, 'y') == y4 + sqrt(r2_4) && sphereTest4.getIntersection(x4, y4, z4, 'z') == z4 + sqrt(r2_4)) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Fail!***" << endl;
		cout << "x: " << sphereTest4.getIntersection(x4, y4, z4, 'x') << " ?= " << x4 + sqrt(r2_4) << endl;
		cout << "y: " << sphereTest4.getIntersection(x4, y4, z4, 'y') << " ?= " << y4 + sqrt(r2_4) << endl;
		cout << "z: " << sphereTest4.getIntersection(x4, y4, z4, 'z') << " ?= " << z4 + sqrt(r2_4) << endl << endl;
	}


	//Test 4b:  Does the getIntersection function work on a modified sphere?
	double x4b = -0.8;
	double y4b = 9.3;
	double z4b = 96.9;
	cout << "Test 4b:  getIntersection with a modified sphere, point inside the circle, all directions -- ";
	if (sphereTest4.getIntersection(x4b, y4b, z4b, 'x') == 7.82701 && sphereTest4.getIntersection(x4b, y4b, z4b, 'y') == 11.2481 && sphereTest4.getIntersection(x4b, y4b, z4b, 'z') == 99.9783) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Manual Check!***" << endl;
		cout << "x: " << sphereTest4.getIntersection(x4b, y4b, z4b, 'x') << " should be 7.82701 " << endl;
		cout << "y: " << sphereTest4.getIntersection(x4b, y4b, z4b, 'y') << " should be 11.2481 " << endl;
		cout << "z: " << sphereTest4.getIntersection(x4b, y4b, z4b, 'z') << " should be 99.9783 " << endl << endl;
	}

	//Test 4c:  Does the getIntersection function work on a default sphere?
	cout << "Test 4c:  getIntersection with a modified sphere, point outside the circle, all directions -- ";
	if (sphereTest3.getIntersection(-3, 10, 98, 'x') == 7.47214 && sphereTest3.getIntersection(-3, 10, 98, 'y') == -1 && sphereTest3.getIntersection(-3, 10, 98, 'z') == -1) {
		cout << "Pass" << endl << endl;
	}
	else {
		cout << "***Manual Check!***" << endl;
		cout << "x: " << sphereTest4.getIntersection(-3, 10, 98, 'x') << " should be 7.47214" << endl;
		cout << "y: " << sphereTest4.getIntersection(-3, 10, 98, 'y') << " should be -1" << endl;
		cout << "z: " << sphereTest4.getIntersection(-3, 10, 98, 'z') << " should be -1" << endl << endl;
	}



	return 0;
}