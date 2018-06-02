#include <X11/Xlib.h>
#include <stdio.h>
void test_xlib2()
{
  Display* d;
  Window w, wb;
  XEvent event;
  Font f;
  GC gc;
  d = XOpenDisplay(NULL);
  w = XCreateSimpleWindow(d, RootWindow(d, 0),
      180, 50,
      260, 160,
      1, WhitePixel(d, 0), BlackPixel(d, 0));
  XSelectInput(d, w, ExposureMask | ButtonPressMask);
  wb = XCreateSimpleWindow(d, w,
      20, 20,
      100, 20,
      1, WhitePixel(d, 0), BlackPixel(d, 0));
  XSelectInput(d, wb, ExposureMask | ButtonPressMask);
  gc = XCreateGC(d, w, 0, 0);
  f = XLoadFont(d, "fixed");
  XSetFont(d, gc, f);
  XMapWindow(d, w);
  XMapWindow(d, wb);
  XFlush(d);

  while (1) {
    XNextEvent(d, &event);

    switch (event.type) {
    case Expose:
      if (event.xany.window == wb) {
        XSetForeground(d, gc, BlackPixel(d, 0));
        XFillRectangle(d, w, gc, 0, 0, 260, 160);
        XSetForeground(d, gc, WhitePixel(d, 0));
        XDrawString(d, wb, gc, 10, 10, "push", 4);  //��push�����������硱ȷ //��������������Ϊʲô�������ִ�У����ܳ�������
      }

      break;

    case ButtonPress:
      if (event.xany.window == w) {
        printf("w ");
        fflush(stdout);
      }

      if (event.xany.window == wb) {
        printf("wb ");
        fflush(stdout);
      }

      break;
    }
  }
}

