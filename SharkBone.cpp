#include "SharkBone.h"

SharkBone::SharkBone(int _boneNum) {
    boneNum = _boneNum;
    
    //load identity matricies for translations
    localTrans = glm::mat4(1.0f);
    //combTrans = glm::mat4(1.0f);
    //combTrans.useModelViewMatrix();
    //combTrans.loadIdentity();
}

void SharkBone::buildBone(string bName, glm::vec3 headpt, glm::vec3 tailpt) {
	boneName = bName;
	headPoint = headpt;
	tailPoint = tailpt;

	boneLength = glm::distance(headpt, tailpt);
	boneVec = headpt - tailpt;
	
	localTrans = glm::translate(localTrans, boneVec);
}

// This entire function is currently not doing anything... May ax it.
/*void SharkBone::buildTranslation() {
   // tf does this line do honestly...
   glm::vec3 localVec = glm::vec3(headPoint - (headPoint - tailPoint)) * -1.0f;
	localTrans = glm::translate(localTrans, localVec);
	
	glm::vec3 combVec = glm::vec3(headPoint - tailPoint) * -1.0f;
	combTrans.multMatrix(localTrans); // This could easily be wrong

	vector<SharkBone *>::iterator ic;
	for(ic = childBones.begin(); ic != childBones.end(); ic++) {
		(*ic)->buildTranslation();
	}
}*/
