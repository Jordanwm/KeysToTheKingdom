#pragma once

class Bubble {
	double _X, _Y, _Z, yChange;
	bool up;
	float rotation;

public:
	Bubble();
	Bubble(double x, double b);
	void evolve();
	void Draw();

	void setX(double x){_X = x;}
	void setY(double y){_Y = y;}
	void setZ(double z){_Z = z;}
	double getX() { return _X; }
	double getY() { return _Y; }
	double getZ() { return _Z; }
};

