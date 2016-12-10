#ifndef _BUBBLE_SYSTEM_H_
#define _BUBBLE_SYSTEM_H_

#include <vector>
#include "Bubble.h"

class BubbleSystem{
	std::vector<Bubble*> _Bubbles;

public:
	BubbleSystem();

	void Update();
	void Draw();
};

#endif 