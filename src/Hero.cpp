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
    wing_rotation = 0.0;
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

void Plane::DrawWing() {
    
    glPushMatrix();{
        glBegin(GL_QUADS);{
            // top
            glVertex3f(0,0.1,0);
            glVertex3f(0,0.1,2);
            glVertex3f(1,0.1,0.25);
            glVertex3f(1,0.1,0);

            // bottom
            glVertex3f(0,-0.1,0);
            glVertex3f(0,-0.1,2);
            glVertex3f(1,-0.1,0.25);
            glVertex3f(1,-0.1,0);

            // Back Side
            glVertex3f(0,-0.1,0);
            glVertex3f(0,0.1,0);
            glVertex3f(0,0.1,2);
            glVertex3f(0,-0.1,2);

            // left side
            glVertex3f(1,-0.1,0);
            glVertex3f(1,0.1,0);
            glVertex3f(0,0.1,0);
            glVertex3f(0,-0.1,0);

            // front side
            glVertex3f(1,-0.1,0.25);
            glVertex3f(1,0.1,0.25);
            glVertex3f(1,0.1,0);
            glVertex3f(1,-0.1,0);

            //right side
            glVertex3f(0,-0.1,2);
            glVertex3f(0,0.1,2);
            glVertex3f(1,0.1,0.25);
            glVertex3f(1,-0.1,0.25);

        } glEnd();
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
        gluCylinder(cylinder, 0.5f, 0.5f, 1.0f, 60, 60);
    } glPopMatrix();
    
    //Front sphere
    glPushMatrix(); {
        glScalef(1.2, 1, 2);
        glTranslatef(0, 0, 1.0);
        //glutSolidCube(1);
        glTranslatef(0, 0, -0.5);
        glutSolidSphere(0.5, 60,60);

        // Add our spotlight to the front of the plane
        GLfloat pos[4] = {0, 0, 0, 1};
        glLightfv( GL_LIGHT1, GL_POSITION, pos );
        GLfloat dir[4] = {0, 0, 1, 0};
        glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, dir );
        glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 12 );
    } glPopMatrix();


    //Rear sphere
    glPushMatrix(); {
        glScalef(1.2, 1, 1);
        glTranslatef(0, 0, -0.7);
        //glutSolidCube(1);
        glTranslatef(0, 0, -0.5);
        glutSolidSphere(0.5, 60,60);
    } glPopMatrix();
    
    //Rear Prop shaft
    glColor3ub(20,20,20);
    glPushMatrix(); {
        glScalef(1.2, 1, -2.5);
        //glutSolidCube(1);
        glTranslatef(0, 0, 0);
        cylinder = gluNewQuadric();
        gluCylinder(cylinder, 0.1f, 0.1f, 0.8f, 60, 60);
    } glPopMatrix();

    //Main upper cylinder
    glColor3ub(100, 100, 100);
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, -1);
        gluCylinder(cylinder, 0.5f, 0.5f, 1.0f, 60, 60);
    } glPopMatrix();

    //Front upper cylinder
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, 0);
        gluCylinder(cylinder, 0.5f, 0.0f, 1.8f, 60, 60);
    } glPopMatrix();

    //Rear upper cylinder
    glPushMatrix(); {
        cylinder = gluNewQuadric();
        glTranslatef(0, 0.25, -1.5);
        gluCylinder(cylinder, 0.0f, 0.5f, 0.5f, 60, 60);
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
        glTranslatef(0,0.25,0);
        glScalef(0.5, 0.5, 0.5);
        glRotatef(rotate_angle, 0, 1, 0);
        glRotatef(-wing_rotation, 0, 0, 1);
        
        //Draw body
        glPushMatrix(); {
            DrawBody();
        } glPopMatrix();

        glPushMatrix(); {
            glTranslatef(0,0, -2);
            glRotatef(prop_rotation, 0, 0, 1);
            DrawProp();
        } glPopMatrix();

        // Left Wing
        glPushMatrix(); {
            glRotatef(wing_rotation, 1, 0, 0);
            glTranslatef(.5,0,-1);
            DrawWing();
        } glPopMatrix();

        // Right Wing
        glPushMatrix(); {
            glRotatef(-wing_rotation, 1, 0, 0);
            glScalef(-1,1,1);
            glTranslatef(.5,0,-1);
            DrawWing();
        } glPopMatrix();
    } glPopMatrix();
}