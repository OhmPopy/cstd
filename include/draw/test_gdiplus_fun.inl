#include "img/imgio.inl"
typedef struct {
  wchar Codepoint;
  float XAdvance;
  float X0, Y0, X1, Y1;
  float U0, V0, U1, V1;// Texture coordinates
} ImFontGlyph;
typedef struct {
  // Members: Hot ~62/78 bytes
  float FontSize;// <user set> // Height of characters, set during loading (don't change after loading)
  float Scale;// = 1.f // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
  ImVec2 DisplayOffset;// = (0.f,1.f) // Offset font rendering by xx pixels
  int Glyphs_Size;
  int IndexXAdvance_Size;
  int IndexLookup_Size;
  ImFontGlyph* Glyphs;// // All glyphs.
  float* IndexXAdvance;// // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
  unsigned short* IndexLookup;// // Sparse. Index glyphs by Unicode code-point.
  ImFontGlyph* FallbackGlyph;// == FindGlyph(FontFallbackChar)
  float FallbackXAdvance;// == FallbackGlyph->XAdvance
  wchar FallbackChar;// = '?' // Replacement glyph if one isn't found. Only set via SetFallbackChar()
  // Members: Cold ~18/26 bytes
  short ConfigDataCount;// ~ 1 // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
  //ImFontConfig* ConfigData;// // Pointer within ContainerAtlas->ConfigData
  //ImFontAtlas* ContainerAtlas;// // What we has been loaded into
  float Ascent, Descent;// // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
  // Methods
  uchar* TexPixelsAlpha8;
  int TexWidth;
  int TexHeight;
  int TexDesiredWidth;
  ImVec2 TexUvWhitePixel;
  void* TexID;
} ImFont;
typedef struct {
  void* FontData;// // TTF data
  int FontDataSize;// // TTF data size
  bool FontDataOwnedByAtlas;// true // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
  int FontNo;// 0 // Index of font within TTF file
  float SizePixels;// // Size in pixels for rasterizer
  int OversampleH, OversampleV;// 3, 1 // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
  bool PixelSnapH;// false // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
  ImVec2 GlyphExtraSpacing;// 0, 0 // Extra spacing (in pixels) between glyphs
  const ushort* GlyphRanges;// // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
  bool MergeMode;// false // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
  bool MergeGlyphCenterV;// false // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
  // [Internal]
  char Name[32];// Name (strictly for debugging)
  ImFont* DstFont;
} ImFontConfig;
//#include "font/ttffont.inl"
void test_gdiplus1(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //softgc
  //sgClear(g, ColorWhite);
  //�������ڻ���ͼ����ʹ�õĻ���
  gcPenColor(g, ColorRed);
  //������
  gcDrawRectangle(g, 20, 20, 200, 100);
  //��ֱ��
  gcDrawLine(g, 20, 250, 200, 250);
  //����Բ
  gcDrawEllipse(g, 250, 20, 150, 100);
}
void test_gdiplus2(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //����Graphics����
  //gcClear(ColorWhite);
  //�������ʣ���һ������Ϊ��ɫ���ڶ������ʵĿ�ȣ���ѡ������Ĭ��Ϊ1��
  gcPenColor(g, ColorBlack);
  gcLineWidth(g, 2);
  //���ƾ���
  //gcDrawRectangle(g, 50, 50, 200, 100);
  //�������û�����ɫ
  gcPenColor(g, ColorGreen);
  //�������û��ʿ��
  gcLineWidth(g, 10);
  //������Բ
  //gcDrawEllipse(g, 50, 200, 200, 100);
  {
    Path path[1] = {0};
    PathAddArc(path, 100, 100, 100, 200, 10, 270);
    gcPen(g, ColorBlack, 1);
    gcDrawPath2(g, path);
    PathFree(path);
  }
}
void test_gdiplus3(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  float dashArray[] = {
    5.0f, //�߳�5������
      2.0f, //���2������
      15.0f, //�߳�15������
      8.0f //���4������
  };
  //gcClear(ColorWhite);
  //Ĭ��Ϊʵ��
  gcPenColor(g, ColorRed);
  gcLineWidth(g, 3);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //���û�������
  gcDashStyle(g, DashStyleDashDotDot);
  gcDrawRectangle(g, 50, 200, 200, 100);
  //ʹ���Զ�������
  gcLineDash(g, 0, dashArray, countof(dashArray));
  gcPenColor(g, ColorBlack);
  gcDrawLine(g, 280, 100, 500, 100);
  gcDashStyle(g, DashStyleSolid);
}
void test_gdiplus4(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //���������λ���
  gcPen(g, ColorRed, 1);
  gcDrawRectangle(g, 50, 50, 200, 200);
  gcDrawRectangle(g, 300, 50, 200, 200);
  //��Բ
  gcPenAlignment(g, PenAlignmentCenter);
  gcPen(g, ColorBlue, 16);
  gcDrawEllipse(g, 50, 50, 200, 200);
  gcPen(g, ColorGreen, 8);
  gcDrawEllipse(g, 50, 50, 200, 200);
  //���û��ʶ��뷽ʽΪ Inset
  gcPenAlignment(g, PenAlignmentInset);
  //���»�Բ
  gcPen(g, ColorBlue, 16);
  gcDrawEllipse(g, 300, 50, 200, 200);
  gcPen(g, ColorGreen, 8);
  gcDrawEllipse(g, 300, 50, 200, 200);
}
void test_gdiplus5(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  //gcClear(ColorWhite);
  gcPen(g, ColorRed, 16);
  //���������յ����ñ
  //p.StartCap = LineCapDiamondAnchor;
  //p.EndCap = LineCapRound;
  for (i = 0; i < LineCapCustom; ++i) {
    gcLineCap(g, (LineCap)i, (LineCap)i);
    gcDrawLine(g, 50, 50 + 30 * i, 350, 50 + 30 * i);
  }
}
void test_gdiplus6(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorRed, 6);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //���ʵ�����
  //��ˮƽ��������3������ֱ���򱣳ֲ���
  //gcScaleTransform(g, 3,1);
  //ƽ�ƻ�ͼƽ��
  //Path path[1] = {0};
  //PathAddRectangle(path, 50, 50, 200, 100);
  //PathTranslate(path, 300, 0);
  //ʹ�������˵Ļ��ʻ���ͼ��
  gcDrawRectangle(g, 50, 50, 200, 100);
  //60����ת
  //PathRotate(g, 60, MatrixOrderAppend);
  //����ƽ�ƻ�ͼƽ��
  //PathTranslate(g, 0,150);
  //ʹ����ת�˵Ļ��ʻ���ͼ��
  gcDrawRectangle(g, 50, 50, 200, 100);
}
void test_gdiplus7(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPenAlignment(g, PenAlignmentCenter);
  gcPen(g, ColorRed, 25);
  //���û��ʽ�Ϸ�ʽΪ��������
  gcLineJoin(g, LineJoinMiterClipped);
  gcLineJoin(g, LineJoinRound);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //���û��ʽ�Ϸ�ʽΪԲ��
  gcLineJoin(g, LineJoinRound);
  gcDrawRectangle(g, 350, 50, 200, 100);
  //���û��ʵĽ�Ϸ�ʽΪб��
  gcLineJoin(g, LineJoinBevel);
  gcDrawRectangle(g, 50, 250, 200, 100);
  //���û��ʵĽ�Ϸ�ʽΪб��
  gcLineJoin(g, LineJoinMiter);
  gcDrawRectangle(g, 350, 250, 200, 100);
}
void test_gdiplus8(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorBlack, 3);
  //��ɫ��ˢ(SolidBrush)
  gcSolidBrush(g, _RGB(255, 0, 0));
  //ʹ��SolidBrush ��ˢ����һ�����β����Ƴ�rect1�ı߿�
  gcFillRectangle(g, 50, 50, 200, 100);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //=============================================================================
  //Ӱ�߻�ˢ��HatchBrush��
  gcHatchBrush2(g, HatchStyleBackwardDiagonal, _RGB(255, 0, 0), ColorGray);
  //ʹ�� HatchBrush ��ˢ���ڶ������Σ������Ƴ���߿�
  gcFillRectangle(g, 300, 50, 200, 100);
  gcDrawRectangle(g, 300, 50, 200, 100);
#if 0
  //=============================================================================
  Rectangle rect3 = 50, 200, 200, 100);
  //����ˢ(TextureBrush)
  Bitmap bmp = (Bitmap)Image.FromFile(@"C:\Users\ThinkPad\Desktop\GDIPuls Brush\Start.png");
  TextureBrush tb = new TextureBrush(bmp);
  //����ԭͼ
  gcDrawImage(bmp, 50, 320);
  //������3�����ƾ��α߿�
  gcFillRectangle(g, rect3);
  gcDrawRectangle(g, rect3);
#endif
  //===============================================================================
  //���Խ��仭ˢ��LinearGradientBrush��
  gcGradientBrushRectS(g, 300, 200, 200, 100, ColorGreen, ColorYellow, LinearGradientModeForwardDiagonal);
  //������4��������߿�
  gcFillRectangle(g, 300, 200, 200, 100);
  gcDrawRectangle(g, 300, 200, 200, 100);
#if 0
  //===============================================================================
  Rectangle rect5 = 550, 200, 200, 100);
  //·�����仭ˢ(PathGradientBrush)
  Point[] pts = {
    rect5.Left, rect5.Top,
    rect5.Right, rect5.Top,
    rect5.Right, rect5.Bottom,
    rect5.Left, rect5.Bottom
  };
  PathGradientBrush pb = new PathGradientBrush(pts);
  //�������ĵ���ɫ
  pb.CenterColor = ColorGreen;
      //������5��������߿�
      gcFillRectangle(pb, rect5);
      gcDrawRectangle(p, rect5);
#endif
}
    void test_gdiplus9(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //�������ߡ�����ζ˵�����
  FPOINT closedCurvePts[] = {
    100.0f, 250.0f,
      200.0f, 200.0f,
      250.0f, 350.0f,
      50.0f, 300.0f,
      100.0f, 250.0f
  };
  //gcClear(ColorWhite);
  //������ɫ�ĵ�ɫ��ˢ
  gcSolidBrush(g, ColorGreen);
  //�����Բ
  gcFillEllipse(g, 50, 50, 200, 100);
  //ʹ�� .NET ���õĵ���ˢ
  gcSolidBrush(g, ColorRed);
  gcFillRectangle(g, 300, 50, 200, 100);
  //�������߶˵㲢���
  //gcFillClosedCurve(g,closedCurvePts);
  gcSolidBrush(g, ColorGreen);
  gcFillPolygon(g, closedCurvePts, countof(closedCurvePts), 1, GC_FLOAT, sizeof(FPOINT));
  //�������ζ˵㲢���
  //gcTranslate(g, 300,0);
  gcSolidBrush(g, ColorLightBlue);
  gcFillPolygon(g, closedCurvePts, countof(closedCurvePts), 1, GC_FLOAT, sizeof(FPOINT));
  //gcResetPath(g);
}
void test_gdiplus10(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //������һ��Ӱ�߻�ˢ
  gcHatchBrush2(g, HatchStyleVertical, ColorRed, ColorWhite);
  gcFillRectangle(g, 10, 10, 200, 100);
  //�����ڶ��ֻ�ˢ
  gcHatchBrush2(g, HatchStyleHorizontal, ColorRed, ColorLightGray);
  gcFillRectangle(g, 250, 10, 200, 100);
  //���������ֻ�ˢ
  gcHatchBrush2(g, HatchStyleBackwardDiagonal, ColorGreen, ColorLightYellow);
  gcFillRectangle(g, 10, 150, 200, 100);
  //���������ֻ�ˢ
  gcHatchBrush2(g, HatchStyleForwardDiagonal, ColorBlack, ColorRed);
  gcFillRectangle(g, 250, 150, 200, 100);
}
void test_gdiplus11(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //Rectangle rect1 = 10,10,300,300);
  //Rectangle rect2 = 350,10,300,300);
  //����ͼƬ
  
  static img_t im[1] = {0};
  texture_t img[1] = {0};
  if (!im->tt.data) {
    imread("E:/pub/pic/Nemo.jpg", 3, 1, im);
  }
  BMPINITIM(img, im);
  img->fmt = PixelFormat24bppRGB;
  
  gcPen(g, ColorGreen, 3);
  gcWrapMode(g, WrapModeTile, WrapModeTile);
  //ʹ��ͼƬ��ԭʼ��С��������ˢ
  gcTextureBrush(g, img, NULL);
  //�����Բ��������߿�
  gcFillEllipse(g, 10, 10, 300, 300);
  gcDrawEllipse(g, 10, 10, 300, 300);
  //ʹ��ͼƬ�Ĳ������򹹽�����ˢ
  gcTextureBrushRect(g, img, 55, 35, 55, 35, NULL);
  //�����Բ��������߿�
  gcFillEllipse(g, 350, 10, 300, 300);
  gcDrawEllipse(g, 350, 10, 300, 300);
}
void test_gdiplus12(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //װ������ͼƬ
  static img_t im[1] = {0};
  texture_t bmp[1] = {0};
  const float matrix[] = {0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f};
  int i = 0;
  if (!im->tt.data) {
    imread("E:/pub/pic/Nemo.jpg", 3, 1, im);
  }
  BMPINITIM(bmp, im);
  bmp->fmt = PixelFormat24bppRGB;

  //gcClear(ColorWhite);
  gcPen(g, ColorBlue, 3);
  gcSolidBrush(g, ColorBlack);
  //Font myFont = new Font("Arial", 13);
  //��������ˢ
  //�Ի�ˢ�������ţ��Ա�鿴Ч��
  gcTextureBrush(g, bmp, matrix);
  //��ͼƬ��ʹ�����з�ʽ
  gcWrapMode(g, WrapModeClamp, WrapModeClamp);
  gcFillRectangle(g, i * 150, 10, 150, 150);
  gcDrawRectangle(g, i * 150, 10, 150, 150);
  //gcDrawString("Clamp", myFont, sBrush, new PointF(0, 170));
  i += 1;
  //��ͼƬʹ��ƽ�����з�ʽ
  gcWrapMode(g, WrapModeTile, WrapModeTile);
  gcFillRectangle(g, i * 150 + 20, 10, 150, 150);
  gcDrawRectangle(g, i * 150 + 20, 10, 150, 150);
  //gcDrawString("Tile",myFont,sBrush,new PointF(180,170));
  i += 1;
  //��ͼƬʹ��ˮƽ��ת���з�ʽ
  gcWrapMode(g, WrapModeTileFlip, WrapModeTile);
  gcFillRectangle(g, i * 150 + 40, 10, 150, 150);
  gcDrawRectangle(g, i * 150 + 40, 10, 150, 150);
  //gcDrawString("TileFlipX", myFont, sBrush, new PointF(360, 170));
  //��ͼƬʹ�ô�ֱ��ת���з�ʽ
  gcWrapMode(g, WrapModeTile, WrapModeTileFlip);
  gcFillRectangle(g, 10, 200, 150, 150);
  gcDrawRectangle(g, 10, 200, 150, 150);
  //gcDrawString("TileFlipX", myFont, sBrush, new PointF(10, 360));
  //��ͼƬʹ��ˮƽ����ֱͬʱ��ת���з�ʽ
  gcWrapMode(g, WrapModeTileFlip, WrapModeTileFlip);
  gcFillRectangle(g, 180, 200, 150, 150);
  gcDrawRectangle(g, 180, 200, 150, 150);
  //gcDrawString("TileFlipXY", myFont, sBrush, new PointF(180, 360));
}
void test_gdiplus13(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorBlack, 3);
  gcWrapMode(g, WrapModeReflect, WrapModeReflect);
  //gcWrapMode(g, );
  //ʹ��2���㶨�彥������
  gcGradientBrushS(g, 10, 10, 10, 50, ColorRed, ColorBlue);//�յ���ɫ
  //���������򲢻�����߿�
  gcFillRectangle(g, 10, 10, 200, 100);
  gcDrawRectangle(g, 10, 10, 200, 100);
  //ʹ�þ��������彥������
  gcGradientBrushRectS(g, 250, 10, 200, 100, ColorPink, ColorGreen, LinearGradientModeBackwardDiagonal);
  //���������򲢻�����߿�
  gcFillRectangle(g, 250, 10, 200, 100);
  gcDrawRectangle(g, 250, 10, 200, 100);
  //ʹ��ָ����ƫת�Ƕ���������ˢ
  gcGradientBrushRectAngle(g, 500, 10, 200, 100, ColorLightYellow, ColorRed, 60.0f, true);
  gcFillRectangle(g, 500, 10, 200, 100);
  gcDrawRectangle(g, 500, 10, 200, 100);
}
void test_gdiplus14(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //���彥����ɫ����
  COLOR colors[] = { ColorRed, ColorGreen, ColorBlue };
  float positions[] = { 0.0f, 0.3f, 1.0f };
  //����ColorBlend����
  //�������ͽ��仭ˢ
  gcGradientBrushColorsRect(g, 10, 10, 200, 100, LinearGradientModeHorizontal, positions, colors, countof(positions));
  //���ý��仭ˢ�Ķ�ɫ������Ϣ
  //lBrush.InterpolationColors = colorBlend;
  gcFillRectangle(g, 10, 10, 200, 100);
}
void test_gdiplus15(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //�������ǵ�10���ߵĶ˵�����
  FPOINT points[] = {
    75, 0,
    100, 50,
    150, 50,
    112, 75,
    150, 150,
    75, 100,
    0, 150,
    37, 75,
    0, 50,
    50, 50,
  };
  COLOR colors[] = {
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen
  };
  //����·��
  Path path[1] = {0};
  //��·�������ֱ��
  PathAddPolygon(path, points, countof(points), true);
  PathTranslate(path, 100, 100);
  //����·�����仭ˢ
  //�������ĵ���ɫ
  //����ÿ���˵��ɫ��
  //����·�����仭ˢ�ı�Եɫ
  //gcPathGradientBrush2(g, path, colors, countof(colors), NULL, ColorRed);
  //gcPathRadialBrush4(g, path, ColorRed, ColorBlue, NULL);
  gcPathRadialBrush3(g, path, countof(colors), colors, NULL, NULL);
  gcWrapMode2(g, WrapModeTileFlip);
  gcFillPath2(g, path);
  gcFillRect(g, 100, 100, 300, 300);
  PathFree(path);
}
void test_gdiplus16(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //�������
  //gcSmoothingMode = SmoothingModeAntiAlias;
#if 0
  FontFamily fontFamily = new FontFamily("����");
  Font myFont1 = new Font(fontFamily, 15, FontStyleRegular | UnitPixel);
  g.DrawString("����ϵ��Ϊ���壬��СΪ15��������Ϊ����ĺ�ɫ����", myFont1, Brushes.Black, new PointF(10, 50));
  Font myFont2 = new Font("Arial", 25.5f, FontStyle.Italic, UnitPixel);
  g.DrawString("����ϵ��ΪArial����СΪ25.5��������Ϊ��б�ĺ�ɫ����", myFont2, Brushes.Red, new PointF(10, 100));
  Font myFont3 = new Font("΢���ź�", 20, FontStyleUnderline, UnitPixel);
  TextRenderer.DrawText(g, "ʹ��TextRenderer������ַ���", myFont3, new Point(10, 150), ColorGreen);
#endif
}
void test_gdiplus17(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
#if 0
  Graphics g = e.Graphics;
  g.Clear(ColorWhite);
  g.SmoothingMode = SmoothingMode.AntiAlias;
  //�����������
  Font myFont = new Font("Arial", 15, FontStyle.Regular, GraphicsUnit.Pixel);
  //�����ı������ʽ
  StringFormat strFmt = new StringFormat();
  strFmt.Alignment = StringAlignment.Near;
  strFmt.LineAlignment = StringAlignment.Near;
  string tempStr = string.Empty;
  //��ȡϵͳ�����Ѱ�װ������ϵ��
  InstalledFontCollection installedCollection = new InstalledFontCollection();
  FontFamily[] fontFamilyArray = installedCollection.Families;
  int index = 0;
  //���� fontFamilyArray �����е�ÿһ����Ա
  foreach(FontFamily i in fontFamilyArray) {
    //��ȡ��ǰ����ϵͳ������
    tempStr += i.Name + ",";
    index++;
  }
  //������������
  Rectangle textOutRect = new Rectangle(0, 15, Width, Height);
  g.DrawString(tempStr, myFont, Brushes.Black, textOutRect, strFmt);
  tempStr = string.Format("ϵͳ���Ѿ���װ��������{0}�֣������Ʒֱ�Ϊ��\n", index + 1);
  g.DrawString(tempStr, myFont, Brushes.Red, new PointF(0, 0));
#endif
}
void test_gdiplus18(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;// TODO: Add your command handler code here
  //����Graphics����
  //����20��"����"����
  font_t font[1] = {0};
#if 0
  gcFont("����_GB2312", 20, FontStyleRegular, UnitPoint);//�������UNICODE�ַ���
  char* string = "�켫���Ķ������ѣ����ã�";//����ɫ��ˢ��20��"����"��ʾ�ַ���
  int len = strlen(string);
  gcSolidBrush(g, ColorBlue)
  gcDrawString(g, string, len, 30, 10);//�����ַ�����ʾ��ˢ
  gcGradientBrush(g, 30, 50, 100, 50, _RGBA(255, 255, 0, 0), _RGBA(255, 0, 0, 255)); //�����Խ��仭ˢ�ʹ�����20��"����"��ʾ�ַ���
  gcDrawString(g, string, len, 30, 50);//����20��"�����п�"����
  gcFont("�����п�", 20, FontStyleRegular, UnitPoint);//�����Խ��仭ˢ��20��"�����п�"��ʾ�ַ���
  gcDrawString(g, string, len, 30, 90);//��ͼ�񴴽���ˢ
  Image image(L "d://3.jpg");
  gcTextureBrush(&image);//��ͼ��ˢ��20��"�����п�"��ʾ�ַ���
  gcDrawString(g, string, len, 30, 130);//����25��"��������"����
  gcFont("��������", 25, FontStyleRegular, UnitPoint);//��ͼ��ˢ��20��"�����п�"��ʾ�ַ���
  gcDrawString(g, string, len, 30, 170);
#endif
}
#if 0
//GdiPlus[16]: IGPLinearGradientBrush ֮ SetBlendBellShape��SetBlendTriangularShape
//SetBlendBellShape ����: ���������������ߵĽ�����ɹ���;
//SetBlendTriangularShape ����: ����һ��������ɫ�����˵�����ɫ���Թ��ɵ����Խ������.
//���������������𲻴�.
//SetBlendBellShape ����Ч��ͼ:
//SetBlendTriangularShape ����Ч��ͼ:
//SetBlendBellShape ���Դ���:
void test_gdiplus19(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  IRECT Rect = iRECT2(10, 10, 120, 150);
  gcGradientBrushRectAngle2(g, Rect, 0xFFFF0000, 0xFF0000FF, 0);
  Brush.SetBlendBellShape(1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  //
  Graphics.TranslateTransform(-Graphics.Transform.OffsetX, Rect.Y + Rect.Height);
  Brush.SetBlendBellShape(1, 1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
}
//SetBlendTriangularShape ���Դ���:
void test_gdiplus20(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
Graphics :
    = TGPGraphics.Create(Canvas.Handle);
  Rect.Initialize(10, 10, 120, 150);
Brush :
    = TGPLinearGradientBrush.Create(Rect, 0xFFFF0000, 0xFF0000FF, 0);
  Brush.SetBlendBellShape(1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  //
  Graphics.TranslateTransform(-Graphics.Transform.OffsetX, Rect.Y + Rect.Height);
  Brush.SetBlendTriangularShape(1, 1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
}
#endif
void test_gdiplus21(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  IRECT rc, Rect = iRECT2(30, 10, 150, 200);
  // ʵ�Ļ�ˢ
  rc = Rect;
  gcSolidBrush(g, 0xFFFF0000);
  gcFillEllipseRect(g, rc);
  // ��Ӱ��ˢ
  rc = iRectOffset2(Rect, 250, 0);
  gcHatchBrush2(g, HatchStyleDiagonalCross, 0xFF0000FF, 0xFFCCCCCC);
  gcFillEllipseRect(g, rc);
  //rc = iRectOffset2(Rect, 250, 0);
  // gcTextureBrush('C:\GdiPlusImg\Texture.jpg');
  //FillEllipse(Brush, rc);
  rc = iRectOffset2(Rect, 0, 250);
  gcGradientBrushRectAngle2(g, rc, 0xFFFF0000, 0xFF0000FF, 90);
  gcFillEllipseRect(g, rc);
  rc = iRectOffset2(Rect, 250, 250);
  gcRadialBrushRectS2(g, rc, 0xFFFF0000, 0xFFFFFF00);
  gcFillEllipseRect(g, rc);
  return ;
}
#if 0
LineCapAnchorMask ָ�����ڼ����ñ�Ƿ�Ϊêͷñ�����롣
LineCapArrowAnchor ָ����ͷ״êͷñ��
LineCapCustom ָ���Զ�����ñ��
LineCapDiamondAnchor ָ������êͷñ��
LineCapFlat ָ��ƽ��ñ��
LineCapNoAnchor ָ��û��ê��
LineCapRound ָ��Բ��ñ��
LineCapRoundAnchor ָ��Բêͷñ��
LineCapSquare ָ������ñ��
LineCapSquareAnchor ָ����êͷñ��
LineCapTriangle ָ��������ñ��
#endif
//������ñ - Cap
void test_gdiplus22(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcPen(g, ColorBlue, 8);
  gcLineCap(g, LineCapArrowAnchor, LineCapRoundAnchor);//{�����ñ, �յ���ñ}
  gcDrawLine(g, 20, 20, 300, 175);
}
// kCGPathStroke kCGPathFill
void test_gdiplus23(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  // An opaque type that represents a Quartz 2D drawing environment.
  //Graphics Context��ͼ��������,���Խ������Ϊһ�黭��,���ǿ�����������л滭����,������ɺ�,�������ŵ����ǵ�view����ʾ����,view������һ������.
  //�Լ�ѧϰʱʵ�ֵ�demo��ϣ���Դ���а����������ʵ�ֿ����룬����������ע�ͽ��ͣ�����һЩ���Ұ����Ĳ��Ĺ���Ҳο������ڴ������档
  // ����drawRect������������ڴ��Զ���滭�Ͷ���
  //An opaque type that represents a Quartz 2D drawing environment.
  //һ����͸�����͵�Quartz 2D�滭����,�൱��һ������,���������������滭
  /*д����*/
  COLOR crtext = _rgba(1, 0, 0, 1.0);//���������ɫ
  COLOR colors[] = {ColorWhite, ColorGray, ColorBlack, ColorYellow, ColorBlue, ColorRed, ColorGreen, ColorOrange, ColorBrown,};
  IRECT rc;
  //UIFont *font = [UIFont boldSystemFontOfSize:15.0];//����
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  gcSolidText(g, iRECT2(10, 20, 180, 20), "��Բ�� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 80, 180, 20), "���߼����ߣ� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 120, 180, 20), "�����Σ� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 160, 180, 20), "�����κ���Բ�� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 220, 180, 20), "�������Σ� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 260, 180, 20), "��Բ�Ǿ��Σ� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 300, 180, 20), "�����������ߣ� ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 340, 180, 20), "ͼƬ�� ", NULL, io->font, 0, TF_VCENTER, crtext);
  /*��Բ*/
  //�߿�Բ
  gcPen(g, _rgba(1, 1, 1, 1.0), 1); //�����ߵ���ɫ �ߵĿ��
  //void PathAddArc(CGContextRef c,float x, float y,float radius,float startAngle,float endAngle, int clockwise)1���ȣ�180��/�� ����57.3�㣩 �ȣ����ȡ�180��/�� 360�㣽360����/180 ��2�� ����
  // x,yΪԲ�����꣬radius�뾶��startAngleΪ��ʼ�Ļ��ȣ�endAngleΪ �����Ļ��ȣ�clockwise 0Ϊ˳ʱ�룬1Ϊ��ʱ�롣
  PathAddArc2(path, 100, 20, 15, 15, 0, 360);//���һ��Բ
  gcDrawPath2(g, path);//����·��
  //���Բ���ޱ߿�
  PathReset(path);
  PathAddArc2(path, 150, 30, 30, 30, 0, 360);//���һ��Բ
  gcFillPath2(g, path);//�������
  //����Բ�������
  gcSolidBrush(g, _rgba(1, 0, 0, 1));//�����ɫ
  gcLineWidth(g, 3.0);//�ߵĿ��
  PathReset(path);
  PathAddArc2(path, 250, 40, 40, 40, 0, 360);//���һ��Բ
  //kCGPathFill��������������,kCGPathEOFill��ʾ����ż����,kCGPathStroke·��,kCGPathFillStroke·�����,kCGPathEOFillStroke��ʾ���ߣ��������
  gcPath2(g, GcOptFillStroke, path);//����·�������
  /*���߼�����*/
  //����
  {
    FPOINT aPoints[2] = { 100, 80, 130, 80 };//����2
  //PathAddLines(CGContextRef c, const FPOINT points[],size_t count)
  //points[]�������飬��count��С
  PathReset(path);
  PathAddLines(path, aPoints, 2);//�����
  }
  gcPath2(g, GcOptStroke, path);//�����������·��
  //��Ц������
  gcPenColor(g, _rgba(0, 0, 1, 1));//�ı仭����ɫ
  PathReset(path);
  //PathAddArcToPoint(CGContextRef c, float x1, float y1,float x2, float y2, float radius)
  //x1,y1��p1�γ�һ���ߵ�����p2��x2,y2���������p3�γ�һ���ߵ�p3,radius�뾶,ע��, ��Ҫ��ð뾶�ĳ���,
  //����
  PathMoveTo(path, 140, 80);
  PathAddArcToPoint(path, 140, 80, 156, 80, 10, false);
  //����
  PathMoveTo(path, 160, 80);
  PathAddArcToPoint(path, 160, 80, 176, 80, 10, false);
  //���
  PathMoveTo(path, 150, 90);
  PathAddArcToPoint(path, 150, 90, 166, 90, 10, true);
  gcStrokePath2(g, path);//�滭·��
  //ע���������ûŪ������ô���£���ο���http://donbe.blog.163.com/blog/static/138048021201052093633776/
  //������
  PathReset(path);
  gcStrokeRect(g, 100, 120, 10, 10); //������
  gcFillRect(g, 120, 120, 10, 10); //����
  //���Σ���������ɫ
  gcLineWidth(g, 2.0);//�ߵĿ��
  gcSolidBrush(g, ColorBlue);//�����ɫ
  gcStrokeColor(g, ColorYellow);//�߿���ɫ
  PathAddRect(path, 140, 120, 60, 30);//������
  gcPath2(g, GcOptFillStroke, path);//�滭·��
  //���Σ�������������ɫ
  //������ɫ�ο�http://blog.sina.com.cn/s/blog_6ec3c9ce01015v3c.html
  //http://blog.csdn.net/reylen/article/details/8622932
  //��һ����䷽ʽ����һ�ַ�ʽ���뵼�����quartcore��#import <QuartzCore/QuartzCore.h>������Ͳ�������g�ϻ������ǽ�����뵽view�����档��ô�������Ƶ�Quartz Core ͼ�����ˡ�
  
  rc = iRECT2(240, 120, 60, 30);
  gcGradientBrushRect2(g, rc, NULL, colors, countof(colors), LinearGradientModeVertical);
  gcFillRect2(g, rc);
  
  //�ڶ�����䷽ʽ
  {
    COLOR colors[] = {
      _rgba(1, 1, 1, 1),
      _rgba(1, 1, 0, 1),
      _rgba(1, 0, 0, 1),
      _rgba(1, 0, 1, 1),
      _rgba(0, 1, 1, 1),
      _rgba(0, 1, 0, 1),
      _rgba(0, 0, 1, 1),
      _rgba(0, 0, 0, 1),
    };
    //�γ����Σ������Ч��
  }
  //�����γ�һ������
  //CGContextSaveGState��CGContextRestoreGState������
  //CGContextSaveGState�����������ǽ���ǰͼ��״̬�����ջ��֮������ͼ��״̬�������޸Ļ�Ӱ�������軭����������Ӱ��洢�ڶ�ջ�еĿ��������޸���ɺ�������ͨ��CGContextRestoreGState�����Ѷ�ջ������״̬���������ص�֮ǰ��ͼ��״̬����������͵����ķ�ʽ�ǻص�֮ǰͼ��״̬�Ŀ��ٷ�������������������е�״̬�޸ģ���Ҳ�ǽ�ĳЩ״̬������ü�·�����ָ���ԭ�����õ�Ψһ��ʽ��
  //CGContextSaveGState(g);
  PathReset(path);
  PathMoveTo(path, 220, 90);
  PathLineTo(path, 240, 90);
  PathLineTo(path, 240, 110);
  PathLineTo(path, 220, 110);
  PathCloseFigure(path);
  //CGContextClip(g);//g�ü�·��,����������·��
  //CGContextDrawLinearGradient(CGContextRef g,CGGradientRef gradient, FPOINT startPoint, FPOINT endPoint,CGGradientDrawingOptions options)
  //gradient������ɫ,startPoint��ʼ�������ʼλ��,endPoint��������,options��ʼ����֮ǰor��ʼ֮��ʼ����
  gcGradientBrush(g, 220, 90, 240, 110, NULL, colors, countof(colors));
  gcFillPath2(g, path);
  //��дһ������Ч��
  //CGContextSaveGState(g);
  PathReset(path);
  PathMoveTo(path, 260, 90);
  PathLineTo(path, 280, 90);
  PathLineTo(path, 280, 100);
  PathLineTo(path, 260, 100);
  PathCloseFigure(path);
  //gcClip(g);//�ü�·��
  //˵���ˣ���ʼ����ͽ��������ǿ��ƽ���ķ������״
  gcGradientBrush(g, 260, 90 , 260, 100, NULL, colors, countof(colors));
  gcFillPath2(g, path);
  //�����ٿ�һ����ɫ�����Բ
  rc = iRECT2(300, 100, 20, 20);
  gcRadialBrushRect2(g, rc, NULL, colors, countof(colors));
  gcFillEllipseRect(g, rc);
  /*�����κ���Բ*/
  //�����Σ�Ҳ�ͻ�Բ��ֻ���������ýǶȵĴ�С���γ�һ������
  gcSolidBrush(g, _rgba(0, 1, 1, 1));//�����ɫ
  //��10Ϊ�뾶Χ��Բ�Ļ�ָ���Ƕ�����
  PathReset(path);
  PathMoveTo(path, 160, 180);
  PathAddArc2(path, 160, 180, 30, 30, -60, -60);
  PathCloseFigure(path);
  gcPath2(g, GcOptFillStroke, path);//����·��
  //����Բ
  PathReset(path);
  PathAddEllipse(path, 160, 180, 20, 8);//��Բ
  gcPath2(g, GcOptFillStroke, path);
  /*��������*/
  //ֻҪ��������и���һ���߷�ʽһ������������������
  //�����
  {
    FPOINT sPoints[3] = {100, 220, 130, 220, 130, 160,};
  PathAddLines(path, sPoints, 3);//�����
  }
  PathCloseFigure(path);//������
  gcPath2(g, GcOptFillStroke, path);//�����������·��
  /*��Բ�Ǿ���*/
  PathReset(path);
  PathAddRoundRectangle(path, 120, 250, 60, 30, 10, 10, 10, 10);
  gcPath2(g, GcOptFillStroke, path);//�����������·��
  //������������
  //��������
  PathReset(path);
  PathMoveTo(path, 120, 300);//����Path�����
  PathQuadTo(path, 190, 310, 120, 390); //���ñ��������ߵĿ��Ƶ�������յ�����
  //�������ߺ���
  PathMoveTo(path, 200, 300);//����Path�����
  PathCubicTo(path, 250, 280, 250, 400, 280, 300);//���ñ��������ߵĿ��Ƶ�����Ϳ��Ƶ������յ�����
  gcPath2(g, GcOptStroke, path);
#if 0
  //ͼƬ
UIImage* image = [UIImage imageNamed: @"apple.jpg"];
[image drawInRect: CGRectMake(60, 340, 20, 20)]; //�������л���ͼƬ
  // [image drawAtPoint:fPOINT(100, 340)];//����ͼƬ��С��point�㿪ʼ��ͼƬ�����԰�ע��ȥ������
  CGContextDrawImage(g, CGRectMake(100, 340, 20, 20), image.CGImage);//ʹ�����ʹͼƬ���µߵ��ˣ��ο�http://blog.csdn.net/koupoo/article/details/8670024
  // CGContextDrawTiledImage(g, CGRectMake(0, 0, 20, 20), image.CGImage);//ƽ��ͼ
#endif
  PathFree(path);
}
void test_gdiplus24(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  //��Ellipses��Arcs��������
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // And draw with a blue fill color
  gcSolidBrush(g, _rgba(0.0, 0.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Add an ellipse circumscribed in the given rect to the current path, then stroke it
  PathAddEllipse(path, 30.0, 30.0, 60.0, 60.0);
  gcStrokePath2(g, path);
  // Stroke ellipse convenience that is equivalent to AddEllipseInRect(); StrokePath();
  gcDrawEllipse(g, 30.0, 120.0, 60.0, 60.0);
  // Fill rect convenience equivalent to AddEllipseInRect(); FillPath();
  gcFillEllipse(g, 30.0, 210.0, 60.0, 60.0);
  // Stroke 2 seperate arcs
  PathReset(path);
  PathAddArc2(path, 150.0, 60.0, 30.0, 30.0, 0.0, 90);
  gcStrokePath2(g, path);
  PathReset(path);
  PathAddArc2(path, 150.0, 60.0, 30.0, 30.0, 270, -90);
  gcStrokePath2(g, path);
  // Stroke 2 arcs together going opposite directions.
  PathReset(path);
  PathAddArc2(path, 150.0, 150.0, 30.0, 30.0, 0, 90);
  PathAddArc2(path, 150.0, 150.0, 30.0, 30.0, 270, -90);
  gcStrokePath2(g, path);
  // Stroke 2 arcs together going the same direction..
  PathReset(path);
  PathAddArc2(path, 150.0, 240.0, 30.0, 30.0, 0, 90);
  PathAddArc2(path, 150.0, 240.0, 30.0, 30.0, 180, 90);
  gcStrokePath2(g, path);
  // Stroke an arc using AddArcToPoint
  PathReset(path);
  PathAddArcToPoint(path, 210.0, 30.0, 240.0, 60.0, 30.0, true);
  gcStrokePath2(g, path);
  // Show the two segments that are used to determine the tangent lines to draw the arc.
  //gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  //PathReset(path);
  //PathAddLines(path, p, sizeof(p)/sizeof(p[0]));
  //gcStrokePath2(g, path);
  // As a bonus, we'll combine arcs to create a round rectangle!
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // If you were making this as a routine, you would probably accept a rectangle
  // that defines its bounds, and a radius reflecting the "rounded-ness" of the rectangle.
  // NOTE: At this point you may want to verify that your radius is no more than half
  // the width and height of your rectangle, as this technique degenerates for those cases.
  // In order to draw a rounded rectangle, we will take advantage of the fact that
  // PathAddArcToPoint will draw straight lines past the start and end of the arc
  // in order to create the path from the current position and the destination position.
  // In order to create the 4 arcs correctly, we need to know the min, mid and max positions
  // on the x and y lengths of the given rectangle.
  // Next, we will go around the rectangle in the order given by the figure below.
  // minx midx maxx
  // miny 2 3 4
  // midy 1 9 5
  // maxy 8 7 6
  // Which gives us a coincident start and end point, which is incidental to this technique, but still doesn't
  // form a closed path, so we still need to close the path to connect the ends correctly.
  // Thus we start by moving to point 1, then adding arcs through each pair of points that follows.
  // You could use a similar tecgnique to create any shape with rounded corners.
  // Start at 1
  // Close the path
  PathReset(path);
  {
    float radius = 10.0;
  PathAddRoundRectangle(path, 210.0, 90.0, 60.0, 60.0, radius, radius, radius, radius);
  }
  // Fill & stroke the path
  gcPath2(g, GcOptFillStroke, path);
  PathFree(path);
}
//����Beziers��Quadratics����
void test_gdiplus25(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  FPOINT s = fPOINT(30.0, 120.0);
  FPOINT e = fPOINT(300.0, 120.0);
  FPOINT cp1 = fPOINT(120.0, 30.0);
  FPOINT cp2 = fPOINT(210.0, 210.0);
  Path path[1] = {0};

  //��Ellipses��Arcs��������
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Draw a bezier curve with end points s,e and control points cp1,cp2
  PathMoveTo(path, s.x, s.y);
  PathCurveTo(path, cp1.x, cp1.y, cp2.x, cp2.y, e.x, e.y);
  gcStrokePath2(g, path);
  // Show the control points.
  gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathLineTo(path, cp1.x, cp1.y);
  PathMoveTo(path, e.x, e.y);
  PathLineTo(path, cp2.x, cp2.y);
  gcStrokePath2(g, path);
  // Draw a quad curve with end points s,e and control point cp1
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  s = fPOINT(30.0, 300.0);
  e = fPOINT(270.0, 300.0);
  cp1 = fPOINT(150.0, 180.0);
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathQuadTo(path, cp1.x, cp1.y, e.x, e.y);
  gcStrokePath2(g, path);
  // Show the control point.
  gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathLineTo(path, cp1.x, cp1.y);
  PathLineTo(path, e.x, e.y);
  gcStrokePath2(g, path);
  PathFree(path);
}
// ���ƾ���
void test_gdiplus26(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // And drawing with a blue fill color
  gcFillColor(g, _rgba(0.0, 0.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Add Rect to the current path, then stroke it
  PathAddRect(path, 30.0, 30.0, 60.0, 60.0);
  gcStrokePath2(g, path);
  // Stroke Rect convenience that is equivalent to above
  gcStrokeRect(g, 30.0, 120.0, 60.0, 60.0);
  // Stroke rect convenience equivalent to the above, plus a call to gcLineWidth().
  gcLineWidth(g, 10);
  gcStrokeRect(g, 30.0, 210.0, 60.0, 60.0);
  // Demonstate the stroke is on both sides of the path.
  
  //red
  gcStrokeColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  gcLineWidth(g, 2);
  gcStrokeRect(g, 30.0, 210.0, 60.0, 60.0);

  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  {
    FRECT rects[3];
  rects[0] = fRECT2(120.0, 30.0, 60.0, 60.0);
  rects[1] = fRECT2(120.0, 120.0, 60.0, 60.0);
  rects[2] = fRECT2(120.0, 210.0, 60.0, 60.0);
  PathReset(path);
  // Bulk call to add rects to the current path.
  PathAddRectangles(path, rects, countof(rects));
  }
  gcStrokePath2(g, path);
  // Create filled rectangles via two different paths.
  // Add/Fill path
  path->Count = 0;
  PathAddRect(path, 210.0, 30.0, 60.0, 60.0);
  gcFillPath2(g, path);
  // Fill convienience.
  gcFillRect(g, 210.0, 120.0, 60.0, 60.0);

  PathFree(path);
}
// ���ƶ����
void test_gdiplus27(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i, j=0, drawingMode[] = {GcOptFill, GcOptStroke, GcOptEOFill, GcOptFillStroke, GcOptEOFillStroke};
  Path path[1] = {0};
  double r = 50;
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  for (j=0; j<countof(drawingMode); ++j) {
    FPOINT center;
    float y;
    // Drawing with a white stroke color
    gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
    // Drawing with a blue fill color
    gcFillColor(g, _rgba(0.0, 0.0, 1.0, 1.0));
    // Draw them with a 2.0 stroke width so they are a bit more visible.
    gcLineWidth(g, 2.0);
    y = j*(r*2+10) + r;
    // Add a star to the current path
    center = fPOINT(90.0, y);
    PathReset(path);
    PathMoveTo(path, center.x, center.y + r);
    for (i = 1; i < 5; ++i)
    {
      float x = r * sinf(i * 4.0 * M_PI / 5.0);
      float y = r * cosf(i * 4.0 * M_PI / 5.0);
      PathLineTo(path, center.x + x, center.y + y);
    }
    // And close the subpath.
    PathCloseFigure(path);
    // Now add the hexagon to the current path
    center = fPOINT(210.0, y);
    PathMoveTo(path, center.x, center.y + r);
    for (i = 1; i < 6; ++i)
    {
      float x = r * sinf(i * 2.0 * M_PI / 6.0);
      float y = r * cosf(i * 2.0 * M_PI / 6.0);
      PathLineTo(path, center.x + x, center.y + y);
    }
    // And close the subpath.
    PathCloseFigure(path);
    // Now draw the star & hexagon with the current drawing mode.
    gcPath2(g, drawingMode[j], path);
  }
  PathFree(path);
}
// ����line
void test_gdiplus28(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  FPOINT addLines[] ={10.0, 90.0,70.0, 60.0,130.0, 90.0,190.0, 60.0,250.0, 90.0,310.0, 60.0,};
  FPOINT strokeSegments[] ={10.0, 150.0,70.0, 120.0,130.0, 150.0,190.0, 120.0,250.0, 150.0,310.0, 120.0,};
  Path path[1] = {0};
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  // Drawing lines with a white stroke color
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Draw a single line from left to right
  PathMoveTo(path, 10.0, 30.0);
  PathLineTo(path, 310.0, 30.0);
  gcStrokePath2(g, path);
  // Draw a connected sequence of line segments
  PathReset(path);
  // Bulk call to add lines to the current path.
  // Equivalent to MoveToPoint(points[0]); for(i=1; i<count; ++i) AddLineToPoint(points[i]);
  PathAddLines(path, addLines, countof(addLines));
  gcStrokePath2(g, path);
  // Draw a series of line segments. Each pair of points is a segment
  // Bulk call to stroke a sequence of line segments.
  // Equivalent to for(i=0; i<count; i+=2) { MoveToPoint(point[i]); AddLineToPoint(point[i+1]); StrokePath(); }
  PathAddLineSegments(path, strokeSegments, countof(strokeSegments));
  gcStrokePath2(g, path);
  PathFree(path);
}
// ������
void test_gdiplus29(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  float dashPhase = 5;
  float dashPattern[] = {10, 10};
  int dashCount = countof(dashPattern);
  PathReset(path);
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Each dash entry is a run-length in the current coordinate system
  // The concept is first you determine how many points in the current system you need to fill.
  // Then you start consuming that many pixels in the dash pattern for each element of the pattern.
  // So for example, if you have a dash pattern of {10, 10}, then you will draw 10 points, then skip 10 points, and repeat.
  // As another example if your dash pattern is {10, 20, 30}, then you draw 10 points, skip 20 points, draw 30 points,
  // skip 10 points, draw 20 points, skip 30 points, and repeat.
  // The dash phase factors into this by stating how many points into the dash pattern to skip.
  // So given a dash pattern of {10, 10} with a phase of 5, you would draw 5 points (since phase plus 5 yields 10 points),
  // then skip 10, draw 10, skip 10, draw 10, etc.
  gcLineDash(g, dashPhase, dashPattern, dashCount);
  PathReset(path);
  // Draw a horizontal line, vertical line, rectangle and circle for comparison
  PathMoveTo(path, 10.0, 20.0);
  PathLineTo(path, 310.0, 20.0);
  PathMoveTo(path, 160.0, 30.0);
  PathLineTo(path, 160.0, 130.0);
  PathAddRect(path, 10.0, 30.0, 100.0, 100.0);
  PathAddEllipse(path, 210.0, 30.0, 100.0, 100.0);
  // And width 2.0 so they are a bit more visible
  gcLineWidth(g, 2.0);
  gcStrokePath2(g, path);
  gcDashStyle(g, DashStyleSolid);
  PathFree(path);
}
#if 0
void test_gdiplus30(ctrl_t* c)
{
}
void test_gdiplus31(ctrl_t* c)
{
}
void test_gdiplus32(ctrl_t* c)
{
}
void test_gdiplus33(ctrl_t* c)
{
}
#endif