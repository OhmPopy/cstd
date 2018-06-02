#include <GL/glut.h>
#include <math.h>
void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
  glFlush();
}
const GLfloat Pi = 3.1415926536f;
// ��һ��Բ
/*
���ı��Σ�������Σ��������Σ�������ֱ����n���Σ���nԽ��ʱ�����ͼ�ξ�Խ�ӽ�Բ
��n��һ���̶Ⱥ����۽��޷�������������Բ������
��ʱ�����Ѿ��ɹ��Ļ�����һ����Բ��
��ע����Բ�ķ����ܶ࣬����ʹ�õ��ǱȽϼ򵥣���Ч�ʽϵ͵�һ�֣�
���޸������const int n��ֵ���۲쵱n=3,4,5,8,10,15,20,30,50�Ȳ�ͬ��ֵʱ����ı仯���
��GL_POLYGON��ΪGL_LINE_LOOP��GL_POINTS��������ʽ���۲�����ı仯���
*/
void myDisplay1(void)
{
  int i, n = 60;
  double R = 0.5f;;
  glClear(GL_COLOR_BUFFER_BIT);
  //glBegin(GL_POLYGON);
  //glBegin(GL_POINTS);
  glBegin(GL_LINE_LOOP);
  for (i = 0; i < n; ++i) {
    glVertex2f(R * cos(2 * Pi / n * i), R * sin(2 * Pi / n * i));
  }
  glEnd();
  glFlush();
}
// ��һ�������
/*
������ǵ��������ֲ�λ�ù�ϵ���£�
      A
E        B
    D    C
���ȣ��������Ҷ����з��̣���������ǵ����ĵ�����ľ���a
����������Ƕ�Ӧ������εı߳�Ϊ.0��
a = 1 / (2-2*cos(72*Pi/180));
Ȼ�󣬸������Һ����ҵĶ��壬����B��x����bx��y����by���Լ�C��y����
����������ǵ�����������ԭ�㣩
bx = a * cos(18 * Pi/180);
by = a * sin(18 * Pi/180);
cy = -a * cos(18 * Pi/180);
����������Ϳ���ͨ�������ĸ�����һЩ�����򵥵ı�ʾ����
*/
void myDisplay2(void)
{
  GLfloat a = 1 / (2 - 2 * cos(72 * Pi / 180));
  GLfloat bx = a * cos(18 * Pi / 180);
  GLfloat by = a * sin(18 * Pi / 180);
  GLfloat cy = -a * cos(18 * Pi / 180);
  GLfloat
  PointA[2] = { 0, a },
      PointB[2] = { bx, by },
          PointC[2] = { 0.5, cy },
              PointD[2] = { -0.5, cy },
                  PointE[2] = { -bx, by };
  glClear(GL_COLOR_BUFFER_BIT);
  // ����A->C->E->B->D->A��˳�򣬿���һ�ʽ�����ǻ���
  glBegin(GL_LINE_LOOP);
  glVertex2fv(PointA);
  glVertex2fv(PointC);
  glVertex2fv(PointE);
  glVertex2fv(PointB);
  glVertex2fv(PointD);
  glEnd();
  glFlush();
}
//�������Һ�����ͼ��
/*
����OpenGLĬ������ֵֻ�ܴ�-1��1���������޸ģ������������Ժ󽲣�
������������һ������factor�������е�����ֵ�ȱ�����С��
�����Ϳ��Ի����������������
���޸�factor��ֵ���۲�仯���
*/
#include <math.h>
const GLfloat factor = 0.1f;
void myDisplay3(void)
{
  GLfloat x;
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINES);
  glVertex2f(-1.0f, 0.0f);
  glVertex2f(1.0f, 0.0f);         // ������������Ի�x��
  glVertex2f(0.0f, -1.0f);
  glVertex2f(0.0f, 1.0f);         // ������������Ի�y��
  glEnd();
  glBegin(GL_LINE_STRIP);
  for (x = -1.0f / factor; x < 1.0f / factor; x += 0.01f) {
    glVertex2f(x * factor, sin(x)*factor);
  }
  glEnd();
  glFlush();
}
/*���ڵ�
��Ĵ�СĬ��Ϊ1�����أ���Ҳ���Ըı�֮���ı������ΪglPointSize���亯��ԭ�����£�
void glPointSize(GLfloat size);
size�������0.0f��Ĭ��ֵΪ1.0f����λΪ�����ء���
ע�⣺���ھ����OpenGLʵ�֣���Ĵ�С���и��޶ȵģ�������õ�size�������ֵ�������ÿ��ܻ������⡣
*/
void myDisplay4(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(5.999f); //ZB_plot
  glBegin(GL_POINTS);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f, 0.5f);
  glEnd();
  glFlush();
}
void myDisplay5(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(2, 0x0F0F);
  glLineWidth(10.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.5f, 0.5f);
  glEnd();
  glFlush();
}
void myDisplay6(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT, GL_LINE);  // ��������Ϊ���ģʽ
  glPolygonMode(GL_BACK, GL_LINE);   // ���÷���Ϊ����ģʽ
  glFrontFace(GL_CCW);               // ������ʱ�뷽��Ϊ����
  glBegin(GL_POLYGON);               // ����ʱ�����һ�������Σ������·�
  glVertex2f(-0.5f, -0.5f);
  glVertex2f(0.0f, -0.5f);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(-0.5f, 0.0f);
  glEnd();
  glBegin(GL_POLYGON);               // ��˳ʱ�����һ�������Σ������Ϸ�
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.5f);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(0.5f, 0.0f);
  glEnd();
  glFlush();
}
// һֻ��Ӭ��
//ʹ��glPolygonStipple�������οյ���ʽ��
//���еĲ���maskָ��һ������Ϊ128�ֽڵĿռ䣬
//����ʾ��һ��32*32�ľ���Ӧ������� �ա�
//���У���һ���ֽڱ�ʾ�������·��Ĵ����ң�Ҳ�����Ǵ��ҵ�����������޸ģ�
//8�������Ƿ��οգ�1��ʾ���οգ���ʾ�����أ�0��ʾ�οգ���ʾ����� ����ɫ����
//���һ���ֽڱ�ʾ�������Ϸ���8�������Ƿ��οա�
static GLubyte Mask[128] = {
  0x00, 0x00, 0x00, 0x00,    //   �����������һ��
  0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0x01, 0xC0,    //   ��
  0x06, 0xC0, 0x03, 0x60,    //   ��
  0x04, 0x60, 0x06, 0x20,    //   ��
  0x04, 0x30, 0x0C, 0x20,    //   ��
  0x04, 0x18, 0x18, 0x20,    //   ʼ
  0x04, 0x0C, 0x30, 0x20,    //   ��
  0x04, 0x06, 0x60, 0x20,    //   ��
  0x44, 0x03, 0xC0, 0x22,    //   ��
  0x44, 0x01, 0x80, 0x22,    //   ��
  0x44, 0x01, 0x80, 0x22,    //   ��
  0x44, 0x01, 0x80, 0x22,    //   ʹ
  0x44, 0x01, 0x80, 0x22,    //   ��
  0x44, 0x01, 0x80, 0x22,
  0x44, 0x01, 0x80, 0x22,
  0x66, 0x01, 0x80, 0x66,
  0x33, 0x01, 0x80, 0xCC,
  0x19, 0x81, 0x81, 0x98,
  0x0C, 0xC1, 0x83, 0x30,
  0x07, 0xE1, 0x87, 0xE0,
  0x03, 0x3F, 0xFC, 0xC0,
  0x03, 0x31, 0x8C, 0xC0,
  0x03, 0x3F, 0xFC, 0xC0,
  0x06, 0x64, 0x26, 0x60,
  0x0C, 0xCC, 0x33, 0x30,
  0x18, 0xCC, 0x33, 0x18,
  0x10, 0xC4, 0x23, 0x08,
  0x10, 0x63, 0xC6, 0x08,
  0x10, 0x30, 0x0C, 0x08,
  0x10, 0x18, 0x18, 0x08,
  0x10, 0x00, 0x00, 0x08    // �����������һ��
};
void myDisplay7(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_POLYGON_STIPPLE);
  glPolygonStipple(Mask);
  glRectf(-0.5f, -0.5f, 0.0f, 0.0f);   // �����·�����һ�����ο�Ч����������
  glDisable(GL_POLYGON_STIPPLE);
  glRectf(0.0f, 0.0f, 0.5f, 0.5f);     // �����Ϸ�����һ�����ο�Ч����������
  glFlush();
}
#if 0
void myDisplay8(void)
{
  int i;
  for (i = 0; i < 8; ++i) {
    auxSetOneColor(i, (float)(i & 0x04), (float)(i & 0x02), (float)(i & 0x01));
  }
  glShadeModel(GL_FLAT);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0.0f, 0.0f);
  for (i = 0; i <= 8; ++i) {
    glIndexi(i);
    glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
  }
  glEnd();
  glFlush();
}
#endif
void myDisplay9(void)
{
  int i;
  // glShadeModel(GL_FLAT);
  //glShadeModel(GL_SMOOTH);//����Ϊ�⻬����ģʽ
  glClear(GL_COLOR_BUFFER_BIT);
  if (0) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); //���õ�һ������Ϊ��ɫ
    glVertex2f(-1.0, -1.0); //���õ�һ�����������Ϊ��-1.0��-1.0��
    glColor3f(0.0, 1.0, 0.0); //���õڶ�������Ϊ��ɫ
    glVertex2f(0.0, -1.0); //���õڶ������������Ϊ��0.0��-1.0��
    glColor3f(0.0, 0.0, 1.0); //���õ���������Ϊ��ɫ
    glVertex2f(-0.5, 1.0); //���õ��������������Ϊ��-0.5��1.0��
    glEnd();
  }
  if (0) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for (i = 0; i <= 2; ++i) {
      glColor3f(i & 0x04, i & 0x02, i & 0x01);
      glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();
  }
  else {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    for (i = 0; i <= 8; ++i) {
      glColor3f(i & 0x04, i & 0x02, i & 0x01);
      glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();
  }
  glFlush();
}
void myDisplay10(void)
{
  glColor3f(1, 0, 0);
  drawSphere(0, 0, -20, 5, 32, 8);
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(400, 400);
  glFrustum(-10, 10, -10, 10, 5, 60);
  glutCreateWindow("��һ��OpenGL����");
  glutDisplayFunc(myDisplay10);
  glutMainLoop();
  return 0;
}

