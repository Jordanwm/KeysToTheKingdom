#include "Camera.h"

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <iostream>

Camera::Camera()
{
	_LookAt = Point(0.0, 0.0, 0.0);
	_Location = Point(1.0, 1.0, 1.0);
	_UpVec = Vector(0,1,0);
}
Camera::Camera(Point lookAt, Point location)
{
	_LookAt = lookAt;
	_Location = location;
	_UpVec = Vector(0,1,0);
}

void Camera::Print()
{
	std::cout << "LookAt:" << _LookAt.toString() 
		<< " Location: " << _Location.toString() << std::endl;
}

ArcBall::ArcBall()
	:Camera()
{
	_Theta = M_PI;
	_Phi = 3.5 * M_PI/8.0;
	_Radius = 10;
	Recompute();
	Update();
}
ArcBall::ArcBall(Point lookAt, double r)
	:Camera(lookAt, Point(0.0, 0.0, 0.0))
{
	_Theta = M_PI;
	_Phi = M_PI/4.0;
	_Radius = r;
	Recompute();
	Update();
}
void ArcBall::Recompute()
{
	_Location.setX( _LookAt.getX() + _Radius * sin(_Phi) * cos(_Theta) );
    _Location.setY( _LookAt.getY() + _Radius * cos(_Phi) );
    _Location.setZ( _LookAt.getZ() + _Radius * sin(_Phi) * sin(_Theta) );
}
void ArcBall::Update()
{
    gluLookAt(_Location.getX(), _Location.getY(), _Location.getZ(),
    	_LookAt.getX(), _LookAt.getY(), _LookAt.getZ(),
    	_UpVec.getX(), _UpVec.getY(), _UpVec.getZ());
}