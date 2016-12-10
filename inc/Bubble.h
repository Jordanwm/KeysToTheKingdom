#pragma once

class Bubble {

public:
	Bubble();
	Bubble(float a, float b);
	void evolve();
	void draw();
	float getX() { return x; }
	void setX(float a);
	void setY(float a);
	void setZ(float a);
	float getY() { return y; }
	float getZ() { return z; }
	bool up;
	float rotation;

private:
	float x, y, z, yChange;

};

