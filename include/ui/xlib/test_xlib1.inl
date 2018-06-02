#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
void draw(Display* display, Window win, GC gc, XFontStruct* fs, XFontStruct* fs16)
{
  const char* str1 = "Hello World!";
  const char* str2 = "��ã����??��";
  //�ο���
  XDrawLine(display, win, gc, 20, 100, 300, 100);
  //���ַ���
  XSetFont(display, gc, fs->fid);
  XDrawString(display, win, gc, 20, 100, str1, strlen(str1));
  //������
  printf("str1 width:%d\n", XTextWidth(fs, str1, strlen(str1)));
  //�������ַ���
  XSetFont(display, gc, fs16->fid);
  XDrawString16(display, win, gc, 200, 100, (XChar2b*)str2,
      strlen(str2) / 2);
  //������
  printf("str2 width:%d\n", XTextWidth16(fs16,
      (XChar2b*)str2, strlen(str2)));
}
int test_xlib1()
{
  int i;
  Display* display;
  int screen_num;
  Window win; //����ID
  unsigned int width, height; //���ڳߴ�
  unsigned int border_width = 4; //�߽�հ�
  unsigned int display_width, display_height;//��Ļ�ߴ�
  int count;
  XEvent report;
  GC gc;
  unsigned long valuemask = 0;
  XGCValues values;
  char* display_name = NULL;
  char** fonts;
  int font_count;
  XFontStruct* fs, *fs16;

  // ��X ����������
  if ((display = XOpenDisplay(display_name)) == NULL) {
    printf("Cannot connect to X server %s\n",
        XDisplayName(display_name));
    return (-1);
  }

  //���ȱʡ�� screen_num
  screen_num = DefaultScreen(display);
  //�����Ļ�Ŀ�Ⱥ͸߶�
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);
  //ָ������������?��Ⱥ͸߶�
  //width = display_width/2;
  //height = display_height/2;
  width = 600;
  height = 400;
  //�г�����GB2312
  printf("All gb2312 fonts on this server:\n");
  fonts = XListFonts(display, "*gb2312*", 10000, &font_count);

  for (i = 0; i < font_count; i++) {
    printf("%s\n", fonts[i]);
  }

  XFreeFontNames(fonts);

  //����8x16����
  if ((fs = XLoadQueryFont(display, "9x15")) == NULL) {
    printf("Cannot load font 9x15\n");
    return(1);
  }

  //��������16��������
  if ((fs16 = XLoadQueryFont(display, "hanzigb16st")) == NULL) {
    printf("Cannot load Chinese font\n");
    return(1);
  }

  //��������
  win = XCreateSimpleWindow(display, //display
      RootWindow(display, screen_num), //������
      0, 0, width, height, //λ�úʹ�С
      border_width, //�߽���
      BlackPixel(display, screen_num), //ǰ��ɫ
      WhitePixel(display, screen_num)); //����ɫ
  //ѡ�񴰿ڸ���Ȥ���¼�����
  XSelectInput(display, win,
      ExposureMask | KeyPressMask |
      ButtonPressMask | StructureNotifyMask);
  //����GC
  gc = XCreateGC(display, win, valuemask, &values);
  //��ʾ����
  XMapWindow(display, win);

  //�����¼�ѭ��
  while (1) {
    //ȡ�ö����е��¼�
    XNextEvent(display, &report);

    switch (report.type) {
      //�ع��¼�, ����Ӧ�ػ�
    case Expose:

      //ȡ�����һ���ع��¼�
      if (report.xexpose.count != 0) {
        break;
      }

      draw(display, win, gc, fs, fs16);
      break;

      //���ڳߴ�ı�, ����ȡ�ô��ڵĿ�Ⱥ͸߶�
    case ConfigureNotify:
      width = report.xconfigure.width;
      height = report.xconfigure.height;
      break;

      //��������а���, �ͷ���Դ?�˳�
    case ButtonPress:
      XFreeGC(display, gc);
      XCloseDisplay(display);
      return(1);

    case KeyPress:
      printf("report: %c\n", report.xkey.keycode);
      break;

    default:
      break;
    }
  }
}
