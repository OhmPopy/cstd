#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#pragma comment(lib,"glew32.lib")
//���ھ��
static GLint window;
GLuint ProgramObject = 0; //�������
GLuint VertexShaderObject = 0; //������ɫ������
GLuint FragmentShaderObject = 0; //Ƭ����ɫ������
//�������OpenGL�汾����ҪGLSL 2.0֧��
void getGlVersion(int* major, int* minor)
{
  const char* verstr = (const char*)glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
    *major = *minor = 0;
    fprintf(stderr, "Invalid GL_VERSION format!!!/n");
  }
}
static void display(void)
{
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //��ѯ������ɫ�����Ա���VertexTemp���������������ã�ÿ��������Ҫָ��һ������
  GLint tempLoc = glGetAttribLocationARB(ProgramObject, "VertexTemp");
  glBegin(GL_TRIANGLES);
  glVertexAttrib1f(tempLoc, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertexAttrib1f(tempLoc, 0.5f);
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glVertexAttrib1f(tempLoc, 1.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glEnd();
  glFlush();
  glutSwapBuffers();
}
static void reshape(int wid, int ht)
{
  float vp = 0.8f;
  float aspect = (float) wid / (float) ht;
  glViewport(0, 0, wid, ht);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
  glFrustum(-vp, vp, -vp / aspect, vp / aspect, 3, 10.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
/*public*/
int installShaders()
{
  GLint vertCompiled, fragCompiled; // status values
  GLint linked;
  /*public*/
  static char* Vertex =
      "//�ȿ�������ɫ��temp.vert��\n"
      "uniform float CoolestTemp;\n"
      "uniform float TempRange;\n"
      "attribute float VertexTemp;//ÿ�����㶼��Ӧһ���¶�ֵ\n"
      "varying float Temperature;//���ݵ�Ƭ�δ��������к�������\n"
      "void main(void)\n"
      "{\n"
      " //���в�ֵ\n"
      " Temperature = (VertexTemp - CoolestTemp) / TempRange;\n"
      " gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
      "}\n";
  static char* Fragment =
      "//Ƭ����ɫ��temp.frag��\n"
      "uniform vec3 CoolestColor;\n"
      "uniform vec3 HottestColor;\n"
      "varying float Temperature;\n"
      "void main(void)\n"
      "{\n"
      " //ͨ���¶�ֵѰ��һ����Ӧ����ɫ��λ�����������֮��\n"
      " vec3 color = mix(CoolestColor, HottestColor,Temperature);\n"
      " gl_FragColor = vec4(color,1.0);\n"
      "}\n";
  // Create a vertex shader object and a fragment shader object
  VertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
  FragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  // Load source code strings into shaders
  glShaderSource(VertexShaderObject, 1, &Vertex, NULL);
  glShaderSource(FragmentShaderObject, 1, &Fragment, NULL);
  // Compile the brick vertex shader, and print out
  // the compiler log file.
  glCompileShader(VertexShaderObject);
  glGetShaderiv(VertexShaderObject, GL_COMPILE_STATUS, &vertCompiled);
  // Compile the brick vertex shader, and print out
  // the compiler log file.
  glCompileShader(FragmentShaderObject);
  glGetShaderiv(FragmentShaderObject, GL_COMPILE_STATUS, &fragCompiled);
  if (!vertCompiled || !fragCompiled) {
    return 0;
  }
  // Create a program object and attach the two compiled shaders
  ProgramObject = glCreateProgram();
  glAttachShader(ProgramObject, VertexShaderObject);
  glAttachShader(ProgramObject, FragmentShaderObject);
  // Link the program object and print out the info log
  glLinkProgram(ProgramObject);
  glGetProgramiv(ProgramObject, GL_LINK_STATUS, &linked);
  if (!linked) {
    return 0;
  }
  // Install program object as part of current state
  glUseProgram(ProgramObject);
  // Set up initial uniform values
  glUniform1f(glGetUniformLocation(ProgramObject, "CoolestTemp"), 0.0f);
  glUniform1f(glGetUniformLocation(ProgramObject, "TempRange"), 1.0f);
  glUniform3f(glGetUniformLocation(ProgramObject, "CoolestColor"), 0.0, 0.0, 1.0);
  glUniform3f(glGetUniformLocation(ProgramObject, "HottestColor"), 1.0, 0.0, 0.0);
  return 1;
}
int test_shading_triangle(int argc, char** argv)
{
  int success = 0;
  int gl_major, gl_minor;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  window = glutCreateWindow("Temperature Shader");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  // Initialize the "OpenGL Extension Wrangler" library
  glewInit();
  // Make sure that OpenGL 2.0 is supported by the driver
  getGlVersion(&gl_major, &gl_minor);
  printf("GL_VERSION major=%d minor=%d/n", gl_major, gl_minor);
  if (gl_major < 2) {
    printf("GL_VERSION major=%d minor=%d/n", gl_major, gl_minor);
    printf("Support for OpenGL 2.0 is required for this demo...exiting/n");
    exit(1);
  }
  success = installShaders();
  if (success) {
    glutMainLoop();
  }
  return 0;
}

