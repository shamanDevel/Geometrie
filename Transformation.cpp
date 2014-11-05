#include "stdafx.h"
#include "Transformation.h"
#include <assert.h>
#include <math.h>

Transformation::Transformation(void)
{
}

Transformation::Transformation(const matrix_t& mat)
	: matrix(mat), invMat(mat.inverse())
{
	assert (mat.determinant() != 0);
}
Transformation::Transformation(const matrix_t& mat, const matrix_t& inv)
	: matrix(mat), invMat(inv)
{
	assert (mat.determinant() != 0);
	assert (inv.determinant() != 0);
	assert ((mat * inv).isIdentity());
}
Transformation::~Transformation(void)
{
}

Transformation Transformation::inverse() const
{
	return Transformation(invMat, matrix);
}

Point Transformation::apply(const Point& p) const
{
	return Point((Point::homogeneVector_t) (matrix * p.vector));
}

Line Transformation::apply(const Line& l) const
{
	return Line(invMat.transpose() * l.vector);
}

Transformation Transformation::combine(const Transformation& rhs) const
{
	return Transformation(matrix * rhs.matrix, rhs.invMat * invMat);
}

const Transformation Transformation::IDENTITY = Transformation(Transformation::matrix_t::Identity(), Transformation::matrix_t::Identity());
Transformation Transformation::Identity()
{
	return IDENTITY;
}
Transformation Transformation::Rotation(Transformation::scalar_t angle)
{
	scalar_t s = sin(angle);
	scalar_t c = cos(angle);
	matrix_t mat;
	mat << c, -s, 0,
		   s,  c, 0,
		   0,  0, 1;
	s = sin(-angle);
	c = cos(-angle);
	matrix_t inv;
	inv << c, -s, 0,
		   s,  c, 0,
		   0,  0, 1;
	return Transformation(mat, inv);
}
Transformation Transformation::Translation(scalar_t x, scalar_t y)
{
	matrix_t mat;
	mat << 1, 0, x,
		   0, 1, y,
		   0, 0, 1;
	matrix_t inv;
	inv << 1, 0, -x,
		   0, 1, -y,
		   0, 0,  1;
	return Transformation(mat, inv);
}
Transformation Transformation::Scaling(scalar_t sx, scalar_t sy)
{
	matrix_t mat;
	mat << sx, 0, 0,
		   0, sy, 0,
		   0,  0, 1;
	matrix_t inv;
	inv << 1/sx, 0, 0,
		   0, 1/sy, 0,
		   0,    0, 1;
	return Transformation(mat, inv);
}
Transformation Transformation::Shear(scalar_t m, scalar_t n)
{
	matrix_t mat;
	mat << 1, m, 0,
		   n, 1, 0,
		   0,  0, 1;
	matrix_t inv;
	inv << 1, -m, 0,
		  -n,  1, 0,
		   0,  0, 1;
	return Transformation(mat, inv);
}

bool Transformation::operator==(const Transformation& rhs) const
{
	for (int x=0; x<3; ++x) {
		for (int y=0; y<3; ++y) {
			if (matrix(x,y)!=0 && rhs.matrix(x,y)!=0) {
				//use this entry as scaling
				return (matrix*rhs.matrix(x,y) - rhs.matrix*matrix(x,y)).isZero(0.0001);
			}
		}
	}
	return false;
}
bool Transformation::operator!=(const Transformation& rhs) const
{
	return !(*this == rhs);
}
Transformation Transformation::operator*(const Transformation& rhs) const
{
	return combine(rhs);
}
Point Transformation::operator*(const Point& rhs)
{
	return apply(rhs);
}
Line Transformation::operator*(const Line& rhs)
{
	return apply(rhs);
}
ostream& operator<<(ostream& os, const Transformation& l)
{
	os << l.matrix;
	return os;
}