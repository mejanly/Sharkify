#include "SharkSkeleton.h"

using namespace std;

SharkSkeleton::SharkSkeleton() {
   rootNode = "Spine3"; // TODO check this
   bones = map<string, SharkBone*>();
}

string SharkSkeleton::nextToken(char delimit, FILE* readFile) {
	string numb;
	while(!feof(readFile)) 	{
		char cur = fgetc(readFile);
		if(cur == delimit || cur == '\n') {break;}
		numb.append(1,cur);
	}
	return numb;
}

SharkBone *SharkSkeleton::gBone(string key) {
	return bones.find(key)->second;
}

string SharkSkeleton::itoa(int key) {
	char dec = (char) key%10;
	char tens = (char) (key/10)%10;
	char hunds = (char) (key/100)%10;
	char thous = (char) (key/1000)%10;
	char tents = (char) (key/10000)%10;
	return string(1,tents)+string(1,thous)+string(1,hunds)+string(1,tens)+string(1,dec);
}

//bone getter where the key prefers to be an integer
SharkBone *SharkSkeleton::gBone(int key) { 
	return gBone(itoa(key));
}

/*sets the bone, with its name as its key */
void SharkSkeleton::sBone(SharkBone *newBone) {
	bones[newBone->gName()] = newBone;	
}

/*Builds an entire mesh and skeleton straight out of a .aobj file */
void SharkSkeleton::buildSkeleton(string filename) {
   FILE* readFile;
	readFile = fopen(filename.c_str(), "r");
     	if(readFile == NULL) {
	   	printf(".aobj file not found\n");
	  	exit(-1);
 	} 

	//go though the rest of the parsing, pulling out each bone	
	char identifier = fgetc(readFile);
	int index = 0;
	string temp, bName;
	vector<vector<string> > boneRelationships = vector<vector<string> >();
	while(identifier != EOF) {
		//b name headRestArmature tailRestArmature ... child names ...
		char cur;
		if(identifier == 'b') {
			identifier = 0;
			cur = fgetc(readFile);  //space
			vector<string> childNames = vector<string>();
			while(cur != '\n') { //per line
			   // bone name is first token
				bName = nextToken(' ', readFile);
				// next three tokens for head armature
				glm::vec3 headr;
				headr.x = atof(nextToken(' ', readFile).c_str());
				headr.y = atof(nextToken(' ', readFile).c_str());
				headr.z = atof(nextToken(' ', readFile).c_str());
            // next three tokens for tail armature
				glm::vec3 tailr;
				tailr.x = atof(nextToken(' ', readFile).c_str());
				tailr.y = atof(nextToken(' ', readFile).c_str());
				tailr.z = atof(nextToken(' ', readFile).c_str());
			
			   // create bone
            SharkBone *bone = new SharkBone(index);
				bone->buildBone(bName, headr, tailr);
				sBone(bone);

				// first bone is parent 
				childNames.push_back(bName);
				fseek(readFile, -1, SEEK_CUR);
				cur = fgetc(readFile);
				while(cur != '\n') {
				   // add the names of all the other children
				   temp = nextToken(' ', readFile);
					childNames.push_back(temp);
					fseek(readFile, -1, SEEK_CUR);
					cur = fgetc(readFile);
				}
				// push the parent and child bones into the bone relationship
				// structure
				boneRelationships.push_back(childNames);
				index++;
			}
		}
		else {
			while(cur != '\n') {
			   // add the names of all the other children
			   cur = fgetc(readFile);
			}
		}

		identifier = fgetc(readFile);
	}

	//Now, build the relationship tree among the bones
	SharkBone *parent, *child;
	for(int i = 0; i < boneRelationships.size(); i++) {
		parent = gBone(boneRelationships[i][0]);
		for(int j = 1; j < boneRelationships[i].size(); j++) {
		   // first bone of i is the parent, add the jth bone as a child
		   child = gBone(boneRelationships[i][j]);	
			parent->addChild(child);	
	   }
	   if (strcmp(parent->gName().c_str(), "Spine4") == 0) {
	      parent->buildTranslation(glm::vec3(0, 1, 0), 90);
	   }
	}
	fclose(readFile);
}
