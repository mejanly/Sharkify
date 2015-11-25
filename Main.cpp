#include "includes.h"

int printOglError (const char *file, int line) {
   /* Returns 1 if an OpenGL error occurred, 0 otherwise. */
   GLenum glErr;
   int    retCode = 0;
  
   glErr = glGetError();
   while (glErr != GL_NO_ERROR)
   {
      retCode = 1;
      glErr = glGetError();
   }
   return retCode;
}

void initialize() {
   sharkMesh = new SharkMesh(g_width, g_height);
   sharkMesh->loadShapes("Model/LeopardShark.aobj");
   
   // Set the background color
   glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
   // Enable Z-buffer test
   glEnable(GL_DEPTH_TEST);
   glPointSize(18);

   // Send the position array to the GPU
	const vector<float> &posBuf = sharkMesh->mesh[0].mesh.positions;
	glGenBuffers(1, &posBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
		
	// Send the index array to the GPU
	const vector<unsigned int> &indBuf = sharkMesh->mesh[0].mesh.indices;
	glGenBuffers(1, &indBufObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

	const vector<float> &norBuf = sharkMesh->computeNormals(posBuf, indBuf);//sharkMesh->mesh[0].mesh.normals;
	glGenBuffers(1, &norBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
	glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLSL::checkVersion();
	assert(glGetError() == GL_NO_ERROR);
}

bool installShaders(const string &vShaderName, const string &fShaderName) {
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
   sharkMesh->setShaderVariables(ShadeProg);

   assert(glGetError() == GL_NO_ERROR);
   return true;
}

void window_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   // Zoom in
   if (key == GLFW_KEY_W && action == GLFW_PRESS)
	  sharkMesh->updateZoom(.1);
	// Zoom out
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	  sharkMesh->updateZoom(-.1);
}

int main(int argc, char **argv)
{
	// Initialise GLFW
   if(!glfwInit()) {
      fprintf(stderr, "Failed to initialize GLFW\n");
      return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

   // Open a window and create its OpenGL context
   window = glfwCreateWindow(g_width, g_height, "Sharkify!!", NULL, NULL);
   if(window == NULL) {
      fprintf(stderr, "Failed to open GLFW window.\n");
      glfwTerminate();
      return -1;
   }
   
   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   glfwSetWindowSizeCallback(window, window_size_callback);
   // Initialize glew
   if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	initialize();
	installShaders("vert.glsl", "frag.glsl");

   glClearColor(0.0f, 0.1f, 0.5f, 1.0f);

   do{
      sharkMesh->draw(posBufObj, norBufObj, indBufObj, ShadeProg);
      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
   } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );

   // Close OpenGL window and terminate GLFW
   glfwTerminate();

	return 0;
}
