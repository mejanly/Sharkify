#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
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

struct Particle{
    glm::vec3 pos, speed;
    float size, angle, weight;
    float life;
 
};

const int MaxParticles = 400000;
const int ParticleCount = 400;

Particle ParticlesContainer[MaxParticles];

GLFWwindow* window;
using namespace std;
using namespace glm;

vector<tinyobj::shape_t> sHead;
vector<tinyobj::shape_t> sJaw;
vector<tinyobj::shape_t> sLeftFin;
vector<tinyobj::shape_t> sRightFin;
vector<tinyobj::shape_t> sFrontBod;
vector<tinyobj::shape_t> sBackBod;
vector<tinyobj::shape_t> sTailFront;
vector<tinyobj::shape_t> sTailBack;
vector<tinyobj::shape_t> sTail;
vector<tinyobj::shape_t> sphere;
vector<tinyobj::material_t> materials;

int g_SM = 1;
int g_width;
int g_height;
float g_Camtrans = -2.5;
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

float g_movex = 0;
float g_movey = 0;
float sPositions[1200];
float sharkRotY = 90;
float sharkZoom = -5;

float theta = 0;
float thetaAdd = .005;

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

float intensity = 0.5;

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
    case 1: //white - bubbles
		glUniform3f(h_uMatAmb, 0.09, 0.07, 0.08);
        glUniform3f(h_uMatDif, 0.91, 0.782, 0.82);
        glUniform3f(h_uMatSpec, 1.0, 0.913, 0.8);
        glUniform1f(h_uMatShine, 200.0);
		glUniform1f(h_uLightInts, intensity);
        break;
	case 2: //blue - wall
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

/* model transforms */
void SetModel(vec3 trans, float rot, float sc) {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), trans);
  glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), rot, glm::vec3(0.0f, 1, 0));
  glm::mat4 Sc = glm::scale( glm::mat4(1.0f), vec3(sc));
  glm::mat4 com = Trans*RotateY*Sc;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(com));
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
	ModelTrans.rotate(g_movex+sAngle, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(1.05, 0, 0));
	ModelTrans.rotate(fbAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.5);
	drawSharkPiece(posBufObjFB, norBufObjFB, indBufObjFB, sFrontBod);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(0, -.43, -.65));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjLF, norBufObjLF, indBufObjLF, sLeftFin);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(0, 0, 1.3));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjRF, norBufObjRF, indBufObjRF, sRightFin);
	
	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(0, .43, -.65));
	ModelTrans.rotate(hAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(.95, -.03, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.7);
	drawSharkPiece(posBufObjH, norBufObjH, indBufObjH, sHead);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(.05, -.33, 0));
	ModelTrans.rotate(jAngle, glm::vec3(0, 1, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.3);
	drawSharkPiece(posBufObjJ, norBufObjJ, indBufObjJ, sJaw);
}

void DrawModelBack() {
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex+sAngle, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(.45, .3, 0));
	ModelTrans.rotate(bbAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.8);
	drawSharkPiece(posBufObjBB, norBufObjBB, indBufObjBB, sBackBod);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.45, -.3, 0));
	ModelTrans.rotate(tfAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.4, -.04, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.60);
	drawSharkPiece(posBufObjTF, norBufObjTF, indBufObjTF, sTailFront);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.475, .03, 0));
	ModelTrans.rotate(tbAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.475, 0, 0));
	ModelTrans.pushMatrix();
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.39);
	drawSharkPiece(posBufObjTB, norBufObjTB, indBufObjTB, sTailBack);

	ModelTrans.popMatrix();
	ModelTrans.translate(glm::vec3(-.4, 0, 0));
	ModelTrans.rotate(tAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(-.4, -.09, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.75);
	drawSharkPiece(posBufObjT, norBufObjT, indBufObjT, sTail);
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

void initSphere(std::vector<tinyobj::shape_t>& shape) {
	// Send the position array to the GPU
	const vector<float> &posBufS = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjS);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjS);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles*posBufS.size()*sizeof(float), NULL, GL_STATIC_DRAW);

	const vector<float> &norBufS = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjS);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjS);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles*norBufS.size()*sizeof(float), NULL, GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBufS = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjS);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjS);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBufS.size()*sizeof(unsigned int), &indBufS[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSHead(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjH);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjH);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjH);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjH);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjH);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjH);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSJaw(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjJ);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjJ);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjJ);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjJ);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjJ);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjJ);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSLeftFin(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjLF);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjLF);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjLF);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjLF);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjLF);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjLF);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSRightFin(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjRF);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjRF);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjRF);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjRF);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjRF);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjRF);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSFrontBod(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjFB);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjFB);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjFB);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjFB);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjFB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjFB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSBackBod(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjBB);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjBB);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjBB);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjBB);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjBB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjBB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSTailFront(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjTF);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjTF);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjTF);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjTF);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjTF);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjTF);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSTailBack(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjTB);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjTB);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjTB);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjTB);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjTB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjTB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

void initSTail(std::vector<tinyobj::shape_t>& shape) {

	// Send the position array to the GPU
	const vector<float> &posBuf = shape[0].mesh.positions;
	glGenBuffers(1, &posBufObjT);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjT);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	const vector<float> &norBuf = shape[0].mesh.normals;
	glGenBuffers(1, &norBufObjT);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjT);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = shape[0].mesh.indices;
	glGenBuffers(1, &indBufObjT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjT);
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

	initSHead(sHead);
	initSJaw(sJaw);
	initSLeftFin(sLeftFin);
	initSRightFin(sRightFin);
    initSFrontBod(sFrontBod);
	initSBackBod(sBackBod);
	initSTailFront(sTailFront);
	initSTailBack(sTailBack);
	initSTail(sTail);
	initSphere(sphere);

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

	SetMaterial(0);

	theta += thetaAdd;

	sAngle = cos(theta)*.95;

	hAngle = -cos(theta)*.2;
	jAngle = -cos(theta)*.2;
	lfAngle = -cos(theta)*.9;
	rfAngle = -cos(theta)*.9;
	fbAngle = -cos(theta)*.9;

	bbAngle = cos(theta)*5;
	tfAngle = cos(theta)*10;
	tbAngle = cos(theta)*15;
	tAngle = cos(theta)*20;

	DrawModelFront();
	DrawModelBack();

    SetMaterial(2);
    SetModel(glm::vec3(0, 0, 0), 0, 1);
   
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

	glUseProgram(0); 
	assert(glGetError() == GL_NO_ERROR);
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
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
      g_movex += 2;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
      g_movex -= 2;
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
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
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
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
		wallCol -= .01;
		if (intensity - .005 >= .005) {
			intensity -= .005;
		}
		g_light1.y += .5;
		g_light2.y += .5;
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
   window = glfwCreateWindow( g_width, g_height, "shark swimming", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, cursor_callback);	
    glfwSetWindowSizeCallback(window, window_size_callback);
	// Initialize glad
    if (!gladLoadGL()) {
       fprintf(stderr, "Failed to initialize glad\n");
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
	loadShapes("sphere.obj", sphere);
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



/*
void SetView() {
	x = cos(phi)*cos(theta);
	y = sin(phi);
	z = cos(phi)*cos(90.0 - theta);

	//doesnt work
	speed = glm::vec3(g_movex, 0, g_movez);
	lookat = glm::vec3(x, y, z);
	
	view = lookat - eye;
	glm::vec3 viewn = normalize(view);

	lookat += viewn * speed;
	eye += viewn * speed;
	//

	glm::mat4 Cam = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(x, y, z), glm::vec3(0, 1, 0));
    safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(Cam));
}

void DrawModelFront() {
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(2, -.03, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.7);
	drawSharkPiece(posBufObjH, norBufObjH, indBufObjH, sHead);
	
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(2.05, -.36, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.3);
	drawSharkPiece(posBufObjJ, norBufObjJ, indBufObjJ, sJaw);

	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(1.0, -.43, -.65));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjLF, norBufObjLF, indBufObjLF, sLeftFin);

	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(1.0, -.43, .65));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.6);
	drawSharkPiece(posBufObjRF, norBufObjRF, indBufObjRF, sRightFin);
	
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(1.05, 0, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.5);
	drawSharkPiece(posBufObjFB, norBufObjFB, indBufObjFB, sFrontBod);
}

void DrawModelBack() {
	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(.45, .3, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.8);
	drawSharkPiece(posBufObjBB, norBufObjBB, indBufObjBB, sBackBod);

	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(-.45, -.04, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.60);
	drawSharkPiece(posBufObjTF, norBufObjTF, indBufObjTF, sTailFront);

	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(-1.4, -.01, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.36);
	drawSharkPiece(posBufObjTB, norBufObjTB, indBufObjTB, sTailBack);

	ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, sharkZoom));
	ModelTrans.rotate(g_movey, glm::vec3(0, 1, 0));
	ModelTrans.rotate(g_movex, glm::vec3(1, 0, 0));
	ModelTrans.translate(glm::vec3(-2.15, -.09, 0));
	ModelTrans.rotate(sharkRotY, glm::vec3(0, 1, 0));
	ModelTrans.scale(.75);
	drawSharkPiece(posBufObjT, norBufObjT, indBufObjT, sTail);
}
*/

	//BUBBLES

	/*
	SetMaterial(1);

	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(h_aPosition);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObjS);
	glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array for drawing
	GLSL::enableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObjS);
	glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	int sIndices = (int)sphere[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjS);
	
	glVertexAttribDivisor(h_aNormal, 1);
	glVertexAttribDivisor(h_aPosition, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, sIndices, ParticleCount);
	*/

	/*for (i = 0; i < 400; i++) {
		// Enable and bind position array for drawing
		GLSL::enableVertexAttribArray(h_aPosition);
		glBindBuffer(GL_ARRAY_BUFFER, posBufObjS);
		glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
		// Enable and bind normal array for drawing
		GLSL::enableVertexAttribArray(h_aNormal);
		glBindBuffer(GL_ARRAY_BUFFER, norBufObjS);
		glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
		// Bind index array for drawing
		int sIndices = (int)sphere[0].mesh.indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjS);

		//draw the bubbles
		SetMaterial(1);

		SetModel(glm::vec3(g_movex+sPositions[3*i+0], sPositions[3*i+1], g_movez+sPositions[3*i+2]), 0, .05);      
		glDrawElements(GL_TRIANGLES, sIndices, GL_UNSIGNED_INT, 0);
	}

    GLSL::disableVertexAttribArray(h_aPosition);
	GLSL::disableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/


/*
void randomize() {
	//float r = (float)((rand() % 6) + 1) / 100.0;

	int i;
    int mod = 20;

	for (i = 0; i < 50; i++) {
		sPositions[3*i+0] = rand() % mod;
		sPositions[3*i+1] = rand() % mod;
		sPositions[3*i+2] = rand() % mod;
	}
	for (i = 50; i < 100; i++) {
		sPositions[3*i+0] = -rand() % mod;
		sPositions[3*i+1] = -rand() % mod;
		sPositions[3*i+2] = -rand() % mod;
	}
	for (i = 100; i < 150; i++) {
		sPositions[3*i+0] = rand() % mod;
		sPositions[3*i+1] = -rand() % mod;
		sPositions[3*i+2] = -rand() % mod;
	}
	for (i = 150; i < 200; i++) {
		sPositions[3*i+0] = -rand() % mod;
		sPositions[3*i+1] = -rand() % mod;
		sPositions[3*i+2] = rand() % mod;
	}
	//problem
    for (i = 200; i < 250; i++) {
		sPositions[3*i+0] = rand() % mod;
		sPositions[3*i+1] = -rand() % mod;
		sPositions[3*i+2] = rand() % mod;
	}
	//problem
	for (i = 250; i < 300; i++) {
		sPositions[3*i+0] = -rand() % mod;
		sPositions[3*i+1] = rand() % mod;
		sPositions[3*i+2] = -rand() % mod;
		printf("%lf %lf %lf\n", sPositions[3*i+0], sPositions[3*i+1], sPositions[3*i+2]);
	}
	for (i = 200; i < 350; i++) {
		sPositions[3*i+0] = rand() % mod;
		sPositions[3*i+1] = rand() % mod;
		sPositions[3*i+2] = -rand() % mod;
	}
	for (i = 350; i < 400; i++) {
		sPositions[3*i+0] = -rand() % mod;
		sPositions[3*i+1] = rand() % mod;
		sPositions[3*i+2] = rand() % mod;
	}
}
*/

/*

void cursor_callback (GLFWwindow *window, double xPos, double yPos) {
	float xCenter = g_width / 2;
	float yCenter = g_height / 2;
	
	theta = xCenter - xPos / (xCenter / 4);
	phi = yCenter - yPos * .89 / yCenter;
}

*/