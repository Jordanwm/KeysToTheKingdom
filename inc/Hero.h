#ifndef _HERO_H_
#define _HERO_H_

#include "Point.h"
#include "Vector.h"

class Hero {
protected:
	Point _Location;

public:
	Hero();

	Point getLocation(){ return _Location; }
	void setLocation(Point p){ _Location = p; }

	virtual void Draw() = 0;
	virtual void Update() = 0;
};

class Plane : public Hero {
	double wheel_rotation;
    double prop_rotation;
    double rotate_angle;

public:
	Plane();
	void Draw();
	void Update();

	void setWheelRotation(double r){wheel_rotation = r;}
	double getWheelRotation(){return wheel_rotation;}

	void setPropRotation(double r){prop_rotation = r;}
	double getPropRotation(){return prop_rotation;}

	void setRotateAngle(double r){rotate_angle = r;}
	double getRotateAngle(){return rotate_angle;}

// Helper Draw Functions
	void DrawBody();
	void DrawWheel();
	void DrawProp();
};

#endif