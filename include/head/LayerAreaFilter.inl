
int Centroid(int h, int w, int* A, int al, POINT* pt, int maxptlen)
{
  int i, j, k, n = 0;
  int ptn[256];
  assert(maxptlen <= 256);
  memset(pt, 0, sizeof(POINT)*maxptlen);
  memset(ptn, 0, sizeof(int) * 256);

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      k = A[i * al + j] - 1;

      if (k >= 0 && k < maxptlen) {
        ptn[k]++;
        pt[k].x += j;
        pt[k].y += i;
      }

      if (k + 1 > n) {
        n = k + 1;
      }
    }
  }

  for (k = 0; k < n; ++k) {
    pt[k].x /= ptn[k] + 1;
    pt[k].y /= ptn[k] + 1;
  }

  return n;
}

// g:ԭʼ����ͼƬ
// LayerThresh������˲���ֵ
// AreaThresh������˲���ֵ
// verson������ѡ���Ƿ����g�ĸ�ʴ
//         Y ��ʾ���и�ʴ��N ��ʾ�����и�ʴ
// ������˲����������˲�����������ͨ����
// edit time: 2013.07.02
// buflen=9*h*w
int LayerAreaFilter(int h, int w, const DEPTH_TYPE* g, int gl, int LayerThresh, int AreaThresh, int verson,
    unsigned char* FAI, int FAI_step, POINT* pt, int maxptlen, int Order_verson, unsigned char* buf, int buflen)
{
  int i, j, kkk = 6, n;
  BUFUSEBEGIN(buf, buflen);
  unsigned char* Bg = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned char* IME = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  unsigned char* Tg_bw = (unsigned char*)BUFMALLOC(unsigned char, h * w);
  DEPTH_TYPE* Tg = (DEPTH_TYPE*)BUFMALLOC(DEPTH_TYPE, h * w);
  int* label = (int*)BUFMALLOC(int, h * w);
  int areas[100];
  unsigned char map[400];
  int LayThresh = LayerThresh; // �����ֵ
  BUFBOUNDCHECK();

  if (verson == 'Y') { // �� g ���и�ʴ
    // ��ʴ��Ķ�ֵͼ
    if (1 == Order_verson) {
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          Tg[i * w + j] = g[i * gl + j];

          if (Tg[i * w + j] < LayThresh) { // С��һ������Ŀ��ȥ��
            Tg[i * w + j] = 0;
          }

          Bg[i * w + j] = Tg[i * w + j] > 0 ? 255 : 0;
        }
      }

      imdilate_rect(h, w, Bg, w, IME, w, kkk, kkk, kkk / 2, kkk / 2, 0);

      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          Tg[i * w + j] = IME[i * w + j] > 0 ? g[i * gl + j] : 0;
        }
      }
    }

    if (2 == Order_verson) {
      // �� g ���ж�ֵ��
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          Bg[i * w + j] = g[i * gl + j] > 0 ? 255 : 0;
        }
      }

      imdilate_rect(h, w, Bg, w, IME, w, kkk, kkk, kkk / 2, kkk / 2, 0);

      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          Tg[i * w + j] = IME[i * w + j] > 0 ? g[i * gl + j] : 0;

          if (Tg[i * w + j] < LayThresh) { // С��һ������Ŀ��ȥ��
            Tg[i * w + j] = 0;
          }
        }
      }
    }
  }
  else {
    ASSERT(verson == 'N'); // ���� g ���и�ʴ

    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        Tg[i * w + j] = g[i * gl + j];

        // ��g����Ԫ��ֵ��С�˲�
        if (Tg[i * w + j] < LayThresh) { // С��һ������Ŀ��ȥ��
          Tg[i * w + j] = 0;
        }
      }
    }
  }

  cvShowImage_u16("LayerFilterI", h, w, Tg, w, 1);

  //cvShowImage_u16( "Tg", h, w, Tg, w, 1 );
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      Tg_bw[i * w + j] = Tg[i * w + j] > 0 ? 255 : 0;
    }
  }

  memset(map, 255, sizeof(map));
  map[0] = 0;
  //cvShowImage( "Tg_bw", h, w, Tg_bw, w, 1 );
  n = bwlabel(h, w, Tg_bw, w, label, w, 4, areas, 100, AreaThresh, map, FAI, FAI_step);
  n = Centroid(h, w, label, w, pt, MIN(maxptlen, n));
  //free(Bg);
  //free(IME);
  //free(Tg);
  //free(label);
  //free(Tg_bw);
  BUFUSEEND();
  return n;
}
