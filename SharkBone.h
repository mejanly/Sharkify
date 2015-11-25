#ifndef SHARK_BONE_3D_D
#define SHARK_BONE_3D_D

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;

/*This is a shark bone, a collection of quads and vertices that can all rotate and translate in unison. It makes up a SharkSkeleton,
 which simulates the movement of a shark's swim by moving the individual bones around. */
class SharkBone
{
	public:
		SharkBone(int _boneNum);
		~SharkBone(){}
		
		void buildBone(string boneName, glm::vec3 headpt, glm::vec3 tailpt); //general case bone builder. Needs a mesh with bone info in it
		void addChild(SharkBone *ch) {childBones.push_back(ch);}
      void buildTranslation();
		string gName() {return boneName;}
		
		/*void transformBone(Matrix* stackmatri, bool isARigidBodyTransform);
		//void buildBone(Mesh *mesh, float start, float end, Matrix matrixMultiplier); //takes in mesh input and records them in 
		//Quad and SharkVertex form
		void changeAngle(int newAngle, bool isAheadRoot); //cal shark. changes the quaternion and associted angle
		void changeAngle(int newAngle); 
		void changeAngle(glQuaternion newAngle); 
		void printToFrame();
      //lowerclass management
		Quad* gQuad(int index){return quads[index];}
		void sQuad(Quad* q){return quads.push_back(q);}
		void sLength(double len){boneLength = len;}
		double gLength(){return boneLength;}
		//void buildTranslation(Vector3f root, Vector3f prevhead, Vector3f prevtail);*/
		/*void boneLengthToTranslation(bool downStream); //for the non-general building (Cal Shark) method only  	
		void addTranslation(Matrix trans){transMatHeir.multRight(trans);}
		void sJointTranslation(Matrix trans){jointTrans.multRight(trans);}
		void sName(string newname){boneName = newname;}
		Vector3f gHead(){return headPoint;}
		Vector3f gTail(){return tailPoint;}*/
      //void draw(Matrix *stackMatrix);

	//private:		
		/*vector<Quad*> quads; //Quads are organized into rows, and each row has a list of quads in it. Good for rigib body transforms
		SharkMesh *sMesh;  //pointer to the main shark mesh.
		Matrix rotationMatrix;*/
		//double boneLength; //this is the length of the mesh segment. 

	private:
		string boneName; //given bone name.  A bone's name should be unique in its skeleton.
      glm::vec3 headPoint;
		glm::vec3 tailPoint;
		vector<SharkBone *> childBones; //child bones. Recursive transform downwards
      double boneLength;
      int boneNum;  // the index of the bone in the skeleton
		glm::mat4 transMatHeir;   // apply this translation to the stack           translates from boneLength
		glm::mat4 transMatLocal;  // only apply this translation to this bone      translates from startB and endB
		
		/*void drawTri(Matrix matrix);
		Matrix createTransforms(Matrix *stackmatrix);
      glQuaternion rotatQ;
		float angleOfRot; //rotation along xz plane
		double uLength; //if the shark were to travel the path like a monorail, 
		   //this is the u difference between the bone passing the same location as the bone in front of it.
		float startB;
		float endB;  //these are the start and end points on the bone, relative to the entire mesh in units. related to boneLength.
		*/

};
#endif

