#pragma once

#include "Globals.h"

class Skybox
{
public:
	Skybox();
	~Skybox();
	void Load(float zfar);
	void Draw(int tex_id, float player_x, float player_y, float player_z);

private:
	int id_Skybox;
};
