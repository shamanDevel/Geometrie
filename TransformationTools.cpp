#include "stdafx.h"
#include "TransformationTools.h"

#include <math.h>
#include "Eigen\Dense"

using namespace Eigen;

TransformationTools::TransformationTools(void)
{
}


TransformationTools::~TransformationTools(void)
{
}

Transformation TransformationTools::rotateAroundPoint(Transformation::scalar_t angle, const Point& point)
{
	Point::euclidianVector_t coords = point.dehomogenize();
	//Move point into center
	Transformation move1 = Transformation::Translation(-coords.x(), -coords.y());
	//Rotate around origin
	Transformation rot = Transformation::Rotation(angle);
	//Move back
	Transformation move2 = Transformation::Translation(coords.x(), coords.y());
	//combine
	return move2 * rot * move1;
}

Transformation TransformationTools::axialMirrorAtALine(const Line& line)
{
	assert (line.isNatural());
	//get two points lying on the line
	if (line.vector.x() == 0) {
		//parallel to y axis
		Point p1 = Point(Point::euclidianVector_t(0, -line.vector.z() / line.vector.y()));
		Point p2 = Point(Point::euclidianVector_t(1, -line.vector.z() / line.vector.y()));
		return axialMirrorAtALine(p1, p2);
	} else if (line.vector.y() == 0) {
		//parallel to x axis
		Point p1 = Point(Point::euclidianVector_t(-line.vector.z() / line.vector.x(), 0));
		Point p2 = Point(Point::euclidianVector_t(-line.vector.z() / line.vector.x(), 1));
		return axialMirrorAtALine(p1, p2);
	} else {
		//normal line
		Point p1 = Point(Point::euclidianVector_t(0, -line.vector.z()/line.vector.y()));
		Point p2 = Point(Point::euclidianVector_t(1, (-line.vector.x() - line.vector.z())/line.vector.y()));
		return axialMirrorAtALine(p1, p2);
	}
}

Transformation TransformationTools::axialMirrorAtALine(const Point& p1, const Point& p2)
{
	Point::euclidianVector_t c1 = p1.dehomogenize();
	Point::euclidianVector_t c2 = p2.dehomogenize();
	Point::euclidianVector_t c = c2 - c1;
	Transformation::scalar_t angle = atan2(c.y(), c.x());
	//transform line to x axis
	Transformation t = Transformation::Rotation(angle) * Transformation::Translation(-c1.x(), -c1.y());
	Transformation tInv = t.inverse();
	//mirror at x axis
	Transformation m = Transformation::Scaling(1, -1);
	//combine
	return tInv * m * t;
}

Transformation TransformationTools::pointReflectionAtAPoint(const Point& point)
{
	Point::euclidianVector_t coords = point.dehomogenize();
	//Move point into center
	Transformation move1 = Transformation::Translation(-coords.x(), -coords.y());
	//reflect around origin
	Transformation refl = Transformation::Scaling(-1, -1);
	//Move back
	Transformation move2 = Transformation::Translation(coords.x(), coords.y());
	//combine
	return move2 * refl * move1;
}

Transformation TransformationTools::calcAffineTransformation(transformedPoint a, transformedPoint b, transformedPoint c)
{
	//Can be optimized, so that vector va and vb are combined to one matrix (va|vb)
	//that is solved at the same time.

	Matrix3f ma;
	ma << a.first.vector.x(), a.first.vector.y(), a.first.vector.z(), 
		  b.first.vector.x(), b.first.vector.y(), b.first.vector.z(),
		  c.first.vector.x(), c.first.vector.y(), c.first.vector.z();
	Vector3f va;
	va << a.second.vector.x() / a.second.vector.z(),
		  b.second.vector.x() / a.second.vector.z(),
		  c.second.vector.x() / a.second.vector.z();

	Matrix3f mb = ma;
	Vector3f vb;
	vb << a.second.vector.y() / a.second.vector.z(),
		  b.second.vector.y() / a.second.vector.z(),
		  c.second.vector.y() / a.second.vector.z();

	Vector3f xa = ma.fullPivLu().solve(va);
	Vector3f xb = mb.fullPivLu().solve(vb);

	Matrix3f m;
	m << xa.x(), xa.y(), xa.z(),
		 xb.x(), xb.y(), xb.z(),
		 0     , 0     , 1     ;

	return Transformation(m);
}

Transformation TransformationTools::calcProjectiveTransformation
		(transformedPoint a, transformedPoint b, transformedPoint c, transformedPoint d)
{
	//transform to the images
	Matrix3f A1;
	A1 << a.second.vector.x(), b.second.vector.x(), c.second.vector.x(),
		  a.second.vector.y(), b.second.vector.y(), c.second.vector.y(),
		  a.second.vector.z(), b.second.vector.z(), c.second.vector.z();
	Vector3f xa = A1.fullPivLu().solve(d.second.vector);
	Matrix3f M1;
	M1 << a.second.vector.x() * xa.x(), b.second.vector.x() * xa.y(), c.second.vector.x() * xa.z(),
		  a.second.vector.y() * xa.x(), b.second.vector.y() * xa.y(), c.second.vector.y() * xa.z(),
		  a.second.vector.z() * xa.x(), b.second.vector.z() * xa.y(), c.second.vector.z() * xa.z();

	//transform to the sources
	Matrix3f A2;
	A2 << a.first.vector.x(), b.first.vector.x(), c.first.vector.x(),
		  a.first.vector.y(), b.first.vector.y(), c.first.vector.y(),
		  a.first.vector.z(), b.first.vector.z(), c.first.vector.z();
	Vector3f xb = A2.fullPivLu().solve(d.first.vector);
	Matrix3f M2;
	M2 << a.first.vector.x() * xb.x(), b.first.vector.x() * xb.y(), c.first.vector.x() * xb.z(),
		  a.first.vector.y() * xb.x(), b.first.vector.y() * xb.y(), c.first.vector.y() * xb.z(),
		  a.first.vector.z() * xb.x(), b.first.vector.z() * xb.y(), c.first.vector.z() * xb.z();

	Matrix3f M = M1 * M2.inverse();

	return Transformation(M);
}