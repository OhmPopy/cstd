
void gcClear(gc_t* g, const IRECT* pclip, COLOR clr)  {
  IRECT rc = pclip ? *pclip : iRECT2(0, 0, 10000, 10000);
  //gcPushClipRect(g, rc);
  gcSolidBrush(g, clr);
  gcFillRect2(g, rc);
  //gcPopClipRect(g);
}

#if 0
void menuItem2(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, &c->rc, ColorBlack);
  Bitmap bitmap(L"demo.bmp");
  int iWidth = bitmap.GetWidth();
  int iHeight = bitmap.GetHeight();

  //��ʼ��ɫ�ʱ任����
  ColorMatrix colorMatrix=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  ImageAttributes imageAtt;

  //��0��1�����޸�ɫ�ʱ任�������Խ����ϵ���ֵ
  //ʹ���ֻ�׼ɫ�ı��ͶȽ���
  for(float i=0.0f;i<=1.0f;i+=0.02f)
  {  
    colorMatrix.m[0][0]=i;
    colorMatrix.m[1][1]=i;
    colorMatrix.m[2][2]=i;
    colorMatrix.m[3][3]=i;
    //����ɫ��У������
    imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
    //����ͼƬ
    gcRectImage(g, &bitmap, Rect(0, 0, iWidth, iHeight), 0,0,        iWidth,iHeight,UnitPixel, &imageAtt);
  }

  MyDrawString(g, "������ʾ����Ч��...", Font(L"Arial",12), SolidBrush(ColorWhite), Point(20,iHeight+20));

  //��1��0�����޸�ɫ�ʱ任�������Խ����ϵ���ֵ
  //���μ���ÿ��ɫ�ʷ���
  for(i=1.0f;i>=0.0f;i-=0.02f)
  {  
    colorMatrix.m[0][0]=i;
    colorMatrix.m[1][1]=i;
    colorMatrix.m[2][2]=i;
    colorMatrix.m[3][3]=i;
    //����ɫ��У������
    imageAtt.SetColorMatrix(&colorMatrix,
      ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
    //����ͼƬ
    g.DrawImage(
      &bitmap, Rect(0, 0, iWidth, iHeight), 
      0,0,
      iWidth,iHeight,
      UnitPixel, 
      &imageAtt);
  }

  gcSolidBrush(g, ColorWhite);
  MyDrawString(g, "���", Font(L"Arial",12), Point(20,iHeight+40));
}

//
void GrayScale()
{
  Graphics &g=GetGraphics();
  g.Clear(ColorWhite);
  Bitmap image(L"head.bmp");      
  int Width =image.GetWidth()-1;
  int Height =image.GetHeight()-1;

  //����ԭͼ
  g.DrawImage(&image,0, 0);
  g.TranslateTransform((REAL)image.GetWidth(),0.f);

  /*image2��image3�ֱ������������ֵ��
  �ͼ�Ȩƽ��������ĻҶ�ͼ��*/
  Bitmap *image2=image.Clone(Rect(0,0,image.GetWidth(), image.GetHeight()),PixelFormatDontCare);
  Bitmap *image3=image.Clone(Rect(0,0,image.GetWidth(), image.GetHeight()),PixelFormatDontCare);

  Color color;
  //ʹ��ƽ��ֵ���лҶȴ���
  for(int i=Width; i>=0;i--)
    for( int j=Height; j>=0;j--)
    {
      image.GetPixel(i,j, &color);  
      //���ƽ������ɫ�ʷ�����ƽ��ֵ
      int middle=(color.GetR()+color.GetG()+color.GetB())/3;
      Color colorResult(255,middle,middle,middle);
      image.SetPixel(i,j, colorResult);
    }  
  //���»��ƻҶȻ�ͼ
  g.DrawImage(&image, Rect(0, 0, Width, Height));

  //����λ����ʾ���ֵ�����лҶȴ���Ľ��
  g.TranslateTransform((REAL)image.GetWidth(),0.f);
  //ʹ�����ֵ�����лҶȴ���
  for(i=Width; i>=0;i--)
  {
    for(int j=Height; j>=0;j--)
    {
      image2->GetPixel(i,j, &color);
      int tmp=color.GetR()>color.GetG()? color.GetR():color.GetG();
      int maxcolor=tmp>color.GetB()? tmp:color.GetB();
      Color colorResult(255,maxcolor,maxcolor,maxcolor);
      //���ô����ĻҶ���Ϣ
      image2->SetPixel(i, j, colorResult);
    }
  }

  //���»��ƻҶȻ�ͼ
  g.DrawImage(image2, Rect(0, 0, Width, Height));
  //�ڵڶ��л���ͼƬ
  g.ResetTransform();
  g.TranslateTransform(0.f, (REAL)image.GetHeight());

  //ʹ�ü�Ȩƽ�������лҶȴ���  
  for(i=Width; i>=0;i--)
  {
    for(int j=Height; j>=0;j--)
    {
      image3->GetPixel(i, j, &color);  
      int R=(int)(0.3f*color.GetR());
      int G=(int)(0.59f*color.GetG());
      int B=(int)(0.11f*color.GetB());

      Color colorResult(255,R,G,B);
      //���ô����ĻҶ���Ϣ
      image3->SetPixel(i, j, colorResult);
    }  
  }
  //���»��ƻҶȻ�ͼ
  g.DrawImage(image3, Rect(0, 0, Width, Height));
  
  g.TranslateTransform((REAL)image.GetWidth(),0.f);
  //�ҶȵĻ�ԭ��ʾ����ԭʹ�����ֵ������ĻҶ�ͼ��image2
  for(i=Width; i>0;i--)
  {
    for(int j=Height; j>0;j--)
    {
      image2->GetPixel(i,j, &color);
      int R=color.GetR();
      int G=color.GetG();
      int B=color.GetB();
      //�ֱ��RGB����ɫ�ʷ�������α��ɫ��ԭ

      //���к�ɫ�����Ļ�ԭ
      if(R<127)
        R=0;
      if(R>=192)
        R=255;
      if(R<=191&&R>=128)
        R=4*R-510;

      /*������ɫ�����Ļ�ԭ,Ϊ�˻�ԭ�����ɫ�����ٴβμӱȽϣ�
      ��������һ������YES��ʾG�Ƿ��Ѿ��μ��˱Ƚ�*/

      bool yes;
      yes=false;
      if(G<=191&&G>=128&&(!yes))
      {
        G=255;
        yes=true;
      }
      if(G>=192&&(!yes))
      {
        G=1022-4*G;
        yes=true;
      }
      if(G<=63&&(!yes))
      {
        G=254-4*G;
        yes=true;
      }
      if(G<=127&&G>=67&&(!yes))
        G=4*G-257;
      
      //������ɫ�����Ļ�ԭ
      if(B<=63)
        B=255;
      if(B>=128)
        B=0;
      if(B>=67&&B<=127)
        B=510-4*B;
      
      //��ԭ���α��ɫ
      Color colorResult(255,R,G,B);
      //����ԭ���RGB��Ϣ����д��λͼ
      image2->SetPixel(i, j, colorResult);
          
    }  
  }
  //���»��ƻ�ԭ���α��ɫλͼ
  //���»��ƻҶȻ�ͼ
  g.DrawImage(image2, Rect(0, 0, Width, Height));

  delete image2;
  delete image3;
}

//
void Inverse()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);
  
  Bitmap image(L"head.bmp");        
  int Width =image.GetWidth();
  int Height =image.GetHeight();

  Color colorTemp,color2;
  Color color;
  //����ԭͼ
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  
  for(int  i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      //��ɫ�ʽ��з�ת����õ�ƬЧ��
      int r=255-color.GetRed();
      int g=255-color.GetGreen();
      int b=255-color.GetBlue();
      Color colorResult(255,r,g,b);
      //����ԭ���RGB��Ϣ����д��λͼ
      image.SetPixel(i, j, colorResult);
    }
    //��̬���Ƶ�Ƭ�˾�Ч��ͼ
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }
  //���Ѿ�ʵ���˵�ƬЧ����λͼ�ٷ�ɫ(�ָ���ԭͼ)
  for(i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      int r=255-color.GetRed();
      int g=255-color.GetGreen();
      int b=255-color.GetBlue();
      Color colorResult(255,r,g,b);
      //����ԭ���RGB��Ϣ����д��λͼ
      image.SetPixel(i, j, colorResult);
    }
    //���ƾ������η�ɫ��λͼ
    gcDrawImage(&image, Rect(Width*2, 0, Width, Height));
  }
}

//
void Emboss()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"head.bmp");        
  int Width =image.GetWidth();
  int Height =image.GetHeight();

  //image2:���е�̴���
  Bitmap *image2=image.Clone(Rect(0,0,Width,Height), PixelFormatDontCare);

  //����ԭͼ
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  Color color, colorTemp,colorLeft;

  //����ͼƬ�ĸ�����
  //���η���ÿ�����ص�RGBֵ
  for(int i=Width-1; i>0;i--)
  {
    for( int j=Height-1; j>0;j--)
    {
      //��ȡ�����������ص�R��G��Bֵ
      image.GetPixel(i, j, &color);
      image.GetPixel(i-1, j-1, &colorLeft);
      //���������Ͻ����ص�RGB����֮��
      //67������ͼƬ����ͻҶȣ�128������������������ֵ��õ���ͬ��Ч��
      int r=max(67,min(255,
        abs(color.GetRed()-colorLeft.GetRed()+128)));
      int g=max(67,min(255,
        abs(color.GetGreen()-colorLeft.GetGreen()+128)));
      int b=max(67,min(255,
        abs(color.GetBlue()-colorLeft.GetBlue()+128)));
      Color colorResult(255,r,g,b);
      //��������RGBֵ��д��λͼ
      image.SetPixel(i, j,colorResult);
    }

    //���Ƹ���ͼ
    gcDrawImage(&image, Rect(Width+10, 0, Width, Height));
  }

  //����ͼƬ�ĵ�̴���
  for(i=0; i<Height-1;i++)
  {
    for( int j=0; j<Width-1;j++)
    {
      image2->GetPixel(j, i, &color);  
      image2->GetPixel(j+1, i+1, &colorLeft);  
      //���������½����صķ���֮��
      //67������ͼƬ����ͻҶȣ�128������������������ֵ��õ���ͬ��Ч��
      int r=max(67,min(255,abs(color.GetRed()-colorLeft.GetRed()+128)));
      int g=max(67,min(255,abs(color.GetGreen()-colorLeft.GetGreen()+128)));
      int b=max(67,min(255,abs(color.GetBlue()-colorLeft.GetBlue()+128)));
      Color colorResult(255,r,g,b);
      image2->SetPixel(j, i,colorResult);
    }

    //���Ƶ��ͼ
    gcDrawImage(image2, Rect(Width*2+20, 0, Width, image.GetHeight()));
  }
  delete image2;
}
#endif


void CreatePenFromBrush(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
#if 0
  //�������Խ��仭ˢ
  gcGradientBrushRectS(g, 0,0,10,10, ColorBlue, ColorRed, LinearGradientModeBackwardDiagonal);
  //�����Խ��仭ˢ�й��컭��
  gcPen(&LineargradientBrush);
  pen.SetWidth(10);
  //���ƾ���
  g.DrawRectangle(&pen,10,10,100,100);

  //װ������ͼƬ
  Bitmap image(L"butterfly.bmp");
  //��������ˢ
  TextureBrush  tBrush(&image);
  //����ˢ���뻭�ʵĹ��캯��
  Pen texturedPen(&tBrush,42);

  //���ñ��������ߵ���ֹ�㼰���Ƶ�
  Point p1(10, 100);   
  Point c1= Point(100, 10);   
  Point c2= Point(150, 150);  
  Point p2= Point(200, 100);  
  g.TranslateTransform(130,0);
  //���Ʊ���������
  g.DrawBezier(&texturedPen, p1, c1, c2, p2);
#endif
}


//���ʵ�������ʾ
void DashStyle_Custom(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
  gcPen(g, ColorBlue, 5);
#if 0
  //�����ı�������뷽ʽ������
  StringFormat fmt;
  gcSetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //����
  Font font(L"Arial",12);
  SolidBrush sBrush(ColorBlack);
  gcTranslateTransform(0,30);
#endif
  //�ֱ�ʹ�ó������������ͻ���ֱ��
  for(i=0;i<5;i++) {
    //��������
    gcDashStyle(g, (DashStyle)i);
    gcDrawLine(g, 10, 30*i, 260, 30*i);
    //�����ǰ���͵�����
    //gcSolidFormatText(g, io->font, sBrush, PointF(260.f,30.f*i), fmt, _T("%d"), i+1);
  }

  //ʹ���Զ���������
  {
    float dashVals[]=
    {
      5.0f,   // �߳�5������
        2.0f,   // ���2������
        15.0f,  // �߳�15������
        4.0f    // ���4������
    };
    gcLineDash(g, 0, dashVals, 4);
    gcPenColor(g, ColorRed);
    gcDrawLine(g, 10, 30*i, 260, 30*i);
    //CString s; s.Format(_T("%d"), i+1);
    //MyDrawString(graphics, s, font, sBrush, PointF(260.f,30.f*i), fmt);
  }
}

void Pen_Align(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
  gcPen(g, _ARGB(155,255,0,0),10);
  for(i=0; i<5;i++) {
    gcPenAlignment(g, (PenAlignment)i);
    gcDrawLine(g, 0 + i*70,10, 60 + i*70,10);
  }
  //gcResetTransform();
  gcPen(g, ColorGray, 1.0f);
  gcDrawLine(g,0,10,600,10);
}
void Pen_Tranform(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
  //����һ֧���Ϊ5�ĺ�ɫ����
  gcPen(g, ColorRed, 3.0f);
  //�����ʴӴ�ֱ��������6����ˮƽ���򱣳ֲ���
  //ScaleTransform(1, 6);
  //ʹ��δ����ת����Ļ��ʻ�Բ
  gcDrawEllipse(g, 0, 50, 80, 80);
  //60����ת
  // TranslateTransform(100,0);
  //pen.RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 100, 50, 80, 80);
  //120����ת
  //gcTranslateTransform(100,0);
  // RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 0, 100+50, 80, 80);
  //180����ת
  //gcTranslateTransform(g, 100,0);
  //RotateTransform(60, MatrixOrderAppend);
  gcDrawEllipse(g, 100+0, 100+50, 80, 80);
}

//��ñ��ʾ
void Pen_LineCap(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  gcClear(g, NULL, ColorWhite);
#if 0
  //�����ı�������뷽ʽ������
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //����
  Font font(L"Arial",12);
  SolidBrush sBrush(ColorBlack);
#endif

  //�������Ϊ15�Ļ���
  gcPen(g, ColorBlack,15);

  //�ֱ�ʹ�ò�ͬ����ñ
  {
    LineCap lincap[]=
    {
      LineCapFlat,
        LineCapSquare,
        LineCapRound,
        LineCapTriangle,
        LineCapNoAnchor,
        LineCapSquareAnchor,
        LineCapRoundAnchor,
        LineCapDiamondAnchor,
        LineCapArrowAnchor,
        LineCapCustom,
    };
    
    for (i=0; i<countof(lincap); i++)
    {
      int y = i*30;
      gcLineCap(g, lincap[i], lincap[i]);//��� �յ�
      gcDrawLine(g,50,10+y,300,10+y);
      //�����ǰ��ñ���ͣ�LineCapö�ٳ�Ա����
      //CString s; s.Format(_T("%d"), lincap[i]);
      //MyDrawString(graphics, s, font, sBrush, PointF(320.f,10.f), fmt);
      //ƽ�ƻ�ͼƽ��
      //gcTranslateTransform(0,30);
    }
  }
}

//���ʵ�͸����֧��
void Pen_TransColor(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcClear(g, NULL, ColorWhite);
  
  //�ֱ�����ɫ����ɫ����
  //��������
  {
    int x, y = 256;
    for (x = 0; x < 256; x += 5)
    {
      gcPenColor(g, ColorBlue);
      gcDrawLine(g, 0, y, x, 0);
      gcPenColor(g, ColorRed);
      gcDrawLine(g, 256, x, y, 256);  
      y -= 5;
      //��ʱ�Բ鿴��̬Ч��
      //sys_sleep(20);
    }    
    //ʹ����ɫ���ʻ��Ʋ�ͬ͸���ȵ�����
    //͸�������ϵ������εݼ�
    for (y = 0; y < 256; y++)
    {
      gcPenColor(g, _A_RGB(y, ColorGreen));
      gcDrawLine(g, 0, y, 256, y);
      //��ʱ�Ա�鿴��̬Ч��
      //sys_sleep(20);
    }
    
    //ʹ����ɫ���ʻ��Ʋ�ͬ͸���ȵ�����
    //͸�������������εݼ�
    for (x = 0; x < 256; x++)
    {
      gcPenColor(g, _A_RGB(x, ColorBlue));
      gcDrawLine(g, x, 100, x, 200);
      //��ʱ�Բ鿴��̬Ч��
      //sys_sleep(20);
    }
  }
}

#if 0
//�򵥵ĵ�ɫ��ˢʾ��
void Brush_SolidBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //������ɫ��ˢ
  SolidBrush greenBrush(ColorGreen);
  //�������ߡ�����ζ˵�����
  PointF point1(100.0f, 100.0f);
  PointF point2(200.0f, 50.0f);
  PointF point3(250.0f, 200.0f);
  PointF point4(50.0f, 150.0f);
  PointF point5(100.0f, 100.0f);
  PointF points[]={point1, point2, point3, point4};
  //���պ�����
  gcFillClosedCurve(&greenBrush, points, 4, FillModeAlternate, 1.0f);

  //��������(�պ�)
  PointF poly[]={point1, point2, point3, point4,point5};
  //����һ��λ���������
  gcTranslateTransform(300,0);
  gcFillPolygon(&greenBrush, poly, 5, FillModeAlternate);
}
//�����Ҷ����
void Brush_FillVurve()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //������ɫ��ˢ
  SolidBrush greenBrush(ColorGreen);
  int cx,cy;
  //�Ե�ǰ���ڵ����ĵ������Ҷ����
  cx=MyClient.Width/2;
  cy=MyClient.Height/2;
  //����"Ҷ"��
  int LeafLength=80;
  //����ҶƬ����=2* LeafNum
  int LeafNum=5;
  double x,y,x2,y2,r;

  //����һ��ͼ��·����������������Ҷ���ߵı߽���
  GraphicsPath tmpPath(FillModeAlternate);
  //�������߽߱����ݣ��Ƕȱ仯Ϊһ��PI*2
  for(float i=0.0f;i<PI*2+0.1f;i+=(float)PI/180)
  {
    r=(int)abs(LeafLength*cos(double(LeafNum*i)));
    x=r*cos(i);
    y=r*sin(i);
    x2=cx+x;
    y2=cy+y;
    /*�����ߵı߽���Ϣ������ʱ·��,�����Ҫ�鿴��Щ��Ϣ�����ɵ�����
    �������ڴ˼���gcDrawLine(&pen,x2,y2,x2-1,y2-1);*/
    tmpPath.AddLine((int)x2,(int)y2,(int)x2,(int)y2);
  }

  //���·��
  gcFillPath(&greenBrush, &tmpPath);

  //��������������
  Pen pen(ColorGray);
  gcDrawLine(&pen,0,cy,cx*2,cy);
  gcDrawLine(&pen,cx,0,cx,cy*2);
}
//Ӱ�߻�ˢʾ��
void Brush_HatchBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����Ӱ�߻�ˢ��ǰ��ɫ��
  Color black=ColorBlack;
  Color white=ColorWhite;

  //ʹ�õ�һ�ַ���Ӱ�߻�ˢ
  HatchBrush brush(HatchStyleHorizontal, black, white);
  gcFillRectangle(&brush, 20, 20, 100, 50);

  //ʹ�õڶ��ַ���Ӱ�߻�ˢ
  HatchBrush brush1(HatchStyleVertical, black, white);
  gcFillRectangle(&brush1, 120, 20, 100, 50);

  //ʹ�õ����ַ���Ӱ�߻�ˢ
  HatchBrush brush2(HatchStyleForwardDiagonal, black, white);
  gcFillRectangle(&brush2, 220, 20, 100, 50);

  //ʹ�õ����ַ���Ӱ�߻�ˢ
  HatchBrush brush3(HatchStyleBackwardDiagonal, black, white);
  gcFillRectangle(&brush3, 320, 20, 100, 50);

  //ʹ�õ����ַ���Ӱ�߻�ˢ
  HatchBrush brush4(HatchStyleCross, black, white);
  gcFillRectangle(&brush4, 420, 20, 100, 50);

  //ʹ�õ����ַ���Ӱ�߻�ˢ
  HatchBrush brush5(HatchStyleDiagonalCross, black, white);
  gcFillRectangle(&brush5, 520, 20, 100, 50);
}
//�оٳ����з���Ӱ�߻�ˢ
void Brush_EnumAllStyle()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�趨��ˢ��ǰ��ɫΪ��ɫ������ɫΪ��ɫ
  Color black=ColorBlack;
  Color white=ColorWhite;

  //Ԥ�����������Ŀ�ȼ��߶�
  int WIDTH=140;
  int HEIGHT=40;

  //�趨����ı�������Ϣ
  SolidBrush redBrush(ColorRed);
  Font myFont(L"Arial", 10);

  //column_count������ÿһ���ܹ����ƾ��ε�����
  int column_count=MyClient.Width/WIDTH;
  int rol=0;
  int column=0;

  //�ڵ�ǰ����ʹ�����е�Ӱ�߻�ˢ�ַ��������
  Pen pen(ColorBlue, 1);
  for (int i=HatchStyleHorizontal; i<HatchStyleTotal; i++)
  {
    HatchStyle style=(HatchStyle)i;
    //���һ���Ѿ�������ϣ�����
    if (rol>column_count-1)
    {
      column+=2;
      rol=0;
    }
    //������ʱ��ˢ
    HatchBrush brush_tmp(style, black, white);
    //�����Σ����ÿ��ΪWIDTH-20��Ŀ�����þ���֮���������
    gcFillRectangle(&brush_tmp, rol*WIDTH, column*HEIGHT, WIDTH-20, HEIGHT);
    //���ƾ��α߿�
    gcDrawRectangle(&pen, rol*WIDTH, column*HEIGHT, WIDTH-20, HEIGHT);

    //��ʾÿ�ֻ�ˢ����ö������
    //�����ı��������
    RectF layoutRect(rol*WIDTH, (column+1)*HEIGHT, WIDTH, HEIGHT);
    StringFormat format;
    //�����ı������ʽ��ˮƽ����ֱ����
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);
    //�ھ��ο��������ö��ֵ
    CString s; s.Format(_T("%d"), style);
    MyDrawString(graphics, s, myFont, redBrush, layoutRect, format);
    rol+=1;
  }
}
//���û���ԭ��
void Brush_SetRenderingOrigin()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�趨��ˢ��ǰ��ɫΪ��ɫ������ɫΪ��ɫ
  Color black=ColorBlack;
  Color white=ColorWhite;
  HatchBrush hatchBrush(HatchStyleDarkDownwardDiagonal,black,white);

  //����ֱ�������8�����Σ�ʹ��Ĭ�ϵĻ�ˢԭ��
  for(int i=0;i<8;i++)
  {
    gcFillRectangle(&hatchBrush, 0, i*50, 100, 50);  
  }

  //ʹ�ò�ͬ�Ļ���ԭ������������
  for(i=0;i<8;i++)
  {
    //���û�ˢԭ��(ˮƽ�������)
    gcSetRenderingOrigin(i, 0);
    gcFillRectangle(&hatchBrush, 100, i*50, 100, 50);
  }
}
//����ˢ�Ĳ�ͬ���ط�ʽ
void Brush_Texture()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"����",12);

  //��������ˢ�Ĳ�ͬ�������
  RectF rect1(10, 10, 200,200);
  RectF rect2(210, 10, 200, 200);
  RectF rect3(410, 10, 200, 200);

  //װ������ͼƬ
  Bitmap image(L"nemo.bmp");
  //��������ˢ1��ʹ��Ĭ�ϵķ�ʽ
  TextureBrush tBrush(&image);
  //ʹ������ˢ���Բ������
  gcFillEllipse(&tBrush,rect1);
  //����Բ��
  gcDrawEllipse(&pen,rect1);
  MyDrawString(graphics, "ͼƬԭʼ��С", myFont, brush, PointF(40,220));

  //��������ˢ2��ֻʹ�ø���ͼƬ�Ĳ�������
  TextureBrush tBrush2(&image, Rect(55,35,55,35));
  gcFillEllipse(&tBrush2,rect2);
  gcDrawEllipse(&pen,rect2);
  MyDrawString(graphics, "ʹ�ò��ֽ�ͼ", myFont, brush, PointF(240,220)); 

  //��������ˢ3����ʹ��ͼƬ�Ļ�ˢ��������
  TextureBrush tBrush3(&image);
  //�Ի�ˢ����50%������
  Matrix mtr(0.5f, 0.0f, 0.0f,0.5f, 0.0f, 0.0f);
  tBrush3.SetTransform(&mtr);
  gcFillEllipse(&tBrush3,rect3);
  gcDrawEllipse(&pen,rect3);
  MyDrawString(graphics, "������СͼƬ",myFont,brush, PointF(440,220));
}

//ʹ��ͼƬ���з�ʽ
void Brush_Texture_WrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 13);

  //װ������ͼƬ
  Bitmap image(L"nemo.bmp");
  //��������ˢ
  TextureBrush tBrush(&image);
  //����ˢ��������
  Matrix mtr(0.5f, 0.0f, 0.0f,0.5f, 0.0f, 0.0f);
  tBrush.SetTransform(&mtr);

  int i=0;
  //��ͼƬ��ʹ�����з�ʽ
  tBrush.SetWrapMode(WrapModeClamp);
  gcFillRectangle(&tBrush, Rect(i*150,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150,0,150,150));
  MyDrawString(graphics, "Clamp", myFont,brush, PointF(0,155)); 

  i+=1;
  //��ͼƬʹ��ƽ�����з�ʽ  
  tBrush.SetWrapMode(WrapModeTile);
  gcFillRectangle(&tBrush, Rect(i*150+10,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150+10,0,150,150));
  MyDrawString(graphics, "Tile",myFont,brush, PointF(170,155) ); 

  //��ͼƬʹ��ˮƽ��ת���з�ʽ
  i+=1;
  tBrush.SetWrapMode(WrapModeTileFlipX);
  gcFillRectangle(&tBrush, Rect(i*150+20,0,150,150));
  gcDrawRectangle(&pen, Rect(i*150+20,0,150,150));

  MyDrawString(graphics, "TileFlipX",myFont,brush, PointF(320,155)); 

  //��ͼƬʹ�ô�ֱ��ת���з�ʽ
  tBrush.SetWrapMode(WrapModeTileFlipY);
  gcFillRectangle(&tBrush, Rect(0,180,150,150));
  gcDrawRectangle(&pen, Rect(0,180,150,150));
  MyDrawString(graphics, "TileFlipY",myFont,brush,  PointF(0,335)); 

  //��ͼƬʹ��ˮƽ����ֱͬʱ��ת���з�ʽ
  tBrush.SetWrapMode(WrapModeTileFlipXY);
  gcFillRectangle(&tBrush, Rect(160,180,150,150));
  gcDrawRectangle(&pen, Rect(160,180,150,150));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, PointF(170,335)); 
}
//����ˢ�ı任
void Brush_TextureTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //Ϊ���ֲ�ͬ�ı任��ʽ�Ļ�ˢ�����������
  RectF rect1= RectF (10, 10, 200,200);
  RectF rect2= RectF (210, 10, 200, 200);
  RectF rect3= RectF (410, 10, 200, 200);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"����", 12);

  //װ������ͼƬ
  Bitmap image(L"nemo.bmp");
  //��������ˢ
  TextureBrush tBrush(&image);

  //����ˢ��ת30��
  tBrush.RotateTransform(30);                     
  gcFillEllipse(&tBrush,rect1);
  gcDrawEllipse(&pen,rect1);
  MyDrawString(graphics, "��ת30��",myFont,brush, PointF(40,220)); 

  //���ñ任���󣺻ָ����仯ǰ��״̬
  tBrush.ResetTransform();
  //����ˢ��ˮƽ��������������
  tBrush.ScaleTransform(3, 1);
  gcFillEllipse(&tBrush, rect2);
  gcDrawEllipse(&pen,rect2);
  MyDrawString(graphics, "������������",myFont,brush,  PointF(240,220)); 

  //ƽ�Ʊ任
  tBrush.ResetTransform();
  //����ˢ��ˮƽ������ƽ��30������
  tBrush.TranslateTransform(30, 0, MatrixOrderAppend);
  gcFillEllipse(&tBrush, rect3);
  gcDrawEllipse(&pen,rect3);
  MyDrawString(graphics, "����ƽ��30������",myFont,brush, PointF(440,220)); 
}
//��ѯ��ˢ�ı任��Ϣ
void Brush_GetTextureMatrix()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Matrix matrix;
  float elements[6];
  RectF rect1(10, 10, 200,200);
  Pen pen(ColorBlack, 2);
  SolidBrush brush(ColorBlack);

  Bitmap image(L"nemo.bmp");
  TextureBrush tBrush(&image);
  //������������任
  tBrush.RotateTransform(30);
  tBrush.TranslateTransform(5,3);
  tBrush.ScaleTransform(0.5f,2.0f);

  //��ȡĿǰ�Ѿ����еĻ�ˢ�任
  tBrush.SetTransform(&matrix);
  //�Ӿ��ε�����
  matrix.GetElements(elements);  
  gcFillEllipse(&tBrush,rect1);
  gcDrawEllipse(&pen,rect1);

  //��������Ԫ��
  for(int j = 0; j <6; ++j)
  {
    CString s; s.Format(_T("%d %f ����Ԫ��ֵ"), j, elements[j]);
    AfxMessageBox(s);
  }
}
//���Խ��仭ˢ����
void Brush_LinearGradientBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����һ���ɺ�ɫ����ɫ����Ļ�ˢ:ˮƽ�任����Ŀ��Ϊ40
  //��ֱ���򲻽���ɫ�ʽ���
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(40, 0),
    ColorRed,  //���ɫ��  
    ColorBlue);  //ֹ��ɫ��

  //����һ��ɫ�ʳʶԽ��߱任�����������СΪ40*40
  gcGradientBrush linGrBrush2(
    Point(0, 0),
    Point(40, 40),
    ColorRed,  //���ɫ��  
    ColorBlue); //ֹ��ɫ��  

  //�ֱ���ʾ��ͬ�����Խ��仭ˢ��Ŀ������Ĳ�ͬ���Ч��
  gcFillRectangle(&linGrBrush, 0, 0, 200, 200);
  gcFillRectangle(&linGrBrush2, 240, 0, 200, 200);

  Pen pen(ColorGray,1);
  //�ڶԽ��߷����ϻ��Ƶ�����ˢ���������
  for(int i=0;i<5;i++)
  {
    gcDrawRectangle(&pen, 240+i*40,i*40,40,40);
  }
}
//�������Խ��仭ˢ����䷽ʽ
void Brush_LinearArrange()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 13);

  //����һ��ɫ�ʳʶԽ��߱任�����������СΪ40*40
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(40, 40),
    ColorRed, //���ɫ��  
    ColorBlue); //ֹ��ɫ��  

  int i=0;
  //�Խ��仭ˢʹ��ƽ�����з�ʽ(Ĭ�Ϸ�ʽ)
  linGrBrush.SetWrapMode(WrapModeTile);
  //���һ����СΪ160��������
  gcFillRectangle(&linGrBrush, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "Tile", myFont, brush, Point(20,165)); 

  //�Խ��仭ˢʹ��ˮƽ��ת���з�ʽ
  i+=1;
  linGrBrush.SetWrapMode(WrapModeTileFlipX);
  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(160,0);
  gcFillRectangle(&linGrBrush, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "TileFlipX", myFont, brush, Point(170,165)); 

  //�Խ��仭ˢʹ�ô�ֱ��ת���з�ʽ
  linGrBrush.SetWrapMode(WrapModeTileFlipY);
  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(0,200);
  gcFillRectangle(&linGrBrush, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "TileFlipY",myFont,brush, Point(0,375)); 

  //�Խ��仭ˢʹ��ˮƽ����ֱͬʱ��ת���з�ʽ
  linGrBrush.SetWrapMode(WrapModeTileFlipXY);
  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush, Rect(160,200,160,160));
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, Point(170,375)); 

  //��ע���仭ˢ��������Ĵ�С��������
  Pen pen2(ColorGray, 1);
  for(i=0;i<8;i++)
    gcDrawLine(&pen2,0,i*40,320,i*40);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i*40,0,i*40,360);
}
//�������Խ��仭ˢ�Ľ���ģʽ
void Brush_LinearGradientMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //����һ��ˮƽ���仭ˢ����С40*20
  gcGradientBrush linGrBrush1(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeHorizontal);  

  //����һ����ֱ���仭ˢ
  gcGradientBrush linGrBrush2(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeVertical); 

  //�����ϵ����µĽ��仭ˢ
  gcGradientBrush linGrBrush3(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeForwardDiagonal);  

  //�����ϵ����µĽ��仭ˢ
  gcGradientBrush linGrBrush4(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,LinearGradientModeBackwardDiagonal);  

  int i=0;
  //ʹ��ˮƽ����Ļ�ˢ�������
  gcFillRectangle(&linGrBrush1, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "ˮƽ����",myFont,brush, PointF(20,165));

  i+=1;
  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(160,0);
  //ʹ�ô�ֱ����Ļ�ˢ�������
  gcFillRectangle(&linGrBrush2, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "��ֱ����",myFont,brush, PointF(170,165)); 

  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(0,200);
  //ʹ�ô����ϵ����½���Ļ�ˢ�������
  gcFillRectangle(&linGrBrush3, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "����->����",myFont,brush, PointF(0,375)); 

  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush4, Rect(160,200,160,160));
  //ʹ�ô����ϵ����½���Ļ�ˢ�������
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "����->����",myFont,brush, PointF(170,375)); 

  //�ڲ�ͬ�������б�ע���彥�䷽��ľ���
  Pen pen2(ColorGray, 1);
  for(i=0;i<18;i++)
    gcDrawLine(&pen2,0,i*20,320,i*20);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i* 40,0,i*40,360);
}
//������ƫת�Ƕ�
void Brush_LinearAngle()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //����һ��������ƫת�Ƕ�Ϊ30�ȵĽ��仭ˢ����С40*20
  gcGradientBrush linGrBrush1(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,30.0f);  
  //����һ��������ƫת�Ƕ�Ϊ45�ȵĽ��仭ˢ
  gcGradientBrush linGrBrush2(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,45.0f); 
  //����һ��������ƫת�Ƕ�Ϊ90�ȵĽ��仭ˢ
  gcGradientBrush linGrBrush3(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,90.0f);  
  //����һ��������ƫת�Ƕ�Ϊ180�ȵĽ��仭ˢ
  gcGradientBrush linGrBrush4(
    Rect(0, 0, 40, 20),
    ColorRed,  
    ColorBlue,180.0f);  

  int i=0;
  //ʹ��ƫת�Ƕ�Ϊ30�ȵĽ��仭ˢ�������
  gcFillRectangle(&linGrBrush1, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,0,160,160));
  MyDrawString(graphics, "30��ƫת",myFont,brush, PointF(20,165)); 

  i+=1;
  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(160,0);
  //ʹ��ƫת�Ƕ�Ϊ45�ȵĽ��仭ˢ�������
  gcFillRectangle(&linGrBrush2, Rect(i*160,0,160,160));
  gcDrawRectangle(&pen, Rect(i*160,160,0,160));
  MyDrawString(graphics, "45��ƫת",myFont,brush, PointF(170,165));

  //���û�ͼƽ��ԭ��
  gcSetRenderingOrigin(0,200);
  //ʹ��ƫת�Ƕ�Ϊ90�ȵĽ��仭ˢ�������
  gcFillRectangle(&linGrBrush3, Rect(0,200,160,160));
  gcDrawRectangle(&pen, Rect(0,200,200,160));
  MyDrawString(graphics, "90��ƫת",myFont,brush, PointF(0,375));

  gcSetRenderingOrigin(160,200);
  gcFillRectangle(&linGrBrush4, Rect(160,200,160,160));
  //ʹ��ƫת�Ƕ�Ϊ180�ȵĽ��仭ˢ�������
  gcDrawRectangle(&pen, Rect(160,200,160,160));
  MyDrawString(graphics, "180��ƫת",myFont,brush, PointF(170,375));

  //�ڲ�ͬ�������б�ע���彥�䷽��ľ���
  Pen pen2(ColorGray, 1);
  for(i=0;i<18;i++)
    gcDrawLine(&pen2,0,i*20,320,i*20);
  for(i=0;i<9;i++)
    gcDrawLine(&pen2,i* 40,0,i*40,360);
}

//��ɫ���仭ˢ
void Brush_LinearInterpolation()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�������ֲ��뽥���ɫ��
  Color colors[] = 
  {
    ColorRed,   // ��ɫ
    ColorGreen,//����ɫΪ��ɫ
    ColorBlue // ��ɫ
  };

  float positions[] = 
  {
    0.0f,   // �ɺ�ɫ��
    0.3f,   // ��ɫʼ�ڻ�ˢ���ȵ�����֮һ
    1.0f   // ����ɫֹ
  };  

  //����һ���Ӻ�ɫ����ɫ�Ľ��仭ˢ
  gcGradientBrush linGrBrush(
    Point(0, 0), 
    Point(180, 0), 
    ColorBlack,ColorWhite);

  //���ý��仭ˢ�Ķ�ɫ������Ϣ
  //linGrBrush.InterpolationColors=clrBlend;
  linGrBrush.SetInterpolationColors(colors, positions, 3);
  //ʹ�ö�ɫ���仭ˢ���Ŀ������
  gcFillRectangle(&linGrBrush, 0, 0, 180, 100);

  //ʹ����ͨ�ķ���ʵ�ֶ�ɫ����
  //�ɺ쵽�̣�����60
  gcGradientBrush linGrBrush1(
    Point(0, 0), 
    Point(60, 0),
    ColorRed,         
    ColorGreen);  
  //���̵���������120
  gcGradientBrush linGrBrush2(
    Point(60, 0), 
    Point(181, 0),
    ColorGreen,        
    ColorBlue);  
  //�ֱ�ʹ��������ˢ����������������γɶ�ɫ����
  gcFillRectangle(&linGrBrush1, 0, 120, 60, 100);
  gcFillRectangle(&linGrBrush2, 60, 120, 120, 100);
}
//�Զ��彥����̣�������
void Brush_LinearCustomize()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  Font myFont(L"����", 12);
  //����һ��˫ɫ���仭ˢ
  gcGradientBrush linGrBrush(
    Point(0, 0),
    Point(250, 0),
    ColorRed,   
    ColorBlue);  

  //��������ʹ��Ĭ�Ͻ��䷽ʽ���ľ��Σ��������Ƚ�
  gcFillRectangle(&linGrBrush, 0, 0, 250, 15);
  gcFillRectangle(&linGrBrush, 500, 0, 250, 15);

  //���θı�ϳɵ�λ��
  int row=1;
  for(float i=0.0f;i<1.0f;i+=0.1f)
  {
    linGrBrush.SetBlendTriangularShape(i);  
    gcFillRectangle(&linGrBrush, 0, row*15, 250, 15);
    row++;
  }
  MyDrawString(graphics, "�ı�ϳɵ�λ��",
    myFont,brush, PointF(40,200));

  //���θı��ɫ�ϳ�����
  row=1;
  for(i=0.0f;i<1.0f;i+=0.1f)
  {
    //���ϳɵ����λ�����������������50%��
    linGrBrush.SetBlendTriangularShape (0.5f,i);  
    gcFillRectangle(&linGrBrush, 500, row*15, 250, 15);
    row++;
  }
  MyDrawString(graphics, "�ı�ɫ�ʺϳ�����",
    myFont,brush, PointF(540,200));

}
//�����������ߵĽ��仭ˢ    
void Brush_LinearGradientBrush_BellShape()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect myRect(10, 10, 150, 75);
  //�������Խ��仭ˢ
  gcGradientBrush myLGBrush(
    myRect,ColorBlue,ColorRed,0.0f,true);

  //ʹ��Ĭ�ϵ����Խ��仭ˢ�����Բ
  gcFillEllipse(&myLGBrush, myRect);

  //������������óɻ����������ߵĽ���
  myLGBrush.SetBlendBellShape(.5f, 1.0f);

  //ʹ���Զ��彥����̵Ļ�ˢ�����Բ
  gcTranslateTransform(160,0);
  gcFillEllipse(&myLGBrush, myRect);
}

void Brush_PathGradientBrush_Star()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorGray,1);
  SolidBrush pthGrBrush(ColorRed);
  SolidBrush blackbrush(ColorBlue);
  gcTranslateTransform(20,20);

  //�������ǵ�10���ߵĶ˵�����
  Point points[] = 
  {
     Point(75, 0), Point(100, 50), 
     Point(150, 50), Point(112, 75),
     Point(150, 150), Point(75, 100), 
     Point(0, 150), Point(37, 75), 
     Point(0, 50), Point(50, 50),
     Point(75, 0)
   };

  // ����·��
  GraphicsPath path;
  //��·���м���ֱ��
  path.AddLines(points, sizeof(points)/sizeof(Point));
  //���·��
  gcFillPath(&pthGrBrush, &path);
  //���Ʊ߽�
  gcDrawLines(&pen, points, sizeof(points)/sizeof(Point));
  //���ƶ���10���ߵĶ˵�
  for(int i=0;i<10;i++)
    //ÿ��Բ���ֱ��Ϊ10
    gcFillEllipse(&blackbrush, points[i].X-5,points[i].Y-5,10,10);
}
//ʹ��·�����仭ˢ��������
void Brush_PathGradientBrush_Star2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�������ǵ�10���ߵĶ˵�����
  Point points[] =
  {
     Point(75, 0), Point(100, 50), 
     Point(150, 50), Point(112, 75),
     Point(150, 150), Point(75, 100), 
     Point(0, 150), Point(37, 75), 
     Point(0, 50), Point(50, 50),
     Point(75, 0)
  };
  // ����·��
  GraphicsPath path;
  //��·�������ֱ��
  path.AddLines(points, sizeof(points)/sizeof(Point));
  //����·�����仭ˢ
  PathGradientBrush pthGrBrush(&path);
  //�������ĵ�ɫ��(�յ�ɫ)
  pthGrBrush.SetCenterColor(ColorRed);

  //����ÿ���˵��ɫ��(�յ�ɫ)
  Color colors[] =
  {
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite, 
    ColorBlack, ColorGreen, 
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen
  };
  //����·�����仭ˢ�ı�Եɫ
  int clrCount=sizeof(colors)/sizeof(Color);
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  // ���Ŀ��·��
  gcFillPath(&pthGrBrush, &path);

  //������ɫ���߽�ɫ�����ó����ɫ���鿴����Ч��
  for(int z=0;z<10;z++)
  {
    //��ˮƽ������ƽ�ƻ�ͼƽ��
    gcTranslateTransform(200.0f, 0.0f);
    //�������ĵ�ɫ��Ϊ���ɫ
    pthGrBrush.SetCenterColor(_MakeA_RGB(rand_Next(255)%155,rand_Next(255)%255,rand_Next(255)%255));
    //ʹ��ԭ�еı�Եɫ
    pthGrBrush.SetSurroundColors(colors, &clrCount);
    gcFillPath(&pthGrBrush, &path);
  }
}
//ʹ�ö��·�����仭ˢ
void Brush_Using_MorePathGradientBrush()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����һ�������Σ��߳�Ϊ50  
  float fHalf = 50 * (float)sin(120.0f/360.0f*PI); 
  PointF pts[] =
  {
    PointF( 50, 0),           
    PointF( 50 * 1.5f,  0),
    PointF( 50,         0),           
    PointF( 50 / 2,    -fHalf),
    PointF(-50 / 2,    -fHalf),
    PointF(-50,         0),
    PointF(-50 * 1.5f,  0), 
    PointF(-50,         0),
    PointF(-50 / 2,     fHalf),     
    PointF( 50 / 2,     fHalf)
  };

  //���������ν��仭ˢ
  PathGradientBrush pgbrush1(pts, 10);

  //��ˮƽ�ʹ�ֱ������ƽ�������εĶ���
  for (int i = 0; i <10; i++)
  {
    pts[i].X += 50*1.5f;
    pts[i].Y += fHalf;
  }

  //���ݸı������ĵ���������·�����仭ˢ
  PathGradientBrush pgbrush2(pts, 10);

  //����·�����仭ˢ�ķ�ת��ʽΪƽ��
  pgbrush1.SetWrapMode(WrapModeTile);
  pgbrush2.SetWrapMode(WrapModeTile);

  //�ֱ�����������ˢ�����ĵ�ɫ��Ϊ�졢��ɫ
  pgbrush1.SetCenterColor(ColorRed);
  pgbrush2.SetCenterColor(ColorGreen);

  //��䵱ǰ����
  gcFillRectangle(&pgbrush1, 0, 0,MyClient.Width,MyClient.Height);
  //���ϴ�δ�������������ٴ���䵱ǰ���ڵĿհײ���
  gcFillRectangle(&pgbrush2, 0, 0,MyClient.Width,MyClient.Height);
}
//·�����仭ˢ����䷽ʽ
void Brush_PathGradientBrush_WrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 2);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //�������ǵı������꣬Ϊ��ʾ��תЧ���������ǵ�һ������
  Point points[] =
  {
    Point(75, 0), Point(100, 50), 
    Point(150, 50), Point(112, 75),
    Point(150, 150), Point(75, 100), 
    Point(0, 190), Point(37, 75), 
    Point(10, 50), Point(50, 50)
  };

  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(Point));

  // ����·�����仭ˢ
  PathGradientBrush pthGrBrush(&path);
  //�������ĵ�ɫ��(�յ�ɫ)
  pthGrBrush.SetCenterColor(ColorRed);

  //����ÿ���˵��ɫ��(�յ�ɫ)
  Color colors[]=
  {
    ColorBlack,   ColorGreen,
    ColorBlue, ColorWhite, 
    ColorBlack,   ColorGreen, 
    ColorBlue, ColorWhite,
    ColorBlack,   ColorGreen
  };

  int clrCount=sizeof(colors)/sizeof(Color);
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  //��С��ˢ
  pthGrBrush.ScaleTransform(0.2f,0.2f);
  int i=0;

  //�Խ��仭ˢʹ��ƽ�����з�ʽ(Ĭ�Ϸ�ʽ)
  pthGrBrush.SetWrapMode(WrapModeTile);
  gcFillRectangle(&pthGrBrush, Rect(i*120,0,120,120));
  gcDrawRectangle(&pen, Rect(i*120,0,120,120));
  MyDrawString(graphics, "Tile",myFont,brush, PointF(20,125));

  i+=1;
  pthGrBrush.SetWrapMode(WrapModeTileFlipX);
  gcFillRectangle(&pthGrBrush, Rect(i*120,0,120,120));
  gcDrawRectangle(&pen, Rect(i*120,0,120,120));
  MyDrawString(graphics, "TileFlipX",myFont,brush, PointF(170,125));

  //�Խ��仭ˢʹ�ô�ֱ��ת���з�ʽ
  pthGrBrush.SetWrapMode(WrapModeTileFlipY);
  gcFillRectangle(&pthGrBrush, Rect(0,200,120,120));
  gcDrawRectangle(&pen, Rect(0,200,120,120));
  MyDrawString(graphics, "TileFlipY",myFont,brush, PointF(0,325));

  //�Խ��仭ˢʹ��ˮƽ����ֱͬʱ��ת���з�ʽ
  pthGrBrush.SetWrapMode(WrapModeTileFlipXY);
  gcFillRectangle(&pthGrBrush, Rect(120,200,120,120));
  gcDrawRectangle(&pen, Rect(120,200,120,120));
  MyDrawString(graphics, "TileFlipXY",myFont,brush, PointF(170,325));

  //���·����Լ�����Ρ����ĵ���Ϣ
  RectF rect;
  //��ȡ��ˢ��Լ�����ζ���
  pthGrBrush.GetRectangle(&rect);
  PointF CenterPoint;
  //��ȡ��ˢ�����ĵ���Ϣ
  pthGrBrush.GetCenterPoint(&CenterPoint);
  CString tmp;

  //��ʽ���ַ���
  tmp.Format("��ǰԼ�����ε���������Ϊ(%f,%f),���=%f �߶�=%f\n��ǰ����·�������ĵ�����Ϊ(%.2f,%.2f)",
    rect.X,rect.Y,rect.Height,rect.Width,
    CenterPoint.X,CenterPoint.Y);
  //������ĵ㼰Լ�����ζ������Ϣ
  MyDrawString(graphics, tmp,myFont,brush, PointF(0,395));
}
//����·�����仭ˢ�����ĵ�
void Brush_PathGradientBrush_CenterPoint()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����һ��Բ������
  GraphicsPath path;
  path.AddEllipse(0, 0, 200, 200);
  SolidBrush brush(_MakeA_RGB(155,ColorRed));
  //����һ��Բ��·�����仭ˢ
  PathGradientBrush pthGrBrush(&path);

  //�������ĵ�ɫ��
  pthGrBrush.SetCenterColor(_MakeA_RGB(155,ColorWhite));
  Color colors[] ={_MakeA_RGB(55, ColorBlue)};
  //���ñ�Եɫ
  int clrCount=1;
  pthGrBrush.SetSurroundColors(colors, &clrCount);
  //�������ʹ�õ�ɫ��ˢ
  gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
  //��ȡ���ĵ�ɫ��
  PointF center(0,0);
  pthGrBrush.GetCenterPoint(&center);

  //���Ļ�ˢ�����ĵ���Բ�ܵ��ϰ벿��ƽ��
  for(int i=0;i<200;i++)
  {
    center.X=(REAL)i;
    center.Y=10.f;
    pthGrBrush.SetCenterPoint(center);
    gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
    //��ǵ�ǰ���ĵ�
    gcFillEllipse(&brush, center.X, center.Y, 2.f, 2.f);
  }

  //���Ļ�ˢ�����ĵ���Բ�ܵ��°벿��ƽ��
  for(i=200;i>0;i--)
  {
    center.X=(REAL)i;
    center.Y=190;
    pthGrBrush.SetCenterPoint(center);
    gcFillEllipse(&pthGrBrush, 0, 0, 200, 200);
    //��ǵ�ǰ���ĵ�
    gcFillEllipse(&brush, center.X, center.Y, 2.f, 2.f);
  }

}
//��·�����仭ˢʹ�ö�ɫ����
void Brush_PathGradientBrush_InterpolationColors()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorRed);
  //���������ε�������
  Point points[]=
  {
    Point(100, 0), 
    Point(200, 200),
    Point(0, 200)
  };
  //����һ�������ν��仭ˢ
  PathGradientBrush pthGrBrush(points, 3);
  //-������뽥���ɫ��
  Color  colors[]=
  {
    ColorRed,     //��
    ColorGreen,     //��
    ColorBlue // ��
  }; 

  //���úϳɵ��λ��
  float pos[] =
  {
    0.0f,    // ��ɫ������߽�Ϊ��ɫ
    0.4f,    //�ھ�������40%��λ�ô�ʹ����ɫ
    1.0f  //���ĵ�ʹ����ɫ
  };   

  //���ý���Ĺ���ɫ
  pthGrBrush.SetInterpolationColors(colors, pos, 3);
  //�������
  gcFillRectangle(&pthGrBrush, 0, 0, 300, 300);  
  //������ĵ�
  PointF centerpoint(0,0);;
  pthGrBrush.GetCenterPoint(&centerpoint);
  gcFillEllipse(&brush, centerpoint.X-5.f, centerpoint.Y-5.f, 10.f, 10.f);
}
//����·�����仭ˢ�Ľ������ű���
void Brsuh_PathGradietBrush_Focus()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlack,3);
  SolidBrush brush(ColorBlack);
  Font myFont(L"Arial", 12);

  //ָ����������������,���������ν��仭
  Point points[]=
  {
    Point(100, 0),
    Point(200, 200),
    Point(0, 200)
  };

  PathGradientBrush pthGrBrush(points, 3);
  //ָ������ɫ
  Color colors[] = {ColorRed, ColorBlue};
  //ָ��ɫ�ʺϳ�λ��
  float relativePositions[]=
  {
    0.0f,    // ��ɫ���߽�
    1.0f // ��ɫΪ����
  };

  //���ý���ɫ
  pthGrBrush.SetInterpolationColors(colors, relativePositions, 2);

  //ʹ��Ĭ���������ӽ������
  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200); 

  //��ȡĬ���������� 
  PointF FocusScales(0,0);
  pthGrBrush.GetFocusScales(&FocusScales.X, &FocusScales.Y);
  //���������Ϣ
  CString tmp;
  tmp.Format("ˮƽ:x=%.2f\n��ֱ:y=%.2f",FocusScales.X,FocusScales.Y);
  MyDrawString(graphics, tmp,myFont,brush, PointF(0,210));

  //ƽ�ƻ�ͼƽ��
  gcTranslateTransform(200,0);
  //������������
  FocusScales.X=0.6f;
  FocusScales.Y=0.6f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200);  
  MyDrawString(graphics, "ˮƽ:x=0.6\n��ֱ:y=0.6", myFont,brush, PointF(0,210));

  //ˮƽ���Ų����ڴ�ֱ����ʱ����
  gcTranslateTransform(200,0);
  //������������
  FocusScales.X=0.1f;
  FocusScales.Y=0.8f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush,0, 0, 200, 200); 
  MyDrawString(graphics, "ˮƽ:x=0.1\n��ֱ:y=0.8", myFont,brush, PointF(0,210));

  //ˮƽ����=��ֱ����=1.0
  gcTranslateTransform(200,0);
  FocusScales.X=1.0f;
  FocusScales.Y=1.0f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200); 
  MyDrawString(graphics, "ˮƽ:x=1.0\n��ֱ:y=1.0", myFont,brush, PointF(0,210));

  //ˮƽ���Ų����ڴ�ֱ����ʱ�������Ŵ�ԭ������
  gcTranslateTransform(200,0);
  FocusScales.X=2.0f;
  FocusScales.Y=1.5f;
  pthGrBrush.SetFocusScales(FocusScales.X,FocusScales.Y);

  gcFillRectangle(&pthGrBrush, 0, 0, 200, 200);
  MyDrawString(graphics, "ˮƽ:x=2.0\n��ֱ:y=1.5", myFont,brush, PointF(0,210));
}
//·�����仭ˢ�ı任
void Brush_PathGradientBrush_Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // ����һ��������·�����仭ˢ
  Point pts[]={Point(50, 0), Point(100, 100), Point(0, 100)};
  PathGradientBrush pthGrBrush(pts, 3);

  //�Ի�ˢʹ�ô�ֱ��ת��ʽ
  pthGrBrush.SetWrapMode(WrapModeTileFlipY);
  //��С��ˢ  
  pthGrBrush.ScaleTransform(0.5f, 0.5f);
  //��䵱ǰ����
  gcFillRectangle(&pthGrBrush, 0, 0, MyClient.Width,MyClient.Height);

  //����ˢ��ת90��
  pthGrBrush.RotateTransform(90.0f, MatrixOrderAppend);
  //�ٴ���䵱ǰ����
  gcFillRectangle(&pthGrBrush, 0, 0, MyClient.Width,MyClient.Height);
}

void Brsuh_LinearGradientBrush_UsingGamma()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  gcGradientBrush linGrBrush(
    Point(0, 10),
    Point(200, 10),
    ColorRed,
    ColorBlue);

  gcFillRectangle(&linGrBrush, 0, 0, 200, 50);
  linGrBrush.SetGammaCorrection(true);
  gcFillRectangle(&linGrBrush, 0, 60, 200, 50);

}
//�򵥵�ʹ������ʾ��
void Font_UsingFontInGDIPlus()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //�½�һ������Ի���
  CFontDialog dlg;
  //������������ɫ������
  //dlg.ShowColor=true;
  //��������ѡ��Ի���
  if(dlg.DoModal()!=IDOK)
    return;
  //��ȡ������Ի����е�������Ϣ
  LOGFONT lf; dlg.GetCurrentFont(&lf);
  Font myFont(CWideCharString(dlg.GetFaceName()), lf.lfHeight);
  //��������ɫ�ʴ�����ˢ
  SolidBrush brush(dlg.GetColor());
  //�����ı������ʽ���ڵ�ǰ�����о�����ʾ
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);
  //����ı�
  gcDrawString(L"����ı�", 4, &myFont, RectF(0,0,MyClient.Width,MyClient.Height),&fmt, &brush);
}

//ö�����е�����ϵ��
void Font_EnumAllFonts()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush   solidBrush(ColorBlack);
  FontFamily   fontFamily(L"Arial");
  Font         font(&fontFamily, 8, FontStyleRegular, UnitPoint);
  //�����ı������ʽ
  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentNear);
  fmt.SetLineAlignment(StringAlignmentNear);

  CString tmp="";
  //��ȡ�����Ѿ���װ������ϵ��
  InstalledFontCollection installedFontCollection;
  int ffcount=installedFontCollection.GetFamilyCount();
  FontFamily *fontfamily = ::new FontFamily[ffcount];
  installedFontCollection.GetFamilies(ffcount, fontfamily, &ffcount);
  int index=0;
  //����fontfamily�����ÿһ����Ա
  for( int i=0; i<ffcount; i++)
  {
    //��ȡ��ǰ����ϵ������
    WCHAR name[LF_FACESIZE];
    fontfamily[i].GetFamilyName(name);
    CString t=name;
    CString s; s.Format(_T("%s%s"), (i? _T(", "):_T("")), t);
    tmp+=s;
    index++;
  }
  ::delete []fontfamily;

  //�ڴ�����������е�����ϵ����
  RectF r(0,10,MyClient.Width,MyClient.Height);
  MyDrawString(graphics, tmp, font, solidBrush, r, fmt);
  //�������ϵ����Ϣ
  tmp.Format("�����ϵͳ���Ѿ���װ��������%d�֣������Ʒֱ�Ϊ:\n",index);
  MyDrawString(graphics, tmp,font,solidBrush, Point(0,0));
}

//������ǿ��ѡ��Ի����
void Font_EnhanceFontDialog()
{
  MyDrawString(GetGraphics(), "����ǿ��ѡ��Ի����",
    Font(L"Arial",12), SolidBrush(ColorRed), Point(0,0));
/*
  Form3 f= Form3();
  f.ShowDialog();
*/
}


//ʹ�ò�ͬ�������Ե����ʽ
void Font_UsingTextRenderHint()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brs(ColorBlack);
  FontFamily  fontFamily(L"Arial");
  Font        font(&fontFamily, 60, FontStyleRegular, UnitPixel);

  //ʹ�ò�ͬ�ı�Ե����ʽ��������ı�
  for (int i=TextRenderingHintSystemDefault; i<=TextRenderingHintClearTypeGridFit; i++)
  {
    //���ñ�Ե����ʽ
    gcSetTextRenderingHint((TextRenderingHint)i);
    //����ı�
    MyDrawString(graphics, "Render",font,brs, PointF(0, 0));
    //��ͼƽ�����Ƶ���
    gcTranslateTransform(0, font.GetHeight(&graphics));
  }
}
//ʹ��˽�����弯��
void Font_Privatefontcollection()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 10.0f);
  SolidBrush  solidBrush(ColorRed);
  //����˽�����弯
  PrivateFontCollection privateFontCollection;

  //��˽�����弯��׷���㷱�����ֿ��ļ�"������.TTF"
  WCHAR fontfile[]= L"c:\\������.TTF";
  if (Ok != privateFontCollection.AddFontFile(fontfile))
  {
    AfxMessageBox("�����ļ�����ʧ��\n��������");
    return;
  }

  //��˽�����弯���й��췱���壬��СΪ35����
  FontFamily pFontFamily(L"���㷱����", &privateFontCollection);
  Font tmpFont(&pFontFamily, 35);
  //�������������
  MyDrawString(graphics, "���۲���ǧ����", tmpFont,solidBrush, pointF);
  //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
  pointF.Y +=tmpFont.GetHeight(&graphics);
  MyDrawString(graphics, "����ǰͷ��ľ��", tmpFont,solidBrush, pointF);
}

//��˽�����弯����ʹ�ö�������
void Font_Privatefontcollection2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 10.0f);
  SolidBrush  solidBrush(ColorRed);
  //����˽�����弯
  PrivateFontCollection privateFontCollection;
  //��˽�����弯��׷��������ͬ�������ļ�
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\STCAIYUN.TTF");
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\SIMLI.TTF");
  privateFontCollection.AddFontFile(L"C:\\WINDOWS\\Fonts\\Arial.ttf");

  FontFamily fontfamily[200];
  int ffcount=privateFontCollection.GetFamilyCount();
  if (ffcount>200) ffcount=200;
  privateFontCollection.GetFamilies(ffcount, fontfamily, &ffcount);
  int index=0;
  //����˽�����弯���е���������ϵ��
  for(int i=0; i<ffcount; i++)
  {
    Font tmpFont(&fontfamily[i], 35);
    //��ȡ��ǰ����ϵ������
    WCHAR name[LF_FACESIZE];
    fontfamily[i].GetFamilyName(name);
    CString s; s.Format(_T("������:  %s"), name);
    //�������������
    MyDrawString(graphics, s, tmpFont, solidBrush, pointF);
    gcTranslateTransform(0, tmpFont.GetHeight(&graphics));
  }
/*
  //ͨ������PrivateFontCollection���Families�����Ա������
  for(int index=0;index< privateFontCollection..Families.Length;index++)
  {
    Font tmpFont(privateFontCollection.Families[index].Name,35);
    //�������������
    MyDrawString(graphics, "������:  "+privateFontCollection.Families[index].Name,
      tmpFont,solidBrush, pointF);
    gcTranslateTransform(0,tmpFont.Height);
  }
*/
}
//����������Ƿ����
void Font_IsStyleAvailable()
{

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorGray);
  PointF      pointF(10.0f, 0.0f);
  SolidBrush  solidBrush(ColorBlack);
  //��Ϣ�����ʹ�õ�����
  Font msgfont(L"Arial",12);

  //����˽�����弯
  PrivateFontCollection privateFontCollection;
  //��˽�����弯��׷�����������ļ�
  //����
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\simhei.ttf");
  //Courier New����
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\CourBI.ttf");
  //��������
  privateFontCollection.AddFontFile(L"C:\\windows\\Fonts\\STLITI.ttf");

  CString     familyName;
  CString     tmpmsg;

  //��ѯ˽�����弯������ϵ������
  int count=privateFontCollection.GetFamilyCount();
  //��ȡ����ϵ������
  FontFamily *pFontFamily = ::new FontFamily[count];      
  //������ϵ���е��������м���pFontFamily��
  privateFontCollection.GetFamilies(count, pFontFamily, &count);

  /*��˽�����弯��ÿһ������ϵ�н�������������
  �Ĳ�ѯ�������������ã�ʹ�ø÷������ı�����
  */
  for(int j = 0; j< count; ++j)
  {
    //������ϵ���л�ȡ������
    WCHAR name[LF_FACESIZE];
    pFontFamily[j].GetFamilyName(name);
    familyName=name;
    // �������Ƿ����
    if(pFontFamily[j].IsStyleAvailable(FontStyleRegular))
    {
      tmpmsg="  ����ĳ��������";
      //��������
      FontFamily fm(CWideCharString(familyName), &privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleRegular, UnitPixel);
      //���ʹ�ó����������
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        CString(L"  ����ĳ����񲻿���"),msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // �������Ƿ����
    if(pFontFamily[j].IsStyleAvailable(FontStyleBold))
    {
      tmpmsg="  ����Ĵ��������";
      //��������
      FontFamily fm(CWideCharString(familyName), &privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleBold, UnitPixel);
      //���ʹ�ó����������
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        CString("  ����Ĵ����񲻿���"),msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // б�����Ƿ����
    if(pFontFamily[j].IsStyleAvailable(FontStyleItalic))
    {
      tmpmsg="  �����б�������";
      //��������
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleItalic, UnitPixel);
      //���ʹ�ó����������
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  �����б���񲻿���",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // ��ѯ�»��߷���Ƿ����
    if(pFontFamily[j].IsStyleAvailable(FontStyleUnderline))
    {
      tmpmsg="  ������»��߷�����";
      //��������
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleUnderline, UnitPixel);
      //���ʹ�ó����������
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  ������»��߷�񲻿���",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // ��ѯǿ���߷���Ƿ����
    if(pFontFamily[j].IsStyleAvailable(FontStyleStrikeout))
    {
      tmpmsg="  �����ǿ���߷�����";
      //��������
      FontFamily fm(CWideCharString(familyName),&privateFontCollection);
      Font tmpFont(&fm, 12, FontStyleStrikeout, UnitPixel);
      //���ʹ�ó����������
      MyDrawString(graphics, familyName+tmpmsg,tmpFont,solidBrush,pointF );
      //��ֱ����һ��һ�У��и�Ϊ  ����ĸ߶�
      pointF.Y += tmpFont.GetHeight(&graphics);
    }
    else
    {
      MyDrawString(graphics, familyName+tmpmsg+
        "  �����ǿ���߷�񲻿���",msgfont,solidBrush,pointF);
      pointF.Y += msgfont.GetHeight(&graphics);
    }
    tmpmsg.Empty();

    // �ڲ�ͬ������ϵ��֮����ϼ����
    gcDrawLine(&pen,0,(int )pointF.Y,400,(int)pointF.Y);
    pointF.Y += 10.0f;
  }
  ::delete []pFontFamily;
}
//��ȡ����Ĵ�С
void Font_Size()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  PointF      pointF(0.0f, 0.0f);
  SolidBrush  solidBrush(ColorBlack);

  CString infoString;
  int  ascent;
  float  ascentPixel;            
  int  descent;               
  float  descentPixel;          
  int  lineSpacing;           
  float  lineSpacingPixel;      

  FontFamily   fontFamily(L"Arial");
  //����һ����СΪ16���ص�Aria����
  Font  font(&fontFamily, 16, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);

  // ��ʾ�����С
  infoString.Format("font��Size����ֵΪ%.2f",font.GetSize());
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
  // ����һ��
  pointF.Y += font2.GetHeight(&graphics);
  //��ʾ����߶ȣ�����
  infoString.Format("font��Height����ֵΪ%.2f", font.GetHeight(&graphics));
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
  // ����һ��
  pointF.Y += font2.GetHeight(&graphics);

  // ��ʾ����ϵ�еĸ߶�(��Ƶ�λ)
  infoString.Format("ʹ��fontFamily.GetEmHeight�������ص�����߶�Ϊ %d ����Ƶ�λ��"
    ,fontFamily.GetEmHeight(FontStyleRegular));
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // ��������(Height���Ե�ֵ������ĸ߶ȣ���λΪ����)
  pointF.Y += 2.0f*font2.GetHeight(&graphics);

  // ��ȡ�����Ascent(�ϲ�����)
  ascent = fontFamily.GetCellAscent(FontStyleRegular);

  //���ϲ��������Ƶ�λת�������ص�λ
  ascentPixel= 
    font.GetSize() * ascent / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("�ϲ�����Ϊ %d����Ƶ�λ, %.2f�����ء�",
    ascent, ascentPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // ����һ��
  pointF.Y += font2.GetHeight(&graphics);
  // ��ȡ�����Descent(�²�����),��Ƶ�λ
  descent = fontFamily.GetCellDescent(FontStyleRegular);

  //���²��������Ƶ�λת�������ص�λ
  descentPixel = 
    font.GetSize() * descent / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("�²�����Ϊ%d ����Ƶ�λ, %.2f�����ء�",
    descent, descentPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);

  // ����һ��
  pointF.Y += font2.GetHeight(&graphics);
  //��ȡ�о�(��Ƶ�λ)
  lineSpacing = fontFamily.GetLineSpacing(FontStyleRegular);
  //���о����Ƶ�λת�������ص�λ
  lineSpacingPixel = 
    font.GetSize() * lineSpacing / fontFamily.GetEmHeight(FontStyleRegular);
  infoString.Format("�о�Ϊ%d ����Ƶ�λ, %.2f ���ء�",
    lineSpacing, lineSpacingPixel);
  MyDrawString(graphics, infoString,font2,solidBrush,pointF);
}
//�����ı��������
void Font_BaseLine()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily   fontFamily(L"Arial");
  //����һ����СΪ60���ص�Aria����
  Font  font(&fontFamily, 60, FontStyleRegular, UnitPixel);
  //��ȡ��ǰ���ڵľ��ο���
  int  cy=MyClient.Height;
  int cx=MyClient.Width;

  Pen pen(ColorBlue, 1);
  SolidBrush brush(ColorBlue);

  //�����ڴ�ֱ�����1/2������Ϊ����
  float yBaseline = cy / 2.f;
  //���ƻ���
  gcDrawLine(&pen, PointF(0, yBaseline), PointF((REAL)cx,yBaseline));
  //�õ�����߶�
  float cyLineSpace = font.GetHeight(&graphics);
  //��ȡ�о�
  int   iCellSpace  = fontFamily.GetLineSpacing(FontStyleRegular);
  //�õ��ϰ벿�־���
  int   iCellAscent = fontFamily.GetCellAscent(FontStyleRegular);
  //�����ı��������ʼλ������ڻ��ߵ�ƫ��
  float cyAscent    = cyLineSpace * iCellAscent / iCellSpace;

  //���������ϻ�������ֱ�ߣ����ΪcyAscent
  gcDrawLine(&pen, 
    PointF(0, yBaseline-cyAscent), PointF(cx,yBaseline-cyAscent));
  gcDrawLine(&pen, 
    PointF(0, yBaseline-2.0f*cyAscent), PointF(cx,yBaseline-2.0f*cyAscent));

  //�ֱ�����������������ı�
  MyDrawString(graphics, "AaFfgSs}", font,brush, PointF(0,yBaseline - cyAscent));
  MyDrawString(graphics, "AaFfgSs}",font,brush, PointF(0,yBaseline - 2.0f*cyAscent));

}

void Font_DrawString()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ı�
  CString msg="ʾ���ı�";

  Font myFont(L"����", 16);
  RectF layoutRect(50, 50, 200, 50);
  //���ö��뷽ʽ
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  SolidBrush blackBrush(ColorBlack);

  // �����ı�
  MyDrawString(graphics, msg, myFont, blackBrush, layoutRect, format);
  // �����ı�����ľ�������
  Pen pen(ColorBlack, 3);
  gcDrawRectangle(&pen, layoutRect);

}

void Font_MeasureString()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����һ�����������ı����ַ���
  CString txtOut="123456789\n";
  txtOut+="ABCDEFGHIJKLM\n";
  txtOut+="һ�����������߰˾�";

  FontFamily   fontFamily(L"Arial");
  //������������С��ͬ��Aria����
  Font  font(&fontFamily, 30, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  SolidBrush brush(ColorBlack);

  StringFormat fmt;
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);

  SizeF stringSize, strSize(1000,1000);      
  //��������ַ�������Ҫ�ľ��οռ�
  CWideCharString txtOutW(txtOut);
  gcMeasureString(txtOutW, txtOutW.GetLength(), &font, strSize, &fmt, &stringSize);

  CString tmp;
  tmp.Format("����ַ�������Ҫ�Ŀ��Ϊ:%2f  �߶�Ϊ%2f", stringSize.Width,stringSize.Height);

  // ��������ı������ƾ���
  Pen pen(ColorRed);
  gcDrawRectangle(&pen,
    10.0f,10.0f,stringSize.Width,stringSize.Height);
  //����ַ����Ĳ�����Ϣ
  MyDrawString(graphics, txtOut, font, brush,
    RectF(10.0f,10.0f,stringSize.Width,stringSize.Height),fmt);
  gcTranslateTransform(0,10+stringSize.Height);
  MyDrawString(graphics, tmp,font2,brush, PointF(0,0));
}
//������ָ������������ʾ���ַ�����������
void Font_MeasureString2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  CString txtOut="123456789";
  txtOut+="ABCDEFGHIJKLM";
  txtOut+="һ�����������߰˾�";

  FontFamily   fontFamily(L"Arial");
  //������������С��ͬ��Aria����
  Font  font(&fontFamily, 30, FontStyleRegular, UnitPixel);
  Font  font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  //�����ı��������
  RectF layoutRect(10, 10,100,100);
  SolidBrush brush(ColorBlack);

  //�����ı���ʾ��ʽ
  StringFormat fmt;    
  fmt.SetAlignment(StringAlignmentCenter);
  fmt.SetLineAlignment(StringAlignmentCenter);

  int codepointsFitted=0;
  int linesFilled=0;

  SizeF stringSize;
  //����ָ���������ܹ���ʾ���ַ�����������
  CWideCharString txtOutW(txtOut);
  gcMeasureString(txtOutW, txtOutW.GetLength(), 
    &font, SizeF(layoutRect.Width,layoutRect.Height),
    &fmt, &stringSize, &codepointsFitted, &linesFilled);

  CString tmp;
  tmp.Format("��������ִ���%d���ַ�\n����,��ָ�������������\nֻ�����%d�й�%d���ַ�",
    txtOutW.GetLength(), linesFilled, codepointsFitted);

  // ����ָ�����ı��������
  Pen pen(ColorBlue,2);
  gcDrawRectangle(&pen, layoutRect);
  //��ָ���������������ʾ�ı�
  MyDrawString(graphics, txtOut, font, brush, layoutRect, fmt);

  //����ַ����Ĳ�����Ϣ
  gcTranslateTransform(0,10+stringSize.Height);
  MyDrawString(graphics, tmp, font2, brush, PointF(0,0));
}
//�ı��ķ������
void Font_ColumnTextOut()
{
  CString s_Filename;

  //�����ȡ <exePath> ../ReadMe.txt
  TCHAR ModuleDirectory[_MAX_PATH];
  ::GetModuleFileName(NULL, ModuleDirectory, _MAX_PATH);
  TCHAR *p=::_tcsrchr(ModuleDirectory, '\\'); *p=0;
  p=::_tcsrchr(ModuleDirectory, '\\'); *p=0;
  ::_tcscat(ModuleDirectory, "\\ReadMe.txt");

  s_Filename=ModuleDirectory;

  FILE *stream = ::fopen(s_Filename, "rt");
  if (!stream)
    return;

  CString str;

  //��ȡ���е��ļ�����  
  char buffer[1024]="";
  while (!feof(stream))
  {
    CString s;
    ::fgets(buffer, 1024, stream);
    s=buffer; s.TrimLeft(); s.TrimRight(); s+="\r\n";
    str+=s;
  }
  //�ر��ļ�
  fclose(stream);

  //��������������֮��ļ��
  int COLUMWIDTH=256;
  int SPACE=10;

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  Font myFont(L"����", 10);
  int codepointsFitted=0;
  int linesFilled=0;

  //���������ʽ
  StringFormat format;
  //�����Զ�����
  format.SetFormatFlags(StringFormatFlagsNoClip);
  //����ı��Ե��ʽ���
  format.SetTrimming(StringTrimmingWord);

  CWideCharString strW(str);
  //�������Ұ���(��)����ı�
  for (int x = 0; strW.GetLength()> 0&&x<MyClient.Width; x +=(COLUMWIDTH+SPACE))
  {
    //�����ı���Ӧ�ľ�������(һ��)
    RectF layoutRect(x, 0, COLUMWIDTH,MyClient.Height-myFont.GetHeight(&graphics));
    SizeF stringSize;
    //����ָ���������ܹ���ʾ���ַ�����������
    gcMeasureString(strW, strW.GetLength(),
      &myFont, SizeF(layoutRect.Width,layoutRect.Height),
      &format, &stringSize, &codepointsFitted, &linesFilled);

    //��ʾһ���ı�
    gcDrawString(strW, strW.GetLength(), &myFont, layoutRect, &format, &brush); 
    //ȥ���Ѿ�������ַ����Ա���һ���ܹ�������ʾ        
    strW=strW.Mid(codepointsFitted);
  }
}
//�����ı���ȥβ��ʽ
void Font_StirngTrimming()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"Times New Roman");
  //�����ı����������
  Font        font(&fontFamily, 25, FontStyleRegular, UnitPixel);

  Pen pen(ColorRed);
  StringFormat stringFormat;
  //������ʾ��Ϣ�����弰��ʽ
  Font        font2(&fontFamily, 14, FontStyleRegular, UnitPixel);
  //�����ı�����ĸ�ʽ
  StringFormat msgFormat;
  msgFormat.SetAlignment(StringAlignmentCenter);
  msgFormat.SetLineAlignment(StringAlignmentCenter);

  //��ʾ��Ϣ�������
  RectF outrect(30, 100, 160, font2.GetHeight(&graphics)*2);

  //ȥβ��ʽ��Character
  stringFormat.SetTrimming(StringTrimmingCharacter);
  //����ı�
  CString text="One two three four five seven eight nine ten";
  MyDrawString(graphics, text, font, solidBrush, 
    RectF(30, 30, 160, 60), stringFormat);

  //�����ı��������
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  //�����ʾ��Ϣ
  MyDrawString(graphics, "Character",font2,solidBrush,outrect, 
    msgFormat);

  //��ͼƽ����ˮƽ����ƽ��160������
  gcTranslateTransform(160,0);

  //����ȥβ��ʽ��Word
  stringFormat.SetTrimming(StringTrimmingWord);
  MyDrawString(graphics, text,font,solidBrush,Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "Word",font2,solidBrush,outrect, msgFormat);


  //����ȥβ��ʽ��EllipsisCharacter
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisCharacter);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisCharacter",font2,solidBrush,outrect, msgFormat);


  //����ȥβ��ʽ��EllipsisWord
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisWord);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisWord",font2,solidBrush,outrect, msgFormat);

  //����ȥβ��ʽ��EllipsisPath
  gcTranslateTransform(160,0);
  stringFormat.SetTrimming(StringTrimmingEllipsisPath);
  MyDrawString(graphics, text,font,solidBrush, Rect(30, 30, 160, 60), stringFormat);
  gcDrawRectangle(&pen, 30, 30, 160, 60);
  MyDrawString(graphics, "EllipsisPath",font2,solidBrush,outrect, msgFormat);

}
//�ı�����ļ��ô���
void Font_TextOutClip()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"����");
  Font        font(&fontFamily, 25, FontStyleRegular, UnitPixel);

  Pen pen(ColorRed);
  StringFormat stringFormat;
  RectF outtext(10, 10, 120, 45);
  //ʹ��Ĭ��ֵ����ı�
  CString text="�ı��ļ���ʾ��";
  MyDrawString(graphics, text,font,solidBrush,outtext, 
    stringFormat);
  //�����ı��������
  gcDrawRectangle(&pen, outtext.X,
    outtext.Y,outtext.Width,outtext.Height);

  //��ˮƽ����ƽ��170�����ظ�
  gcTranslateTransform(120,0);
  RectF out2=outtext;
  //���������ĸ߶ȵ���Ϊ����߶ȵ�������
  out2.Height=font.GetHeight(&graphics)*2;
  MyDrawString(graphics, text,font,solidBrush,out2, stringFormat);
  //�����ı��������
  gcDrawRectangle(&pen, out2.X,
    out2.Y,out2.Width,out2.Height);

  gcTranslateTransform(120,0);
  //������ı�ʱ��ʹ�ü���
  stringFormat.SetFormatFlags(StringFormatFlagsNoClip);
  MyDrawString(graphics, text,font,solidBrush,outtext, stringFormat);

  //�����ı��������
  gcDrawRectangle(&pen, outtext.X,outtext.Y, outtext.Width,outtext.Height);
}
//�����ı��ֲ��������
void Font_MeasureCharacterRanges()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // ���û����ı����������Ļ�ˢ
  SolidBrush TextBrush(ColorRed);
  SolidBrush RegionBrush(_MakeA_RGB(100, ColorBlack));
  Pen        blackPen(ColorBlack);

  // ʹ��������Ȳ�ͬ�ľ�����������ı���ʾ�ⲻͬ�ֲ�����������
  RectF   layoutRect_A(20.0f, 20.0f, 120.0f, 180.0f);
  RectF   layoutRect_B(140.0f, 20.0f, 140.0f, 180.0f);
  RectF   layoutRect_C(280.0f, 20.0f, 180.0f, 180.0f);

  //�����Ҫ������ִ�
  CString ts="��ʱ���º�ʱ�أ�";
  ts+="���ﳤ����δ����";
  ts+="��ʹ���Ƿɽ��ڣ�";
  ts+="���̺������ɽ��";

  // ���ִ�����Ҫ�ֲ��������ı���������ÿ��ʫ�����������
  CharacterRange charRanges[] =
  { 
    CharacterRange(4, 3),
    CharacterRange(12, 3),
    CharacterRange(20, 3), 
    CharacterRange(28, 3)
  };

  // �����ı�ʱʹ�õ����弰��С 
  FontFamily   fontFamily(L"������κ");
  Font  myFont(&fontFamily, 25,  FontStyleItalic, UnitPixel);

  StringFormat strFormat;
  //ָ����Ҫ�ֲ��������������
  Region CharRangeRegions[4];

  //�������ʽ��������Ҫ�ֲ�����������
  strFormat.SetMeasurableCharacterRanges(4, charRanges);

  //�ھ���layoutRect_A�ж��ı����оֲ������������Ľ��������CharRangeRegions��
  CWideCharString tsW(ts);
  gcMeasureCharacterRanges(tsW,tsW.GetLength(),&myFont,layoutRect_A, &strFormat,4,CharRangeRegions);
  //����ı�
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_A, strFormat);
  //�����ı�����߿�
  gcDrawRectangle(&blackPen, layoutRect_A.X,layoutRect_A.Y,layoutRect_A.Width,layoutRect_A.Height);
  short   i;                 
  //�����CharRangeRegions���������
  for ( i = 0; i < 4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

  //�ھ���layoutRect_B�ж��ı����оֲ������������Ľ��������CharRangeRegions��
  gcMeasureCharacterRanges(tsW, tsW.GetLength(), &myFont, layoutRect_B, &strFormat,4,CharRangeRegions);
  //����ı�
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_B, strFormat );
  gcDrawRectangle(&blackPen, 
    layoutRect_B.X,layoutRect_B.Y,layoutRect_B.Width,layoutRect_B.Height);
  //�������
  for ( i = 0; i <4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

  //�ھ���layoutRect_c�ж��ı����оֲ������������Ľ��������CharRangeRegions��
  gcMeasureCharacterRanges(tsW,tsW.GetLength(),&myFont,layoutRect_C, &strFormat,4,CharRangeRegions);
  //����ı�
  MyDrawString(graphics, ts, myFont,TextBrush,layoutRect_C, strFormat);
  gcDrawRectangle(&blackPen,
    layoutRect_C.X,layoutRect_C.Y,layoutRect_C.Width,layoutRect_C.Height);
  //�������
  for ( i = 0; i <4; i++)
  {
    gcFillRegion(&RegionBrush, CharRangeRegions+i);
  }

}
//�ؼ��ı��������
void Font_TextoutDirection()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�����Ҫ������ִ�
  CString ts="��ʱ���º�ʱ�أ�";
  ts+="���ﳤ����δ����";
  ts+="��ʹ���Ƿɽ��ڣ�";
  ts+="���̺������ɽ��";

  //�����������
  FontFamily   fontFamily(L"��Բ");
  Font  myFont(&fontFamily, 20,  FontStyleBold, UnitPixel);
  SolidBrush brush(ColorRed);
  Pen pen(ColorBlack);

  //�����������
  Rect f(10,10,120,myFont.GetHeight(&graphics)*8);
  //����StringFormat����
  StringFormat strFormat;
  //����������ı�
  int flag=StringFormatFlagsDirectionRightToLeft;
  //�ı�����ֱ���������
  flag|=StringFormatFlagsDirectionVertical;
  //���������ʽ��־
  strFormat.SetFormatFlags(flag);

  //�����ı����뷽ʽ
  strFormat.SetAlignment(StringAlignmentCenter);
  strFormat.SetLineAlignment(StringAlignmentCenter);

  //����������估�ı�
  gcDrawRectangle(&pen,f);
  MyDrawString(graphics, ts,myFont,brush,f,strFormat);
}
//�����ı����뷽ʽ
void Font_TextAlignment()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ǰ���ڴ�С��Ϣ����
  Rect   rect(0, 0,
    MyClient.Width, MyClient.Height);
  SolidBrush brush(ColorBlack);
  Pen pen(ColorGray);

  //�Ե�ǰ�������ĵ�Ϊԭ�����������
  gcDrawLine(&pen,
    PointF(rect.Width/2,0), PointF(rect.Width/2,rect.Height));
  gcDrawLine(&pen,
    PointF(0,rect.Height/2), PointF(rect.Width,rect.Height/2));

  //��StringAlignmentö�ٵ����ֲ�ͬ�Ķ��뷽ʽ���ó������Ա���Ա�ֱ�ӷ���
  StringAlignment Align[3]=
  {
    StringAlignmentNear,
    StringAlignmentCenter,
    StringAlignmentFar
  };
  CString strAlign[] ={ "��", "��", "Զ" };

  //�����������
  FontFamily   fontFamily(L"��Բ");
  Font  font(&fontFamily, 12,  FontStyleRegular);
  StringFormat strfmt;

  //�ֱ��ڴ�ֱ��ˮƽ������ʹ�����ֲ�ͬ�Ķ��뷽ʽ
  for (int iVert = 0; iVert < 3; iVert++)
    for (int iHorz = 0; iHorz < 3; iHorz++)
    {
      //���ô�ֱ���뷽ʽ
      strfmt.SetLineAlignment((StringAlignment)Align[iVert]);
      //����ˮƽ���뷽ʽ
      strfmt.SetAlignment((StringAlignment)Align[iHorz]);
      //��������Ϣ���
      CString s;
      s.Format("ˮƽ����:%d\n��ֱ����:%d", strAlign[iVert],strAlign[iHorz]);

      //�ڵ�ǰ�����а������õĶ��뷽ʽ�����ı�
      MyDrawString(graphics, s,font,brush,rect, strfmt);
    }
}

void Font_TextAlignment2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ǰ���ڴ�С��Ϣ����
  Rect   rect(0, 0, MyClient.Width, MyClient.Height);
  SolidBrush brush(ColorBlack);
  Pen pen(ColorGray);

  //�Ե�ǰ�������ĵ�Ϊԭ�����������
  gcDrawLine(&pen,
    PointF(rect.Width/2,0), PointF(rect.Width/2,rect.Height));
  gcDrawLine(&pen,
    PointF(0,rect.Height/2), PointF(rect.Width,rect.Height/2));

  //��StringAlignmentö�ٵ����ֲ�ͬ�Ķ��뷽ʽ���ó������Ա���Ա�ֱ�ӷ���
  StringAlignment Align[3]=
  {
    StringAlignmentNear,
    StringAlignmentCenter,
    StringAlignmentFar
  };
  CString strAlign[]={ "��", "��", "Զ" };

  //�����������
  FontFamily   fontFamily(L"��Բ");
  Font  font(&fontFamily, 12,  FontStyleRegular);
  StringFormat strfmt;

  //�ֱ��ڴ�ֱ��ˮƽ������ʹ�����ֲ�ͬ�Ķ��뷽ʽ
  for (int iVert = 0; iVert < 3; iVert+=2)
    for (int iHorz = 0; iHorz < 3; iHorz+=2)
    {
      //���ô�ֱ���뷽ʽ
      strfmt.SetLineAlignment((StringAlignment)Align[iVert]);
      //����ˮƽ���뷽ʽ
      strfmt.SetAlignment((StringAlignment)Align[iHorz]);
      //��������Ϣ���
      CString s;
      s.Format("ˮƽ����:%d\n��ֱ����:%d", strAlign[iVert],strAlign[iHorz]);

      //�ڵ�ǰ�����а������õĶ��뷽ʽ�����ı�
      MyDrawString(graphics, s,font,brush, PointF(MyClient.Width/2,MyClient.Height/2),strfmt);
    }
}
//ʹ���Ʊ�λ
void Font_TextoutUsingTabs()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  StringFormat stringFormat;
  //����ı�ʹ�õ�����
  FontFamily   fontFamily(L"Arial");
  Font         font(&fontFamily, 30, FontStyleBold, UnitPixel);
  SolidBrush solidBrush(ColorBlack);

  //��ʹ���Ʊ�λ��Ϣ�����
  CString text = "A\tB\tC\t\t\tD";
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);

  //�����Ʊ�λ��Ϣ1
  float  tabs[]={40.0f, 30.0f,40};
  stringFormat.SetTabStops(0.0f, sizeof(tabs)/sizeof(float), tabs);

  //ʹ���Ʊ�λ��Ϣ�Ľ����ı����
  gcTranslateTransform(0,30);
  text="A\tB\tC\tD";
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);

  //�����Ʊ�λ��Ϣ2
  float tabs2[] = {60.0f, 60.0f, 60};
  stringFormat.SetTabStops(0.0f, 3, tabs2);
  gcTranslateTransform(0,30);
  MyDrawString(graphics, text, font, solidBrush, PointF(0,10), stringFormat);

  //��ȡ�Ʊ���Ϣ  
  float  firstTabOffset = 0;
  float tabStops[3];
  stringFormat.GetTabStops(3, &firstTabOffset, tabStops);

  //����Ʊ�λ
  for(int j = 0; j <3; ++j)
    tabStops[j]=0;
  stringFormat.SetTabStops(0.0f, 3, tabStops);

  //������Ʊ�λ��Ϣ������ı�
  gcTranslateTransform(0,30);
  MyDrawString(graphics, text, font,  solidBrush, PointF(0,10), stringFormat);
}

//������Ʒ��Ϣ�ṹ
struct SListItem
{
  public: CString name;//��Ʒ��
  public: float price;//����
  public: float  num;//����
  //���绯
  public: SListItem(CString p1, float p2,float p3) 
  {
    name = p1;
    price=p2;
    num = p3;    
  }
};
void Font_UsingTabs()
{
  //��ʼ����Ʒ��Ϣ
  SListItem items[5]=
  {
    SListItem(L"MP3���Ż�",1228,10),
    SListItem(L"�ʼǱ�����",17200,1),
    SListItem(L"�����ӡ��",3200,5),
    SListItem(L"��ī��ӡ��",620,1),
    SListItem(L"�������",4800,5)
  };

  //�����ͷ
  CString title;
  title.Format("\n%s\t%s\t%s\t%s\n","��Ʒ��","����","����","�ܼ�");
  //��ʽ��ÿ����Ʒ��Ӧ���ַ���
  CString iteminfo[5];
  for (int i=0;i<5;i++)
  {
    //�����ܼ�
    float total=items[i].price*items[i].num;
    //���������ʽ
    CString s;
    s.Format("%d\t%.2f\t%.2f\t%.2f\n",
      items[i].name,items[i].price,items[i].num,total);
    iteminfo[i]=s;
  }

  //�ڱ��ÿһ�е����ݺϲ�
  CString xxx;;
  for (i=0;i<5;i++)
    xxx+=iteminfo[i];
  //��ͷ+����
  title+=xxx;

  //�ڵ�ǰ������������
  RectF  rect(0.0f,0.0f,
    MyClient.Width,MyClient.Height);

  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush   solidBrush(ColorBlack);
  //�����������
  FontFamily   fontFamily(L"����");
  Font         font(&fontFamily,
    16, FontStyleRegular, UnitPixel);

  StringFormat stringFormat;
  //�����Ʊ�λ��Ϣ
  float   tabs[] = {120.0f, 120.0f,60.0f};
  stringFormat.SetTabStops(0.0f, sizeof(tabs)/sizeof(float), tabs);
  //������
  MyDrawString(graphics, title,font, solidBrush,rect, stringFormat);
}

void Font_TextoutHotkeyPrefix()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush  solidBrush(ColorBlack); 
  FontFamily  fontFamily(L"����");
  Font   font(&fontFamily, 20, FontStyleRegular, UnitPixel);
  Pen pen(ColorRed);

  StringFormat stringFormat;
  //��ʾ��ݼ��ַ���������"&"��
  stringFormat.SetHotkeyPrefix(HotkeyPrefixNone);
  MyDrawString(graphics, "������(&T )\n״̬��(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  //���Ʊ߿�
  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

  //����""��Ϊ�»���
  stringFormat.SetHotkeyPrefix(HotkeyPrefixShow);
  gcTranslateTransform(0,font.GetHeight(&graphics)*3);
  MyDrawString(graphics, "������(&T )\n״̬��(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  //���Ʊ߿�
  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

  //����""��
  gcTranslateTransform(0,font.GetHeight(&graphics)*3);
  stringFormat.SetHotkeyPrefix(HotkeyPrefixHide);
  MyDrawString(graphics, "������(&T )\n״̬��(&S )", font, solidBrush,
    RectF(30, 30, 160, font.GetHeight(&graphics)*2),stringFormat);

  gcDrawRectangle(&pen, Rect(30, 30, 160,font.GetHeight(&graphics)*2));

}

void Font_TextoutShadow()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //���ı�ʹ��ȥ��ݵı�Ե����
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"���Ŀ���");
  Font        font(&fontFamily, 100, FontStyleBold, UnitPixel);

  //�ı������
  RectF textout(font.GetHeight(&graphics),
    MyClient.Height/2,MyClient.Width,MyClient.Height);

  //��������ͬ��λ�û����ı����γ���Ӱ
  //solidBrush��ɫ��͸����Ϊ100������
  SolidBrush solidBrush(_MakeA_RGB(100,ColorBlack));
  SolidBrush redBrush(ColorRed);
  MyDrawString(graphics, "��Ӱ��",font, solidBrush,
    PointF(27.0f, 27.0f));
  MyDrawString(graphics, "��Ӱ��",font, redBrush,
    PointF(12.0f, 20.0f));

  //��һ����Ӱ�֣���ӰΪ����
  //����Ӱ�߻�ˢ
  HatchBrush brush_tmp(
    HatchStyleDarkDownwardDiagonal,ColorBlack, ColorWhite);

  int reptime=40;
  //�Ȼ�����
  for(int i=0;i<reptime;i++)
    MyDrawString(graphics, "��Ӱ��",font,brush_tmp,
      PointF(textout.X+i+2,textout.Y+i+2));

  //�ٻ�ǰ��
  MyDrawString(graphics, "��Ӱ��",font, redBrush, PointF(textout.X,textout.Y));
}

void Font_TextoutHashline()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //���ı�ʹ��ȥ��ݵı�Ե����
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"���Ŀ���");
  Font        font(&fontFamily, 100, FontStyleBold, UnitPixel);

  //�ı������
  RectF textout(font.GetHeight(&graphics),0,MyClient.Width,MyClient.Height);
  //��ˢ������ʾ��
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));
  //����һ��Ӱ�߻�ˢ
  HatchBrush brush_tmp(HatchStyle75Percent,ColorBlack, ColorWhite);

  //�����ı������ʽ��ˮƽ���󡢴�ֱ����
  StringFormat format;
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);

  //��ʾ��ˢ����
  gcFillRectangle(&brush_tmp,brushdemo);
  //ʹ�û�ˢ�����ı�
  MyDrawString(graphics, "Ӱ����",font,brush_tmp,textout,format);

}

void Font_TextoutTexture()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //���ı�ʹ��ȥ��ݵı�Ե����
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"���Ŀ���");
  Font        font(&fontFamily, 70, FontStyleBold, UnitPixel);

  //�ı������
  RectF textout(0,0,MyClient.Width,MyClient.Height);
  //��ˢ������ʾ��
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));

  //װ������ͼƬ
  Bitmap image(L"TEXTURE3.bmp");
  //��������ˢ
  TextureBrush brush_tmp(&image);

  //�����ı������ʽ��ˮƽ����ֱ����
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  format.SetLineAlignment(StringAlignmentCenter);

  //��ʾ��ˢ����
  gcFillRectangle(&brush_tmp,brushdemo);
  //ʹ�û�ˢ�����ı�
  MyDrawString(graphics, "������",font,brush_tmp,textout,format);
}

void Font_TextoutGradient()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //���ı�ʹ��ȥ��ݵı�Ե����
  gcSetTextRenderingHint(TextRenderingHintAntiAlias);

  FontFamily  fontFamily(L"���Ŀ���");
  Font        font(&fontFamily, 70, FontStyleBold, UnitPixel);

  RectF textout(0,0,MyClient.Width,MyClient.Height);
  //��ˢ������ʾ��
  RectF brushdemo(0,0,font.GetHeight(&graphics),font.GetHeight(&graphics));
  //����һ����ɫ���仭ˢ
  Color colors[] =
  {
    ColorRed,   // ��ɫ
    ColorGreen,//����ɫΪ��ɫ
    ColorBlue  // ��ɫ
  };
  float positions[] = 
  {
    0.0f,   // �ɺ�ɫ��
    0.3f,   // ��ɫʼ�ڻ�ˢ���ȵ�����֮һ
    1.0f // ����ɫֹ
  }; 

  //����һ���Ӻ�ɫ����ɫ�Ľ��仭ˢ
  gcGradientBrush brush_tmp(
    Point(0, 0), 
    Point(100, font.GetHeight(&graphics)), 
    ColorWhite,         // ��
    ColorWhite);  // ��

  //���ý��仭ˢ�Ķ�ɫ������Ϣ
  //ColorBlend clrBlend(3);
  //clrBlend.Colors=colors;
  //clrBlend.Positions=positions;
  brush_tmp.SetInterpolationColors(colors, positions, 3);

  //�����ı������ʽ��ˮƽ���󡢴�ֱ����
  StringFormat format;
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);
  //��ʾ��ˢ����
  gcFillRectangle(&brush_tmp,brushdemo);
  //ʹ�û�ˢ�����ı�
  MyDrawString(graphics, "������",font,brush_tmp,textout,format);
}
//����·��
void Path_Construct()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //���幹��·���ĵ�����
  PointF data[]=
{
  PointF(40,140),
  PointF(275,200),
  PointF(105,225),
  PointF(190,300),
  PointF(50,350),
  PointF(20,180)
};

  //���ö��������ͽ�ÿ�㶼�����ֱ�ߵĶ˵�
  BYTE typeline[]=
{
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine
};

  //����·��
  GraphicsPath tmp1(data,typeline,6);
  //���·��
  gcFillPath(&SolidBrush(ColorRed),&tmp1);

  //���ƶ���·���Ķ˵�
  for(int i=0;i<6;i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      RectF(data[i].X-5,data[i].Y-5,10,10));

  //��ͼƽ������
  gcTranslateTransform(360,0);

  //����·���Ķ˵�����
  BYTE type[]=
{
  (BYTE)PathPointTypeStart,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeBezier,
  (BYTE)PathPointTypeLine,
  (BYTE)PathPointTypeLine
};
  //���첢���·��
  GraphicsPath tmp(data,type,6);
  gcFillPath(&SolidBrush(ColorRed),&tmp);

  //���ƶ���·���Ķ˵�
  for(i=0;i<6;i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      RectF(data[i].X-5,data[i].Y-5,10,10));
}

void Path_AddLines()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  //����·������
  GraphicsPath path;

  Rect rect1(20, 20, 100, 200);
  Rect rect2(40, 40, 100, 200);
  //������λ���
  path.AddArc(rect1, 0.0f, 180.0f); 
  path.AddArc(rect2, 0.0f, 180.0f); 
  gcDrawPath(&pen, &path);
}
//���ͼ��
void Path_CloseFigure()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  //����·������
  GraphicsPath path;

  Rect rect1(20, 20, 100, 200);
  Rect rect2(40, 40, 100, 200);
  GraphicsPath path2;
  path2.AddArc(rect1, 0.0f, 180.0f);  
  //��ջ���1
  path2.CloseFigure();
  path2.AddArc(rect2, 0.0f, 180.0f);  
  //��ջ���2
  path2.CloseFigure();
  gcDrawPath(&pen, &path2);
}
//·�������
void Path_FillPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  GraphicsPath path1;
  GraphicsPath path;
  Pen pen(_MakeA_RGB(128, ColorBlue), 5);
  SolidBrush brush(ColorRed);

  //�������ֱ�ߣ����ŵ�ͼ��
  path1.AddLine(10,10,10,50);
  path1.AddLine(10,50,50,50);
  path1.AddLine(50,50,50,10);
  path1.StartFigure();

  //���һ���պϵ�ͼ��
  RectF rect(110,10,40,40);
  //��ʼ�ڶ�����·����ͼ��׷��
  path1.AddRectangle(rect);
  //���ơ����·��
  gcDrawPath(&pen, &path1);
  gcFillPath(&brush, &path1);

  //��ӿ��ŵĻ���
  path.AddArc(0, 0, 150, 120, 30, 120);
  //��ӷ�յ���Բ
  path.AddEllipse(50, 50, 50, 100);

  //ʹ��Ĭ�ϵ���䷽ʽ���·��
  gcFillPath(&brush, &path);
  gcDrawPath(&pen, &path);
}

void Path_AddSubPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,2);
  Rect rect(0, 0, 100, 50);
  GraphicsPath path;

  //��·���а���ֱ���������λ���
  path.AddArc(0, 0, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 30, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 60, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 90, 100, 50, 0.0f, 180.0f);
  path.AddArc(0, 120, 100, 50, 0.0f, 180.0f);
  gcDrawPath(&pen, &path);

  //��ȡ·����ռ����
  RectF rect2; path.GetBounds(&rect2);

  //��ͼƽ������
  gcTranslateTransform(rect2.Width+10,0);

  GraphicsPath path2;
  //�����һ����·��
  path2.AddArc(0, 0, 100, 50, 0.0f, 180.0f);
  path2.AddArc(0, 30, 100, 50, 0.0f, 180.0f);
  path2.AddArc(0, 60, 100, 50, 0.0f, 180.0f);

  //�ڲ���յ�ǰ��·��������¿�ʼ�ڶ�����·���Ļ���
  path2.StartFigure();
  //��ӻ���
  path2.AddArc(0, 90, 100, 50, 0.0f, 180.0f);

  //�ڷ�յ�ǰ��·��������¿�ʼ��������·���Ļ���
  path2.CloseFigure();
  path2.AddArc(0, 120, 100, 50, 0.0f, 180.0f);

  //������·��
  gcDrawPath(&pen, &path2);
}

void Path_GetSubPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,3);
  GraphicsPath path;
  // ��·��1
  path.AddRectangle(Rect(20, 20, 60, 30));   
  // ��·��2
  path.AddLine(100, 20, 160, 50);            
  path.AddArc(180, 20, 60, 30, 0.0f, 180.0f);
  // ��·��3
  path.AddRectangle(Rect(260, 20, 60, 30)); 
  // ��·��4
  path.AddLine(340, 20, 400, 50);           
  path.AddArc(340, 20, 60, 30, 0.0f, 180.0f);
  path.CloseFigure();
  // ��·��5
  path.AddRectangle(Rect(420, 20, 60, 30));  
  //����·��
  gcDrawPath(&bluePen, &path);

  // ����·����������iterator
  GraphicsPathIterator iterator(&path);
  GraphicsPath subpath;
  int isClosed=false;
  int count=0;
  //������������NextSubpath,��ȡ��·��
  //��·���ĵ�ǰλ���ƶ���1
  count =iterator.NextSubpath(&subpath, &isClosed);
  //��·���ĵ�ǰλ���ƶ���2
  count = iterator.NextSubpath(&subpath, &isClosed);

  //�ڵڶ��л�����ȡ����·��
  gcTranslateTransform(0,50);   
  gcDrawPath(&bluePen, &subpath);

  //��λ���е�λ���ƶ���Ϣ
  iterator.Rewind();
  count = iterator.NextSubpath(&subpath, &isClosed);
  //�ڵ����л����³�ȡ����·��
  gcTranslateTransform(0,50);   
  Pen redPen(ColorRed,3);
  gcDrawPath(&redPen, &subpath);

  //������������NextSubpath���Ի�ȡ��·���Ķ������Ϣ
  int start=0;
  int end=0;
  CString msg;
  //��λ���е�λ���ƶ���Ϣ
  iterator.Rewind();
  count = iterator.NextSubpath(&start, &end, &isClosed);
  //����λ��������Ϣ
  msg.Format(_T("��һ���ƶ���·���Ķ������ֹλ��Ϊ%d\t��\t%d\n"),start,end);
  count = iterator.NextSubpath(&start, &end, &isClosed);
  CString s;
  s.Format(_T("�ڶ����ƶ���·���Ķ������ֹλ��Ϊ%d\t��\t%d\n"),start,end);
  msg+=s;
  //������·���Ķ����λ��������Ϣ
  FontFamily  fontFamily(L"����");
  Font font(&fontFamily,12);
  gcTranslateTransform(0,60);
  MyDrawString(graphics, msg,font,SolidBrush(ColorBlack), PointF(0,0));
}

void Path_GetPoints()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����·������
  GraphicsPath path;
  //���ֱ��
  path.AddLine(20, 100, 150, 120);
  //��Ӿ���
  path.AddRectangle(Rect(40, 30, 20, 60));
  //�����Բ
  path.AddEllipse(Rect(70, 30, 100, 40));
  //������ߡ�
  Point points[] =
  {
    Point(300, 40),
    Point(350, 60),
    Point(300, 80),
    Point(300, 100),
    Point(350, 150)
  };
  path.AddCurve(points,5);

  //����·��
  Pen pen(ColorBlue);
  gcDrawPath(&pen, &path);

  // ��ȡ����·���ĵ�����
  int count =path.GetPointCount();
  //���Ƶ㶨��·���ĵ�
  SolidBrush brush(ColorRed);
  PointF *points0 = new PointF[count];
  path.GetPathPoints(points, count);
  for(int j = 0; j < count; ++j)
  {
    gcFillEllipse(
      &brush, 
      points0[j].X - 3.0f, 
      points0[j].Y - 3.0f,
      6.0f,
      6.0f);
  }
  delete points0;
  /*����һһ�ֱ�������ķ�ʽ
  foreach( PointF x in path.PathPoints)
    gcFillEllipse(
      brush, 
      x.X - 3.0f, 
      x.Y - 3.0f,
      6.0f,
      6.0f);
  */
}

void Path_PathData()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //���ߵĶ����
  Point points[] =
{
  Point(40, 60), 
  Point(50, 70), 
  Point(30, 90)
};

  Pen pen(ColorRed, 2);
  GraphicsPath path;
  //����
  path.AddArc(175, 50, 50, 50, 0.0f, -180.0f);
  //ֱ��
  path.AddLine(100, 0, 250, 20);
  path.StartFigure();
  //ֱ��
  path.AddLine(50, 20, 5, 90);
  //����
  path.AddCurve(points, 3);
  path.AddLine(50, 150, 150, 180);
  path.CloseAllFigures();
  //����·��
  gcDrawPath(&pen, &path);
  //��ȡPathData����ֵ
  PathData pathData; 
  path.GetPathData(&pathData);
  SolidBrush brush(ColorRed);

  //���ƶ����
  for (int i=0; i<pathData.Count; i++)
    gcFillEllipse(&SolidBrush(ColorBlack),
      pathData.Points[i].X - 5.0f,pathData.Points[i].Y - 5.0f,10.0f,10.0f);
  //���ÿһ��·������������
  FontFamily  fontFamily(L"����");
  Font font(&fontFamily,12);
  RectF rect;
  path.GetBounds(&rect);
  gcTranslateTransform(rect.Width+10,0);
  for (i=0; i<pathData.Count; i++)
  {
    CString msg;
    msg.Format("��%d�������Ϊ:\t%d", i+1, pathData.Types[i]);
    MyDrawString(graphics, msg,
      font,SolidBrush(ColorBlack), PointF(0,(i+1)*font.GetHeight(&graphics)));
  }
}

void Path_ViewPointFLAG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.8f,0.8f);


  SolidBrush brush(ColorRed);
  //����һ�������ĸ���·����·��
  GraphicsPath path;
  path.AddRectangle(Rect(20, 20, 60, 30));
  path.AddLine(100, 20, 160, 50);
  path.AddArc(180, 20, 60, 30, 0, 180);
  path.AddRectangle(Rect(260, 20, 60, 30));
  path.AddLine(340, 20, 400, 50);
  path.AddArc(340, 20, 60, 30, 0, 180);
  path.CloseAllFigures();
  //���·��
  gcFillPath(&brush, &path);

  //��·���ı����Ϣ����GraphicsPathIterator����
  GraphicsPathIterator iterator(&path);

  //��ȡ��ǵ���Ϣ
  int start=0;
  int end=0;
  int count=0;
  //��ȡ��һ���������
  count = iterator.NextMarker(&start, &end);
  //Ϊ�����ꡢ���ͷ���ռ�
  PointF *points = new PointF[count];
  BYTE *types = new BYTE[count];
  //��iterator������Ϣ
  iterator.CopyData(points, types, start, end);
  //���ݸ��Ƶ���Ϣ�´���һ��·��
  GraphicsPath pathnew(points, types, count);

  //����λ�û���·���Ķ����
  gcTranslateTransform(0,60);
  for(int j = 0; j < count; ++j)
    gcFillEllipse(&SolidBrush(ColorBlack),points[j].X - 2.0f,
      points[j].Y - 2.0f,  4.0f,4.0f);

  //����´�����·��
  gcTranslateTransform(0,60);
  gcFillPath(&brush, &pathnew);
  gcTranslateTransform(0,60);

  //������еı�ǵ���Ϣ
  CString msg;
  for(j = 0; j < count; ++j)
  {
    CString s;
    s.Format("��%s����Ϣ:X=%.2f\tY=%.2f\t����=%d\n",
      j,points[j].X,points[j].Y,types[j]);
    msg+=s;
  }

  //�����ַ���
  FontFamily  fontFamily(L"����");
  Font   font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  MyDrawString(graphics, msg, font,SolidBrush(ColorBlack), PointF(0,0));

  delete types;
  delete points;
}

void Path_SubPathRange()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorRed);
  //����һ�����������·����·��
  GraphicsPath path;
  // *************path.SetMarkers();**************
  //��ӵ�һ����·��
  path.AddRectangle(Rect(20, 20, 60, 30));

  //��ӵڶ�����·��
  path.AddLine(100, 20, 160, 50);
  //�ڶ�����ǽ��ڶ�����·��������ض�
  path.SetMarker(); 
  path.AddArc(180, 20, 60, 30, 0, 180);

  //��ӵ�������·��
  path.AddRectangle(Rect(260, 20, 60, 30));
  //���������
  path.SetMarker(); 

  //��ӵ��ĸ���·��
  path.AddLine(340, 20, 400, 50);
  path.AddArc(340, 20, 60, 30, 0, 180);

  //���·��
  gcFillPath(&brush,&path);

  //��·���ı����Ϣ����GraphicsPathIterator����
  GraphicsPathIterator iterator(&path);
  int d=iterator.GetSubpathCount();
  //��ȡ��ǵ���Ϣ
  int start;
  int end;
  int count;
  //�Ƶ��ڶ������
  count = iterator.NextMarker(&start, &end);
  //�Ƶ����������
  count = iterator.NextMarker(&start, &end);
  //Ϊ�����ꡢ���ͷ���ռ�
  PointF *points = new PointF[count];
  BYTE *types = new BYTE[count];

  //��iterator������Ϣ
  iterator.CopyData(points, types, start, end);
  //���ݸ��Ƶ���Ϣ�´���һ��·��
  GraphicsPath pathnew(points, types, count);

  //����λ�û��Ʊ�ǵ�
  gcTranslateTransform(0,60);
  for(int j = 0; j < count; ++j)
    gcFillEllipse(&SolidBrush(ColorBlack),
      points[j].X - 2.0f,points[j].Y - 2.0f,
      4.0f,
      4.0f);

  //����´�����·��
  gcTranslateTransform(0,60);
  gcFillPath(&brush,&pathnew);

  //������еı�ǵ���Ϣ
  CString msg;
  for( j = 0; j < count; ++j)
  {
    CString s;
    s.Format(_T("��%d�����Ϣ:X=%.2f\tY=%.2f \t���ͣ�%d\n"),
      j,points[j].X,points[j].Y,types[j]);
    msg+=s;
  }

  FontFamily  fontFamily(L"����");
  Font        font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  //�����ַ���
  gcTranslateTransform(0,60);
  MyDrawString(graphics, msg, font,SolidBrush(ColorBlack), PointF(0,0));

  delete points;
  delete types;
}


void Path_Flatten()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //���߶����
  Point pts[] =
  {
    Point(20,50), 
    Point(60,70), 
    Point(80,10), 
    Point(120,50)
  };

  GraphicsPath path;
  path.AddCurve(pts, 4);
  path.AddEllipse(160, 10, 150, 80);
  path.AddBezier(20, 100, 20, 150, 50, 80, 100, 60);

  //����Ĭ�ϵķ�ʽ����·��
  Pen pen(ColorBlack,3);
  gcDrawPath(&pen, &path);
  //��ȡ·������Ϣ
  PathData pathData;
  path.GetPathData(&pathData);

  // ����·�������
  SolidBrush brush(ColorRed);
  for(int j = 0; j <pathData.Count; ++j)
  {
    gcFillEllipse(&brush, pathData.Points[j].X - 3.0f,
      pathData.Points[j].Y - 3.0f,6.0f, 6.0f);
  }

  //��ͼƽ������100����λ 
  gcTranslateTransform(0,120);
  //��������ת  
  path.Flatten(&Matrix(), 14.0f);
  pen.SetColor(ColorGreen);
  //����ת�����·��  
  gcDrawPath(&pen, &path);
}


void Path_Warp()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  // ����һ��ȫ����������ɵ�·��
  PointF points[] =
  {
    PointF(20.0f, 60.0f),
    PointF(30.0f, 90.0f),
    PointF(15.0f, 110.0f),
    PointF(15.0f, 145.0f),
    PointF(55.0f, 145.0f),
    PointF(55.0f, 110.0f),
    PointF(40.0f, 90.0f),
    PointF(50.0f, 60.0f)
  };

  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(PointF));
  path.CloseFigure();

  // ����Ť��ǰ��·��
  Pen bluePen(ColorBlue,3);
  gcDrawPath(&bluePen, &path);

  // �������·����Դ����
  RectF srcRect(10.0f, 50.0f, 50.0f, 100.0f);
  //�������ӳ���Ŀ���
  PointF destPts[] =
  {
    PointF(220.0f, 10.0f),
    PointF(280.0f, 10.0f),
    PointF(100.0f, 150.0f),
    PointF(300.0f, 150.0f)
  };

  //Ť������
  path.Warp(destPts, 4, srcRect);
  // ����Ť�����·��
  gcDrawPath(&bluePen, &path);

  // ����Դ���κ�Ŀ��Ķ����
  Pen blackPen(ColorBlack,1);
  gcDrawRectangle(&blackPen, 
    srcRect.X,srcRect.Y,srcRect.Width,srcRect.Height);
  gcDrawLine(&blackPen, destPts[0], destPts[1]);
  gcDrawLine(&blackPen, destPts[0], destPts[2]);
  gcDrawLine(&blackPen, destPts[1], destPts[3]);
  gcDrawLine(&blackPen, destPts[2], destPts[3]);
}

void Path_Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  GraphicsPath path;
  //���һ��������·��   
  path.AddRectangle(Rect(40, 10, 200, 50));
  //��������任ǰ��·��
  gcDrawPath(&Pen(ColorBlue,4), &path);

  //��ת·��
  Matrix matrix;
  matrix.Rotate(15.0f);
  path.Transform(&matrix);
  //���ƽ�������任���·��
  gcDrawPath(&Pen(ColorRed,4), &path);
}

void Path_Widen()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,1);
  Pen greenPen(ColorGreen, 10);
  //��������
  PointF points[] =
  {
    PointF(10.0f, 10.0f),
    PointF(130.0f, 90.0f),
    PointF(140.0f, 60.0f),
    PointF(60.0f, 90.0f)
  };

  GraphicsPath path;
  //�����������
  path.AddClosedCurve(points, 4);

  //����������ͬ��·�����������ؿ���
  GraphicsPath *path2=path.Clone();
  GraphicsPath *path3=path.Clone();
  GraphicsPath *path4=path.Clone();

  //ʹ�ÿ��Ϊ10�Ļ��ʻ���·��
  gcDrawPath(&Pen(ColorBlue,10), &path);
  RectF rect; path.GetBounds(&rect);

  //ʹ����ɫ�����ؿ�·������
  path2->Widen(&greenPen);
  //��ͼƽ�����ƣ������ؿ��·��
  gcTranslateTransform(rect.Width+10,0);
  gcDrawPath(&bluePen, path2);

  //����
  gcTranslateTransform(-rect.Width,rect.Height);
  //ʹ�ü���߷�����ɫ�����ؿ�·��
  greenPen.SetDashStyle(DashStyleDash);
  path3->Widen(&greenPen);
  //���·��
  gcFillPath(&SolidBrush(ColorRed), path3);

  //�Ի��ʽ������ű任 
  Pen pentmp(ColorBlue);
  pentmp.ScaleTransform(3.0f, 20.0f);
  path4->Widen(&pentmp);
  //��ͼƽ������
  gcTranslateTransform(rect.Width,0);
  gcDrawPath(&bluePen, path4);

  delete path2;
  delete path3;
  delete path4;
}

void Path_WidenDemo()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen bluePen(ColorBlue,1);
  Pen greenPen(ColorGreen, 20);
  GraphicsPath path;
  //���һ��ֱ��
  path.AddLine(10,20,100,20);
  GraphicsPath *path2=path.Clone();

  PathData pathData;
  PathData pathData2;

  path.GetPathData(&pathData);
  int num1=path.GetPointCount();

  //����·���Ķ������Ϣ
  CString msg;
  for(int i=0;i<num1;i++)
  {
    CString s;
    s.Format("��%d������ֱ�Ϊ��X=%.2f\tY=%.2f\n",
      i,pathData.Points[i].X,pathData.Points[i].Y);
    msg+=s;
  }
  //����·��
  gcDrawPath(&Pen(ColorBlue), &path);

  //ʹ����ɫ�����ؿ�·������
  path2->Widen(&greenPen);
  path2->GetPathData(&pathData2);
  int num2=path2->GetPointCount();

  //��ͼƽ�����ƣ������ؿ��·��
  RectF rect; path.GetBounds(&rect);
  gcTranslateTransform(rect.Width*2,0);
  gcDrawPath(&bluePen, path2);
  gcResetTransform();
  //�����ؿ��·���Ķ������Ϣ
  CString msg2;
  for(i=0;i<num2;i++)
  {
    CString s;
    s.Format("�ؿ���·����%d������ֱ�Ϊ��X=%.2f\tY=%.2f\n",
      i,pathData2.Points[i].X,pathData2.Points[i].Y);
    msg2+=s;
  }

  //���·���ؿ�ǰ��Ķ��������λ����Ϣ
  FontFamily  fontFamily(L"����");
  Font  font(&fontFamily, 12, FontStyleRegular, UnitPixel);
  gcTranslateTransform(0,120);
  MyDrawString(graphics, msg,font,SolidBrush(ColorBlack), PointF(0,0));  
  gcTranslateTransform(0,font.GetHeight(&graphics)*(num1+1));
  MyDrawString(graphics, msg2,font,SolidBrush(ColorBlack), PointF(0,0));  

}

void Region_FromPath()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //�趨�ı��������
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  FontFamily   fontFamily(L"����");
  //����·������
  GraphicsPath path;

  //��������׷���ı�
  path.AddString(L"��������",4,&fontFamily,
    (int)FontStyleRegular,100.0f, Point(0, 0), &StringFormat() );

  Pen pen(ColorRed);
  //����·��
  gcDrawPath(&pen, &path);
  gcTranslateTransform(0,80);
  //��·���й�������
  Region region(&path);
  //�������  
  gcFillRegion(&SolidBrush(ColorRed),&region);
}

void Region_Calculate()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily  fontFamily(L"����");
  Font   font(&fontFamily, 16, FontStyleRegular, UnitPixel);

  SolidBrush solidBrush2(ColorRed);
  SolidBrush solidBrush(ColorGreen);
  Pen pen(ColorCyan);

  //�������εĶ˵�����
  Point points[]=
  {
    Point(75, 0), 
    Point(100, 50), 
    Point(150, 50), 
    Point(112, 75),
    Point(150, 150), 
    Point(75, 120), 
    Point(10, 150), 
    Point(37, 75), 
    Point(0, 50), 
    Point(50, 50),
    Point(75, 0)
  };

  //����·�����������
  GraphicsPath path;
  path.AddLines(points, sizeof(points)/sizeof(Point));

  //��С·��
  Matrix matrix;
  matrix.Scale(0.5f,0.5f);
  path.Transform(&matrix);

  //��ȡ·����ռ�ľ�������
  RectF tmp; path.GetBounds(&tmp);

  //��������2
  RectF rect(0,10,tmp.Width,50); 
  //�����ı���Ϣ�������
  RectF textout(0,tmp.Height,tmp.Width,font.GetHeight(&graphics)*2);

  //�����ı������ʽ��ˮƽ���С���ֱ����
  StringFormat format;
  format.SetAlignment(StringAlignmentCenter);
  format.SetLineAlignment(StringAlignmentFar);

  //���·�� 
  gcFillPath(&solidBrush2,&path);

  gcTranslateTransform(tmp.Width+10,0);
  //������ 
  gcFillRectangle(&solidBrush,rect);

  //��·���д�������
  Region region(&path); 

  //�󽻼�
  region.Intersect(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //��佻��
  gcFillRegion(&solidBrush2, &region);
  MyDrawString(graphics, "����",font,SolidBrush(ColorBlack),textout,format);  
  //����·����ռ�ľ�������
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //�󲢼�
  Region region2(&path); 
  region2.Union(rect);
  gcTranslateTransform(tmp.Width+10,0);
  gcFillRegion(&solidBrush2, &region2);
  MyDrawString(graphics, "����",font,solidBrush,textout,format);  
  //����·����ռ�ľ�������
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //�����������������
  gcTranslateTransform(-(tmp.Width+10)*3,tmp.Height+textout.Height);

  //���첢��
  Region region3(&path); 
  region3.Xor(rect);
  //����첢��
  gcFillRegion(&solidBrush2, &region3);
  MyDrawString(graphics, "�첢��",font,SolidBrush(ColorBlack),textout,format);  
  //����·����ռ�ľ�������
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //�󲹼�
  Region region4(&path); 
  region4.Complement(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //��䲹��
  gcFillRegion(&solidBrush2, &region4);
  MyDrawString(graphics, "����",font,SolidBrush(ColorBlack),textout,format);  
  //����·����ռ�ľ�������
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);

  //���ų⼯
  Region region5(&path); 
  region5.Exclude(rect);
  gcTranslateTransform(tmp.Width+10,0);
  //���ų⼯
  gcFillRegion(&solidBrush2, &region5);
  MyDrawString(graphics, "�ų⼯",font,SolidBrush(ColorBlack),textout,format);  
  gcDrawRectangle(&pen,tmp.X,tmp.Y,tmp.Width,tmp.Height);
}

void Region_GetRects()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush solidBrush(ColorRed);
  Pen pen(ColorBlack);
  GraphicsPath path;
  Matrix matrix;

  //��·���д�������
  path.AddEllipse(10, 0, 80, 120);
  Region pathRegion(&path);    
  //�������
  gcFillRegion(&solidBrush, &pathRegion);

  //��ȡ·���ľ�����ɼ���Ϣ����ʹ�ñ任��
  int count = pathRegion.GetRegionScansCount(&matrix);
  RectF *rects = new RectF[count];
  pathRegion.GetRegionScans(&matrix, rects, &count);
  //����ƽ�����ƣ��Ի��ƾ��μ������еľ���
  gcTranslateTransform(90,0); 
  for(int i=0; i<count; i++)
    gcDrawRectangle(&pen, rects[i]);  

  //ʵʩ�任
  matrix.Scale(1.0f,0.75f);
  matrix.Shear(0.2f,0.3f);
  //��ȡ·���ľ�����ɼ���Ϣ��ʹ�ñ任��
  count = pathRegion.GetRegionScansCount(&matrix);
  delete rects;
  rects = new RectF[count];
  pathRegion.GetRegionScans(&matrix, rects, &count);
  //����ƽ�����ƣ��Ի��Ʊ任��ľ��μ�
  gcTranslateTransform(90,0); 
  for(i=0; i<count; i++)
    gcDrawRectangle(&pen, rects[i]);  

  delete rects;
}

void Transform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush mySolidBrush1(ColorBlue);
  SolidBrush mySolidBrush2(_MakeA_RGB(155,ColorChocolate));
  GraphicsPath myGraphicsPath;
  Rect myRect(60, 60, 60, 60);
  //��·������Ӿ���
  myGraphicsPath.AddRectangle(myRect);
  //���·��
  gcFillPath(&mySolidBrush1, &myGraphicsPath);

  // ��������任
  Matrix myPathMatrix;
  //ˮƽ����������������ֱ���򱣳ֲ���
  myPathMatrix.Scale(2, 1);
  myPathMatrix.Rotate(30.0f);
  //��·��ʵ������任
  myGraphicsPath.Transform(&myPathMatrix);

  //���任���·��
  gcFillPath(&mySolidBrush2, &myGraphicsPath);
}

void TranslateTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"nemo.bmp");
  //����ͼƬ����ʾ����
  Rect rect(0,0,110,70);
  for(int i=0;i<10;i++)
  {
    //��ʾͼƬ
    gcDrawImage(&image,rect);
    //��ͼƽ�����ƣ�ÿ����ˮƽ��������110������
    gcTranslateTransform(110,0);
  }
}

void RotateTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"nemo.bmp");

  //��ȡ��ǰ���ڵ����ĵ�
  Rect rect(0,0,MyClient.Width,MyClient.Height);
  PointF center(rect.Width/2,rect.Height/2);

  float offsetX=0;
  float offsetY=0;
  offsetX=center.X-image.GetWidth()/2;
  offsetY=center.Y-image.GetHeight()/2;
  //����ͼƬ��ʾ����:��ͼƬ�����ĵ��봰�ڵ����ĵ�һ��
  RectF picRect(offsetX,offsetY,image.GetWidth(),image.GetHeight());
  PointF Pcenter(picRect.X+picRect.Width/2,
    picRect.Y+picRect.Height/2);

  //��ͼƬ��������תһ��
  for(int i=0;i<361;i+=10)
  {
    // ��ͼƽ����ͼƬ�����ĵ���ת
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    gcRotateTransform(i);
    //�ָ���ͼƽ����ˮƽ�ʹ�ֱ�����ƽ��
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //����ͼƬ����ʱ
    gcDrawImage(&image,picRect);  
    ::sys_sleep(100);
    //���û�ͼƽ������б任
    gcResetTransform();
  }
}

void DrawWatch()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);

  //����·���ĸߺͿ�
  float WIDTH=clientrect.Width/2;
  float HEIGHT =clientrect.Height/2;

  //�����ı�������
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  FontFamily  fontFamily(L"Times New Roman");
  Font   font(&fontFamily, 14, FontStyleBold, UnitPixel);

  //����ͼƽ�������ԭ���Ƶ���������
  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  //�����Ǳ���
  gcFillEllipse(&SolidBrush(ColorBlack),
    HEIGHT/-2,HEIGHT/-2,HEIGHT, HEIGHT);
  //����ı�
  MyDrawString(graphics, "����/Сʱ",font,SolidBrush(ColorGreen),
    PointF(-26,HEIGHT/-4-font.GetHeight(&graphics)));

  //���ƿ̶ȱ��
  gcRotateTransform(225);
  for(int x = 0; x<55;x++)
  {
    gcFillRectangle(&SolidBrush(ColorGreen),
      Rect(-2, -(HEIGHT/2-2), 3, 15));
    gcRotateTransform(5);
  }

  //���û�ͼƽ�������任
  gcResetTransform();

  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  gcRotateTransform(225);
  int sp = 0;
  CString tmp;
  //���ƿ̶�ֵ(������)
  for(x = 0; x<7;x++)
  {
    tmp.Format("%d",sp);

    //���ƺ�ɫ�̶�
    gcFillRectangle(&SolidBrush(ColorRed),Rect(-3,(HEIGHT/2 -2)* -1,6,25));
    //������ֵ
    MyDrawString(graphics, tmp,font,SolidBrush(ColorGreen), PointF(tmp.GetLength()*(-6),
      (HEIGHT/-2) + 25));
    //��ת45��
    gcRotateTransform(45);
    sp += 20;
  }

  //���û�ͼƽ�������任
  gcResetTransform();
  gcTranslateTransform(WIDTH /2,HEIGHT/2);

  //����ָ����30����/�������
  float angle= 30*2.25f+ 225;
  gcRotateTransform((float)angle);

  Pen pen(ColorBlue,14);
  //������ñ
  pen.SetEndCap(LineCapArrowAnchor);
  pen.SetStartCap(LineCapRoundAnchor);
  //��ָ��
  gcDrawLine(&pen, PointF(0,0), PointF(0,(-1)*(HEIGHT/2.75F)));

  gcResetTransform();
  gcTranslateTransform(WIDTH /2,HEIGHT/2);
  //�������ĵ�
  gcFillEllipse(&SolidBrush(ColorBlack),-6,-9,14,14);
  gcFillEllipse(&SolidBrush(ColorRed),-7,-7,14,14);

  //�����ٶȼ��ޱ��
  pen.SetWidth(4);
  pen.SetColor(ColorBlack);
  pen.SetEndCap(LineCapRound);
  pen.SetStartCap(LineCapFlat);
  gcDrawLine(&pen, PointF(HEIGHT/15.75F,HEIGHT/3.95F),
    PointF(HEIGHT/10.75F,HEIGHT/5.2F));
  pen.SetColor(ColorRed);
  gcDrawLine(&pen, PointF(HEIGHT/15.75F,HEIGHT/3.95F),
    PointF(HEIGHT/15.75F,HEIGHT/4.6F));
}

void ScaleTransform()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);
  //��ȡ��ǰ���ڵ����ĵ�
  float width=clientrect.Width/2;
  float height=clientrect.Height/2;
  PointF center(width,height);

  //���þ��γ�ʼ��СΪ30*30
  float offsetX=center.X-15;
  float offsetY=center.Y-15;

  //������ε���������:�þ��ε����ĵ��봰�ڵ����ĵ�һ��
  RectF rotateRect(offsetX,offsetY,30,30);
  PointF Pcenter(rotateRect.X+rotateRect.Width/2,
    rotateRect.Y+rotateRect.Height/2);

  //���ó�ʼ���ű���
  int scale=1;
  //�Ծ���rotateRect��ת360�Ȳ����ϷŴ����
  for(int i=0;i<360;i+=60)
  {
    // ��ͼƽ����ͼƬ�����ĵ���ת
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    //��ˮƽ�ʹ�ֱ������ͬʱ�������
    gcScaleTransform(scale,scale);
    gcRotateTransform(i);
    //�ָ���ͼƽ����ˮƽ�ʹ�ֱ�����ƽ��
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //���ƾ��β���ʱ
    gcDrawRectangle(&Pen(ColorBlack),
      rotateRect.X,rotateRect.Y,rotateRect.Width,rotateRect.Height);
    ::sys_sleep(100);
    //���û�ͼƽ������б任
    gcResetTransform();
    scale++;
  }
}

void RectScale()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Rect clientrect(0,0,
    MyClient.Width,MyClient.Height);
  //��ȡ��ǰ���ڵ����ĵ�
  float width=clientrect.Width/2;
  float height=clientrect.Height/2;
  PointF center(width,height);

  //���þ��γ�ʼ��СΪ30*30
  float offsetX=center.X-15;
  float offsetY=center.Y-15;

  //������ε���������:�þ��ε����ĵ��봰�ڵ����ĵ�һ��
  RectF rotateRect(offsetX,offsetY,30,30);
  PointF Pcenter(rotateRect.X+rotateRect.Width/2,
    rotateRect.Y+rotateRect.Height/2);

  //���ó�ʼ���ű���
  int scale=0;
  //�Ծ���rotateRect��ת360�Ȳ����ϷŴ����
  for(int i=0;i<361;i+=60)
  {
    // ��ͼƽ����ͼƬ�����ĵ���ת
    gcTranslateTransform(Pcenter.X, Pcenter.Y);
    //��ˮƽ�ʹ�ֱ������ͬʱ���Ӿ��εĿ�Ⱥ͸߶�
    rotateRect.Inflate(scale*15,scale*15);
    gcRotateTransform(i);
    //�ָ���ͼƽ����ˮƽ�ʹ�ֱ�����ƽ��
    gcTranslateTransform(-Pcenter.X, -Pcenter.Y);
    //����ͼƬ����ʱ
    gcDrawRectangle(&Pen(ColorBlack),
      rotateRect.X,rotateRect.Y,rotateRect.Width,rotateRect.Height);
    ::sys_sleep(100);
    //���öԾ��δ�С�����б任
    gcResetTransform();
    rotateRect.Inflate(-scale*15,-scale*15);
    scale++;
  }

}

void FontRotate()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  FontFamily  fontFamily(L"����");
  Font        myFont(&fontFamily, 20, FontStyleBold, UnitPixel);
  SolidBrush redBrush(ColorRed);

  //�����ı��������
  RectF layoutRect(myFont.GetHeight(&graphics),myFont.GetHeight(&graphics),
    MyClient.Width/2,MyClient.Height/2);

  StringFormat format;
  //�����ı������ʽ
  format.SetAlignment(StringAlignmentNear);
  format.SetLineAlignment(StringAlignmentCenter);
  //���ı������������óɴ��ڵ�����
  gcTranslateTransform(layoutRect.Width,layoutRect.Height);

  //����תʱÿ��30������ı�
  for(int i=0;i<360;i+=30)
  {
    //��ת��ͼƽ��
    gcRotateTransform(i);
    MyDrawString(graphics, " ��ת����",myFont, redBrush,
      layoutRect, format);
    //�ָ���������ת
    gcRotateTransform(-i);
  }

}

void MirrorText()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  SolidBrush redBrush(ColorRed);

  //�������壬���ھ����ı����
  FontFamily  fontFamily(L"���Ŀ���");
  Font        myFont(&fontFamily,
    48, FontStyleRegular, UnitPixel);
  //����Ӣ�����壬������ʾ�ı����������
  FontFamily  fontFamily2(L"Courier New");
  Font        myFont2(&fontFamily2, 16, FontStyleRegular, UnitPixel);

  //�����ı��������
  RectF layoutRect(myFont.GetHeight(&graphics),myFont.GetHeight(&graphics),
    MyClient.Width/2,MyClient.Height/2);

  StringFormat format;

  //�����ı����ʱ��ȷλ��
  //�о࣭���в��֡�(��Ƶ�λ)
  float ascent =fontFamily.GetLineSpacing(FontStyleRegular)- 
    fontFamily.GetCellDescent(FontStyleRegular);
  //�о࣭���в��֡�(��Ƶ�λ)
  float ascent2 =fontFamily.GetLineSpacing(FontStyleRegular)-
    fontFamily.GetCellAscent(FontStyleRegular);

  //���ϡ��²��������Ƶ�λת�������ص�λ
  float ascentPixel =   myFont.GetSize()*
    ascent /(fontFamily.GetEmHeight(FontStyleRegular));
  float ascentPixel2 =   myFont.GetSize()* 
    ascent2 /(fontFamily.GetEmHeight(FontStyleRegular));

  //��ͼƽ������
  gcTranslateTransform(0,MyClient.Height/4);
  //��������������ı�
  MyDrawString(graphics, "�������",myFont,redBrush,
    PointF(0,-myFont.GetHeight(&graphics)+ascentPixel2), format);

  //��ͼƽ�����ƣ���ʾ��ǰ�ı��������
  gcTranslateTransform(200,0);
  CString msg;
  for(int i=0;i<3;i++)
  {
    msg.Format("Line %d...........",i);
    MyDrawString(graphics, msg,myFont2,redBrush, 
      PointF(0,myFont2.GetHeight(&graphics)*i),format);
  }

  //�ָ��Ի�ͼƽ�������
  gcTranslateTransform(-200,0);

  //��ͼƽ���ڴ�ֱ������
  gcScaleTransform(1,-1);
  //��������ı�
  MyDrawString(graphics, "�������",myFont,SolidBrush(ColorGray), 
    PointF(0,-ascentPixel), format);
  //��ͼƽ���ٴ����ƣ���ʾ��ǰ�ı��������
  gcTranslateTransform(200,0);
  for(i=0;i<3;i++)
  {
    msg.Format("Line %d...........",i);
    MyDrawString(graphics, msg,myFont2, SolidBrush(ColorGray),
      PointF(0,myFont2.GetHeight(&graphics)*i), format);
  }

  //�����ڻ�ͼƽ���Ͻ��е����б任����ʾˮƽ�����ı����
  gcResetTransform ( ) ; 
  //��ͼƽ������
  gcTranslateTransform(MyClient.Width/2,
    MyClient.Height/2+myFont.GetHeight(&graphics)*2);
  //��������������ı�
  MyDrawString(graphics, "�������",myFont, redBrush,
    PointF(0,0), format);
  //��ͼƽ����ˮƽ������
  gcScaleTransform(-1,1);
  //���ˮƽ�����ı�
  MyDrawString(graphics, "�������",myFont, SolidBrush(ColorGray),
    PointF(0,0), format);
}


void Matrix_ListElements_Click_1()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush redBrush(ColorRed);
  FontFamily  fontFamily(L"���Ŀ���");
  Font  myFont(&fontFamily,20,
    FontStyleRegular, UnitPixel);    

  //����ƽ�Ʊ任  
  gcTranslateTransform(10,10);
  gcTranslateTransform(20,10); 
  Matrix m;;
  gcGetTransform(&m);
  //��ȡ�Ѿ�ʵʩ�ڻ�ͼƽ�������任
  CString msg;

  float x[6]; m.GetElements(x);
  //��������ÿһ��Ԫ��
  for(int i=0; i<6; i++)
  {      
    msg.Format("��%d������Ԫ��Ϊ%.2f",i,x[i]);
    MyDrawString(graphics, msg,myFont, redBrush,
      PointF(0, myFont.GetHeight(&graphics)*(i+1)));
  }
}

void MatrixPos()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,4);
  Pen pen2(ColorGray);

  FontFamily  fontFamily(L"����");
  Font  myFont(&fontFamily,
    20, FontStyleRegular, UnitPixel);

  //��ȡ��ǰ���ڵĴ�С
  Rect rect(0,0,
    MyClient.Width,MyClient.Height);

  //���ö�����Բ�ľ���
  Rect r(-40, -65,80, 130);
  Matrix matrix;
  gcTranslateTransform(rect.Width/2, rect.Height/2); 
  //ʹ��Ĭ�ϵ���ת˳��(MatrixOrderPrepend)
  gcRotateTransform(20);   
  //��ȡ�Ի�ͼƽ���Ѿ����е�����任
  gcGetTransform(&matrix);
  //������Բ
  gcDrawEllipse(&pen, r);

  //ʹ��MatrixOrderAppend��ת˳��
  gcResetTransform();
  gcTranslateTransform(rect.Width/2, rect.Height/2); 
  gcRotateTransform(20,MatrixOrderAppend);   
  //ʹ�ú�ɫ���ʻ�����һ����Բ
  gcDrawEllipse(&Pen(ColorRed,1), r);
  Matrix matrix2;
  gcGetTransform(&matrix2);

  //��������任���Դ������ĵ�Ϊԭ�����������
  gcResetTransform();
  gcDrawLine(&pen2, 0, rect.Height/2, 
    rect.Width, rect.Height/2);
  gcDrawLine(&pen2, rect.Width/2, 
    0, rect.Width/2, rect.Height);  

  CString msg;
  //��ȡ����Ԫ��ֵ
  float x[6]; matrix.GetElements(x);
  //�������Ԫ��ֵ
  int i=0;
  for (i=0; i<6; i++)
  {
    msg.Format("%8.2f",x[i]);
    //���������
    if(i==1||i==3||i==5)
    {
      //��ֱ����������һ��
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
      MyDrawString (graphics, msg, myFont, SolidBrush(ColorBlack), PointF(0,0));
    }
  }
}

void Martrix_Invert()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen myPen(ColorBlue,4);
  Matrix matrix(1.0f, 0.0f, 0.0f, 1.0f, 30.0f, 20.0f);
  //�������Ƿ���棿
  if(!matrix.IsInvertible())
  {
    AfxMessageBox("�þ���Ϊ��������Σ��޷���ʾ");
    return;
  }
  gcSetTransform(&matrix);
  for(int i=0;i<200;i++) 
  {
    //���������
    matrix.Invert();
    //�Ի�ͼƽ���������任
    gcSetTransform(&matrix);
    gcDrawRectangle(&myPen, 35, 25, 200, 100);  
  }
}

void Matrix_Multiply()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"Arial");
  Font    myFont(&fontFamily,
    16, FontStyleRegular, UnitPixel);
  //����ͼƬ
  Bitmap image(L"jieba.bmp");

  //����
  Matrix matrix1(0.80f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f); 
  //λ��
  Matrix matrix2(1.0f, 0.0f, 0.0f, 1.0f, 20.0f, 10.0f);  
  //�����������·�תͼƬ��X����Y��Ե�
  Matrix matrix3(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);

  //��һ��׷�ӱ任
  matrix1.Multiply(&matrix2, MatrixOrderAppend);
  //�ڶ���׷�ӱ任
  matrix1.Multiply(&matrix3,MatrixOrderAppend);
  //���и��ϱ任
  gcSetTransform(&matrix1);

  //����ͼƬ
  gcDrawImage(&image, 0, 0);  
  //��ȡ�Ѿ������ڻ�ͼƽ���ϵ�����任
  Matrix matrix;
  gcGetTransform(&matrix);
  gcResetTransform();
  //����λ���ϲ鿴�任������Ϣ
  gcTranslateTransform(image.GetHeight(),0);

  //��ȡ����Ԫ��ֵ
  float x[6]; matrix.GetElements(x);
  //���ÿһ������Ԫ�ص�ֵ
  CString msg;
  int i=0;
  for(i=0; i<6; i++)
  {
    msg.Format("%6.2f",x[i]);
    //���������
    if(i==1||i==3||i==5)
    {
      //��ֱ����������һ��
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
      MyDrawString(graphics, msg,myFont,SolidBrush(ColorBlack), 
        PointF(0,0));
    }
  }
}

void Matrix_TransformPoints()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush redBrush(ColorRed);
  SolidBrush blueBrush(ColorBlue);
  Pen pen1(ColorBlue,2);
  Pen pen2(ColorRed,1);

  //���幹�����ߵĵ�����
  Point points[] =
  {
    Point(50, 100), 
    Point(100, 50),
    Point(150, 125),
    Point(200, 100),
    Point(250, 125),
  };

  //��������(δʹ�ñ任ʱ)
  gcDrawCurve(&pen1, points, 5);
  //�������߶������Ϣ
  for(int i=0;i<5;i++)  
  {
    gcFillEllipse(&blueBrush,
      points[i].X-5,points[i].Y-5,10,10);
  }

  //����һ������ֱ��������������Ϊ0.5�ľ���
  Matrix matrix(1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f);
  //��points�����е�ÿһ����Ա���о�������
  matrix.TransformPoints(points);

  //��������(ʹ�ñ任��)  
  gcDrawCurve(&pen2, points,5);

  //�ٴλ������߶������Ϣ
  for(i=0;i<5;i++)  
  {
    gcFillEllipse(&redBrush,
      points[i].X-5,points[i].Y-5,10,10);
  }
}

void Matrix_TransformPoints2()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //��������ϵ��
  FontFamily   fontFamily(L"����");

  SolidBrush redBrush(ColorRed);
  //�����ı��������
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  //����·������
  GraphicsPath path;
  //��������׷���ı��������СΪ60
  path.AddString(L"������Ч",4 ,&fontFamily,(int)FontStyleRegular, 
    60, Point(0, 0), &StringFormat());

  //��ȡ·���ĵ���Ϣ
  //��ȡ·���ĵ�������Ϣ
  PathData pathdata;
  path.GetPathData(&pathdata);

  //���ı���ˮƽ��������С���ڴ�ֱ����������
  Matrix matrix(0.50f, 0.0f, 0.0f, 3.5f, 0.0f,0.0f);  

  //��points�����е�ÿһ����Ա���о�������
  matrix.TransformPoints(pathdata.Points, pathdata.Count);
  //���ݼ����ĵ����¹���·��
  GraphicsPath newpath(pathdata.Points,pathdata.Types,pathdata.Count);
  //���·��
  gcFillPath(&redBrush,&newpath);

  //�ڶ�����Ч
  Matrix matrix2(0.6f, 0.5f, 0.20f, 1.5f, 160.0f, -40.0f);
  PathData pathdata2;
  path.GetPathData(&pathdata2);
  //�����еĵ���м���
  matrix2.TransformPoints(pathdata2.Points, pathdata2.Count);
  //���ݼ����ĵ����¹���·��
  GraphicsPath newpath2(pathdata2.Points,pathdata2.Types,pathdata2.Count);
  gcFillPath(&redBrush,&newpath2);
}

void Matrix_TransformVectors()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue, 7);
  pen.SetEndCap(LineCapArrowAnchor);
  SolidBrush brush(ColorBlue);

  // ����һ�����һ������
  PointF point[]={PointF(100, 50)};
  PointF vector[]={PointF(100, 50)};

  // ���Ƶ�point
  gcFillEllipse(&brush, RectF(point[0].X-5, point[0].Y-5, 10, 10));
  //����(10,10)��vector��ʾ��λ����������
  gcDrawLine(&pen, PointF(10, 10), vector[0]);

  // ����任����
  Matrix matrix(0.8f, 0.6f, -0.6f, 0.8f, 100.0f, 0.0f);
  //�Ե�point��λ����Ϣ���б任
  matrix.TransformPoints(point);
  //�Ե�vector��λ����Ϣ���ж�ά�����任
  matrix.TransformVectors(vector);

  // ���Ʊ任��ĵ�
  pen.SetColor(ColorRed);
  brush.SetColor(ColorRed);
  gcFillEllipse(&brush, RectF(point[0].X - 5, point[0].Y - 5, 10, 10));
  //����(10,10)��任���vector��ʾ��λ����������
  gcDrawLine(&pen, PointF(10, 10), vector[0]); 
}

void Matrix_RotateAt()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Pen pen(ColorBlue,4);
  Pen pen2(ColorGray);

  //��ȡ��ǰ��������
  RectF rect(0,0,MyClient.Width,MyClient.Height);;
  //���첢ƽ�ƾ��󵽴������ĵ�
  Matrix matrix;
  matrix.Translate(rect.Width/2, rect.Height/2);  

  //����ͼƽ��������תһ��
  for(int i=0;i<360;i+=30)
  {
    //��ת��������ڴ������ĵ�
    matrix.RotateAt(i, PointF(rect.Width/2, 
      rect.Height/2), MatrixOrderAppend);
    //����ת��ľ��������ڻ�ͼƽ��
    gcSetTransform(&matrix);
    //������Բ
    gcDrawEllipse(&pen, -80, -30, 160, 60);
    //���������ڻ�ͼƽ�������еı任
    gcResetTransform();
  }

  //�Դ������ĵ�Ϊԭ�����������
  gcDrawLine(&pen2, PointF(0, rect.Height/2),
    PointF(rect.Width, rect.Height/2));
  gcDrawLine(&pen2, PointF(rect.Width/2, 0), 
    PointF(rect.Width/2, rect.Height));
}

void Matrix_Shear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"jieba.bmp");
  //����ͼƬ����ʾ����
  Rect rect(0,0,100,150);
  //����Դͼ
  gcDrawImage(&image,rect);

  Matrix matrix;
  //����ˮƽͶ������
  matrix.Shear(0.8f,0.0f);
  //�Ի�ͼƽ��ʹ��Ͷ��任
  gcSetTransform(&matrix);
  gcTranslateTransform(100,0);
  //����Ͷ��任���ͼƬ
  gcDrawImage(&image,rect);

  gcResetTransform();
  Matrix matrix2;
  //���ô�ֱͶ������
  matrix2.Shear(0.0f,0.8f);
  //�Ի�ͼƽ��ʹ��Ͷ��任
  gcSetTransform(&matrix2);
  gcTranslateTransform(200,0);
  //����Ͷ��任���ͼƬ
  gcDrawImage(&image,rect);
}

void Matrix_TextoutShear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"���Ŀ���");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  Font myfont(L"Times New Roman", 100);  
  Matrix mymat;
  //Ͷ��
  mymat.Shear( -1.4f, 0.0f ) ;
  //����
  mymat.Scale( 1, 0.5f ) ;
  //ƽ��
  mymat.Translate ( 236, 170 ) ; 
  //�Ի�ͼƽ��ʵʩ����任����
  gcSetTransform(&mymat);

  SolidBrush mybrush(ColorGray);
  SolidBrush redbrush(ColorRed);
  //������Ӱ
  MyDrawString(graphics, "Hello",myfont,mybrush, PointF(0, 50));
  gcResetTransform ( ) ;
  //����ǰ��
  MyDrawString(graphics, "Hello",myfont,redbrush, PointF(0, 50));
  CString msg;
  //��ȡ����Ԫ��ֵ
  float x[6]; mymat.GetElements(x);
  //���ÿһ������Ԫ�ص�ֵ
  int i=0;
  for(; i<6; i++)
  {
    msg.Format("{0,%6.2f}",x[i]);
    //���������
    if(i==1||i==3||i==5)
    {
      MyDrawString(graphics, msg,myFont,SolidBrush(ColorBlack), 
        PointF(0,0));
      //��ֱ����������һ��
      gcTranslateTransform(0,myFont.GetHeight(&graphics));
    }
  }
}

void Matrix_ChangeFontHeight()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //��������ϵ��
  FontFamily   fontFamily(L"����");
  //����·������
  GraphicsPath path;
  SolidBrush redBrush(ColorRed);

  //�����ı��������
  gcSetTextRenderingHint(TextRenderingHintClearTypeGridFit);
  gcSetSmoothingMode(SmoothingModeAntiAlias);

  //��������׷���ı��������СΪ80
  path.AddString(L"��С����",4,&fontFamily,(int)FontStyleRegular, 
    80, Point(0, 0), &StringFormat());
  //��ȡ·����ռ�ľ�������
  RectF bound; path.GetBounds(&bound);
  //��ȡ·����������ĵ�
  float halfH=bound.Height/2;
  float halfW=bound.Width/2;

  //��·��ʵʩ�任������·����������ĵ�
  Matrix pathMartrix(1,0,0,1,-halfW,-halfH);
  path.Transform(&pathMartrix);

  //��ȡ·���ĵ���Ϣ
  //��ȡ·���ĵ�������Ϣ
  PathData pathdata;
  path.GetPathData(&pathdata);

  //���ζ�·���Ķ�����Yֵ��������
  for(int i=0;i<pathdata.Count;i++)
  {
    //���ݸõ��·�����ľ���ռ����·�����ȵı�������Yֵ
    pathdata.Points[i].Y*=2*(bound.Width-abs(pathdata.Points[i].X))/bound.Width;
  }

  //���ݸ��ĺ��·����������¹���·��
  GraphicsPath newpath(pathdata.Points,pathdata.Types,pathdata.Count);
  //����ͼƽ���ԭ���Ƶ���������
  gcTranslateTransform(MyClient.Width/2,MyClient.Height/2-40);
  //���·��
  gcFillPath(&redBrush,&newpath);
}

void ColorMatrix_Start()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap image(L"ColorInput.bmp");
  int width = image.GetWidth();
  int  height = image.GetHeight();
  ImageAttributes  imageAttributes;

  //����ɫ�ʱ任����
  ColorMatrix colorMatrix=
  {
    2.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //����Դͼ
  gcDrawImage(&image, 0, 0);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void TranslateColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap  image(L"ColorBar.bmp");
  ImageAttributes  imageAttributes;
  int  width = image.GetWidth();
  int  height = image.GetHeight();

  //����ɫ�ʱ任����
  ColorMatrix colorMatrix=
  {
    1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
    0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
    0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.75f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(&colorMatrix, 
    ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);

  //����Դͼ
  gcDrawImage(&image, 0, 0);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);
}

void ScaleColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap          image(L"ColorBar.bmp");
  ImageAttributes imageAttributes;
  int             width = image.GetWidth();
  int             height = image.GetHeight();

  //����ɫ�ʱ任����1
  ColorMatrix colorMatrix=
  {
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����2
  ColorMatrix colorMatrix2=
  {
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 500.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����1
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //����Դͼ
  gcDrawImage(&image, 0, 0);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);

  //����Ѿ���ȡ��ɫ�ʱ任
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //���¼�����һ�任����Matrix2
  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //��ʾʹ��Matrix2��ɫ�ʵ������
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void RotateColor()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap    image(L"Colorinput.bmp");
  int   width = image.GetWidth();
  int   height = image.GetHeight();
  float   degrees = 90;
  //�ӽǶȵ�����
  double           r = degrees * PI/ 180.0f;  
  ImageAttributes  imageAttributes;
  //����Դͼ
  gcDrawImage(&image, 0, 0);

  //��ɫ������ɫ��ת
  ColorMatrix colorMatrix=
  {
    (float)cos(r),  (float)sin(r), 0.0f, 0.0f, 0.0f,
    -(float)sin(r), (float)cos(r), 0.0f, 0.0f, 0.0f,
    0.0f,    0.0f,   1.0f, 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  //ʹ��ɫ�ʱ任�������ͼƬ(R->B)
  gcTranslateTransform(width+10,0);
  //����R->Bɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  gcDrawImage(
    &image, 
    Rect(0, 10, width, height),  
    0, 0,width,height,      
    UnitPixel,
    &imageAttributes);

  //����Ѿ���ȡ��ɫ�ʱ任
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);

  //���¼�����һ�任����Matrix2
  //��ɫ���ź�ɫ��ת
  ColorMatrix colorMatrix2=
  {
    1,  0, 0.0f, 0.0f, 0.0f,
    0, (float)cos(r), (float)sin(r), 0.0f, 0.0f,
    0.0f, -(float)sin(r), (float)cos(r), 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //�ڵڶ������
  gcResetTransform();
  gcTranslateTransform(0,height+10);
  gcDrawImage(&image, 
    Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,
    &imageAttributes);

  //����Ѿ���ȡ��ɫ�ʱ任
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //��ɫ���ź�ɫ��
  ColorMatrix colorMatrix3=
  {
    (float)cos(r),  0, -(float)sin(r), 0.0f, 0.0f,
    0, 1, 0.0f, 0.0f, 0.0f,
    (float)sin(r), 0, (float)cos(r), 0.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 1.0f, 0.0f,
    0.0f,    0.0f,   0.0f, 0.0f, 1.0f
  };

  //���¼�����һ�任����Matrix3
  imageAttributes.SetColorMatrix(
    &colorMatrix3, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, width, height),
    0, 0,
    width, height,UnitPixel,
    &imageAttributes);
}

void ColorShear()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap  image(L"Colorinput.bmp");
  ImageAttributes  imageAttributes;
  int   width = image.GetWidth();
  int   height = image.GetHeight();

  //����ɫ�ʱ任����
  ColorMatrix colorMatrix=
  {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //����Դͼ
  gcDrawImage(&image, 0, 0);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

}

void ColorRemap()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //������ɫ������ͼƬ
  Bitmap            image(L"Nemo_Blue.bmp");
  ImageAttributes  imageAttributes;

  int   width = image.GetWidth();
  int   height = image.GetHeight();
  //����ɫ�滻�ɰ�ɫ,�Դﵽ�ٳ���Ч��
  ColorMap   colorMap;
  colorMap.oldColor = Color(255, 0, 0, 255); 
  colorMap.newColor = Color(255, 255,255,255);
  //����ɫ��ת����
  ColorMap remapTable[1];  remapTable[0]=colorMap;

  //����ͼƬ��ɫ����Ϣ
  imageAttributes.SetRemapTable(1, remapTable, ColorAdjustTypeBitmap);
  //����ԭʼͼ��
  gcDrawImage(&image, 0, 0, width, height);
  //�����Ѿ��ٳ�����ɫ����ͼ��
  gcDrawImage(&image, 
    Rect(width+10, 0, width, height),  //Ŀ������
    0, 0,        // Դͼ���Ͻ�����
    width,       // Դͼ��
    height,      // Դͼ��
    UnitPixel,
    //ͼƬ��ɫ����Ϣ
    &imageAttributes);
}

void SetRGBOutputChannel()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap  image(L"jieba.bmp");
  //����Դͼ
  gcDrawImage(&image, 0, 0);

  int  width = image.GetWidth();
  int height = image.GetHeight();
  ImageAttributes  imageAttributes;

  //���ú�ɫͨ��
  ColorMatrix colorMatrix=
  {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

  //����Ѿ���ȡ��ɫ�ʱ任
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);
  //������ɫͨ��
  ColorMatrix colorMatrix2=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix2, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);
  //ʹ��ɫ�ʱ任�������ͼƬ
  gcResetTransform();
  gcTranslateTransform(0,height+10);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);

  //����Ѿ���ȡ��ɫ�ʱ任
  imageAttributes.ClearColorMatrix(ColorAdjustTypeBitmap);

  //������ɫͨ��
  ColorMatrix colorMatrix3=
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  //����ɫ�ʱ任����
  imageAttributes.SetColorMatrix(
    &colorMatrix3, 
    ColorMatrixFlagsDefault,
    ColorAdjustTypeBitmap);

  //ʹ��ɫ�ʱ任�������ͼƬ
  gcTranslateTransform(width+10,0);
  gcDrawImage(&image, Rect(0, 0, width, height),
    0, 0,width, height,UnitPixel,&imageAttributes);
}

void Metafile()
{
  //  Note:  To record a metafile, you must construct a Graphics
  //object based on a Metafile object. The recording of the metafile
  //ends when that Graphics object is deleted or goes out of scope.

  //�½�һ��ͼԪ�ļ�
  HDC hdc =GetGraphics().GetHDC();
  Metafile metaFile1(L"ͼԪ�ļ�ʾ��.emf", hdc);
  {
    //ʹ��Metafile����ĵ�ַ��Ϊ��ͼƽ��
    Graphics graphics(&metaFile1);

    //����һ���ɺ쵽���Ľ���ɫ��ˢ
    gcGradientBrush RtoBBrush(
      Point(0, 10),
      Point(200, 10),
      ColorRed,   
      ColorBlue);

    //����һ���������ƵĽ���ɫ��ˢ
    gcGradientBrush BtoYBrush(
      Point(0, 10),
      Point(200, 10),
      ColorBlue,   
      ColorYellow);
    Pen bluePen(ColorBlue);

    // ���µĲ���������Ļ�ϻ���һ����ͼ��
    Rect ellipseRect(0, 0, 200, 200);
    Rect left(0, 50, 100, 100);
    gcDrawArc(&bluePen,left,180.0f,180.0f);
    Rect right(100, 50, 100, 100);
    gcFillPie(&RtoBBrush, ellipseRect,0.0f,180.0f);
    gcFillPie(&BtoYBrush, ellipseRect,180.0f,180.0f);
    gcFillPie(&RtoBBrush, left,180.0f,180.0f);
    gcFillPie(&BtoYBrush, right,0.0f,180.0f);

    //�ı���� 
    SolidBrush solidBrush(ColorBlack);
    FontFamily fontFamily(L"����");
    Font font(&fontFamily, 27,
      FontStyleRegular, UnitPixel);
    MyDrawString(graphics, "ͼԪ�ļ�ʾ��", font, solidBrush,
      PointF(20.0f, 80.0f));
    //���ˣ�GDI+���е�ֻ����Ӳ���д��ͼƬ��Ϣ�Ĳ���
  }

  //������Ļ�ͼ��Ϣ���лط�
  Graphics playbackGraphics(hdc);
  playbackGraphics.Clear(ColorWhite);
  //�򿪲���ʾͼԪ�ļ�
  Metafile metaFile2(L"ͼԪ�ļ�ʾ��.emf");
  playbackGraphics.DrawImage(&metaFile2, Point(0,0));
  //�ͷ�HDC
  playbackGraphics.ReleaseHDC(hdc);
}

void CroppingAndScaling()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap image(L"nemo.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  // Ŀ����ʾ������Դͼ��С�Ļ����ϷŴ�1.4��
  RectF destinationRect(
    width+10, 0.0f, 1.4f* width, 1.4f* height);
  //����Դͼ
  gcDrawImage(&image, 0, 0);

  //��Ŀ�����������λͼ
  gcDrawImage(
    &image,
    destinationRect,
    0.f, 0.f,    // ԭͼ���Ͻ�
    0.65f*width,      // ����ʾԭͼ��ȵ�65%����
    0.65f*height,      // ����ʾԭͼ�߶ȵ�65%����
    UnitPixel);
}

void UsingInterpolationMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"eagle.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  //����Դͼ
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),  //Ŀ������ 
    0, 0,        //Դͼ���Ͻ�����
    width,       //Դͼ���
    height,      //Դͼ��
    UnitPixel);

  //��ͼƽ������
  gcTranslateTransform( width+10,0);
  //���ٽ���ֵ��(������)
  gcSetInterpolationMode(InterpolationModeNearestNeighbor);
  gcDrawImage(
    &image,
    RectF(0.0f, 0.0f, 0.6f*width, 0.6f*height),  //Ŀ������
    0, 0,        //Դͼ���Ͻ�����
    width,       //Դͼ���
    height,      //Դͼ��
    UnitPixel);

  //��ͼƽ������
  gcTranslateTransform( 0.6f*width+10,0);
  // ������˫���Բ�ֵ��
  gcSetInterpolationMode(InterpolationModeHighQualityBilinear);
  gcDrawImage(
    &image,
    RectF(0, 0, 0.6f * width, 0.6f * height),  //Ŀ������
    0, 0,        //Դͼ���Ͻ�����
    width,       //Դͼ���
    height,      //Դͼ��
    UnitPixel);

  //��ͼƽ������
  gcTranslateTransform(width*0.6f+10, 0.f);
  // ������˫���β�ֵ��
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);
  gcDrawImage(
    &image,
    RectF(0, 0, 0.6f * width, 0.6f * height),  //Ŀ������
    0, 0,        //Դͼ���Ͻ�����
    width,       //Դͼ���
    height,      //Դͼ��
    UnitPixel);
}

void RotateFlip()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  //����ͼƬ  
  Bitmap photo(L"nemo2.bmp");

  //�õ�ͼƬ�ߴ�
  int iWidth = photo.GetWidth();
  int iHeight = photo.GetHeight();
  //����ԭʼͼƬ
  gcDrawImage(&photo, 10+photo.GetWidth()+2,
    10, photo.GetWidth(), photo.GetHeight());
  //ˮƽ��תͼƬ 
  photo.RotateFlip(RotateNoneFlipX);  
  //��ת���ͼƬ
  gcDrawImage(&photo, 10, 10, photo.GetWidth(), photo.GetHeight());   
}

void ImageSkewing()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼ�ε�Ŀ����ʾ����
  Point destination[]=
  {
    Point(200, 20),   //ԭʼͼ�����Ͻ�ӳ��������
    Point(110, 100), //ԭʼͼ�����Ͻ�ӳ��������
    Point(250, 30)  //ԭʼͼ�����½�ӳ��������
  };
  Bitmap image(L"Stripes.bmp");
  // ����ԭʼͼ��
  gcDrawImage(&image, 0, 0);

  // ���ƻ���ƽ���ı���ӳ����ͼ��
  gcTranslateTransform(image.GetWidth(),0);
  gcDrawImage(&image, destination, 3);  

}


void Cubeimage()
{
  int WIDTH=200;
  int LEFT=200;
  int TOP=200;

  Graphics &graphics=GetGraphics();
  //ʹ����ɫ������ɫ����
  gcClear(ColorBlue);

  //���ò�ֵģʽ��������˫���β�ֵ��
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);

  //�ֱ�װ�������������������ͼƬ
  Bitmap face(L"rose.bmp");
  Bitmap top(L"flower.bmp");
  Bitmap right(L"yujinxiang.bmp");

  //���¶������������������ͼƬ����
  Point destinationFace[] =
  {
    Point(LEFT,TOP), 
    Point(LEFT+WIDTH, TOP),
    Point(LEFT, TOP+WIDTH)
  };  
  //��������ͼ��
  gcDrawImage(&face,destinationFace,3); 

  //���¶������������ڶ�����ͼƬ����  
  PointF destinationTop[]=
  {
    PointF(LEFT+WIDTH/2, TOP-WIDTH/2),
    PointF(LEFT+WIDTH/2+WIDTH, TOP-WIDTH/2), 
    PointF(LEFT, TOP)
  };  
  //����������ͼ��    
  gcDrawImage(&top, destinationTop, 3); 

  //���¶��������������Ҳ��ͼƬ����        
  Point destinationRight[]=
  {
    Point(LEFT+WIDTH, TOP),
    Point(LEFT+WIDTH/2+WIDTH, TOP-WIDTH/2), 
    Point(LEFT+WIDTH,TOP+WIDTH)
  };
  //�����Ҳ���ͼ��            
  gcDrawImage(&right, destinationRight, 3);
}

void ThumbnailImage()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //���ò�ֵģʽ��������˫���β�ֵ��
  gcSetInterpolationMode(InterpolationModeHighQualityBicubic);

  //�������鿴����ͼ��ͼƬ
  Bitmap image(L"flower.bmp");
  //��ȡ��ǰ���ڴ�С
  Rect client(0,0,
    MyClient.Width,MyClient.Height);

  float width=image.GetWidth();
  float height=image.GetHeight();

  //��ȡָ����С������ͼ
  Image *pThumbnail = image.GetThumbnailImage(40,40,  NULL, NULL);

  //������ͼ��Ϊ��ˢ
  TextureBrush picBrush(pThumbnail);
  //��䴰��
  gcFillEllipse(&picBrush,client);
}

void Clone()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"head.bmp");
  int Height=image.GetHeight();
  int Width=image.GetWidth();

  //���彫ͼƬ�зֳ��ĸ����ֵ�����
  RectF block[4]=
  {
    RectF(0,0,Width/2,Height/2),
    RectF(Width/2,0,Width/2,Height/2),
    RectF(0,Height/2,Width/2,Height/2),
    RectF(Width/2,Height/2,Width/2,Height/2)
  };

  //�ֱ��¡ͼƬ���ĸ�����  
  Bitmap *s[4];
  s[0]=image.Clone(block[0],PixelFormatDontCare);
  s[1]=image.Clone(block[1],PixelFormatDontCare);
  s[2]=image.Clone(block[2],PixelFormatDontCare);
  s[3]=image.Clone(block[3],PixelFormatDontCare);
  //����ͼƬ���ĸ����֣������ֻ���ʱ����Ϊ1��
  gcDrawImage(s[0],0,0);

  //��ʱ���Դﵽ�ֿ���ʾ��Ч��
  ::sys_sleep(1000);
  gcDrawImage(s[1],Width/2,0);
  ::sys_sleep(1000);
  gcDrawImage(s[3],Width/2,Height/2);
  ::sys_sleep(1000);
  gcDrawImage(s[2],0,Height/2);
  delete s[0];
  delete s[1];
  delete s[2];
  delete s[3];
}

void Picturescale()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"photo.bmp");

  //����ͼƬ����ʾ����
  Rect rect(0,0,image.GetWidth(),image.GetHeight());
  gcDrawImage(&image,rect);

  //�ֲ���С�������СΪ80*80
  gcTranslateTransform(image.GetWidth()+10,0);
  Rect smallrect(0,0,80,80);
  //�ֲ���С
  gcDrawImage(&image,smallrect,80,10,106,112,UnitPixel);

  gcTranslateTransform(0,100);
  //�ֲ��Ŵ�������СΪ80*80
  Rect largerect(0,0,80,80);
  //���ƷŴ��ľֲ�ͼ��
  gcDrawImage(&image,largerect,56,101,35,40,UnitPixel);
}

void ImageAttributesSetNoOp()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"ColorTable.bmp");
  int width=image.GetWidth();
  //���Ʊ�׼ͼƬ  
  gcDrawImage(&image,0,0);

  gcTranslateTransform(image.GetWidth()+10,0);
  ImageAttributes imAtt;

  //����һ����ɫת������ɫ�ı任����   


  ColorMatrix brushMatrix=
  {
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  //����ɫ��У��
  imAtt.SetColorMatrix(
    &brushMatrix, 
    ColorMatrixFlagsDefault, 
    ColorAdjustTypeBitmap);

  //ʹ��ɫ��У������ͼƬ
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ��λ��
    0, 0, image.GetWidth(), image.GetHeight(),        //Դλ��
    UnitPixel,
    &imAtt);

  //��ʱ�ر�ɫ��У��
  imAtt.SetNoOp(ColorAdjustTypeBitmap);

  //��ʹ��ɫ��У������ͼƬ��ɫ->��ɫ
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ��λ��
    0, 0, image.GetWidth(), image.GetHeight(),          //Դλ��
    UnitPixel,
    &imAtt);

  //������ɫ��У���Ĺر�
  imAtt.ClearNoOp(ColorAdjustTypeBitmap);

  //ʹ��ɫ��У������ͼƬ����ɫ->��ɫ
  gcTranslateTransform(width+10,0);
  gcDrawImage(
    &image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ��λ��
    0, 0, image.GetWidth(), image.GetHeight(),          //Դλ��
    UnitPixel,
    &imAtt);
}


void CreateMetaFile()
{
  Graphics &metagraph=GetGraphics();

  //�½�һ��ͼԪ�ļ�
  HDC hdc =metagraph.GetHDC();
  Metafile metaFile1(CWideCharString("ddd.emf"), hdc);
  //ʹ��Metafile����ĵ�ַ��Ϊ��ͼƽ��
  Graphics *graphics=Graphics::FromImage(&metaFile1);
  graphics->ScaleTransform(0.8f,0.8f);

  //����ˮƽ�������������Բ�����
  graphics->SetSmoothingMode(SmoothingModeHighQuality);
  //��ɫ��Բ
  graphics->DrawEllipse(&Pen(Color.Red,10), Rect(0, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorRed), Rect(0, 0, 75, 95));

  //��ɫ��Բ
  graphics->DrawEllipse(&Pen(ColorGreen,10), Rect(40, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorGreen), Rect(40, 0, 75, 95));

  //��ɫ��Բ
  graphics->DrawEllipse(&Pen(Color.Blue,10), Rect(80, 0, 75, 95));
  graphics->FillEllipse(&SolidBrush(ColorBlue), Rect(80, 0, 75, 95));

  // ׷������ɫ�ʵ��ı�
  FontFamily fontFamily(CWideCharString("Arial"));
  Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
  graphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

  //��ɫ����
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorRed), PointF(-80.0f, 0.0f));
  //��ɫ����
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorGreen), PointF(-80.0f, font.GetHeight(graphics)));
  //��ɫ����
  MyDrawString(*graphics, "GDI+",font, SolidBrush(ColorBlue), PointF(-80.0f, font.GetHeight(graphics)*2));
  //�ͷ�������Դ��
  //graphics->Dispose();
  //metaFile1.Dispose();
  metagraph.ReleaseHDC(hdc);
  //metagraph.Dispose();
  delete graphics;
}

void SetColorMatrices()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼԪ�ļ�  
  Metafile image(L"ddd.emf");
  Unit  unit=UnitPixel;
  //��ȡͼƬ����
  RectF rect; image.GetBounds(&rect, &unit);

  //��ʹ���κ�ɫ��У�����ͼƬ
  gcDrawImage(&image, 0.0f, 0.0f, rect.Width, rect.Height);
  ImageAttributes  imAtt;

  //����һ��ʹ��ɫ��������1.5�ľ���
  ColorMatrix defaultColorMatrix=
  { 
    1.5f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //����һ��ʹ��ɫ��������1.5�ľ���
  ColorMatrix defaultGrayMatrix=
  {  
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.5f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //���ʵĲ�ɫɫ����Ϣ��У��������ɫ��������1.5�ľ���
  ColorMatrix penColorMatrix= 
  {  
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.5f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  //���ʵĻҶ�ɫ�������з�������1.5�ľ���
  ColorMatrix penGrayMatrix=
  {
    1.5f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.5f,  0.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.5f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  1.0f
  };

  // ����Ĭ�ϵĲ�ɫ���Ҷ�У������.
  //ColorAdjustTypeDefault:�޸����е�ɫ����Ϣ
  imAtt.SetColorMatrices(
    &defaultColorMatrix,
    &defaultGrayMatrix, 
    ColorMatrixFlagsAltGray,
    ColorAdjustTypeDefault);
  //ʹ��У���������ͼԪ�ļ���У�����е�ɫ��
  gcTranslateTransform(image.GetWidth()+10,0);
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt);

  //���û��ʵĲ�ɫ���Ҷ�ɫ��У������
  //ColorAdjustTypePen����������ɫ��
  imAtt.SetColorMatrices(
    &penColorMatrix,
    &penGrayMatrix, 
    ColorMatrixFlagsAltGray,
    ColorAdjustTypePen); 
  //�ڵڶ��л���
  gcResetTransform();
  gcTranslateTransform(0,image.GetHeight());
  //ʹ��������Ļ��ʻ���ͼƬ 
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //����ڻ����ϵ����б任
  imAtt.ClearColorMatrix(ColorAdjustTypePen);
  gcDrawImage(
    &image, 
    Rect(0, 0, (int)rect.Width,(int)rect.Height),  
    rect.X, rect.Y,               
    rect.Width,                   
    rect.Height,                   
    UnitPixel,
    &imAtt); 
}

void SetOutputChannelColorProfile()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"car.bmp");
  ImageAttributes imAtt;
  int width = image.GetWidth();
  int height = image.GetHeight();
  Rect rect(0, 0, width, height);
  //����ԭʼͼƬ
  gcDrawImage(&image, rect);

  gcTranslateTransform(width,0);
  //����ɫ�������ļ�
  imAtt.SetOutputChannelColorProfile(
    CWideCharString("kodak_dc.ICM"), ColorAdjustTypeBitmap);

  //ʹ��ɫ�������ļ����ͼƬ
  gcDrawImage(
    &image,
    rect,  
    0, 0, width, height,          
    UnitPixel,
    &imAtt);
}

void Gammaadjust()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ԭʼͼƬ
  Bitmap image(L"warrior.bmp");
  int width = image.GetWidth();
  int height = image.GetHeight();

  // ����������Ϣ
  Font myFont(L"����", 12);
  //������ʾ��Ϣ����ʾ����
  PointF origin(width+10, height+20);
  SolidBrush blackBrush(ColorBlack);

  ImageAttributes imAtt;   
  CString msg;
  //��0-3���ε������ͼƬʱ��ʹ�õ�Gammaֵ
  for(float i=0.0f;i<3.0f;i+=0.1f)
  {  
    //����ԭʼͼƬ
    gcDrawImage(&image,0,0);

    //����Gammaֵ
    imAtt.SetGamma(i,ColorAdjustTypeBitmap);
    //ʹ���޸ĺ�Gammaֵ����ͼƬ���
    gcDrawImage(
      &image,
      Rect(width+10, 0, width, height),  //Ŀ������
      0, 0, width, height,           //Դ����
      UnitPixel,
      &imAtt);
    msg.Format("�����޸�Gammaֵ��Gamma=%.2f",i);
    //��ʾ��ǰ��Gammaֵ��Ϣ
    MyDrawString(graphics, msg,myFont,blackBrush,origin);
    //��ʱһ���Ա�ۿ�Ч��
    ::sys_sleep(100);
    gcClear(ColorWhite);
    imAtt.ClearGamma();
  }
}

void SetOutputChannel()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ͼƬ
  Bitmap image(L"jieba.bmp");
  //ͼƬ�ĸ߶�
  int width = image.GetWidth();
  int height = image.GetHeight();

  //����ԭʼͼƬ
  gcDrawImage(&image, RectF(0, 0, width, height));
  ImageAttributes imAtt;
  //����ɫ�����ͨ��cyan
  imAtt.SetOutputChannel(ColorChannelFlagsC,
    ColorAdjustTypeBitmap);
  //���ƣ�����ͼƬ
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),  
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //����ɫ�����ͨ��:magenta 
  imAtt.SetOutputChannel(ColorChannelFlagsM, 
    ColorAdjustTypeBitmap);
  //���ƣ�����ͼƬ
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //����ɫ�����ͨ��:yellow
  imAtt.SetOutputChannel(ColorChannelFlagsY, 
    ColorAdjustTypeBitmap);
  //���ƣ�����ͼƬ
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,           
    UnitPixel,
    &imAtt);

  //����ɫ�����ͨ��:black
  imAtt.SetOutputChannel(ColorChannelFlagsK, 
    ColorAdjustTypeBitmap);
  //���ƣ�����ͼƬ
  gcTranslateTransform(width, 0);
  gcDrawImage(
    &image,
    Rect(0, 0, width, height),
    0, 0, width, height,            
    UnitPixel,
    &imAtt); 

}

void Colorkey()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //װ��ǰ�󱳾�ͼƬ
  Bitmap forground(L"grid.bmp");
  Bitmap background(L"snike.bmp");

  int width=background.GetWidth();
  int height=background.GetHeight();
  Rect rect(0,0,width,height);

  //����ɫ���óɹؼ�ɫ
  ImageAttributes imAtt;
  imAtt.SetColorKey(
    ColorRed,
    ColorRed,
    ColorAdjustTypeBitmap);

  //���Ʊ���
  gcDrawImage(&background,0,0); 
  //����ǰ��
  gcDrawImage(
    &forground, 
    rect,
    0, 0, forground.GetWidth(), forground.GetHeight(),
    UnitPixel,
    &imAtt);

  gcTranslateTransform(width+20,0);
  gcDrawImage(&background,0,0); 
  //����Ѿ�Ӧ�õĹؼ�ɫ��Ϣ
  imAtt.ClearColorKey(ColorAdjustTypeBitmap);   
  ///����ɫ���óɹؼ�ɫ
  imAtt.SetColorKey(
    ColorBlue,
    ColorBlue,
    ColorAdjustTypeBitmap);
  gcDrawImage(&forground, 
    rect,
    0, 0, forground.GetWidth(), forground.GetHeight(),        
    UnitPixel,
    &imAtt);
  gcTranslateTransform(width+20,0);

  //����Դͼ
  gcDrawImage(&background,0,0); 
}

void Setthreshold()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"box-2.bmp");
  int Width=image.GetWidth();
  int Height=image.GetHeight();
  //����ԭʼͼƬ
  gcDrawImage(&image, 10, 10, Width, Height);

  //����ֵ��0��1��������
  ImageAttributes imAtt;
  for(float i=0.0f;i<1.0f;i+=0.1f)
  {
    //�������ͼƬʱʹ�õ���ֵ
    imAtt.SetThreshold(i, ColorAdjustTypeBitmap);
    //�����Ѿ�ʹ������ֵ��ͼƬ
    gcDrawImage(&image,
      Rect(10+Width, 10, Width, Height),  
      0, 0, Width, Height, 
      UnitPixel,
      &imAtt);
    //��ʱ
    ::sys_sleep(1000);
  }
}

void AdjustedPalette()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  //����ͼƬ
  Bitmap image(L"lord-256.bmp");
  //����ͼƬ
  Bitmap image2(L"lord-256.bmp");
  //��ȡͼƬʹ�õĵ�ɫ����Ϣ
  int size=image.GetPaletteSize();
  ColorPalette *palette= (ColorPalette *)new BYTE[size];
  image.GetPalette(palette, size);

  //��ȡ��ɫ����������ɫ������
  int count=palette->Count;
  if(count<1)
  {
    AfxMessageBox("ͼƬ�޵�ɫ����Ϣ����");
    return;
  }  

  //���ĵ�ɫ���е�ÿһ��ɫ����Ϣ    
  for(int i=0;i<count;i++)
  {
    Color u(palette->Entries[i]);
    int r=u.GetR()/2;
    int g=u.GetG()/2;
    int b=u.GetB()/2;

    if(r<1)
      r=0;
    if(g<1)
      g=0;
    if(b<1)
      b=0;
    palette->Entries[i]=u.MakeARGB(255, r,g,b);
  }

  //����ͼ����µ�ɫ��
  image.SetPalette(palette);
  //����ԭͼ
  gcDrawImage(&image2,0,0);
  //�����޸ĺ��ͼƬ
  gcDrawImage(&image,image.GetWidth()+10,0);
  delete palette;
}

void SetWrapMode()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //����ͼƬ
  Bitmap image(L"yueru.bmp");
  ImageAttributes imAtt;

  //����ͼƬ���з�ʽΪWrapModeClamp��ͼƬ������ƽ��
  imAtt.SetWrapMode(WrapModeClamp,ColorRed);

  //��С��ʾԴͼ
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ������
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),      //ԴͼƬ����
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //����ͼƬ���з�ʽΪWrapModeTileFlipXY��ͼƬ��ˮƽ�ʹ�ֱ������ͬʱ��ת
  imAtt.SetWrapMode(WrapModeTileFlipXY);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ������
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //ԴͼƬ����
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //����ͼƬ���з�ʽΪWrapModeTileFlipX��ͼƬ��ˮƽ�Ϸ�ת
  imAtt.SetWrapMode(WrapModeTileFlipX);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ������
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //ԴͼƬ����
    UnitPixel,
    &imAtt);

  gcTranslateTransform(image.GetWidth()+10,0);
  //����ͼƬ���з�ʽΪWrapModeTileFlipY��ͼƬ�ڴ�ֱ�Ϸ�ת
  imAtt.SetWrapMode(WrapModeTileFlipY);
  gcDrawImage(&image,
    Rect(0, 0, image.GetWidth(), image.GetHeight()),  //Ŀ������
    0, 0, 2*image.GetWidth(), 2*image.GetHeight(),       //ԴͼƬ����
    UnitPixel,
    &imAtt);
}

void ListAllImageEncoders()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"���Ŀ���");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  //��ȡ��������Ϣ
  UINT num, size;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*)new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);

  //���������Ϣ
  CString msg;
  for(UINT j = 0; j <num; ++j)
  { 
    CString s;
    s.Format("����������:%s\t�ļ���ʽ��չ��:%s\t\n", 
      CString(pImageCodecInfo[j].CodecName),
      CString(pImageCodecInfo[j].FilenameExtension));
    msg+=s;
  }
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
  delete pImageCodecInfo;
}

//��������
CString MyStringFromGUID2(const GUID &rguid)
{
  WCHAR strGuid[39];
  StringFromGUID2(rguid, strGuid, 39);
  return CString(strGuid);
}

void ListImageEncoder_Detail()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //��ȡ��������Ϣ
  UINT num, size;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);
  //����ָ����ʽ�ļ��ı�������Ϣ
  CString msg;
  //��ѯ���еı�������Ϣ
  for(UINT j = 0; j <num; j++)
  {
    CString s;
    s.Format("��ʼ������%d��ͼ�α�����Ϣ\n\n",j); msg+=s;
    s.Format("�����ʶ: %s\n",MyStringFromGUID2(pImageCodecInfo[j].Clsid));  msg+=s;
    s.Format("�ļ���ʽ��ʶ: %s\n", MyStringFromGUID2(pImageCodecInfo[j].FormatID)); msg+=s;
    s.Format("����������: %s\n", CString(pImageCodecInfo[j].CodecName)); msg+=s;
    s.Format("����������Ķ�̬���ӿ���: %s\n",CString(pImageCodecInfo[j].DllName)); msg+=s;
    s.Format("��������: %s\n", CString(pImageCodecInfo[j].FormatDescription)); msg+=s;
    s.Format("��������Ӧ���ļ���չ��: %s\n",CString(pImageCodecInfo[j].FilenameExtension)); msg+=s;
    s.Format("��������MIME��������: %s\n",CString(pImageCodecInfo[j].MimeType)); msg+=s;
    s.Format("ImageCodecFlagsö�ٵı�Ǽ�: %d\n",pImageCodecInfo[j].Flags); msg+=s;
    s.Format("�������汾: %d\n",pImageCodecInfo[j].Version); msg+=s;

    int sigCount = pImageCodecInfo[j].SigCount;
    s.Format("���������Ӧ�ı�����ǩ���������С:%d\n", sigCount); msg+=s;
    s.Format("��%d��ͼ�α�����Ϣ�������\n\n",j); msg+=s;
    AfxMessageBox(msg);
    msg.Empty();
  }
  delete pImageCodecInfo;

  //������������ϸ��Ϣд���ļ�
  //StreamWriter sw= StreamWriter(@"listinfo.txt",false,System.Text.Encoding.Unicode);
  //sw.Write(msg);
  //sw.Close();
  //AfxMessageBox("������������򿪵�ǰĿ¼�µ�listinfo.txt�鿴��������Ϣ");
}

void ListImageDecoder()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"���Ŀ���");
  Font        myFont(&fontFamily, 20, FontStyleRegular, UnitPixel);

  //��ȡ��������Ϣ
  UINT num, size;
  GetImageDecodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageDecoders(num, size, pImageCodecInfo);

  //���ÿһ������������ϸ��Ϣ
  CString msg;
  for(UINT j = 0; j <num; j++)
  {
    CString s;
    s.Format("����������:%s\t�ļ���ʽ��չ��:%s\t\n", 
      CString(pImageCodecInfo[j].CodecName),CString(pImageCodecInfo[j].FilenameExtension));
    msg+=s;
  }
  delete pImageCodecInfo;
  //��ʾ��Ϣ
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
}

GUID GetEncoderClsid(CString format)
{
  GUID picGUID;
  //��ȡ��������Ϣ
  UINT num=0, size=0;
  GetImageEncodersSize(&num, &size);
  ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo*) new BYTE[size];
  GetImageEncoders(num, size, pImageCodecInfo);
  //����ָ����ʽ�ļ��ı�������Ϣ
  CWideCharString formatW(format);
  for(UINT i = 0; i <num; i++)
  {  //MimeType�����뷽ʽ�ľ�������
    if(formatW == CWideCharString(pImageCodecInfo[i].MimeType))
    {
      picGUID= pImageCodecInfo[i].Clsid;
      break;
    }
  }
  delete pImageCodecInfo;
  return picGUID;
}

void GetEncoderParameter()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"����");
  Font        myFont(&fontFamily, 16, FontStyleRegular, UnitPixel);

  CString msg;
  // Ϊ�˲�ѯ��λͼ����ΪJPEG��ʽ��ͼƬ���½�һ��λͼ
  Bitmap bitmap(1,1);

  //��ȡJPEG��ʽ��ͼ��������ı�ʶ
  GUID encoderClsid;
  encoderClsid=GetEncoderClsid(L"image/jpeg");

  // ��ȡת����JPG����Ҫ�ľ��������Ϣ
  UINT size=bitmap.GetEncoderParameterListSize(&encoderClsid);
  EncoderParameters *pEncoderParameters= (EncoderParameters *)new BYTE[size];
  bitmap.GetEncoderParameterList(&encoderClsid, size, pEncoderParameters);
  // �鿴pEncoderParameters�����ж��ٸ�EncoderParameter��
  int count=pEncoderParameters->Count;
  msg.Format("�ڱ��������������%d��EncoderParameter��\nÿ�������ϸ��ϢΪ��\n", count);
                      
  EncoderParameter *pEncoderParameter=pEncoderParameters->Parameter;
  /*�ֱ�鿴EncoderParameters�����е�
  ÿһ��EncoderParameter����ĳ�Ա����
  GUID��NumberOfValues��Type*/
  for(int i=0;i<count;i++)
  {
    CString s;
    //��GUIDֵת�����ִ� 
    s.Format("�������õĵ�%d����������(GUID):%s\n", i, MyStringFromGUID2(pEncoderParameter[i].Guid));
    msg+=s;
    //�鿴��ÿһ�����������£��������õĲ�����Ϣ
    s.Format("\t�ڸò������������£�����Խ��е����õĲ����ֱ�����\n");
    msg+=s;
    s.Format("\t\t��������=%d\n", pEncoderParameter[i].NumberOfValues);
    msg+=s;
    s.Format("\t\t��������=%d\n", pEncoderParameter[i].Type);
    msg+=s;
  }
  //�����������б���Ϣ
  MyDrawString(graphics, msg,myFont,brush, PointF(0,0));
  delete pEncoderParameters;
}

HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars);
CString _EncoderParameterCategoryFromGUID(GUID guid)
{
   CONST MAX_CATEGORY_LENGTH = 50;
   WCHAR strParameterCategory[MAX_CATEGORY_LENGTH] = L"";
   if (Ok==EncoderParameterCategoryFromGUID(guid, strParameterCategory, MAX_CATEGORY_LENGTH))
     return CString(strParameterCategory);
   return _T("ʧ��");
}

CString ShowAllEncoderParameters(CString format)
{
  CString outmsg;    
  
  //��λͼΪ�����鿴pImageCodecInfo����ϸ��Ϣ  
  Bitmap bitmap(L"head.bmp");

  GUID encoderClsid=GetEncoderClsid(format);
  UINT size=bitmap.GetEncoderParameterListSize(&encoderClsid);
  if (0==size)
  {
    return _T("����");
  }
  EncoderParameters *encodersarameters= (EncoderParameters *)new BYTE[size];
  bitmap.GetEncoderParameterList(&encoderClsid, size, encodersarameters);

  // ��ȡ��������Ĳ����б�
  //��ȡEncoderParameter��������
  int count=encodersarameters->Count;
  outmsg.Format("��EncoderParameters�У���%d�� EncoderParameter����\n", 
    count);
  EncoderParameter *pEncoderParameter=encodersarameters->Parameter;

  // �鿴ÿһ��EncoderParameter������Ϣ
  for(int k = 0; k <count;++k)
  {
    //��ԭGUID��Ϣ
    CString strParameterCategory=_EncoderParameterCategoryFromGUID(
      pEncoderParameter[k].Guid);
    CString s;
    s.Format("\t��������: %s.\n", strParameterCategory); outmsg+=s;
    s.Format("\t�ò���������ֵһ���� %d��\n",
      pEncoderParameter[k].NumberOfValues); outmsg+=s;
    s.Format("\t��������%d.\n", pEncoderParameter[k].Type); outmsg+=s;
  } 
  delete encodersarameters;
  //��������Ϣ����outmsg
  return outmsg;
}

void GetAllEncoderParameter()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //���������Ϣʱʹ�õ����塢��ˢ
  SolidBrush brush(ColorBlue);
  FontFamily  fontFamily(L"����");
  Font  myFont(&fontFamily, 16, FontStyleRegular,UnitPixel);

  //�������б���ϸ��Ϣ���浽msg֮��
  CString msg=ShowAllEncoderParameters(L"image/jpeg");
  //��ʾ�����б���Ϣ
  MyDrawString(graphics, msg, myFont, SolidBrush(ColorBlack), PointF(0,0));
}

void menuItem17()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap image(L"snike.bmp");
  RectF rect(0,0,image.GetWidth()/2,image.GetHeight()/2);
  gcDrawImage(&image,rect);

  //��BMP����ΪPNG�ļ�,��ʹ�ñ������
  GUID pngGuid = GetEncoderClsid("image/png");
  MyImageSave(image, L"snike.png", &pngGuid, NULL);

  //ʹ�õڶ��ַ�������encoder����
  MyImageSave(image,L"snike2.png", &pngGuid);
    
  //�ֱ�����ַ��������ͼ��
  Bitmap image_png1(L"snike.png");
  Bitmap image_png2(L"snike2.png");

  //����PNG��ʽ��ͼƬ
  gcTranslateTransform(rect.Width,0);
  gcDrawImage(&image_png1,rect);
  gcTranslateTransform(rect.Width,0);
  gcDrawImage(&image_png2,rect);
}

void SaveBmp2tif()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  Bitmap myBitmap(L"jieba.bmp");

  // ��ȡTIFF��ʽ�ļ��ı�����Ϣ
  GUID tiffGuid = GetEncoderClsid("image/tiff");

  EncoderParameters params;  //�����Ѿ�������һ�� EncoderParameter
  params.Count=1;
  // ʹ��LZWѹ����ʽ��ͼͼ��Ϊ TIFF�ļ�
  params.Parameter[0].Guid= EncoderCompression;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueCompressionLZW;

  MyImageSave(myBitmap,L"jieba.tif", &tiffGuid, &params);

  delete params.Parameter[0].Value;
}

void SaveBMP2JPG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //��BMP�ļ�
  Bitmap myBitmap(L"car.bmp");
  //��ȡ��ʾͼƬ����Ҫ����
  Rect imgrect(0,0, myBitmap.GetWidth(),myBitmap.GetHeight());

  gcDrawImage(&myBitmap, imgrect);

  //��ȡJPEG��ʽ�ı��뷽ʽ
  GUID jpegGuid= GetEncoderClsid("image/jpeg");

  //�ֱ�����JPEG�ļ���ͼƬ����
  //�����������Ϊ Quality,
  EncoderParameters params;  //�����Ѿ�������һ�� EncoderParameter
  params.Count=1;
  params.Parameter[0].Guid= EncoderQuality;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  //����JPEGͼƬ����Ϊ25��
  ((long*)params.Parameter[0].Value)[0] = 25L;
  MyImageSave(myBitmap,L"car025.jpg", &jpegGuid, &params);

  //����JPEGͼƬ����Ϊ50��
  ((long*)params.Parameter[0].Value)[0] = 50L;
  MyImageSave(myBitmap,L"car050.jpg", &jpegGuid, &params);

  //����JPEGͼƬ����Ϊ75��
  ((long*)params.Parameter[0].Value)[0] = 75L;
  MyImageSave(myBitmap,L"car075.jpg", &jpegGuid, &params);

  delete params.Parameter[0].Value;

  //�ֱ���ʾ��ͬͼƬ������JPEG�ļ�
  Bitmap image01(L"car025.jpg");
  //��ͼƽ������
  gcTranslateTransform(imgrect.Width+10,0);
  gcDrawImage(&image01,imgrect);

  Bitmap image02(L"car050.jpg");
  //���û�ͼƽ�棬����
  gcResetTransform();

  gcTranslateTransform(0,imgrect.Height+10);
  gcDrawImage(&image02,imgrect);

  Bitmap image05(L"car075.jpg");
  gcTranslateTransform(imgrect.Width+10,0);
  gcDrawImage(&image02,imgrect);
}

void TransformingJPEG()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  
  //��JPEG�ļ�
  Bitmap myBitmap(L"car.jpg");
  //��ȡ��ʾͼƬ����Ҫ����
  Rect imgrect(0,0, myBitmap.GetWidth(), myBitmap.GetHeight());
  //����ԭͼ
  gcDrawImage(&myBitmap,imgrect);

  //��ȡJPEG��ʽ�ı��뷽ʽ
  GUID jpegGuid= GetEncoderClsid("image/jpeg");

  //�ֱ�����JPEG�ļ���λ�ñ任��Ϣ
  //�����������Ϊ Transformation
  EncoderParameters params;  //�����Ѿ�������һ�� EncoderParameter
  params.Count=1;
  params.Parameter[0].Guid= EncoderTransformation;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueTransformRotate270;

  // ��ͼƬ��ת90�Ⱥ󱣴�
  MyImageSave(myBitmap,L"car0_rotate.jpg", &jpegGuid, &params);

  delete params.Parameter[0].Value;

  //������ת���ͼƬ
  gcTranslateTransform(imgrect.Width,0);
  Bitmap myBitmap2(L"car0_rotate.jpg");
  //��ȡ��ʾͼƬ����Ҫ����
  Rect imgrect2(0,0, myBitmap2.GetWidth(), myBitmap2.GetHeight());
  gcDrawImage(&myBitmap2, imgrect2);
}
void MultipleFrameImage()
{
  //װ�����Ų�ͬ��ʽ��ͼƬ
  Bitmap multi(L"dog.bmp");
  Bitmap page2(L"dog.gif");
  Bitmap page3(L"cute.jpg");
  Bitmap page4(L"cat.png");
  
  //��ȡtiffͼ���ʽ�ı�����Ϣ
  GUID tiffGuid = GetEncoderClsid("image/tiff");
  
  EncoderParameters params;  //�����Ѿ�������һ�� EncoderParameter
  params.Count=1;
  //�������ͣ�SaveFlag
  params.Parameter[0].Guid= EncoderSaveFlag;
  params.Parameter[0].NumberOfValues=1;
  params.Parameter[0].Type=EncoderParameterValueTypeLong;
  params.Parameter[0].Value=new long[1];

  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueMultiFrame;
  
  //�����һ��ͼƬ
  MyImageSave(multi,L"Multiframe.tiff", &tiffGuid, &params);
  
  //����ڶ���ͼƬ
  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueFrameDimensionPage;
  multi.SaveAdd(&page2, &params);
  
  //���������ͼƬ
  multi.SaveAdd(&page3, &params);
  
  //���������ͼƬ
  multi.SaveAdd(&page4, &params);

  // �ر�TIFF�ļ�
  ((long*)params.Parameter[0].Value)[0] = (long)EncoderValueFlush;
  multi.SaveAdd(&params);

  delete params.Parameter[0].Value;

  AfxMessageBox("������������򿪵�ǰĿ¼�µ�Multiframe.tiff�鿴ͼƬ��ӽ��");
}

void GetImageFromMultyFrame()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  //�������зֳ��ĸ����֣�������ʾ�ķ�֡ͼƬ
  Rect ClientRect(0,0,
    MyClient.Width,MyClient.Height);
  Rect r1(0,0,
    ClientRect.Width/2,ClientRect.Height/2);
  Rect r2(ClientRect.Width/2,0,
    ClientRect.Width/2,ClientRect.Height/2);
  Rect r3(0,
    ClientRect.Height/2,ClientRect.Width/2,ClientRect.Height/2);
  Rect r4(ClientRect.Width/2,
    ClientRect.Height/2,ClientRect.Width/2,ClientRect.Height/2);

  //��TIF�ļ�
  Bitmap  multi(L"Multiframe.tiff");

  //FrameDimension.Page:��֡ͼƬ
  GUID pageGuid = FrameDimensionPage;

  GUID pngGuid = GetEncoderClsid("image/png");

  //��ʾ�������һ֡ͼƬ
  multi.SelectActiveFrame(&pageGuid, 0);
  gcDrawImage(&multi, r1);
  MyImageSave(multi,L"Page0.png", &pngGuid);

  //��ʾ������ڶ�֡ͼƬ
  multi.SelectActiveFrame(&pageGuid, 1);
  gcDrawImage(&multi,r2);
  MyImageSave(multi,L"Page1.png",&pngGuid);

  ////��ʾ���������֡ͼƬ
  multi.SelectActiveFrame(&pageGuid, 2);
  gcDrawImage(&multi, r3);
  MyImageSave(multi,L"Page2.png",&pngGuid);

  ////��ʾ���������֡ͼƬ
  multi.SelectActiveFrame(&pageGuid, 3);
  gcDrawImage(&multi, r4);
  MyImageSave(multi,L"Page3.png",&pngGuid);
}

void QueryImage()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);

  SolidBrush brush(ColorBlack);
  FontFamily  fontFamily(L"����");
  Font        myFont(&fontFamily, 16, FontStyleRegular, UnitPixel);

  Bitmap image(L"car.jpg");
  //��ȡͼ���������
  UINT size, count=0;
  image.GetPropertySize(&size, &count);
  PropertyItem *propItem= (PropertyItem *)new BYTE[size];
  image.GetAllPropertyItems(size, count, propItem);

  // ͼ���й��ж����������ɹ���ѯ
  if(count == 0)
  {
    AfxMessageBox("���������ɹ���ѯ");
    return;
  }

  CString tmp;
  for(UINT j = 0; j < count; ++j)
  {
    CString s;
    s.Format("�� %d ���ɹ�����ѯ���������ı��ֵΪ��16���ƣ���%x\n",
      j,propItem[j].id); tmp+=s;
    s.Format("�� %d ����������Ӧ�����������ϢΪ��\n",j); tmp+=s;

    //��ȡ��������ϸ������Ϣ
    s.Format("\t������ĳ���Ϊ%d\n\t��������Ϊ%d\n",
      propItem[j].length, propItem[j].type); tmp+=s;
  }

  delete propItem;

  //���ͼ���������Ϣ
  MyDrawString(graphics, tmp, myFont, brush, PointF(0,0));
}

void SetProp()
{
  //װ��ͼƬ�Թ��޸�
  Bitmap image(L"car.jpg");

  // ����ͼƬ������ΪJasmine
  BYTE newWriterValue[] = {(BYTE)'J', (BYTE)'a', (BYTE)'s', (BYTE)'m', (BYTE)'i', (BYTE)'e', 0};

  //��ȡͼ���������
  UINT size, count=0;
  image.GetPropertySize(&size, &count);
  PropertyItem *pp= (PropertyItem *)new BYTE[size];
  image.GetAllPropertyItems(size, count, pp);

  //0x13b���ñ��ֵ��ӦͼƬ����
  pp[0].id=0x13b;
  //����ֵ����
  pp[0].length=strlen((const char*)newWriterValue);
  //2������ֵ����������Ϊ�ַ���
  pp[0].type=2;
  pp[0].value=newWriterValue;

  //����ͼƬ����
  image.SetPropertyItem(pp+0);
  GUID jpegGuid= GetEncoderClsid("image/jpeg");
  MyImageSave(image,L"newwriter.jpg", &jpegGuid);

  delete pp;

  //���¼����޸ĺ��ͼƬ
  Bitmap image2(L"newwriter.jpg");
  //�鿴ͼƬ��������Ϣ
  size = image2.GetPropertyItemSize(0x13b);
  pp= (PropertyItem *)new BYTE[size];
  image2.GetPropertyItem(0x13b, size, pp);
  CString msg;
  msg.Format("ͼƬ�������Ѿ�����Ϊ\n\n%s",CString((BYTE*)pp->value));
  AfxMessageBox(msg);

  delete pp;
}

void OnCanvas()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"box-2.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  Color color;
  gcDrawImage(&image,
    Rect(0, 0, Width, Height));
  
  //�������������
  for(int i=0;i<Width-5;i++)
  {
    for( int j=0; j<Height-5;j++)
    {
      int a=rand_Next(1000)%5;
      
      image.GetPixel(i+a, j+a, &color);
      //���õ��RGBֵ���óɸ������֮�ڵ���һ��
      image.SetPixel(i, j, color);
    }
    //��̬�����˾�Ч��ͼ
    gcDrawImage(&image,
      Rect(Width, 0, Width, Height));
  }
}

void OnWood()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.7f,0.7f);

  Bitmap image(L"box-2.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  Color colorTemp,color2;
  Color color;
  //����ԭͼ
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  int tmp;
  for(int i=0;i<Width;i++)
  {
    for( int j=0; j<Height;j++)
    {
      image.GetPixel(i, j, &color);
      //���ݸõ�RGB��ƽ��ֵ��ȷ�ϸõ�ġ�������
      int avg;
      avg=(color.GetRed()+color.GetGreen()+color.GetBlue())/3;
      if(avg>=128)
        tmp=255;
      else
        tmp=0;
      colorTemp = Color(255,tmp,tmp,tmp);
      //��������RGBֵ��д��λͼ
      image.SetPixel(i, j, colorTemp);
    }
    //��̬�����˾�Ч��ͼ
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }

}
//��������A��B֮��ľ��Ծ���
float fDistance(Point A,Point B)
{
  double i=((A.X-B.X)*(A.X-B.X))+((A.Y-B.Y)*(A.Y-B.Y));
  return (float)sqrt(i);
}

void Flashligt()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  
  Bitmap image(L"sports.bmp");
  int Width = image.GetWidth();
  int Height = image.GetHeight();
  int A=Width/2;
  int B=Height/2;
  //Center:ͼƬ���ĵ㣬������ֵ����ǿ�����ķ���ƫ��
  Point Center= Point(A,B);
  //R��ǿ��������İ뾶���������Ρ�
  int R=100;
  Color colorTemp,color2;
  Color color;
  gcDrawImage(&image, Rect(0, 0, Width, Height));
  //���η���ÿ������
  for(int x=0;x<Width;x++)
  {
    for( int y=0; y<Height;y++)
    {
      Point tmp= Point(x,y);
      //�������λ�ڡ����Ρ�֮��
      if(fDistance(tmp,Center)<R)
      {
        image.GetPixel(x, y, &color);
        int r,g,b;
        //���ݸõ����ǿ�����ĵ�ľ��룬�ֱ���RGBֵ����
        //220:�������ӳ�������ֵԽ�󣬹�����Խǿ
        float tmp_r=220.0f*(1.0f-fDistance(tmp,Center)/R);
        
        r=color.GetRed()+(int)tmp_r;
        r=max(0,min(r,255));
        
        g=color.GetGreen()+(int)tmp_r;
        g=max(0,min(g,255));

        b=color.GetBlue()+(int)tmp_r;
        b=max(0,min(b,255));
    
        colorTemp = Color(255,(int)r,(int)g,(int)b);
        //�������������ֵ��д��λͼ
        image.SetPixel(x, y, colorTemp);
      }
    }
    //��̬�����˾�Ч��ͼ
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }
}

void BlurAndSharpen()
{
  Graphics &graphics=GetGraphics();
  gcClear(ColorWhite);
  gcScaleTransform(0.8f,0.8f);

  Bitmap image(L"snike.bmp");

  int Width = image.GetWidth();
  int Height = image.GetHeight();
  //image2:�����񻯴���
  Bitmap *image2=image.Clone(0,0,Width,Height,PixelFormatDontCare);

  Color colorTemp;
  Color color[3][3];
  //����ԭͼ
  gcDrawImage(&image, Rect(0, 0, Width, Height));

  for(int i=1;i<Width-2;i++)
  {
    for( int j=1; j<Height-2;j++)
    {
      //������Χ9�����RGBֵ
      image.GetPixel(i-1, j-1, &color[0][0]);
      image.GetPixel(i-1, j, &color[0][1]);
      image.GetPixel(i-1, j+1, &color[0][2]);

      image.GetPixel(i, j-1, &color[1][0]);
      image.GetPixel(i, j, &color[1][1]);
      image.GetPixel(i, j+1, &color[1][2]);

      image.GetPixel(i+1, j-1, &color[2][0]);
      image.GetPixel(i+1, j, &color[2][1]);
      image.GetPixel(i+1, j+1, &color[2][2]);

      int rSum=0;
      int gSum=0;
      int bSum=0;
      //�ֱ������Χ9�����R��G��B֮��
      for(int n=0;n<3;n++)
        for(int nn=0;nn<3;nn++)
        {
          rSum+=color[n][nn].GetR();  
          gSum+=color[n][nn].GetG();  
          bSum+=color[n][nn].GetB();  
        }
      //��RGB��ƽ��ֵ��Ϊ��ǰ���RGBֵ
      colorTemp = Color(255,(int)(rSum/9),(int)(gSum/9),(int)(bSum/9));
      //��������RGBֵ��д��λͼ
      image.SetPixel(i, j,colorTemp);
    }
    //���ƾ���ƽ�������Ч��ͼ
    gcDrawImage(&image, Rect(Width, 0, Width, Height));
  }

  //�����񻯴���
  Color colorLeft,colornow;
  //����dep����ϵ������ֵԽ����Ч��Խ����
  float dep=0.550f;
  for(i=1;i<Width-1; i++)
  {
    for( int j=1;j<Height-1; j++)
    {
      image2->GetPixel(i, j, &colornow);  
      image2->GetPixel(i-1, j-1, &colorLeft);  

      float r=colornow.GetR()+(colornow.GetR()
        -colorLeft.GetR()*dep);
      r=min(255,max(0,r));

      float g=colornow.GetG()+(colornow.GetG() -colorLeft.GetG()*dep);
      g=min(255,max(0,g));
      float b=colornow.GetB()+(colornow.GetB()-colorLeft.GetB()*dep);
      b=min(255,max(0,b));

      colorTemp = Color(255,(int)r,(int)g,(int)b);

      //��������RGBֵ��д��λͼ
      image2->SetPixel(i, j, colorTemp);
    }

    gcDrawImage(image2, Rect(Width*2, 0, Width, Height));
  }
  delete image2;
}


void ShowAllEncoderParameters(ImageCodecInfo*, CString &msg);
HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars);
HRESULT ValueTypeFromULONG(ULONG index, WCHAR* strValueType, UINT maxChars);

void ShowAllEncoder()
{
   UINT  num;        // Number of image encoders
   UINT  size;       // Size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo;

   // How many encoders are there?
   // How big (in bytes) is the array of all ImageCodecInfo obects?
   GetImageEncodersSize(&num, &size);

   // Create a buffer large enough to hold the array of ImageCodecInfo
   // objects that will be returned by GetImageEncoders.
   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

   // GetImageEncoders creates an array of ImageCodecInfo objects
   // and copies that array into a previously allocated buffer. 
   // The third argument, imageCodecInfos, is a pointer to that buffer. 
   GetImageEncoders(num, size, pImageCodecInfo);
  
   // For each ImageCodecInfo object in the array, show all parameters.
   for(UINT j = 0; j < num; ++j)
   {
    CString msg;
    ShowAllEncoderParameters(&(pImageCodecInfo[j]), msg);
    AfxMessageBox(msg);
   }

   free(pImageCodecInfo);
}


VOID ShowAllEncoderParameters(ImageCodecInfo* pImageCodecInfo, CString &msg)
{
   CONST MAX_CATEGORY_LENGTH = 50;
   CONST MAX_VALUE_TYPE_LENGTH = 50;
   WCHAR strParameterCategory[MAX_CATEGORY_LENGTH] = L"";
   WCHAR strValueType[MAX_VALUE_TYPE_LENGTH] = L"";

  CString s(pImageCodecInfo->MimeType);
  s+="\n";
  msg+=s;

   // Create a Bitmap (inherited from Image) object so that we can call
   // GetParameterListSize and GetParameterList.
   Bitmap bitmap(1, 1);

   // How big (in bytes) is the encoder's parameter list?
   UINT listSize = 0; 
   listSize = bitmap.GetEncoderParameterListSize(&pImageCodecInfo->Clsid);
   s.Format("  The parameter list requires %d bytes.\n", listSize);
  msg+=s;

   if(listSize == 0)
      return;

   // Allocate a buffer large enough to hold the parameter list.
   EncoderParameters* pEncoderParameters = NULL;
   pEncoderParameters = (EncoderParameters*)malloc(listSize);

   if(pEncoderParameters == NULL)
      return;

   // Get the parameter list for the encoder.
   bitmap.GetEncoderParameterList(
      &pImageCodecInfo->Clsid, listSize, pEncoderParameters);

   // pEncoderParameters points to an EncoderParameters object, which
   // has a Count member and an array of EncoderParameter objects.
   // How many EncoderParameter objects are in the array?
   s.Format("  There are %d EncoderParameter objects in the array.\n", 
      pEncoderParameters->Count);
  msg+=s;

   // For each EncoderParameter object in the array, list the
   // parameter category, data type, and number of values.
   for(UINT k = 0; k < pEncoderParameters->Count; ++k)
   {
      EncoderParameterCategoryFromGUID(
         pEncoderParameters->Parameter[k].Guid, strParameterCategory, MAX_CATEGORY_LENGTH);

      ValueTypeFromULONG(
         pEncoderParameters->Parameter[k].Type, strValueType, MAX_VALUE_TYPE_LENGTH);

      s.Format("    Parameter[%d]\n", k); msg+=s;
      s.Format("      The category is %s.\n", CString(strParameterCategory)); msg+=s;
      s.Format("      The data type is %s.\n", strValueType); msg+=s;

      s.Format("      The number of values is %d.\n",
      pEncoderParameters->Parameter[k].NumberOfValues); msg+=s;
   } // for

   free(pEncoderParameters);
} // ShowAllEncoderParameters


HRESULT MyStringCchCopy(LPWSTR pszDest, size_t cchDest, LPCWSTR pszSrc)
{
  return wcsncpy(pszDest, pszSrc, cchDest)? Ok: E_FAIL;
}

HRESULT EncoderParameterCategoryFromGUID(GUID guid, WCHAR* category, UINT maxChars)
{
   HRESULT hr = E_FAIL;

   if(guid == EncoderCompression)
      hr = MyStringCchCopy(category, maxChars, L"Compression");
   else if(guid == EncoderColorDepth)
      hr = MyStringCchCopy(category, maxChars, L"ColorDepth");
   else if(guid == EncoderScanMethod)
      hr = MyStringCchCopy(category, maxChars, L"ScanMethod");
   else if(guid == EncoderVersion)
      hr = MyStringCchCopy(category, maxChars, L"Version");
   else if(guid == EncoderRenderMethod)
      hr = MyStringCchCopy(category, maxChars, L"RenderMethod");
   else if(guid == EncoderQuality)
      hr = MyStringCchCopy(category, maxChars, L"Quality");
   else if(guid == EncoderTransformation)
      hr = MyStringCchCopy(category, maxChars, L"Transformation");
   else if(guid == EncoderLuminanceTable)
      hr = MyStringCchCopy(category, maxChars, L"LuminanceTable");
   else if(guid == EncoderChrominanceTable)
      hr = MyStringCchCopy(category, maxChars, L"ChrominanceTable");
   else if(guid == EncoderSaveFlag)
      hr = MyStringCchCopy(category, maxChars, L"SaveFlag");
   else
      hr = MyStringCchCopy(category, maxChars, L"Unknown category");

   return hr;
} // EncoderParameterCategoryFromGUID


HRESULT ValueTypeFromULONG(ULONG index, WCHAR* strValueType, UINT maxChars)
{
   HRESULT hr = E_FAIL;

   WCHAR* valueTypes[] = {
      L"Nothing",                  // 0
      L"ValueTypeByte",            // 1
      L"ValueTypeASCII",           // 2
      L"ValueTypeShort",           // 3
      L"ValueTypeLong",            // 4
      L"ValueTypeRational",        // 5
      L"ValueTypeLongRange",       // 6
      L"ValueTypeUndefined",       // 7
      L"ValueTypeRationalRange"};  // 8

   hr = MyStringCchCopy(strValueType, maxChars, valueTypes[index]);
   return hr;

} // ValueTypeFromULONG

#endif
