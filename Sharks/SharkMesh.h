#ifndef D_SHARK_MESH_VERTEX_QUAD
#define D_SHARK_MESH_VERTEX_QUAD

/*This is a "smart mesh" structure that remembers unique vertices. The other mesh structure is to read blender files with */
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <utility>

#include "SharkVertex.h"
#include "Quad.h"
#include "Matrix.h"

/*Compares two vertices to make sure they don't already exist in the mesh. */
struct compareVect3 {
   bool operator() (Vector3f one, Vector3f two) const {
      if (!(one.x < two.x+.00001 && one.x > two.x-.00001)) {
         return one.x < two.x;//+.00001;
      }
      else if (!(one.y < two.y+.00001 && one.y > two.y-.00001)) {
         return one.y < two.y;//+.00001;
      }
      else {
         return one.z < two.z;//+.00001;
      }
   }
};

using namespace std;
class SharkMesh {
	public:
		SharkMesh(){
		   vertices = map<Vector3f, SharkVertex*, compareVect3>(); 
		   faces = vector<Quad*>();
		}
		~SharkMesh(){}
		void deleteHeap(); 

		void buildAOBJ(FILE* readFile);   //returns the pointer to the point it read the first b

		SharkVertex *gVertex(Vector3f key){return vertices.find(key)->second;}	
		map<Vector3f, SharkVertex*, compareVect3>::iterator gVertices(){return vertices.begin();}
		map<Vector3f, SharkVertex*, compareVect3>::iterator gVerticesEnd(){return vertices.end();}
		Matrix gSkinMatrix(string name){return skinTransforms.find(name)->second;}
		Quad* gFace(int index){return faces[index];}
		vector<Quad*>::iterator gFaceBegin(){return faces.begin();}
		vector<Quad*>::iterator gFaceEnd(){return faces.end();}
		map<Vector3f, SharkVertex*,compareVect3>::iterator gVertBegin(){return vertices.begin();}
		map<Vector3f, SharkVertex*,compareVect3>::iterator gVertEnd(){return vertices.end();}

		int faceSize(){return faces.size();}
		int vertSize(){return vertices.size();}
		void pushFace(Quad* f){faces.push_back(f);}
		bool isTransformReady(){return hasNewTransform;}
		bool isUpdateApproved(){return newUpdateApproved;}
		void sNewTransform(bool n){hasNewTransform = n;}
		void sUpdateApproved(bool n){newUpdateApproved = n;}
		
		void restPosition();  //resets transform vertex back to local (rest) pose
		void linearBlendTransform(Matrix matrix, string boneName);
		void linearBlendTransform();
		void setSkinMatrix(string bName, Matrix rix){skinTransforms.insert(pair<string, Matrix>(bName, rix));}
      void insertVec (pair< Vector3f, SharkVertex*> u ){vertices.insert(u);}
      void countWeights();  //prints out combined weights of all the vertices
      string nextToken(char delimit, FILE* readFile);
		
		map<Vector3f, SharkVertex*, compareVect3> vertices;
		map<string, Matrix> skinTransforms;
		vector<Quad*> faces;
		bool hasNewTransform;
		bool newUpdateApproved;
};

#endif
