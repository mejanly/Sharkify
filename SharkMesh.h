#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "RenderingHelper.h"
#include "tiny_obj_loader.h"
#include "GLSL.h"

#include "SharkSkeleton.h"

#define LIGHT_INTS 0.5

using namespace std;

class SharkMesh {
	public:
		SharkMesh(int _winWidth, int _winHeight);
		~SharkMesh() {}
		
      vector<tinyobj::shape_t> mesh;
      vector<tinyobj::material_t> materials;
      
      void resizeObj(std::vector<tinyobj::shape_t> &shapes);
      void loadShapes(const string &objFile);
      void setShaderVariables(GLuint ShadeProg);
      void drawSkeleton(GLuint ShadeProg);
      void draw(GLuint posBufObj, GLuint norBufObj, GLuint indBufObj, GLuint ShadeProg);
      vector<float> computeNormals(vector<float> posBuf, vector<unsigned int> indBuf);
      void updateZoom(float amount);
      void updateYRot(float amount);
      void initSkeleton();
      
   private:
      GLuint posBufObj;
      GLuint norBufObj;
      GLuint indBufObj;
      RenderingHelper ModelTrans;
      GLint h_aPosition;
      GLint h_aNormal;
      GLint h_uModelMatrix;
      GLint h_uViewMatrix;
      GLint h_uProjMatrix;
      GLint h_uLightPos;
      GLint h_uMatAmb, h_uMatDif, h_uMatSpec;
      GLint h_uMatShine;
      GLint h_uLightInts;
      SharkSkeleton *skeleton;
      //Keyframe *keyframes;
      
      int winWidth;
      int winHeight;
      float zoom;
      float yRot;
      glm::vec3 g_light;
      
      void SetProjectionMatrix();
      void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};

