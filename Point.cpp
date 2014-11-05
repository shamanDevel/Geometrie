#include "stdafx.h"
#include "Point.h"
#include <assert.h>

Point::Point(void)
{
}

Point::Point (euclidianVector_t vector)
	: vector(vector.x(), vector.y(), 1)
{
}

Point::Point(homogeneVector_t homogeneVector)
	: vector(homogeneVector)
{
}

Point::~Point(void)
{
}

 
Point::euclidianVector_t Point::dehomogenize() const
{
	assert (isNatural());
	return euclidianVector_t(vector.x() / vector.z(), vector.y() / vector.z());
}

 
bool Point::isNatural() const
{
	return vector.z() != 0;
}

 
bool Point::isFar() const
{
	return vector.x() == 0 && (vector.x() != 0 || vector.y() != 0);
}

 
bool Point::isNull() const
{
	return vector.isZero();
}

Line Point::connect(const Point& other) const
{
	return Line(*this, other);
}

bool Point::operator==(const Point& rhs) const
{
	static const float DELTA = 0.0001;
	return abs(vector.x()*rhs.vector.z() - rhs.vector.x()*vector.z()) < DELTA
		&& abs(vector.y()*rhs.vector.z() - rhs.vector.y()*vector.z()) < DELTA;
}
bool Point::operator!=(const Point& rhs) const
{
	return !operator==(rhs);
}
Line Point::operator*(const Point& other) const
{
	return connect(other);
}
ostream& operator<<(ostream& os, const Point& dt)
{
	os << "(" << dt.vector.x() << "|" << dt.vector.y() << "|" << dt.vector.z() << ")";
	return os;
}