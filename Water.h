#pragma once

#include "Globals.h"

#define FLOW_SPEED 2.0f
#define WATER_HEIGHT_MAX 4.0f
#define WATER_HEIGHT_MIN 1.0f

class Water
{
public:
	Water();
	~Water();
	void Load(float terrain_size);
	void Draw(int tex_id);
	float GetHeight();
	float GetHeightMax();
	void Update();

private:
	int water_id;
	float height, ang;
	bool up;
};
