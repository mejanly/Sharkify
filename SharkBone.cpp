#include "SharkBone.h"

SharkBone::SharkBone(int _boneNum) {
    boneNum = _boneNum;
    
    //load identity matricies for translations
    localTrans = glm::mat4(1.0f);
    //combTrans = glm::mat4(1.0f);
    combTrans.useModelViewMatrix();
    combTrans.loadIdentity();
}

void SharkBone::buildBone(string bName, glm::vec3 headpt, glm::vec3 tailpt) {
	boneName = bName;
	headPoint = headpt;
	tailPoint = tailpt;

	boneLength = glm::distance(headpt, tailpt);
	boneVec = headpt - tailpt;
	
	localTrans = glm::translate(localTrans, boneVec);
}

void SharkBone::buildTranslation(glm::vec3 dir, float rot) {
   glm::vec3 localVec = glm::vec3(headPoint - (headPoint - tailPoint)) * -1.0f;
	localTrans = glm::translate(localTrans, localVec);
	localTrans = glm::rotate(localTrans, rot, dir);
	
	boneVec = glm::rotateY(boneVec, rot); // ONLY ROTATES AROUND Y AXIS
	glm::vec3 temp = boneVec - boneLength;
	tailPoint = glm::vec3(temp.x, temp.y, temp.z);
	headPoint = glm::vec3(boneVec.x, boneVec.y, boneVec.z);
	
	
	//glm::vec3 combVec = glm::vec3(headPoint - tailPoint) * -1.0f;
	//combTrans.multMatrix(localTrans); // This could easily be wrong

	vector<SharkBone *>::iterator ic;
	for(ic = childBones.begin(); ic != childBones.end(); ic++) {
		(*ic)->buildTranslation(dir, rot);
	}
}
