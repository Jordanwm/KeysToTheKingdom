#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H 1

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <windows.h>
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "SOIL/soil.h"

#include <vector>
#include <iostream>

bool LoadTextures(GLuint* &textureHandles, std::vector<char*> textureNames);
void BindTexture(int index, GLuint* textureHandles);

#endif TEXTURE_UTILS_H