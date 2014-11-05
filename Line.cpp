#include "stdafx.h"
#include "Line.h"

const Line Line::FAR_LINE = Line(Vector3f(0,0,1));

Line::Line(void)
{
}

Line::Line(homogeneVector_t homogeneVector)
	: vector (homogeneVector)
{
}

Line::Line(const Point& p1, const Point& p2)
	: vector (p1.vector.cross(p2.vector))
{
}

Line::~Line(void)
{
}

bool Line::isNatural() const
{
	return !(isNull() || isFar());
}

bool Line::isNull() const
{
	return vector.isZero();
}

bool Line::isFar() const
{
	return *this == FAR_LINE;
}

Point Line::cut(const Line& other) const
{
	return Point(vector.cross(other.vector));
}

bool Line::isIncident(const Point& p) const
{
	return vector.dot(p.vector) == 0;
}

bool Line::isParallel(const Line& other) const
{
	homogeneVector_t v = vector.cross(other.vector);
	v.z() = 0;
	return v.isZero();
}

bool Line::operator==(const Line& rhs) const
{
	return vector.x()*rhs.vector.z() == rhs.vector.x()*vector.z()
		&& vector.y()*rhs.vector.z() == rhs.vector.y()*vector.z();
}
bool Line::operator!=(const Line& rhs) const
{
	return !operator==(rhs);
}
Point Line::operator*(const Line& other) const
{
	return cut(other);
}
ostream& operator<<(ostream& os, const Line& l)
{
	os << "(" << l.vector.x() << "|" << l.vector.y() << "|" << l.vector.z() << ")";
	return os;
}