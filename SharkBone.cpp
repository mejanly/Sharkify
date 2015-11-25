#include "SharkBone.h"

SharkBone::SharkBone(int _boneNum) {
    boneNum = _boneNum;
    
    // TODO unsure about these
    transMatHeir = glm::mat4(1.0f);
    transMatLocal = glm::mat4(1.0f);
}

/*builds a bone from an aobj file. Copies over the vertices. Does not set child bones; That has to be done separate 
 * The shark mesh should be loaded already*/
void SharkBone::buildBone(string bName, glm::vec3 headpt, glm::vec3 tailpt)
{
	boneName = bName;
	headPoint = headpt;
	tailPoint = tailpt;

	//length of bone
	boneLength = glm::distance(headpt, tailpt); // TODO unsure
	
	//get rotation
	glm::vec3 boneVec = headpt - tailpt;
	glm::vec3 rot = glm::normalize(boneVec);
	float theta = acos(glm::dot(rot, glm::vec3(1,0,0))); // TODO beware
   //changeAngle(theta); // TODO may need this
}

/*creates translation matrices for this bone and all child bones */
void SharkBone::buildTranslation() {
   glm::vec3 transLoc = glm::vec3(headPoint - (headPoint - tailPoint)) * -1.0f;
	transMatLocal = glm::translate(transMatLocal, transLoc); //skin space to joint space
	
	glm::vec3 transHeir = glm::vec3(headPoint - tailPoint) * -1.0f;
	transMatHeir = glm::translate(transMatHeir, transHeir); // joint space to shark space

	vector<SharkBone *>::iterator ic;
	for(ic = childBones.begin(); ic != childBones.end(); ic++)
	{
		(*ic)->buildTranslation();
	}
}
