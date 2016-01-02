#include "Keyframe.h"

Keyframe::Keyframe(vector<SharkBone *> _bones, RenderignHelper *_ModelTrans) {
    ModelTrans = _ModelTrans;
    bones = _bones;
}

void Keyframe::setKeyframes(const char *boneName, glm::vec3 dir, float angle) {
   typedef map<string, SharkBone*> BoneMap;
   BoneMap boneMap = bones;
   
   for(BoneMap::const_iterator it = boneMap.begin(); it != boneMap.end(); ++it) {
      string name = it->first;
      SharkBone *bone = it->second;

      ModelTrans.pushMatrix();
      ModelTrans.rotate(dir, angle);
   }
}
