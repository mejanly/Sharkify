#ifndef __INCLUDES__
#define __INCLUDES__

#include "GL/glew.h"
#include <vector>
#include "tiny_obj_loader.h"

using namespace std;

vector<tinyobj::material_t> materials;
GLuint ShadeProg;

GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos1;
GLint h_uLightPos2;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uLightInts;


#endif
