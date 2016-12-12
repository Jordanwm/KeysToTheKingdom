#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include "Point.h"

using namespace std;

class BezierSurface;

class BezierCurve {
	Point xCoefficient1;
	Point xCoefficient2;
	Point xCoefficient3;
	Point xCoefficient4;
	vector<Point> xControlPoints;
	int xResolution;

public:
	BezierCurve();
	BezierCurve(Point p0, Point p1, Point p2, Point p3);

	Point Evaluate(float t);
	Point Evaluate(float u, float v);
	Point EvaluateTangent(float t);

	//void Print();

	vector<Point> getControlPoints() { return xControlPoints; }

	friend class BezierSurface;
};

class BezierSurface {
	vector<BezierCurve> xControlCurves;
	int xResolution;

	float BinarySearchU(float, float, float, float);
	float BinarySearchV(float, float, float, float, float);

public:
	BezierSurface() { xControlCurves = vector<BezierCurve>(); }
	BezierSurface(BezierCurve BC0, BezierCurve BC1, BezierCurve BC2, BezierCurve BC3);

	int GetResolution() { return xResolution; }

	
	/*Point EvaluateBasedOnXZ(float x, float z);
	Point EvaluateNormalBasedOnXZ(float x, float z);
	Point EvaluateBasedOnXZ_V2(float x, float z);
	void EvaluateLocationAndNormalBasedOnXZ(float x, float z, Point & loc, Point & norm);*/
};

#endif 
