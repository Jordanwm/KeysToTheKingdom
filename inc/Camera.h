#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Point.h"
#include "Vector.h"
#include <math.h>
#include "Bezier.h"

class Camera {
protected:
	Point _LookAt;
	Point _Location;
	Vector _UpVec;
	BezierCurve path;

public:
	Camera();
	Camera(Point lookAt, Point location);

	virtual bool IsArcBall(){return false;}
	virtual bool IsTopDown(){return false;}

	Point getLookAt(){ return _LookAt; }
	void setLookAt(Point p){ _LookAt = p; }

	virtual void Recompute() = 0;
	virtual void Update() = 0;
	virtual void setZ(double a) = 0;
	virtual void setDirection(Point a) = 0;
	virtual void Print();
};

class ArcBall : public Camera {
	double _Radius;
	double _Phi, _Theta;
	Point direction;
	double z;

public:
	ArcBall();
	ArcBall(Point lookAt, double r);

	bool IsArcBall(){return true;}

	void setZ(double a) { z = a; }
	void setDirection(Point a) { direction = a; }

	double getRadius(){ return _Radius; }
	void setRadius(double r)
	{
		if (r > 0) 
			_Radius = r; 
	}

	double getPhi(){ return _Phi; }
	void setPhi(double p)
	{ 
		if (p >= 0 && p <= M_PI)
			_Phi = p; 
	}
	
	double getTheta(){ return _Theta; }
	void setTheta(double t){ _Theta = t; }

	void Recompute();
	void Update();
};

class TopDown : public Camera {
	double _Radius;
	double z;
	Point direction;
public:
	TopDown();

	bool IsTopDown(){return true;}
	void setZ(double a) { z = a; }
	void setUpVec(Vector v) {_UpVec = v;}
	void setDirection(Point a) { direction = a; }
	void Recompute();
	void Update();
};

#endif