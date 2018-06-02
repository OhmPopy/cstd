#include "imui.inl"
// ��ɫ
#define BLACK 0
#define BLUE 0xAA0000
#define GREEN 0x00AA00
#define CYAN 0xAAAA00
#define RED 0x0000AA
#define MAGENTA 0xAA00AA
#define BROWN 0x0055AA
#define LIGHTGRAY 0xAAAAAA
#define DARKGRAY 0x555555
#define LIGHTBLUE 0xFF5555
#define LIGHTGREEN 0x55FF55
#define LIGHTCYAN 0xFFFF55
#define LIGHTRED 0x5555FF
#define LIGHTMAGENTA 0xFF55FF
#define YELLOW 0x55FFFF
#define WHITE 0xFFFFFF
// ������ɫת����
#define EASYX_COLOR(color) ( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) | (0xFF000000) )
//#define EASYX_COLOR(color) (color)
typedef struct {
  ImGuiHost host[1];
  COLOR m_BkColor; // ��ǰ������ɫ
  gc_t ctx[1];
} easyx_t;
easyx_t g_easyx[1] = {0};
int initgraph(int width, int height, int flag DEFAULT(0))
{
  ImGuiDriverInit_t drv_init;
  drv_init = gl2_Init;
  drv_init = d3d9_Init;
  drv_init = gdip_Init;
  drv_init = gdi_Init;
  drv_init = soft_Init;
  ImGuiHost_Init(g_easyx->host, drv_init, "easyx", 100, 100, width, height);
  return 0;
}
int _getch()
{
  for (; imuiLoop(50);) {
    GETGUIIO2();
    if (host_begin(g_easyx->host, g_easyx->m_BkColor)) {
      gc_t* ctx = g_easyx->host->rootWindow->ctx;
      char* ptr = (char*)myAllocPush(ctx->buf, ctx->len, g_easyx->ctx->len);
      memcpy(ptr, g_easyx->ctx->buf, g_easyx->ctx->len);
      //g_easyx->ctx->len = 0;
      if (MSG_KEYDOWN == io->lastMsg) {
        break;
      }
      host_end();
    }
  }
  return 0;
}
int closegraph()
{
  imuiUnInit();
  return 0;
}
// ��ͼ��������
// ����
int cleardevice()
{
  return 0;
}
int setcliprect(int x, int y, int w, int h); // ���õ�ǰ��ͼ�豸�Ĳü���
int clearclip(); // ����ü�������Ļ����
int setlinestyle(int line_style, double wline) // ���õ�ǰ������ʽ
{
  gcLineStyle(g_easyx->ctx, line_style | ALIGN_CENTER);
  gcLineWidth(g_easyx->ctx, wline);
  return 0;
}
// ���õ�ǰ�����ʽ
//int setfillstyle(const FILLSTYLE* pstyle);
// ��������ԭ��
int setorigin(double x, double y)
{
  gcTranslate(g_easyx->ctx, x, y);
  return 0;
}
int getaspectratio(float* pxasp, float* pyasp); // ��ȡ��ǰ��������
int setaspectratio(float xasp, float yasp); // ���õ�ǰ��������
int getrop2(); // ��ȡǰ���Ķ�Ԫ��դ����ģʽ
int setrop2(int mode); // ����ǰ���Ķ�Ԫ��դ����ģʽ
int getpolyfillmode(); // ��ȡ��������ģʽ
int setpolyfillmode(int mode); // ���ö�������ģʽ
int graphdefaults(); // �������л�ͼ����ΪĬ��ֵ
COLOR getlinecolor(); // ��ȡ��ǰ������ɫ
// ���õ�ǰ������ɫ
int setlinecolor(COLOR color)
{
  gcStrokeColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
COLOR gettextcolor(); // ��ȡ��ǰ������ɫ
int settextcolor(COLOR color); // ���õ�ǰ������ɫ
COLOR getfillcolor(); // ��ȡ��ǰ�����ɫ
// ���õ�ǰ�����ɫ
int setfillcolor(COLOR color)
{
  gcFillColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
int setcolor(COLOR color)
{
  gcFillColor(g_easyx->ctx, EASYX_COLOR(color));
  gcStrokeColor(g_easyx->ctx, EASYX_COLOR(color));
  return 0;
}
COLOR getbkcolor(); // ��ȡ��ǰ��ͼ����ɫ
// ���õ�ǰ��ͼ����ɫ
int setbkcolor(COLOR color)
{
  g_easyx->m_BkColor = EASYX_COLOR(color);
  return 0;
}
int getbkmode(); // ��ȡ�������ģʽ
int setbkmode(int mode);
// ���ñ������ģʽ
// ��ɫģ��ת������
COLOR RGBtoGRAY(COLOR rgb);
int RGBtoHSLf(COLOR rgb, float* H, float* S, float* L);
int RGBtoHSVf(COLOR rgb, float* H, float* S, float* V);
COLOR HSLtoRGB(float H, float S, float L);
COLOR HSVtoRGBf(float H, float S, float V);
// ��ͼ����
COLOR getpixel(int x, int y); // ��ȡ�����ɫ
int putpixel(int x, int y, COLOR color); // ����
int moveto(double x, double y); // �ƶ���ǰ��(��������)
int moverel(double dx, double dy); // �ƶ���ǰ��(�������)
int line(double x1, double y1, double x2, double y2) // ����
{
  gc_t* g = g_easyx->ctx;
  gcBeginPath(g);
  gcMoveTo(g, x1, y1);
  gcLineTo(g, x2, y2);
  gcStroke(g);
  return 0;
}
int linerel(double dx, double dy); // ����(���������)
int lineto(double x, double y); // ����(����������)
// ��Բ�Ǿ���
int roundrect(double left, double top, double right, double bottom, double r);
// �����Բ�Ǿ���(�б߿�)
int fillroundrect_impl(double left, double top, double right, double bottom, double r, int dotype)
{
  gc_t* g = g_easyx->ctx;
  double t;
  r *= 0.5;
  gcBeginPath(g);
  if (left > right) {
    CV_SWAP(left, right, t);
  }
  if (top > bottom) {
    CV_SWAP(top, bottom, t);
  }
  gcRoundRect(g, left, top, right - left, bottom - top, r, r, r, r);
  gcClosePath(g);
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
int rectangle_impl(double left, double top, double right, double bottom, int dotype)
{
  gc_t* g = g_easyx->ctx;
  gcBeginPath(g);
  gcRect(g, left, top, right - left, bottom - top);
  gcClosePath(g);
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
// ������
int rectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 2);
}
// ��������(�б߿�)
int fillrectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 3);
}
// ��������(�ޱ߿�)
int solidrectangle(double left, double top, double right, double bottom)
{
  return rectangle_impl(left, top, right, bottom, 2);
}
int clearrectangle(double left, double top, double right, double bottom); // ��վ�������
int fillroundrect(double left, double top, double right, double bottom, double r)
{
  return fillroundrect_impl(left, top, right, bottom, r, 3);
}
// �����Բ�Ǿ���(�ޱ߿�)
int solidroundrect(double left, double top, double right, double bottom, double r)
{
  return fillroundrect_impl(left, top, right, bottom, r, 1);
}
int arc_impl(double left, double top, double right, double bottom, double stangle, double endangle, int dotype, int close)
{
  gc_t* g = g_easyx->ctx;
  if (stangle > endangle) {
    //double t;
    //CV_SWAP(stangle, endangle, t);
  }
  gcBeginPath(g);
  gcArc(g, (right + left) * 0.5, (bottom + top) * 0.5, fabs(right - left) * 0.5, fabs(bottom - top) * 0.5, -endangle * 180 / PI, -stangle * 180 / PI);
  if (close) {
    gcClosePath(g);
  }
  if (dotype & 1) {
    gcFill(g);
  }
  if (dotype & 2) {
    gcStroke(g);
  }
  return 0;
}
// ����Բ����(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
int pie(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 1, 1);
}
// ����Բ��(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
int arc(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 2, 0);
}
// �������Բ����(�б߿�)
int fillpie(double left, double top, double right, double bottom, double stangle, double endangle)
{
  return arc_impl(left, top, right, bottom, stangle, endangle, 3, 1);
}
int solidpie(double left, double top, double right, double bottom, double stangle, double endangle); // �������Բ����(�ޱ߿�)
int clearpie(double left, double top, double right, double bottom, double stangle, double endangle); // �����Բ��������
int ellipse(double left, double top, double right, double bottom); // ����Բ
// �������Բ(�б߿�)
int fillellipse(double left, double top, double right, double bottom)
{
  return arc_impl(left, top, right, bottom, 0, 2 * PI, 3, 1);
}
// �������Բ(�ޱ߿�)
int solidellipse(double left, double top, double right, double bottom)
{
  return arc_impl(left, top, right, bottom, 0, 2 * PI, 1, 1);
}
int clearellipse(double left, double top, double right, double bottom); // �����Բ������
// ��Բ
int circle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 2, 1);
}
// �����Բ(�б߿�)
int fillcircle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 3, 1);
}
// �����Բ(�ޱ߿�)
int solidcircle(double x, double y, double radius)
{
  return arc_impl(x - radius, y - radius, x + radius, y + radius, 0, 2 * PI, 1, 1);
}
// ���Բ������
int clearcircle(double x, double y, double radius);
int polyline(const IPOINT* points, int num); // ��������������
int polygon(const IPOINT* points, int num); // �������
int fillpolygon(const IPOINT* points, int num); // �����Ķ����(�б߿�)
int solidpolygon(const IPOINT* points, int num); // �����Ķ����(�ޱ߿�)
int clearpolygon(const IPOINT* points, int num); // ��ն��������
//int floodfill(int x, int y, int border); // �������
// ������غ���
int outtext(const char* str); // �ڵ�ǰλ������ַ���
int outchar(int c); // �ڵ�ǰλ������ַ�
int outtextxy(int x, int y, const char* str); // ��ָ��λ������ַ���
int outcharxy(int x, int y, int c); // ��ָ��λ������ַ�
int textwidth(const char* str); // ��ȡ�ַ���ռ�õ����ؿ�
int charwidth(int c); // ��ȡ�ַ�ռ�õ����ؿ�
int textheight(const char* str); // ��ȡ�ַ���ռ�õ����ظ�
int charheight(int c); // ��ȡ�ַ�ռ�õ����ظ�
int drawtext(const char* str, IRECT* pRect, UINT uFormat); // ��ָ����������ָ����ʽ����ַ���
int drawchar(int c, IRECT* pRect, UINT uFormat); // ��ָ����������ָ����ʽ����ַ�
// ���õ�ǰ������ʽ(�������)
// nHeight: �ַ���ƽ���߶ȣ�
// nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
// lpszFace: �������ƣ�
// nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
// nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
// nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
// bItalic: �Ƿ�б�壻
// bUnderline: �Ƿ��»��ߣ�
// bStrikeOut: �Ƿ�ɾ���ߣ�
// fbCharSet: ָ���ַ�����
// fbOutPrecision: ָ�����ֵ�������ȣ�
// fbClipPrecision: ָ�����ֵļ������ȣ�
// fbQuality: ָ�����ֵ����������
// fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�
int settextstyle(int nHeight, int nWidth, const char* lpszFace);
int settextstyle2(int nHeight, int nWidth, const char* lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut);
int settextstyle3(int nHeight, int nWidth, const char* lpszFace, int nEscapement, int nOrientation, int nWeight, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);
//int settextfont(const char* font); // ���õ�ǰ������ʽ
//int gettextfont1(LOGFONT* font); // ��ȡ��ǰ������ʽ
#include "softgc.inl"
#include "imui_win32.inl"
#include "imui_gdi.inl"
#include "imui_gdip.inl"
#include "imui_d3d9.inl"
#include "imui_gl2.inl"
#include "imui_soft.inl"

