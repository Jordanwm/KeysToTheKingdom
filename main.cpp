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
 * 1) Add Hero to its own object file.
 * 2) Add airplane wings to the hero that tilt as you turn left and right
 * 3) Control hero with mouse movement
 * 4) Add health to hero
 * 5) Display health in HUD
 * 6) Add viewport with top down view?
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
#include "Skybox.h"
#include "Hero.h"
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
Skybox* gSkybox = NULL;
Hero* gHero = NULL;

Point planeLocation;

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
    gSkybox = new Skybox();
    gHero = new Plane();
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

    glPushMatrix();
        if (gMap)
            gMap->getLocation().glTranslate();
        if (gSkybox)
            gSkybox->Draw();
    glPopMatrix();

    if (DEBUG_MAIN_LOOP)
        cout << "Drawing Map" << endl;
    if (gMap)
        gMap->Draw();

    if (gHero)
        gHero->Draw();

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
    if (!(x < windowWidth && x > 0 && y < windowHeight && y > 0))
        return;

    // Top Left corner of window is 0,0 so flip y;
    int mousey = windowHeight - y - windowHeight / 2;
    int mousex = x - windowWidth / 2;

    // Y moves forward
    // X moves side to side
    Vector heading;
    
    if (gMap)
        heading = gMap->getHeading();

    heading.normalize();
    Vector up(0,1,0);
    Vector right = cross(heading, up);
    right.normalize();

    planeLocation = Point();
    planeLocation = planeLocation + ((mousex)*0.015) * right;
    planeLocation = planeLocation + ((mousey)*0.03) * heading;

    if(false && gLeftMouseButton == GLUT_DOWN && gCamera) {
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
    }

    glutPostRedisplay(); // redraw our scene from our new camera POV
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

    if (gHero){
        gHero->setLocation(Point(gMap->getLocation().getX() + planeLocation.getX(), gMap->getLocation().getY(), gMap->getLocation().getZ() + planeLocation.getZ()));
        gHero->Update();
    }

    
    glutPostRedisplay();

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
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(1000.0/60.0, updateScene, 0);

    glutWarpPointer( windowWidth / 2, windowHeight / 2 );
    glutSetCursor(GLUT_CURSOR_NONE); 

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
