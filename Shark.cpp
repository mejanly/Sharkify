#include "Shark.h"

Shark::Shark() {
	hdAngle = 0.0;
   fbAngle = 0.0;
   mbAngle = 0.0;
	rbAngle = 0.0;
	tfAngle = 0.0;
   // Random numbers to determing where the shark is and how
   // fast it is. Basically all arbitrary
   zoom = ((rand() % 10 + 1) * -1.0);
   speed = speed = (float)((rand() % 50 + 30)) / 1500.0; //0.05;
   float x = (float)(rand() % 16 + 4 + (-8)); //-.65;
   float y = (float)(rand() % 5 + (-2));
   loc = glm::vec3(x, y, 0);
   
   // Adds a little bit of randomness to the starting position
   // of the swimming shark
   theta = 0.0 + (float)(rand() % 360 + 1);
   // Abritrary number for approximately smooth swimming animation
   thetaAdd = 2.25*speed;

	printf("%lf %lf %lf\n", speed, theta, thetaAdd);
   
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
   hdAngle = -cos(theta)*5;//*3;
	fbAngle = -cos(theta)*4;
	mbAngle = cos(theta)*7;
	rbAngle = cos(theta)*10;
	tfAngle = cos(theta)*13;

	//printf("%lf\n", hdAngle);

   // Moves the shark in the x direction (traveling across the screen)
   loc = glm::vec3(loc.x-speed, loc.y, loc.z);
}

void Shark::feed() {
	//theta *= 3;
	thetaAdd *= 3;
	speed *= 3;
}
