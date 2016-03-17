#ifndef __SHAPE__
#define __SHAPE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include "tiny_obj_loader.h"
#include "GLSL.h"
#include "Headers.h"

using namespace std;

class Shape {
	public:
		Shape(vector<tinyobj::material_t> _materials);
		~Shape(){}

		glm::vec3 transVec;
		glm::vec3 explodeVec;
		float velocity;
		bool exploding;
		int xrot;
		int yrot;
		int zrot;
		int rotspeed;

		void loadShapes(const string &objFile);
		void initObj();
		GLuint gPos() {return posBufObj;}
		GLuint gNor() {return norBufObj;}
		GLuint gInd() {return indBufObj;}	
		std::vector<tinyobj::shape_t> gShapes() {return shapes;}	
		void setTransVec(glm::vec3 _transVec) { transVec = _transVec; }
		glm::vec3 updateTrans();
		void genExplodeVars();	
		
	private:
	   std::vector<tinyobj::shape_t> shapes;
	   vector<tinyobj::material_t> materials;
	   GLuint posBufObj; 
	   GLuint norBufObj;
	   GLuint indBufObj;
	   
	   void resize_obj();
		float randFloat(float l, float h);
};
#endif
