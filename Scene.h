#pragma once

#include "GameData.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Water.h"


class Scene
{
public:
	Scene(void);
	virtual ~Scene(void);

	void LoadLevel(int level, Terrain *Terrain, Water *Water, float zfar);
	void Draw(GameData *Data, Terrain *Terrain, Water *Water, Coord playerPos);

private:
	Skybox Skybox;
};

