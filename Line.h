#pragma once

#include <iostream>
#include "Eigen/Dense"

class Line;
#include "Point.h"

using namespace std;
using namespace Eigen;

//Represents a homogene line in the projective plane RP².
//An instance of this class is immutable.
class Line
{
public:
	typedef Vector3f homogeneVector_t;
	static const Line FAR_LINE;

	Line(void);
	//Constructs the line by the specified homogone coordinates
	Line(homogeneVector_t homogeneVector);
	//Creates the line that crosses these two points
	Line(const Point& p1, const Point& p2);

	~Line(void);

	//Tests if the line is natural, so it can be dehomogenized
	bool isNatural() const;
	//a null line is not defined, it is the result of crossing two identical points
	bool isNull() const;
	//the far line is the line that results in the cross of two infinite points
	bool isFar() const;

	//Cuts this line with another line, the result is the cut point.
	Point cut(const Line& other) const;

	//Tests if the specified is incident to this line (the point lies on this line)
	bool isIncident(const Point& p) const;

	//Tests if this line is parallel (or identical) to the other line.
	bool isParallel(const Line& other) const;

	bool operator==(const Line& rhs) const;
	bool operator!=(const Line& rhs) const;

	// equals to cut(other)
	Point operator*(const Line& other) const;

	friend ostream& operator<<(ostream& os, const Line& l);
public:
	const homogeneVector_t vector;
};
ostream& operator<<(ostream& os, const Line& l);
