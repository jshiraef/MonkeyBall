#include "Debug.h"

Debug::Debug(void)
{
	SetVel(0.0, 0.0, 0.0);
	rotation = 0.0f; pitch = 0.0f;
}
Debug::~Debug(void) {}

void Debug::SetPos(float posx, float posy, float posz)
{
	x = posx;
	y = posy;
	z = posz;
}
void Debug::SetX(float posx)
{
	x = posx;
}
void Debug::SetY(float posy)
{
	y = posy;
}
void Debug::SetZ(float posz)
{
	z = posz;
}
float Debug::GetX()
{
	return x;
}
float Debug::GetY()
{
	return y;
}
float Debug::GetZ()
{
	return z;
}

void Debug::SetVel(float velx, float vely, float velz)
{
	vx = velx;
	vy = vely;
	vz = velz;
}
void Debug::SetVX(float velx)
{
	vx = velx;
}
void Debug::SetVY(float vely)
{
	vy = vely;
}
void Debug::SetVZ(float velz)
{
	vz = velz;
}
float Debug::GetVX()
{
	return vx;
}
float Debug::GetVY()
{
	return vy;
}
float Debug::GetVZ()
{
	return vz;
}

void Debug::SetRotation(float ang)
{
	rotation = ang;
}
void Debug::SetPitch(float ang)
{
	pitch = ang;
}
float Debug::GetRotation()
{
	return rotation;
}
float Debug::GetPitch()
{
	return pitch;
}

void Debug::SetState(int s)
{
	state = s;
}
int Debug::GetState()
{
	return state;
}

void Debug::SetMaxHealth(int max_h)
{
	max_health = max_h;
}
int Debug::GetMaxHealth()
{
	return max_health;
}
void Debug::SetHealth(int h)
{
	health = h;
}
int Debug::GetHealth()
{
	return health;
}