#include "CheckPoint.h"

CheckPoint::CheckPoint() {}
CheckPoint::~CheckPoint() {}


void CheckPoint::Draw(int tex_id, bool activated)
{
	//GLfloat ViewMatrix[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, ViewMatrix); //transformation of last gluLookAt

	glPushMatrix();

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glTranslatef(x, y + HEIGHT_OFFSET, z);

	//circle
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(CIRCLE_RADIUS, 0, -CIRCLE_RADIUS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-CIRCLE_RADIUS, 0, -CIRCLE_RADIUS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-CIRCLE_RADIUS, 0, CIRCLE_RADIUS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(CIRCLE_RADIUS, 0, CIRCLE_RADIUS);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//f.circle

	//aura
	glDepthMask(GL_FALSE); //disables writing to the zbuffer
	glDisable(GL_CULL_FACE);
	GLUquadricObj *q = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	if (activated) glColor4f(1.0f, 0.4f, 0.0f, 0.6f);
	else glColor4f(0.5f, 0.5f, 1.0f, 0.6f);


	gluCylinder(q, CIRCLE_RADIUS, CIRCLE_RADIUS, AURA_HEIGHT, 16, 16);


	glTranslatef(0, 0, HEIGHT_OFFSET);
	gluDisk(q, 0, CIRCLE_RADIUS, 16, 16);

	glColor4f(1, 1, 1, 1);
	gluDeleteQuadric(q);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	//f.aura

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void CheckPoint::SetPos(float posx, float posy, float posz)
{
	x = posx;
	y = posy;
	z = posz;
}
float CheckPoint::GetX()
{
	return x;
}
float CheckPoint::GetY()
{
	return y;
}
float CheckPoint::GetZ()
{
	return z;
}