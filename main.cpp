/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: Key to the Kingdom
 *  File: main.cpp
 *
 *  Author: Jordan Moser
 *      Taylor McClure
 *      Claire Collier - Fall 2015
 *
 */

 /*
 *
 * TODO:
 * 1) todos in Map file.
 * 2) Add Hero to road.
 * 3) Control hero with mouse movement
 * 4) Add Skybox
 * 5) Add health to hero
 * 6) Display health in HUD
 * 7) Add viewport with top down view?
 *
*/

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "SOIL/soil.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Camera.h"
#include "Map.h"
#include "Texture_Utils.h"
#include "Config_Utils.h"

#define DEBUG_MAIN_LOOP 0

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth  = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint gLeftMouseButton;
GLboolean gCtrlDown;
int gMouseX = 0, gMouseY = 0;
bool gKeysPressed[256] = {false};

GLuint* gSkyboxTextureHandles;
vector<GLchar*> gSkyboxTextureNames;

Camera* gCamera = NULL;
Map* gMap = NULL;

float wheel_rotation = 0;
float prop_rotation = 0;
float rotate_angle = 0;

void drawProp() {
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

void drawWheel() {
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

void drawBody() {
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
		drawProp();
	} glPopMatrix();
}

void drawCar() {
	//Draw body
	glPushMatrix(); {
		drawBody();
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
		drawWheel();
	} glPopMatrix();

	//Front right wheel
	glPushMatrix(); {
		glTranslatef(-1, 0, 1);
		glRotatef(90, 0, 1, 0);
		glRotatef(wheel_rotation, 0, 0, 1);
		drawWheel();
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
		drawWheel();
	} glPopMatrix();

	//Rear right wheel
	glPushMatrix(); {
		glTranslatef(-1, 0, -1);
		glRotatef(90, 0, 1, 0);
		glRotatef(wheel_rotation, 0, 0, 1);
		drawWheel();
	} glPopMatrix();
}


double getRand() {
    return rand() / (double)RAND_MAX;
}

void resizeWindow(int w, int h) {
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,aspectRatio,0.1,100000);
}

void initScene()  {
    glEnable(GL_DEPTH_TEST);

    float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = { 0.6, 0.6, 0.6, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    // tell OpenGL not to use the material system; just use whatever we
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************

    glShadeModel(GL_FLAT);

    srand( time(NULL) );    // seed our random number generator

    gCtrlDown = false;
    gCamera = new ArcBall();
}

void renderScene(void)  {
    if (DEBUG_MAIN_LOOP)
        cout << "Rendering Scene" << endl;

    //clear the render buffer
    glDrawBuffer( GL_BACK );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);              //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    if (DEBUG_MAIN_LOOP)
        cout << "Updating Camera" << endl;
    if (gCamera)
        gCamera->Update();

    if (DEBUG_MAIN_LOOP)
        cout << "Drawing Map" << endl;
    if (gMap)
        gMap->Draw();

    glPushMatrix(); {
		glTranslatef(gMap->getLocation().getX(), gMap->getLocation().getY(), gMap->getLocation().getZ());
        //glScalef(0, 0, 0);
		glRotatef(rotate_angle, 0, 1, 0);
		drawCar();
	} glPopMatrix();

    //push the back buffer to the screen
    glutSwapBuffers();

    if (DEBUG_MAIN_LOOP)
        cout << "Finished Rendering Scene" << endl;
}

void normalKeysDown(unsigned char key, int x, int y) {
    if (DEBUG_MAIN_LOOP)
        cout << "Key down" << endl;

    if(key == 'q' || key == 'Q' || key == 27)
        exit(0);

    gKeysPressed[key] = true;

    glutPostRedisplay(); // redraw our scene from our new camera POV
}

void normalKeysUp(unsigned char key, int x, int y) {
    if (DEBUG_MAIN_LOOP)
        cout << "KeyUp" << endl;

    gKeysPressed[key] = false;
}

void mouseCallback(int button, int state, int x, int y) {
    if (DEBUG_MAIN_LOOP)
        cout << "Mouse Callback" << endl;

    if(button == GLUT_LEFT_BUTTON)
        gLeftMouseButton = state;

    gMouseX = x;
    gMouseY = y;

    if (glutGetModifiers() & GLUT_ACTIVE_CTRL) {
        gCtrlDown = true;
    }
    else {
        gCtrlDown = false;
    }
}

void mouseMotion(int x, int y) {
    if(gLeftMouseButton == GLUT_DOWN) {
        cout << "X:" << gMouseX << "Y: " << gMouseY << endl;

        //cameraTheta += (x - mouseX)*0.005;
        //cameraPhi   += (y - mouseY)*0.005;
    //     if (gCtrlDown && gCamera->IsArcBall()) {
    //         ((ArcBall*)gCamera)->setRadius(((ArcBall*)gCamera)->getRadius() + 0.05 * (y - gMouseY));
    //     }
    //     else if (gCamera->IsArcBall()){
    //         ((ArcBall*)gCamera)->setTheta(((ArcBall*)gCamera)->getTheta() + 0.005 * (x - gMouseX));
    //         ((ArcBall*)gCamera)->setPhi(((ArcBall*)gCamera)->getPhi() + 0.005 * (gMouseY- y));
    //     }

    //     gCamera->Recompute();

        gMouseX = x;
        gMouseY = y;
        glutPostRedisplay(); // redraw our scene from our new camera POV
    }
}

void updateScene(int value){
    if (DEBUG_MAIN_LOOP)
        cout << "Updating Scene" << endl;

    if (gMap)
        gMap->Update();

    if (gCamera && gCamera->IsArcBall()){
        Vector heading = gMap->getHeading();
        Vector positiveX(1,0,0);

        Vector c = cross(heading, positiveX);
        double a = angle(heading, positiveX);

        // Angle is actually negative, angle returns abs value
        if (c.getY() < 0)
            a *= -1;

        ((ArcBall*) gCamera)->setTheta(a + M_PI);
    }

    if (gCamera){
        gCamera->setLookAt(gMap->getLocation());
        gCamera->Recompute();
    }

    glutPostRedisplay();

    prop_rotation += 10;

    glutTimerFunc(1000.0/60.0, updateScene, 0);

    if (DEBUG_MAIN_LOOP)
        cout << "Done Updating Scene" << endl;
}

int main(int argc, char **argv) {
    // create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Keys to the Kingdom");

    // register callback functions...
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(normalKeysDown);
    glutKeyboardUpFunc(normalKeysUp);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(1000.0/60.0, updateScene, 0);

    // do some basic OpenGL setup
    initScene();

    //LoadTextures();

    if (!LoadGameFile(argc, argv)) // Load config file containing map, texture names, etc.
        return(1);

    if (DEBUG_MAIN_LOOP)
        cout << "Entering Main Loop" << endl;
    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
