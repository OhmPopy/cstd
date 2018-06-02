
#define FONTTESTDEF_DEF(FONTTESTDEF) \
  FONTTESTDEF( test_font1, NULL) \
  FONTTESTDEF( test_font1, �оٳ�ϵͳ�е���������)

void test_font1(gc_t* g) {
#if 0
  gcSolidBrush(g, ColorBlack);
FontFamily fontfamily("Arial");
Font font(&fontfamily,8,FontStyleRegular,UnitPoint);  

int count=0;  
int found=0;  

WCHAR familyName[100];//����Ϊ�˼򻯳��򣬷����㹻��Ŀռ�  
WCHAR *familyList=NULL;  
FontFamily pFontFamily[500];//ͬ���������㹻��Ŀռ�  

CRect rect;  
this->GetClientRect(&rect);  
RectF r(0,0,rect.Width(),rect.Height());  

InstalledFontCollection installedFontCollection;  
count=installedFontCollection.GetFamilyCount();  
installedFontCollection.GetFamilies(count,pFontFamily,&found);  

familyList=new WCHAR[count*sizeof(familyName)];  
wcscpy(familyList,L"");//�����familyList��wcscpyʵ�ֶԿ��ֽڵĸ��Ʋ���  
for(int j=0;j<count;j++){  
  pFontFamily[j].GetFamilyName(familyName);  
  wcscat(familyList,familyName);//��familyName��ӵ�familyList�����  
  wcscat(familyList,L",");//wcscatʵ�ֶԿ��ֽ��ַ�����Ӳ���  
}  
graphics.DrawString(familyList,-1,&font,r,NULL,&solidbrush);  

delete[] familyList;
#endif
}

int test_font(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc, rcLeft;
  int i, left_w = 250;
  static int testid = 1;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  double t2 = 0;
  typedef void (*test_font_fun_t)(gc_t* g);
  const test_font_fun_t test_font_fun[] = {
#define FONTTESTDEF(FUN, NAME)   FUN,
    FONTTESTDEF_DEF(FONTTESTDEF)
#undef FONTTESTDEF
  };
  iRectCutR(&rc, -left_w, 0, &rcLeft);
  if (!inited) {
    inited = 1;
  }
  {
    utime_start(time1);
    test_font_fun[testid](g);
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }

  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[10] = {0};
    const char* s_test = "|"
#define FONTTESTDEF(FUN, NAME)   #NAME "|"
      FONTTESTDEF_DEF(FONTTESTDEF)
#undef FONTTESTDEF
      ;
    
    i = 0;
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms", t2 * 1000);
    rbox_ctrl(cc + i++, "�������", s_test, &testid);
  }
  return 0;
}
