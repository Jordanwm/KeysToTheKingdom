#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "Point.h"
#include "Vector.h"

using namespace std;

class Map {
	int _IndexInPoints;
    bool _MapComplete;
    vector<Point*> _Points;
    vector<Point*> _PointsWithRoundedCorners;
    Point _CurrentLocation;
    Vector _Heading;
    double _Length;
    double _CurrentProgress;
    double _Distance;
    double _WidthOfTrack;
	//Point nextLocation;
	//Vector nextHeading;
	//bool isFuture;

public:
    Map();
    Map(vector<Point*> pts);

    Point getLocation(){return _CurrentLocation;}
    void setLocation(Point loc){_CurrentLocation = loc;}

    Vector getHeading(){return _Heading;}
    void setHeading(Vector h){_Heading = h;}

    double getWidthOfTrack(){return _WidthOfTrack;}
    double getNumPointsOnTrack(){return _Points.size();}

    Point getPointAtIndex(int i);

    double getProgress(){return _CurrentProgress;}

    void CreateRoundedCorners();
    void MoveForward();

    void Draw();
    void Update();

	double getLength() { return _Length; }
	//void setFuture();
	//Point getFutureLocation() { return nextLocation; }
	//Vector getFutureHeading() { return nextHeading; }
	//bool future() { return isFuture; }

	
};

#endif