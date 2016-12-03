#include "Map.h"

#ifdef __APPLE__            // if compiling on Mac OS
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                   // else compiling on Linux OS
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include <iostream>

using namespace std;

/*
 *
 * TODO: 
 * 1) Create rounded corners for game map or something to that effect to smooth transition 
 *    from one stretch to the next. Add to Map::Map(vector<Point>) function
 * 2) Add Texture to road in Draw function
 * 3) Add more details to road?
 *
*/

Map::Map()
{
	_IndexInPoints = 1;
    _Points.push_back(Point(0,0,0));
    _Points.push_back(Point(100,0,0));
    
    _CurrentLocation = _Points[0];
    _MapComplete = false;

    _Heading = _Points[_IndexInPoints] - _Points[_IndexInPoints-1];
    _Heading.normalize();
}

Map::Map(vector<Point> pts)
{
	_IndexInPoints = 1;
    
    if (pts.size() > 1){
    	_Points = pts;
    } else {
		_Points.push_back(Point(0,0,0));
		_Points.push_back(Point(100,0,0));
    }

    _CurrentLocation = _Points[0];
    _MapComplete = false;

    _Heading = _Points[_IndexInPoints] - _Points[_IndexInPoints-1];
    _Heading.normalize();
}

void Map::MoveForward()
{
	double Speed = 0.1;

	_CurrentLocation = _CurrentLocation + Speed * _Heading;
}

void Map::Update()
{
	// If we have reached the next point and there are more points, increment index
	if (_CurrentLocation == _Points[_IndexInPoints] && _Points.size() - 1 > _IndexInPoints){
		_CurrentLocation = _Points[_IndexInPoints];
		++_IndexInPoints;
		_Heading = _Points[_IndexInPoints] - _Points[_IndexInPoints-1];
		_Heading.normalize();
	}

	// We have reached the end of the map!
	if (_CurrentLocation == _Points[_IndexInPoints] && _Points.size() - 1 == _IndexInPoints)
		_MapComplete = true;

	if (!_MapComplete)
	{
		MoveForward();
	}
}

void Map::Draw()
{
	double WidthOfTrack = 5;

	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	Point p;
	for (int i = 0; i < _Points.size() - 1; ++i)
	{
		Vector heading = _Points[i+1] - _Points[i];
		heading.normalize();
		Vector normal = cross(heading, Vector(0,1,0));
		normal.normalize();

		p = _Points[i];
		Point corner = normal * WidthOfTrack/2.0 + p;
		corner = -1 * heading * WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		corner.glVertex();
		
		corner = -1 * normal * WidthOfTrack/2.0 + p;
		corner = -1 * heading * WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		corner.glVertex();

		p = _Points[i+1];
		corner = -1 * normal * WidthOfTrack/2.0 + p;
		corner = -1 * heading * WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		corner.glVertex();
		
		corner = normal * WidthOfTrack/2.0 + p;
		corner = -1 * heading * WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		corner.glVertex();
	}
	glEnd();
}