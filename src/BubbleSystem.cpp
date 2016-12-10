#include "BubbleSystem.h"
#include "Map.h"
#include "Hero.h"
#include "cmath"

#include <iostream>

extern double getRand();
extern Map* gMap;
extern Hero* gHero;

extern double maxHealth;

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
		if (d.mag() > maxd - widthOfTrack )
			d -= widthOfTrack * 2 * Dist;

		Point pos = gMap->getPointAtIndex(index) + d;

		Vector norm = cross(Dist, Vector(0,1,0));
		norm.normalize();

		pos += RandWidth * norm - (widthOfTrack/2.0 * norm);

		_Bubbles.push_back(new Bubble(pos.getX(),pos.getZ()));
	}
}

void BubbleSystem::Update(){
	for (int i = 0; i < _Bubbles.size(); ++i){
		_Bubbles[i]->Update();

		if (gHero){
			Vector dist = gHero->getLocation() - _Bubbles[i]->getLocation();
			int bonus = 2;
			if (dist.mag() < 1.25)
				if (gHero->getHealth() < maxHealth - bonus)
					gHero->incrementHealth(bonus); 
		}	
	}
}
void BubbleSystem::Draw(){
	for (int i = 0; i < _Bubbles.size(); ++i)
		_Bubbles[i]->Draw();
}