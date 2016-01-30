#ifndef __INCLUDES__
#define __INCLUDES__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tiny_obj_loader.h"
#include "Shape.h"
#include "GLSL.h"

using namespace std;

GLFWwindow* window;
vector<tinyobj::material_t> materials;
Shape *head;
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
