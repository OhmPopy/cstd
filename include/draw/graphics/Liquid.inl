//��ѧ�����壨Liquid����VC�汾��

//�������ԭ���ǲ��� HTML 5 Ч����һ����ʾ���򣬶�̬Ч������Ư�������ڣ�������ʵĳ��� krissi ��ֲ���� vc �ϡ�

///////////////////////////////////////////////////
// �������ƣ�����(Liquid)
// ���뻷����Visual C++ 6.0 / 2010��EasyX 20120404(beta)
// ԭ �� Ʒ��http://spielzeugz.de/html5/liquid-particles.html (HTML5)
// ��ֲ���ߣ�krissi <zh@easyx.cn>
// ����޸ģ�2012-4-5
//

#define NUM_MOVERS  600     // С������
#define FRICTION  0.96f   // Ħ����


// ����С��ṹ
typedef struct Mover {
  COLORREF  color;      // ��ɫ
  double   x,  y;      // ����
  double   vX, vY;     // �ٶ�
} Mover;

// ����ȫ�ֱ���
Mover movers[NUM_MOVERS];     // С������
int   mouseX,   mouseY;     // ��ǰ�������
int   mouseVX,  mouseVY;    // ����ٶ�
int   prevMouseX, prevMouseY;   // �ϴ��������
BOOL  isMouseDown;        // �������Ƿ���


// ��ʼ��
int init_Liquid()
{
  int i;
  img_t im[1] = {0};
  GetImageBuffer(im);

  // ��ʼ��С������
  for (i = 0; i < NUM_MOVERS; i++) {
    movers[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
    movers[i].x   = im->w * 0.5;
    movers[i].y   = im->h * 0.5;
    movers[i].vX  = (double)(cos((double)(i))) * (rand() % 34);
    movers[i].vY  = (double)(sin((double)(i))) * (rand() % 34);
  }

  // ��ʼ��������
  mouseX = prevMouseX = im->w / 2;
  mouseY = prevMouseY = im->h / 2;

  // �����������
  srand((unsigned int)time(NULL));
  return 0;
}

// ���ƶ���(һ֡)
int run()
{
  int i;
  double toDist, stirDist, blowDist;
  img_t im[1] = {0};
  DWORD*  pBuffer;
  // ��ȡ�Դ�ָ��
  pBuffer = GetImageBuffer(im);

  // ȫ���䰵 50%
  for (i = im->w * im->h - 1; i >= 0; i--) {
    if (pBuffer[i] != 0) {
      pBuffer[i] = _RGB(GetRV(pBuffer[i]) >> 1, GetGV(pBuffer[i]) >> 1, GetBV(pBuffer[i]) >> 1);
    }
  }

  mouseVX    = mouseX - prevMouseX;
  mouseVY    = mouseY - prevMouseY;
  prevMouseX = mouseX;
  prevMouseY = mouseY;

  toDist   = im->w * 0.86f;
  stirDist = im->w * 0.125f;
  blowDist = im->w * 0.5f;

  for (i = 0; i < NUM_MOVERS; i++) {
    double avgVX, avgVY, avgV, sc, x, y, vX, vY, dX, dY, d, nextX, nextY;
    x  = movers[i].x;
    y  = movers[i].y;
    vX = movers[i].vX;
    vY = movers[i].vY;

    dX = x - mouseX;
    dY = y - mouseY;
    d  = (double)sqrt(dX * dX + dY * dY);
    dX = d ? dX / d : 0;
    dY = d ? dY / d : 0;

    if (isMouseDown && d < blowDist) {
      double blowAcc = (1 - (d / blowDist)) * 14;
      vX += dX * blowAcc + 0.5f - (double)(rand()) / RAND_MAX;
      vY += dY * blowAcc + 0.5f - (double)(rand()) / RAND_MAX;
    }

    if (d < toDist) {
      double toAcc = (1 - (d / toDist)) * im->w * 0.0014f;
      vX -= dX * toAcc;
      vY -= dY * toAcc;
    }

    if (d < stirDist) {
      double mAcc = (1 - (d / stirDist)) * im->w * 0.00026f;
      vX += mouseVX * mAcc;
      vY += mouseVY * mAcc;
    }

    vX *= FRICTION;
    vY *= FRICTION;

    avgVX = fabs(vX);
    avgVY = fabs(vY);
    avgV  = (avgVX + avgVY) * 0.5f;

    if (avgVX < 0.1) {
      vX *= (double)(rand()) / RAND_MAX * 3;
    }

    if (avgVY < 0.1) {
      vY *= (double)(rand()) / RAND_MAX * 3;
    }

    sc = avgV * 0.45f;
    sc = max(min(sc, 3.5f), 0.4f);

    nextX = x + vX;
    nextY = y + vY;

    if (nextX > im->w) {
      nextX = im->w;
      vX *= -1;
    }
    else if (nextX < 0)   {
      nextX = 0;
      vX *= -1;
    }

    if (nextY > im->h) {
      nextY = im->h;
      vY *= -1;
    }
    else if (nextY < 0)   {
      nextY = 0;
      vY *= -1;
    }

    movers[i].vX = vX;
    movers[i].vY = vY;
    movers[i].x  = nextX;
    movers[i].y  = nextY;

    // ��С��
    setcolor(movers[i].color);
    setfillcolor(movers[i].color);
    fillcircle((nextX + 0.5), (nextY + 0.5), (sc + 0.5));
  }

  return 0;
}


// ������
int test_liquid()
{
  // �����Ϣ����
  MOUSEMSG m;

  // ������ͼ����
  initgraph(640, 480, 0);
  // ��������ͼģʽ
  BeginBatchDraw();

  // ��ʼ��
  init_Liquid();

  while (1) {
    // ���������Ϣ
    if (GetMouseMsg(&m, 20)) {
      switch (m.uMsg) {
      case WM_MOUSEMOVE:
        mouseX = m.x;
        mouseY = m.y;
        break;

      case WM_LBUTTONDOWN:
        isMouseDown = TRUE;
        break;

      case WM_LBUTTONUP:
        isMouseDown = FALSE;
        break;
      }
    }

    // ���ƶ���
    run();

    // ��ʾ����Ļ�������
    FlushBatchDraw();
  }

  // �ر�
  EndBatchDraw();
  closegraph();
}