void CirclePoints(HDDC hDC, int x0, int y0, int x, int y, COLORREF color)
{
  //�е㷨��Բ
  _SetPixel(hDC, x + x0, y + y0, color);
  _SetPixel(hDC, y + x0, x + y0, color);
  _SetPixel(hDC, - y + x0, x + y0, color);
  _SetPixel(hDC, - x + x0, y + y0, color);
  _SetPixel(hDC, y + x0, - x + y0, color);
  _SetPixel(hDC, x + x0, - y + y0, color);
  _SetPixel(hDC, - x + x0, - y + y0, color);
  _SetPixel(hDC, - y + x0, - x + y0, color);
}
void MidPointCircle(HDDC hDC, int x0, int y0, int x1, int y1, COLORREF color)
{
  //�е㷨���˷�֮һԲ
  int x, y, d, radius, m;
  m = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
  radius = (int)pow(m, 0.5); //Բ�뾶
  y = radius;
  x = 0;
  d = 5 - 4 * radius;
  CirclePoints(hDC, x0, y0, x, y, color);

  while (y > x) {
    if (d <= 0) {
      d += 8 * x + 12;
    }
    else {
      d += 8 * (x - y) + 20;
      y--;
    }

    x++;
    CirclePoints(hDC, x0, y0, x, y, color);
  }
}
//����Ϊ���߷�������δ���
int maxY(int y1, int y2)
{
  if (y1 > y2) {
    return y1;
  }
  else {
    return y2;
  }
}
int minY(int y1, int y2)
{
  if (y1 < y2) {
    return y1;
  }
  else {
    return y2;
  }
}
void MultSheXian(HDDC hDC, POINT* poly, int poly_len, RECT* rec, COLORREF color)
{
  //���߷��������
  int nOddEven;
  int row, col, i;
  double x;

  for (row = rec->top; row < rec->bottom; row++) {
    for (col = rec->left; col < rec->right; col++) {
      //��ǰ�������Ϊ(col,row)
      nOddEven = 0;

      for (i = 0; i < poly_len - 1; i++) {
        if ((row >= minY(poly[i].y, poly[i + 1].y)) && (row < maxY(poly[i].y, poly[i + 1].y))) {
          x = poly[i].x + (row - poly[i].y) * 1.0 * (poly[i + 1].x - poly[i].x) / (poly[i + 1].y - poly[i].y);

          if (x > col) {
            nOddEven++;
          }
        }
      }

      if (nOddEven % 2 == 1) {
        _SetPixel(hDC, col, row, color);
      }
    }
  }
}
int line_width = 8;
int runFlag = 1;
int line_space = 1;
int line_spacelength = 1;
//���߶���ȫ���ɼ�ʱ������FALSE,���򷵻�TURE
int ClipT(float q, float d, float* t0, float* t1)
{
  float r;

  if (q < 0) {
    r = d / q;

    if (r > *t1) {
      return (FALSE);
    }
    else if (r > *t0) {
      *t0 = r;
      return (TRUE);
    }
  }
  else if (q > 0) {
    r = d / q;

    if (r < *t0) {
      return (FALSE);
    }
    else if (r < *t1) {
      *t1 = r;
      return (TRUE);
    }
  }
  else if (d < 0) {
    return (FALSE);
  }

  return (TRUE);
}
//����Ϊֱ�߲ü�֮���Ѷ�������
void LiangBarskyLineClip(HDDC hDC, float x0, float y0, float x1, float y1, RECT* rect1, DWORD clr)
{
  //���Ѷ���ֱ�߲ü�
  float deltax, deltay, t0, t1;
  int _x0, _y0;
  t0 = 0, t1 = 1;
  deltax = x1 - x0;

  if (ClipT(- deltax, x0 - rect1->left, &t0, &t1)) {
    if (ClipT(deltax, rect1->right - x0, &t0, &t1)) {
      deltay = y1 - y0;

      if (ClipT(- deltay, y0 - rect1->top, &t0, &t1)) {
        if (ClipT(deltay, rect1->bottom - y0, &t0, &t1)) {
          TMoveTo(hDC, (int)(x0 + t0 * deltax), (int)(y0 + t0 * deltay));
          TLineTo(hDC, (int)(x0 + t1 * deltax), (int)(y0 + t1 * deltay), clr);
          return ;
        }
      }
    }
  }
}
//����Ϊ����������
void FillRectangle(HDDC hDC, RECT rect2, COLORREF color) //�������
{
  int x, y;

  for (y = rect2.top; y <= rect2.bottom; y++)
    for (x = rect2.left; x <= rect2.right; x++) {
      _SetPixel(hDC, x, y, color);
    }
}
typedef struct Vertex {
  int x, y;
} Vertex;
typedef POINT Edge[2];
//����Ϊ��߲ü�����
void Intersect(Vertex* s, Vertex* p, Edge clipBoundary, Vertex* I)
{
  //����������ü���clipBoundary�Ľ���
  if (clipBoundary[0].y == clipBoundary[1].y) {
    I->y = clipBoundary[0].y;
    I->x = s->x + (clipBoundary[0].y - s->y) * (p->x - s->x) / (p->y - s->y);
  }
  else {
    I->x = clipBoundary[0].x;
    I->y = s->y + (clipBoundary[0].x - s->x) * (p->y - s->y) / (p->x - s->x);
  }
}
POINT clipBoundary[2];
POINT outVertexArray[2];
int Inside(Vertex* testVertex)
{
  //���Զ���textVertex��ü��ߵ������ϵ
  if (clipBoundary[1].x > clipBoundary[0].x) {
    if (testVertex->y >= clipBoundary[0].y) {
      return TRUE;
    }
  }
  else if (clipBoundary[1].x < clipBoundary[0].x) {
    if (testVertex->y <= clipBoundary[0].y) {
      return TRUE;
    }
  }
  else if (clipBoundary[1].y > clipBoundary[0].y) {
    if (testVertex->x <= clipBoundary[0].x) {
      return TRUE;
    }
  }
  else if (clipBoundary[1].y < clipBoundary[0].y) {
    if (testVertex->x >= clipBoundary[0].x) {
      return TRUE;
    }
  }

  return FALSE;
}
int outLength;
void Output(Vertex* newVertex)
{
  //��newVertex���뵽�������ζ����outVertexArray��
  outVertexArray[outLength].x = newVertex->x;
  outVertexArray[outLength].y = newVertex->y;
  outLength++;
}
Vertex inVertexArray[100];
int inLength;
void SutherlandHodgmanPolygonClip()
{
  // inVertexArrayΪ�����������飬outVertexArrayΪ�ü�����ζ������飬
  // clipBoundaryΪ�ü��ߣ���һ���߲ü������
  Vertex* s, *p, I;
  int j;
  outLength = 0;
  s = &(inVertexArray[inLength - 1]);

  for (j = 0; j < inLength; j++) {
    p = &(inVertexArray[j]);

    if (Inside(p)) {
      if (Inside(s)) {
        Output(p);
      }
      else {
        Intersect(s, p, clipBoundary, &I);
        Output(&I);
        Output(p);
      }
    }
    else if (Inside(s)) {
      Intersect(s, p, clipBoundary, &I);
      Output(&I);
    }

    s = p;
  }
}
RECT rect1;
RECT rect;
void InputVertexArray1() //����������������Ϊ�´���������
{
  int i;
  inLength = outLength;

  for (i = 0; i < outLength; i++) {
    inVertexArray[i].x = outVertexArray[i].x;
    inVertexArray[i].y = outVertexArray[i].y;
  }
}
void StartSutherClip() //�þ��ο������߷ֱ�ü������
{
  clipBoundary[0].x = (float)rect1.left;
  clipBoundary[0].y = (float)rect1.bottom;
  clipBoundary[1].x = (float)rect1.left;
  clipBoundary[1].y = (float)rect1.top;
  SutherlandHodgmanPolygonClip(); //�þ��ο�����߲ü�
  InputVertexArray1();
  clipBoundary[0].x = (float)rect1.left;
  clipBoundary[0].y = (float)rect1.top;
  clipBoundary[1].x = (float)rect1.right;
  clipBoundary[1].y = (float)rect1.top;
  SutherlandHodgmanPolygonClip(); //�þ��ο��ϱ��߲ü�
  InputVertexArray1();
  clipBoundary[0].x = (float)rect1.right;
  clipBoundary[0].y = (float)rect1.top;
  clipBoundary[1].x = (float)rect1.right;
  clipBoundary[1].y = (float)rect1.bottom;
  SutherlandHodgmanPolygonClip(); //�þ��ο��ұ��߲ü�
  InputVertexArray1();
  clipBoundary[0].x = (float)rect.right;
  clipBoundary[0].y = (float)rect.bottom;
  clipBoundary[1].x = (float)rect.left;
  clipBoundary[1].y = (float)rect.bottom;
  SutherlandHodgmanPolygonClip(); //�þ��ο��±��߲ü�
}
void DrawVertex(HDDC hDC, DWORD clr) //�����ü���Ķ����
{
  int _x0, _y0;
  int count;
  TMoveTo(hDC, (int)outVertexArray[0].x, (int)outVertexArray[0].y);

  for (count = 1; count < outLength; count++) {
    TLineTo(hDC, (int)outVertexArray[count].x, (int)outVertexArray[count].y, clr);
  }

  TLineTo(hDC, (int)outVertexArray[0].x, (int)outVertexArray[0].y, clr);
}
typedef struct OutCode {
  int all, left, top, right, bottom;
} OutCode;
void CompOutCode(float x, float y, RECT* rect, OutCode* outCode) //����˵����
{
  outCode->all = 0;
  outCode->top = outCode->bottom = 0;
  outCode->right = outCode->left = 0;

  if (y < (float)rect->top) {
    outCode->top = 1;
    outCode->all += 1;
  }
  else if (y > (float)rect->bottom) {
    outCode->bottom = 1;
    outCode->all += 2;
  }
  else if (x > (float)rect->right) {
    outCode->right = 1;
    outCode->all += 4;
  }
  else if (x < (float)rect->left) {
    outCode->left = 1;
    outCode->all += 8;
  }
}
POINT winMinPoint;
POINT winMaxPoint;
//����Ϊ���ӷ�������δ���
//����Ϊ���뷨ֱ�߲ü�����
void CohenSutherlandLineClip(HDDC hDC, float x0, float y0, float x1, float y1, RECT* rect, DWORD clr)
{
  //���뷨ֱ�߲ü�
  int accept, done;
  OutCode outCode0, outCode1;
  OutCode* outCodeOut;
  float x, y;
  accept = FALSE;
  done = FALSE;
  CompOutCode(x0, y0, rect, &outCode0);
  CompOutCode(x1, y1, rect, &outCode1);

  do {
    if (outCode0.all == 0 && outCode1.all == 0) {
      //��ȫ�ɼ�
      accept = TRUE;
      done = TRUE;
    }
    else if ((outCode0.all & outCode1.all) != 0)
      //��Ȼ���ɼ�
    {
      done = TRUE;
    }
    else {
      //�����󽻲���
      if (outCode0.all != 0)
        //�ж��ĵ�λ�ڴ�����
      {
        outCodeOut = &outCode0;
      }
      else {
        outCodeOut = &outCode1;
      }

      if (outCodeOut->left) {
        //�߶��봰�������
        y = y0 + (y1 - y0) * (rect->left - x0) / (x1 - x0);
        x = (float)rect->left;
      }
      else if (outCodeOut->top) {
        //�߶��봰���ϱ���
        x = x0 + (x1 - x0) * (rect->top - y0) / (y1 - y0);
        y = (float)rect->top;
      }
      else if (outCodeOut->right) {
        //�߶��봰���ұ���
        y = y0 + (y1 - y0) * (rect->right - x0) / (x1 - x0);
        x = (float)rect->right;
      }
      else if (outCodeOut->bottom) {
        //�߶��봰���±���
        x = x0 + (x1 - x0) * (rect->bottom - y0) / (y1 - y0);
        y = (float)rect->bottom;
      }

      if (outCodeOut->all == outCode0.all) {
        //�ѽ���Ϊ�磬���߶�λ�ڴ��ڱ����ڵ�ֱ�ߵ���ಿ�ֶ�������ʣ�ಿ�ּ����ü�
        x0 = x;
        y0 = y;
        CompOutCode(x0, y0, rect, &outCode0);
      }
      else {
        x1 = x;
        y1 = y;
        CompOutCode(x1, y1, rect, &outCode1);
      }
    }
  }
  while (!done);

  winMinPoint.x = (int)x0;
  winMinPoint.y = (int)y0;
  winMaxPoint.y = (int)y1;
  winMaxPoint.x = (int)x1;

  if (accept) {
    //��ʾ�ɼ�����
    int _x0, _y0;
    TMoveTo(hDC, winMinPoint.x, winMinPoint.y);
    TLineTo(hDC, winMaxPoint.x, winMaxPoint.y, clr);
  }
}

