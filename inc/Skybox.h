#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <vector>

class Skybox{
	enum eSide {
	    RIGHT = 0,
	    LEFT = 1,
	    TOP,
	    BOTTOM,
	    BACK,
	    FRONT
	};
	void GenerateQuad(int s);
public:
	Skybox();

	void Draw();
};

#endif