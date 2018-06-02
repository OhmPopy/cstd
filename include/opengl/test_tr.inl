#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glut.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
void init()
{
  glClearColor(0, 0, 0, 0);
  //�������㣬��ɫ����
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_COLOR_ARRAY);
}
//�� glArrayElement��ʽ����
void useGlArrayElement()
{
  static GLfloat vertexs[] = {
    -1, -1, 0,
    -0.5, -0.5, 0,
    -1, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //�󶨶�������
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //������ �� ����
  glBegin(GL_TRIANGLES);
  glArrayElement(1);
  glArrayElement(0);
  glArrayElement(2);
  glEnd();
}
void useGlDrawElements()
{
  static GLfloat vertexs[] = {
    -0.4, -1, 0,
    -0.2, -1, 0,
    -0.2, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //�󶨶�������
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //������ �� ����
  GLubyte array[] = {0, 1, 2};
  //type ������ �޷������Σ� GL_BYTE -- > gl
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, array);
}
//glDraw
void useGlDrawArrays()
{
  static GLfloat vertexs[] = {
    0, 0, 0,
    0, 0.5, 0,
    0.3, 0.4, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //�󶨶�������
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //gldraw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //gl
  useGlArrayElement();
  useGlDrawElements();
  useGlDrawArrays();
  glFlush();
}
//��ӡopengl��Ϣ
void printGlInfo()
{
  const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����
  const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨
  const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��
  const GLubyte* gluVersion = gluGetString(GLU_VERSION); //���ص�ǰGLU���߿�汾
  printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
  printf("��Ⱦ����ʶ����%s\n", biaoshifu);
  printf("OpenGLʵ�ֵİ汾�ţ�%s\n", OpenGLVersion);
  printf("OGLU���߿�汾��%s\n", gluVersion);
}
int test_tr(int argc, char* argv[])
{
  glutInit(&argc, (char**)argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("vertexArray");
  printGlInfo();
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

