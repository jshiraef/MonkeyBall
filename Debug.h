#pragma once

#include "Scene.h"

class Debug
{
public:
	Debug(void);
	~Debug(void);

	void  SetPos(float posx, float posy, float posz);
	void  SetX(float posx);
	void  SetY(float posy);
	void  SetZ(float posz);
	float GetX();
	float GetY();
	float GetZ();

	void  SetVel(float velx, float vely, float velz);
	void  SetVX(float velx);
	void  SetVY(float vely);
	void  SetVZ(float velz);
	float GetVX();
	float GetVY();
	float GetVZ();

	void  SetRotation(float ang);
	void  SetPitch(float ang);
	float GetRotation();
	float GetPitch();

	void SetState(int s);
	int  GetState();

	void SetMaxHealth(int max_h);
	void SetHealth(int h);
	int  GetMaxHealth();
	int  GetHealth();

private:
	float x, y, z;   // Position
	float vx, vy, vz;   // Velocity
	float rotation, pitch;   // rotation angles (by movement) in degrees
	int max_health, health, state;
};
#pragma once
