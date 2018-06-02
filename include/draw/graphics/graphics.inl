
#ifndef _GRAPHICS_INL_
#define _GRAPHICS_INL_

#include "ui/uiwin.inl"

#ifndef WINVER
#define WINVER 0x0400      // Specifies that the minimum required platform is Windows 95 and Windows NT 4.0.
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500    // Specifies that the minimum required platform is Windows 2000.
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410  // Specifies that the minimum required platform is Windows 98.
#endif

#include <windows.h>
#include <tchar.h>

// ��ͼ���ڳ�ʼ������
#define SHOWCONSOLE    1    // ����ͼ�δ���ʱ����������̨����ʾ
#define NOCLOSE        2    // û�йرչ���
#define NOMINIMIZE     4    // û����С������

// ��ɫ
#define  BLACK         0
#define  BLUE          0xAA0000
#define  GREEN         0x00AA00
#define  CYAN          0xAAAA00
#define  RED           0x0000AA
#define  MAGENTA       0xAA00AA
#define  BROWN         0x0055AA
#define  LIGHTGRAY     0xAAAAAA
#define  DARKGRAY      0x555555
#define  LIGHTBLUE     0xFF5555
#define  LIGHTGREEN    0x55FF55
#define  LIGHTCYAN     0xFFFF55
#define  LIGHTRED      0x5555FF
#define  LIGHTMAGENTA  0xFF55FF
#define  YELLOW        0x55FFFF
#define  WHITE         0xFFFFFF

// ������ɫת����
#define BGR(color)  ( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )


// �����ߵ���ʽ
typedef struct LINESTYLE {
  DWORD  style;
  DWORD  thickness;
  DWORD* puserstyle;
  DWORD  userstylecount;
} LINESTYLE;

typedef struct FILLSTYLE {
  int    style;      // �����ʽ
  long    hatch;      // ���ͼ����ʽ
  //IMAGE*  ppattern;   // ���ͼ��
} FILLSTYLE;

// ����ͼ�����
typedef struct IMAGE {
  int width, height;    // ����Ŀ��
  HBITMAP m_hBmp;
  HWND hwnd;
  HDC hdc;
  int m_MemCurX;      // ��ǰ��X����
  int m_MemCurY;      // ��ǰ��Y����
  float m_data[6];
  COLORREF  m_LineColor;    // ��ǰ������ɫ
  COLORREF  m_FillColor;    // ��ǰ�����ɫ
  COLORREF  m_TextColor;    // ��ǰ������ɫ
  COLORREF  m_BkColor;      // ��ǰ������ɫ
  DWORD*    m_pBuffer;      // ��ͼ�����ڴ�

  LINESTYLE  m_LineStyle;    // ������ʽ
  FILLSTYLE  m_FillStyle;    // �����ʽ
} IMAGE;
IMAGE g_win[1] = {0};

int IMAGE_SetDefault(IMAGE* im);            // ����ΪĬ��״̬

int IMAGE_resize(IMAGE* im, int _width, int _height);  // �����ߴ�
#define DEFAULT(x)

static LRESULT CALLBACK WindowProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT ret = 0;
  IMAGE* win = g_win;

  switch (uMsg) {
  case WM_GETMINMAXINFO: {
  }
  break;

  case WM_SIZE: {
    ret = hdc_resize(&g_win->hdc, HIWORD(lParam), LOWORD(lParam), 4);
  }
  break;

  case WM_PAINT:
    if (win) {
      PAINTSTRUCT ps = {0};
      HDC hDC = BeginPaint(hwnd, &ps);
      img_t im[1] = {0};
      hdc_get_image(win->hdc, im);
      imflip(im);
      //BitBlt(hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);
      //ret = win->ctrl->ctrlproc(0, win->ctrl, hwnd, uMsg, (WPARAM)im, lParam);
      //imflip(im);
      BitBlt(hDC, 0, 0, im->w, im->h, win->hdc, 0, 0, SRCCOPY);
      imflip(im);
      EndPaint(hwnd, &ps);
    }

    break;

  case WM_NCLBUTTONDBLCLK: {
    //RECT rc1 = {300, 400, 600, 600};
    //GetWindowRect(hwnd, &rc1);
    //drawDragFrame(rc1);
    //printf("drawDragFrame\n");
    //return 1;
  }
  break;

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    break;

  default:
    //ret = win->ctrl->ctrlproc(0, win->ctrl, hwnd, uMsg, wParam, lParam);
    break;
  }

  if (ret) {
    return ret;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// �ر�ͼ�λ���
void closegraph(void)
{
  if (g_win->hwnd) {
    ShowWindow(g_win->hwnd, SW_HIDE);
    SendMessage(g_win->hwnd, WM_CLOSE, 0, 0);
    g_win->hwnd = NULL;
  }

  if (g_win->hdc) {
    DeleteObject(SelectObject(g_win->hdc, 0));
    DeleteDC(g_win->hdc);
    g_win->hdc = NULL;
  }

  return ;
}

// ��ͼģʽ��غ���
HWND initgraph(int width, int height, int flag DEFAULT(0))
{
  static int inited = 0;

  if (!inited) {
    g_hInstance = GetModuleHandle(0);
    // first fill in the window class stucture
    g_winclass.cbSize = sizeof(WNDCLASSEX);
    g_winclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    g_winclass.lpfnWndProc = WindowProc1;
    g_winclass.cbClsExtra = 0;
    g_winclass.cbWndExtra = 0;
    g_winclass.hInstance = g_hInstance;
    g_winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    g_winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    //g_winclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    g_winclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    g_winclass.lpszMenuName = NULL;
    g_winclass.lpszClassName = MYWINDOWCLASSNAME;
    g_winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    //register the window class
    if (!RegisterClassEx(&g_winclass)) {
      MessageBox(NULL, "Class Registration Failed!", "Error", 0);
      //exit the application
      return 0;
    }

    inited = 1;
    atexit(closegraph);
  }

  ASSERT(0 == g_win->hdc);
  ASSERT(0 == g_win->hwnd);
  {
    DWORD dwStyleEx = 0;
    DWORD dwStyle = MYWS_WINDOW;
    IRECT rc = iRECT(0, 0, width, height);
    hdc_resize(&g_win->hdc, height, width, 4);
    g_win->hwnd = CreateWindowEx(dwStyleEx, MYWINDOWCLASSNAME, "test", dwStyle,
        rc.l, rc.t, width, height, 0, 0, g_hInstance, 0); // creation parameters
    ShowWindow(g_win->hwnd, SW_SHOW);
  }
  return g_win->hwnd;
}  // ��ʼ��ͼ�λ���

// ��ͼ��������

int cleardevice();          // ����
int setcliprgn(HRGN hrgn);      // ���õ�ǰ��ͼ�豸�Ĳü���
int clearcliprgn();        // ����ü�������Ļ����

int getlinestyle(LINESTYLE* pstyle);            // ��ȡ��ǰ������ʽ
int setlinestyle(const LINESTYLE* pstyle) {      // ���õ�ǰ������ʽ
  g_win->m_LineStyle = *pstyle;
  return 0;
}
int setlinestyle2(int style, int thickness DEFAULT(1), const DWORD* puserstyle DEFAULT(NULL), DWORD userstylecount DEFAULT(0)) {  // ���õ�ǰ������ʽ
  LINESTYLE pstyle[1] = {0};
  pstyle->style = style;
  pstyle->thickness = thickness;
  pstyle->puserstyle = (DWORD*)puserstyle;
  pstyle->userstylecount = userstylecount;
  
  return 0;
}
int getfillstyle(FILLSTYLE* pstyle);            // ��ȡ��ǰ�����ʽ
int setfillstyle(const FILLSTYLE* pstyle);      // ���õ�ǰ�����ʽ
int setfillstyle2(int style, long hatch DEFAULT(NULL), IMAGE* ppattern DEFAULT(NULL));    // ���õ�ǰ�����ʽ
int setfillstyle3(BYTE* ppattern8x8);           // ���õ�ǰ�����ʽ

int setorigin(int x, int y);              // ��������ԭ��
int getaspectratio(float* pxasp, float* pyasp);  // ��ȡ��ǰ��������
int setaspectratio(float xasp, float yasp);      // ���õ�ǰ��������

int getrop2();                 // ��ȡǰ���Ķ�Ԫ��դ����ģʽ
int setrop2(int mode);        // ����ǰ���Ķ�Ԫ��դ����ģʽ
int getpolyfillmode();         // ��ȡ��������ģʽ
int setpolyfillmode(int mode);    // ���ö�������ģʽ

int graphdefaults();        // �������л�ͼ����ΪĬ��ֵ

COLORREF getlinecolor();      // ��ȡ��ǰ������ɫ
int setlinecolor(COLORREF color);  // ���õ�ǰ������ɫ
COLORREF gettextcolor();      // ��ȡ��ǰ������ɫ
int settextcolor(COLORREF color);  // ���õ�ǰ������ɫ
COLORREF getfillcolor();      // ��ȡ��ǰ�����ɫ
int setfillcolor(COLORREF color)
{
  g_win->m_FillColor = SWAPRB(color);
  return 0;
}  // ���õ�ǰ�����ɫ
COLORREF getbkcolor();        // ��ȡ��ǰ��ͼ����ɫ
int setbkcolor(COLORREF color);  // ���õ�ǰ��ͼ����ɫ
int getbkmode();          // ��ȡ�������ģʽ
int setbkmode(int mode);      // ���ñ������ģʽ

// ��ɫģ��ת������
COLORREF RGBtoGRAY(COLORREF rgb);
int RGBtoHSLf(COLORREF rgb, float* H, float* S, float* L);
int RGBtoHSVf(COLORREF rgb, float* H, float* S, float* V);
COLORREF HSLtoRGB(float H, float S, float L);
COLORREF HSVtoRGBf(float H, float S, float V);


// ��ͼ����

COLORREF getpixel(int x, int y);        // ��ȡ�����ɫ
int putpixel(int x, int y, COLORREF color);  // ����

int moveto(double x, double y);            // �ƶ���ǰ��(��������)
int moverel(double dx, double dy);          // �ƶ���ǰ��(�������)

int line(double x1, double y1, double x2, double y2) {    // ����
  img_t im[1] = {0};
  hdc_get_image(g_win->hdc, im);
  imdrawaa_line(im, 0, x1, y1, x2, y2, g_win->m_LineColor, g_win->m_LineStyle.thickness);
  return 0;
}
int linerel(double dx, double dy);          // ����(���������)
int lineto(double x, double y);            // ����(����������)

int rectangle(double left, double top, double right, double bottom);       // ������
int fillrectangle(double left, double top, double right, double bottom);   // ��������(�б߿�)
int solidrectangle(double left, double top, double right, double bottom);  // ��������(�ޱ߿�)
int clearrectangle(double left, double top, double right, double bottom);  // ��վ�������

int circle(double x, double y, double radius);        // ��Բ
int fillcircle(double x, double y, double radius)
{
  img_t im[1] = {0};
  hdc_get_image(g_win->hdc, im);
  imdrawaa_ellipse(im, 0, x, y, radius, radius, g_win->m_FillColor, 0, 0);
  return 0;
}     // �����Բ(�б߿�)
int solidcircle(double x, double y, double radius);    // �����Բ(�ޱ߿�)
int clearcircle(double x, double y, double radius);    // ���Բ������

int ellipse(double left, double top, double right, double bottom);     // ����Բ
int fillellipse(double left, double top, double right, double bottom);   // �������Բ(�б߿�)
int solidellipse(double left, double top, double right, double bottom);  // �������Բ(�ޱ߿�)
int clearellipse(double left, double top, double right, double bottom);  // �����Բ������

int roundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);         // ��Բ�Ǿ���
int fillroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);     // �����Բ�Ǿ���(�б߿�)
int solidroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);    // �����Բ�Ǿ���(�ޱ߿�)
int clearroundrect(double left, double top, double right, double bottom, int ellipsewidth, int ellipseheight);    // ���Բ�Ǿ�������

int arc(double left, double top, double right, double bottom, double stangle, double endangle);     // ����Բ��(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
int pie(double left, double top, double right, double bottom, double stangle, double endangle);     // ����Բ����(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
int fillpie(double left, double top, double right, double bottom, double stangle, double endangle);   // �������Բ����(�б߿�)
int solidpie(double left, double top, double right, double bottom, double stangle, double endangle);  // �������Բ����(�ޱ߿�)
int clearpie(double left, double top, double right, double bottom, double stangle, double endangle);  // �����Բ��������

int polyline(const POINT* points, int num);     // ��������������
int polygon(const POINT* points, int num);     // �������
int fillpolygon(const POINT* points, int num);   // �����Ķ����(�б߿�)
int solidpolygon(const POINT* points, int num);  // �����Ķ����(�ޱ߿�)
int clearpolygon(const POINT* points, int num);  // ��ն��������

int floodfill(int x, int y, int border);      // �������

// ������غ���

int outtext(LPCTSTR str);          // �ڵ�ǰλ������ַ���
int outchar(TCHAR c);            // �ڵ�ǰλ������ַ�
int outtextxy(int x, int y, LPCTSTR str);  // ��ָ��λ������ַ���
int outcharxy(int x, int y, TCHAR c);    // ��ָ��λ������ַ�
int textwidth(LPCTSTR str);          // ��ȡ�ַ���ռ�õ����ؿ�
int charwidth(TCHAR c);            // ��ȡ�ַ�ռ�õ����ؿ�
int textheight(LPCTSTR str);        // ��ȡ�ַ���ռ�õ����ظ�
int charheight(TCHAR c);          // ��ȡ�ַ�ռ�õ����ظ�
int drawtext(LPCTSTR str, RECT* pRect, UINT uFormat);  // ��ָ����������ָ����ʽ����ַ���
int drawchar(TCHAR c, RECT* pRect, UINT uFormat);    // ��ָ����������ָ����ʽ����ַ�

// ���õ�ǰ������ʽ(�������)
//    nHeight: �ַ���ƽ���߶ȣ�
//    nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//    lpszFace: �������ƣ�
//    nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//    nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//    nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//    bItalic: �Ƿ�б�壻
//    bUnderline: �Ƿ��»��ߣ�
//    bStrikeOut: �Ƿ�ɾ���ߣ�
//    fbCharSet: ָ���ַ�����
//    fbOutPrecision: ָ�����ֵ�������ȣ�
//    fbClipPrecision: ָ�����ֵļ������ȣ�
//    fbQuality: ָ�����ֵ����������
//    fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
int settextstyle(int nHeight, int nWidth, LPCTSTR lpszFace);
int settextstyle2(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int settextstyle3(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
int settextfont(const LOGFONT* font);  // ���õ�ǰ������ʽ
int gettextfont1(LOGFONT* font);    // ��ȡ��ǰ������ʽ



// ͼ������
//int loadimage(IMAGE *pDstImg, LPCTSTR pImgFile, int nWidth DEFAULT(0), int nHeight DEFAULT(0), BOOL bResize DEFAULT(FALSE));          // ��ͼƬ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
//int loadimage(IMAGE *pDstImg, LPCTSTR pResType, LPCTSTR pResName, int nWidth DEFAULT(0), int nHeight DEFAULT(0), BOOL bResize DEFAULT(FALSE));  // ����Դ�ļ���ȡͼ��(bmp/jpg/gif/emf/wmf)
//int saveimage(LPCTSTR pImgFile, IMAGE* pImg DEFAULT(NULL));                                    // ����ͼ��
int getimage(IMAGE* pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);                        // �ӵ�ǰ��ͼ�豸��ȡͼ��
int putimage(int dstX, int dstY, const IMAGE* pSrcImg, DWORD dwRop DEFAULT(SRCCOPY));                        // ����ͼ����Ļ
int putimage2(int dstX, int dstY, int dstWidth, int dstHeight, const IMAGE* pSrcImg, int srcX, int srcY, DWORD dwRop DEFAULT(SRCCOPY));    // ����ͼ����Ļ(ָ�����)
int rotateimage(IMAGE* dstimg, IMAGE* srcimg, double radian, COLORREF bkcolor DEFAULT(BLACK), BOOL autosize DEFAULT(FALSE), BOOL highquality DEFAULT(TRUE));// ��תͼ��
int Resize(IMAGE* pImg, int width, int height);  // ������ͼ�豸�Ĵ�С
DWORD* GetImageBuffer(img_t* im)
{
  hdc_get_image(g_win->hdc, im);
  ASSERT(4 == im->c);
  return (DWORD*)im->tt.data;
}      // ��ȡ��ͼ�豸���Դ�ָ��
IMAGE* GetWorkingImage();              // ��ȡ��ǰ��ͼ�豸
int SetWorkingImage(IMAGE* pImg DEFAULT(NULL));      // ���õ�ǰ��ͼ�豸
HDC GetImageHDC(IMAGE* pImg DEFAULT(NULL));        // ��ȡ��ͼ�豸���(HDC)


// ��������

int getwidth();      // ��ȡ��ͼ�����
int getheight();    // ��ȡ��ͼ���߶�
int getx();        // ��ȡ��ǰ x ����
int gety();        // ��ȡ��ǰ y ����

int BeginBatchDraw()
{
  return 0; // ��ʼ��������
}
int FlushBatchDraw()
{
  InvalidateRect(g_win->hwnd, 0, 0);  // ִ��δ��ɵĻ�������
  return 0;
}
int EndBatchDraw()
{
  return 0; // �����������ƣ���ִ��δ��ɵĻ�������
}

HWND GetHWnd();                // ��ȡ��ͼ���ھ��(HWND)
TCHAR* GetEasyXVer();            // ��ȡ EasyX ��ǰ�汾

// ��ȡ�û�����
BOOL InputBox(LPTSTR pString, int nMaxCount, LPCTSTR pPrompt DEFAULT(NULL), LPCTSTR pTitle DEFAULT(NULL), LPCTSTR pDefault DEFAULT(NULL), int width DEFAULT(0), int height DEFAULT(0), BOOL bOnlyOK DEFAULT(TRUE));



// �����Ϣ
// ֧��������Ϣ��
//    WM_MOUSEMOVE    ����ƶ�
//    WM_MOUSEWHEEL    �����ֲ���
//    WM_LBUTTONDOWN    �������
//    WM_LBUTTONUP    �������
//    WM_LBUTTONDBLCLK  ���˫��
//    WM_MBUTTONDOWN    �м�����
//    WM_MBUTTONUP    �м�����
//    WM_MBUTTONDBLCLK  �м�˫��
//    WM_RBUTTONDOWN    �Ҽ�����
//    WM_RBUTTONUP    �Ҽ�����
//    WM_RBUTTONDBLCLK  �Ҽ�˫��
typedef struct MOUSEMSG {
  UINT uMsg;       // ��ǰ�����Ϣ
  BOOL mkCtrl;     // Ctrl ���Ƿ���
  BOOL mkShift;    // Shift ���Ƿ���
  BOOL mkLButton;  // �������Ƿ���
  BOOL mkMButton;  // ����м��Ƿ���
  BOOL mkRButton;  // ����Ҽ��Ƿ���
  short x;         // ��ǰ��� x ����
  short y;         // ��ǰ��� y ����
  short wheel;     // �����ֹ���ֵ (120 �ı���)
} MOUSEMSG;

BOOL GetMouseMsg(MOUSEMSG* msg, int delay)
{
  int time0 = GetTickCount();

  if (msg) {
    memset(msg, 0, sizeof(*msg));
  }

  for (;;) {
    MSG message;
    int is_processed = 0;

    if (0 == g_win->hwnd) {
      return -2;
    }

    if ((delay > 0 && abs((int)(GetTickCount() - time0)) >= delay)) {
      return -1;
    }

    if (delay <= 0) {
      GetMessage(&message, 0, 0, 0);
    }
    else if (PeekMessage(&message, 0, 0, 0, PM_REMOVE) == FALSE) {
      Sleep(1);
      continue;
    }

    if (is_processed == 0) {
      if (g_win->hwnd == message.hwnd) {
        is_processed = 1;

        switch (message.message) {
        case WM_DESTROY:
        case WM_CHAR:
          DispatchMessage(&message);
          return (int) message.wParam;

        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_MOUSEMOVE:
          if (msg) {
            msg->uMsg = message.message;
            msg->x = GET_X_LPARAM(message.lParam);
            msg->y = GET_Y_LPARAM(message.lParam);
            msg->mkCtrl = !!(MK_CONTROL & message.wParam);
            msg->mkShift = !!(MK_SHIFT & message.wParam);
            msg->mkLButton = !!(MK_LBUTTON & message.wParam);
            msg->mkRButton = !!(MK_RBUTTON & message.wParam);
            msg->mkMButton = !!(MK_MBUTTON & message.wParam);
            is_processed = 1;
          }

          break;

        case WM_SYSKEYDOWN:
          if (message.wParam == VK_F10) {
            is_processed = 1;
            return (int)(message.wParam << 16);
          }

          break;

        case WM_KEYDOWN:
          TranslateMessage(&message);

          if ((message.wParam >= VK_F1 && message.wParam <= VK_F24) ||
              message.wParam == VK_HOME || message.wParam == VK_END ||
              message.wParam == VK_UP || message.wParam == VK_DOWN ||
              message.wParam == VK_LEFT || message.wParam == VK_RIGHT ||
              message.wParam == VK_INSERT || message.wParam == VK_DELETE ||
              message.wParam == VK_PRIOR || message.wParam == VK_NEXT) {
            DispatchMessage(&message);
            is_processed = 1;
            return (int)(message.wParam << 16);
          }

          break;

        default:
          DispatchMessage(&message);
          is_processed = 1;
          break;
        }
      }
    }

    if (!is_processed) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  return 0;
}    // ��ȡһ�������Ϣ�����û�У��͵ȴ�
int FlushMouseMsgBuffer();  // ��������Ϣ������


// ���� initgraph ��ͼģʽ�ĺ궨�壨��ʵ�����壩
#define DETECT  0
#define VGA    0
#define  VGALO  0
#define VGAMED  0
#define VGAHI  0
#define CGA    0
#define  CGAC0  0
#define  CGAC1  0
#define CGAC2  0
#define CGAC3  0
#define CGAHI  0
#define EGA    0
#define EGALO  0
#define  EGAHI  0

int bar(double left, double top, double right, double bottom);    // ���ޱ߿�������
int bar3d(double left, double top, double right, double bottom, int depth, BOOL topflag);  // ���б߿���ά������

int drawpoly(int numpoints, const int* polypoints);  // �������
int fillpoly(int numpoints, const int* polypoints);  // �����Ķ����

int getmaxx();          // ��ȡ���Ŀ��ֵ
int getmaxy();          // ��ȡ���ĸ߶�ֵ

COLORREF getcolor();      // ��ȡ��ǰ��ͼǰ��ɫ
int setcolor(COLORREF color)
{
  g_win->m_LineColor = color;
  return 0;
}  // ���õ�ǰ��ͼǰ��ɫ

int setwritemode(int mode);  // ����ǰ���Ķ�Ԫ��դ����ģʽ




///////////////////////////////////////////////////////////////////////////////////
// ���º�����Ϊ�������� EasyX �Ľӿڣ�������ʹ�á���ʹ������º����滻��
//

// ���õ�ǰ������ʽ(�ú����Ѳ���ʹ�ã���ʹ�� settextstyle ����)
//    nHeight: �ַ���ƽ���߶ȣ�
//    nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//    lpszFace: �������ƣ�
//    nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//    nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//    nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//    bItalic: �Ƿ�б�壻
//    bUnderline: �Ƿ��»��ߣ�
//    bStrikeOut: �Ƿ�ɾ���ߣ�
//    fbCharSet: ָ���ַ�����
//    fbOutPrecision: ָ�����ֵ�������ȣ�
//    fbClipPrecision: ָ�����ֵļ������ȣ�
//    fbQuality: ָ�����ֵ����������
//    fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
int setfont(int nHeight, int nWidth, LPCTSTR lpszFace);
int setfont2(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int setfont3(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
int setfont4(const LOGFONT* font);  // ���õ�ǰ������ʽ
int getfont(LOGFONT* font);      // ��ȡ��ǰ������ʽ

// ���������ʽ����ʹ�ã��µ������ʽ��μ������ļ�
#define  NULL_FILL       BS_NULL
#define  EMPTY_FILL      BS_NULL
#define  SOLID_FILL      BS_SOLID
// ��ͨһ��
#define BDIAGONAL_FILL    BS_HATCHED, HS_BDIAGONAL          // /// ��� (��Ӧ BGI �� LTSLASH_FILL)
#define CROSS_FILL        BS_HATCHED, HS_CROSS            // +++ ���
#define DIAGCROSS_FILL    BS_HATCHED, HS_DIAGCROSS          // xxx ��� (heavy cross hatch fill) (��Ӧ BGI �� XHTACH_FILL)
#define DOT_FILL          (BYTE*)"\x80\x00\x08\x00\x80\x00\x08\x00"  // xxx ��� (��Ӧ BGI �� WIDE_DOT_FILL)  
#define FDIAGONAL_FILL    BS_HATCHED, HS_FDIAGONAL          // \\\ ���
#define HORIZONTAL_FILL   BS_HATCHED, HS_HORIZONTAL          // === ���
#define VERTICAL_FILL     BS_HATCHED, HS_VERTICAL            // ||| ���
// �ܼ�һ��
#define BDIAGONAL2_FILL   (BYTE*)"\x44\x88\x11\x22\x44\x88\x11\x22"
#define CROSS2_FILL       (BYTE*)"\xff\x11\x11\x11\xff\x11\x11\x11"  // (��Ӧ BGI �� fill HATCH_FILL)
#define DIAGCROSS2_FILL   (BYTE*)"\x55\x88\x55\x22\x55\x88\x55\x22"
#define DOT2_FILL         (BYTE*)"\x88\x00\x22\x00\x88\x00\x22\x00"  // (��Ӧ BGI �� CLOSE_DOT_FILL)
#define FDIAGONAL2_FILL   (BYTE*)"\x22\x11\x88\x44\x22\x11\x88\x44"
#define HORIZONTAL2_FILL  (BYTE*)"\x00\x00\xff\x00\x00\x00\xff\x00"
#define VERTICAL2_FILL    (BYTE*)"\x11\x11\x11\x11\x11\x11\x11\x11"
// ����һ��
#define BDIAGONAL3_FILL   (BYTE*)"\xe0\xc1\x83\x07\x0e\x1c\x38\x70"  // (��Ӧ BGI �� SLASH_FILL)
#define CROSS3_FILL       (BYTE*)"\x30\x30\x30\x30\x30\x30\xff\xff"
#define DIAGCROSS3_FILL   (BYTE*)"\xc7\x83\xc7\xee\x7c\x38\x7c\xee"
#define DOT3_FILL         (BYTE*)"\xc0\xc0\x0c\x0c\xc0\xc0\x0c\x0c"
#define FDIAGONAL3_FILL   (BYTE*)"\x07\x83\xc1\xe0\x70\x38\x1c\x0e"
#define HORIZONTAL3_FILL  (BYTE*)"\xff\xff\x00\x00\xff\xff\x00\x00"  // (��Ӧ BGI �� LINE_FILL)  
#define VERTICAL3_FILL    (BYTE*)"\x33\x33\x33\x33\x33\x33\x33\x33"
// ����
#define INTERLEAVE_FILL   (BYTE*)"\xcc\x33\xcc\x33\xcc\x33\xcc\x33"  // (��Ӧ BGI �� INTERLEAVE_FILL)

#endif // _GRAPHICS_INL_
