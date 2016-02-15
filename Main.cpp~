#include "Includes.h"

/*GLuint FBO_Basic, FBO_CBasic, FBO_DBasic, FBO_TBasic;
void initFBO() {
   // Init basic FBO
   glGenFramebuffers(1, &FBO_Basic);
   glBindFramebuffer(GL_FRAMEBUFFER, FBO_Basic);
   // rendered texture
   glGenTextures(1, &FBO_TBasic);
   glBindTexture(GL_TEXTURE_2D, FBO_TBasic);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   // depth buffer
   glGenRenderbuffers(1, &FBO_DBasic);
   glBindRenderbuffer(GL_RENDERBUFFER, FBO_DBasic);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO_DBasic);
   // color buffer
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_TBasic, 0);
   GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
   glDrawBuffers(1, DrawBuffers);
   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      printf("Error making basic frame buffer object\n");
      return;
   }
}*/

/*void setTexture(int tex) {
   texture_id = tex;

   // reference for fresnel values: http://forums.cgsociety.org/archive/index.php/t-513458.html
   // Set cook torrance values depending on texture
   switch (tex) {
   case TEX_SKY_DAY:
      roughness = 1.0;
      fresnel = 1.0;
      geometric = 1.0;
      break;
   case TEX_WOOD_LIGHT:
   case TEX_WOOD_DARK:
   case TEX_WOOD_RED:
   case TEX_WOOD_WALL:
   case TEX_TARGET:
   case TEX_HAMMER:
      roughness = 0.8;
      fresnel = 1.3;
      geometric = 0.9;
      break;
   case TEX_LANTERN:
      roughness = 1.0;
      fresnel = 1.6;
      geometric = 1.0;
      break;
   case TEX_MELON_OUT:
      roughness = 0.2;
      fresnel = 1.6;
      geometric = 0.3;
      break;
   case TEX_MELON_IN:
      roughness = 0.5;
      fresnel = 1.6;
      geometric = 0.1;
      break;
   case TEX_PARTICLE:
      roughness = 0.0;
      fresnel = 1.0;
      geometric = 0.1;
      break;
   case TEX_MIKU:
   case TEX_RIN:
   case TEX_LEN:
   case TEX_KAITO:
   case TEX_SQUISH_BLUE:
   case TEX_SQUISH_PURPLE:
   case TEX_GIRL_RED:
   case TEX_GIRL_BLUE:
   case TEX_GIRL_GREEN:
   case TEX_GIRL_PINK:
      roughness = 0.6;
      fresnel = 1.6;
      geometric = 0.8;
      break;
   case TEX_STEEL:
      roughness = 0.1;
      fresnel = 2.5;
      geometric = 0.2;
      break;
   case TEX_GROUND_SAKURA:
   case TEX_GROUND_GRASS:
   case TEX_MISC:
   default:
      roughness = 1.0;
      fresnel = 1.6;
      geometric = 1.0;
      break;
   }
}*/

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
	// Rotate shark around Y axis -- BROKEN
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	   SharkRotY += .08;
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	   SharkRotY -= .08;
}

void initGL()
{
   //loadAllTextures();

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
   
   // initialize shaders
   shaders[SHADER_DEFAULT] = new Program();
   shaders[SHADER_DEFAULT]->setShaderNames(DEFAULT_VERT_SHADER, DEFAULT_FRAG_SHADER);
   shaders[SHADER_TEXT] = new Program();
   shaders[SHADER_TEXT]->setShaderNames(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);
   shaders[SHADER_BILLBOARD] = new Program();
   shaders[SHADER_BILLBOARD]->setShaderNames(BILLBOARD_VERT_SHADER, BILLBOARD_FRAG_SHADER);

   // Set the background color
   glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
   
   // Enable Z-buffer test
   glEnable(GL_DEPTH_TEST);
   glPointSize(18);
   
   printf("Shaders loaded.\n");

   printf("Initializing shaders.\n");
   shaders[SHADER_DEFAULT]->init();
   shaders[SHADER_TEXT]->init();
   
   
   h_aPosition = shaders[SHADER_DEFAULT]->addAttribute("aPosition");
   h_aNormal = shaders[SHADER_DEFAULT]->addAttribute("aNormal");
   h_uProjMatrix = shaders[SHADER_DEFAULT]->addUniform("uProjMatrix");
   h_uModelMatrix = shaders[SHADER_DEFAULT]->addUniform("uModelMatrix");
   h_uLightPos1 = shaders[SHADER_DEFAULT]->addUniform("uLightPos1");
   h_uMatAmb = shaders[SHADER_DEFAULT]->addUniform("UaColor");
   h_uMatDif = shaders[SHADER_DEFAULT]->addUniform("UdColor");
   h_uMatSpec = shaders[SHADER_DEFAULT]->addUniform("UsColor");
   h_uMatShine = shaders[SHADER_DEFAULT]->addUniform("Ushine");
   h_uLightInts = shaders[SHADER_DEFAULT]->addUniform("Uints");
   
   /*shaders[SHADER_BILLBOARD]->addAttribute("vertPosition");
   shaders[SHADER_BILLBOARD]->addAttribute("vertTexCoords");
   shaders[SHADER_BILLBOARD]->addUniform("P");
   shaders[SHADER_BILLBOARD]->addUniform("MV");
   shaders[SHADER_BILLBOARD]->addUniform("scale");
   shaders[SHADER_BILLBOARD]->addUniform("color");
   shaders[SHADER_BILLBOARD]->addTexture(&texture);*/

   shaders[SHADER_TEXT]->addAttribute("aCoord");
   shaders[SHADER_TEXT]->addUniform("uTex");
   shaders[SHADER_TEXT]->addUniform("uCol");

   // Enable texture drawing
   /*glEnable(GL_TEXTURE_2D);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/

   shaders[SHADER_TEXT]->addAttribute("aCoord");
   shaders[SHADER_TEXT]->addUniform("uTex");
   shaders[SHADER_TEXT]->addUniform("uCol");
   
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
   
   fontEngine = new FontEngine(g_width, g_height, shaders[SHADER_TEXT], shaders[SHADER_DEFAULT]);
   fontEngine->init(shaders[SHADER_TEXT]->getPID());
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
   //glUseProgram(ShadeProg);
   shaders[SHADER_DEFAULT]->bind();
   glUniform3f(h_uLightPos1, g_light1.x, g_light1.y, g_light1.z);
   glUniform3f(h_uLightPos2, g_light2.x, g_light2.y, g_light2.z);

   SetProjectionMatrix();
   ModelTrans.useModelViewMatrix();
   
   // Increment the variable that determines the speed of the rotations
   theta += thetaAdd;
   
   hdAngle = -cos(theta)*.07;
	fbAngle = -cos(theta)*.1;
	mbAngle = cos(theta)*.2;
	rbAngle = cos(theta)*.25;
	tfAngle = cos(theta)*.3;
	
   // Draw shark
   drawShark();
   //setTexture(textures[TEX_GROUND_GRASS]);
   char buff[25];
   sprintf(buff, "SHARKTASTIC!!!");
   fontEngine->useFont("caviar", 100);   
   fontEngine->display(glm::vec4(0.0, 0.0, 1.0, 1.0), buff, 0-(fontEngine->getTextWidth(buff)/2), 0);      

   shaders[SHADER_DEFAULT]->unbind();
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
   //installShaders("vert.glsl", "frag.glsl");

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
