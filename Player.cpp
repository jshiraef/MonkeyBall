#include "Player.h"

Player::Player()
{
	fade = true;
}
Player::~Player() {}

void Player::Draw(GameData *Data, Camera *Camera, Water *Water)
{
	glPushMatrix();
	glTranslatef(GetX(), GetY(), GetZ());

	//rotational motion
	if (cos(GetRotation()*(PI / 180)) >= 0.0f) glRotatef(GetPitch(), cos(GetRotation()*(PI / 180)), 0.0, -sin(GetRotation()*(PI / 180)));
	else glRotatef(GetPitch(), -cos(GetRotation()*(PI / 180)), 0.0, sin(GetRotation()*(PI / 180)));

	if (Camera->GetState() != STATE_FPS)
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_PLAYER));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_PLAYER_NMAP));
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		glEnable(GL_BLEND);
		GLUquadricObj *q = gluNewQuadric();
		gluQuadricTexture(q, true);

	
		gluSphere(q, RADIUS, 16, 16);


		gluDeleteQuadric(q);
		glDisable(GL_BLEND);
	}

	glPopMatrix();
}

void Player::SetFade(bool b)
{
	fade = b;
}