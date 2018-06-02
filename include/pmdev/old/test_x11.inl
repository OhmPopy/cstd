#include <X11/Xlib.h>

int test_x11()
{
  XSetWindowAttributes winattrs;
  int swidth;/*��Ļ���*/
  int sheight;/*��Ļ�߶�*/
  int winmask;
  Display* display;
  int screen;
  Window window;

  display = XOpenDisplay(getenv("DISPLAY"));
  screen = DefaultScreen(display);
  swidth = DisplayWidth(display, screen);/*��ȡ��Ļ���*/
  sheight = DisplayHeight(display, screen);/*��ȡ��Ļ�߶�*/
  window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, swidth, sheight, 0, 0, 0);
  winattrs.override_redirect = True;
  winmask = CWOverrideRedirect;/*ָ��ֻ��override_redirect�������ı�*/
  XChangeWindowAttributes(display, window, winmask, &winattrs);

  XMapWindow(display, window);

  while (1) {
    /*event loop*/
  }

  return 0;
}
