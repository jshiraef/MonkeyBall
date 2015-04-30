#include "Camera.h"

Camera::Camera()
{
	rotation = 0.0f;
	last_Rotation = 0.0f;
	pitch = -PI / 2;
	distance = 0.4f;
	lambda = 1.0f;
	state = STATE_TPS;
}
Camera::~Camera() {}

void Camera::Refresh()
{
	vx = cos(rotation) * cos(pitch);
	vy = sin(pitch);
	vz = sin(rotation) * cos(pitch);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, x + vx, y + vy, z + vz, 0, 1, 0);
}

float Camera::GetWaterLambda(float Py, float Qy, float height)
{
	float Vy = Qy - Py;
	float D = -height;
	if (Vy == 0.0f) return 1.0f;
	float lambda = -(Py + D) / Vy;
	if (lambda < 0.0f || lambda > 1.0f) return 1.0f;
	return lambda;
}

void Camera::Update(Terrain *Terrain, Water *Water, float player_x, float player_y, float player_z)
{
	vx = cos(rotation) * cos(pitch);
	vy = sin(pitch);
	vz = sin(rotation) * cos(pitch);

	//camera follows player
	if (GetState() == STATE_FPS) SetPos(player_x, player_y + 0.5, player_z);
	else
	{
		float newLambda = Terrain->GetSegmentIntersectionLambda(player_x, player_y, player_z, vx, vy, vz, CAMERA_MAX_DISTANCE);
		newLambda = min(newLambda, GetWaterLambda(player_y, player_y - CAMERA_MAX_DISTANCE*vy, Water->GetHeight()));

		if (lambda < newLambda)
		{
			lambda += CAMERA_SMOOTHING_SPEED;
			if (lambda > newLambda) lambda = newLambda;
		}
		else lambda = newLambda;

		distance = CAMERA_MAX_DISTANCE*lambda*0.85;

		SetPos(player_x - distance*vx, player_y - distance*vy, player_z - distance*vz);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(x, y, z, x + vx, y + vy, z + vz, 0, 1, 0);
}

void Camera::SetState(int s)
{
	state = s;
}
int  Camera::GetState()
{
	return state;
}

void Camera::SetPos(float posx, float posy, float posz)
{
	x = posx;
	y = posy;
	z = posz;

	Refresh();
}

float Camera::GetDistance()
{
	return distance;
}

void Camera::GetDirectionVector(float &x, float &y, float &z)
{
	x = vx;
	y = vy;
	z = vz;
}

void Camera::TurnRotation(float angle)
{
	rotation += angle;

	Refresh();
}
void Camera::SetRotation(float angle)
{
	rotation = angle;
}
float Camera::GetRotation()
{
	return rotation;
}

void Camera::SetLastRotation(float angle)
{
	last_Rotation = angle;
}
float Camera::GetLastRotation()
{
	return last_Rotation;
}

void Camera::RotatePitch(float angle)
{
	const float limit = 89.0 * PI / 180.0;

	pitch += angle;

	if (pitch < -limit)
		pitch = -limit;

	if (pitch > limit)
		pitch = limit;

	Refresh();
}
void Camera::SetPitch(float angle)
{
	pitch = angle;
}
float Camera::GetPitch()
{
	return pitch;
}

float Camera::GetX()
{
	return x;
}
float Camera::GetY()
{
	return y;
}
float Camera::GetZ()
{
	return z;
}