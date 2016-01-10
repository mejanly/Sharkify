#include "Keyframe.h"

Keyframe::Keyframe(GLint _h_uModelMatrix) {
   h_uModelMatrix = _h_uModelMatrix;
   
   MatStack.useModelViewMatrix();
   MatStack.loadIdentity();
   MatStack.translate(glm::vec3(1.2, -.3, -3));
   
   glLineWidth(2.5); 
   glBegin(GL_LINES);
}

void Keyframe::setKeyframes(SharkBone *bone, glm::vec3 dir, float angle) {
   MatStack.pushMatrix();
   MatStack.rotate(angle, dir); 
   
   glm::mat4 curr = MatStack.getMatrix();
   
   glm::vec4 head = curr * glm::vec4(bone->headPoint - glm::vec3(0, 0, 0), 1);
   glm::vec4 tail = curr * glm::vec4(bone->tailPoint - glm::vec3(0, 0, 0), 1);

   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(MatStack.modelViewMatrix));
   
   glVertex4f(head.x, head.y, head.z, head.w);
   glVertex4f(tail.x, tail.y, tail.z, tail.w);
   
   vector<SharkBone *>::iterator ic;
   vector<SharkBone *> bones = bone->gBones();

   MatStack.popMatrix();
   
   for(ic = bones.begin(); ic != bones.end(); ic++) {
      setKeyframes((*ic), dir, angle); 
   }
}

void Keyframe::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}
