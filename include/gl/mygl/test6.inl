#include <gl/glut.h>
#define WIDTH 400
#define HEIGHT 400
#include <math.h>
#define ColoredVertex(c, v) do{ glColor3fv(c); glVertex3fv(v); }while(0)   //��ξͲ��ý����˰ɡ���
GLfloat angle = 0.0f; //�趨ת��
void myDisplay(void)   //��ͼ����
{
  static int list = 0;
  if (list == 0) {
    // �����ʾ�б����ڣ��򴴽�
    GLfloat   //GLfloatΪOpenGL�õ����������ͣ���C��float����һ��
    PointA[] = { -0.5, -5 * sqrt(5) / 48, sqrt(3) / 6}, //�˴�Ϊ4����������꣬��Ϊʱ3D����ϵ�µģ�����ÿ��������3���������ֱ��ӦX��Y��Z�ᡣ���ڸ��᷽���塭��Ĭ������ĻˮƽΪX����ֱΪY������ΪZ��
        PointB[] = { 0.5, -5 * sqrt(5) / 48, sqrt(3) / 6},
            PointC[] = { 0, -5 * sqrt(5) / 48, -sqrt(3) / 3},
                PointD[] = { 0, 11 * sqrt(6) / 48, 0};
    /*GLfloat
    PointA[] = { 0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
    PointB[] = {-0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
    PointC[] = { 0.0f, -sqrt(6.0f)/12, sqrt(3.0f)/3},
    PointD[] = { 0.0f, sqrt(6.0f)/4, 0};
    */
    GLfloat
    ColorR[] = {1, 0, 0},   //������ɫ���飬ÿ������Ϊһ����ɫ��Ҳ����3����������Ӧ�죬�̣�����������Χ[0��1]��ÿ����ɫ���ɿ�������3����ɫ��ϵõ�����һ�Լ��ı������е���ֵ��������Ч����
        ColorG[] = {0, 1, 0},
            ColorB[] = {0, 0, 1},
                ColorY[] = {1, 1, 0};
    list = glGenLists(1);
    glNewList(list, GL_COMPILE);  //����һ������������������л�ͼ�Ķ�����Ϣ
    glBegin(GL_TRIANGLES);   //��ʼ��ͼ����GL_TRIANGLES����ʾ����������
    // ƽ��ABC
    ColoredVertex(ColorR, PointA);  //����ɫR���Ƶ�A���������ƣ�ColoredVertex���������ڳ���ͷ�����ˡ�
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorB, PointC);
    // ƽ��ACD
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorY, PointD);
    // ƽ��CBD
    ColoredVertex(ColorB, PointC);
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorY, PointD);
    // ƽ��BAD
    ColoredVertex(ColorG, PointB);
    ColoredVertex(ColorR, PointA);
    ColoredVertex(ColorY, PointD);
    glEnd();
    glEndList();  //������ͼ ������ͼ�����
    glEnable(GL_DEPTH_TEST);  //����Ȳ��ԡ�����Ȳ��Ե������ǣ�����ڳ������ж�����壬��������Թ۲��ߵľ��벻ͬ�������ΪԶ��������ô���ʱ��ǰ�����������Ե�ס��������壨û��ɣ���ʹ����������ȸС����������Ȳ��ԣ���ô��ͼ�ᰴ���Ƶ�˳�򣬺���Ƶ����帲��ס�ֻ��Ƶ����塣����Ҫע����ǣ���Ƚ�Ӱ�������غ�ʱ˭��ʾ˭����ʾ������Ӱ����3DЧ����Զ����������Ȼ��䡰С���������ڿռ��е�λ����Ϊ��ά�ġ�
  }
  // �Ѿ���������ʾ�б���ÿ�λ�����������ʱ��������
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //�����ɫ�������Ȼ���
  glPushMatrix();  //��ͼ����ϵ��ջ�����Լ����Ϊ��������λ�ͼʱ���ʵ���ʼλ��
  glRotatef(angle, 1, 0.5, 0);  //��ͼ����ϵ�ƣ�1��0.5��0������תangle�ǣ������Ϊģ���Ƹ�����תangle�ǡ�
  glCallList(list);  //���ö��������������ģ��
  glPopMatrix();  //��ͼ����ϵ��ջ����ص��˸ղŵĻ�ͼ��ʼλ��
  glutSwapBuffers();  //ʹ��˫���棨��һ����ǰ����ʾģ�ͣ���һ���ں�������µ�ģ�ͣ�����ģ�ͻ�����Ϻ��͸���һ��������ʾ�������������ģ����һЩ���Ѿ��������ˣ�����һЩ�㻹�ڻ��Ƶ��������
}
void myIdle(void)
{
  //++angle;
  if (angle >= 360.0f) {
    angle = 0.0f;  //ת�ǳ���360�ȣ��������㡣
  }
  myDisplay();  //����ģ��
  Sleep(10);
}
int main(int argc, char* argv[])
{
  glutInit(&argc, argv); //������ͼ����
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //���û�ͼģʽ
  glutInitWindowPosition(200, 200); //���ô���λ��
  glutInitWindowSize(WIDTH, HEIGHT); //���ô��ڴ�С
  glutCreateWindow("OpenGL ����"); //���ô��ڱ���
  glutDisplayFunc(&myDisplay); //�ظ����ú���
  glutIdleFunc(&myIdle);//�ظ����ú���
  glutMainLoop(); //��ѭ�����������������ظ����ú������ϵ���
  return 0;
}

