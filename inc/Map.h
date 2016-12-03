#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "Point.h"
#include "Vector.h"

using namespace std;

class Map {
	int _IndexInPoints;
    bool _MapComplete;
    vector<Point> _Points;
    Point _CurrentLocation;
    Vector _Heading;

public:
    Map();
    Map(vector<Point> pts);

    Point getLocation(){return _CurrentLocation;}
    void setLocation(Point loc){_CurrentLocation = loc;}

    Vector getHeading(){return _Heading;}
    void setHeading(Vector h){_Heading = h;}

    void MoveForward();

    void Draw();
    void Update();
};

#endif