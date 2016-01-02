#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <GL/glew.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "RenderingHelper.h"

using namespace std;

/* Cycles through the different keyframes */
class Keyframe
{
	public:
	   Keyframe(vector<SharkBone *> _bones, RenderingHelper *_MatStack);
	   ~Keyframe(){}
	   
      void setKeyframes(const char *boneName, glm::vec3 dir, float angle);
		
	private:
      vector<SharkBone *> bones;
      RenderingHelper *MatStack;
};
#endif

