#pragma once

#include <iostream>
#include "Eigen/Dense"
#include "Point.h"
#include "Line.h"

using namespace std;
using namespace Eigen;

//Represents a transformation (affine and projective) of points and lines
//in the projective plane RP².
//An instance of this class is immutable.
class Transformation
{
public:
	typedef Matrix3f matrix_t;
	typedef float scalar_t;

	Transformation(void);
	
	//Constructs the transformation by the specified matrix.
	//The inverse is automatically calulcated.
	//It throws an exception if the matrix is not invertible.
	Transformation(const matrix_t& mat);

	//Constructs the transformation by the specified matrix
	//and uses the specified inverse matrix directly
	//(no need to calculate it).
	//An exception is thrown if mat*inv != id
	Transformation(const matrix_t& mat, const matrix_t& inv);

	~Transformation(void);

	//Returns the inverse transformation
	Transformation inverse() const;

	//Applies the transformation to the specified point
	Point apply(const Point& p) const;

	//Applies the transformation to the specified line
	Line apply(const Line& l) const;

	//applies this transformation AFTER the specified transformation
	//and returns the combination of them.
	Transformation combine(const Transformation& rhs) const;

	//The identity transformation
	static const Transformation IDENTITY;
	//Constructs the identity transformation
	static Transformation Identity();

	//The rotation transformation around the origin using the
	//angle 'alpha' in radians.
	static Transformation Rotation(scalar_t alpha);

	//The translation of (x,y)
	static Transformation Translation(scalar_t x, scalar_t y);

	//The scaling transformation with the factors (sx, sy)
	static Transformation Scaling(scalar_t sx, scalar_t sy);

	//Shearing of factor m to x-axis and of factor n to y-axis
	static Transformation Shear(scalar_t m, scalar_t n);

	bool operator==(const Transformation& rhs) const;
	bool operator!=(const Transformation& rhs) const;
	//delegates to combine()
	Transformation operator*(const Transformation& rhs) const;
	//delegates to apply
	Point operator*(const Point& rhs);
	//delegates to apply
	Line operator*(const Line& rhs);

	friend ostream& operator<<(ostream& os, const Transformation& l);
public:
	const matrix_t matrix;
	const matrix_t invMat;
};

ostream& operator<<(ostream& os, const Transformation& l);