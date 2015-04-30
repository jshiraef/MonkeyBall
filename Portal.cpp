#include "Portal.h"

Portal::Portal() {}
Portal::~Portal() {}

void Portal::Draw(GameData *Data, bool activated, Model *Model)
{
	//vortex
	if (activated)
	{
		glPushMatrix();

		glTranslatef(x, y, z);
		ang = fmod(ang + PORTAL_SPEED, 360);
		glTranslatef(0, PORTAL_SIDE / 2, 0);
		glRotatef(ang, 0.0f, 0.0f, 1.0f);

		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_VORTEX));
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(PORTAL_SIDE / 2, -PORTAL_SIDE / 2, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-PORTAL_SIDE / 2, -PORTAL_SIDE / 2, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-PORTAL_SIDE / 2, PORTAL_SIDE / 2, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(PORTAL_SIDE / 2, PORTAL_SIDE / 2, 0);
		glEnd();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		glPopMatrix();
	}
	//f.vortex

	//portal
	glPushMatrix();

	glTranslatef(x, y, z);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_PORTAL));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Data->GetID(IMG_PORTAL_NMAP));
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);


	Model->Draw(MODEL_PORTAL);


	glEnable(GL_BLEND);
	GLUquadricObj *q = gluNewQuadric();

	glTranslatef(0, PORTAL_SIDE * 3 / 2, 0);
	glColor3f(0.0f, 1.0f, 0.0f); //green

	gluSphere(q, 0.2, 16, 16);


	glTranslatef(PORTAL_SIDE / 2, -PORTAL_SIDE / 2, 0);
	glColor3f(1.0f, 1.0f, 0.0f); //yellow

	gluSphere(q, 0.2, 16, 16);


	glTranslatef(-PORTAL_SIDE, 0, 0);
	glColor3f(0.2f, 0.2f, 1.0f); //blue

	gluSphere(q, 0.2, 16, 16);


	glTranslatef(0, -(PORTAL_SIDE - 1), 0);
	glColor3f(1.0f, 0.0f, 1.0f); //violet

	gluSphere(q, 0.2, 16, 16);


	glTranslatef(PORTAL_SIDE, 0, 0);
	glColor3f(1.0f, 0.0f, 0.0f); //red

	gluSphere(q, 0.2, 16, 16);


	gluDeleteQuadric(q);
	glDisable(GL_BLEND);

	glColor4f(1, 1, 1, 1);

	glPopMatrix();
	//f.portal
}

void Portal::SetPos(float posx, float posy, float posz)
{
	x = posx;
	y = posy;
	z = posz;

	//red
	receptors[0].first = x + PORTAL_SIDE / 2;
	receptors[0].second = y + 1.0f;
	//yellow
	receptors[1].first = x + PORTAL_SIDE / 2;
	receptors[1].second = y + PORTAL_SIDE;
	//green
	receptors[2].first = x;
	receptors[2].second = y + PORTAL_SIDE * 3 / 2;
	//blue
	receptors[3].first = x - PORTAL_SIDE / 2;
	receptors[3].second = y + PORTAL_SIDE;
	//violet
	receptors[4].first = x - PORTAL_SIDE / 2;
	receptors[4].second = y + 1.0f;
}

bool Portal::InsidePortal(float px, float py, float pz, float r)
{
	return ((px - r <= x + (PORTAL_SIDE / 2) && px + r >= x - (PORTAL_SIDE / 2)) && (py - r <= y + PORTAL_SIDE && py + r >= y));
}

float Portal::GetX()
{
	return x;
}
float Portal::GetY()
{
	return y;
}
float Portal::GetZ()
{
	return z;
}
float Portal::GetReceptorX(int i)
{
	return receptors[i].first;
}
float Portal::GetReceptorY(int i)
{
	return receptors[i].second;
}