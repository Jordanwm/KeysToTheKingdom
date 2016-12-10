#pragma once

#include "Point.h"

class Bubble {
	static const double _MaxHeight = 0.5;
	static const double _MinHeight = 0;

	Point _Location;
	bool _up;

public:
	Bubble();
	Bubble(double x, double b);
	
	void Update();
	void Draw();

	void setLocation(Point x){_Location = x;}
	Point getLocation() { return _Location; }
};

