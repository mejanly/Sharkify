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
#include "Program.h"
#include "FontEngine.h"
#include "Texture.h"

#define SHADER_DEFAULT "default"
#define SHADER_TEXT "text"
#define SHADER_BILLBOARD "billboard"
#define DEFAULT_VERT_SHADER "shaders/vert.glsl"
#define DEFAULT_FRAG_SHADER "shaders/frag.glsl"
#define TEXT_VERT_SHADER "shaders/text.v.glsl"
#define TEXT_FRAG_SHADER "shaders/text.f.glsl"
#define BILLBOARD_VERT_SHADER "shaders/billboard_vert.glsl"
#define BILLBOARD_FRAG_SHADER "shaders/billboard_frag.glsl"

using namespace std;

GLFWwindow* window;
vector<tinyobj::material_t> materials;
GLuint ShadeProg;
RenderingHelper ModelTrans;

int g_width = 1024;
int g_height = 768;

glm::vec3 g_light1(-5, 8, 0);
glm::vec3 g_light2(5, 8, 0);

// Shark pieces
Shape *uHead;
Shape *lHead;
Shape *fBody;
Shape *mBody;
Shape *rBody;
Shape *lsFin;
Shape *rsFin;
Shape *tbFin;
Shape *btFin;
Shape *bbFin;

// An angle of rotation for each piece
float hdAngle = 0;
float fbAngle = 0;
float mbAngle = 0;
float rbAngle = 0;
float tfAngle = 0;

// Whole shark rotation variables
float SharkRotX = 0.0;
float SharkRotY = 0.0;

// Determines the speed of the rotation
float theta = 0;
float thetaAdd = .05;

// Shader variables
GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos1;
GLint h_uLightPos2;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uLightInts;

map<string, Program*> shaders;

FontEngine *fontEngine;

int texture_id;
float roughness; // 0 : smooth, 1: rough
float fresnel; // fresnel reflectance at normal incidence
float geometric; // fraction of diffuse reflection (specular reflection = 1 - k)

#endif
