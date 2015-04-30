#include "Scene.h"

Scene::Scene(void) {}
Scene::~Scene(void) {}


void Scene::LoadLevel(int level, Terrain *Terrain, Water *Water, float zfar)
{
	Terrain->Load(level);
	Skybox.Load(zfar);
	Water->Load(TERRAIN_SIZE);
}

void Scene::Draw(GameData *Data, Terrain *Terrain, Water *Water, Coord playerPos)
{
	glDisable(GL_LIGHTING);

	glDisable(GL_DEPTH_TEST); //disable the depth test so the player does not look like the sky
	Skybox.Draw(Data->GetID(IMG_SKYBOX), playerPos.x, playerPos.y, playerPos.z);
	glEnable(GL_DEPTH_TEST);

	//DrawOGLAxis();

	Water->Draw(Data->GetID(IMG_WATER));

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_GRASS));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_ROCK));
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);


		Terrain->Draw();


	glEnable(GL_LIGHTING);
}