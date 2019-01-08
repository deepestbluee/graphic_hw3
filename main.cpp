#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

static GLFWwindow * win = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


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
  c_pos += c_speed * c_gaze;
  
  glm::mat4 Projection = glm::perspective(c_angle, c_aspect, c_near, c_far);
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

bool isFull = false ;
int w,h,wp,hp;
float pitch = 45;
float yaw = 90; 
static void processInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_RELEASE) return;
  
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
  {
    heightFactor += 0.5;
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
  {
    heightFactor -= 0.5;
  }
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
  {
    c_speed += 0.5;
  }
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
  {
    c_speed -= 0.5;
  }
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
  {

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(!isFull)
    {
      glfwGetWindowSize(win ,&w, &h);
      glfwGetWindowPos(win ,&wp, &hp);
      glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
      isFull = true;
    }
    else
    {
      isFull = false;     
      glfwSetWindowMonitor(window, 0,wp, hp, w,h,0);
    }
    //glutFullScreen();
  }
  
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {

    pitch += 1.0;
    c_gaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    c_gaze.y = sin(glm::radians(pitch-45.0f));
    c_gaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));  
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    yaw -= 1.0;
    c_gaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    c_gaze.y = sin(glm::radians(pitch-45.0f));
    c_gaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));    


  }  
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    pitch -= 1.0;
    c_gaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    c_gaze.y = sin(glm::radians(pitch-45.0f));
    c_gaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));    
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {

    yaw += 1.0;
    c_gaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    c_gaze.y = sin(glm::radians(pitch-45.0f));
    c_gaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));    
  }
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

  c_pos = glm::vec3(widthTexture/2.0,widthTexture/10.0,-widthTexture/4.0); 
  c_up = glm::vec3(0,1,0);
  c_gaze = glm::vec3(0,0,1);
  c_near = 0.1;
  c_far = 1000;
  c_aspect = 1;
  c_angle = 45.0;
  c_speed = 0;
  heightFactor = 10.0;


  glm::vec3 light_Pos = glm::vec3(widthTexture/2.0,widthTexture+heightTexture,heightTexture/2.0);
  
  uni_MVP = glGetUniformLocation(idProgramShader, "MVP");
  //uni_MV = glGetUniformLocation(idProgramShader, "MV");
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
  
  
  glfwSetKeyCallback(win, processInput);

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(win)) {
    
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
