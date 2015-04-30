#include "Model.h"

Model::Model() {}
Model::~Model() {}

int Model::GetDisplayList(char* path)
{
	struct Coord { float x, y, z; };

	std::vector<Coord> vertexs;
	std::vector< std::pair<float, float> > texcoords;
	std::vector<Coord> normals; //per vertex
	std::vector< std::vector< std::vector<int> > > faces;

	char c;
	FILE *fd = fopen(path, "r");

	do {

		fscanf(fd, "%c", &c);

		if (c == 'v')
		{
			Coord aux;
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.x);
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.y);
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.z);

			vertexs.push_back(aux);
			fscanf(fd, "%c", &c); //new line
		}
		if (c == 't')
		{
			std::pair<float, float> st;
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &st.first);
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &st.second);

			texcoords.push_back(st);
			fscanf(fd, "%c", &c); //new line
		}
		if (c == 'n')
		{
			Coord aux;
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.x);
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.y);
			fscanf(fd, "%c", &c); //space
			fscanf(fd, "%f", &aux.z);

			normals.push_back(aux);
			fscanf(fd, "%c", &c); //new line
		}
		if (c == 'f')
		{
			std::vector< std::vector<int> > points;
			std::vector<int> point(3);

			fscanf(fd, "%c", &c); //space

			do {
				fscanf(fd, "%d", &point[0]);
				fscanf(fd, "%c", &c); //separator
				fscanf(fd, "%d", &point[1]);
				fscanf(fd, "%c", &c); //separator
				fscanf(fd, "%d", &point[2]);

				points.push_back(point);
				fscanf(fd, "%c", &c); //space or newline
			} while (c == ' ');

			faces.push_back(points);
		}

	} while (!feof(fd));

	fclose(fd);

	int dl = glGenLists(1);
	glNewList(dl, GL_COMPILE);

	for (int i = 0; i<faces.size(); i++)
	{
		if (faces[i].size() == 3)
		{
			glBegin(GL_TRIANGLES);
			glTexCoord2f(texcoords[faces[i][0][1] - 1].first, texcoords[faces[i][0][1] - 1].second);
			glNormal3f(normals[faces[i][0][2] - 1].x, normals[faces[i][0][2] - 1].y, normals[faces[i][0][2] - 1].z);
			glVertex3f(vertexs[faces[i][0][0] - 1].x, vertexs[faces[i][0][0] - 1].y, vertexs[faces[i][0][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][1][1] - 1].first, texcoords[faces[i][1][1] - 1].second);
			glNormal3f(normals[faces[i][1][2] - 1].x, normals[faces[i][1][2] - 1].y, normals[faces[i][1][2] - 1].z);
			glVertex3f(vertexs[faces[i][1][0] - 1].x, vertexs[faces[i][1][0] - 1].y, vertexs[faces[i][1][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][2][1] - 1].first, texcoords[faces[i][2][1] - 1].second);
			glNormal3f(normals[faces[i][2][2] - 1].x, normals[faces[i][2][2] - 1].y, normals[faces[i][2][2] - 1].z);
			glVertex3f(vertexs[faces[i][2][0] - 1].x, vertexs[faces[i][2][0] - 1].y, vertexs[faces[i][2][0] - 1].z);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			glTexCoord2f(texcoords[faces[i][0][1] - 1].first, texcoords[faces[i][0][1] - 1].second);
			glNormal3f(normals[faces[i][0][2] - 1].x, normals[faces[i][0][2] - 1].y, normals[faces[i][0][2] - 1].z);
			glVertex3f(vertexs[faces[i][0][0] - 1].x, vertexs[faces[i][0][0] - 1].y, vertexs[faces[i][0][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][1][1] - 1].first, texcoords[faces[i][1][1] - 1].second);
			glNormal3f(normals[faces[i][1][2] - 1].x, normals[faces[i][1][2] - 1].y, normals[faces[i][1][2] - 1].z);
			glVertex3f(vertexs[faces[i][1][0] - 1].x, vertexs[faces[i][1][0] - 1].y, vertexs[faces[i][1][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][2][1] - 1].first, texcoords[faces[i][2][1] - 1].second);
			glNormal3f(normals[faces[i][2][2] - 1].x, normals[faces[i][2][2] - 1].y, normals[faces[i][2][2] - 1].z);
			glVertex3f(vertexs[faces[i][2][0] - 1].x, vertexs[faces[i][2][0] - 1].y, vertexs[faces[i][2][0] - 1].z);

			glTexCoord2f(texcoords[faces[i][3][1] - 1].first, texcoords[faces[i][3][1] - 1].second);
			glNormal3f(normals[faces[i][3][2] - 1].x, normals[faces[i][3][2] - 1].y, normals[faces[i][3][2] - 1].z);
			glVertex3f(vertexs[faces[i][3][0] - 1].x, vertexs[faces[i][3][0] - 1].y, vertexs[faces[i][3][0] - 1].z);
			glEnd();
		}
	}

	glEndList();

	return dl;
}



void Model::Load()
{
	models[MODEL_KEY] = GetDisplayList("Models/key.obj");
	models[MODEL_PORTAL] = GetDisplayList("Models/portal.obj");
	models[MODEL_LANDMARK] = GetDisplayList("Models/landmark.obj");
}

void Model::Draw(int model_id)
{
	glCallList(models[model_id]);
}