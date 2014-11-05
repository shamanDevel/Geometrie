#pragma once

#include <iostream>
#include "Eigen/Dense"

class Point;
#include "Line.h"

using namespace std;
using namespace Eigen;

//Represents a 2d point in the projective plane RP².
//An instance of this class is immutable.
class Point
{
public:
	typedef Vector2f euclidianVector_t;
	typedef Vector3f homogeneVector_t;

public:
	const homogeneVector_t vector;
	
public:
	Point(void);
	//Constructs the point by homogenize the specified euclidian vecotr
	explicit Point(euclidianVector_t euclidianVector);
	//Constructs the point by using the specified homogene coordinates
	Point(homogeneVector_t homogeneVector);
	~Point(void);

	//Tests if this point is natural, it is then no far point and not the null point.
	//A natural point can be dehomogenized
	bool isNatural() const;
	//Tests if this point is a far point
	bool isFar() const;
	//Tests if this point is the null point (0,0,0).
	//This illegal point results in cutting two identical lines
	bool isNull() const;

	//Dehomogenizes the point.
	//An exception is thrown if the point is not natural
	euclidianVector_t dehomogenize() const;

	//Connects this point with another point to define a line through these two pointss
	Line connect(const Point& other) const;

	bool operator==(const Point& rhs) const;
	bool operator!=(const Point& rhs) const;
	//equals to connect(other)
	Line operator*(const Point& other) const;

	friend ostream& operator<<(ostream& os, const Point& dt);
};

ostream& operator<<(ostream& os, const Point& dt);