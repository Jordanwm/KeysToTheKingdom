#include "Bezier.h"
#include <iostream>
#include "math.h"


using namespace std;

BezierCurve::BezierCurve() {
	// Create 4 empty points
	xCoefficient1 = Point();
	xCoefficient2 = Point();
	xCoefficient3 = Point();
	xCoefficient4 = Point();
}

BezierCurve::BezierCurve(Point p0, Point p1, Point p2, Point p3)
{
	// 100 time steps along curve
	xResolution = 100;
	xControlPoints = vector<Point>();

	// Store the coefficents that are used in the interpolation equations
	xCoefficient1 = (-1.0f * p0 + 3.0f * p1 + (-3.0f) * p2 + p3);
	xCoefficient2 = (3.0f * p0 + (-6.0f) * p1 + 3.0f * p2);
	xCoefficient3 = (-3.0f * p0 + 3.0f * p1);
	xCoefficient4 = p0;

	xControlPoints.push_back(p0);
	xControlPoints.push_back(p1);
	xControlPoints.push_back(p2);
	xControlPoints.push_back(p3);
}

Point BezierCurve::EvaluateTangent(float t)
{
	return 3.0f * xCoefficient1 * t * t + 2.0f * xCoefficient2 * t + xCoefficient3;
}

Point BezierCurve::Evaluate(float t)
{
	return xCoefficient1 * t * t * t + xCoefficient2 * t * t + xCoefficient3 * t + xCoefficient4;
}

BezierSurface::BezierSurface(BezierCurve BC0, BezierCurve BC1, BezierCurve BC2, BezierCurve BC3)
{
	// Number of time steps
	xResolution = 100;

	// Store the control curves
	xControlCurves = vector<BezierCurve>();
	xControlCurves.push_back(BC0);
	xControlCurves.push_back(BC1);
	xControlCurves.push_back(BC2);
	xControlCurves.push_back(BC3);
}

