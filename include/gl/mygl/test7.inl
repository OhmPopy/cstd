// ��OpenGL���ò������̻�������
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
const double pi = 3.1415926;
#define SOLID 3000
#define WIRE  3001
typedef int SPHERE_MODE;
typedef struct Point3f {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} point;
void init(void)
{
  GLfloat _ambient[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat _diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat _specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat _position[] = {200, 200, 200, 0};
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1);
  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_AMBIENT, _ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, _diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, _specular);
  glLightfv(GL_LIGHT0, GL_POSITION, _position);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 500, 0.0, 500, -500, 500);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
int getPoint(GLfloat radius, GLfloat a, GLfloat b, point* p)
{
  p->x = radius * sin(a * pi / 180.0) * cos(b * pi / 180.0);
  p->y = radius * sin(a * pi / 180.0) * sin(b * pi / 180.0);
  p->z = radius * cos(a * pi / 180.0);
  return 1;
}
void drawSlice(point* p1, point* p2, point* p3, point* p4, SPHERE_MODE mode)
{
  switch (mode) {
  case SOLID:
    glBegin(GL_QUADS);
    break;
  case WIRE:
    glBegin(GL_LINE_LOOP);
    break;
  }
  glColor3f(1, 0, 0);
  glVertex3f(p1->x, p1->y, p1->z);
  glVertex3f(p2->x, p2->y, p2->z);
  glVertex3f(p3->x, p3->y, p3->z);
  glVertex3f(p4->x, p4->y, p4->z);
  glEnd();
}
point* getPointMatrix(GLfloat radius, GLint slices)
{
  int i, j, w = 2 * slices, h = slices;
  float a = 0.0, b = 0.0;
  float hStep = 180.0 / (h - 1);
  float wStep = 360.0 / w;
  int length = w * h;
  point* matrix;
  matrix = (point*)malloc(length * sizeof(point));
  if (!matrix) {
    return NULL;
  }
  for (a = 0.0, i = 0; i < h; i++, a += hStep) {
    for (b = 0.0, j = 0; j < w; j++, b += wStep) {
      getPoint(radius, a, b, &matrix[i * w + j]);
    }
  }
  return matrix;
}
int drawSphere1(GLfloat radius, GLint slices, SPHERE_MODE mode)
{
  int i = 0, j = 0, w = 2 * slices, h = slices;
  point* mx;
  mx = getPointMatrix(radius, slices);
  if (!mx) {
    return 0;
  }
  for (; i < h - 1; i++) {
    for (j = 0; j < w - 1; j++) {
      drawSlice(&mx[i * w + j], &mx[i * w + j + 1], &mx[(i + 1)*w + j + 1], &mx[(i + 1)*w + j], mode);
    }
    drawSlice(&mx[i * w + j], &mx[i * w], &mx[(i + 1)*w], &mx[(i + 1)*w + j], mode);
  }
  free(mx);
  return 1;
}
void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glTranslated(250, 250, 0);
  glRotatef(30, 1, 0, 0);
  glRotatef(60, 0, 1, 0);
  glRotatef(90, 0, 0, 1);
  glColor3f(1.0, 1.0, 1.0);
  drawSphere1(200, 20, WIRE);
  //drawSphere1(200, 20, SOLID);
  glFlush();
}
int main(int n, char** s)
{
  glutInit(&n, s);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Sphere");
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
#if 0
д����дע�ͣ�����ֻ��Ӣ�ģ�������ҵķ�������б�Ҫ������Ķ�������һ�¡�
����Ҫ���������������������ĳ�������ļ�����ϱ���װ��GLUT�������ߣ���Ҫ��GLUT��ͷ�ļ��Ϳ��ļ���ӵ��㿪��������ͷ�ļ��Ϳ��ļ���Ŀ¼�У������ñ��������ҵ�GLUT�Ķ�̬���ӿ⣬��GLUT32.DLL��������μ�GLUT�İ�װ��������
�������⣺
1 ͷ�ļ�
����Sphere.h��������windows.hͷ�ļ���һЩ���������õ��ı�׼Cͷ�ļ�������ֱ��д��Sphere.cpp���档��������Sphere.cpp�������Sphere.h��GLUT��ͷ�ļ�glut.h
2 �Զ���ꡢ���ͺͽṹ��
#define pi 3.1415926
����Բ����pi�������������ʱҪ�õ�
#define SOLID 3000
#define WIRE  3001
���廭���ģʽ��SOLID��ʾ��ʵ����WIRE��ʾ��������
typedef int SPHERE_MODE;
���廭��ģʽ�����ͣ���ʵ�Ǹ�int����
typedef struct Point3f {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} point;
�����¼�ռ������Ľṹ��point��GLfloat������ʵ����GLUT���涨���float����
3 ����������ʵ��
void init(void);
void reshape(int w, int h);
void display(void);
int getPoint(GLfloat radius, GLfloat a, GLfloat b, point& p);
void drawSlice(point& p1, point& p2, point& p3, point& p4, SPHERE_MODE mode);
point* getPointMatrix(GLfloat radius, GLint slices);
int drawSphere1(GLfloat radius, GLint slices, SPHERE_MODE mode);
init()��reshape()��display()��3��������OpenGL��ͼ��һ��������õĺ�������3�������ĺ��������Բ�ͬ�����ββ��ֵ��β����ͱ���һ����ע��������3���������������������ε��õġ�
��ע�� void reshape(int w, int h);
Ҳ���� void resizeWindow(int width, int height)��
��init()������ֱ����Ļ������ɫ��ͶӰģʽ�����գ���ԭλ�õȽ��������ã������������ƹ����Ȳ��ԣ�
reshape()�������棬���ӿں�ͶӰ����������ã�
��������display()���������ƶ�ԭ�����굽��������λ�ã��ٶ���Ҫ���Ƶ�ͼ�ν���һ���Ƕȵ���ת�����û�ͼ����ɫ��Ȼ�����drawSphere1()�������塣
����init()��reshape()��display()��3����������ľ�����̣���μ�OpenGL����ؽ̡̳�
����ļ��������ǻ����������õģ������ȼ��Ը������ǵ����ã�����ʵ�ֽ�����һ������ϸ������
int getPoint(GLfloat radius, GLfloat a, GLfloat b, point& p) ������
���ݰ뾶radius��a�ǣ��뾶��Z������ļнǣ���b�ǣ��뾶��xyƽ���ͶӰ��x������ļнǣ���������ϵ�����겢��¼��point���͵Ĳ���p�С�
void drawSlice(point& p1, point& p2, point& p3, point& p4, SPHERE_MODE mode) ������
����ʵ�ʵ���ʱ�ṩ��4����������ڿռ仭�ı��Σ�����mode����ȷ�������ı����ǿ��Ļ���ʵ�ĵġ�
point* getPointMatrix(GLfloat radius, GLint slices) ������
�����ṩ�İ뾶radius�ͷֿ���slices�����һϵ�������ϵĵ�����꣬������һ����̬�����ľ����У�������ָ��þ����ָ�룬Ҫ��ķֿ���Խ�࣬����ĵ��Խ�࣬�����Խ�⻬������Ҳ��Խ��
int drawSphere1(GLfloat radius, GLint slices, SPHERE_MODE mode) ������
���������õĺ����������ṩ�İ뾶radius�ͷֿ���slices�ڴ����л������壬�ֿ���Խ������Խ�⻬��mode��������ȷ��Ҫ����ʵ������������
(����д������
4 ���򲿷�
������Ƶ�����ʵ�Ǹ����档�����濴�����ɺܶ��С���ı���ƽ�湹�ɵģ������Ϳ���ͨ��������ЩС���ı���ƽ���������������档�������������֣���Ҫ�������ν��������˷��������������Щ�����Ϳ����������������غϵ��ı��Σ�����͵õ���һ���ļ򻯡����ֻ��֪��������һЩ�е�Ŀռ����꣬�Ϳ���������Щ���������ı��δӶ��õ��������档����������չ�����ɼ���������Щ�㹹�ɵ���һ�����󣬶�����������ݺ����ڵ��ĸ�Ԫ�ص��������ù��ɵľ��������ϵ�һ��Сƽ�档
��λ�ȡ�����ϵ�����ꣿ
������Ŀռ������������
x = r��sin(��)��cos(��)
y = r��sin(��)��sin(��)
z = r��cos(��)
r����İ뾶�������ǰ뾶��Z������ļнǣ��½��ǰ뾶��xyƽ���ͶӰ��x������ļнǣ����ǵ�ȡֵ��Χ��
0��r�ܡ�    0�ܦ��ܦ�    0�ܦ¡�2��
��˺��� getPoint() ����ͨ���˲�����������ÿռ������ꣻ
int getPoint(GLfloat radius, GLfloat a, GLfloat b, point& p)
{
  p.x = radius * sin(a * pi / 180.0) * cos(b * pi / 180.0);
  p.y = radius * sin(a * pi / 180.0) * sin(b * pi / 180.0);
  p.z = radius * cos(a * pi / 180.0);
  return 1;
}
ֵ��ע����� sin() �� cos() �����Ĳ����ǻ��Ƚǣ����βδ�����ǽǶȽǣ������Ҫ����ת����
��ȡ�����������
�β�slicesȷ��ȡ��ļ����С��
���ǵĴ�С��0~180�ȣ��½ǵĴ�С��0~360�ȣ����������z�᷽��ÿ��180 / (slice - 1)ȡһ���еĵ���Ϊ���ǣ��ڴ�ֱ��z���ƽ���ϣ�ÿ��360 / sliceȡһ���еĵ���Ϊ�½ǡ����ڦ½ǵķ�Χ�Ǧ��ǵ����������Ծ������ȡ��ĸ���������ȡ������������������Ƕ�̬�����ڴ�ռ䣬Ȼ��ͨ��ѭ��Ϊ�����е�Ԫ�أ��������ϵĵ�����긳ֵ��������� getPoint() �������������ռ�����ꡣ��Ȼ��������Ŀռ������Եģ�������ͨ��Ԫ�ص��±����ȷ����Ԫ�������Կռ��λ�á�
point* getPointMatrix(GLfloat radius, GLint slices)
{
  int i, j, w = 2 * slices, h = slices;
  float a = 0.0, b = 0.0;
  float hStep = 180.0 / (h - 1);
  float wStep = 360.0 / w;
  int length = w * h;
  point* matrix;
  matrix = (point*)malloc(length * sizeof(point));
  if (!matrix) {
    return NULL;
  }
  for (a = 0.0, i = 0; i < h; i++, a += hStep)
    for (b = 0.0, j = 0; j < w; j++, b += wStep) {
      getPoint(radius, a, b, matrix[i * w + j]);
    }
  return matrix;
}
�ĵ����
void drawSlice(point& p1, point& p2, point& p3, point& p4, SPHERE_MODE mode)
{
  switch (mode) {
  case SOLID:
    glBegin(GL_QUADS);
    break;
  case WIRE:
    glBegin(GL_LINE_LOOP);
    break;
  }
  glColor3f(1, 0, 0);
  glVertex3f(p1.x, p1.y, p1.z);
  glVertex3f(p2.x, p2.y, p2.z);
  glVertex3f(p3.x, p3.y, p3.z);
  glVertex3f(p4.x, p4.y, p4.z);
  glEnd();
}
�β�modeȷ���˻�ͼ��ģʽ������ΪSOLIDʱ����GL_QUADSģʽ��ʵƽ�棬��ΪWIREʱ������
GL_LINE_LOOP������β�����������ߡ�ע�����Ҫ�� glEnd() �����Ը�ƽ��Ļ��ƣ������õ������족�Ľ����
��������
int drawSphere1(GLfloat radius, GLint slices, SPHERE_MODE mode)
{
  int i = 0, j = 0, w = 2 * slices, h = slices;
  point* mx;
  mx = getPointMatrix(radius, slices);
  if (!mx) {
    return 0;
  }
  for (; i < h - 1; i++) {
    for (j = 0; j < w - 1; j++) {
      drawSlice(mx[i * w + j], mx[i * w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j], mode);
    }
    drawSlice(mx[i * w + j], mx[i * w], mx[(i + 1)*w], mx[(i + 1)*w + j], mode);
  }
  free(mx);
  return 1;
}
����ǻ������������ܺ����ˣ��ȵ��� getPointMatrix() �������������󣬲��þֲ�����mx��¼�þ������ڴ��е�λ�á����������ǰѸþ����ϼ�¼�ĵ��� drawSlice() �������ƽ�档����mx[0, 0]��mx[0, 1]��mx[1, 1]��mx[1, 0]���ĸ�Ԫ���м�¼�ľ�������ʱ�뷽�򹹳������ϵ�һ��ƽ���4�����ˡ�Ҫע����ǣ��ھ���ĺ����ϣ����һ�е���Ҫ�͵�һ�е��������ٻ���ƽ�棬����������������Ƿ�յġ��ں������Ҫ�Ѷ�̬������ڴ�ռ��ͷŵ�����Ȼ������ڴ�й¶��������Ƴɹ�����������1�����򷵻�0��
������д����β�ˡ�����
�������漸�������Ϳ�����ֱ�ӵ��� drawSphere1() ���������Ʋ�ͬ��С����ͬ���ȺͲ�ͬģʽ�����ˡ�
#endif

