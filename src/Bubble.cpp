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

Bubble::Bubble(float a, float b) {
	x = a; 
	z = b;
	rotation = 0;
	float ran = (rand() % 41 + 80) / 100;
	y = ran;
	/*int random = (rand() % 2);
	if (random == 1) {
		up = true;
	}
	else {
		up = false;
	}*/
}

void Bubble::draw() {
	//glTranslatef(x, y, z);
	glRotatef(rotation, 0, 1, 0);
	glPushMatrix(); {		
		glColor3f(.11, .97, .36);
		//glutWireTorus(0.74, 0.75, 5, 30);	
		glBegin(GL_LINE_STRIP);
		for (double i = 0; i < 2 * M_PI; i = i + (M_PI / 50)) {
			glVertex3f(0, cos(i) / 4 * 3, sin(i) / 4 * 3);
		}
		glEnd();
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

void Bubble::setX(float a) {
	x = a;
}

void Bubble::setY(float a) {
	y = a;
}

void Bubble::setZ(float a) {
	z = a;
}