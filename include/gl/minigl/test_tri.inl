
// OpenGL�����������֮һ���򵥵�����
// �ȱ�������һ���򵥵����ӣ��������ǿ�����һ��ֱ�۵�ӡ�󡣴�����������ǿ��Կ���OpenGL������ʲô����Ȼ�������ֻ���˺ܼ򵥵�һ����--����һ����ɫ�������Ρ��������⣬���ǻ����Կ������͵�OpenGL����ṹ��openGL������˳��

// ��1�������ں�ɫ�ı����»���һ����ɫ�������Σ���ͼһ��ʾ��

//static GLuint Object;

#include <gl/gl.h>
#include <gl/glut.h>

#if 0
static GLuint make_object(void)
{
  //GLuint list;

  //list = glGenLists(1);

  //glNewList(list, GL_COMPILE);

  glBegin(GL_TRIANGLES);//��ʼ��������
  glShadeModel(GL_SMOOTH);//����Ϊ�⻬����ģʽ

  glColor3f(1.0, 0.0, 0.0); //���õ�һ������Ϊ��ɫ
  glVertex2f(-1.0, -1.0); //���õ�һ�����������Ϊ��-1.0��-1.0��

  glColor3f(0.0, 1.0, 0.0); //���õڶ�������Ϊ��ɫ
  glVertex2f(0.0, -1.0); //���õڶ������������Ϊ��0.0��-1.0��

  glColor3f(0.0, 0.0, 1.0); //���õ���������Ϊ��ɫ
  glVertex2f(-0.5, 1.0); //���õ��������������Ϊ��-0.5��1.0��
  glEnd();//�����ν���

  //glEndList();

  return list;
}
#endif


void init(void)
{
  //Object = make_object();
  //glCullFace(GL_BACK);
  /*   glEnable( GL_CULL_FACE );*/
  
  //glDisable(GL_DITHER);
  //glShadeModel(GL_SMOOTH);

  glClearColor(0.0, 0.0, 0.0, 0.0); //���ñ�����ɫΪ��ɫ
}

void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);//buffer����Ϊ��ɫ��д
  //glCallList(Object);
  glBegin(GL_TRIANGLES);//��ʼ��������
  glShadeModel(GL_SMOOTH);//����Ϊ�⻬����ģʽ
  
  glColor3f(1.0, 0.0, 0.0); //���õ�һ������Ϊ��ɫ
  glVertex2f(-1.0, -1.0); //���õ�һ�����������Ϊ��-1.0��-1.0��
  
  glColor3f(0.0, 1.0, 0.0); //���õڶ�������Ϊ��ɫ
  glVertex2f(0.0, -1.0); //���õڶ������������Ϊ��0.0��-1.0��
  
  glColor3f(0.0, 0.0, 1.0); //���õ���������Ϊ��ɫ
  glVertex2f(-0.5, 1.0); //���õ��������������Ϊ��-0.5��1.0��
  glEnd();//�����ν���
  
  glFlush();//ǿ��OpenGL����������ʱ��������
  //glutSwapBuffers();
}

void idle(void)
{
  draw();
}

void reshape(int width, int height)
{
  GLfloat  h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#if 0
  glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
#else
  glOrtho(-1.0, 1.0, -1, 1, 5.0, 60.0);
#endif
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLenum key(int k, GLenum mask)
{
  switch (k) {
  case GLUT_KEY_F1:
    exit(0);
  }

  return GL_FALSE;
}

int test_tri(int argc, char** argv)
{
  /*��ʼ��*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(200, 200);

  /*��������*/
  glutCreateWindow("Triangle");

  /*��������ʾ*/
  //background();
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);

  glutMainLoop();
  return(0);
}


#if 0
ͼһ��һ����ɫ��������

���ȴ������̣��䲽�����£�

1������һ��Win32 Console Application��

2������OpenGL libraries����Visual C++���ȵ���Project���ٵ���Settings�����ҵ�Link�����������Object / library modules ����ǰ�����OpenGL32.lib GLu32.lib GLaux.lib

3������Project Settings�е�C / C++��ǩ����Preprocessor definitions �е�_CONSOLE��Ϊ__WINDOWS����󵥻�OK��

��������԰���������ӿ�����������ȥ���������С�����Կ���һ����ɫ�������Ρ�

�����ȿ���main��������������glut��ͷ�ĺ�����������glut.h�С�GLUT��ĺ�����Ҫִ���紦��ര�ڻ��ơ�����ص������¼������ɲ��ʽ�����˵�������λͼ����ͱʻ����壬�Լ����ִ��ڹ��������

glutInit������ʼ��GLUT�Ⲣͬ����ϵͳ�Ի�Э�̡�

glutInitDisplayMode����ȷ�����������ڵ���ʾģʽ�������еĲ���GLUT_SINGLE ָ�������洰�ڣ���Ҳ��ȱʡģʽ����Ӧ��ģʽΪGLUT_DOUBLE ˫���洰�ڡ�����GLUT_RGBָ����ɫRGBAģʽ����Ҳ��ȱʡģʽ����Ӧ��ģʽΪGLUT_INDEX ��ɫ����ģʽ���ڡ�

glutInitWindowSize ��ʼ�����ڵĴ�С����һ������Ϊ���ڵĿ�ȣ��ڶ�������Ϊ���ڵĸ߶ȣ�������Ϊ��λ��

glutInitWindowPosition ���ó�ʼ���ڵ�λ�ã���һ������Ϊ�������Ͻ�x�����꣬�ڶ�������Ϊ�������Ͻ�y�����꣬������Ϊ��λ����Ļ�����Ͻǵ�����Ϊ��0��0�������������������ӣ����������������ӡ�

glutCreateWindow �������㴰�ڣ����ڵ�����Ϊ�����еĲ�����

background() �����Լ�д�ĺ��������ñ�������ʵ��������е�������д��display�����У���Ϊ��ʹ���ܿ�������������԰ѱ�����һ���ֵ����������

glutReshapeFunc ע�ᵱǰ���ڵ���״�仯�ص����������ı䴰�ڴ�Сʱ���ô��ڵ���״�ı�ص������������á��ڴ����о���myReshapeָ����״�仯������

glutDisplayFunc ע�ᵱǰ���ڵ���ʾ�ص���������һ�����ڵ�ͼ�����Ҫ���»���ʱ��GLUT�����øô��ڵĵ���ʾ�ص��������ڴ����е�mydisplay������ʾ�ص���������ʾ�ص����������κβ���������������ͼ���Ļ��ơ����ǵĴ󲿷ֹ�������������������С�

glutMainLoop ����GLUT�¼�����ѭ����glutMainLoop������GLUT���������ֻ�ܵ���һ�Σ���һ�������þͲ��ٷ��أ����ҵ���ע����Ļص�������������������������ע��ص������ĺ��棬������ΪglutReshapeFunc�� glutDisplayFunc��


#endif
