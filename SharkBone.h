#ifndef SHARK_BONE_3D_D
#define SHARK_BONE_3D_D

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

/*This is a shark bone, a collection of quads and vertices that can all rotate and translate in unison. It makes up a SharkSkeleton,
 which simulates the movement of a shark's swim by moving the individual bones around. */
class SharkBone
{
	public:
	   SharkBone(int _boneNum);
	   ~SharkBone(){}
	   
      glm::vec3 headPoint;
      glm::vec3 tailPoint;
	
		void buildBone(string bName, glm::vec3 headpt, glm::vec3 tailpt);
		void addChild(SharkBone *ch) {childBones.push_back(ch);}
		string gName() {return boneName;}
		vector<SharkBone *> gBones() {return childBones;}
		void sCombTrans(glm::mat4 mat) {combTrans = mat;}
	   glm::mat4 gCombTrans() {return combTrans;}
		
	private:
	   int boneNum;
		string boneName; //given bone name.  A bone's name should be unique in its skeleton.
      float boneLength;
      glm::mat4 localTrans;
      glm::mat4 combTrans;
      glm::vec3 boneVec;
      vector<SharkBone *> childBones;      
};
#endif

