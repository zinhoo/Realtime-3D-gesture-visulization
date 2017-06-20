#pragma once
#include <GL/glew.h>
#include "StdAfx.h"
class myTexture
{
public:
	int width, height, pixelsize;
	GLuint texName;

	bool readTexture(char *filename);
};
