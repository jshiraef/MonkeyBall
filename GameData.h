#pragma once

#include "Texture.h"

//Image array size
#define NUM_TEX		 15

//Image identifiers
enum {
	IMG_GRASS,
	IMG_ROCK,
	IMG_WATER,
	IMG_SKYBOX,
	IMG_PLAYER,
	IMG_PLAYER_NMAP,
	IMG_CIRCLE_ON,
	IMG_CIRCLE_OFF,
	IMG_VORTEX,
	IMG_KEY,
	IMG_KEY_NMAP,
	IMG_PORTAL,
	IMG_PORTAL_NMAP,
	IMG_LANDMARK,
	IMG_LANDMARK_NMAP
};

class GameData
{
public:
	GameData(void);
	~GameData(void);

	int  GetID(int img);
	void GetSize(int img, int *w, int *h);
	bool Load();

private:
	Texture textures[NUM_TEX];
	bool LoadImage(int img, char *filename, int type = GL_RGBA);
};

