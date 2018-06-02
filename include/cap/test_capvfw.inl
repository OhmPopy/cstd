

typedef unsigned char uchar;

// ͼƬ��ת����֧��90�ı���
// ����ͼƬ����img  ��h ��w ÿ������ռ���ֽ���al ͨ����ai
// ���ͼƬ����rgb2 ��ph ��pw ÿ������ռ���ֽ���pal ͨ����������ͼƬ��ͬ
// istrans=1 ��ת90�� istrans=2 ��ת180�� istrans=3 ��ת270��
static int imtrans(int h, int w, const uchar* img, int al, int ai, int* ph, int* pw, uchar* rgb2, int* pal, int istrans, int isflop)
{
  int i, j;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      const uchar* sss;
      uchar* ddd;
      int i1 = i, j1 = j, i2 = i, j2 = j, ll = w;

      if (1 == istrans) {
        i1 = i, j1 = j, i2 = j, j2 = h - i - 1, ll = h;
        sss = img + (i * w + j) * ai;
        ddd = rgb2 + (j * h + (h - i - 1)) * ai;
      }
      else if (2 == istrans) {
        i1 = i, j1 = j, i2 = h - i - 1, j2 = j, ll = w;
        sss = img + (i * w + j) * ai;
        ddd = rgb2 + ((h - i - 1) * w + j) * ai;
      }
      else if (3 == istrans) {
        i1 = i, j1 = w - j - 1, i2 = j, j2 = i, ll = h;
        sss = img + (i * w + w - j - 1) * ai;
        ddd = rgb2 + (j * h + i) * ai;
      }

      if (isflop) {
        j2 = ll - 1 - j2;
      }

      sss = img + (i1 * w + j1) * ai;
      ddd = rgb2 + (i2 * ll + j2) * ai;
      *ddd++ = *sss++;
      *ddd++ = *sss++;
      *ddd++ = *sss++;
    }
  }

  img = rgb2;

  if (istrans % 2) {
    *pw = h, *ph = w, *pal = h * ai;
  }
  else {
    *pw = w, *ph = h, *pal = w * ai;
  }

  return 0;
}


int test_capvfw()
{
  char buf[256];
  static const camera_param_t cp = { 720, 576, 15};//��Ƶ���� 720��576 15֡ÿ��
  capcam* cap = cam_start(0, &cp);    //������Ƶ
  int trans_flag = 1;//��ת����
  char ch;

  if (cap) {
    int h, w, al, ai, bl = 0, bi = 0;
    unsigned char* a;
    unsigned char* img = 0;

    for (;;) {
      char buf[256];
      cam_QueryFrame(cap, h, w, a, al, ai);  //����Ƶ���л�ȡһ֡a

      if (NULL == img) {   //����ռ�img
        img = MALLOC(unsigned char, h * w * 3);
      }

      // ��תͼƬ
      imtrans(h, w, a, al, ai, &h, &w, img, &al, trans_flag, 0);
      cvShowImage("cam", h, w, img, al, ai);  // ��ʾͼƬimg
      ch = cvWaitKey(10);   //��ȡ�û��������ӳ�10ms

      if ('q' == ch) {
        break;
      }

      if ('t' == ch) {
        trans_flag++;
        trans_flag %= 4;
      }
    }

    SAFEFREE(img);   //�ͷ�ͼƬ�ڴ�
    cam_stop(cap);   //ֹͣ��Ƶ
  }

  return 0;
}
