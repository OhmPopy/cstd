
//#include "gdiplus.inl"
//#include "draw/imdraw.inl"

#include "test_gdiplus_fun.inl" 

#define TEST_GDIPLUS_FUN_DEF_DEF(TEST_GDIPLUS_FUNDEF) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus1, NULL) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus1, Graphics) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus3, ��������DashStyle) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus4, ���ʶ���PenAlignment) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus5, ������ñLineCap) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus6, ����ת��Transform) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus7, ֱ�����ӵ�LineJoin) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus8, ��ˢBrush���) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus9, ��ɫ��ˢSolidBrush) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus10, Ӱ�߻�ˢHatchBrush) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus11, ����ˢTextureBrush�Ļ���ʹ��) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus12, ����ˢTextureBrush�����з�ʽ) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus13, ���Խ��仭ˢLinearGrandientBrush) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus14, ���Խ��仭ˢ֮��ɫ����) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus16, ����Font) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus1, ��ȡϵͳ�Ѱ�װ����������) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus15, ·�����仭ˢ) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus2, ����Pen) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus21, ���ֻ�ˢ����) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus22, ������ñ) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus23, ������ͼ��) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus24, ��Ellipses��Arcs��������) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus25, ����Beziers��Quadratics����) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus26, ���ƾ���) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus27, ���ƶ����) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus28, ����line) \
  TEST_GDIPLUS_FUNDEF(test_gdiplus29, ������) \


  
int test_gdiplusui(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc, rcLeft;
  int i, left_w = 250;
  static int testid = 120;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  double t2 = 0;
  static ctrl_t cc[1] = {0};
  typedef void (*test_gdiplus_fun_t)(ctrl_t* c);
  const test_gdiplus_fun_t test_gdiplus_fun[] = {
#define TEST_GDIPLUS_FUNDEF(FUN, NAME)   FUN,
    TEST_GDIPLUS_FUN_DEF_DEF(TEST_GDIPLUS_FUNDEF)
#undef TEST_GDIPLUS_FUNDEF
  };
  iRectCutR(&rc, -left_w, 0, &rcLeft);
  cc->rc = rc;
  if (!inited) {
    inited = 1;
  }
  testid = BOUND(testid, 0, countof(test_gdiplus_fun)-1);
  gcClipRectR(g, cc->rc);
  {
    utime_start(time1);
    color_background_ctrl(rc, ColorBlack, ColorBlack, 1, false);
    test_gdiplus_fun[testid](cc);
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }
  gcClipReset(g);
  //gcResetPath(g);
  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[10] = {0};
    const char* s_test = "|"
#define TEST_GDIPLUS_FUNDEF(FUN, NAME)    #NAME "|"
      TEST_GDIPLUS_FUN_DEF_DEF(TEST_GDIPLUS_FUNDEF)
#undef TEST_GDIPLUS_FUNDEF
      ;
    
    i = 0;
    color_background_ctrl(rcLeft, ColorWhite, ColorWhite, 1, true);
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms", t2 * 1000);
    if (rbox_ctrl(cc + i++, "GDI+����", s_test, &testid)) {
      FORCE_REDRAW(c->rc);
    }
  }
  return 0;
}



int test_gdip(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc, rcLeft;
  int i, left_w = 250;
  static int testid = 120;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  double t2 = 0;
  static ctrl_t cc[1] = {0};
  typedef void (*test_gdiplus_fun_t)(ctrl_t* c);
  const test_gdiplus_fun_t test_gdiplus_fun[] = {
#define TEST_GDIPLUS_FUNDEF(NAME, FUN)   FUN,
    TEST_GDIP_FUN_DEF_DEF(TEST_GDIPLUS_FUNDEF)
#undef TEST_GDIPLUS_FUNDEF
  };
  iRectCutR(&rc, -left_w, 0, &rcLeft);
  cc->rc = rc;
  if (!inited) {
    inited = 1;
  }
  testid = BOUND(testid, 0, countof(test_gdiplus_fun)-1);
  {
    utime_start(time1);
    if (test_gdiplus_fun[testid]) {
      test_gdiplus_fun[testid](cc);
    }
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }
  //gcResetPath(g);
  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[10] = {0};
    const char* s_test = "|"
#define TEST_GDIPLUS_FUNDEF(NAME, FUN)    #NAME "|"
      TEST_GDIP_FUN_DEF_DEF(TEST_GDIPLUS_FUNDEF)
#undef TEST_GDIPLUS_FUNDEF
      ;
    
    i = 0;
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms", t2 * 1000);
    rbox_ctrl(cc + i++, "GDI+����", s_test, &testid);
  }
  return 0;
}
