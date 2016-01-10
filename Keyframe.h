#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "RenderingHelper.h"
#include "SharkBone.h"
#include "GLSL.h"

using namespace std;

/* Cycles through the different keyframes */
class Keyframe
{
	public:
	   Keyframe(GLint _h_uModelMatrix);
	   ~Keyframe(){}
	   
      void setKeyframes(SharkBone *boneName, glm::vec3 dir, float angle);
		
	private:
      RenderingHelper MatStack;
      GLint h_uModelMatrix;
      void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};
#endif

