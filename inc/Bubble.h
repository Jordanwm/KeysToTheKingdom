#pragma once

class Bubble {
	static const double _MaxHeight = 0.5;
	static const double _MinHeight = 0;

	double _X, _Y, _Z, yChange;
	bool _up;
	float rotation;

public:
	Bubble();
	Bubble(double x, double b);
	
	void Update();
	void Draw();

	void setX(double x){_X = x;}
	void setY(double y){_Y = y;}
	void setZ(double z){_Z = z;}
	double getX() { return _X; }
	double getY() { return _Y; }
	double getZ() { return _Z; }
};

