// Geometrie.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include "Eigen\Dense"
#include "Point.h"
#include "Line.h"
#include "Transformation.h"
#include "TransformationTools.h"
#include "LieCircle.h"

using namespace std;
using namespace Eigen;

int main(int argc, char** argv)
{
	Point p1;
	Point p2(Vector2f(0, 2));
	Point p3(Vector3f(3, 4, 1));
	Point p4(Vector3f(6, 8, 2));
	assert (p3 == p4);

	Line l1;
	Line l2(Vector3f(1, 0, 0)); //y-axis
	assert (l2.isIncident(p2));
	Line l3(p3, p4);
	assert (l3.isNull());
	Line l4 = p2*p3;
	assert (l3.isIncident(p2));
	assert (l3.isIncident(p3));
	Point p5 = l2.cut(l4);
	assert (p5.isNatural());

	cout << "l3: " << l3 << endl;
	cout << "l4: " << l4 << endl;
	cout << "p5: " << p5 << endl;

	Line l5 = Line::FAR_LINE;
	assert (l5.isFar());
	Point p6 = l5*l2;
	assert (p6.isFar());

	Transformation id = Transformation::Identity();
	assert (p4 == id.apply(p4));
	assert (l4 == id.apply(l4));
	Transformation rot = Transformation::Rotation(M_PI / 2);
	Line l6 = rot.apply(l2);
	Point p7 = rot.apply(p2);
	Transformation rot2 = Transformation::Rotation(M_PI / 2 + 2*M_PI);
	Transformation reflY = Transformation::Scaling(1, -1);
	Line l7 = reflY.apply(l2);
	Point p8 = reflY.apply(p2);
	Transformation shear = Transformation::Shear(1, 0);
	assert(id != rot);
	assert(rot != reflY);
	cout << "rot1:" << endl << rot << endl;
	cout << "rot2:" << endl << rot2 << endl;
	assert(rot == rot2);

	//Hausaufgabe Blatt 2, H6
	Point au (Point::homogeneVector_t(1, 3, 1));
	Point ab (Point::homogeneVector_t(0, -2, 1));
	Point bu (Point::homogeneVector_t(3, 3, 1));
	Point bb (Point::homogeneVector_t(2, -1, 1));
	Point cu (Point::homogeneVector_t(-3, -1, 1));
	Point cb (Point::homogeneVector_t(-6, 2, 1));
	Transformation M = TransformationTools::calcAffineTransformation(make_pair(au, ab), make_pair(bu, bb), make_pair(cu, cb));
	cout << "affine transformation M" << endl;
	cout << M << endl;
	Point ar = M * au;
	Point br = M * bu;
	Point cr = M * cu;
	assert (ar == ab);
	assert (br == bb);
	assert (cr == cb);

	//Hausaufgabe zu Lie-Kreisen
	cout << endl << "Lie-Circles" << endl;
	LieCircle a (2, 3, 3);
	Point p (Point::euclidianVector_t(1,1));
	Point ey (Point::euclidianVector_t(0,1));
	LieCircle b = LieCircle::fromPoint(p);
	pair<LieCircle, LieCircle> c = LieCircle::fromLine(ey*p);
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "c = " << c.first << "  and " << c.second << endl;
	assert (a.touch(a));
	assert (b.touch(b));
	assert (c.first.touch(c.first));
	assert (c.second.touch(c.second));
	assert (b.touch(c.first));
	vector<LieCircle> results1 = LieCircle::apollonius(a,b,c.first);
	vector<LieCircle> results2 = LieCircle::apollonius(a,b,c.second);
	cout << "solutions:" << endl;
	for (LieCircle c : results1)
		cout << c << endl;
	cout << " and" << endl;
	for (LieCircle c : results2)
		cout << c << endl;

	cout << endl << "Enter zum Verlassen";
	cin.get();
	return 0;
}

