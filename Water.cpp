#include "Water.h"

Water::Water()
{
	height = (WATER_HEIGHT_MAX + WATER_HEIGHT_MIN) / 2;
	ang = 0.0f;
	up = true;
}
Water::~Water() {}

void Water::Load(float terrain_size)
{
	water_id = glGenLists(1);
	glNewList(water_id, GL_COMPILE);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f * 16); glVertex3f(terrain_size, 0.0f, 0.0f);
	glTexCoord2f(1.0f * 16, 1.0f * 16); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f * 16, 0.0f); glVertex3f(0.0f, 0.0f, terrain_size);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(terrain_size, 0.0f, terrain_size);
	glEnd();

	glEndList();
}


void Water::Draw(int tex_id)
{
	glPushMatrix();
	glTranslatef(0.0f, height, 0.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glCallList(water_id);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

float Water::GetHeight()
{
	return height;
}

float Water::GetHeightMax()
{
	return WATER_HEIGHT_MAX;
}

void Water::Update()
{
	if (up)
	{
		ang = fmod(ang + FLOW_SPEED, 360);
		height = ((WATER_HEIGHT_MAX - WATER_HEIGHT_MIN) / 2.0f)*sin(ang*(PI / 180)) + (WATER_HEIGHT_MAX + WATER_HEIGHT_MIN) / 2.0f;
		up = (height < WATER_HEIGHT_MAX);
	}
	else
	{
		ang = fmod(ang - FLOW_SPEED, 360);
		height = ((WATER_HEIGHT_MAX - WATER_HEIGHT_MIN) / 2.0f)*sin(ang*(PI / 180)) + (WATER_HEIGHT_MAX + WATER_HEIGHT_MIN) / 2.0f;
		up = (height <= WATER_HEIGHT_MIN);
	}
}