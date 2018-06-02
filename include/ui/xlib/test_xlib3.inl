/*
* Xlib ѧϰ
*
*
* XEvent
* XMotion
*
*
*
*
* �������Xlib�ĳ�����Ҫ��Xlib�����ӡ�����ʹ������������У�

  cc prog.c -o prog -lX11

  ��������������Ҳ���X11�⣬�������ż���"-L"��־����������

   cc prog.c -o prog -L/usr/X11/lib -lX11

   �������������ʹ��X11�İ汾6��

   cc prog.c -o prog -L/usr/X11R6/lib -lX11

   ��SunOs 4 ϵͳ�ϣ�X�Ŀⱻ�ŵ��� /usr/openwin/lib

   cc prog.c -o prog -L/usr/openwin/lib -lX11
*
* GC-ͼ��������.[ͼ��,�ı���, ����,ǰ��,����,ʹ��ʲô��ɫ,ʹ��ʲô����ȵ�.]
* ������:����: ����,��ͼ���͹��-��Ӧ�ĺ����ͻ᷵��һ�����.
* �ͷ��ڴ�: XFree()
* �¼�: XEvent,(������), XMotion, XButon.
* */

/*
*
Display* display;
display = XOpenDisplay("simey:0");
if (NULL == display){
fprintf(stderr, "���Ӳ���X Server %s\n", "simey:0");
exit(-1);
}
�ر�X����������
XCloseDisplay(display)
* */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/keysymdef.h>
#include <X11/Xlib.h>

int test_xlib3()
{

  Display* display;
  display = XOpenDisplay("simey:0");

  if (NULL == display) {
    fprintf(stderr, "���Ӳ���X Server %s\n", "simey:0");
    exit(-1);
  }

  int screen_num;
  int screen_width;
  int screen_height;
  Window root_window;

  unsigned long white_pixel;
  unsigned long black_pixel;

  screen_num = DefaultScreen(display);

  screen_width = DisplayWidth(display, screen_num);
  screen_height = DisplayHeight(display, screen_num);

  puts("�������:");
  printf("���:%x���:%d�߶�:%d\n",
      screen_num, screen_width, screen_height);

  root_window = RootWindow(display, screen_num);
  white_pixel = WhitePixel(display, screen_num);
  black_pixel = BlackPixel(display, screen_num);

  Window win;
  int win_width;
  int win_height;
  int win_x;
  int win_y;
  int win_border_width;
  int win_border_height;

  int width;
  int height;

  win_x = win_y = win_border_width = win_border_height = 0;
  win_width = DisplayWidth(display, screen_num);
  win_height = DisplayHeight(display, screen_num);

  width = (win_width / 3);
  height = (win_height / 3);
  win_border_width = 2;

  /*����һ������*/
  win = XCreateSimpleWindow(
      display,
      RootWindow(display, screen_num),
      win_x, win_y,
      width, height,
      win_border_width, win_border_height,
      WhitePixel(display, screen_num)
      );
  /*ע���¼�*/
  /* XSelectInput(display, win, ExposureMask); */
  /*ExposureMask��ͷ�ļ�"X.h"�б����壬���������ע�������¼����ͣ����ǿ���ʹ���߼�"or"*/

  XSelectInput(display, win,
      ExposureMask | ButtonPressMask |
      ButtonReleaseMask | ButtonPress |
      ButtonRelease | EnterWindowMask |
      LeaveWindowMask | EnterNotify |
      LeaveNotify
              );
  /*���Ľ�����뿪 Enter Leave��GTK����.*/

  XMapWindow(display, win);

  /*����*/
  GC gc;
  XGCValues values;
  unsigned long valuemask = 0;
  /* XGCValues values = CapButt | JoinBevel; */
  /* unsigned long valuemask = GCCapStyle | GCJoinStyle; */
  gc = XCreateGC(display, win, valuemask, &values);
  XSync(display, False);

  if (gc < 0) {
    fprintf(stderr, "XCreateGC:\n");
  }

  /*����.*/

  XSetBackground(display, gc, WhitePixel(display, screen_num));/*���ñ�����ɫ*/
  XSetForeground(display, gc, BlackPixel(display, screen_num));/*����ǰ��ɫ*/
  unsigned int line_width = 2;
  int line_style = LineSolid;
  int cap_style = CapButt;
  int join_style = JoinBevel;

  XSetLineAttributes(display, gc,
      line_width, line_style, cap_style, join_style);
  XSetFillStyle(display, gc, FillSolid);


  /* sleep(14); */
  XEvent an_event;

  /*�¼�ѭ��*/
  while (1) {
    XNextEvent(display, &an_event);
    /*��������ж������¼�*/


    switch (an_event.type) {

    case KeyPress:
      printf("���̱�����");
      break;

    case Expose: /*�ػ�*/
      if (an_event.xexpose.count > 0) {
        break;
      }

      XDrawLine(display, win, gc, 0, 100, 400, 100);
      XDrawPoint(display, win, gc, 5, 5);
      XDrawRectangle(display, win, gc, 120, 150, 50, 60);
      XFillRectangle(display, win, gc, 60, 150, 50, 60);
      /*ˢ��*/
      XFlush(display);
      printf("�����ػ��¼�\n");
      break;

    case ButtonPress: /*�����¼�*/

      /* int x; */
      /* int y; */
      /* x = an_event.xbutton.x; */
      /* y = an_event.xbutton.window; */
      switch (an_event.xbutton.button) {
      case Button1:
        XDrawRectangle(display, win, gc, 120, 150, 50, 60);
        puts("����������");
        break;

      case Button2:
        puts("button2...");
        break;

      default:
        break;
      }
    }

  }

  /*�ر�X����������*/
  XCloseDisplay(display);
  return 0;
}


/*
  Display* display
  ָ����ʾ�ṹ��ָ��
  Window parent
  �´��ڵĸ����ڵ�ID��
  int x
  ���ڵ�����X���꣨��λΪ��Ļ���أ�
  int y
  ���ڵ�����Y���꣨��λΪ��Ļ���أ�
  unsigned int width
  ���ڵĿ�ȣ���λΪ��Ļ���أ�
  unsigned int height
  ���ڵĸ߶ȣ���λΪ��Ļ���أ�
  unsigned int border_width
  ���ڵı߿��ȣ���λΪ��Ļ���أ�
  unsigned long border
  �������ƴ��ڱ߿����ɫ
  unsigned long background
  �������ƴ��ڱ�������ɫ
* */

/*
  ���¼��ṹ�ͨ��"an_event.xbutton"������¼������ͣ�������������������Щ��Ȥ�����ݣ�

  Window window
  �¼����͵�Ŀ�괰�ڵ�ID���������Ϊ�������ע�����¼���

  int x, y
  �Ӵ��ڵ���������������������ʱ����ڴ����е�����

  int button
  ������Ǹ���ŵİ�ť�������ˣ�ֵ������Button1��Button2��Button3

  Time time
  �¼����Ž����е�ʱ�䡣���Ա�����ʵ��˫���Ĵ���
* */


/*

  �����Ľ�����뿪�¼�

  ��һ������ͨ�������Ȥ���¼��ǣ��й���������һ�����ڵ������Լ��뿪�Ǹ����ڵ�������¼�����Щ�������ø��¼��������û����������ڽ������档Ϊ��ע�������¼������ǽ����ں���XSelectInput()��ע�Ἰ����ߡ�

  EnterWindowMask
  ֪ͨ�����������������ǵĴ����е�����һ��

  LeaveWindowMask
  ֪ͨ����������뿪�����ǵĴ����е�����һ��

  ���ǵ��¼�ѭ���еķ�֧��齫������µ��¼�����

  EnterNotify
  �������������ǵĴ���

  LeaveNotify
  ������뿪�����ǵĴ���

  ��Щ�¼����͵����ݽṹͨ������"an_event.xcrossing"�����ʣ���������������Ȥ�ĳ�Ա������

  Window window
  �¼����͵�Ŀ�괰�ڵ�ID���������Ϊ�������ע�����¼���

  Window subwindow
  ��һ�������¼��У�������˼�Ǵ��Ǹ��Ӵ��ڽ������ǵĴ��ڣ���һ���뿪�¼��У�������˼�ǽ������Ǹ��Ӵ��ڣ������"none"��������˼�Ǵ�������������ǵĴ��ڡ�

  int x, y
  �Ӵ��ڵ��������������¼�����ʱ������ڴ����е�����

  int mode
  ������Ǹ���ŵİ�ť�������ˣ�ֵ������Button1��Button2��Button3

  Time time
  �¼����Ž����е�ʱ�䡣���Ա�����ʵ��˫���Ĵ���

  unsigned int state
  ����¼�����ʱ��갴ť�����Ǽ��̼��������µ���� - ����еĻ��������Աʹ�ð�λ��ķ�ʽ����ʾ
  Button1Mask
  Button2Mask
  Button3Mask
  Button4Mask
  ShiftMask
  LockMask
  ControlMask
  Mod1Mask
  Mod2Mask
  Mod3Mask
  Mod4Mask

  ���ǵ������ǿ�����չ�ģ�����������ť������ʱ��ʣ�µ����Ծ�ָ�����������������Mod1һ����"ALT"������"META"����

  Bool focus
  ��ֵ��True��ʱ��˵�����ڻ���˼��̽��㣬False��֮
* */


/*
* ���̼����º��ɿ��¼�
������ǳ�����ƵĴ��ڻ���˼��̽��㣬���Ϳ��Խ��ܰ����İ��º��ɿ��¼���Ϊ��ע����Щ�¼������ͣ����Ǿ���Ҫͨ������XSelectInput()��ע���������ߡ�


KeyPressMask
֪ͨ���ǵĳ���ʲôʱ�򰴼���������

KeyPressMask
֪ͨ���ǵĳ���ʲôʱ�򰴼����ɿ���

���ǵ��¼�ѭ���еķ�֧��齫������µ��¼�����

Window window
�¼����͵�Ŀ�괰�ڵ�ID���������Ϊ�������ע�����¼���

unsigned int keycode
�����£����ɿ����ļ��ı��롣����һЩX�ڲ����룬Ӧ�ñ������һ�����̼����Ų��ܷ���ʹ�ã�������������ܡ�

int x, y
�Ӵ��ڵ��������������¼�����ʱ������ڴ����е�����

Time time
�¼����Ž����е�ʱ�䡣���Ա�����ʵ��˫���Ĵ���

unsigned int state
����¼�����ʱ��갴ť�����Ǽ��̼��������µ���� - ����еĻ��������Աʹ�ð�λ��ķ�ʽ����ʾ
Button1Mask
Button2Mask
Button3Mask
Button4Mask
ShiftMask
LockMask
ControlMask
Mod1Mask
Mod2Mask
Mod3Mask
Mod4Mask

���ǵ������ǿ�����չ�ģ�����������ť������ʱ��ʣ�µ����Ծ�ָ�����������������Mod1һ����"ALT"������"META"���� */
