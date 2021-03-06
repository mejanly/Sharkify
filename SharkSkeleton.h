#ifndef SHARK_SKELETON
#define SHARK_SKELETON

#include <map>
#include <string>
#include <stdio.h>
#include <ostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "SharkBone.h"

using namespace std;

class SharkSkeleton
{
	public:
		SharkSkeleton();
		~SharkSkeleton(){}
		
		void buildSkeleton(string filename);
		map<string, SharkBone*> gBones() {return bones;}
		
   private:
      map<string, SharkBone*> bones; //List of Bones in the shark
      string rootNode;
   
      void sBone(SharkBone *newBone);
      string nextToken(char delimit, FILE* readFile);
		string itoa(int key);   
		SharkBone *gBone(string key);
		SharkBone *gBone(int key);
};
#endif
