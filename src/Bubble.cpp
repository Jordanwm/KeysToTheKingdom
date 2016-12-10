#include "Bubble.h"

#ifdef __APPLE__            // if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <iostream>

extern double getRand();

Bubble::Bubble(double x, double z) {
	_Location = Point(x, 
		getRand() * (_MaxHeight - _MinHeight) + _MinHeight,
		z);
}

void Bubble::Draw() {
	glPushMatrix(); {
		_Location.glTranslate();		
		glColor4f(.11, .97, .36, 0.5);
        glutSolidSphere(0.75, 60, 60);
    }; glPopMatrix();
}

void Bubble::Update() {
	double speed = 0.01;

	if (_up)
		_Location.setY(_Location.getY() + speed);
	else 
		_Location.setY(_Location.getY() - speed);

	if (_Location.getY() > _MaxHeight)
		_up = false;
	if (_Location.getY() < _MinHeight)
		_up = true;
}