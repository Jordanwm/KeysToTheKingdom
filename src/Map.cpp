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
#include <cmath>
#include "Texture_Utils.h"

#define DEBUG_CREATE_CORNERS 0
#define DEBUG_ANGLE_OF_TURNS 0

extern GLuint* gTrackTextureHandles;
extern vector<GLchar*> gTrackTextureNames;

extern GLuint trackShaderHandle;

using namespace std;

Map::Map()
{
	_IndexInPoints = 1;
	_WidthOfTrack = 5;
    Point* p1 = new Point(0,0,0);
    _Points.push_back(p1);
    Point* p2 = new Point(100,0,0);
    _Points.push_back(p2);
    
    _CurrentLocation = *_Points[0];
    _MapComplete = false;

    _Heading = *_Points[_IndexInPoints] - *_Points[_IndexInPoints-1];
    _Heading.normalize();
}

Map::Map(vector<Point*> pts)
{
	_IndexInPoints = 1;
    _WidthOfTrack = 5;

    if (pts.size() > 1){
    	_Points = pts;
    } else {
		Point* p1 = new Point(0,0,0);
	    _Points.push_back(p1);
	    Point* p2 = new Point(100,0,0);
	    _Points.push_back(p2);
    }

    _CurrentLocation = *_Points[0];
    _MapComplete = false;

    _Heading = *_Points[_IndexInPoints] - *_Points[_IndexInPoints-1];
    _Heading.normalize();

    _Length = 0;
    for (int i = 0; i < _Points.size()-1; ++i){
    	_Length += (*_Points[i+1] - *_Points[i]).mag();
    }

    CreateRoundedCorners();
}

Point Map::getPointAtIndex(int i){
    if (_Points.size() > i) return *(_Points[i]);
    else return Point();
}

void Map::CreateRoundedCorners()
{
	_PointsWithRoundedCorners.push_back(_Points[0]);

	for (int i = 1; i < _Points.size() - 1; ++i) {
		double Radius = 0.5;
		Vector v1 = *_Points[i-1] - *_Points[i];
		Vector v2 = *_Points[i+1] - *_Points[i];

		// Reverse vectors if angle greater than PI
		Vector direction = cross(v1, v2);
		double ang = angle(v1, v2);

		// Determine if radius is too big
		double v1Length = v1.mag();
		double v2Length = v2.mag();
		double minLength = v1Length;
		if (v2Length < minLength)
			minLength = v2Length;

		if (Radius > minLength){
			Radius = minLength;
		}

		v1.normalize();
		v2.normalize();

		Point Center1 = v1 * Radius + *_Points[i];
		Point Center2 = v2 * Radius + *_Points[i];

		Point Center = (Center1 + Center2) / 2.0;

		// Vector from corner to center point of arc
		Vector d = *_Points[i] - Center;
		// Length from corner to center point
		double dist = d.mag();
		// Actual radius from center point to the vectors v1 and v2.
		double ActualRadius = dist * cos(ang/2.0);

		// Perpendicular Point.
		Point perpv1 = ActualRadius * v1 + *_Points[i];
		Vector FromCenterToV1 = perpv1 - Center;
		Point perpv2 = ActualRadius * v2 + *_Points[i];
		Vector FromCenterToV2 = perpv2 - Center;

		double startAngle = angle(Vector(0,0,1), FromCenterToV1);
		double endAngle = angle(Vector(0,0,1), FromCenterToV2);

		if (DEBUG_CREATE_CORNERS){
			cout << "V1: " << v1.toString() << endl;
			cout << "V2: " << v2.toString() << endl;
			cout << "Point on V1: " << Center1.toString() << endl;
			cout << "Point on V2: " << Center2.toString() << endl;
			cout << "Center: " << Center.toString() << endl;
			cout << "Perp v1: " << perpv1.toString() << endl;
			cout << "Perp v2: " << perpv2.toString() << endl;
			cout << "Vector from c to v1: " << FromCenterToV1.toString() << endl;
			cout << "Vector from c to v2: " << FromCenterToV2.toString() << endl;
			cout << "Start: " << startAngle << endl;
			cout << "End: " << endAngle << endl;
		}

		Point* p = new Point(perpv1);
		_PointsWithRoundedCorners.push_back(p);
		if (direction.getY() > 0 && startAngle > endAngle){
			if (DEBUG_ANGLE_OF_TURNS)
				cout << _Points[i]->toString() << " : Condition 1" << endl;
			for (double a = startAngle; a > endAngle; a -= .1){
				p = new Point(ActualRadius * sin(a) + Center.getX(), 0.0, ActualRadius * cos(a) + Center.getZ());
				_PointsWithRoundedCorners.push_back(p);
			}
		} else if (direction.getY() > 0) {
			if (DEBUG_ANGLE_OF_TURNS)
				cout << _Points[i]->toString() << " : Condition 2" << endl;
			for (double a = startAngle; a < endAngle; a += .1){
				p = new Point(ActualRadius * sin(a) + Center.getX(), 0.0, ActualRadius * cos(a) + Center.getZ());
				_PointsWithRoundedCorners.push_back(p);
			}
		} else if (direction.getY() < 0 && startAngle > endAngle){
			if (DEBUG_ANGLE_OF_TURNS)
				cout << _Points[i]->toString() << " : Condition 3" << endl;
			startAngle *= -1;
			endAngle *= -1;
			for (double a = startAngle; a < endAngle; a += .1){
				p = new Point(ActualRadius * sin(a) + Center.getX(), 0.0, ActualRadius * cos(a) + Center.getZ());
				_PointsWithRoundedCorners.push_back(p);
			}
		} else {
			if (DEBUG_ANGLE_OF_TURNS)
				cout << _Points[i]->toString() << " : Condition 4" << endl;
			for (double a = startAngle; a < endAngle; a += .1){
				p = new Point(ActualRadius * sin(a) + Center.getX(), 0.0, ActualRadius * cos(a) + Center.getZ());
				_PointsWithRoundedCorners.push_back(p);
			}
		}
		p = new Point(perpv2);
		_PointsWithRoundedCorners.push_back(p);
	}
	_PointsWithRoundedCorners.push_back(_Points[_Points.size()-1]);

}

void Map::MoveForward()
{
	double Speed = 0.1;
	_CurrentLocation = _CurrentLocation + Speed * _Heading;
	_Distance += (Speed * _Heading).mag();
	_CurrentProgress = _Distance / _Length;
}

void Map::Update()
{
	// If we have reached the next point and there are more points, increment index
	if (_CurrentLocation == *_PointsWithRoundedCorners[_IndexInPoints] && _PointsWithRoundedCorners.size() - 1 > _IndexInPoints){
		_CurrentLocation = *_PointsWithRoundedCorners[_IndexInPoints];
		++_IndexInPoints;
		_Heading = *_PointsWithRoundedCorners[_IndexInPoints] - *_PointsWithRoundedCorners[_IndexInPoints-1];
		_Heading.normalize();
	}
	
	// We have reached the end of the map!
	if (!_MapComplete && _CurrentLocation == *_PointsWithRoundedCorners[_IndexInPoints] && _PointsWithRoundedCorners.size() - 1 == _IndexInPoints)
		_MapComplete = true;

	if (!_MapComplete)
	{
		MoveForward();
	}
}

void Map::Draw()
{
	Vector heading, normal;
	double dist;

	glEnable(GL_TEXTURE_2D);
	BindTexture(0, gTrackTextureHandles);

	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	Point p;

	for (int i = 0; i < _Points.size() - 1; ++i)
	{
		heading = *_Points[i+1] - *_Points[i];
		dist = heading.mag();
		heading.normalize();
		normal = cross(heading, Vector(0,1,0));
		normal.normalize();

		p = *_Points[i];
		Point corner = normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		corner.glVertex();
		
		corner = -1 * normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(_WidthOfTrack, 0);
		corner.glVertex();

		p = *_Points[i+1];
		corner = -1 * normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(_WidthOfTrack, dist + _WidthOfTrack);
		corner.glVertex();
		
		corner = normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(0, dist + _WidthOfTrack);
		corner.glVertex();
	}
	glEnd();

	if (false){ // Draw Heading
		glColor3f(0, 1, 0);
		glBegin(GL_LINES);
			glVertex3f(_CurrentLocation.getX(), 1, _CurrentLocation.getZ());
			glVertex3f(_CurrentLocation.getX() + _Heading.getX(), 1, _CurrentLocation.getZ() +  _Heading.getZ());
		glEnd();
	}

	if (false){ // Draw rounded corners
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for(int i = 0; i < _PointsWithRoundedCorners.size() - 1; ++i){
			glVertex3f(_PointsWithRoundedCorners[i]->getX(), 1, _PointsWithRoundedCorners[i]->getZ());
			glVertex3f(_PointsWithRoundedCorners[i+1]->getX(), 1, _PointsWithRoundedCorners[i+1]->getZ());
		}
		glEnd();
	}

	if (false){ // Draw Axis in camera
		glBegin(GL_LINES);
			// +x
			glColor3f(1, 0, 0);
			glVertex3f(_CurrentLocation.getX(), 1, _CurrentLocation.getZ());
			glVertex3f(_CurrentLocation.getX() + 1, 1, _CurrentLocation.getZ());
			
			// +y
			glColor3f(0, 1, 0);
			glVertex3f(_CurrentLocation.getX(), 1, _CurrentLocation.getZ());
			glVertex3f(_CurrentLocation.getX(), 2, _CurrentLocation.getZ());

			// +z
			glColor3f(0, 0, 1);
			glVertex3f(_CurrentLocation.getX(), 1, _CurrentLocation.getZ());
			glVertex3f(_CurrentLocation.getX(), 1, _CurrentLocation.getZ()+1);
			
		glEnd();
	}
}

void Map::DrawFinishLine(){
	Vector heading, normal;
	double dist;
	Point p;

	glEnable(GL_TEXTURE_2D);
	BindTexture(0, gTrackTextureHandles);

	// The finish line!
	glColor3f(0,0,1);
	glBegin(GL_QUADS);{
		heading = *_Points[_Points.size() - 1] - *_Points[_Points.size() - 2];
		heading.normalize();
		normal = cross(heading, Vector(0,1,0));
		normal.normalize();

		p = *_Points[_Points.size() - 1];
		Point corner = normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		corner.glVertex();
		
		corner = -1 * normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(_WidthOfTrack, 0);
		corner.glVertex();

		p = p + heading * _WidthOfTrack * 2;
		corner = -1 * normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(_WidthOfTrack, _WidthOfTrack);
		corner.glVertex();
		
		corner = normal * _WidthOfTrack/2.0 + p;
		corner = -1 * heading * _WidthOfTrack/2.0 + corner;
		glNormal3f(0,1,0);
		glTexCoord2f(0, _WidthOfTrack);
		corner.glVertex();
	}glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
