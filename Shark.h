#ifndef __Shark__
#define __Shark__

#include "Headers.h"
#include "Shape.h"
#include "tiny_obj_loader.h"

#define DECREMENT 0.005

class Shark {
public:
   Shark();
   virtual ~Shark(){};

   Shape *uHead;
   Shape *lHead;
   Shape *fBody;
   Shape *mBody;
   Shape *rBody;
   Shape *lsFin;
   Shape *rsFin;
   Shape *tbFin;
   Shape *btFin;
   Shape *bbFin;
   
   // An angle of rotation for each piece
   float hdAngle;
   float fbAngle;
   float mbAngle;
   float rbAngle;
   float tfAngle;
	
	float lhAngle;

   // Determines the speed of the rotation
   float theta;
   float thetaAdd;
   
   // How close/fast the shark is and its direction
   float zoom;
   float speed;
   float yRot;
   // Shark's location
   glm::vec3 loc;
   
	void update(float isFeed);
	void explode();
    
private:
   vector<tinyobj::material_t> materials;
};

#endif
