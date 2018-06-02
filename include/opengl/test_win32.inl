#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/Glut.h>
// #include <GL/glaux.h>
// #include <GL/glext.h>
// #include <GL/wglext.h>
//#pragma comment(lib,"glu32.lib")
//pragma comment(lib,"glaux.lib")
//pragma comment(lib,"opengl32.lib")
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //���ں���˵��
int test_win32()
{
  HINSTANCE hInstance = 0; //WinMain����˵��
  HINSTANCE hPrevInstance = 0;
  LPSTR lpCmdLine = 0;
  int nCmdShow = 0;
  //�������Pascal�ı��������񣬼��ڳ��򣨺�������ʼ���������б���
  //��ȻC++�ı�������Ƚ���������Ϊ��ʹ�����������,δ�������ַ���
  char lpszClassName[] = "����"; //��������
  char lpszTitle[] = "Win32����ʾ��"; //���ڱ�����
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //��������Ϊȱʡ����
  wndclass.lpfnWndProc = WndProc; //���ڴ�����ΪWndProc
  wndclass.cbClsExtra = 0; //����������չ
  wndclass.cbWndExtra = 0; //����ʵ������չ
  wndclass.hInstance = hInstance; //��ǰʵ�����
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //ʹ��ȱʡͼ��
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); //���ڲ��ü�ͷ���
  wndclass.hbrBackground = NULL; //���ڱ���Ϊ��ɫ
  wndclass.lpszMenuName = NULL; //�������޲˵�
  wndclass.lpszClassName = lpszClassName; //��������Ϊ'����ʵ��'
  if (!RegisterClass(&wndclass)) { //ע�ᴰ�ڣ���ʧ�ܣ��򷢳�����
    MessageBeep(0);
    return FALSE;
  }
  //�������ڲ���
  HWND hwnd; //���ڽṹ
  hwnd = CreateWindow(lpszClassName, //�������ڣ���������
      lpszTitle, //����ʵ���ı�����
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, //���ڵķ��
      CW_USEDEFAULT,
      CW_USEDEFAULT, //�������Ͻ�����Ϊȱʡֵ
      CW_USEDEFAULT,
      CW_USEDEFAULT, //���ڵĸ߶ȺͿ��Ϊȱʡֵ
      NULL, //�˴����޸�����
      NULL, //�˴��������˵�
      hInstance, //Ӧ�ó���ǰ���
      NULL); //��ʹ�ø�ֵ
  ShowWindow(hwnd, nCmdShow); //��ʾ����
  UpdateWindow(hwnd); //�����û���
  // int InitGL(GLvoid);
  MSG msg; //��Ϣ�ṹ
  while (GetMessage(&msg, NULL, 0, 0)) { //��Ϣѭ��
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam; //������ֹʱ������Ϣ���ز���ϵͳ
}
void SetupPixelFormat(HDC hDC) //Ϊ�豸�����������ظ�ʽ
{
  int nPixelFormat; //���ظ�ʽ����
  static PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), //���ݽṹ��С
    1, //�汾�ţ�����Ϊ1
    PFD_DRAW_TO_WINDOW | //֧�ִ���
    PFD_SUPPORT_OPENGL | //֧��OpenGL
    PFD_DOUBLEBUFFER, //֧��˫����
    PFD_TYPE_RGBA, //RGBA��ɫģʽ
    32, //32λ��ɫģʽ
    0, 0, 0, 0, 0, 0, //������ɫΪ����ʹ��
    0, //��alpha����
    0, //����ƫ��λ
    0, //���ۻ�����
    0, 0, 0, 0, //�����ۻ�λ
    16, //16λz-buffer��z���棩��С
    0, //��ģ�建��
    0, //�޸�������
    PFD_MAIN_PLANE, //������ƽ��
    0, //������������
    0, 0, 0
  };
  //���Բ�����ģ
  //ѡ����ƥ������ظ�ʽ����������ֵ
  nPixelFormat = ChoosePixelFormat(hDC, &pfd);
  //���û����豸�����ظ�ʽ
  SetPixelFormat(hDC, nPixelFormat, &pfd);
}
int InitGL(GLvoid) // �˴���ʼ��OpenGL������������
{
  glShadeModel(GL_SMOOTH); // ������Ӱƽ��
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��ɫ����
  glClearDepth(1.0f); // ������Ȼ���
  glEnable(GL_DEPTH_TEST); // ������Ȳ���
  glDepthFunc(GL_LEQUAL); // ������Ȳ��Ե�����
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// ����ϵͳ��͸�ӽ�������
  return TRUE; // ��ʼ�� OK
}
void ChangeSize(int width, int height)
{
  glViewport(0, 0, width, height); // ���õ�ǰ���ӿ�glMatrixMode(GL_PROJECTION);
  glMatrixMode(GL_PROJECTION); // ѡ��ͶӰ����
  glLoadIdentity(); // ����ͶӰ����
  // ����ͶӰģʽΪ͸��ͶӰ
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����������Ȼ�����
  glLoadIdentity(); // ���õ�ǰ��ģ�͹۲����
  glTranslatef(-1.5f, 0.0f, -26.0f); // ���� 1.5 ��λ����������Ļ 6.0
  glBegin(GL_TRIANGLES); // ����������
  glVertex3f(0.0f, 1.0f, 0.0f); // �϶���
  glVertex3f(-1.0f, -1.0f, 0.0f); // ����
  glVertex3f(1.0f, -1.0f, 0.0f); // ����
  glEnd(); // �����λ��ƽ���
  glTranslatef(3.0f, 0.0f, 0.0f); // ����3��λ
  glRotatef(45, 1, 1, 0);
  glutWireCube(2);
  //�˴���ӻ�ͼ����
  //glutSwapBuffers();
  // glFlush();
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static HGLRC hRC; //���ƻ���
  static HDC hDC; //�豸����
  switch (message) {
  case WM_CREATE: {
    hDC = GetDC(hwnd); //�õ���ǰ���ڵ��豸����
    SetupPixelFormat(hDC); //�������ظ�ʽ���ú���
    hRC = wglCreateContext(hDC); //����OpenGL��ͼ����������һ��ָ��OpenGL���ƻ����ľ��
    wglMakeCurrent(hDC, hRC); //�����ݹ����Ļ��ƻ�������ΪOpenGL��Ҫ���л��Ƶĵ�ǰ���ƻ���
    // SetTimer(hwnd, 33, 1, NULL);
    InitGL();
    return 0;
  }
  break;
  case WM_DESTROY: {
    // KillTimer(hwnd, 101);
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hRC);
    PostQuitMessage(0); //���øú�������WM_QUIT��Ϣ
  }
  break;
  case WM_SIZE: {
    //ChangeSize(LOWORD(lParam), HIWORD(lParam));
    ChangeSize(LOWORD(lParam), HIWORD(lParam));
  }
  break;
  // case WM_TIMER:
  // {
  // //IdleFunction();
  // InvalidateRect(hwnd, NULL, FALSE);
  // }
  // break;
  case WM_PAINT: {
    RenderScene();
    SwapBuffers(hDC);
    ValidateRect(hwnd, NULL);
  }
  break;
  // case
  default: //ȱʡ��Ϣ������
    return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return 0;
}

