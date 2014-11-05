#include "stdafx.h"
#include "LineTools.h"


LineTools::LineTools(void)
{
}


LineTools::~LineTools(void)
{
}

Line LineTools::parallelLineThroughPoint(const Line& line, const Point& point)
{
	/*Point Q = line * Line::FAR_LINE;
	return Q * point;*/

	//faster:
	return (line.vector.cross(Line::FAR_LINE.vector)).cross(point.vector);
}