typedef TDrawDC* PGpGraphics;
typedef struct PGpPen {
  COLORREF color;
  double wline;
} PGpPen;
typedef HDDC PGpGraphics;
void DrawAlignmentRect(PGpGraphics g, PGpPen pen, PGpPen basePen, pen_alignment_e alignment, double x, double y)
{
  DPOINT pt[10];
  int n;
  n = vcgen_rect(pt, x + 0, y + 0, x + 100, y + 100);
  drawaa_strokepoly_impl(g, pt, n, alignment, pen.color, pen.wline, round_cap, miter_join, inner_round);
  drawaa_strokepoly_impl(g, pt, n, alignment, basePen.color, basePen.wline, round_cap, miter_join, inner_round);
  //PenSetAlignment(pen, alignment);
  //GraphicsDrawRectangle(g, pen, 0, 0, 100, 100);
  //GraphicsDrawRectangle(g, basePen, 0, 0, 100, 100);
}
void DrawDashStyleLine(PGpGraphics g, PGpPen pen, double x, double y, int cap)
{
  // �Զ�������ʽ�����飬���ݺ���Ϊ�߶Σ��հף��߶Σ��հ�...��
  // ʵ���߶γ���Ϊ���ݳ˻��ʿ��
  int ds;
  DPOINT pt[2] = {0, 0, 300, 0};
  double dashes[][2] = {
    1, 0,
    20, 10,
  };
  poly_offset(pt, 2, x, y);

  for (ds = 0; ds < 2; ds ++) {
    drawaa_dashpoly_impl(g, pt, 2, 0, pen.color, pen.wline, 0, dashes[ds], 2, cap, round_join, inner_round);
    //GraphicsDrawLine(g, pen, 0, 0, 300, 0);
    poly_offset(pt, 2, 0, 20);
  }
}
void test_drawline(PGpGraphics g)
{
  // ���帴���߱������飬2������Ϊ1�飬��֮��Ĵ�СΪ�հף���3����
  double cp[6] = {0.0f, 0.15f, 0.3f, 0.45f, 0.7f, 1.0f};
  PGpPen redPen = {rgb(1, 0, 0), 1}; // ��ɫ��
  PGpPen greenPen = {_RGB(2, 127, 0), 20}; // ��ɫ�ʿ��20
  // �������������ͼ��ʽ�������ٶȣ�����ע�͸þ��Ƚ���ͼЧ��
  //GraphicsSetSmoothingMode(g, SmoothingModeAntiAlias);
  // ����ƽ�ƣ�������ͼʱʹ���������
  int x = 20, y = 20;
  // ����ͬ�Ķ��뷽ʽ��ͬ���ľ��Σ�redPenΪ�Ƚϻ�׼��
  DrawAlignmentRect(g, greenPen, redPen, AlignmentCenter, x, y); // ���У�ȱʡ��
  DrawAlignmentRect(g, greenPen, redPen, AlignmentInset, x, y + 130); // ����
  // ��λͼ�α任
  x = 150, y = 20;
  // �ÿ��6��ɫ�ʻ���������ʽ�����߶�����Ϊƽͷñ��ȱʡֵ��
  greenPen.wline = 6;
  DrawDashStyleLine(g, greenPen, x, y, butt_cap);
#if 0
  // �ÿ��6��ɫ�ʻ���������ʽ�����߶�����ΪԲͷñ
  PenSetDashCap(greenPen, DashCapRound);
  GraphicsTranslate(g, 0, 10, MatrixOrderPrepend);
  DrawDashStyleLine(g, greenPen);
  // �ÿ��30��ɫ�ʻ���������
  GraphicsTranslate(g, -140, 20, MatrixOrderPrepend);
  PenSetWidth(redPen, 30);
  PenSetCompoundArray(redPen, cp, 6); // ���ø����߱�������
  GraphicsDrawLine(g, redPen, 0, 0, 440, 0);
#endif
}
// ��ʾ��ˢ
int test_brush(HDDC g)
{
  path_point path[] = {
    m_path_move_to(40, 140),
    m_path_curve4(275, 200, 105, 225, 190, 300),
    m_path_line_to(50, 350),
    m_path_line_to(20, 180),
  };
  DWORD ac[] = {Green, Yellow, Red, Blue, Orange, White};
  RECT clientRect = {0, 0, g->w, g->h};
  // ����һ��ȫ��ͼ��ˢ��������䴰�ڱ���
  //PGpImage backgroundImage = ImageCreate(L"..//..//Media//colorbars.jpg");
  //backgroundBrush = TextureBrushCreate(backgroundImage);
  DPOINT pt[1000];
  COLORREF col[256];
  int n;
  int lens[10];
  // ��ͼ��ˢ��䱳����ͬʱ�ð�ɫ��͸��ʵɫ��ˢ���ձ���ͼ��
  //draw_fillrectR(g, backgroundBrush, 0, 0, clientRect.right, clientRect.bottom);
  draw_fillrectR(g, &clientRect, _SetAlpha(White, 180));
  // �ú�ɫʵɫ��ˢ�����Σ�ͬʱ�ð�͸����ɫʵɫˢ����ͬ����С����������
  draw_fillrect(g, 20, 20, 50, 50, Red);
  draw_fillrect(g, 40, 40, 50, 50, _SetAlpha(Yellow, 180));
  // ��ͼ����ˢ��һ��Բ��ͼ��ǰ��ɫΪ��ɫ������ɫ��Ϊ���͸����ɫ
  //hb = HatchBrushCreate(HatchStyleForwardDiagonal, Green, _SetAlpha(Yellow, 100));
  drawaa_ellipse(g, 300, 80, 50, 50, _SetAlpha(Yellow, 100), 0);
  // �ý���ɫ��ˢ�����������»�һ�졢��ɫ�������
  fill_color_array(256, col, Red, Yellow);
  n = vcgen_rect(pt, 300, 250, 400, 350);
  drawaa_fillpoly_lut(g, pt, &n, 1, 400, 250, 300, 350, col);
  // �ã�����ɫ·����ˢƥ�䣶�����·����һͼ�Σ�ע���ⲻ�Ƕ�ɫ��䣬
  // ����ƥ��·������㽥����䣬��ɫ�������ڵ����ܴ���·������㣻
  // ��ɫ·��ˢ�������·�����ĵ����Ե���涨�ı�����䣩��Ӧ�ú�Ư����
  // ���ź����ǣ��һ�û������˭���ɹ������ͼ�Σ�ֻ�ܻ���ɫ��Ҳ��Ư���ġ�
  n = vcgen_path(path, countof(path), pt, lens);
  drawaa_fillpoly_solid(g, pt, lens, n, Green);
  // ����ɫʵɫ��ˢ��һ����ʱ����ת30�ȵľ���
  n = vcgen_rect(pt, 100, 250, 175, 350);
  {
    TRANS_AFFINE_DEF(ta);
    trans_affine_rotation(ta, -30 * (PI / 180));
    trans_affine_transform_pt(ta, pt, n);
  }
  drawaa_fillpoly_solid(g, pt, &n, 1, SlateBlue);
  return 0;
}
#if 0
WCHAR flowedText1[] =
    L"I went down to the St James Infirmary,/n /
    Saw my baby there,/n /
    Stretched out on a long white table,/n /
    So sweet,so cold,so fair,/n /
    Let her go,let her go,God bless her,/n /
    Wherever she may be,/n /
    She can look this wide world over,/n /
    But she'll never find a sweet man like me,/n /
    When I die want you to dress me in straight lace shoes,/n /
    I wanna a boxback coat and a Stetson hat,/n /
    Put a twenty dollar gold piece on my watch chain,/n /
    So the boys'll know that I died standing up.";
WCHAR flowedText2[] =
    L"the sky seems full when you're in the cradle/n /
    the rain will fall and wash your dreams/n /
    stars are stars and they shine so hard.../n /
    now spit out the sky because its empty/n /
    and hollow and all your dreams are hanging out to dry/n /
    stars are stars and they shine so cold.../n /
    once i like crying twice i like laughter/n /
    come tell me what i'm after.";
WCHAR japaneseText[] = {
  31169, 12398, 21517, 21069, 12399, 12463,
  12522, 12473, 12391, 12377, 12290, 0
};
PGpBrush backgroundBrush;
PGpBrush textTextureBrush;
PGpBrush titleShadowBrush;
PGpBrush linearGradBrush;
PGpFont titleFont;
PGpFont textFont;
PGpFont thisFont;
PGpFont japaneseFont;
void OnCreate(void)
{
  Point ps[] = {{0, 0}, {0, 45}};
  PGpFontFamily serifFontFamily = FontFamilyGenericSerif();
  //Load the image to be used for the background from the exe's resource fork
  PGpImage image = ImageCreate(L"..//..//Media//colorbars.jpg");
  //Now create the brush we are going to use to paint the background
  backgroundBrush = TextureBrushCreate(image);
  ImageDelete(image);
  //Load the image to be used for the textured text from the exe's resource fork
  image = ImageCreate(L"..//..//Media//marble.jpg");
  textTextureBrush = TextureBrushCreate(image);
  ImageDelete(image);
  //Load the fonts we want to use
  thisFont = FontFromFamily(serifFontFamily, 20, 0, UnitPoint);
  titleFont = FontFromFamily(serifFontFamily, 60, 0, UnitPoint);
  textFont = FontFromFamily(serifFontFamily, 11, 0, UnitPoint);
  //Set up shadow brush - make it translucent
  titleShadowBrush = SolidBrushCreate(_SetAlpha(Black, 70));
  //Set up fonts and brushes for printing japanese text
  japaneseFont = FontCreate(L"MS Mincho", 36, 0);
  linearGradBrush = LineBrushFromPoint(&ps[0], &ps[1], Blue, Red);
}
void OnPaint(HDC DC)
{
  WCHAR s[40] = L"Hello Symetrical World";
  RECT clientRect;
  RectF r;
  REAL center;
  INT characters, lines;
  PGpStrFormat format = StrFormatCreate();
  PGpSolidBrush sb = SolidBrushCreate(_SetAlpha(White, 180));
  PGpGraphics g = GraphicsCreate(DC);
  GraphicsSetTextRenderingHint(g, TextRenderingHintAntiAlias);
  //Fill the background use the texture brush
  //and then apply a white wash
  GetClientRect(Handle, &clientRect);
  GraphicsFillRectangle(g, backgroundBrush, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
  GraphicsFillRectangle(g, sb, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
  //Simple draw hello world
  SolidBrushSetColor(sb, Black);
  GraphicsDrawStringXY(g, L"Hello World", thisFont, sb, 10, 10);
  //Draw a textured string
  GraphicsDrawStringXY(g, L"Graphics Samples", titleFont, titleShadowBrush, 15, 25);
  GraphicsDrawStringXY(g, L"Graphics Samples", titleFont, textTextureBrush, 10, 20);
  //Use Measure string to display a string at the center of the window
  SolidBrushSetColor(sb, Red);
  center = clientRect.right / 2.0f;
  GraphicsMeasureString(g, s, textFont, &r);
  GraphicsDrawStringXY(g, s, textFont, sb, center - r.Width / 2.0f, 10.0f);
  //Now draw a string flowed into a rectangle
  r = MakeRectF(20.0f, 150.0f, 250.0f, 300.0f);
  SolidBrushSetColor(sb, Gainsboro);
  GraphicsFillRectangleF(g, sb, r.X, r.Y, r.Width, r.Height);
  SolidBrushSetColor(sb, Blue);
  GraphicsDrawString(g, flowedText1, textFont, sb, &r, NULL);
  //Draw more flowed text but this time center it
  r = MakeRectF(420.0f, 150.0f, 250.0f, 300.0f);
  SolidBrushSetColor(sb, Gainsboro);
  GraphicsFillRectangleF(g, sb, r.X, r.Y, r.Width, r.Height);
  StrFormatSetAlignment(format, StringAlignmentCenter);
  SolidBrushSetColor(sb, Blue);
  GraphicsDrawString(g, flowedText2, textFont, sb, &r, format);
  //Work out how many lines and characters we printed just now
  characters = lines = 0;
  GraphicsMeasureStringSize(g, flowedText2, textFont,
      250.0f, 300.0f, format, &r, &characters, &lines);
  wsprintfW(s, L"We printed %d characters and %d lines", characters, lines);
  SolidBrushSetColor(sb, Black);
  GraphicsDrawStringXY(g, s, textFont, sb, 390.0f, 440.0f);
  //If we have the Japanese language pack draw some text in Japanese
  //Rotate it to make life truly exciting
  GraphicsRotate(g, -30, MatrixOrderPrepend);
  GraphicsTranslate(g, -180, 300, MatrixOrderPrepend);
  GraphicsDrawStringXY(g, japaneseText, japaneseFont, linearGradBrush, 175, 125);
  GraphicsResetTransform(g);
  StrFormatDelete(format);
  BrushDelete(sb);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  /* ����ʹ����˫���廭���ڣ��ɽ���3��������ΪFALSE�Ƚ�һ��Ч�� */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  return RunApplication(TEXT("C����Gdiplus��ʾ���� -- ����(��c#����)"), 700, 500);
}

#pragma hdrstop
#define ID_REASE 100
#define ID_ALPHA 101
#define ID_BRIGHT 102
#define ID_GRAY 103
#define ID_TWO 104
#define ID_INVERT 105
PGpBitmap bitmap;
PGpImageAttr attr;
Rect dRect;
HFONT hFont = NULL;
void CreateButton(LPTSTR caption, INT top, INT itemID, HFONT hFont)
{
  HWND hButton = CreateWindow(TEXT("Button"), caption, WS_CHILD | WS_VISIBLE,
      488, top, 80, 25, Handle, (HMENU)itemID, hInstance, NULL);
  SendMessage(hButton, WM_SETFONT, (WPARAM)(hFont), 0);
}
void OnCreate(void)
{
  hFont = CreateFont(14, 0, 0, 0, FW_NORMAL,
      FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, TEXT("����"));
  CreateButton(TEXT("��͸��"), 30, ID_ALPHA, hFont);
  CreateButton(TEXT("����20%"), 65, ID_BRIGHT, hFont);
  CreateButton(TEXT("�ҶȻ�"), 100, ID_GRAY, hFont);
  CreateButton(TEXT("��ֵ��"), 135, ID_TWO, hFont);
  CreateButton(TEXT("�� ɫ"), 170, ID_INVERT, hFont);
  CreateButton(TEXT("�� ��"), 205, ID_REASE, hFont);
  bitmap = BitmapFromFile(L"..//..//Media//Source.jpg", FALSE);
  attr = ImageAttrCreate();
  dRect = MakeRect(0, 0, ImageGetWidth(bitmap), ImageGetHeight(bitmap));
}
void OnDestroy(void)
{
  if (hFont) {
    DeleteObject(hFont);
  }

  ImageDelete(bitmap);
  ImageAttrDelete(attr);
}
void OnCommand(WORD NotifyCode, WORD itemID, HWND ctlHandle)
{
  ColorMatrix matrix;
  INT i;
  memset(&matrix, 0, sizeof(ColorMatrix));

  for (i = 0; i < 5; i ++) {
    matrix.m[i][i] = 1.0f;
  }

  ImageAttrReset(attr, ColorAdjustTypeDefault);

  switch (itemID) {
  case ID_REASE:
    InvalidateRect(Handle, &dRect, FALSE);
    return;

  case ID_ALPHA:
    matrix.m[3][3] = 0.5f;
    break;

  case ID_BRIGHT:
    matrix.m[4][0] = matrix.m[4][1] = matrix.m[4][2] = 0.2f;
    break;

  case ID_GRAY:
  case ID_TWO:
    matrix.m[0][0] = matrix.m[0][1] = matrix.m[0][2] = 0.30f;
    matrix.m[1][0] = matrix.m[1][1] = matrix.m[1][2] = 0.59f;
    matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][2] = 0.11f;
    break;

  case ID_INVERT:
    matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = -1.0f;
    break;
  }

  ImageAttrSetColorMatrix(attr, &matrix, ColorMatrixFlagsDefault, ColorAdjustTypeDefault);

  if (itemID == ID_TWO) {
    ImageAttrSetThreshold(attr, 0.5, ColorAdjustTypeDefault);
  }

  InvalidateRect(Handle, &dRect, FALSE);
}
void OnPaint(HDC DC)
{
  PGpGraphics g = GraphicsCreate(DC); // GDI+ ����
  GraphicsDrawImageRectRect(g, bitmap, &dRect, 0, 0, dRect.Width, dRect.Height, UnitPixel, attr);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // ����ʹ����˫���廭���� */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  CommandProc = OnCommand;
  return RunApplication(TEXT("C����Gdiplus��ʾ���� -- ColorMatrix"), 582, 392);
}

PGpBitmap bitmap;
PGpBitmap photo;
PGpBitmap watermark;
INT wmWidth, wmHeight;
INT phWidth, phHeight;
HFONT hFont = NULL;
ColorMatrix colorMatrix = {1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.3, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0
                          };
void OnCreate(void)
{
  PGpImageAttr attr;
  PGpFont font;
  PGpBrush brush;
  PGpStrFormat format;
  PGpGraphics g;
  float x, y;
  Rect dRect;
  RectF fRect;
  HWND hButton;
  hFont = CreateFont(14, 0, 0, 0, FW_NORMAL,
      FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, TEXT("����"));
  hButton = CreateWindow(TEXT("Button"), TEXT("����"), WS_CHILD | WS_VISIBLE,
      400, 204, 80, 25, Handle, (HMENU)ID_SAVE, hInstance, NULL);
  SendMessage(hButton, WM_SETFONT, (WPARAM)(hFont), 0);
  // ��ȡԭʼͼƬ
  photo = BitmapFromFile(L"..//..//Media//100_0349.jpg", FALSE);
  phWidth = ImageGetWidth(photo);
  phHeight = ImageGetHeight(photo);
  // ��ȡˮӡͼƬ
  watermark = BitmapFromFile(L"..//..//Media//Watermark.bmp", FALSE);
  wmWidth = ImageGetWidth(watermark);
  wmHeight = ImageGetHeight(watermark);
  // ����һ���µ�λͼ���ֱ���Ϊ72
  bitmap = BitmapCreate(phWidth, phHeight, PixelFormat32bppARGB);
  BitmapSetResolution(bitmap, 72, 72);
  // ������λͼ�Ļ�����������ͼ����ʾ�������ı���ʾ����
  g = GraphicsFromImage(bitmap);
  GraphicsSetSmoothingMode(g, SmoothingModeAntiAlias);
  GraphicsSetTextRenderingHint(g, TextRenderingHintAntiAlias);
  /* �ڻ����ϻ�ԭʼͼ�� */
  GraphicsDrawImage(g, photo, 0, 0, phWidth, phHeight);
  /* ��ˮӡԭʼͼ�� */
  // ����һ��ͼ����ʾ���Զ���
  attr = ImageAttrCreate();
  // ����͸����ɫΪˮӡͼƬ�Ľǵĵ�ɫ��ˮӡͼ��ʾΪԲ��ͼƬ
  ImageAttrSetColorKey(attr, 0xff00ff00, 0xff00ff00, ColorAdjustTypeBitmap);
  // ����ˮӡͼƬ��͸����Ϊ0.3
  ImageAttrSetColorMatrix(attr, &colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
  // �ڻ������Ͻǻ�ԭʼˮӡͼ
  dRect = MakeRect(10, 10, wmWidth, wmHeight);
  GraphicsDrawImageRectRect(g, watermark, &dRect, 0, 0, wmWidth, wmHeight, UnitPixel, attr);
  /* �ڻ����·����л��ı� */
  // �����������
  font = FontCreate(L"arial", 16, FontStyleBold);
  // ������ˢ����
  brush = SolidBrushCreate(0x99000000);
  // �����ı���ʽ����
  format = StrFormatCreate();
  // �����ı�������ʾ
  StrFormatSetAlignment(format, StringAlignmentCenter);
  // ���ı���Ӱ
  fRect = MakeRectF(phWidth / 2 + 1, phHeight - 26 + 1, 0, 0);
  GraphicsDrawString(g, L"Copyright ? 2010 - Maozefa", font, brush, &fRect, format);
  // ���ı�
  fRect = MakeRectF(phWidth / 2, phHeight - 26, 0, 0);
  SolidBrushSetColor(brush, 0x99ffffff);
  GraphicsDrawString(g, L"Copyright ? 2010 - Maozefa", font, brush, &fRect, format);
  ImageAttrDelete(attr);
  StrFormatDelete(format);
  FontDelete(font);
  BrushDelete(brush);
  GraphicsDelete(g);
}
void OnDestroy(void)
{
  if (hFont) {
    DeleteObject(hFont);
  }

  ImageDelete(bitmap);
  ImageDelete(photo);
  ImageDelete(watermark);
}
void OnCommand(WORD NotifyCode, WORD itemID, HWND ctlHandle)
{
  EncoderParameters parameters;
  UINT quality = 100;
  GUID clsid;

  if (itemID != ID_SAVE) {
    return;
  }

  // ����ͼ��Ʒ�ʱ������
  parameters.Count = 1;
  parameters.Parameter[0].Guid = EncoderQuality;
  parameters.Parameter[0].Type = EncoderParameterValueTypeLong;
  parameters.Parameter[0].NumberOfValues = 1;
  // ���ò�����ֵ��Ʒ�ʵȼ������Ϊ100��ͼ���ļ���С��Ʒ�ʳ�����
  parameters.Parameter[0].Value = &quality;

  // ����ˮӡͼƬ
  if (GetEncoderClsid(L"image/jpeg", &clsid)) {
    ImageSaveToFile(bitmap, L"d://WatermarkPhoto.jpg", &clsid, ? meters);
  }
}
void OnPaint(HDC DC)
{
  PGpGraphics g = GraphicsCreate(DC); // GDI+ ����
  // ��ʾԭʼͼƬ
  GraphicsDrawImage(g, photo, 0, 0, phWidth, phHeight);
  // ��ʾˮӡԭʼͼƬ
  GraphicsTranslate(g, 0, phHeight + 5, MatrixOrderPrepend);
  GraphicsDrawImage(g, watermark, 0, 0, wmWidth, wmHeight);
  // ��ʾ��ˮӡ���ı���ͼ��
  GraphicsTranslate(g, phWidth, -(phHeight + 5), MatrixOrderPrepend);
  GraphicsDrawImage(g, bitmap, 0, 0, phWidth, phHeight);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // ����ʹ����˫���廭���� */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  CommandProc = OnCommand;
  return RunApplication(TEXT("C����Gdiplus��ʾ���� -- ˮӡͼƬ"), 518, 280);
}

����������ˮӡͼƬ�Ĳ��裺
1����ԭʼͼƬ��ԭʼˮӡͼƬ��
2������һ���µ�ͼ�񣬼�Ҫ������ˮӡͼƬ�����С��ͬ��ԭʼͼƬ��
3����ԭʼͼƬ����ͼ��
4������ͼ����ʾ���Զ�������ԭʼˮӡͼƬ�ؼ���ɫ�������ĸ��ǵ���ɫ������Ϊ͸��ɫ��������ʾ��ͼ��ΪԲ����״��
5������ͼ����ʾ���Զ�������ԭʼˮӡͼƬ��ɫ����ʹ�䰴30 % �Ĳ�͸������ʾ��
6���������úõ�ͼ����ʾ���Զ�����ʾԭʼˮӡͼƬ����ͼ�����Ͻǣ�
7�������ı���ʾ��ʽ����Ϊ�ı�������ʾ��
8���԰�Ȩ�ı���ʾ��������ƫ����1���أ���0x99000000��ɫ����ͼ����ʾ��Ȩ�ı���Ӱ����ɫ�е�0x99Ϊ�ı���Ӱ��ɫ��͸���ȣ�
9���԰�Ȩ�ı���ʾ�������꣬��0x99ffffff��ɫ����ͼ����ʾ��Ȩ�ı�����ɫ�е�0x99Ϊ�ı���ɫ��͸���ȣ�
10��������ϡ�
�������ۺ�������GDI + ���õ�ͼ��������̺��ı���ʾ���̣�����ͼ����ʾ���Զ�����ı���ʾ��ʽ��������á�
ԭC++ GDI + �汾��Graphics.DrawString�����и���x, y����Ϊ���������ط�����C�汾����ɾȥ�ˣ�ֻ�������Բ��־���Ϊ�����ķ����������еĴ���fRect = MakeRectF(phWidth / 2 , phHeight - 26, 0, 0);
�ǽ������־��Σ����е�x��y����Ϊ�ı���ʾ�����ĵ㣬�����ı���ʾ�����Ͻ����꣬������Ϊ�������ı���ʽ����Ϊ������ʾ��StrFormatSetAlignment(format, StringAlignmentCenter);
���������ı���ʾ��ʼ��������Ϊ�ı����ĵ㣻��Width��Height�����ֱ�����Ϊ0����˼�ǲ������ı���ʾ���ұߺ��±ߵı߽磬�൱��ԭC++�汾��x, y����Ϊ���������ط��������ı����ȳ���ʱ��ضϣ���֮��������������Width��Height�����ı����ȳ���ʱ�ỻ�С�
������Ҳ��ʾ����α���ͼ�񣬲����������ñ���ͼ��������������Ĵ��루����jpeg��ʽͼ����Ч��������ͼ��ʱ�õ���GetEncoderClsid������ԭGDI + ��û�еģ�Ϊ�����д����ͼ����룬�ҽ��ú���д����GDI + ͷ�ļ�GdipImageCode_c.h�С����û�����ñ���ͼ����������ImageSavexxxxϵ�к����е�encoderParams��������ΪNULL���ɣ������е�clsidEncoder�����Ǳ���ġ�
���Ӵ���ʹ�õĴ��ڿ�ܴ����GDI + C���԰汾���ص�ַ������C����WindowsӦ�ó�����ʹ��GDI + ����
�������������н���ͼ����������ΪԭʼͼƬ������ΪԭʼˮӡͼƬ���ұ��������õĴ���Ȩӡ�ǵ�ˮӡͼƬ��
FORCEINLINE
INT CheckValue(INT value)
{
  return (value & ~0xff) == 0 ? value : value > 255 ? 255 : 0;
}

// ��������/�Աȶȵ���
VOID LineBrightAndContrast(BitmapData* data, INT bright, INT contrast, BYTE threshold)
{
  if (bright == 0 && contrast == 0) {
    return;
  }

  FLOAT bv = bright <= -255 ? -1.0f : bright / 255.0f;

  if (bright > 0 && bright < 255) {
    bv = 1.0f / (1.0f - bv) - 1.0f;
  }

  FLOAT cv = contrast <= -255 ? -1.0f : contrast / 255.0f;

  if (contrast > 0 && contrast < 255) {
    cv = 1.0f / (1.0f - cv) - 1.0f;
  }

  BYTE values[256];

  for (INT i = 0; i < 256; i ++) {
    INT v = contrast > 0 ? CheckValue(i + (INT)(i * bv + 0.5f)) : i;

    if (contrast >= 255) {
      v = v >= threshold ? 255 : 0;
    }
    else {
      v = CheckValue(v + (INT)((v - threshold) * cv + 0.5f));
    }

    values[i] = contrast <= 0 ? CheckValue(v + (INT)(v * bv + 0.5f)) : v;
  }

  PARGBQuad p = (PARGBQuad)data->Scan0;
  INT offset = data->Stride - data->Width * sizeof(ARGBQuad);

  for (UINT y = 0; y < data->Height; y ++, (BYTE*)p += offset) {
    for (UINT x = 0; x < data->Width; x ++, p ++) {
      p->Blue = values[p->Blue];
      p->Green = values[p->Green];
      p->Red = values[p->Red];
    }
  }
}

void __fastcall TForm1::Button2Click(TObject* Sender)
{
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\source1.jpg");
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(bmp, 0, 0);
  BitmapData data;
  LockBitmap(bmp, &data);
  LineBrightAndContrast(&data, 20, 0, 121);
  UnlockBitmap(bmp, &data);
  g->DrawImage(bmp, data.Width, 0);
  delete g;
  delete bmp;
}
C++ͼ���� -- ͼ��ϳ�
���ࣺ C / C++ GDI + (VCL)Ӧ�� C / C++ͼ��ͼ�� 2012 - 01 - 04 21: 20 2603���Ķ� ����(3) �ղ� �ٱ�
ARGBcCc++C++gdi + GDI + mixer
�Ķ���ʾ��
��C++ͼ����ϵ���Դ����������ɶ���Ϊ����ȫ��ʹ��C++���롣
��Delphiͼ����ϵ����Ч��Ϊ���ص㣬һ�����ΪPASCAL�����Ĵ������BASM��
�����ܱ��ֶ�������һ�£����໥���ա�
���Ĵ�����������C++ͼ���� -- �������ͼ����ú����������е�BmpData.hͷ�ļ���
��ͼ��������У�ͼ��ĺϳɲ�����ʹ��Ƶ����ߵģ���ͼ����ʾ��ͼ�񿽱���ͼ��ƴ���Լ���ͼ��ƴ�ϵ��ӵȡ�
ͼ��ϳɣ���ʵҲ����ͼ��������ɫ�Ļ�ϣ���Photoshop�У���ɫ����Ǹ��ܸ��ӵĶ�������ͬ�Ļ��ģʽ����������ͬ�ĺϳ�Ч���������֮ȫ���о�͸�������ƾ͵�дһ���顣��ˣ�����̸̸ֻ�������ͼ��ϳɣ�Ҳ����Photoshop�е��������ģʽ��
ֻҪ�Ӵ���ͼ����ģ���֪���и�ͼ�����ػ�Ϲ�ʽ��
1��dstRGB = srcRGB * alpha + dstRGB * (1 - alpha)
    ���У�dstRGBΪĿ��ͼ����ֵ��srcRGBΪԴͼ����ֵ��alphaΪԴͼ����ֵ��ϱ�������͸���ȣ���Χ0 - 1����
    ��ʵ��������ػ�Ϲ�ʽ�кܴ�����ԣ�ֻ�ʺϲ���Alpha��Ϣ��ͼ��
    Ҫ���������Alphaͨ��ͼ�񣨲㣩�Ļ�ϣ��������Ĺ�ʽӦ���ǣ�
    2 - 1��srcRGB = srcRGB * srcAlpha * alpha / 255 ��Դͼ����Ԥ��ת��ΪPARGB��
        2 - 2��dstRGB = dstRGB * dstAlpha / 255 ��Ŀ��ͼ����Ԥ��ת��ΪPARGB��
            2 - 3��dstRGB = dstRGB + srcRGB - dstRGB * srcAlpha * alpha / 255 ��Դͼ����ֵ��Ŀ��ͼ����ֵ��ϣ�
                2 - 4��dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255 ����Ϻ��Ŀ��ͼAlphaͨ��ֵ��
                    2 - 5��dstRGB = dstRGB * 255 / dstAlpha ����Ϻ��Ŀ��ͼ����ת��ΪARGB��
                        ���У�dstRGBΪĿ��ͼ����ֵ��srcRGBΪԴͼ����ֵ��dstAlphaΪĿ��ͼAlphaͨ��ֵ��srcAlphaΪԴͼAlphaͨ��ֵ��dstARGBΪ��AlphaĿ��ͼ����ֵ��alphaΪԴͼ����ֵ��ϱ�������͸���ȣ���Χ0 - 1����
                        ����ʽ2�е�2 - 1ʽ����2 - 3ʽ���򻯿ɵã�
                        3 - 1��dstRGB = dstRGB * dstAlpha / 255
                            3 - 2��dstRGB = dstRGB + (srcRGB - dstRGB) * srcAlpha * alpha / 255
                                3 - 3��dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255
                                    3 - 4��dstRGB = dstRGB * 255 / dstAlpha
                                        ��dstAlpha = srcAlpha = 255ʱ����ʽ3��3 - 1ʽ��3 - 3ʽ��3 - 4ʽû�����壬3 - 2ʽҲ�仯Ϊ��
                                            4��dstRGB = dstRGB + (srcRGB - dstRGB) * alpha
                                                ���ѿ�������ʽ4�ǹ�ʽ1�ı��Ρ���ˣ���ʽ1ֻ�ǹ�ʽ3�����߹�ʽ2����Ŀ��ͼ��Դͼ������Alpha��Ϣ������Alpha = 255������µ�һ���������ѡ�
                                                    ����ʽ4�е�alpha = 1ʱ��Ŀ��ͼ���ص���Դͼ���أ����ԣ�����ǰ��˵ͼ�񿽱���ʵҲ��ͼ��ϳɵķ��롣
                                                        ͨ���������ϸ�ķ��������Կ�������ʹ�����������ͼ����ģʽҲ�Ǻܸ��ӵġ���ʵ�����滹���������ķ�������Ϊ����Ŀ��ͼAlpha��Ϣ��ԴͼAlpha��Ϣ�Լ�Դͼ�ϳɱ���������Ҫ�ص���ȫ��������ϣ�����������8����ʽ��
                                                        ����Ͱ��������ģʽ��ȫ��8���������2���غϣ�ʵ��Ϊ7����������ֱ���д���ʵ�֣�Ҳ�����ƺͲ������������������
                                                        [cpp] view plaincopyprint ?

                                                        FORCEINLINE
                                                        static VOID ARGBMixer(PARGBQuad pd, CONST PARGBQuad ps, INT alpha)
{
  pd->Blue += (((ps->Blue - pd->Blue) * alpha + 127) / 255);
  pd->Green += (((ps->Green - pd->Green) * alpha + 127) / 255);
  pd->Red += (((ps->Red - pd->Red) * alpha + 127) / 255);
}

FORCEINLINE
static VOID PARGBMixer(PARGBQuad pd, CONST PARGBQuad ps, INT alpha)
{
  pd->Blue = (pd->Blue * pd->Alpha + 127) / 255;
  pd->Green = (pd->Green * pd->Alpha + 127) / 255;
  pd->Red = (pd->Red * pd->Alpha + 127) / 255;
  pd->Blue += (((ps->Blue - pd->Blue) * alpha + 127) / 255);
  pd->Green += (((ps->Green - pd->Green) * alpha + 127) / 255);
  pd->Red += (((ps->Red - pd->Red) * alpha + 127) / 255);
  pd->Alpha += (alpha - (pd->Alpha * alpha + 127) / 255);
  pd->Blue = pd->Blue * 255 / pd->Alpha;
  pd->Green = pd->Green * 255 / pd->Alpha;
  pd->Red = pd->Red * 255 / pd->Alpha;
}

// source alpha = FALSE, dest alpha = FALSE, alpha < 255
static VOID Mixer0(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, alpha);
    }
  }
}

// source alpha = FALSE, dest alpha = FALSE, alpha = 255
// source alpha = FALSE, dest alpha = TRUE, alpha = 255
static VOID Mixer1(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, *pd ++ = *ps ++);
  }
}

// source alpha = FALSE, dest alpha = TRUE, alpha < 255
static VOID Mixer2(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      PARGBMixer(pd, ps, alpha);
    }
  }
}

// source alpha = TRUE, dest alpha = FALSE, alpha < 255
static VOID Mixer4(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, (alpha * ps->Alpha + 127) / 255);
    }
  }
}

// source alpha = TRUE, dest alpha = FALSE, alpha = 255
static VOID Mixer5(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, ps->Alpha);
    }
  }
}

// source alpha = TRUE, dest alpha = TRUE, alpha < 255
static VOID Mixer6(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      INT alpha0 = (alpha * ps->Alpha + 127) / 255;

      if (alpha0) {
        PARGBMixer(pd, ps, alpha0);
      }
    }
  }
}

// source alpha = TRUE, dest alpha = TRUE, alpha = 255
static VOID Mixer7(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      if (ps->Alpha) {
        PARGBMixer(pd, ps, ps->Alpha);
      }
    }
  }
}

typedef VOID (*MixerProc)(BitmapData*, CONST BitmapData*, INT);
// ͼ��ϳɡ�������32λĿ�����ݣ�32λԴ���ݣ���͸����(0 - 1.0)
VOID ImageMixer(BitmapData* dest, CONST BitmapData* source, FLOAT alpha)
{
  INT alphaI = (INT)(alpha * 255);

  if (alphaI <= 0) {
    return;
  }

  if (alphaI > 255) {
    alphaI = 255;
  }

  MixerProc proc[] = {Mixer0, Mixer1, Mixer2, Mixer1, Mixer4, Mixer5, Mixer6, Mixer7};
  INT index = (alphaI / 255) |
      (HasAlphaFlag(dest) << 1) |
      (HasAlphaFlag(source) << 2);
  proc[index](dest, source, alphaI);
}

����ImageMixer�������������ֱ�ΪĿ��ͼ���ݽṹ������GDI + ��BitmapData�ṹ��ָ�롢Դͼ���ݽṹָ���Դͼ���ػ�ϱ�������͸���ȣ�ȡֵ��ΧΪ0 - 1�����������е�proc���������ͼ���ϵ�ȫ��8��������Ӻ�������index�򰴻�ϱ�����Ŀ��ͼAlpha��Ϣ��ԴͼAlpha��Ϣ��ϳ��Ӻ��������±�ֵ��Alpha��Ϣ��BitmapData�ṹ�ı����ֶ��У���
��Ȼ����ʵ�ʵ������У�ȫ��8������ƺ��Ƕ��˵㣬�ɸ�����������ʵ��ϲ�ȡ�ᣬ�Լ�˴���ĸ��ӶȺ�ִ��Ч�ʡ�����������Ϊ�ȽϺ���ľ����ImageMixer������
[cpp] view plaincopyprint ?
// ͼ��ϳɡ�������32λĿ�����ݣ�32λԴ���ݣ���͸����(0 - 1.0)
VOID ImageMixer(BitmapData* dest, CONST BitmapData* source, FLOAT alpha)
{
  INT alphaI = (INT)(alpha * 255);

  if (alphaI <= 0) {
    return;
  }

  if (alphaI > 255) {
    alphaI = 255;
  }

  if (alphaI == 255 && !HasAlphaFlag(source)) {
    Mixer1(dest, source, alphaI); // �����ϳ�
  }
  else if (HasAlphaFlag(dest)) {
    Mixer6(dest, source, alphaI); // PARGB�ϳ�
  }
  else {
    Mixer4(dest, source, alphaI); // ARGB�ϳ�
  }
}

���ImageMixer����ֻ������3�������Ӻ��������У�Mixer6����ȫ���������ģʽ����ǰ�湫ʽ3��ʵ�֣�Mixer4Ϊ�Բ���Alpha��ϢĿ��ͼ�Ļ�ϣ����ڹ�ʽ4���������������˵��������Mixer1��Ϊ����ģʽ��
�����ǲ���BCB2010��GDI + ����ImageMixer���������ӣ�
[cpp] view plaincopyprint ?

void __fastcall TForm1::Button1Click(TObject* Sender)
{
  Gdiplus::Bitmap* dest = new Gdiplus::Bitmap(L"d:\\xmas_011.png");
  Gdiplus::Bitmap* source = new Gdiplus::Bitmap(L"d:\\Apple.png");
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(dest, 0, 0);
  g->DrawImage(source, dest->GetWidth(), 0);
  BitmapData dst, src;
  LockBitmap(dest, &dst);
  LockBitmap(source, &src);
  ImageMixer(&dst, &src, 0.75);
  UnlockBitmap(source, &src);
  UnlockBitmap(dest, &dst);
  g->DrawImage(dest, dest->GetWidth() << 1, 0);
  delete g;
  delete source;
  delete dest;
}
typedef union {
  FLOAT Elements[6];
  struct {
    FLOAT m11;
    FLOAT m12;
    FLOAT m21;
    FLOAT m22;
    FLOAT dx;
    FLOAT dy;
  };
} MatrixElements, *PMatrixElements;

struct TransformMatrix {
private:
  MatrixElements elements;
  VOID ElementsInit(MatrixElements& e) {
    e.m11 = e.m22 = 1.0f;
    e.m12 = e.m21 = e.dx = e.dy = 0.0f;
  }
  VOID ElementsMultiply(MatrixElements& e) {
    FLOAT m11 = elements.m11;
    FLOAT m12 = elements.m12;
    elements.m11 = e.m11 * m11 + e.m12 * elements.m21;
    elements.m12 = e.m11 * m12 + e.m12 * elements.m22;
    elements.m21 = e.m21 * m11 + e.m22 * elements.m21;
    elements.m22 = e.m21 * m12 + e.m22 * elements.m22;
  }
public:
  // ����һ����ʵ��������ʼ��Ϊ��λ���� Elements = 1,0,0,1,0,0
  TransformMatrix(VOID) {
    Reset();
  }
  // ����һ����ʵ����������matrix��Ԫ��
  TransformMatrix(TransformMatrix* matrix) {
    SetElements(matrix->elements);
  }
  TransformMatrix(TransformMatrix& matrix) {
    SetElements(matrix.elements);
  }
  // ����һ����ָ����Ԫ�س�ʼ������ʵ��
  TransformMatrix(FLOAT m11, FLOAT m12, FLOAT m21, FLOAT m22, FLOAT dx, FLOAT dy) {
    SetElements(m11, m12, m21, m22, dx, dy);
  }
  // ���ö���Ϊ��λ����
  VOID Reset(VOID) {
    ElementsInit(elements);
  }
  // ��������matrix���
  VOID Multiply(TransformMatrix* matrix) {
    elements.dx += (matrix->elements.dx * elements.m11 + matrix->elements.dy * elements.m21);
    elements.dy += (matrix->elements.dx * elements.m12 + matrix->elements.dy * elements.m22);
    ElementsMultiply(matrix->elements);
  }
  VOID Multiply(TransformMatrix& matrix) {
    Multiply(&matrix);
  }
  // ����ƽ��
  VOID Translate(FLOAT offsetX, FLOAT offsetY) {
    elements.dx += (offsetX * elements.m11 + offsetY * elements.m21);
    elements.dy += (offsetX * elements.m12 + offsetY * elements.m22);
  }
  // ��������
  VOID Scale(FLOAT scaleX, FLOAT scaleY) {
    MatrixElements e;
    ElementsInit(e);
    e.m11 = scaleX;
    e.m22 = scaleY;
    ElementsMultiply(e);
  }
  // ���ü��У�ע�ⲻҪ��shearX, shearYͬʱ����Ϊ1
  VOID Shear(FLOAT shearX, FLOAT shearY) {
    MatrixElements e;
    ElementsInit(e);
    e.m21 = shearX;
    e.m12 = shearY;
    ElementsMultiply(e);
  }
  // ���ð��Ƕ�angle��ԭ����ת
  VOID Rotate(FLOAT angle) {
    MatrixElements e;
    angle = angle * M_PI / 180.0f;
    e.m11 = e.m22 = cos(angle);
    e.m12 = sin(angle);
    e.m21 = -e.m12;
    e.dx = e.dy = 0.0f;
    ElementsMultiply(e);
  }
  // ���ð��Ƕ�angle�����ĵ�centerX, centerY��ת
  VOID RotateAt(FLOAT angle, FLOAT centerX, FLOAT centerY) {
    Translate(centerX, centerY);
    Rotate(angle);
    Translate(-centerX, -centerY);
  }
  // ����˶����ǿ���ת�ģ�����ת�ö��󣬷���TRUE�����򷵻�FALSE
  BOOL Invert(VOID) {
    double tmp = elements.m11 * elements.m22 - elements.m12 * elements.m21;

    if ((INT)(tmp * 1000.0f) == 0) {
      return FALSE;
    }

    tmp = 1.0f / tmp;
    FLOAT m11 = elements.m11;
    FLOAT dx = -elements.dx;
    elements.m11 = tmp * elements.m22;
    elements.m12 = tmp * -elements.m12;
    elements.m21 = tmp * -elements.m21;
    elements.m22 = tmp * m11;
    elements.dx = dx * elements.m11 - elements.dy * elements.m21;
    elements.dy = dx * elements.m12 - elements.dy * elements.m22;
    return TRUE;
  }
  // �������Ĵ�С���㲢����ʵʩ�任��ĳߴ�
  VOID GetTransformSize(INT width, INT height, FLOAT& fx, FLOAT& fy, FLOAT& fwidth, FLOAT& fheight) {
    FLOAT fxs[3], fys[3], v;
    fxs[1] = fys[0] = 0.0f;
    fxs[0] = fxs[2] = width;
    fys[1] = fys[2] = height;
    fx = fy = fwidth = fheight = 0.0f;

    for (INT i = 0; i < 3; i ++) {
      v = fxs[i] * elements.m11 + fys[i] * elements.m21;

      if (v < fx) {
        fx = v;
      }
      else if (v > fwidth) {
        fwidth = v;
      }

      v = fxs[i] * elements.m12 + fys[i] * elements.m22;

      if (v < fy) {
        fy = v;
      }
      else if (v > fheight) {
        fheight = v;
      }
    }

    fwidth -= fx;
    fheight -= fy;
    fx += elements.dx;
    fy += elements.dy;
  }
  // �������Ĵ�С���㲢����ʵʩ�任������������
  VOID GetTransformRect(INT width, INT height, RECT& r) {
    FLOAT fx, fy, fwidth, fheight;
    GetTransformSize(width, height, fx, fy, fwidth, fheight);
    r.left = (INT)fx;
    r.top = (INT)fy;
    r.right = (INT)(fwidth + fx + 0.999999f);
    r.bottom = (INT)(fheight + fy + 0.999999f);
  }
  // �жϴ˶����Ƿ��ǵ�λ����
  BOOL GetIdentity(VOID) {
    return (elements.m11 == 1.0f &&
        elements.m22 == 1.0f &&
        elements.m12 == 0.0f &&
        elements.m21 == 0.0f &&
        elements.dx == 0.0f &&
        elements.dy == 0.0f);
  }
  // ��ȡ�����xƫ����
  FLOAT GetOffsetX(VOID) {
    return elements.dx;
  }
  // ��ȡ�����yƫ����
  FLOAT GetOffsetY(VOID) {
    return elements.dy;
  }
  // �ж϶����Ƿ��ǿ���ת�ġ�
  BOOL GetInvertible(VOID) {
    return (INT)(1000.0f * (elements.m11 * elements.m22 - elements.m12 * elements.m21)) != 0;
  }
  // ��ȡ����Ԫ��
  MatrixElements& GetElements(VOID) {
    return elements;
  }
  // ���ö���Ԫ�ء�ע������Ԫ���Ǹ�����ʽ��
  VOID SetElements(CONST MatrixElements& value) {
    SetElements(value.m11, value.m12, value.m21, value.m22, value.dx, value.dy);
  }
  VOID SetElements(FLOAT m11, FLOAT m12, FLOAT m21, FLOAT m22, FLOAT dx, FLOAT dy) {
    elements.m11 = m11;
    elements.m12 = m12;
    elements.m21 = m21;
    elements.m22 = m22;
    elements.dx = dx;
    elements.dy = dy;
  }
};

��������ж�����һ�����α任�����Ա����MatrixElements������ʵ�ʱ��ʱ��ȡ�����ü��α任�����Ա��TransformMatrixֻ�Ǽ򵥵Ķ�������˷�װ����ͨ������ʵ���йصļ��α任��
TransformMatrix�ĺ��Ĵ�����Multiply��������ElementsMultiply��������Invert������
Multiply��������ɸ��ָ��ӵļ��α任���㣬�����ܹ�ʵ�ֵľ��弸�α任���ǿ���ͨ������ɵģ������е�ƽ�ƺ���TranslateҲ����ͨ������ɵģ���Ȼ����һЩ����Ҫ�ļ��㣩����˵���ı����ǡ�ʵ��������ͼ��ƽ�漸�α任������TransformMatrix�еļ��ֻ����ı任������������ȫ���ļ��α任����ԳƼ��α任�����񣩣�������˵���ӵ���ϱ任���⵹���Ǳ���Ҫ�������⵳��������˵�ġ�ʵ��������ͼ�񼸺α任������ָ����ͨ��Multiply�������߸�ֱ�ӵı任�����Ա����ȥʵ�֡������ġ�ͼ�񼸺α任�������䲻��ʹ��ƽ�漸�α任������������������α任�Ҿ�û�뵽��ôʵ�֣�Ҳ���䳬����ƽ�漸�α任���󷶳룿�������߲��ܽ���ʵ�ʵļ��α任�������棩����ʵ��������ͼ�񼸺α任������һ�㺬���������ͼ��任ִ�к�����ʵ��TransformMatrix���ܱ�ʾ������ͼ�񼸺α任��������ȥдһ��������ģ������š���ת�任�����ȡ�
Invert����ʵ���˱任����������ͨ��������α任����󣬿��Ժܷ����ʵ��ͼ��ͼ�񼸺α任��ʵ�ʲ�����ΪʲôҪ�����α任���������󣬶�����ֱ�����ݱ任������ʵ��ͼ��ͼ�񼸺α任��ʵ�ʲ����أ���Ϊ���α任�����ʾ����˼�ǣ���Դͼ������������ͨ�����α任��ͶӰ��Ŀ��ͼ����ΪԴͼ������ͨ�����α任����Ŀ��ͼ���ϵ����ص��п��ܲ���һһ��Ӧ����ͼ�����ű任�󣬲��Ƕ��Դͼ�����ص��Ӧͬһ��Ŀ�����ص㣨��С��������Դͼ�����ص㲻�������ȫ����Ŀ�����ص㣨�Ŵ󣩣�����п������Ŀ��ͼ�����ص㱻�ظ����ƻ��߱���©���������������α任���������ʾ����˼�ǣ�����Ŀ��ͼ������һ�����ص㣬����ڼ��α任ǰ��Դͼ�����ص������Ӧ������и��ơ�����Ŀ��ͼ�����ص���ܱ�֤Ŀ��ͼ�����ص�Ȳ��ظ���Ҳ����©�ı����ơ�
������һ��ͼ�񼸺α任�������룺
[cpp] view plaincopyprint ?
// ִ��ͼ�����ݼ��α任
VOID Transform(BitmapData* dest, INT x, INT y, CONST BitmapData* source, TransformMatrix* matrix)
{
  // ���Ƽ��α任�������
  TransformMatrix m(matrix);
  // ���α任�����������ƽ����x, y
  m.GetElements().dx += x;
  m.GetElements().dy += y;
  // �����α任������㲢��ȡĿ��ͼ������������
  float fx, fy, fwidth, fheight;
  m.GetTransformSize(source->Width, source->Height, fx, fy, fwidth, fheight);
  BitmapData dst;

  if (!GetBitmapData(dest, (INT)fx, (INT)fy,
      (INT)(fwidth + 0.999999f), (INT)(fheight + 0.999999f), &dst)) {
    return;
  }

  // ��ȡ���α任�����
  if (!m.Invert()) {
    return;
  }

  // �����ͼ������Ŀ��ͼ��ԭ�㲻һ�£����α任�����������ƽ����fx, fy
  if (fx > 0.0f || fy > 0.0f) {
    if (fx < 0.0f) {
      fx = 0.0f;
    }
    else if (fy < 0.0f) {
      fy = 0.0f;
    }

    m.Translate(fx, fy);
  }

  // ������ͼɨ����ָ�뼰��ƫ�ƿ��
  UINT* pix = (UINT*)dst.Scan0;
  INT dstOffset = (dst.Stride >> 2) - dst.Width;
  // ���α任������ƽ����Ϊ����ͼԭ���Ӧ��Դͼ��ʼ�����
  MatrixElements e = m.GetElements();
  float xs = e.dx;
  float ys = e.dy;

  // �����㲢����Դͼ���α任������ݵ�Ŀ����ͼ
  for (y = 0; y < (INT)dst.Height; y ++, pix += dstOffset, xs += e.m21, ys += e.m22) {
    float xs0 = xs;
    float ys0 = ys;

    for (x = 0; x < (INT)dst.Width; x ++, pix ++, xs0 += e.m11, ys0 += e.m12) {
      INT x0 = xs0 < 0.0f ? (INT)(xs0 - 0.5f) : (INT)(xs0 + 0.5f);
      INT y0 = ys0 < 0.0f ? (INT)(ys0 - 0.5f) : (INT)(ys0 + 0.5f);

      if (y0 >= 0 && y0 < (INT)source->Height && x0 >= 0 && x0 < (INT)source->Width) {
        *pix = *(UINT*)((CHAR*)source->Scan0 + y0 * source->Stride + (x0 << 2));
      }
    }
  }
}
����ͼ�񼸺α任�����ļ����ص㣺
1������ʵ�������ͼ�񼸺α任��ֻҪTransformMatrix����ȷ���ģ����任������棩��
2��������GDI + ��BitmapData�ṹ��ת��Ϊ32λARGB���ظ�ʽ�����������κξ����ͼ���ʽ����֤����ͨ���ԣ�
3������ʹ�ø��������㣬���ڼ������ص�λ��ʱ������ͨ���ĸ������˳����㣬�������һ���������ٶȣ�ҲΪ�Ժ��޸�Ϊ����������춨�˻�����
4�����������ٽ����ز�ֵ����û�б߽����ش�����룬���ظ��������ϲ
���Կ�����Transform���������ص������ص㣨1������ʵ�ʵ�ʵ�ִ����У����԰�����Ϊһ����ܽ���������޸ġ�
������һ������Transform������GDI + λͼ������ת�任�����ӣ�ʹ��BCB2007����
[cpp] view plaincopyprint ?
void __fastcall TForm1::Button1Click(TObject* Sender)
{
  // ��ȡԴͼ��ɨ��������
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\IMG_9440_mf.jpg");
  BitmapData source, dest;
  LockBitmap(bmp, &source);
  // ���ü��α任
  TransformMatrix matrix;
  matrix.Rotate(45);
  // ����Ŀ��λͼ����ȡ��ɨ��������
  RECT r;
  matrix.GetTransformRect(source.Width, source.Height, r);
  Gdiplus::Bitmap* newBmp = new Gdiplus::Bitmap(
      r.right - r.left, r.bottom - r.top, PixelFormat32bppARGB);
  LockBitmap(newBmp, &dest);
  // ִ��ͼ�񼸺α任
  Transform(&dest, 0, 0, &source, &matrix);
  // �ͷ�ͼ��ɨ��������(λͼ����)
  UnlockBitmap(newBmp, &dest);
  UnlockBitmap(bmp, &source);
  // �����α任���ͼ��
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(newBmp, 0, 0);
  delete g;
  delete newBmp;
  delete bmp;
}

������ͼ����ת�任�������н����ͼ��
��ת45��
����ͼ�񼸺α任����Դͼԭ�㣨0��0��Ϊ�任ԭ�㣬���Խ�����ֻ�ܿ���ԭ�����±ߵ�ͼ�񡣻���Щ���α任������ת90�ȡ�180�ȵȣ����ܻᵼ�¼��α任���ͼ����ȫ���ɼ���Ϊ��ֱ�۵Ŀ������ּ��α任�������ͼ�񣬿����޸�һ�����Ӵ��룬��TransformMatrix::GetTransformRect�������ؾ��ε����ϱ߲���Ҳ����������
[cpp] view plaincopyprint ?
void __fastcall TForm1::Button1Click(TObject* Sender)
{
  // ��ȡԴͼ��ɨ��������
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\IMG_9440_mf.jpg");
  BitmapData source, dest;
  LockBitmap(bmp, &source);
  // ���ü��α任
  TransformMatrix matrix;
  matrix.Rotate(45);
  // ����Ŀ��λͼ����ȡ��ɨ��������
  RECT r;
  matrix.GetTransformRect(source.Width, source.Height, r);
  Gdiplus::Bitmap* newBmp = new Gdiplus::Bitmap(
      r.right - r.left, r.bottom - r.top, PixelFormat32bppARGB);
  LockBitmap(newBmp, &dest);
  // ִ��ͼ�񼸺α任��
  // ע������ʹ��-r.left, -r.topΪ���꣬ʹ�ñ任���ͼ����ȫ�ɼ�
  Transform(&dest, -r.left, -r.top, &source, &matrix);
  // �ͷ�ͼ��ɨ��������(λͼ����)
  UnlockBitmap(newBmp, &dest);
  UnlockBitmap(bmp, &source);
  // �����α任���ͼ��
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(newBmp, 0, 0);
  delete g;
  delete newBmp;
  delete bmp;
}

C++ͼ���� -- ͼ��ڰ׵���Ӧ��
���ࣺ C / C++ C / C++ͼ��ͼ�� 2011 - 07 - 10 15: 05 4505���Ķ� ����(18) �ղ� �ٱ�
cCc++C++gdi + GDI + ͼ����ɫͼ��ڰ׵���
�Ķ���ʾ��
��C++ͼ����ϵ���Դ����������ɶ���Ϊ����ȫ��ʹ��C++���롣
��Delphiͼ����ϵ����Ч��Ϊ���ص㣬һ�����ΪPASCAL�����Ĵ������BASM��
�����ܱ��ֶ�������һ�£����໥���ա�
���Ĵ�����������C++ͼ���� -- �������ͼ����ú����������е�BmpData.hͷ�ļ���
Photoshop CS��ͼ��ڰ׵������ܣ���ͨ���Ժ졢�ơ��̡��ࡢ��������6����ɫ�ı�����������ɵġ��ܸ���ϸ�ؽ���ɫͼƬת��Ϊ�������ĺڰ���Ƭ��
Photoshop CSͼ��ڰ׵������ܵļ��㹫ʽΪ��
gray = (max - min) * ratio_max + (mid - min) * ratio_max_mid + min
    ��ʽ�У�grayΪ���ػҶ�ֵ��max��mid��min�ֱ�Ϊͼ������R��G��B������ɫ�����ֵ���м�ֵ����Сֵ��ratio_maxΪmax������ķ�����ɫ����ɫ�����ʣ�ratio_max_mid��Ϊmax��mid���ַ�����ɫ���γɵĸ�ɫ���ʡ�
    �����湫ʽ����ĻҶ�ֵ��������ͨ�����õĻҶȼ��㷽���кܴ�ͬ��ͨ�����õĻҶȹ�ʽΪ����ֱ�ӽ���ɫ������������Ӧ�ı�����Ӷ��ɣ��磺gray = 0.3R + 0.59G + 0.11B�������湫ʽ��������Сֵ�������ɫ���������ϣ������ֵ����Сֵ֮���ʾ��ɫ���֣��졢�̡����������м�ֵ����Сֵ֮���ʾ��ɫ���֣��ơ��ࡢ��죩������ɫ�͸�ɫ���ֱַ������֮��Ӧ�ı��ʺ���ӣ��ټ�����Сֵ���õ��Ҷ�ֵ������ÿ��������������˵������Ҷ�ֵֻ��Ҫ�õ�����6����ɫ�����е�2�ּ��ɡ��ڼ�������пɸ�������RGB�໥��ϵѡ���Ӧ�ĵ�ɫ�͸�ɫ���ʣ�������RGB�Ĵ�С��ϵΪR > G > B����ɫ����ѡ���ֵR��ɫ����ɫ������Ϊ���ֵR���м�ֵG���γɵĸ�ɫ��ɫ��
        �ó������ʵ������ĻҶȼ��㹫ʽ�������ӣ��ѵ㻹��ǰ����˵�ĸ�������RGB�໥��ϵѡ���Ӧ�ĵ�ɫ�͸�ɫ���ʡ���ǰ����д�ġ�C++ͼ���� -- ͼ����ɫ��ϣ��ϣ��������У��Ѿ�ʵ��������ܣ�ͬʱ��Photoshopͼ��ڰ׵��������и��ӵ���ɫ���ܣ�Ҳ��������ʵ�֡����ĵ����������´�������ϣ���дһ����Լ򵥵�ͼ��ڰ׵������棬��ʵ��ͼ��̬�ڰ׵�����
        ��������BCB2007д��һ�����������룺
        ����ͷ�ļ����֣�
        [cpp] view plaincopyprint ?

#ifndef bwMainH
#define bwMainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#define USE_GDIPLUS
#include "BmpData.h"

        enum TLockType {ltEdit, ltTrack};
typedef Set<TLockType, ltEdit, ltTrack> TLockTypes;
struct TForm1 : public TForm {
__published: // IDE-managed Components
  TPaintBox* PaintBox1;
  TLabel* Label1;
  TLabel* Label2;
  TLabel* Label3;
  TLabel* Label4;
  TLabel* Label5;
  TLabel* Label6;
  TLabel* Label7;
  TLabel* Label8;
  TLabel* Label9;
  TLabel* Label10;
  TLabel* Label11;
  TLabel* Label12;
  TLabel* Label13;
  TLabel* Label18;
  TComboBox* ComboBox1;
  TEdit* Edit1;
  TTrackBar* TrackBar1;
  TEdit* Edit2;
  TTrackBar* TrackBar2;
  TEdit* Edit3;
  TTrackBar* TrackBar3;
  TEdit* Edit4;
  TTrackBar* TrackBar4;
  TEdit* Edit5;
  TTrackBar* TrackBar5;
  TEdit* Edit6;
  TTrackBar* TrackBar6;
  TCheckBox* CheckBox1;
  TGroupBox* GroupBox1;
  TLabel* Label14;
  TLabel* Label15;
  TLabel* Label16;
  TLabel* Label17;
  TPaintBox* PaintBox2;
  TEdit* Edit7;
  TTrackBar* TrackBar7;
  TEdit* Edit8;
  TTrackBar* TrackBar8;
  TColorDialog* ColorDialog1;
  void __fastcall FormCreate(TObject* Sender);
  void __fastcall FormDestroy(TObject* Sender);
  void __fastcall ComboBox1Change(TObject* Sender);
  void __fastcall TrackBar1Change(TObject* Sender);
  void __fastcall Edit1Change(TObject* Sender);
  void __fastcall Edit1KeyPress(TObject* Sender, char& Key);
  void __fastcall Edit1Exit(TObject* Sender);
  void __fastcall CheckBox1Click(TObject* Sender);
  void __fastcall TrackBar7Change(TObject* Sender);
  void __fastcall Edit7Change(TObject* Sender);
  void __fastcall Edit7KeyPress(TObject* Sender, char& Key);
  void __fastcall PaintBox2Click(TObject* Sender);
  void __fastcall PaintBox1Paint(TObject* Sender);
  void __fastcall PaintBox2Paint(TObject* Sender);
  void __fastcall TrackBar8Change(TObject* Sender);
private: // User declarations
  Bitmap* Source; // Դͼ��
  Bitmap* Dest; // �������ͼ��
  BitmapData srcData;
  BitmapData dstData;
  float bwColors[6]; // �Ҷ�ѡ������
  int Bright; // ����
  TTrackBar* TrackBars[6]; // �Ҷ�ѡ����Ԫ������
  TEdit* Edits[6]; // �Ҷ�ѡ��༭������
  TLockTypes Lock;
  Gdiplus::Rect rect;
  ARGBQuad MixColor; // �����ɫ
  int __fastcall GetHue(void);
  int __fastcall GetSat(void);
  void __fastcall SetHue(int hue);
  void __fastcall SetSat(int sat);
  void __fastcall MixColorToHSV(void);
  void __fastcall HSVToMixColor(void);
  void __fastcall Execute(void);
  void __fastcall MixColorChange(void);
public: // User declarations
  __fastcall TForm1(TComponent* Owner);
  __property int Hue = {read = GetHue, write = SetHue}; // ɫ��
  __property int Sat = {read = GetSat, write = SetSat}; // ���Ͷ�
};

const CustomIndex = 11; // �Զ���ѡ������
const DefaultTint = 0xe1d3b3; // ȱʡ�����ɫ
const int DefOptions[][6] = { // Ԥ����Ҷ�ѡ��
  {40, 60, 40, 60, 20, 80},
  {128, 128, 100, 100, 128, 100},
  {100, 100, 100, 100, 100, 100},
  {0, 0, 0, 0, 0, 0},
  { -40, 235, 144, -68, -3, -107},
  {120, 110, -10, -50, 0, 120},
  {50, 120, 90, 50, 0, 0},
  {0, 0, 0, 110, 110, 110},
  {120, 120, -10, -50, -50, 120},
  { -50, -50, -50, 150, 150, 150},
  {120, 110, 40, -30, 0, 70}
};
extern PACKAGE TForm1* Form1;

#endif
�����ļ����֣�
[cpp] view plaincopyprint ?

#include <vcl.h>
#pragma hdrstop
#include "bwMain.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}

ULONG gdiplusToken;
typedef FLOAT BWParams, *PBWParams;
// �ڰ׵���ȱʡ�������죬�ƣ��̣���죬������
CONST INT _BWDefault[] = {410, 614, 410, 819, 205, 614};
enum {
  BWIndexBlue = 0x40000,
  BWIndexGreen = 0x20000,
  BWIndexRed = 0x00000
};
enum {
  IndexBlue = 0x00000,
  IndexGreen = 0x10000,
  IndexRed = 0x20000
};
typedef union { // ��ɫ���������ṹ
  INT tmp; // ����ʱ�õ���ʱ����
  struct {
    SHORT value; // ��ɫ����ֵ
    SHORT index; // ��ɫ��������
  };
} RGBIndex;

// �������ط���
FORCEINLINE
VOID SwapRgb(RGBIndex& a, RGBIndex& b)
{
  a.tmp ^= b.tmp;
  b.tmp ^= a.tmp;
  a.tmp ^= b.tmp;
}

// ��ȡ�ڰ׻Ҷ�
FORCEINLINE iNT GetBWGray(CONST PARGBQuad pixel, CONST PINT bwParams)
{
  RGBIndex max, mid, min;
  min.tmp = pixel->Blue | BWIndexBlue;
  mid.tmp = pixel->Green | BWIndexGreen;
  max.tmp = pixel->Red | BWIndexRed;

  if (max.value < mid.value) {
    SwapRgb(max, mid);
  }

  if (max.value < min.value) {
    SwapRgb(max, min);
  }

  if (min.value > mid.value) {
    SwapRgb(min, mid);
  }

  return (((max.value - mid.value) * bwParams[max.index] +
      (mid.value - min.value) * bwParams[max.index + mid.index - 1] +
      512) >> 10) + min.value;
}

VOID ColorMix(PARGBQuad pd, CONST PARGBQuad ps, INT gray)
{
  // �Ҷȼ��㳣���������̡���
  CONST INT ys[3] = {113, 604, 307};
  RGBIndex max, mid, min;
  min.tmp = ps->Blue | IndexBlue;
  mid.tmp = ps->Green | IndexGreen;
  max.tmp = ps->Red | IndexRed;

  if (max.value < mid.value) {
    SwapRgb(max, mid);
  }

  if (max.value < min.value) {
    SwapRgb(max, min);
  }

  if (min.value > mid.value) {
    SwapRgb(min, mid);
  }

  INT max_min = max.value - min.value;

  // ���Ͷ�Ϊ0�����ػҶ�
  if (max_min == 0) {
    pd->Blue = pd->Green = pd->Red = gray;
    return;
  }

  INT mid_min = mid.value - min.value;
  INT newMax, newMid, newMin;
  gray <<= 10;
  newMax = (gray + (max_min - mid_min) * ys[mid.index] + max_min * ys[min.index] + 512) >> 10;
  newMin = newMax - max_min;

  if (newMax > 255) {
    INT hueCoef = (mid_min << 10) / max_min;
    INT v0 = (ys[mid.index] * hueCoef) >> 10;
    INT v1 = ys[min.index] + ys[mid.index] - v0;
    newMin = (gray - (ys[max.index] + v0) * 255 + (v1 >> 1)) / v1;
    newMid = newMin + (((255 ^ newMin) * hueCoef + 512) >> 10);
    newMax = 255;
  }
  else if (newMin < 0) {
    INT hueCoef = (mid_min << 10) / max_min;
    INT tmp = ys[max.index] + ((ys[mid.index] * hueCoef + 512) >> 10);
    newMax = (gray + (tmp >> 1)) / tmp;
    newMid = (newMax * hueCoef + 512) >> 10;
    newMin = 1;
  }
  else {
    newMid = newMin + mid_min;
  }

  ((LPBYTE)pd)[max.index] = newMax;
  ((LPBYTE)pd)[mid.index] = newMid;
  ((LPBYTE)pd)[min.index] = newMin;
}

// ͼ��ڰ׵�����
// ��������bwParamsΪԪ��������6������ָ�룬�ֱ�Ϊ�죬�ƣ��̣��࣬�������
VOID ImageBWCopy(BitmapData* dest, CONST BitmapData* source, CONST PBWParams bwParams = NULL)
{
  // �������ػҶȲ�������������ɫ�����ɫ
  INT params[6], *pparams;

  if (bwParams) {
    for (INT i = 0; i < 6; i ++) {
      params[i] = (INT)(bwParams[i] * 1024 + 0.5);
    }

    params[3] ^= params[5];
    params[5] ^= params[3];
    params[3] ^= params[5];
    pparams = params;
  }
  else {
    pparams = (INT*)_BWDefault;
  }

  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      INT gray = GetBWGray(ps, pparams);
      pd->Blue = pd->Green = pd->Red =
          (gray & ~0xff) == 0 ? gray : gray > 255 ? 255 : 0;
    }
  }
}

// �Ҷ�ͼ��Ⱦɫ��
VOID ImageTint(BitmapData* grayData, ARGB color)
{
  ARGBQuad colorTable[256];
  PARGBQuad p = colorTable;

  for (INT i = 0; i < 256; i ++, p ++) {
    ColorMix(p, (PARGBQuad)&color, i);
    p->Alpha = 255;
  }

  p = (PARGBQuad)grayData->Scan0;
  INT dataOffset = (grayData->Stride >> 2) - (INT)grayData->Width;

  for (UINT y = 0; y < grayData->Height; y ++, p += dataOffset) {
    for (UINT x = 0; x < grayData->Width; x ++, p ++) {
      p->Color = colorTable[p->Blue].Color;
    }
  }
}

void __fastcall TForm1::FormCreate(TObject* Sender)
{
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  TrackBars[0] = TrackBar1;
  TrackBars[1] = TrackBar2;
  TrackBars[2] = TrackBar3;
  TrackBars[3] = TrackBar4;
  TrackBars[4] = TrackBar5;
  TrackBars[5] = TrackBar6;
  Edits[0] = Edit1;
  Edits[1] = Edit2;
  Edits[2] = Edit3;
  Edits[3] = Edit4;
  Edits[4] = Edit5;
  Edits[5] = Edit6;
  // ���ļ�װ��ͼ��tmp
  Bitmap* tmp = new Bitmap(L"source1.jpg");
  rect.Width = tmp->GetWidth();
  rect.Height = tmp->GetHeight();
  // �ֱ����µ�Դ��Ŀ��ͼ�����ݵ�srcData��dstData
  GetBitmapData(rect.Width, rect.Height, &srcData);
  GetBitmapData(rect.Width, rect.Height, &dstData);
  // ��tmpͼ�����ݷֱ�����������srcData��dstData
  tmp->LockBits(&rect,
      ImageLockModeRead | ImageLockModeWrite | ImageLockModeUserInputBuf,
      PixelFormat32bppARGB, &srcData);
  tmp->UnlockBits(&srcData);
  tmp->LockBits(&rect,
      ImageLockModeRead | ImageLockModeWrite | ImageLockModeUserInputBuf,
      PixelFormat32bppARGB, &dstData);
  tmp->UnlockBits(&dstData);
  delete tmp;
  // �ֱ���ͼ������srcData��dstData����λͼSource��Dest
  // ע��ͼ�����ݽṹ�������ݴ���λͼ������ʾ���������ɰ����ݽṹ��λͼ��
  // ���ܱ���ÿ�δ���ͼ������ʱ�������ͽ�������
  Source = new Bitmap(srcData.Width, srcData.Height, srcData.Stride,
      PixelFormat32bppARGB, (BYTE*)srcData.Scan0);
  Dest = new Bitmap(dstData.Width, dstData.Height, dstData.Stride,
      PixelFormat32bppARGB, (BYTE*)dstData.Scan0);
  ComboBox1Change(NULL);
}

void __fastcall TForm1::FormDestroy(TObject* Sender)
{
  delete Dest;
  delete Source;
  FreeBitmapData(&dstData);
  FreeBitmapData(&srcData);
  GdiplusShutdown(gdiplusToken);
}

// ִ��ͼ��ڰ׵���
void __fastcall TForm1::Execute(void)
{
  for (int i = 0; i < 6; i ++) { // ��ȡ�Ҷ�ѡ��������
    bwColors[i] = TrackBars[i]->Position / 100.0;
  }

  ImageBWCopy(&dstData, &srcData, bwColors); // Դͼ�ڰ׵�����Ŀ��ͼ

  if (CheckBox1->Checked && Sat) { // ���ɫ��ѡ�ѡ����ɫ
    ImageTint(&dstData, MixColor.Color);
  }

  PaintBox1Paint(NULL); // ��ʾͼ��
}

// Ԥ��ڰ׵���ѡ��ı�
void __fastcall TForm1::ComboBox1Change(TObject* Sender)
{
  if (ComboBox1->ItemIndex == CustomIndex) {
    return;
  }

  MixColor.Color = DefaultTint; // ����ȱʡ�����ɫ
  MixColorToHSV(); // ���㲢����ȱʡɫ�ࡢ���Ͷȿؼ�
  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    for (int i = 0; i < 6; i ++) { // װ��Ԥ���ѡ�����ݵ���Ӧ�Ŀؼ�
      TrackBars[i]->Position = DefOptions[ComboBox1->ItemIndex][i];
      Edits[i]->Text = DefOptions[ComboBox1->ItemIndex][i];
    }

    if (CheckBox1->Checked) {
      CheckBox1->Checked = FALSE; // ȡ��ɫ��ѡ��
    }
    else {
      Execute();
    }
  }

  __finally {
    Lock.Clear();
  }
}

// �ڰ׵�������ѡ�����ı�
void __fastcall TForm1::TrackBar1Change(TObject* Sender)
{
  if (Lock.Contains(ltTrack)) {
    return;
  }

  Lock = TLockTypes() << ltEdit;

  try {
    TTrackBar* bar = (TTrackBar*)Sender;
    Edits[bar->Tag]->Text = bar->Position;
    ComboBox1->ItemIndex = CustomIndex; // Ԥ������������Ϊ�Զ���
    Execute();
  }

  __finally {
    Lock.Clear();
  }
}

// �ڰ׵������ݱ༭��ı�
void __fastcall TForm1::Edit1Change(TObject* Sender)
{
  if (Lock.Contains(ltEdit)) {
    return;
  }

  TEdit* edit = (TEdit*)Sender;

  if (edit->Text != "" && edit->Text != "-") {
    TrackBars[edit->Tag]->Position = StrToInt(edit->Text);
  }
}

void __fastcall TForm1::Edit1KeyPress(TObject* Sender, char& Key)
{
  if (Key >= ' ' && Key != '-' && (Key < '0' || Key > '9')) {
    Key = 0;
  }
}

void __fastcall TForm1::Edit1Exit(TObject* Sender)
{
  TEdit* edit = (TEdit*)Sender;

  if (edit->Text == "") {
    edit->Text = TrackBars[edit->Tag]->Position;
  }
}

// �����ɫ�ı䣬�������ɫ����ʾ��RGBֵ
void __fastcall TForm1::MixColorChange(void)
{
  PaintBox2Paint(NULL);
  Label18->Caption = "R: " + IntToStr(MixColor.Red) +
      ", G: " + MixColor.Green +
      ", B: " + MixColor.Blue;
  Execute();
}

inline void RgbSwap(int& a, int& b)
{
  a ^= b;
  b ^= a;
  a ^= b;
}
// �������ɫ���㲢�ı�HSV
void __fastcall TForm1::MixColorToHSV(void)
{
  int max, mid, min;
  max = MixColor.Red;
  mid = MixColor.Green;
  min = MixColor.Blue;

  if (max < mid) {
    RgbSwap(max, mid);
  }

  if (max < min) {
    RgbSwap(max, min);
  }

  if (min > mid) {
    RgbSwap(min, mid);
  }

  int max_min = max - min;

  if (max_min == 0) {
    Hue = 0;
    Sat = 0;
  }
  else {
    int H;

    if (max == MixColor.Red) {
      H = ((MixColor.Green - MixColor.Blue) * 60 + 30) / max_min;
    }
    else if (max == MixColor.Green) {
      H = ((MixColor.Blue - MixColor.Red) * 60 + 30) / max_min + 120;
    }
    else {
      H = ((MixColor.Red - MixColor.Green) * 60 + 30) / max_min + 240;
    }

    Hue = H < 0 ? H + 360 : H;
    Sat = (max_min * 100) / max;
  }

  Bright = max;
}

inline ARGB RgbToColor(int r, int g, int b)
{
  return (r << 16) | (g << 8) | b;
}
// ��HSV���㲢�ı�����ɫ
void __fastcall TForm1::HSVToMixColor(void)
{
  if (Sat == 0) {
    MixColor.Blue = MixColor.Green = MixColor.Red = Bright;
  }
  else {
    int index = Hue / 60;
    int f = Hue % 60;

    if ((index & 1) == 0) {
      f = 60 - f;
    }

    int a = Bright;
    int b = (Bright * (6000 - Sat * f)) / 6000;
    int c = (Bright * (100 - Sat)) / 100;

    switch (index) {
    case 0:
      MixColor.Color = RgbToColor(a, b, c);
      break;

    case 1:
      MixColor.Color = RgbToColor(b, a, c);
      break;

    case 2:
      MixColor.Color = RgbToColor(c, a, b);
      break;

    case 3:
      MixColor.Color = RgbToColor(c, b, a);
      break;

    case 4:
      MixColor.Color = RgbToColor(b, c, a);
      break;

    case 5:
      MixColor.Color = RgbToColor(a, c, b);
    }
  }

  MixColorChange();
}

int __fastcall TForm1::GetHue(void)
{
  return TrackBar7->Position;
}

int __fastcall TForm1::GetSat(void)
{
  return TrackBar8->Position;
}

void __fastcall TForm1::SetHue(int hue)
{
  if (Hue == hue) {
    return;
  }

  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    TrackBar7->Position = hue;
    Edit7->Text = hue;
  }

  __finally {
    Lock.Clear();
  }
}

void __fastcall TForm1::SetSat(int sat)
{
  if (Sat == sat) {
    return;
  }

  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    TrackBar8->Position = sat;
    Edit8->Text = sat;
  }

  __finally {
    Lock.Clear();
  }
}

// ɫ��ѡ�иı�
void __fastcall TForm1::CheckBox1Click(TObject* Sender)
{
  Label14->Enabled = CheckBox1->Checked;
  Label15->Enabled = CheckBox1->Checked;
  Label16->Enabled = CheckBox1->Checked;
  Label17->Enabled = CheckBox1->Checked;
  Label18->Visible = CheckBox1->Checked;
  Edit7->Enabled = CheckBox1->Checked;
  Edit8->Enabled = CheckBox1->Checked;
  TrackBar7->SliderVisible = CheckBox1->Checked;
  TrackBar8->SliderVisible = CheckBox1->Checked;

  if (CheckBox1->Checked) {
    ComboBox1->ItemIndex = CustomIndex;
  }

  MixColorChange();
}

// ɫ��ѡ�����ı�
void __fastcall TForm1::TrackBar7Change(TObject* Sender)
{
  if (!Lock.Contains(ltTrack)) {
    Edit7->Text = TrackBar7->Position;
  }
}

// ���Ͷ�ѡ�����ı�
void __fastcall TForm1::TrackBar8Change(TObject* Sender)
{
  if (!Lock.Contains(ltTrack)) {
    Edit8->Text = TrackBar8->Position;
  }
}

// ɫ����߱��Ͷȱ༭��ı�
void __fastcall TForm1::Edit7Change(TObject* Sender)
{
  TEdit* edit = (TEdit*)Sender;

  if (Lock.Contains(ltEdit) || edit->Text == "") {
    return;
  }

  Lock = TLockTypes() << ltTrack;

  try {
    int val = StrToInt(edit->Text);
    TTrackBar* bar = edit->Tag == 0 ? TrackBar7 : TrackBar8;

    if (bar->Position != val) {
      bar->Position = val;
    }

    HSVToMixColor();
  }

  __finally {
    Lock.Clear();
  }
}

void __fastcall TForm1::Edit7KeyPress(TObject* Sender, char& Key)
{
  if (Key >= ' ' && (Key < '0' || Key > '9')) {
    Key = 0;
  }
}

// ������ɫ�Ի���ѡ������ɫ
void __fastcall TForm1::PaintBox2Click(TObject* Sender)
{
  if (CheckBox1->Checked && ColorDialog1->Execute(Handle)) {
    MixColor.Color = (ARGB)ColorDialog1->Color;
    MixColor.Blue = MixColor.Red;
    MixColor.Red = (BYTE)ColorDialog1->Color;
    MixColorToHSV();
    MixColorChange();
  }
}

// ���ڰ׵���ͼ���Դͼ��
void __fastcall TForm1::PaintBox1Paint(TObject* Sender)
{
  Gdiplus::Graphics* g = new Gdiplus::Graphics(PaintBox1->Canvas->Handle);

  try {
    g->DrawImage(Dest, rect);

    if (Sender != NULL) {
      g->TranslateTransform(0, rect.Height);
      g->DrawImage(Source, rect);
    }
  }

  __finally {
    delete g;
  }
}

// �������ɫ
void __fastcall TForm1::PaintBox2Paint(TObject* Sender)
{
  if (CheckBox1->Checked)
    PaintBox2->Canvas->Brush->Color =
        (MixColor.Blue << 16) | (MixColor.Green << 8) | MixColor.Red;
  else {
    PaintBox2->Canvas->Brush->Color = Color;
  }

  PaintBox2->Canvas->Pen->Color = Color;
  PaintBox2->Canvas->Rectangle(PaintBox2->ClientRect);
}

��������У�ʵ��ͼ��ڰ׵���������Ҫ��Execute������ɡ�
�����Ǽ��ų������н���ͼ��
1��ȱʡ�ڰ׵����������н��棬�������ϱߵ������༭����ʾ����һЩԤ��ڰ�Ч��ѡ�
2��ѡ�������Ч���ڰ׵����������н��棺
3��ʹ��ȱʡ�������кڰ׵�����������ѡ��ɫ������ɫ�Ľ��棺
4���������������ϣ�ɫ�����䣬�Ӵ��ɫ��������ʹͼ�������·���ɫ����һЩ��ͬʱ������ɫ��������ʹ�����Χ����ɫ�䰵һЩ��
��ˮƽ���ޣ������������⣬��ӭָ����ָ���������ַ��maozefa@hotmail.com
����ɷ��ʡ�C++ͼ���� -- ������������
#endif

