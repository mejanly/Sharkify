#include "SharkMesh.h"

SharkMesh::SharkMesh(int _winWidth, int _winHeight) {
   winWidth = _winWidth; 
   winHeight = _winHeight; 
	zoom = -2.5; 
	yRot = 0;
	g_light = glm::vec3(-5, 8, 0);
   initSkeleton();
}

void SharkMesh::initSkeleton() {
   skeleton = new SharkSkeleton();
   skeleton->buildSkeleton("Model/LeopardShark.aobj");
}

void SharkMesh::resizeObj(std::vector<tinyobj::shape_t> &shapes) {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shapes.size(); i++) {
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
            if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

            if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
            if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

            if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
            if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
        }
    }
   //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
     maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
     maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
     maxExtent = zExtent;
   }
    scaleX = 2.0 /maxExtent;
    shiftX = minX + (xExtent/ 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0/ maxExtent;
    shiftZ = minZ + (zExtent)/2.0;

    //Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
      }
   }
}

void SharkMesh::loadShapes(const string &objFile) {
	string err = tinyobj::LoadObj(mesh, materials, objFile.c_str());
	if(!err.empty()) {
		cerr << err << endl;
	}
   resizeObj(mesh);
}

/* projection matrix */
void SharkMesh::SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(50.0f, (float)winWidth/winHeight, 0.1f, 100.f);
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

vector<float> SharkMesh::computeNormals(vector<float> posBuf, vector<unsigned int> indBuf) {
   vector<float> norBuf;
   vector<glm::vec3> crossBuf;
   int idx1, idx2, idx3;
   glm::vec3 v1, v2, v3;
   glm::vec3 vec1, vec2, vec3;
   glm::vec3 cross1, cross2, cross3;

   // For every vertex initialize a normal to 0
   for (int j = 0; j < posBuf.size()/3; j++) {
      norBuf.push_back(0);
      norBuf.push_back(0);
      norBuf.push_back(0);

      crossBuf.push_back(glm::vec3(0, 0, 0));
   }

   // Compute normals for every face then add them to every associated vertex
   for (int i = 0; i < indBuf.size()/3; i++) {
      idx1 = indBuf[3*i+0];
      idx2 = indBuf[3*i+1];
      idx3 = indBuf[3*i+2];
      v1 = glm::vec3(posBuf[3*idx1 +0], posBuf[3*idx1 +1], posBuf[3*idx1 +2]); 
      v2 = glm::vec3(posBuf[3*idx2 +0], posBuf[3*idx2 +1], posBuf[3*idx2 +2]); 
      v3 = glm::vec3(posBuf[3*idx3 +0], posBuf[3*idx3 +1], posBuf[3*idx3 +2]); 

      vec1 = glm::normalize(v1 - v2);
      vec2 = glm::normalize(v2 - v3);
      vec3 = glm::normalize(v3 - v1);

      cross1 = glm::cross(vec1, vec2);
      cross2 = glm::cross(vec2, vec3);
      cross3 = glm::cross(vec3, vec1);

      crossBuf[idx1] += cross1;
      crossBuf[idx2] += cross2;
      crossBuf[idx3] += cross3;
   }

   // Cross products have been added together, normalize them and add to normal buffer
   for (int i = 0; i < indBuf.size()/3; i++) {
      idx1 = indBuf[3*i+0];
      idx2 = indBuf[3*i+1];
      idx3 = indBuf[3*i+2];

      cross1 = glm::normalize(crossBuf[idx1]);
      cross2 = glm::normalize(crossBuf[idx2]);
      cross3 = glm::normalize(crossBuf[idx3]);

      norBuf[3*idx1+0] = cross1.x;
      norBuf[3*idx1+1] = cross1.y;
      norBuf[3*idx1+2] = cross1.z;
      norBuf[3*idx2+0] = cross2.x;
      norBuf[3*idx2+1] = cross2.y;
      norBuf[3*idx2+2] = cross2.z;
      norBuf[3*idx3+0] = cross3.x;
      norBuf[3*idx3+1] = cross3.y;
      norBuf[3*idx3+2] = cross3.z;
   }

   return norBuf;
}

void SharkMesh::updateZoom(float amount) {
   zoom += amount;
}

void SharkMesh::updateYRot(float amount) {
   yRot += amount;
}

void SharkMesh::setShaderVariables(GLuint ShadeProg) {
   h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPosition");
   h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNormal");
   h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "uProjMatrix");
   h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "uViewMatrix");
   h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "uModelMatrix");
   h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
   h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
   h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
   h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
   h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
   h_uLightInts = GLSL::getUniformLocation(ShadeProg, "Uints");
}

void SharkMesh::safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void SharkMesh::drawSkeleton(GLuint ShadeProg) {
   typedef map<string, SharkBone*> BoneMap;
   BoneMap boneMap = skeleton->gBones();

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Use our GLSL program
   glUseProgram(ShadeProg);
   SetProjectionMatrix();
   ModelTrans.useModelViewMatrix();

   // Set color of shark
   glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
   glUniform3f(h_uMatAmb, 0.08, 0.03, 0.1);
   glUniform3f(h_uMatDif, 0.55, 0.51, 0.51);
   glUniform3f(h_uMatSpec, 1.0, 0.9, 0.6);
   glUniform1f(h_uMatShine, 2.0);
   glUniform1f(h_uLightInts, LIGHT_INTS);
   
   // Set/send ModelView
   ModelTrans.loadIdentity();
   ModelTrans.translate(glm::vec3(0, 0, zoom-3));
	ModelTrans.rotate(yRot, glm::vec3(0, 1, 0)); // line of interest
	ModelTrans.translate(glm::vec3(1, 0, 0));
	
	//keyframes = new Keyframe(skeleton->gBones(), &ModelTrans);
	//keyframes->setFrames("Spine4", glm::vec3(0, 1, 0), 90.0);
	
	glLineWidth(2.5); 
   glBegin(GL_LINES);
      
   for(BoneMap::const_iterator it = boneMap.begin(); it != boneMap.end(); ++it) {
      string name = it->first;
      SharkBone *bone = it->second;

      glm::vec3 head = bone->headPoint;
      glm::vec3 tail = bone->tailPoint;

      safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
      
      glVertex3f(head.x, head.y, head.z);
      glVertex3f(tail.x, tail.y, tail.z);
   }

   glEnd();

   
   glUseProgram(0);
}

void SharkMesh::draw(GLuint posBufObj, GLuint norBufObj, GLuint indBufObj, GLuint ShadeProg)
{
   int i;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Use our GLSL program
   glUseProgram(ShadeProg);
   SetProjectionMatrix();
   ModelTrans.useModelViewMatrix();

   // Set color of shark
   glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
   glUniform3f(h_uMatAmb, 0.08, 0.03, 0.1);
   glUniform3f(h_uMatDif, 0.55, 0.51, 0.51);
   glUniform3f(h_uMatSpec, 1.0, 0.9, 0.6);
   glUniform1f(h_uMatShine, 2.0);
   glUniform1f(h_uLightInts, LIGHT_INTS);
   
   // Set/send ModelView
   ModelTrans.loadIdentity();
	ModelTrans.translate(glm::vec3(0, 0, zoom));
	
	safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));

	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(h_aPosition);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
	glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array for drawing
	GLSL::enableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
	glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	int sIndices = (int)mesh[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);

	//draw the head	
	glDrawElements(GL_TRIANGLES, sIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_aPosition);
	GLSL::disableVertexAttribArray(h_aNormal);   
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
   glUseProgram(0); 
   //assert(glGetError() == GL_NO_ERROR);
}
