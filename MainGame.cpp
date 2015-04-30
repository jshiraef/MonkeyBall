#include "MainGame.h"

MainGame::MainGame(void) {}
MainGame::~MainGame(void) {}

//delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>

#include <GLUT/glut.h>
#  include <GLUT/glut.h>

#  include <OpenGL/glext.h>
#else
#  include <gl/glew.h>
//#  include <gl/freeglut.h>
// #  include <gl/glext.h>
#  include <gl/glut.h>
#  include <gl/gl.h>

#pragma comment(lib, "glew32.lib") 
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


bool MainGame::Init(int lvl)
{
	bool res = true;
	noclip = false;
	portal_activated = false;
	time = ang = 0.0f;
	noclipSpeedF = 1.0f;
	level = lvl;
	state = STATE_RUN;
	respawn_id = 0;
	pickedkey_id = -1;

	//Graphics initialization
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_ZNEAR, CAMERA_ZFAR);
	glMatrixMode(GL_MODELVIEW);

	const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[] = { 50.0f, 50.0f, 50.0f, 0.0f };
	const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	const GLfloat mat_shininess[] = { 100.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Texture initialization
	Data.Load();

	//level initialization(terrain, water and skybox)
	Scene.LoadLevel(level, &Terrain, &Water, CAMERA_ZFAR);

	//Sound initialization
	Sound.Load();

	////Shader initialization
	//Shader.Load();

	//Model initialization
	Model.Load();


	//Player initialization
	Player.SetPos(TERRAIN_SIZE / 2, Terrain.GetHeight(TERRAIN_SIZE / 2, TERRAIN_SIZE / 2) + RADIUS, TERRAIN_SIZE / 2);

	//Portal initialization
	Portal.SetPos(TERRAIN_SIZE / 2, Terrain.GetHeight(TERRAIN_SIZE / 2, TERRAIN_SIZE / 2 + 32), TERRAIN_SIZE / 2 + 32);

	//respawn points initialization
	CheckPoint rp;
	rp.SetPos(TERRAIN_SIZE / 2, Terrain.GetHeight(TERRAIN_SIZE / 2, TERRAIN_SIZE / 2), TERRAIN_SIZE / 2);
	respawn_points.push_back(rp);
	rp.SetPos(256, Terrain.GetHeight(256, 160), 160);
	respawn_points.push_back(rp);
	rp.SetPos(840, Terrain.GetHeight(840, 184), 184);
	respawn_points.push_back(rp);
	rp.SetPos(552, Terrain.GetHeight(552, 760), 760);
	respawn_points.push_back(rp);
	rp.SetPos(791, Terrain.GetHeight(791, 850), 850);
	respawn_points.push_back(rp);
	rp.SetPos(152, Terrain.GetHeight(152, 832), 832);
	respawn_points.push_back(rp);
	rp.SetPos(448, Terrain.GetHeight(448, 944), 944);
	respawn_points.push_back(rp);
	rp.SetPos(816, Terrain.GetHeight(816, 816), 816);
	respawn_points.push_back(rp);

	Sound.Play(SOUND_AMBIENT);

	return res;
}

bool MainGame::Loop()
{
	bool res = true;
	int t1, t2;
	t1 = glutGet(GLUT_ELAPSED_TIME);

	if (state == STATE_RUN)
	{
		res = Movement();
		if (res) Render();
	}
	else if (state == STATE_LIVELOSS)
	{
		Render();
		Player.SetPos(respawn_points[respawn_id].GetX(), respawn_points[respawn_id].GetY() + RADIUS, respawn_points[respawn_id].GetZ());
		state = STATE_RUN;
	}
	else if (state == STATE_ENDGAME)
	{
		res = false;
	}

	do {
		t2 = glutGet(GLUT_ELAPSED_TIME);
	} while (t2 - t1 < 1000 / FRAMERATE);
	return res;
}

void MainGame::Finalize()
{
}

//Input
void MainGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	if (key >= 'A' && key <= 'Z') key += 32;
	keys[key] = press;
}
void MainGame::ReadSpecialKeyboard(unsigned char key, int x, int y, bool press)
{
	if (key == GLUT_KEY_F10 && press)
	{
		if (!noclip) Sound.Play(SOUND_SCREWGRAVITY);
		noclip = !noclip;
		Player.SetVel(0.0f, 0.0f, 0.0f);
	}
}

void MainGame::ReadMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			mouse_left_down = true;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			mouse_right_down = true;
		}
	}
	else if (state == GLUT_UP) {
		if (button == GLUT_LEFT_BUTTON) {
			mouse_left_down = false;
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			mouse_right_down = false;
		}
	}
}

void MainGame::ReadMouseMotion(int x, int y)
{
	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
	// This avoids it being called recursively and hanging up the event loop
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}


	int dx = x - SCREEN_WIDTH / 2;
	int dy = y - SCREEN_HEIGHT / 2;

	if (dx) {
		Camera.TurnRotation(CAMERA_SPEED*dx);
		if (Camera.GetState() != STATE_TPS_FREE) Camera.SetLastRotation(Camera.GetRotation());
	}

	if (dy) {
		Camera.RotatePitch(-CAMERA_SPEED*dy);
	}

	glutWarpPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	just_warped = true;
}

//Camera and player movement
bool MainGame::Movement()
{
	bool res = true;

	//Process Input
	if (keys[27])	res = false;

	float vx, vy, vz;
	Camera.GetDirectionVector(vx, vy, vz);
	float factor = sqrt(1.0f / (vx*vx + vz*vz));

	if (keys['1']) Camera.SetState(STATE_FPS);
	Player.SetFade(!keys['2']);
	if (keys['3']) Camera.SetState(STATE_TPS);

	if (noclip)
	{
		if (keys[P_UP])
		{
			Player.SetX(Player.GetX() + noclipSpeedF*vx);
			Player.SetY(Player.GetY() + noclipSpeedF*vy);
			Player.SetZ(Player.GetZ() + noclipSpeedF*vz);
			if (Camera.GetState() == STATE_TPS_FREE) Camera.SetLastRotation(Camera.GetRotation());
		}
		else if (keys[P_DOWN])
		{
			Player.SetX(Player.GetX() - noclipSpeedF*vx);
			Player.SetY(Player.GetY() - noclipSpeedF*vy);
			Player.SetZ(Player.GetZ() - noclipSpeedF*vz);
			if (Camera.GetState() == STATE_TPS_FREE) Camera.SetLastRotation(Camera.GetRotation() - PI);
		}
		if (keys[P_LEFT])
		{
			Player.SetX(Player.GetX() + noclipSpeedF*vz*factor);
			Player.SetZ(Player.GetZ() - noclipSpeedF*vx*factor);
			if (Camera.GetState() == STATE_TPS_FREE)
			{
				if (keys['w']) Camera.SetLastRotation(Camera.GetRotation() - PI / 4);
				else if (keys['s']) Camera.SetLastRotation(Camera.GetRotation() - (PI * 3) / 4);
				else Camera.SetLastRotation(Camera.GetRotation() - PI / 2);
			}
		}
		else if (keys[P_RIGHT])
		{
			Player.SetX(Player.GetX() - noclipSpeedF*vz*factor);
			Player.SetZ(Player.GetZ() + noclipSpeedF*vx*factor);
			if (Camera.GetState() == STATE_TPS_FREE)
			{
				if (keys['w']) Camera.SetLastRotation(Camera.GetRotation() + PI / 4);
				else if (keys['s']) Camera.SetLastRotation(Camera.GetRotation() + (PI * 3) / 4);
				else Camera.SetLastRotation(Camera.GetRotation() + PI / 2);
			}
		}
		if (mouse_left_down) Player.SetY(Player.GetY() + noclipSpeedF);
		else if (mouse_right_down) Player.SetY(Player.GetY() - noclipSpeedF);
		if (keys[P_PLUS])
		{
			noclipSpeedF += 0.01f;
			if (noclipSpeedF > 2.0f) noclipSpeedF = 2.0f;
		}
		else if (keys[P_MINUS])
		{
			noclipSpeedF -= 0.01f;
			if (noclipSpeedF < 0.05f) noclipSpeedF = 0.05f;
		}
	}
	else
	{
		if (keys[P_UP])
		{
			float nextVX = Player.GetVX() + PLAYER_SPEED*vx*factor;
			float nextVZ = Player.GetVZ() + PLAYER_SPEED*vz*factor;
			float limitation_factor;
			if (sqrt(nextVX*nextVX + nextVZ*nextVZ) <= MAX_MOVEMENT) limitation_factor = 1.0f;
			else limitation_factor = sqrt((MAX_MOVEMENT*MAX_MOVEMENT) / (nextVX*nextVX + nextVZ*nextVZ));
			Player.SetVX(nextVX*limitation_factor);
			Player.SetVZ(nextVZ*limitation_factor);

			if (Camera.GetState() == STATE_TPS_FREE) Camera.SetLastRotation(Camera.GetRotation());
		}
		else if (keys[P_DOWN])
		{
			float nextVX = Player.GetVX() - PLAYER_SPEED*vx*factor;
			float nextVZ = Player.GetVZ() - PLAYER_SPEED*vz*factor;
			float limitation_factor;
			if (sqrt(nextVX*nextVX + nextVZ*nextVZ) <= MAX_MOVEMENT) limitation_factor = 1.0f;
			else limitation_factor = sqrt((MAX_MOVEMENT*MAX_MOVEMENT) / (nextVX*nextVX + nextVZ*nextVZ));
			Player.SetVX(nextVX*limitation_factor);
			Player.SetVZ(nextVZ*limitation_factor);

			if (Camera.GetState() == STATE_TPS_FREE) Camera.SetLastRotation(Camera.GetRotation() - PI);
		}
		if (keys[P_LEFT])
		{
			float nextVX = Player.GetVX() + PLAYER_SPEED*vz*factor;
			float nextVZ = Player.GetVZ() - PLAYER_SPEED*vx*factor;
			float limitation_factor;
			if (sqrt(nextVX*nextVX + nextVZ*nextVZ) <= MAX_MOVEMENT) limitation_factor = 1.0f;
			else limitation_factor = sqrt((MAX_MOVEMENT*MAX_MOVEMENT) / (nextVX*nextVX + nextVZ*nextVZ));
			Player.SetVX(nextVX*limitation_factor);
			Player.SetVZ(nextVZ*limitation_factor);

			if (Camera.GetState() == STATE_TPS_FREE)
			{
				if (keys['w']) Camera.SetLastRotation(Camera.GetRotation() - PI / 4);
				else if (keys['s']) Camera.SetLastRotation(Camera.GetRotation() - (PI * 3) / 4);
				else Camera.SetLastRotation(Camera.GetRotation() - PI / 2);
			}
		}
		else if (keys[P_RIGHT])
		{
			float nextVX = Player.GetVX() - PLAYER_SPEED*vz*factor;
			float nextVZ = Player.GetVZ() + PLAYER_SPEED*vx*factor;
			float limitation_factor;
			if (sqrt(nextVX*nextVX + nextVZ*nextVZ) <= MAX_MOVEMENT) limitation_factor = 1.0f;
			else limitation_factor = sqrt((MAX_MOVEMENT*MAX_MOVEMENT) / (nextVX*nextVX + nextVZ*nextVZ));
			Player.SetVX(nextVX*limitation_factor);
			Player.SetVZ(nextVZ*limitation_factor);

			if (Camera.GetState() == STATE_TPS_FREE)
			{
				if (keys['w']) Camera.SetLastRotation(Camera.GetRotation() + PI / 4);
				else if (keys['s']) Camera.SetLastRotation(Camera.GetRotation() + (PI * 3) / 4);
				else Camera.SetLastRotation(Camera.GetRotation() + PI / 2);
			}
		}
		if (keys[P_JUMP])
		{
			//float radius;
			//float radius2;
			//float radius3;


			if (Player.GetY() - RADIUS < Terrain.GetHeight(Player.GetX(), Player.GetZ()) + 0.01f)
			{
				Player.SetVY(PLAYER_JUMP_SPEED);
				Sound.PlayBounce(1.0f);
			}
		}

		float initial_z = Player.GetZ();
		Physics(Player);

		//checks if the player is dead
		if (Player.GetY() <= Water.GetHeight() + RADIUS)
		{
			Player.SetY(Water.GetHeight() + RADIUS);
			Player.SetVel(0.0f, 0.0f, 0.0f);
			pickedkey_id = -1;
			state = STATE_LIVELOSS;
			Sound.Play(SOUND_SWISH);

		}

		Coord P; P.x = Player.GetX(); P.y = Player.GetY(); P.z = Player.GetZ();

		float r = RADIUS;

		//checks if the player enters a respawn Point
		float cr = CIRCLE_RADIUS, ah = AURA_HEIGHT;
		for (unsigned int i = 0; i<respawn_points.size(); i++)
		{
			Coord RP; RP.x = respawn_points[i].GetX(); RP.y = respawn_points[i].GetY(); RP.z = respawn_points[i].GetZ();
			if (sqrt((P.x - RP.x)*(P.x - RP.x) + (P.y - RP.y)*(P.y - RP.y) + (P.z - RP.z)*(P.z - RP.z)) <= RADIUS + CIRCLE_RADIUS)
			{
				if (respawn_id != i) Sound.Play(SOUND_SWISH);
				respawn_id = i;
			}
		}

		

		//checks if the player crosses the checkPoint
		if (portal_activated)
		{
			if (Portal.InsidePortal(P.x, P.y, P.z, RADIUS))
			{
				if ((initial_z - Portal.GetZ() <= 0.0f && Player.GetZ() - Portal.GetZ() >= 0.0f) ||
					(initial_z - Portal.GetZ() >= 0.0f && Player.GetZ() - Portal.GetZ() <= 0.0f)) state = STATE_ENDGAME;
			}
		}
	}

	//clear sound buffer
	Sound.Update();

	return res;
}

void MainGame::Physics(Debug &object)
{
	Coord initialPos; initialPos.x = object.GetX(); initialPos.y = object.GetY(); initialPos.z = object.GetZ();
	Coord center; center.x = object.GetX() + object.GetVX(); center.y = object.GetY() + object.GetVY(); center.z = object.GetZ() + object.GetVZ();
	std::vector<Vector> cnormals = Terrain.GetCollisionNormals(center, RADIUS);
	object.SetPos(center.x, center.y, center.z); //GetCollisionNormals after the center position will validate a position on the surface

												 //update angles of rotation
	if (object.GetZ() != initialPos.z || object.GetX() != initialPos.x)
	{
		float rotation, pitch;
		float dx = abs(abs(object.GetX()) - abs(initialPos.x)), dz = abs(abs(object.GetZ()) - abs(initialPos.z));
		if (object.GetZ() > initialPos.z && object.GetX() >= initialPos.x) rotation = atan(dx / dz); //first
		if (object.GetZ() <= initialPos.z && object.GetX() > initialPos.x) rotation = PI / 2 + atan(dz / dx); //second
		if (object.GetZ() < initialPos.z && object.GetX() <= initialPos.x) rotation = PI + atan(dx / dz);//third
		if (object.GetZ() >= initialPos.z && object.GetX() < initialPos.x) rotation = PI * 3 / 2 + atan(dz / dx);//fourth
		object.SetRotation(rotation*(180 / PI));

		float perimeter = PI * 2 * RADIUS;
		float dy = abs(abs(object.GetY()) - abs(initialPos.y));
		float travel_dist = sqrt(dx*dx + dy*dy + dz*dz);
		if (cos(rotation) >= 0.0f) pitch = object.GetPitch() + (travel_dist / perimeter) * 360.0f;
		else pitch = object.GetPitch() - (travel_dist / perimeter) * 360.0f;
		if (pitch < 0.0f) pitch = 360.0f - abs(pitch);
		object.SetPitch(fmod(pitch, 360.0f));
	}

	if (cnormals.empty()) object.SetVY(object.GetVY() - GRAVITY);
	else
	{
		Vector G, F, G1, F1, cNormal;
		float rz, rx; //angle of rotation
		float factor, N = 0.0f;

		G.x = 0.0f; G.y = -GRAVITY; G.z = 0.0f;
		F.x = object.GetVX(); F.y = object.GetVY(); F.z = object.GetVZ();
		cNormal.x = 0.0f; cNormal.y = 0.0f; cNormal.z = 0.0f;

		for (unsigned int i = 0; i<cnormals.size(); i++)
		{
			if (cnormals[i].x == 0.0f) rz = 0.0f;
			else if (cnormals[i].x >  0.0f) rz = -PI / 2 + atan(cnormals[i].y / cnormals[i].x);
			else rz = PI / 2 + atan(cnormals[i].y / cnormals[i].x);

			if (cnormals[i].z == 0.0f) rx = 0.0f;
			else if (cnormals[i].z >  0.0f) rx = PI / 2 - atan(cnormals[i].y / cnormals[i].z);
			else rx = -PI / 2 - atan(cnormals[i].y / cnormals[i].z);

			//transform OpenGL coordinate system to the coordinate system defined by cnormal
			G1.x = cos(-rz)*G.x - sin(-rz)*G.y;
			G1.y = cos(-rx)*sin(-rz)*G.x + cos(-rx)*cos(-rz)*G.y - sin(-rx)*G.z;
			G1.z = sin(-rx)*sin(-rz)*G.x + sin(-rx)*cos(-rz)*G.y + cos(-rx)*G.z;

			F1.x = cos(-rz)*F.x - sin(-rz)*F.y;
			F1.y = cos(-rx)*sin(-rz)*F.x + cos(-rx)*cos(-rz)*F.y - sin(-rx)*F.z;
			F1.z = sin(-rx)*sin(-rz)*F.x + sin(-rx)*cos(-rz)*F.y + cos(-rx)*F.z;

			//normal force for a single triangle
			float cN = 0.0f;
			if (G1.y < 0.0f) { cN -= G1.y; G1.y = 0.0f; }
			if (F1.y < 0.0f) { cN -= F1.y; F1.y = 0.0f; }
			N += cN; //actualizo la fuerza normal global

					 //force update global cnormal
			cNormal.x += cnormals[i].x;
			cNormal.y += cnormals[i].y;
			cNormal.z += cnormals[i].z;

			// possible friction
			if (cN > 0.0f && abs(F1.x) + abs(F1.z) > 0.0f)
			{
				factor = sqrt(((FRICTION*cN)*(FRICTION*cN)) / (F1.x*F1.x + F1.z*F1.z));

				if (abs(F1.x) < abs(F1.x*factor)) F1.x = 0.0f;
				else F1.x -= F1.x*factor;

				if (abs(F1.z) < abs(F1.z*factor)) F1.z = 0.0f;
				else F1.z -= F1.z*factor;
			}

			//return normal coordinate system ( y-axis) to the coordinate system of OpenGL
			G.x = cos(rz)*G1.x - sin(rz)*cos(rx)*G1.y + sin(rz)*sin(rx)*G1.z;
			G.y = sin(rz)*G1.x + cos(rz)*cos(rx)*G1.y - cos(rz)*sin(rx)*G1.z;
			G.z = sin(rx)*G1.y + cos(rx)*G1.z;

			F.x = cos(rz)*F1.x - sin(rz)*cos(rx)*F1.y + sin(rz)*sin(rx)*F1.z;
			F.y = sin(rz)*F1.x + cos(rz)*cos(rx)*F1.y - cos(rz)*sin(rx)*F1.z;
			F.z = sin(rx)*F1.y + cos(rx)*F1.z;
		}

		float nextVX = F.x + G.x;
		float nextVY = F.y + G.y;
		float nextVZ = F.z + G.z;

		//speed limit for the area not skip triangles
		float limitation_factor;
		if (sqrt(nextVX*nextVX + nextVY*nextVY + nextVZ*nextVZ) <= MAX_MOVEMENT) limitation_factor = 1.0f;
		else limitation_factor = sqrt((MAX_MOVEMENT*MAX_MOVEMENT) / (nextVX*nextVX + nextVY*nextVY + nextVZ*nextVZ));

		nextVX *= limitation_factor;
		nextVY *= limitation_factor;
		nextVZ *= limitation_factor;

		//bounciness
		if (N > GRAVITY * 4) factor = sqrt((N*N) / (cNormal.x*cNormal.x + cNormal.y*cNormal.y + cNormal.z*cNormal.z));
		else factor = 0.0f;

		nextVX += cNormal.x*factor*ELASTICITY;
		nextVY += cNormal.y*factor*ELASTICITY;
		nextVZ += cNormal.z*factor*ELASTICITY;

		float bounceForce = sqrt((cNormal.x*factor*ELASTICITY)*(cNormal.x*factor*ELASTICITY) + (cNormal.y*factor*ELASTICITY)*(cNormal.y*factor*ELASTICITY) + (cNormal.z*factor*ELASTICITY)*(cNormal.z*factor*ELASTICITY));
		if (bounceForce >= PLAYER_JUMP_SPEED) Sound.PlayBounce(1.0f);
		else if (bounceForce / PLAYER_JUMP_SPEED > 0.2f) Sound.PlayBounce(bounceForce / PLAYER_JUMP_SPEED);

		//update speed
		object.SetVel(nextVX, nextVY, nextVZ);
	}
}

//Output
void MainGame::resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();


	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, CAMERA_ZNEAR, CAMERA_ZFAR); //set the perspective (angle of sight, width, height, ,depth)
				  //   glFrustum(-5,5,-5,5,5,200);
				 //   glOrtho(-5,5,-5,5,0,200);
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

void MainGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//updates
	if (state != STATE_LIVELOSS) Water.Update();
	Camera.Update(&Terrain, &Water, Player.GetX(), Player.GetY(), Player.GetZ());
	ang = fmod(ang + 2, 360);

	Coord playerPos; playerPos.x = Player.GetX(); playerPos.y = Player.GetY(); playerPos.z = Player.GetZ();

	//draw scene(terrain + water + skybox)
	Scene.Draw(&Data, &Terrain, &Water, playerPos);

	////draw keys
	//for (unsigned int i = 0; i<target_keys.size(); i++)
	//{
	//	//color dye
	//	if (i == 0) glColor3f(1.0f, 0.0f, 0.0f); //red
	// }

	//	glColor4f(1, 1, 1, 1);
	//}

	//draw landmarks
//	for (unsigned int i = 0; i<landmarks.size(); i++) landmarks[i].Draw(&Model, &Data, i);

	if (abs(Camera.GetZ() - Portal.GetZ()) < Camera.GetDistance())
	{
		//draw player
		Player.Draw(&Data, &Camera, &Water);

		//draw portal
		Portal.Draw(&Data, portal_activated, &Model);
	}
	else
	{
		//draw portal
		Portal.Draw(&Data, portal_activated, &Model);

		//draw player
		Player.Draw(&Data, &Camera, &Water);
	}

	//draw respawn points
	for (unsigned int i = 0; i<respawn_points.size(); i++)
	{
		if (i == respawn_id) respawn_points[i].Draw(Data.GetID(IMG_CIRCLE_ON), true);
		else respawn_points[i].Draw(Data.GetID(IMG_CIRCLE_OFF), false);
	}

	glutSwapBuffers();
}

MainGame Game;

void AppResize(int w, int h)
{
	Game.resize(w, h);
}
void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key, x, y, true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key, x, y, false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key, x, y, true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadSpecialKeyboard(key, x, y, false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button, state, x, y);
}
void AppMouseMotion(int x, int y)
{
	Game.ReadMouseMotion(x, y);
}
void AppIdle()
{
	if (!Game.Loop()) exit(0);
}

// Main routine.
void main(int argc, char** argv)
{
	int res_x, res_y, pos_x, pos_y;


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x >> 1) - (SCREEN_WIDTH >> 1);
	pos_y = (res_y >> 1) - (SCREEN_HEIGHT >> 1);

	glutInitWindowPosition(pos_x, pos_y);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Monkey Ball");
	glutFullScreen();

	//glutGameModeString("800x600:32");
	//glutEnterGameMode();

	//Make the default cursor disappear
	glutSetCursor(GLUT_CURSOR_NONE);

	//Register callback functions
	glutReshapeFunc(AppResize);
	glutDisplayFunc(AppRender);
	glutKeyboardFunc(AppKeyboard);
	glutKeyboardUpFunc(AppKeyboardUp);
	glutSpecialFunc(AppSpecialKeys);
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutMotionFunc(AppMouseMotion);
	glutPassiveMotionFunc(AppMouseMotion);
	glutIdleFunc(AppIdle);


	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}


	Game.Init(1);

	glutMainLoop();
}
