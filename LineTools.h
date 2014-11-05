#pragma once

#include "Point.h"
#include "Line.h"

//Toolkit for working with lines and points
class LineTools
{
private:
	LineTools(void);
	~LineTools(void);

public:
	//Constructs the line that is parallel to the specifie line and cuts the
	//specified point
	static Line parallelLineThroughPoint(const Line& line, const Point& point);
};

