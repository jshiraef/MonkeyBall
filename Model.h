#pragma once

#include "Globals.h"

//Model array size
#define NUM_MODELS 3

//Model identifiers
enum {
	MODEL_KEY,
	MODEL_PORTAL,
	MODEL_LANDMARK
};

class Model
{
public:
	Model();
	~Model();
	void Load();
	void Draw(int model_id);

private:
	int models[NUM_MODELS]; //within the DisplayList id's of each model

	int GetDisplayList(char* path);
};
