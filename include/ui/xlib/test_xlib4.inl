
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
/*
#include <locale.h>
*/
#include <X11/Xlocale.h>


void DrawCenteredMbString(Display* dpy, Window w, XFontSet fontset, GC gc, char* str, int num_bytes, int x, int y, int width, int height)
{
  XRectangle boundingbox;
  XRectangle dummy;
  int originx, originy;
  (void) XmbTextExtents(fontset, str, num_bytes, &dummy, &boundingbox);
  originx = x + (width - boundingbox.width) / 2 - boundingbox.x;
  originy = y + (height - boundingbox.height) / 2 - boundingbox.y;
  XmbDrawString(dpy, w, fontset, gc, originx, originy, str, num_bytes);
}


int test_xlib4()
{
  Display* dpy;
  Window w;
  XFontSet fontset;
  char** missing_charsets;
  int num_missing_charsets;
  char* default_string;
  int i;
  GC gc;
  XEvent xe;
  const char* program_name = "���test_xlib4";

  if (setlocale(LC_ALL, "") == NULL) {
    (void) fprintf(stderr, "%s: cannot set locale.n", program_name);
    return (1);
  }

  if (!XSupportsLocale()) {
    (void) fprintf(stderr, "%s: X does not support locale %s.n",
        program_name, setlocale(LC_ALL, NULL));
    return(1);
  }

  if (XSetLocaleModifiers("") == NULL) {
    (void) fprintf(stderr,
        "%s: Warning: cannot set locale modifiers.n",
        program_name);
  }

  dpy = XOpenDisplay(NULL);
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0), 50, 50,
      100, 50, 5, BlackPixel(dpy, 0),
      WhitePixel(dpy, 0));
  gc = XCreateGC(dpy, w, 0L, (XGCValues*) NULL);
  fontset = XCreateFontSet(dpy, "-*-*-*-*-*-*-16-*-*-*-*-*-*-*",
      &missing_charsets, &num_missing_charsets,
      &default_string);

  if (num_missing_charsets > 0) {
    (void) fprintf(stderr,
        "%s: The following charsets are missing:n",
        program_name);

    for (i = 0; i < num_missing_charsets; i++) {
      (void)fprintf(stderr, "%s: %s\n", program_name, missing_charsets[i]);
    }

    (void) fprintf(stderr, "%s: The string is %s", program_name,
        default_string);
    (void) fprintf(stderr,
        "%s: of any characters from those sets.n",
        program_name);
    XFreeStringList(missing_charsets);
  }

  XSetLineAttributes(dpy, gc, 5, LineSolid, CapRound, JoinRound);
  XSelectInput(dpy, w, ExposureMask | ButtonPressMask
      | EnterWindowMask | LeaveWindowMask);
  XMapWindow(dpy, w);
  XFlush(dpy);

  while (True) {
    XNextEvent(dpy, &xe);

    switch (xe.type) {
    case Expose:
      printf("Expose ����.n");
      XSetForeground(dpy, gc, BlackPixel(dpy, 0));
      DrawCenteredMbString(dpy, w, fontset, gc, "��� Hello", 10, 0, 0, 100, 50);
      break;

    case ButtonPress:
      printf("����!n");
      return(1);
      break;

    case EnterNotify:
      printf("����.n");
      XSetForeground(dpy, gc, BlackPixel(dpy, 0));
      XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
      break;

    case LeaveNotify:
      printf("�뿪.n");
      XSetForeground(dpy, gc, WhitePixel(dpy, 0));
      XDrawRectangle(dpy, w, gc, 0, 0, 100, 50);
      break;

    default:
      printf("Ĭ��.n");
      break;
    }
  }

  return 0;
}
