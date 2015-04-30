#pragma once

#include "Globals.h"
#include "Model.h"
#include "GameData.h"

#define PORTAL_SIDE  3.0f
#define PORTAL_SPEED 2.0f

class Portal
{
public:
	Portal();
	~Portal();
	void Draw(GameData *Data, bool activated, Model *Model);
	void SetPos(float posx, float posy, float posz);
	bool InsidePortal(float px, float py, float pz, float r);
	float GetX();
	float GetY();
	float GetZ();
	float GetReceptorX(int i);
	float GetReceptorY(int i);

private:
	float x, y, z;
	float ang;
	std::pair<float, float> receptors[5];
};
