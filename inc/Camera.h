#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Point.h"
#include "Vector.h"
#include <math.h>

class Camera {
protected:
	Point _LookAt;
	Point _Location;
	Vector _UpVec;

public:
	Camera();
	Camera(Point lookAt, Point location);

	virtual bool IsArcBall(){return false;}

	Point getLookAt(){ return _LookAt; }
	void setLookAt(Point p){ _LookAt = p; }

	virtual void Recompute() = 0;
	virtual void Update() = 0;

	virtual void Print();
};

class ArcBall : public Camera {
	double _Radius;
	double _Phi, _Theta;

public:
	ArcBall();
	ArcBall(Point lookAt, double r);

	bool IsArcBall(){return true;}

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

#endif