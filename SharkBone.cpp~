#include "SharkBone.h"

SharkBone::SharkBone(int _boneNum) {
    boneNum = _boneNum;
    
    //load identity matricies for translations
    localTrans = glm::mat4(1.0f);
}

void SharkBone::buildBone(string bName, glm::vec3 headpt, glm::vec3 tailpt) {
	boneName = bName;
	headPoint = headpt;
	tailPoint = tailpt;

	boneLength = glm::distance(headpt, tailpt);
	boneVec = headpt - tailpt;
	
	localTrans = glm::translate(localTrans, boneVec);
}
