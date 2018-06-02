

void draw()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);
  //���ָ��ߣ��ֳ��ĸ��Ӽ���
  glViewport(0, 0, 400, 400);
  glBegin(GL_LINES);
  glVertex2f(-1.0, 0);
  glVertex2f(1.0, 0);
  glVertex2f(0.0, -1.0);
  glVertex2f(0.0, 1.0);
  glEnd();

  //���������½ǵ�����
  glColor3f(0.0, 1.0, 0.0);
  glViewport(0, 0, 200, 200);
  glBegin(GL_POLYGON);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glEnd();


  //���������Ͻǵ�����
  glColor3f(0.0, 0.0, 1.0);
  glViewport(200, 200, 200, 200);  //ע�⣬���������������Ǹ߶ȺͿ�ȣ�����������
  glBegin(GL_POLYGON);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glEnd();

  //���������Ͻǵ�����
  glColor3f(1.0, 0.0, 0.0);
  glViewport(0, 200, 200, 200);  //ע�⣬���������������Ǹ߶ȺͿ�ȣ�����������
  glBegin(GL_POLYGON);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glEnd();

  //���������½�
  glColor3f(1.0, 1.0, 1.0);
  glViewport(200, 0, 200, 200);  //ע�⣬���������������Ǹ߶ȺͿ�ȣ�����������
  glBegin(GL_POLYGON);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glEnd();
  glFlush();
  glSwapBuffers();
}

void init()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0, 1.0, 1.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //���������
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void reshape(int width, int height)
{
  GLfloat  h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

void idle(void)
{
  draw();
}

GLenum key(int k, GLenum mask)
{
  switch (k) {
  case KEY_ESCAPE:
    exit(0);
  }

  return GL_FALSE;
}