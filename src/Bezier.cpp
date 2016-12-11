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

Point BezierCurve::Evaluate(float t)
{
	return xCoefficient1 * t * t * t + xCoefficient2 * t * t + xCoefficient3 * t + xCoefficient4;
}

Point BezierCurve::EvaluateTangent(float t)
{
	return 3.0f * xCoefficient1 * t * t + 2.0f * xCoefficient2 * t + xCoefficient3;
}

//void BezierCurve::Print() {
//	for (unsigned int i = 0; i < xControlPoints.size(); i++) {
//		cout << '\t';
//		xControlPoints[i].Print();
//	}
//}

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

Point BezierSurface::Evaluate(float u, float v)
{
	Point p0, p1, p2, p3;

	// Determine point at u value on all 4 curves
	p0 = xControlCurves[0].Evaluate(u);
	p1 = xControlCurves[1].Evaluate(u);
	p2 = xControlCurves[2].Evaluate(u);
	p3 = xControlCurves[3].Evaluate(u);

	// Interpolate point based on v time step
	Point a = (-1.0f * p0 + 3.0f * p1 + (-3.0f) * p2 + p3);
	Point b = (3.0f * p0 + (-6.0f) * p1 + 3.0f * p2);
	Point c = (-3.0f * p0 + 3.0f * p1);

	Point f = a * v * v * v + b * v * v + c * v + p0;
	return f;
}
//
//float BezierSurface::BinarySearchU(float u, float min, float max, float z)
//{
//	float epsilon = 0.001;
//	Point p = xControlCurves[0].Evaluate(u);
//	float diff = z - p.getZ();
//	if (diff < epsilon && diff > -1 * epsilon)
//		return u;
//	else if (diff > 0)
//		return BinarySearchU(max - (max - u) / 2, u, max, z);
//	else
//		return BinarySearchU(min + (u - min) / 2, min, u, z);
//}
//
//float BezierSurface::BinarySearchV(float u, float v, float min, float max, float x)
//{
//	float epsilon = 0.001;
//	Point p = Evaluate(u, v);
//	float diff = x - p.getX();
//	if (diff < epsilon && diff > -1 * epsilon)
//		return v;
//	else if (diff > 0)
//		return BinarySearchV(u, max - (max - v) / 2, v, max, x);
//	else
//		return BinarySearchV(u, min + (v - min) / 2, min, v, x);
//}
//
//Point BezierSurface::EvaluateBasedOnXZ(float x, float z)
//{
//	// number of time steps
//	int resolution = xResolution * 2;
//
//	//Use Binary Search to find closest time step value in u direction
//	float it = BinarySearchU(0.5, 0, 1, z);
//	float jt = BinarySearchV(it, 0.5, 0, 1, x);
//
//	//Simply add a small time step to the value to get the next time step
//	float it1 = it + 1 / (float)resolution;
//	float jt1 = jt + 1 / (float)resolution;
//
//	// Use Evaluate function to get point at the time step value
//	Point p0 = Evaluate(it, jt);
//	Point p1 = Evaluate(it, jt1);
//	Point p2 = Evaluate(it1, jt1);
//	Point p3 = Evaluate(it1, jt);
//
//	// Get percentage [0..1] of our provided x,z point inside the square.
//	// How far is it in the X direction and in the Z direction
//	float tx = 0, tz = 0;
//	tx = (x - p0.getX()) / (p1.getX() - p0.getX());
//	tz = (z - p0.getZ()) / (p3.getZ() - p0.getZ());
//
//	cout << "u = " << it << ", v = " << jt << endl;
//	// bilinear interpolation based on percentage values
//	Point p = (1 - tz) * ((1 - tx) * p0 + tx * p1) + tz * ((1 - tx) * p3 + tx * p2);
//	return p;
//}
//

//Point BezierSurface::EvaluateBasedOnXZ_V2(float x, float z)
//{
//	float u = BinarySearchU(0.5, 0, 1, z);
//	float v = BinarySearchV(u, 0.5, 0, 1, x);
//
//	Point p = Evaluate(u, v);
//	return p;
//}
//
//Point BezierSurface::EvaluateNormalBasedOnXZ(float x, float z)
//{
//	// number of time steps
//	int resolution = xResolution * 2;
//
//	//Use Binary Search to find closest time step value in u direction
//	float it = BinarySearchU(0.5, 0, 1, z);
//	float jt = BinarySearchV(it, 0.5, 0, 1, x);
//
//	//Simply add a small time step to the value to get the next time step
//	float it1 = it + 1 / (float)resolution;
//	float jt1 = jt + 1 / (float)resolution;
//
//	// Use Evaluate function to get point at the time step value
//	Point p0 = Evaluate(it, jt);
//	Point p1 = Evaluate(it, jt1);
//	Point p3 = Evaluate(it1, jt);
//
//	// Get the vector in the x direction along the side of the quad
//	Point vecX = p1 - p0;
//
//	// Get the vector in the Z direction along the side of the quad
//	Point vecZ = p3 - p0;
//
//	// Cross the two vectors to get the normal pointing up from the square
//	return Cross(vecZ, vecX);
//}

//void BezierSurface::EvaluateLocationAndNormalBasedOnXZ(float x, float z, Point & loc, Point & norm)
//{
//	// number of time steps
//	int resolution = xResolution * 2;
//
//	//Use Binary Search to find closest time step value in u direction
//	float it = BinarySearchU(0.5, 0, 1, z);
//	float jt = BinarySearchV(it, 0.5, 0, 1, x);
//
//	//Simply add a small time step to the value to get the next time step
//	float it1 = it + 1 / (float)resolution;
//	float jt1 = jt + 1 / (float)resolution;
//
//	// Use Evaluate function to get point at the time step value
//	Point p0 = Evaluate(it, jt);
//	Point p1 = Evaluate(it, jt1);
//	Point p2 = Evaluate(it1, jt1);
//	Point p3 = Evaluate(it1, jt);
//
//	// Get percentage [0..1] of our provided x,z point inside the square.
//	// How far is it in the X direction and in the Z direction
//	float tx = 0, tz = 0;
//	tx = (x - p0.getX()) / (p1.getX() - p0.getX());
//	tz = (z - p0.getZ()) / (p3.getZ() - p0.getZ());
//
//	// bilinear interpolation based on percentage values
//	loc = (1 - tz) * ((1 - tx) * p0 + tx * p1) + tz * ((1 - tx) * p3 + tx * p2);
//
//	// Get the vector in the Z direction along the side of the quad
//	Vector vecX = p1 - p0;
//	Vector vecZ = p3 - p0;
//	norm = cross(vecZ, vecX);
//}