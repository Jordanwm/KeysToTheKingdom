/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: Key to the Kingdom
 *  File: main.cpp
 *
 *  Author: Jordan Moser
 *      Taylor McClure
 *      Claire Collier - Fall 2016
 *
 */

#include <GL/glew.h>

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
#include <time.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include "BubbleSystem.h"
#include "Camera.h"
#include "Map.h"
#include "Skybox.h"
#include "Hero.h"
#include "Texture_Utils.h"
#include "Config_Utils.h"
#include "Bezier.h"

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

GLuint* gTrackTextureHandles;
vector<GLchar*> gTrackTextureNames;

GLuint trackShaderHandle;
GLuint trackProgress;

GLuint healthShaderHandle;
GLuint timeLoc;

Camera* gCamera = NULL;
Map* gMap = NULL;
Skybox* gSkybox = NULL;
Hero* gHero = NULL;
BubbleSystem* gBubbleSystem = NULL;

Point planeLocation;
double maxHealth;
vector<Point*> points;
map<double, Point> directions;
map<double, double> locations;
BezierCurve track;
vector<Point> bezierPoints;

double lastZ = 0;

bool displayWinnerScreen = false;
bool displayLoserScreen = false;

double getRand() {
    return rand() / (double)RAND_MAX;
}

void RestartGame();
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
    float ambientCol[4] = { 0.1, 0.1, 0.1, 0.1 };
    float lPosition[4] = { 0, 100, 0, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    
    // This is our spotlight
    glEnable( GL_LIGHT1 );
    float diffuseCol1[4] = {1.0f, 1.0f, 1.0f, 1.0f };       
    glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuseCol1 );
    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 100 ); 

    // tell OpenGL not to use the material system; just use whatever we
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************

    glShadeModel(GL_FLAT);

    srand( time(NULL) );    // seed our random number generator

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gCtrlDown = false;
    gCamera = new ArcBall();
    gSkybox = new Skybox();
    gHero = new Plane();
}

void renderHealthHUD() {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    double percentage = gHero->getHealth() / maxHealth;
    int centerx = windowWidth / 2;
    int overallWidth = 200;
    int overallHeight = 15;
    vector<Point> squarePercentage, square;
    square.push_back(Point(centerx - overallWidth/2.0, windowHeight-overallHeight - 2, 0.0));
    square.push_back(Point(centerx - overallWidth/2.0, windowHeight - 2, 0.0));
    square.push_back(Point(centerx + overallWidth/2.0, windowHeight - 2, 0.0));
    square.push_back(Point(centerx + overallWidth/2.0, windowHeight-overallHeight - 2, 0.0));

    overallWidth -= 4;
    overallHeight -= 4;
    squarePercentage.push_back(Point(centerx - overallWidth/2.0, windowHeight-overallHeight - 4, 0.0));
    squarePercentage.push_back(Point(centerx - overallWidth/2.0, windowHeight - 4, 0.0));
    squarePercentage.push_back(Point(centerx + overallWidth/2.0 - (1-percentage)*overallWidth, windowHeight - 4, 0.0));
    squarePercentage.push_back(Point(centerx + overallWidth/2.0 - (1-percentage)*overallWidth, windowHeight-overallHeight - 4, 0.0));

    double red = (1-percentage);
    double green = percentage;
	
	if (percentage < .2 && !gHero->isDead() && !gMap->getMapComplete())
		glUseProgram(healthShaderHandle);
    
	glColor3f(red, green, 0.0);

    glPushMatrix();
    glBegin(GL_QUADS);
        squarePercentage[0].glVertex();
        squarePercentage[1].glVertex();
        squarePercentage[2].glVertex();
        squarePercentage[3].glVertex();
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glLineWidth(1);
    glBegin(GL_LINES);
        square[0].glVertex();
        square[1].glVertex();
        
        square[1].glVertex();
        square[2].glVertex();
        
        square[2].glVertex();
        square[3].glVertex();

        square[0].glVertex();
        square[3].glVertex();
    glEnd();
    glPopMatrix();
	glUseProgram(0);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
}

void renderEndingSequence() {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (displayWinnerScreen){
        glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2i(10, 30);
        char* s = "You Win!";

        void * font = GLUT_BITMAP_TIMES_ROMAN_24;
        for (int i = 0; s[i] != '\0'; ++i)
        {
            glutBitmapCharacter(font, s[i]);
        }
        glPopMatrix();

        glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2i(10, 10);
        char* c = "(Press 'R' to Restart)";

        font = GLUT_BITMAP_TIMES_ROMAN_10;
        for (int i = 0; c[i] != '\0'; ++i)
        {
            glutBitmapCharacter(font, c[i]);
        }
        glPopMatrix();
        
        glPushMatrix();
        glColor4f(0.0, 1.0, 0.0, 0.3);
        glBegin(GL_QUADS);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, windowHeight);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f(windowWidth, 0.0);
        glEnd();
        glPopMatrix();
    } else if (displayLoserScreen){
        glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2i(10, 30);
        char* s = "You Lose!";

        void * font = GLUT_BITMAP_TIMES_ROMAN_24;
        for (int i = 0; s[i] != '\0'; ++i)
        {
            glutBitmapCharacter(font, s[i]);
        }
        glPopMatrix();

        glPushMatrix();
        glColor3f(0,0,0);
        glRasterPos2i(10, 10);
        char* c = "(Press 'R' to Restart)";

        font = GLUT_BITMAP_TIMES_ROMAN_10;
        for (int i = 0; c[i] != '\0'; ++i)
        {
            glutBitmapCharacter(font, c[i]);
        }
        glPopMatrix();
        
        glPushMatrix();
        glColor4f(1.0, 0.0, 0.0, 0.3);
        glBegin(GL_QUADS);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, windowHeight);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f(windowWidth, 0.0);
        glEnd();
        glPopMatrix();
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
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

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
	
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glLineWidth(2.0f);

	glBegin(GL_LINE_STRIP);
	glVertex3f(10, 3, 0);
	glVertex3f(40, 3, 0);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

    if (DEBUG_MAIN_LOOP)
        cout << "Updating Camera" << endl;
	if (gCamera) {
		Point po = gMap->getLocation();
		if (directions.find((double)floor(po.getX() * 10) / 10) != directions.end()) {
			gCamera->setDirection(directions[((double)floor(po.getX() * 10) / 10)]);
			gCamera->setZ(locations[((double)floor(po.getX() * 10) / 10)]);
			gCamera->Update();
		}
		else {
			gCamera->Update();
		}
	}
    glPushMatrix();{
        renderHealthHUD();
    }glPopMatrix();

    glPushMatrix();
        if (gMap)
            gMap->getLocation().glTranslate();
        if (gSkybox)
            gSkybox->Draw();
    glPopMatrix();

    if (DEBUG_MAIN_LOOP)
        cout << "Drawing Map" << endl;
    
    glUseProgram(trackShaderHandle);
    if (gMap)
        gMap->Draw();
    glUseProgram(0);

    if (gMap)
        gMap->DrawFinishLine();

    if (gHero)
        gHero->Draw();

    if (gBubbleSystem)
        gBubbleSystem->Draw();

    glPushMatrix();
    if (displayWinnerScreen || displayLoserScreen){
        renderEndingSequence();
    }
    glPopMatrix();

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

    if(key == 'r' || key == 'R')
        RestartGame();

    if (key == '1' && !(displayWinnerScreen || displayLoserScreen))
        gCamera = new ArcBall();

    if (key == '2' && !(displayWinnerScreen || displayLoserScreen))
        gCamera = new TopDown();

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
    glutSetCursor(GLUT_CURSOR_NONE);
    if (gMap && gMap->getMapComplete() || gHero && gHero->isDead()){
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW); 
    } else if (gCamera->IsArcBall()){
        heading = gMap->getHeading();

        heading.normalize();
        Vector up(0,1,0);
        Vector right = cross(heading, up);
        right.normalize();

        double xproportion = 0.015;
        xproportion = ((double(windowHeight - y) / windowHeight)+0.3) * 0.015;
        
        Point oldLocation = planeLocation;
        planeLocation = Point();
        planeLocation = planeLocation + ((mousex)*xproportion) * right;
        planeLocation = planeLocation + ((mousey)*0.03) * heading;

        bool reached_bound = false;
        if (gMap){
            if (right.getX() == 1.0 && abs(planeLocation.getX()) > gMap->getWidthOfTrack()/2.0)
                reached_bound = true;
            if (right.getZ() == 1.0 && abs(planeLocation.getZ()) > gMap->getWidthOfTrack()/2.0)
                reached_bound = true;
        }

        if (reached_bound)
            planeLocation = oldLocation;

        // Update wing rotation if strafing
        Vector movement = oldLocation - planeLocation;
        ((Plane*)gHero)->setWingRotation(50 * dot(right, movement));
    } else {
        heading = gMap->getHeading();

        heading.normalize();
        Vector up(0,1,0);
        Vector right = cross(heading, up);
        right.normalize();

        Point oldLocation = planeLocation;
        planeLocation = Point();
        planeLocation = planeLocation + ((mousex)*0.03) * right;
        planeLocation = planeLocation + ((mousey)*0.03) * heading;

        bool reached_bound = false;
        if (gMap){
            if (right.getX() == 1.0 && abs(planeLocation.getX()) > gMap->getWidthOfTrack()/2.0)
                reached_bound = true;
            if (right.getZ() == 1.0 && abs(planeLocation.getZ()) > gMap->getWidthOfTrack()/2.0)
                reached_bound = true;
        }

        if (reached_bound)
            planeLocation = oldLocation;
    }

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

    if (gMap && gMap->getMapComplete()){
        // We won the game, slowly rotate around hero
        if (gCamera && gCamera->IsArcBall()){
            ((ArcBall*) gCamera)->setTheta(((ArcBall*) gCamera)->getTheta() + 0.01);
            gCamera->Recompute();
        }
        if (gHero)
            gHero->setLocation(Point(gMap->getLocation().getX(), gMap->getLocation().getY(), gMap->getLocation().getZ()));

        displayWinnerScreen = true;

    } else if (gHero && gHero->isDead()) {
        gCamera->setLookAt(Point(gMap->getLocation().getX() + planeLocation.getX(), gMap->getLocation().getY(), gMap->getLocation().getZ() + planeLocation.getZ()));
        if (gCamera && gCamera->IsArcBall()){
            ((ArcBall*) gCamera)->setTheta(((ArcBall*) gCamera)->getTheta() + 0.01);
            gCamera->Recompute();
        }
        displayLoserScreen = true;
    } else {
        if (gMap){
            gMap->Update();

            // Update progress for color in fragment shader
            glUseProgram(trackShaderHandle);
            trackProgress = glGetUniformLocation( trackShaderHandle, "trackProgress" );
            glUniform1f(trackProgress, gMap->getProgress());			
			glUseProgram(healthShaderHandle);
            timeLoc = glGetUniformLocation( healthShaderHandle, "time" );
            glUniform1f(timeLoc, glutGet(GLUT_ELAPSED_TIME) / 100.0);
            glUseProgram(0);
        }

        if (gCamera && gCamera->IsArcBall()){
            Vector heading = gMap->getHeading();
            Vector positiveX(1,0,0);

            Vector c = cross(heading, positiveX);
            double a = angle(heading, positiveX);

            // Angle is actually negative, angle returns abs value
            if (c.getY() < 0)
                a *= -1;

            ((ArcBall*) gCamera)->setTheta(a + M_PI);
        } else if (gCamera && gCamera->IsTopDown()){
            ((TopDown*)gCamera)->setUpVec(gMap->getHeading());
        }

        if (gCamera){
            gCamera->setLookAt(gMap->getLocation());
            gCamera->Recompute();
        }
		
		

        if (gHero){
			if (locations.find((double)floor((gMap->getLocation().getX() + planeLocation.getX()) * 10) / 10) != locations.end()) {
				double shift = locations[(double)floor((gMap->getLocation().getX() + planeLocation.getX()) * 10) / 10];
				gHero->setLocation(Point(gMap->getLocation().getX() + planeLocation.getX(), gMap->getLocation().getY(), shift + planeLocation.getZ()));
				gHero->setLocationDir(directions[(double)floor((gMap->getLocation().getX() + planeLocation.getX()))]);
				cout << "Plane X : " << planeLocation.getX() << endl;
			}
			gHero->Update();
        }

        if (gBubbleSystem)
            gBubbleSystem->Update();
    }
    
    glutPostRedisplay();

    glutTimerFunc(1000.0/60.0, updateScene, 0);

    if (DEBUG_MAIN_LOOP)
        cout << "Done Updating Scene" << endl;
}

void RestartGame(){
    displayWinnerScreen = false;
    displayLoserScreen = false;
    planeLocation = Point();

    delete gCamera;
    delete gSkybox;
    delete gHero;
    delete gMap;
    delete gBubbleSystem;

    gCamera = new ArcBall();
    gSkybox = new Skybox();
    gHero = new Plane();
    gMap = new Map(points);

    // Need map generated before doing bubble system.
    if (gMap){
        gBubbleSystem = new BubbleSystem();
        maxHealth = gMap->getLength() / 0.2;
        gHero->setHealth(maxHealth);
    }
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

    /* initialize GLEW */
    GLenum glewResult = glewInit();
    /* check for an error */
    if( glewResult != GLEW_OK ) {
        printf( "[ERROR]: Error initalizing GLEW\n");
        return 0;
    }
    /* make sure OpenGL 2.0 is supported */
    if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
        printf( "[ERROR]: System does not support OpenGL 2.0 and GLSL\n" );
        return 0;
    }
    
    fprintf(stdout, "[INFO]: /--------------------------------------------------------\\\n");
    fprintf(stdout, "[INFO]: | OpenGL Information                                     |\n");
    fprintf(stdout, "[INFO]: |--------------------------------------------------------|\n");
    fprintf(stdout, "[INFO]: |   OpenGL Version:  %35s |\n", glGetString(GL_VERSION));
    fprintf(stdout, "[INFO]: |   OpenGL Renderer: %35s |\n", glGetString(GL_RENDERER));
    fprintf(stdout, "[INFO]: |   OpenGL Vendor:   %35s |\n", glGetString(GL_VENDOR));
    fprintf(stdout, "[INFO]: |   Shading Version: %35s |\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
    
    printf( "[INFO]: System supports OpenGL2.0 and GLSL!\n\n" );

    // do some basic OpenGL setup
    initScene();

    if (!LoadGameFile(argc, argv)) // Load config file containing map, texture names, etc.
        return true;

	

	//Populate track.
	for (int i = 0; i < points.size() - 1; i += 3) {
		BezierCurve c(*points[i], *points[i + 1], *points[i + 2], *points[i + 3]);
		track = c;
	}

	//Populate bezierPoints;
	int i = 0;
	for (float k = 0; k < 1; k += 1 / (float)1000) {
		Point p = track.Evaluate(k);
		bezierPoints.push_back(p);
		Point tangent = track.EvaluateTangent(k);
		Point direction = Point(tangent.getX(), 0.0, tangent.getZ());
		if (floor(p.getX() * 10) > i) {
			i = floor(p.getX() * 10);
		}
		if (floor(p.getX() * 10) == i) {
			directions.insert(pair<double, Point>((double)(floor(p.getX() * 10)) / 10, direction));
			locations.insert(pair<double, double>((double)(floor(p.getX() * 10)) / 10, p.getZ()));
			//cout << "X : " << (double)(floor(p.getX() * 10)) / 10 << endl;
			i++;
		}
		
	}


    // Need map generated before doing bubble system.
    if (gMap){
        gBubbleSystem = new BubbleSystem();
        maxHealth = gMap->getLength() / 0.2;
        gHero->setHealth(maxHealth);
    }

    if (DEBUG_MAIN_LOOP)
        cout << "Entering Main Loop" << endl;
    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
