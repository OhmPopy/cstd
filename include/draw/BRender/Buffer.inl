//�������������ɫ������
//��ѯ����
//���ñ�����ɫ
//��ָ������ɫ����Ϊ����ɫ
//��һ��ɫ(r, g, b, a)
void SetBkColor2(HDDC pCB, DWORD clr)
{
  int i;
  DWORD dwScanLength;
  DWORD* pbyColor = pCB->data;

  for (i = 0; i < pCB->w; i++) {
    *pbyColor++ = clr;
  }

  //�ڴ�֮�俽��, ����ʣ�µ� (pCB->nHeight - 1)��.
  dwScanLength = 4 * (DWORD)pCB->w;

  for (i = 1; i < pCB->h; i++) {
    //ָ��һ�п�ʼ��λ��
    pbyColor = pCB->data + (i * dwScanLength);
    //�ӵ�һ�п������� i ��:
    memcpy(pbyColor,  pCB->data,  dwScanLength);
  }
}
//����Ϊһ��ָ���ı���ͼ��
//����ͼ�����������ʽ��ʾ��, ������������ͼ��
//ʹ��ʱ�ľ���:ͼ���������ɫ�������ߴ���ȫ��ͬ!
void SetBkImage(HDDC pCB, const BYTE* pbyImage,  int nWidth,  int nHeight)
{
  ASSERT(pbyImage);

  if ((nWidth != pCB->w) || (nHeight != pCB->h)) {
    zb_error("ͼ��ߴ�����ɫ��������С��ƥ��");
    return;
  }

  CopyMemory(pCB->data,  pbyImage,  pCB->w * pCB->h * 4);
}
//����һ�������ɫ
//����z-buffer�㷨������ɫ����������Ҫ����
void CBSetPixel(HDDC pCB, int x,  int y,  float r,  float g,  float b , float a)
{
  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE byRed = (BYTE)(r * 255.0f);
    BYTE byGreen = (BYTE)(g * 255.0f);
    BYTE byBlue = (BYTE)(b * 255.0f);
    BYTE byAlpha = (BYTE)(a * 255.0f);
    BYTE* pbyColor = (BYTE*)((BYTE*)pCB->data + dwIndex);
    *pbyColor++ = byBlue;
    *pbyColor++ = byGreen;
    *pbyColor++ = byRed;
    *pbyColor++ = byAlpha;
  }
}
void CBSetPixel1(HDDC pCB, int x,  int y,  BYTE r,  BYTE g,  BYTE b ,  BYTE a)
{
  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE* pbyColor = (BYTE*)pCB->data + dwIndex;
    //�����̡��졢Alpha
    *pbyColor++ = b;
    *pbyColor++ = g;
    *pbyColor++ = r;
    *pbyColor++ = a;
  }
}
//��ȡ��ɫ
PIXELCOLORRGBA CBGetPixel(HDDC pCB, int x,  int y)
{
  PIXELCOLORRGBA rgba = {0,  0,  0,  0};

  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return rgba;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE* pbyColor = (BYTE*)pCB->data + dwIndex;
    //�����̡��졢Alpha
    rgba.blue = *pbyColor++;
    rgba.green = *pbyColor++;
    rgba.red = *pbyColor++;
    rgba.alpha = *pbyColor++;
    return rgba;
  }
}
//����ɫ�������е�����ֱ�Ӵ���һ��DDB����, ��HBITMAP����
HBITMAP CBCreateDdb(HDDC pCB, HDC hDC)
{
  //��һ��, ����һ��BITMAPV4HEADER�ṹ
  HBITMAP hBitmap;
  //ָ���������ݵ�ָ��,
  BITMAPV4HEADER pbmih[1];
  pbmih->bV4Size = sizeof(BITMAPV4HEADER);  //108
  pbmih->bV4Width = (LONG)pCB->w;
  pbmih->bV4Height = (LONG)(-pCB->h);
  pbmih->bV4Planes = 1;
  pbmih->bV4BitCount = 32;
  pbmih->bV4V4Compression = BI_RGB;
  pbmih->bV4SizeImage = 0;
  pbmih->bV4XPelsPerMeter = 0;
  pbmih->bV4YPelsPerMeter = 0;
  pbmih->bV4ClrUsed = 0;
  pbmih->bV4ClrImportant = 0;
  //�ڶ���, ����һ��DDB
  hBitmap = CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pbmih, CBM_INIT, pCB->data, (BITMAPINFO*)pbmih, DIB_RGB_COLORS);
  return hBitmap;
}
//����ɫ�����������ݽ�����ǰ̨(�������ɫ������)
void CBSwapBuffer(HDDC pCB, HDC hDC)
{
  HDC memDC = CreateCompatibleDC(hDC);
  HBITMAP hBitmap = CBCreateDdb(pCB, hDC);
  HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
  BitBlt(hDC, 0, 0, pCB->w, pCB->h, memDC, 0, 0, SRCCOPY);
  DeleteObject(hBitmap);
  SelectObject(memDC, hOldBitmap);
  DeleteDC(memDC);
}//32λ��Ȼ���

#define newZBuffer(nWidth, nHeight) newMatC(nWidth, nHeight, T_FLOAT, sizeof(float))
//��ʼ�����
void initAllDepth(HMC pZB, float fDepth)
{
  int i, nSize = pZB->w * pZB->h;
  ASSERT(pZB->t == T_FLOAT);

  for (i = 0; i <  nSize; i++) {
    ((float*)(pZB->data))[i] = fDepth;
  }
}
//�������ֵ
void SetDepth(HMC pZB, int x,  int y,  float fDepth)
{
  ASSERT(pZB->t == T_FLOAT);

  if ((x >= 0) && (y >= 0) && (x < pZB->w) && (y < pZB->h)) {
    ((float*)(pZB->data))[y * pZB->w + x] = fDepth;
  }
}
//��ȡ���ֵ
#define GetDepth(pZB, x,  y)  (((x >= 0) && (y >= 0) && (x < pZB->w) && (y < pZB->h)) ? _PixelT(pZB, x, y, float) : -1.0f)

//���۲�����ϵ�µĵ�任����Ļ����ϵ
//��ʵ��, ����任�ʺ����ӵ���Z�����ϵ�����
//vertex----�۲�����ϵ�µĵ�
//������, �������˼�
//��ʵ��, Ӧ�ÿ���
//��һ, �ӵ�����������ϵԭ��ľ��� �� �Ӿ� ֮��Ĺ�ϵ;
//�ڶ�, ���塢��Ļ���ӵ�֮���λ�ù�ϵ
//���ǹ涨:
//��һ, ֻ�ܿ����ӵ�ǰ��������, ���� ze Ӧ�ô��� 0.0f
//    ����, �ӵ�λ�ڳ����������֮��, ����������һҪ��
//�ڶ�, ��Ļ���ӵ�ǰ��.
//���ǲ��ǿ���һ��ͼ��ϵͳ!
//����ƽ��ֱ������ϵ��Ϊ����ֱ������ϵ
VECTOR3D CViewFinder_PerspectiveH(VIEWFINDER* pVF, VECTOR3D vertex)
{
  VECTOR3D pt = vertex;
  float xe,  ye,  ze, w = vertex.w;

  //����һ������Զ��
  if (ABS(w) <= 0.00001f) {
    xe = vertex.x;
    ye = vertex.y;
    ze = 1000000.0f;
  }
  else {
    xe = vertex.x / w;
    ye = vertex.y / w;
    ze = vertex.z / w;
  }

  //����ze = 0.0f�����, ����ֻ���򻯴���:
  if (ABS(ze) < 0.00001f) {
    ze = 1.0f;
  }

  //��Ļ�ϵĵ�
  pt.x = ((xe * pVF->fDistView) / ze * pVF->xScale + pVF->ptCenter.x);
  pt.y = ((-ye * pVF->fDistView) / ze * pVF->yScale + pVF->ptCenter.y);
  return pt;
}
