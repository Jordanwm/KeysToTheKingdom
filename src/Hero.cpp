#include "Hero.h"

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "Map.h"
#include <iostream>
#include "math.h"

Hero::Hero()
{
    _Location = Point(0.0,0.0,0.0);
}

Plane::Plane() 
:Hero()
{
    wheel_rotation = 0.0;
    prop_rotation = 0.0;
    rotate_angle = 0.0;
}
	
extern Map* gMap;
void Plane::Update()
{
    prop_rotation += 10;
    Vector dir(0,0,1);
    if (gMap){
        Vector c = cross(dir, gMap->getHeading());
        rotate_angle = angle(dir, gMap->getHeading()) * 180 / M_PI;
        if (c.getY() < 0)
            rotate_angle *= -1;
    }
}

void Plane::DrawProp() {
    glColor3ub(0, 255, 0);
    glPushMatrix(); {
        glScalef(0.2, 2, 0.2);
        glutSolidCube(0.5);
    } glPopMatrix();
    glColor3ub(255,255, 0);
    glPushMatrix(); {
        glScalef(2, 0.2, 0.2);
        glutSolidCube(0.5);
    } glPopMatrix();
    glColor3ub( 0, 0, 0);
    glPushMatrix(); {
        glutSolidSphere(0.1, 15, 15);
    } glPopMatrix();
}

void Plane::DrawWheel() {
    GLUquadric *cylinder;
    GLUquadric *disk;

    //Draw Tire
    glColor3ub(50, 50, 50);
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        gluCylinder(cylinder, 0.5f, 0.5f, 0.25f, 15, 15);
    } glPopMatrix();

    glPushMatrix(); {
        disk = gluNewQuadric();
        gluDisk(disk, 0, 0.5f, 15, 15);
    } glPopMatrix();

    glPushMatrix(); {
        glTranslatef(0, 0, 0.25);
        disk = gluNewQuadric();
        gluDisk(disk, 0, 0.5f, 15, 15);
    } glPopMatrix();


    //Draw rim
    glColor3f(255, 255, 255);
    glPushMatrix(); {
        glTranslatef(0, 0, -0.01f);
        disk = gluNewQuadric();
        gluDisk(disk, 0, 0.4f, 6, 6);
    } glPopMatrix();

}

void Plane::DrawBody() {
    GLUquadric *cylinder;
    GLUquadric *disk;

    glColor3ub(255,25, 25);
    //Main body
    glPushMatrix(); {
        glScalef(1.2, 1, 2.5);
        //glutSolidCube(1);
        glTranslatef(0, 0, -0.5);
        cylinder = gluNewQuadric();
        gluCylinder(cylinder, 0.5f, 0.5f, 1.0f, 15, 15);
    } glPopMatrix();
    //Front sphere
    glPushMatrix(); {
        glScalef(1.2, 1, 2);
        glTranslatef(0, 0, 1.0);
        //glutSolidCube(1);
        glTranslatef(0, 0, -0.5);
        glutSolidSphere(0.5, 15,15);
    } glPopMatrix();
    //Rear sphere
    glPushMatrix(); {
        glScalef(1.2, 1, 1);
        glTranslatef(0, 0, -0.7);
        //glutSolidCube(1);
        glTranslatef(0, 0, -0.5);
        glutSolidSphere(0.5, 15,15);
    } glPopMatrix();
    //Rear Pop shaft
    glColor3ub(20,20,20);
    glPushMatrix(); {
        glScalef(1.2, 1, -2.5);
        //glutSolidCube(1);
        glTranslatef(0, 0, 0);
        cylinder = gluNewQuadric();
        gluCylinder(cylinder, 0.1f, 0.1f, 0.8f, 15, 15);
    } glPopMatrix();


    //Main upper cylinder
    glColor3ub(100, 100, 100);
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, -1);
        gluCylinder(cylinder, 0.5f, 0.5f, 1.0f, 15, 15);
    } glPopMatrix();
    //Front upper cylinder
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, 0);
        gluCylinder(cylinder, 0.5f, 0.0f, 1.8f, 15, 15);
    } glPopMatrix();
    //Rear upper cylinder
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, -1.5);
        gluCylinder(cylinder, 0.0f, 0.5f, 0.5f, 15, 15);
    } glPopMatrix();

    glPushMatrix(); {
        glTranslatef(0,0, -2);
        glRotatef(prop_rotation, 0, 0, 1);
        DrawProp();
    } glPopMatrix();
}

void Plane::Draw()
{
    if (false){ // Axis
	    glDisable(GL_LIGHTING);
	    glBegin(GL_LINES);
	    	glColor3f(1,0,0);
	    	glVertex3f(0,1,0);
	    	glVertex3f(1,1,0);

	    	glColor3f(0,1,0);
	    	glVertex3f(0,1,0);
	    	glVertex3f(0,2,0);

	    	glColor3f(0,0,1);
	    	glVertex3f(0,1,0);
	    	glVertex3f(0,1,1);
	    glEnd();
	    glEnable(GL_LIGHTING);
    }

    glPushMatrix(); {

        _Location.glTranslate();
        glScalef(0.5, 0.5, 0.5);
        glRotatef(rotate_angle, 0, 1, 0);
        
            //Draw body
        glPushMatrix(); {
            DrawBody();
        } glPopMatrix();

        //Draw front axle
        glColor3f(0,0, 255);
        glPushMatrix(); {
            glRotatef(90, 0, 1, 0);
            glTranslatef(-1, 0, -1);
            GLUquadric *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic, 0.1f, 0.1f, 2.0f, 32, 32);
        } glPopMatrix();

        //Front left wheel
        glPushMatrix(); {
            glTranslatef(1, 0, 1);
            glRotatef(270, 0, 1, 0);
            glRotatef(-1 * wheel_rotation, 0, 0, 1);
            DrawWheel();
        } glPopMatrix();

        //Front right wheel
        glPushMatrix(); {
            glTranslatef(-1, 0, 1);
            glRotatef(90, 0, 1, 0);
            glRotatef(wheel_rotation, 0, 0, 1);
            DrawWheel();
        } glPopMatrix();

        //Draw rear axle
        glColor3f(0,0, 255);
        glPushMatrix(); {
            glRotatef(90, 0, 1, 0);
            glTranslatef(1, 0, -1);
            GLUquadric *quadratic;
            quadratic = gluNewQuadric();
            gluCylinder(quadratic, 0.1f, 0.1f, 2.0f, 32, 32);
        } glPopMatrix();

        //Rear left wheel
        glPushMatrix(); {
            glTranslatef(1, 0, -1);
            glRotatef(270, 0, 1, 0);
            glRotatef(-1 * wheel_rotation, 0, 0, 1);
            DrawWheel();
        } glPopMatrix();

        //Rear right wheel
        glPushMatrix(); {
            glTranslatef(-1, 0, -1);
            glRotatef(90, 0, 1, 0);
            glRotatef(wheel_rotation, 0, 0, 1);
            DrawWheel();
        } glPopMatrix();

    } glPopMatrix();
}