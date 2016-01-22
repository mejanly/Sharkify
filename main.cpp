#define GLM_FORCE_RADIANS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <time.h>
#include "RenderingHelper.h"

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> sHead;
vector<tinyobj::shape_t> sJaw;
vector<tinyobj::shape_t> sLeftFin;
vector<tinyobj::shape_t> sRightFin;
vector<tinyobj::shape_t> sFrontBod;
vector<tinyobj::shape_t> sBackBod;
vector<tinyobj::shape_t> sTailFront;
vector<tinyobj::shape_t> sTailBack;
vector<tinyobj::shape_t> sTail;
//vector<tinyobj::shape_t> sphere;
vector<tinyobj::material_t> materials;

int g_SM = 1;
int g_width;
int g_height;
glm::vec3 g_light1(-5, 8, 0);
glm::vec3 g_light2(5, 8, 0);

GLuint ShadeProg;

GLuint posBufObjH = 0;
GLuint norBufObjH = 0;
GLuint indBufObjH = 0;

GLuint posBufObjJ = 0;
GLuint norBufObjJ = 0;
GLuint indBufObjJ = 0;

GLuint posBufObjLF = 0;
GLuint norBufObjLF = 0;
GLuint indBufObjLF = 0;

GLuint posBufObjRF = 0;
GLuint norBufObjRF = 0;
GLuint indBufObjRF = 0;

GLuint posBufObjFB = 0;
GLuint norBufObjFB = 0;
GLuint indBufObjFB = 0;

GLuint posBufObjBB = 0;
GLuint norBufObjBB = 0;
GLuint indBufObjBB = 0;

GLuint posBufObjTF = 0;
GLuint norBufObjTF = 0;
GLuint indBufObjTF = 0;

GLuint posBufObjTB = 0;
GLuint norBufObjTB = 0;
GLuint indBufObjTB = 0;

GLuint posBufObjT = 0;
GLuint norBufObjT = 0;
GLuint indBufObjT = 0;

GLuint posBufObjS = 0;
GLuint norBufObjS = 0;
GLuint indBufObjS = 0;

GLuint posBufObjW = 0;
GLuint norBufObjW = 0;

//Handles to the shader data
GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos1;
GLint h_uLightPos2;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uLightInts;

float g_movey = 0;
float sharkRot = 90;
float sharkRotY = 0;
float sharkRotZ = 0;
float sharkRotYAdd = 0;
float sharkRotZAdd = 0;
float sharkZoom = -5;

float theta = 0;
float thetaAdd = .01;

float sAngle = 0;
float hAngle = 0;
float jAngle = 0;
float lfAngle = 0;
float rfAngle = 0;
float fbAngle = 0;
float bbAngle = 0;
float tfAngle = 0;
float tbAngle = 0;
float tAngle = 0;

float intensity = 0.8;

glm::vec3 wallCol(0, 0.1, 0.5);

RenderingHelper ModelTrans;

int printOglError (const char *file, int line) {
   /* Returns 1 if an OpenGL error occurred, 0 otherwise. */
   GLenum glErr;
   int    retCode = 0;
  
   glErr = glGetError();
   while (glErr != GL_NO_ERROR)
    {
      //printf ("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
      retCode = 1;
      glErr = glGetError ();
    }
   return retCode;
}

inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void SetMaterial(int i) {
  glUseProgram(ShadeProg);

  switch (i) {
    case 0: //grey - shark
		glUniform3f(h_uMatAmb, 0.08, 0.03, 0.1);
		glUniform3f(h_uMatDif, 0.55, 0.51, 0.51);
        glUniform3f(h_uMatSpec, 1.0, 0.9, 0.6);
        glUniform1f(h_uMatShine, 2.0);
		glUniform1f(h_uLightInts, intensity*.6);
		break;
    case 1: //blue - wall
		glUniform3f(h_uMatAmb, 0.06, 0.06, 0.02);
        glUniform3f(h_uMatDif, wallCol.r, wallCol.g, wallCol.b);
        glUniform3f(h_uMatSpec, 1.0, 0.92, 0.7);
        glUniform1f(h_uMatShine, 4.0);
		glUniform1f(h_uLightInts, intensity);
		break;
  }
}

/* projection matrix */
void SetProjectionMatrix() {
  glm::mat4 Projection = glm::perspective(50.0f, (float)g_width/g_height, 0.1f, 100.f);
  safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

void SetWall(vec3 trans, float sc) {
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.translate(trans);
	ModelTrans.scale(sc);
	safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
}

void drawSharkPiece(GLuint posBufObj, GLuint norBufObj, GLuint indBufObj, vector<tinyobj::shape_t> piece) {
	safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));

	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(h_aPosition);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
	glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array for drawing
	GLSL::enableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
	glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	int sIndices = (int)piece[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);

	//draw the head	
	glDrawElements(GL_TRIANGLES, sIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_aPosition);
	GLSL::disableVertexAttribArray(h_aNormal);   
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawModelFront() {
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(sAngle, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(1.05, 0, 0));
	ModelTrans.rotate(sharkRotZ, glm::vec3(0, 0, 1));
	ModelTrans.rotate(fbAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.5);
	drawSharkPiece(posBufObjFB, norBufObjFB, indBufObjFB, sFrontBod);
	ModelTrans.popMatrix();

   /*ModelTrans.popMatrix();
	ModelTrans.rotate(sharkRotZ, glm::vec3(0, 0, 1));
	ModelTrans.translate(glm::vec3(0, -.43, -.65));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjLF, norBufObjLF, indBufObjLF, sLeftFin);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(0, 0, 1.3));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjRF, norBufObjRF, indBufObjRF, sRightFin);
	
	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(0, .43, -.65));
	ModelTrans.rotate(-sharkRotZ, glm::vec3(0, 0, 1));
	ModelTrans.rotate(hAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(.95, -.03, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.7);
	drawSharkPiece(posBufObjH, norBufObjH, indBufObjH, sHead);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(.05, -.33, 0));
	ModelTrans.rotate(jAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.3);
	drawSharkPiece(posBufObjJ, norBufObjJ, indBufObjJ, sJaw);*/
}

void DrawModelBack() {
	/*ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-1.63, .66, 0));
	ModelTrans.rotate(bbAngle, glm::vec3(0, 1, 0));	
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.8);
	drawSharkPiece(posBufObjBB, norBufObjBB, indBufObjBB, sBackBod);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.45, -.3, 0));
	ModelTrans.rotate(tfAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.4, -.04, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.60);
	drawSharkPiece(posBufObjTF, norBufObjTF, indBufObjTF, sTailFront);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.475, .03, 0));
	ModelTrans.rotate(tbAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.475, 0, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.39);
	drawSharkPiece(posBufObjTB, norBufObjTB, indBufObjTB, sTailBack);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.4, 0, 0));
	ModelTrans.rotate(tAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.4, -.09, 0));
	ModelTrans.rotate(sharkRot, glm::vec3(0, 1, 0));
	ModelTrans.scale(.75);
	drawSharkPiece(posBufObjT, norBufObjT, indBufObjT, sTail);*/
}

//Given a vector of shapes which has already been read from an obj file
// resize all vertices to the range [-1, 1]
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
     maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
     maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
     maxExtent = zExtent;
   }
    scaleX = 2.0 /maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;

    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
            assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
            assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
            shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
            assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
            assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
        }
    }
}

void loadShapes(const string &objFile, std::vector<tinyobj::shape_t>& shapes)
{
	string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
	if(!err.empty()) {
		cerr << err << endl;
	}
   resize_obj(shapes);
}

void initObj(std::vector<tinyobj::shape_t>& shape, GLuint *posBufObj, GLuint *norBufObj, GLuint *indBufObj) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, posBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, *posBufObj);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, norBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, *norBufObj);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, indBufObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indBufObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initWall() {
  float W_edge = 100;
  GLfloat w_backgnd_data[] = {
      -W_edge, -W_edge, -10.0f,
      -W_edge,  W_edge, -10.0f,
      W_edge, -W_edge, -10.0f,
      -W_edge,  W_edge, -10.0f,
      W_edge, W_edge, -10.0f,
      W_edge, -W_edge, -10.0f,
  };


  GLfloat nor_Buf_W[] = { 
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
  };

   glGenBuffers(1, &posBufObjW);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjW);
   glBufferData(GL_ARRAY_BUFFER, sizeof(w_backgnd_data), w_backgnd_data, GL_STATIC_DRAW);
	
   glGenBuffers(1, &norBufObjW);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjW);
   glBufferData(GL_ARRAY_BUFFER, sizeof(nor_Buf_W), nor_Buf_W, GL_STATIC_DRAW);
}

void initGL()
{
	// Set the background color
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	// Enable Z-buffer test
	glEnable(GL_DEPTH_TEST);
    glPointSize(18);

	initObj(sHead, &posBufObjH, &norBufObjH, &indBufObjH);
	initObj(sJaw, &posBufObjJ, &norBufObjJ, &indBufObjJ);
	initObj(sLeftFin, &posBufObjLF, &norBufObjLF, &indBufObjLF);
	initObj(sRightFin, &posBufObjRF, &norBufObjRF, &indBufObjRF);
	initObj(sFrontBod, &posBufObjFB, &norBufObjFB, &indBufObjFB);
	initObj(sBackBod, &posBufObjBB, &norBufObjBB, &indBufObjBB);
	initObj(sTailFront, &posBufObjTF, &norBufObjTF, &indBufObjTF);
	initObj(sTailBack, &posBufObjTB, &norBufObjTB, &indBufObjTB);
	initObj(sTail, &posBufObjT, &norBufObjT, &indBufObjT);
	//initObj(sphere, &posBufObjS, &norBufObjS, &indBufObjS);

    initWall();
}

bool installShaders(const string &vShaderName, const string &fShaderName)
{
	GLint rc;
	
	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
 	glCompileShader(VS);
    std::cout << "just compiled the v shader" << std::endl;
    printOglError(__FILE__, __LINE__);
	GLSL::printError();
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
		return false;
	}
	
	// Compile fragment shader
	glCompileShader(FS);
    std::cout << "just compiled the f shader" << std::endl;
	GLSL::printError();
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
		return false;
	}
	
	// Create the program and link
	ShadeProg = glCreateProgram();
	glAttachShader(ShadeProg, VS);
	glAttachShader(ShadeProg, FS);
	glLinkProgram(ShadeProg);
    std::cout << "just linked the shaders" << std::endl;
   
	GLSL::printError();
	glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(ShadeProg);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
		return false;
	}

    /* get handles to attribute data */
    h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPosition");
    h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNormal");
    h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "uProjMatrix");
    h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "uViewMatrix");
    h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "uModelMatrix");
    h_uLightPos1 = GLSL::getUniformLocation(ShadeProg, "uLightPos1");
    h_uLightPos2 = GLSL::getUniformLocation(ShadeProg, "uLightPos2");
	h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
    h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
    h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
    h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
	h_uLightInts = GLSL::getUniformLocation(ShadeProg, "Uints");
	
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

void drawGL()
{
	int i;
		
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Use our GLSL program
	glUseProgram(ShadeProg);
    glUniform3f(h_uLightPos1, g_light1.x, g_light1.y, g_light1.z);
	glUniform3f(h_uLightPos2, g_light2.x, g_light2.y, g_light2.z);

    SetProjectionMatrix();
	ModelTrans.useModelViewMatrix();

	//draw shark
	SetMaterial(0);
	
	theta += thetaAdd;

	//move shark up or down
	if (sharkRotZ > 0 || (sharkRotZAdd > 0 && sharkRotZ < 10)) {
		sharkRotZ = sin(sharkRotZAdd)*(10+thetaAdd*30);	
		sharkRotZAdd += .002;
	}
	if (sharkRotZ < 0 || (sharkRotZAdd < 0 && sharkRotZ > -10)) {
		sharkRotZ = sin(sharkRotZAdd)*(10+thetaAdd*30);	
		sharkRotZAdd -= .002;
	}

	//move shark left or right
	if (sharkRotY > 0 || (sharkRotYAdd > 0 && sharkRotY < 15)) {
		sharkRotY = sin(sharkRotYAdd)*(15+thetaAdd*45);	
		sharkRotYAdd += .002;
	}
	if (sharkRotY < 0 || (sharkRotYAdd < 0 && sharkRotY > -15)) {
		sharkRotY = sin(sharkRotYAdd)*(15+thetaAdd*45);	
		sharkRotYAdd -= .002;
	}

	sAngle = cos(theta)*.95;

	hAngle = -cos(theta)*.2 - (sharkRotY / 7);
	jAngle = -cos(theta)*.2 - (sharkRotY / 7);
	lfAngle = -cos(theta)*.9 - (sharkRotY / 5);
	rfAngle = -cos(theta)*.9 - (sharkRotY / 5);
	fbAngle = -cos(theta)*.9 - (sharkRotY / 5);

	bbAngle = cos(theta)*5 + (sharkRotY / 5);
	tfAngle = cos(theta)*10 + (sharkRotY / 3);
	tbAngle = cos(theta)*15 + sharkRotY;
	tAngle = cos(theta)*20 + sharkRotY;

	DrawModelFront();
	DrawModelBack();

    SetMaterial(1);
    SetWall(glm::vec3(0, 0, 0), 1);
   
    //draw the wall
    glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, posBufObjW);
    glVertexAttribPointer(h_aPosition, 3,  GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLSL::enableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjW);
	glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

    GLSL::disableVertexAttribArray(h_aPosition);
	GLSL::disableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0); 
	//assert(glGetError() == GL_NO_ERROR);
}

void window_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
      g_movey += 2;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      g_movey -= 2;
   if (key == GLFW_KEY_N && action == GLFW_PRESS)
	  sharkZoom += .1;
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	  sharkZoom -= .1;
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	  thetaAdd += .001;
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		if (thetaAdd - .001 >= .005) {
			thetaAdd -= .001;
		}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		if (wallCol.b <= 1.0) {
			wallCol += .01;
		}
		if (intensity + .005 <= .8) {
			intensity += .005;
		}
		if (g_light1.y - .5 >= 2) {
			g_light1.y -= .5;
			g_light2.y -= .5;
		}
		sharkRotZAdd = .002;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		wallCol -= .01;
		if (intensity - .005 >= .005) {
			intensity -= .005;
		}
		g_light1.y += .5;
		g_light2.y += .5;
		sharkRotZAdd = -.002;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) 
		sharkRotYAdd = -.002;
	if (key == GLFW_KEY_L && action == GLFW_PRESS) 
		sharkRotYAdd = .002;
}

int main(int argc, char **argv)
{
	srand (time(NULL));

   // Initialise GLFW
   if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

   // Open a window and create its OpenGL context
   g_width = 1024;
   g_height = 768;
   window = glfwCreateWindow( g_width, g_height, "Sharkify!!", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);	
    glfwSetWindowSizeCallback(window, window_size_callback);
	// Initialize glew
    if (glewInit() != GLEW_OK) {
       fprintf(stderr, "Failed to initialize glew\n");
       return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadShapes("sharkobj2/head.obj", sHead);
	loadShapes("sharkobj2/jaw_bottom.obj", sJaw);
	loadShapes("sharkobj2/fin_left.obj", sLeftFin);
	loadShapes("sharkobj2/fin_right.obj", sRightFin);
    loadShapes("sharkobj2/body_front.obj", sFrontBod);
	loadShapes("sharkobj2/body_back.obj", sBackBod);
	loadShapes("sharkobj2/tail_front.obj", sTailFront);
	loadShapes("sharkobj2/tail_back.obj", sTailBack);
	loadShapes("sharkobj2/tail_fin.obj", sTail);
	//loadShapes("sphere.obj", sphere);
	initGL();
	installShaders("vert.glsl", "frag.glsl");

	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);

    do{
	    drawGL();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

	return 0;
}
