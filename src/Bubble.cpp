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

Bubble::Bubble(double x, double z) {
	_X = x; 
	_Y = 0;
	_Z = z;

	//rotation = 0;
	//float ran = (rand() % 41 + 80) / 100;
	//y = ran;
	/*int random = (rand() % 2);
	if (random == 1) {
		up = true;
	}
	else {
		up = false;
	}*/
}

void Bubble::Draw() {
	glPushMatrix(); {
		glTranslatef(_X, _Y, _Z);		
		glColor4f(.11, .97, .36, 0.5);
        glutSolidSphere(0.75, 60, 60);
    }; glPopMatrix();
}

void Bubble::evolve() {
	rotation = rotation + 250;
	//if (up && y <= 1.1) {
	//	//y = y + 0.05;
	//	yChange = .05;
	//}
	//else if (up && y > 1.1) {
	//	up = false;
	//	//y = y - 0.05;
	//	yChange = -.05;
	//}
	//else if (!up && y >= 0.9) {
	//	//y = y - 0.05;
	//	yChange = -.05;
	//}
	//else {
	//	up = true;
	//	//y = y + 0.05;
	//	yChange = .05;
	//}

}