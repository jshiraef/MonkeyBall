#pragma once

#include "Debug.h"
#include "Camera.h"

#define PLAYER_SPEED  0.008f
#define PLAYER_JUMP_SPEED  0.4f
#define FRICTION   0.05f
#define ELASTICITY   0.5f //normal range [0.0, 1.0]
#define MAX_MOVEMENT 0.4f
#define RADIUS   0.5f

class Player : public Debug
{
public:
	Player();
	~Player();

	void Draw(GameData *Data, Camera *Camera, Water *Water);
	void SetFade(bool b);

private:
	bool fade;
};
