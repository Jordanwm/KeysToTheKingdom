#include "BubbleSystem.h"
#include "Map.h"
#include "cmath"

#include <iostream>

extern double getRand();
extern Map* gMap;

BubbleSystem::BubbleSystem()
{
	int numberOfBubbles = 10;
	
	// Basic info about map
	double widthOfTrack = 0;
	double length = 0;

	if (gMap){
		widthOfTrack = gMap->getWidthOfTrack();
		length = gMap->getNumPointsOnTrack();
	}

	if(widthOfTrack > 0.75 * 2)
		widthOfTrack -= 0.75 * 2;

	// Create the random bubbles
	for (int i = 0; i < numberOfBubbles; ++i) {
		double RandLength, RandWidth, RandPositionInBetweenPoints;

		RandLength = getRand() * length;
		RandWidth = getRand() * widthOfTrack;

		int index = floor(RandLength);
		if (index >= length - 1)
			index = length - 2;

		Vector Dist = gMap->getPointAtIndex(index + 1) - gMap->getPointAtIndex(index);
		double maxd = Dist.mag();

		RandPositionInBetweenPoints = getRand() * Dist.mag();
		Dist.normalize();

		Vector d = Dist * RandPositionInBetweenPoints;
		if (index == length - 1 && d.mag() < maxd - widthOfTrack )
			d -= widthOfTrack * 2 * Dist;

		Point pos = gMap->getPointAtIndex(index) + d;

		Vector norm = cross(Dist, Vector(0,1,0));
		norm.normalize();

		pos += RandWidth * norm - (widthOfTrack/2.0 * norm);

		_Bubbles.push_back(new Bubble(pos.getX(),pos.getZ()));
	}
}

void BubbleSystem::Update(){
	for (int i = 0; i < _Bubbles.size(); ++i)
		_Bubbles[i]->Update();
}
void BubbleSystem::Draw(){
	for (int i = 0; i < _Bubbles.size(); ++i)
		_Bubbles[i]->Draw();
}