#include "Includes.h"

int printOglError (const char *file, int line) {
   /* Returns 1 if an OpenGL error occurred, 0 otherwise. */
   GLenum glErr;
   int retCode = 0;

   glErr = glGetError();
   while (glErr != GL_NO_ERROR) {
      //printf ("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
      retCode = 1;
      glErr = glGetError();
   }
   return retCode;
}

void window_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   // Roll shark along X axis
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	   SharkRotX += .08;
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	   SharkRotX -= .08;
	// Rotate shark around Y axis
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	   SharkRotY += .08;
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	   SharkRotY -= .08;
	
	/*if (key == GLFW_KEY_A && action == GLFW_PRESS)
      g_movey += 2;
   if (key == GLFW_KEY_D && action == GLFW_PRESS)
      g_movey -= 2;
   if (key == GLFW_KEY_N && action == GLFW_PRESS)
	  sharkZoom += .1;
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	  sharkZoom -= .1;
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	  thetaAdd += .001;
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		if (thetaAdd - .001 >= .005) {
			thetaAdd -= .001;
		}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		if (wallCol.b <= 1.0) {
			wallCol += .01;
		}
		if (intensity + .005 <= .8) {
			intensity += .005;
		}
		if (g_light1.y - .5 >= 2) {
			g_light1.y -= .5;
			g_light2.y -= .5;
		}
		sharkRotZAdd = .002;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		wallCol -= .01;
		if (intensity - .005 >= .005) {
			intensity -= .005;
		}
		g_light1.y += .5;
		g_light2.y += .5;
		sharkRotZAdd = -.002;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) 
		sharkRotYAdd = -.002;
	if (key == GLFW_KEY_L && action == GLFW_PRESS) 
		sharkRotYAdd = .002;*/
}

void initGL()
{
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
   

   // Set the background color
   glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
   
   // Enable Z-buffer test
   glEnable(GL_DEPTH_TEST);
   glPointSize(18);
   
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

bool installShaders(const string &vShaderName, const string &fShaderName)
{
	GLint rc;
	
	// Create shader handles
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read shader sources
	const char *vshader = GLSL::textFileRead(vShaderName.c_str());
	const char *fshader = GLSL::textFileRead(fShaderName.c_str());
	glShaderSource(VS, 1, &vshader, NULL);
	glShaderSource(FS, 1, &fshader, NULL);
	
	// Compile vertex shader
 	glCompileShader(VS);
   std::cout << "just compiled the v shader" << std::endl;
   printOglError(__FILE__, __LINE__);
	GLSL::printError();
	glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(VS);
	if(!rc) {
		printf("Error compiling vertex shader %s\n", vShaderName.c_str());
		return false;
	}
	
	// Compile fragment shader
	glCompileShader(FS);
   std::cout << "just compiled the f shader" << std::endl;
	GLSL::printError();
	glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
	GLSL::printShaderInfoLog(FS);
	if(!rc) {
		printf("Error compiling fragment shader %s\n", fShaderName.c_str());
		return false;
	}
	
	// Create the program and link
   ShadeProg = glCreateProgram();
   glAttachShader(ShadeProg, VS);
   glAttachShader(ShadeProg, FS);
   glLinkProgram(ShadeProg);
   std::cout << "just linked the shaders" << std::endl;
   
	GLSL::printError();
	glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(ShadeProg);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
		return false;
	}

   /* get handles to attribute data */
   h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPosition");
   h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNormal");
   h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "uProjMatrix");
   h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "uViewMatrix");
   h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "uModelMatrix");
   h_uLightPos1 = GLSL::getUniformLocation(ShadeProg, "uLightPos1");
   h_uLightPos2 = GLSL::getUniformLocation(ShadeProg, "uLightPos2");
   h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
   h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
   h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
   h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
   h_uLightInts = GLSL::getUniformLocation(ShadeProg, "Uints");

   assert(glGetError() == GL_NO_ERROR);
   return true;
}

inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

/* projection matrix */
void SetProjectionMatrix() {
  glm::mat4 Projection = glm::perspective(50.0f, (float)g_width/g_height, 0.1f, 100.f);
  safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

void drawSharkPiece(Shape *shp) {
   std::vector<tinyobj::shape_t> shapes = shp->gShapes();

	safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));

	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(h_aPosition);
	glBindBuffer(GL_ARRAY_BUFFER, shp->gPos());
	glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array for drawing
	GLSL::enableVertexAttribArray(h_aNormal);
	glBindBuffer(GL_ARRAY_BUFFER, shp->gNor());
	glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	int sIndices = (int)shapes[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shp->gInd());

	//draw the piece
	glDrawElements(GL_TRIANGLES, sIndices, GL_UNSIGNED_INT, 0);

   GLSL::disableVertexAttribArray(h_aPosition);
	GLSL::disableVertexAttribArray(h_aNormal);   
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawShark() {
   // Color varibales for shark
   glUniform3f(h_uMatAmb, 0.08, 0.03, 0.1);
   glUniform3f(h_uMatDif, 0.55, 0.51, 0.51);
   glUniform3f(h_uMatSpec, 1.0, 0.9, 0.6);
   glUniform1f(h_uMatShine, 2.0);
   glUniform1f(h_uLightInts, .5);

   // FRONT BODY
   ModelTrans.loadIdentity();
   // Zoom out from shark
	ModelTrans.translate(glm::vec3(0, 0, -10)); // To move entire shark back
   // Roll whole shark
	ModelTrans.rotate(135+SharkRotX, glm::vec3(1, 0, 0)); // Wtf @ 135 ?
	// Putting this here rotates shark around middle 
	ModelTrans.translate(glm::vec3(-.65, 0, 0)); 
	ModelTrans.rotate(fbAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.28);
	drawSharkPiece(fBody);
	ModelTrans.popMatrix();
	// RIGHT FIN
	ModelTrans.translate(glm::vec3(.06, -.23, -.39));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.28);
	drawSharkPiece(rsFin);
	ModelTrans.popMatrix();
	// LEFT FIN
   ModelTrans.translate(glm::vec3(0, 0, .78));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.28);
	drawSharkPiece(lsFin);
	ModelTrans.popMatrix();
	// LOWER HEAD
	ModelTrans.translate(glm::vec3(-.45, .215, -.39));
	ModelTrans.rotate(hdAngle, glm::vec3(0, 1, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.26);
	drawSharkPiece(lHead);
	ModelTrans.popMatrix();
	// UPPER HEAD
	ModelTrans.translate(glm::vec3(-.23, .03, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.24);
	drawSharkPiece(uHead);
	ModelTrans.popMatrix();
   
	// MID BODY
	ModelTrans.translate(glm::vec3(.6, -.03, 0));
	ModelTrans.rotate(mbAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(.6, 0, 0));
	ModelTrans.pushMatrix();     
	ModelTrans.scale(.38);
	drawSharkPiece(mBody);
	ModelTrans.popMatrix();
	// TOP FIN
	ModelTrans.translate(glm::vec3(-.15, .4, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.2);
	drawSharkPiece(tbFin);
	ModelTrans.popMatrix();
	// REAR BODY
	ModelTrans.translate(glm::vec3(.41, -.38, 0));
	ModelTrans.rotate(rbAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(.41, 0, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.32);
	drawSharkPiece(rBody);
	ModelTrans.popMatrix();
	// BACK TOP FIN
	ModelTrans.translate(glm::vec3(.27, .16, 0));
	ModelTrans.rotate(tfAngle, glm::vec3(0, 1, 0));
	ModelTrans.translate(glm::vec3(.3, 0, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.26);
	drawSharkPiece(btFin);
	ModelTrans.popMatrix();
	// BACK BOTTOM FIN
	ModelTrans.translate(glm::vec3(-.01, -.43, 0));
	ModelTrans.pushMatrix();
	ModelTrans.scale(.26);
	drawSharkPiece(bbFin);
	ModelTrans.popMatrix();
}

void drawGL()
{
   int i;

   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Use our GLSL program
   glUseProgram(ShadeProg);
   glUniform3f(h_uLightPos1, g_light1.x, g_light1.y, g_light1.z);
   glUniform3f(h_uLightPos2, g_light2.x, g_light2.y, g_light2.z);

   SetProjectionMatrix();
   ModelTrans.useModelViewMatrix();
   
   // Increment the variable that determines the speed of the rotations
   theta += thetaAdd;
   
   hdAngle = -cos(theta)*.07 - (SharkRotY / 7);
	fbAngle = -cos(theta)*.1 - (SharkRotY / 5);
	mbAngle = cos(theta)*.2 + (SharkRotY / 5);
	rbAngle = cos(theta)*.25 + (SharkRotY / 3);
	tfAngle = cos(theta)*.3 + SharkRotY;
	
   // Draw shark
   drawShark();

   glUseProgram(0); 
   assert(glGetError() == GL_NO_ERROR);
}

int main(int argc, char **argv) {
	srand(time(NULL));

   // Initialise GLFW
   if(!glfwInit()) {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

   // Open a window and create its OpenGL context
   window = glfwCreateWindow(g_width, g_height, "Sharkify!!", NULL, NULL);
   
   if (window == NULL){
      fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are");
      fprintf(stderr, " not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   glfwSetWindowSizeCallback(window, window_size_callback);

   // Initialize glew
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize glew\n");
      return -1;
   }

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
   initGL();
   installShaders("vert.glsl", "frag.glsl");

	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);

   do{
      drawGL();
      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
   } // Check if the ESC key was pressed or the window was closed
   while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

	 return 0;
}
