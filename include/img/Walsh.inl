

// ����:
//   double* f        - ָ��ʱ��ֵ��ָ��
//   double* F        - ָ��Ƶ��ֵ��ָ��
//   r                - 2������
// ˵��:
//   �ú�������ʵ�ֿ����ֶ�ʲ-������任��
VOID WINAPI WALSH(const double* f, double* F, int r)
{
  // �ֶ�ʲ-������任����
  LONG  count;

  // ѭ������
  int   i, j, k;

  // �м����
  int   bfsize, p;

  double* X1, *X2, *X;

  // ��������ֶ�ʲ�任����
  count = 1 << r;

  // �����������������
  X1 = new double[count];
  X2 = new double[count];

  // ��ʱ���д������X1
  memcpy(X1, f, sizeof(double) * count);

  // ��������
  for (k = 0; k < r; k++) {
    for (j = 0; j < 1 << k; j++) {
      bfsize = 1 << (r - k);

      for (i = 0; i < bfsize / 2; i++) {
        p = j * bfsize;
        X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
        X2[i + p + bfsize / 2] = X1[i + p] - X1[i + p + bfsize / 2];
      }
    }

    // ����X1��X2
    X = X1;
    X1 = X2;
    X2 = X;
  }

  // ����ϵ��
  for (j = 0; j < count; j++) {
    p = 0;

    for (i = 0; i < r; i++) {
      if (j & (1 << i)) {
        p += 1 << (r - i - 1);
      }
    }

    F[j] = X1[p] / count;
  }

  // �ͷ��ڴ�
  delete X1;
  delete X2;
}


// ����:
//   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
//   LONG  lWidth       - Դͼ���ȣ���������
//   LONG  lHeight      - Դͼ��߶ȣ���������
//
// ����ֵ:
//   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
//
// ˵��:
//   �ú���������ͼ������ֶ�ʲ-������任�������治ͬ���ǣ��˴��ǽ���ά
// ����ת����һ����������Ȼ��Ը�����������һ��һά�ֶ�ʲ-������任��
BOOL WINAPI DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
  // ָ��Դͼ���ָ��
  unsigned char*  lpSrc;

  // ѭ������
  LONG  i;
  LONG  j;

  // ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  LONG  w;
  LONG  h;

  // �м����
  double  dTemp;

  int   wp;
  int   hp;

  // ͼ��ÿ�е��ֽ���
  LONG  lLineBytes;

  // ����ͼ��ÿ�е��ֽ���
  lLineBytes = WIDTHBYTES(lWidth * 8);

  // ����ֵ
  w = 1;
  h = 1;
  wp = 0;
  hp = 0;

  // ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
  while (w * 2 <= lWidth) {
    w *= 2;
    wp++;
  }

  while (h * 2 <= lHeight) {
    h *= 2;
    hp++;
  }

  // �����ڴ�
  double* f = new double[w * h];
  double* F = new double[w * h];

  // ��
  for (i = 0; i < w; i++) {
    // ��
    for (j = 0; j < h; j++) {
      // ָ��DIB��j�У���i�����ص�ָ��
      lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // ��ʱ��ֵ
      f[j + i * w] = *(lpSrc);
    }
  }

  // ���ÿ����ֶ�ʲ��������任
  WALSH(f, F, wp + hp);

  // ��
  for (i = 0; i < w; i++) {
    // ��
    for (j = 0; j < h; j++) {
      // ����Ƶ��
      dTemp = fabs(F[i * w + j] * 1000);

      // �ж��Ƿ񳬹�255
      if (dTemp > 255) {
        // ���ڳ����ģ�ֱ������Ϊ255
        dTemp = 255;
      }

      // ָ��DIB��j�У���i�����ص�ָ��
      lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - j) + i;

      // ����Դͼ��
      * (lpSrc) = (BYTE)(dTemp);
    }
  }

  //�ͷ��ڴ�
  delete f;
  delete F;

  // ����
  return TRUE;
}
