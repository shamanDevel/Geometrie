#pragma once

#include "Point.h"
#include "Line.h"
#include "Transformation.h"

//Toolkit for working with transformations
class TransformationTools
{
private:
	TransformationTools(void);
	~TransformationTools(void);

public:
	//Creates the transformation that rotates around the specified point 
	//using the specified angle in radians.
	//The point must be natural
	static Transformation rotateAroundPoint(Transformation::scalar_t angle, const Point& point);

	//Creates the transformation that performs an axial mirror at the specified line.
	//The line must be natural.
	static Transformation axialMirrorAtALine(const Line& line);

	//Creates the transformation that performs an axial mirror at the specified
	//line defined by the two points.
	//The points must be natural and not equal.
	static Transformation axialMirrorAtALine(const Point& p1, const Point& p2);

	//Creates the transformation that performs a point reflection at the
	//specified point.
	//The point must be natural.
	static Transformation pointReflectionAtAPoint(const Point& point);

	//Describes the transformation of a single point.
	//First entry: the source point
	//Second entry: the target point
	typedef pair<Point, Point> transformedPoint;

	//Calculates the affine transformation defined by three source vectors and their images.
	//The points have to be natural
	static Transformation calcAffineTransformation(transformedPoint a, transformedPoint b, transformedPoint c);

	//Calculates the projective transformation defined by four source vectors and their images.
	//THe points can be far points, but must not be null.
	static Transformation calcProjectiveTransformation
		(transformedPoint a, transformedPoint b, transformedPoint c, transformedPoint d);
};

