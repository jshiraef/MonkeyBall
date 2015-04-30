#pragma once

#include "Globals.h"


#define CIRCLE_RADIUS 2.0f
#define AURA_HEIGHT 3.0f
#define HEIGHT_OFFSET 0.05f

class CheckPoint
{
public:
	CheckPoint();
	~CheckPoint();
	void  Draw(int tex_id, bool activated);
	void  SetPos(float posx, float posy, float posz);
	float GetX();
	float GetY();
	float GetZ();

private:
	float x, y, z;
};
