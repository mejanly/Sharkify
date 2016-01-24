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
#include "RenderingHelper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;

GLFWwindow* window;
vector<tinyobj::material_t> materials;
Shape *head;
GLuint ShadeProg;
RenderingHelper ModelTrans;

int g_width = 1024;
int g_height = 768;

glm::vec3 g_light1(-5, 8, 0);
glm::vec3 g_light2(5, 8, 0);

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
