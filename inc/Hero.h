#ifndef _HERO_H_
#define _HERO_H_

#include "Point.h"
#include "Vector.h"

class Hero {
protected:
	Point _Location;
	double _Health;

public:
	Hero();

	Point getLocation(){ return _Location; }
	void setLocation(Point p){ _Location = p; }

	double getHealth(){ return _Health; }
	void setHealth(double h){ _Health = h; }
	void incrementHealth(double h){_Health += h;}

	bool isDead(){return _Health < 0;}

	virtual void Draw() = 0;
	virtual void Update() = 0;
};

class Plane : public Hero {
	double wing_rotation;
    double prop_rotation;
    double rotate_angle;

public:
	Plane();
	void Draw();
	void Update();

	void setWingRotation(double r){wing_rotation = r;}
	double getWingRotation(){return wing_rotation;}

	void setPropRotation(double r){prop_rotation = r;}
	double getPropRotation(){return prop_rotation;}

	void setRotateAngle(double r){rotate_angle = r;}
	double getRotateAngle(){return rotate_angle;}

// Helper Draw Functions
	void DrawBody();
	void DrawWing();
	void DrawProp();
};

#endif