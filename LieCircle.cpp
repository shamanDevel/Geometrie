#include "stdafx.h"
#include "LieCircle.h"
#include <cmath>
#include <vector>

LieCircle::LieCircle(void)
{
}


LieCircle::~LieCircle(void)
{
}

LieCircle::LieCircle(vector_t vector)
	: vector(vector)
{
}

LieCircle::vector_t createVector(LieCircle::scalar_t a, LieCircle::scalar_t b, LieCircle::scalar_t c, LieCircle::scalar_t d, LieCircle::scalar_t e)
{
	LieCircle::vector_t v;
	v << a, b, c, d, e;
	return v;
}

LieCircle::LieCircle(scalar_t x, scalar_t y, scalar_t r)
	: vector(createVector(x, y, r, x*x + y*y - r*r, 1))
{
}

LieCircle LieCircle::fromPoint(const Point& point)
{
	Point::homogeneVector_t v = point.vector;
	return LieCircle(createVector(v.x()*v.z(), v.y()*v.z(), 0, v.x()*v.x() + v.y()*v.y(), v.z()*v.z()));
}

pair<LieCircle, LieCircle> LieCircle::fromLine(const Line& line)
{
	Line::homogeneVector_t v = line.vector;
	LieCircle::scalar_t root = sqrt(v.x()*v.x() + v.y()*v.y());
	return make_pair(
		LieCircle(createVector(v.x(), v.y(),  root, -2*v.z(), 0)),
		LieCircle(createVector(v.x(), v.y(), -root, -2*v.z(), 0))
	);
}

bool LieCircle::touch(const LieCircle& other)
{
	return vector[3]*other.vector[4] + vector[4]*other.vector[3]
		- 2*vector[0]*other.vector[0] - 2*vector[1]*other.vector[1]
		+ 2*vector[2]*other.vector[2]
		== 0;
}

vector<LieCircle> LieCircle::apollonius(const LieCircle& ca, const LieCircle& cb, const LieCircle& cc)
{
	vector_t a = ca.vector;
	vector_t b = cb.vector;
	vector_t c = cc.vector;
	::std::vector<LieCircle> circles;

	Matrix<scalar_t, 3, 5> A;
	A << -2*a[0], -2*a[1], 2*a[2], a[4], a[3],
		 -2*b[0], -2*b[1], 2*b[2], b[4], b[3],
		 -2*c[0], -2*c[1], 2*c[2], c[4], c[3];
	Matrix<scalar_t, 3, 1> O3 (0,0,0);

	//cout << "Matrix:" << endl;
	//cout << A << endl;

	FullPivLU<Matrix<scalar_t, 3, 5>> lu (A);
	//cout << "Image:" << endl;
	//cout << lu.image(A) << endl;
	//cout << "Kernel:" << endl;
	auto kernel = lu.kernel();
	//cout << kernel << endl;

	if (kernel.cols() != 2) {
		cout << "degenerated case, A has a kernel of dimension " << kernel.cols() << endl;
		return circles;
	}

	Matrix<scalar_t, 5, 1> v1 = kernel.col(0);
	Matrix<scalar_t, 5, 1> v2 = kernel.col(1
		);
	//cout << "v1 = " << v1.transpose() << endl;
	//cout << "v2 = " << v2.transpose() << endl;

	//now solve <<a*v1 + b*v2, a*v1 + b.v2>> = 0
	// ==> a²x + b²y + abz = 0   where x,y,z as follow:
	LieCircle::scalar_t x = v1[3]*v1[4] - v1[0]*v1[0] - v1[1]*v1[1] + v1[2]*v1[2];
	LieCircle::scalar_t y = v2[3]*v2[4] - v2[0]*v2[0] - v2[1]*v2[1] + v2[2]*v2[2];
	LieCircle::scalar_t z = v1[3]*v2[4] + v1[4]*v2[3] - 2*v1[0]*v2[0] - 2*v1[1]*v2[1] + 2*v1[2]*v2[2];
	//cout << "a^2x + b^2y + abz = 0   where x,y,z as follow:" << endl;
	//cout << "x=" << x << "  y=" << y << "  z=" << z << endl;

	//set b=1 -> a²x + az + y = 0
	LieCircle::scalar_t det = z*z - 4*x*y;
	static const LieCircle::scalar_t DELTA = 0.00001;
	if (abs(det) < DELTA) det = 0;
	//cout << "det=" << det << endl;
	if (det < 0) {
		//no solution
	} else if (det==0) {
		//one solution
		LieCircle::scalar_t a = -z / (2*x);
		LieCircle c (a*v1 + 1*v2);
		circles.push_back(c);
	} else {
		//two solutions
		LieCircle::scalar_t s = sqrt(det);
		LieCircle::scalar_t a1 = (-z + s) / (2*x);
		LieCircle::scalar_t a2 = (-z - s) / (2*x);
		circles.push_back(LieCircle (a1*v1 + 1*v2));
		circles.push_back(LieCircle (a2*v1 + 1*v2));
	}

	return circles;
}

bool LieCircle::operator==(const Line& rhs) const
{
	static const float DELTA = 0.0001;
	return abs(vector[0]*rhs.vector[4] - rhs.vector[0]*vector[4]) < DELTA
		&& abs(vector[1]*rhs.vector[4] - rhs.vector[1]*vector[4]) < DELTA
		&& abs(vector[2]*rhs.vector[4] - rhs.vector[2]*vector[4]) < DELTA
		&& abs(vector[3]*rhs.vector[4] - rhs.vector[3]*vector[4]) < DELTA;
}
bool LieCircle::operator!=(const Line& rhs) const
{
	return !operator==(rhs);
}

ostream& operator<<(ostream& os, const LieCircle& c)
{
	os << "[" << c.vector.transpose() << "]";
	return os;
}
