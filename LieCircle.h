#pragma once

#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include "Point.h"
#include "Line.h"

using namespace std;
using namespace Eigen;

//Represents a Lie-Circle
class LieCircle
{
public:
	typedef double scalar_t;
	typedef Matrix<scalar_t, 5, 1> vector_t;

	LieCircle(void);
	LieCircle(vector_t vector);
	LieCircle(scalar_t x, scalar_t y, scalar_t radius);

	~LieCircle(void);

	static LieCircle fromPoint(const Point& point);
	//creates a circle from a line
	//There are two possible orientations of the circle, therefore, two circles are returned
	static pair<LieCircle, LieCircle> fromLine(const Line& line);

	//Tests if this circle touches the other circle
	bool touch(const LieCircle& other);

	//Given three circles, it calculates all possibilities of a fourth circle d that touches all theses circles
	static vector<LieCircle> apollonius(const LieCircle& a, const LieCircle& b, const LieCircle& c);

	bool operator==(const Line& rhs) const;
	bool operator!=(const Line& rhs) const;
	friend ostream& operator<<(ostream& os, const LieCircle& c);

public:
	const vector_t vector;
};
ostream& operator<<(ostream& os, const LieCircle& c);
