#pragma once

#include "Player.h"
#include "Sound.h"
#include "CheckPoint.h"
#include "Portal.h"


#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define FRAMERATE 60
#define GRAVITY 0.015f

enum {
	STATE_LIVELOSS,
	STATE_RUN,
	STATE_ENDGAME
};

#define TOTAL_LEVELS	  1

//controls
#define P_UP		'w'
#define P_DOWN		's'
#define P_LEFT		'a'
#define P_RIGHT		'd'
#define P_JUMP		' '
#define P_PLUS		'+'
#define P_MINUS		'-'

class MainGame
{
public:
	MainGame(void);
	virtual ~MainGame(void);

	bool Init(int lvl);
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadSpecialKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	void ReadMouseMotion(int x, int y);
	//Movement
	bool Movement();
	//Output
	void resize(int w, int h);
	void Render();

private:
	unsigned char keys[256];
	bool mouse_left_down, mouse_right_down;
	int level, state, respawn_id, pickedkey_id;
	bool noclip, portal_activated;
	float time, ang, noclipSpeedF;
	std::vector<CheckPoint> respawn_points;

	Portal Portal;
	Scene Scene;
	GameData Data;
	Player Player;
	Camera Camera;
	Sound Sound;
	Model Model;
	Terrain Terrain;
	Water Water;

	void Physics(Debug &object);
};
