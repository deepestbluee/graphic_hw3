#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static GLFWwindow * win = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;


GLint uni_MVP ;
GLint uni_MV ;
GLint uni_cameraPosition ;
GLint uni_heightFactor;
GLint uni_rgbTexture ;
GLint uni_widthTexture ;
GLint uni_heightTexture ;
//declarations

glm::vec3 c_pos; 
glm::vec3 c_up;
glm::vec3 c_gaze;
GLfloat c_near;
GLfloat c_far;
GLfloat c_aspect;
GLfloat c_angle;
GLfloat c_speed;

GLfloat heightFactor;

int widthTexture, heightTexture;

float * vertices;
int * indices;


void setIndices(int widthTexture,int heightTexture)
{
  indices = new int[widthTexture*heightTexture*6];
  int indIndice = 0 ;
  int indVertex = 0 ; 
  for (int i = 0; i < heightTexture; ++i)
  {
    for (int j = 0; j < widthTexture; ++j)
    {

        indices[indIndice++] = indVertex;
        indices[indIndice++] = indVertex+1;
        indices[indIndice++] = indVertex+widthTexture+1;

        indices[indIndice++] = indVertex+1;
        indices[indIndice++] = indVertex+widthTexture+1;
        indices[indIndice++] = indVertex+widthTexture+2;
        indVertex++;
    }
    indVertex++;    
  }
}

void setVertices(int widthTexture,int heightTexture)
{
  vertices = new float[(widthTexture+1)*(heightTexture+1)*3];
  int count = 0;
  for (float i = 0; i <= heightTexture; ++i)
  {
    for (float j = 0; j <= widthTexture; ++j)
    {
      vertices[count++] = j;
      vertices[count++] = 0.0f;
      vertices[count++] = i;
    } 
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void clear()
{
  glClearColor(0, 0, 0, 1);
  glClearDepth(1.0f);
  glClearStencil(0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}
void communication()
{

  glm::mat4 Projection = glm::perspective(glm::radians(c_angle), c_aspect, c_near, c_far);
  glm::mat4 View = glm::lookAt(c_pos,c_pos+c_gaze,c_up);
  glm::mat4 Model = glm::mat4(1.0f);

  glm::mat4 MVP = Projection * View * Model ;
  
  glUniformMatrix4fv(uni_MVP,1,GL_FALSE,&MVP[0][0]);
  //uni_MV = glGetUniformLocation(idProgramShader, "MV");
  glUniform4fv(uni_cameraPosition,1,value_ptr(c_pos));

  glUniform1f(uni_heightFactor,heightFactor); 
  //uni_rgbTexture = glGetUniformLocation(idProgramShader, "rgbTexture")
  glUniform1i(uni_widthTexture,widthTexture);
  glUniform1i(uni_heightTexture,heightTexture);


}



void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

}




int main(int argc, char * argv[]) {

  if (argc != 2) {
    printf("Only one texture image expected!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);

  if (!win) {
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(win);

  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);


  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    glfwTerminate();
    exit(-1);
  }


 
 //initialization

  initShaders();
  glUseProgram(idProgramShader);
  initTexture(argv[1], &widthTexture, &heightTexture);
    //glViewport(0, 0, widthTexture, heightTexture);

  c_pos = glm::vec3(widthTexture/2,widthTexture/10,-widthTexture/4); 
  c_up = glm::vec3(0,1,0);
  c_gaze = glm::vec3(0,0,1);
  c_near = 0.1;
  c_far = 1000;
  c_aspect = 1;
  c_angle = 45.0;
  c_speed = 0;
  heightFactor = 10.0;


  glm::vec3 light_Pos = glm::vec3(widthTexture/2,widthTexture+heightTexture,heightTexture/2);
  
  uni_MVP = glGetUniformLocation(idProgramShader, "MVP");
 // uni_MV = glGetUniformLocation(idProgramShader, "MV");
  uni_cameraPosition = glGetUniformLocation(idProgramShader, "cameraPosition");
  uni_heightFactor = glGetUniformLocation(idProgramShader, "heightFactor"); 
  
  //uni_rgbTexture = glGetUniformLocation(idProgramShader, "rgbTexture");
  uni_widthTexture = glGetUniformLocation(idProgramShader, "widthTexture");
  uni_heightTexture = glGetUniformLocation(idProgramShader, "heightTexture");

  communication();


  glEnableClientState(GL_VERTEX_ARRAY);
  setIndices(widthTexture, heightTexture);
  setVertices(widthTexture, heightTexture);

  GLuint vab, eab;
  glGenBuffers(1, &vab);
  glGenBuffers(1, &eab);
  
  glBindBuffer(GL_ARRAY_BUFFER, vab);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  
  unsigned int  vertSize = (heightTexture+1)*(widthTexture+1)*3*sizeof(int);
  unsigned int  indicesSize = widthTexture*heightTexture*6*sizeof(int); 
  
  glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glDrawElements(GL_TRIANGLES,indicesSize/sizeof(int),GL_UNSIGNED_INT, 0);


  while (!glfwWindowShouldClose(win)) {
    
    processInput(win);
    clear();

    communication();

    glUseProgram(idProgramShader);
    glDrawElements(GL_TRIANGLES,indicesSize/sizeof(int),GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }


  glDeleteBuffers(1, &eab);
  glDeleteBuffers(1, &vab);

  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}
