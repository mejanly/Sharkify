#ifndef __Vars__
#define __Vars__

#include "GL/glew.h"
#include "tiny_obj_loader.h"

GLFWwindow* window;
vector<tinyobj::material_t> materials;
GLuint ShadeProg;

int g_width = 1024;
int g_height = 768;

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
