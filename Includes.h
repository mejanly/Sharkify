#ifndef __INCLUDES__
#define __INCLUDES__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tiny_obj_loader.h"
#include "GLSL.h"
#include "RenderingHelper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "Program.h"
#include "FontEngine.h"
#include "Sound.h"
#include "Shark.h"

#define NUM_SHARKS 6
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
GLuint ShadeProg;
RenderingHelper ModelTrans;

int g_width = 1024;
int g_height = 768;

glm::vec3 g_light1(-5, 8, 0);
glm::vec3 g_light2(5, 8, 0);

vector<Shark *> sharks;

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

// Different shaders (text, default, billboard for texture)
map<string, Program*> shaders;

// The font and sound objects
FontEngine *fontEngine;
Sound *sound;

// Texture for the background
GLuint texture;

// TODO These should be moved...
float happy;
float sad;
float decrement = .008;
float sAlpha;

#endif
