#include "Map.h"

Map::Map()
{
    _Points.push_back(Point(0,0,0));
}

Map::Map(vector<Point> pts)
{
    _Points = pts;
}