#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "Point.h"

using namespace std;

class Map {
    vector<Point> _Points;

public:
    Map();
    Map(vector<Point> pts);
};

#endif