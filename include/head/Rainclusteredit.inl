
#if _MSC_VER>1200
//#define _USEOPENMP
#endif
#ifdef _USEOPENMP
#include "omp.h"
#endif

int cvShowImage_u16(const char* name, int h, int w, const unsigned short* A, int step, int cn);


// Edit by hudalikm
// Time��2013.06.09
// Second edit time: 2013.07.04
// ��������ĺô�����������Ҫ����̫��Ĳ�������Բ�ͬ�߶ȵ��˺Ͳ�ͬ�߶ȵ������Ҳ�кܺõ�³���ԣ�
// �������������κε�������ֱ��ʹ�á�ȱ�����Ҫ���˲�����̫��Ķ�����
// ������ֵȣ������������������Ҳ���ܴ����������������д����⡣�������Ȥ��
// �������ⷽ�������Ľ���
// �����㷨�Ľ�
// �����д���֤
// ע��ĳЩ���ݵ�ļ��㣬�п��ܳ����߽�
// buflen=12*h*w
int Rainclusteredit(int h, int w, const DEPTH_TYPE* I0, int I0_step, int RN, int RD, int beishu, DEPTH_TYPE* ClusterI, int ClusterI_step, unsigned char* buf, int buflen)
{
  int* index, *I, *g;
  int i, j, k, al = w, pos, pos1, MaxValue = 0, FillValue, sizeforeground, RainNum, thd = 0;
  int neighbour[8];
  int neighbour8[][2] = { -1, 0, 0, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1}; //�͵�ǰ����������ӵõ��˸���������
  BUFUSEBEGIN(buf, buflen);

  for (k = 0; k < 8; ++k) {
    neighbour[k] = neighbour8[k][0] * al + neighbour8[k][1];
  }

  index = (int*)BUFMALLOC(int, h * w);
  I = (int*)BUFMALLOC(int, h * w);
  g = (int*)BUFMALLOC(int, h * w);
  BUFBOUNDCHECK();
  memset(index, 0, h * w * sizeof(int));
  memset(I, 0, h * w * sizeof(int));
  memset(g, 0, h * w * sizeof(int));
  // ���÷�ǰ���������ֵ(��ɫ0�������ֵ)��Ŀ�ķ���߽紦�������빹��һ��ˮ�ص�Χǽ
  FillValue = 99999999;
  k = 0;

  // �ҳ�ͼ���еķ����
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      pos = i * al + j;
      I[pos] = I0[i * I0_step + j];

      //if (i>10 && i<50 && j>10 && j<50) {I[pos]=255;}
      if (I[pos] > thd && i > 0 && j > 0 && i < h - 1 && j < w - 1) {
        index[k++] = pos;
      }
      else {
        I[pos] = FillValue; // ��ǰ���������
      }
    }
  }

  // ǰ������Ŀ
  sizeforeground = k;
  //RainNum=100*sizeforeground/25; // �����Ŀ��Ҳ�����ܵ���ѭ������
  RainNum = sizeforeground * beishu; // �����Ŀ��Ҳ�����ܵ���ѭ������

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      pos = i * al + j;
      g[pos] = 0;
    }
  }

  srand(56);
#ifdef _USEOPENMP
  //printf("Number of core : %d\n", omp_get_num_procs());
  //printf("Number of threads : %d\n", omp_get_num_threads());
  #pragma omp parallel for
#endif

  for (i = 0; i < RainNum; ++i) {
    //int Rands=i%sizeforeground; // ģ����ε��������
    int Rands = rand() % sizeforeground; // ģ����ε��������
    int W = RN;
    int r = RD;
    int findt = 0; // ����ָʾ�ǳ�ʼ��λ������µ����
    int repnum = 0, Flg = 0, Value, DirectInd;
    int d[8] = {0};
    int Index[8] = {0};
    int m, tt;
    pos = index[Rands];

    while (W > 0) {
      if (pos < 0 || pos > h * w) {
        break;
      }

      tt = I[pos] + g[pos];

      for (m = 0; m < 8; ++m) {
        pos1 = pos + neighbour[m];
        d[m] = I[pos1] + g[pos1] - tt;
      }

      repnum = 1; // ��Сֵ�ĸ���
      Index[0] = 0;

      // Ѱ����d����Сֵ������
      for (m = 1; m < 8; ++m) {
        if (d[m] < d[Index[0]]) {
          Index[0] = m;
          repnum = 1;
        }
        else if (d[m] == d[Index[0]]) {
          Index[repnum++] = m;
        }
      }

      if (findt == 0) { // ��ʼ���
        Flg = Index[rand() % repnum]; // �����ǳ�ʼ��Σ���ô����Сֵ�������ѡһ������
        Value = d[Flg]; // ����������Сֵ

        if (Value < 0) {
          pos += neighbour[Flg]; // ������ת
        }
        else {
          g[pos] += MIN(r, W);
          W = W - r;
        }
      }
      else {  // �ǳ�ʼ���
        int Indicate = 0;
        DirectInd = 0; // ����ָʾ��Сֵ����ֵ֮�Ƿ�Ϊ0

        // ���������һ����Сֵ������ͬ�ķ�������ѡ��÷���
        for (m = 0; m < repnum; ++m) {
          if (Flg == Index[m]) {
            Indicate = 1;
            break;
          }
        }

        if (Indicate) {
          Value = d[Flg]; // ����������Сֵ

          if (Value == 0) { //�������Сֵ���ڣ�����DirectInd=1
            DirectInd = 1;
          }
        }
        else {
          // �������һ������ͬ������Сֵ������ֹһ����ʱ�������ѡȡһ��������Ϊ��������(0��С��0������Ƿ���)
          Flg = Index[rand() % repnum]; // �����ǳ�ʼ��Σ���ô����Сֵ�������ѡһ������
          Value = d[Flg]; // ����������Сֵ
        }

        if (DirectInd != 1) { // ������Сֵ����0
          if (Value < 0) { // ��ô����СֵС��0ʱ��ִ��������ת
            pos += neighbour[Flg]; // ������ת
          }
          else {
            g[pos] += MIN(r, W);
            W = W - r;
          }
        }
        else {
          // ����СֵΪ0(����ֻ���������һ��������ͬ��0ֵ��������һ������ͬ��0ֵ�������ڴ���)
          pos += neighbour[Flg]; // ������ת
        }
      }

      findt = findt + 1; // ��ǵ�ǰ���Ѱ�ŵĲ���������ָʾ�Ƿ��ǳ�ʼ���
    }

#if 0

    if (0 && RainNum > 2200) {
      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          ClusterI[i * ClusterI_step + j] = I0[i * I0_step + j] > thd ? 255 : 0;
        }
      }

      cvShowImage_u16("III", h, w, ClusterI, al, 1);

      for (i = 0; i < h; ++i) {
        for (j = 0; j < w; ++j) {
          ClusterI[i * ClusterI_step + j] = g[i * I0_step + j] > 1 ? 255 : 0;
        }
      }

      cvShowImage_u16("ClusterI", h, w, ClusterI, al, 1);

      cvWaitKey(-1);
    }

#endif
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      ClusterI[i * ClusterI_step + j] = g[i * I0_step + j];
    }
  }

#if 0

  if (0 && RainNum > 200) {
    cvShowImage_u16("ClusterI", h, w, ClusterI, al, 1);
    cvWaitKey(-1);
  }

#endif
  //free(g);
  //free(I);
  //free(index);
  BUFUSEEND();
  return 0;
}
