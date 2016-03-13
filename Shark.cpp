#include "Shark.h"

Shark::Shark() {
   // Random numbers to determing where the shark is and how
   // fast it is. Basically all arbitrary
   zoom = ((rand() % 30 + 1) * -1.0);
   speed = (float)((rand() % 30 + 15)) / 1500.0; //0.05;
   float x = (float)(rand() % 16 + 3 + (-8)); //-.65;
   float y = (float)(rand() % 7 + (-3));
   loc = glm::vec3(x, y, 0);
   
   //escapePt = ????;
   
   // Adds a little bit of randomness to the starting position
   // of the swimming shark
   theta = 0.0 + (float)(rand() % 360 + 1);
   // Abritrary number for approximately smooth swimming animation
   thetaAdd = speed*3;
   
   // Rotates the shark around the Y based on which direction
   // it is facing
   if (x < 0) {
      yRot = 180;
      speed *= -1;
   }
   else {
      yRot = 0;
   }

   // Load all pieces and initialize them
   uHead = new Shape(materials);
   uHead->loadShapes("SharkSubdivParts/Head_Upper.obj");
   lHead = new Shape(materials);
   lHead->loadShapes("SharkSubdivParts/Head_Lower.obj");
   lsFin = new Shape(materials);
   lsFin->loadShapes("SharkSubdivParts/SideFin_L.obj");
   rsFin = new Shape(materials);
   rsFin->loadShapes("SharkSubdivParts/SideFin_R.obj");
   tbFin = new Shape(materials);
   tbFin->loadShapes("SharkSubdivParts/TopFin.obj");
   fBody = new Shape(materials);
   fBody->loadShapes("SharkSubdivParts/Body_Front.obj");
   mBody = new Shape(materials);
   mBody->loadShapes("SharkSubdivParts/Body_Mid.obj");
   rBody = new Shape(materials);
   rBody->loadShapes("SharkSubdivParts/Body_Rear.obj");
   btFin = new Shape(materials);
   btFin->loadShapes("SharkSubdivParts/BackFin_Top.obj");
   bbFin = new Shape(materials);
   bbFin->loadShapes("SharkSubdivParts/BackFin_Bottom.obj");
   
   uHead->initObj();
   lHead->initObj();
   lsFin->initObj();
   rsFin->initObj();
   tbFin->initObj();
   fBody->initObj();
   mBody->initObj();
   rBody->initObj();
   btFin->initObj();
   bbFin->initObj();
}

void Shark::update() {
   // Increment the variable that determines the speed of the rotations
   theta += thetaAdd;
   
   // More aribtrary number to determine how far each piece rotates
   hdAngle = -cos(theta)*.07;
	fbAngle = -cos(theta)*.1;
	mbAngle = cos(theta)*.2;
	rbAngle = cos(theta)*.25;
	tfAngle = cos(theta)*.3;

   // Moves the shark in the x direction (traveling across the screen)
   loc = glm::vec3(loc.x-speed, loc.y, loc.z);
   
   // If the shark "escapes" the screen, respawn it
   if (loc.x - escapePt < 0) {
      //respawn();
   }
}

void Shark::respawn() {
   //Basically copy and paste the top portion of the constructor
}
