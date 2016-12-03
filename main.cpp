/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: Key to the Kingdom
 *  File: main.cpp
 *
 *	Author: Jordan Moser
 *		Taylor McClure 
 *		Claire Collier - Fall 2015
 *
 */

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "SOIL/soil.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

#include "Camera.h"
#include "Texture_Utils.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth  = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint gLeftMouseButton;
GLboolean gCtrlDown;
int gMouseX = 0, gMouseY = 0;
bool gKeysPressed[256] = {false};

Camera* gCamera;

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

    srand( time(NULL) );	// seed our random number generator

    gCtrlDown = false;
    gCamera = new ArcBall();
}

void renderScene(void)  {
    //clear the render buffer
    glDrawBuffer( GL_BACK );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);              //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    gCamera->Update();
    
    //push the back buffer to the screen
    glutSwapBuffers();
}

void normalKeysDown(unsigned char key, int x, int y) {
    if(key == 'q' || key == 'Q' || key == 27)
        exit(0);

    gKeysPressed[key] = true;

    glutPostRedisplay(); // redraw our scene from our new camera POV
}

void normalKeysUp(unsigned char key, int x, int y) {
    gKeysPressed[key] = false;
}

void mouseCallback(int button, int state, int x, int y) {
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
        if (gCtrlDown && gCamera->IsArcBall()) {
            ((ArcBall*)gCamera)->setRadius(((ArcBall*)gCamera)->getRadius() + 0.05 * (y - gMouseY));
        }
        else if (gCamera->IsArcBall()){
            ((ArcBall*)gCamera)->setTheta(((ArcBall*)gCamera)->getTheta() + 0.005 * (x - gMouseX));
            ((ArcBall*)gCamera)->setPhi(((ArcBall*)gCamera)->getPhi() + 0.005 * (gMouseY- y));
        }

        gCamera->Recompute();

        gMouseX = x;
        gMouseY = y;
        glutPostRedisplay(); // redraw our scene from our new camera POV
    }
}

void updateScene(int value){
    gCamera->Recompute();
        
    glutPostRedisplay();

    glutTimerFunc(1000.0/60.0, updateScene, 0);
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

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
