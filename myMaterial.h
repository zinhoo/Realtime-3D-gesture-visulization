#include <math.h>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include "vector3d.h"

class myMaterial
{
public:
	GLfloat material_Ka[4];
	GLfloat material_Kd[4];
	GLfloat material_Ks[4];
	GLfloat material_Sh;
};