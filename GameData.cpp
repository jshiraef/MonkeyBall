#include "GameData.h"

GameData::GameData(void) {}
GameData::~GameData(void) {}

int GameData::GetID(int img)
{
	return textures[img].GetID();
}

void GameData::GetSize(int img, int *w, int *h)
{
	textures[img].GetSize(w, h);
}

bool GameData::LoadImage(int img, char *filename, int type)
{
	int res;

	res = textures[img].Load(filename, type);
	if (!res) return false;

	return true;
}
bool GameData::Load()
{
	int res;

	res = LoadImage(IMG_GRASS, "Textures/rock.png", GL_RGBA);
	if (!res) return false;
	//res = LoadImage(IMG_ROCK, "Textures/grass.png", GL_RGBA);
	//if (!res) return false;
	//res = LoadImage(IMG_WATER, "Textures/water.png", GL_RGBA);
	//if (!res) return false;
	//res = LoadImage(IMG_SKYBOX, "Textures/skybox.png", GL_RGBA);
	//if (!res) return false;
	//res = LoadImage(IMG_PLAYER, "Textures/player.png", GL_RGBA);
	//if (!res) return false;
	//res = LoadImage(IMG_PLAYER_NMAP, "Textures/playerNmap.png", GL_RGBA);
	//if (!res) return false;
	//res = LoadImage(IMG_CIRCLE_ON, "Textures/circle_on.png", GL_RGBA);
	//if (!res) return false;

	//res = LoadImage(IMG_PORTAL, "Textures/portal.png", GL_RGBA);
	//if (!res) return false;




	return true;
}