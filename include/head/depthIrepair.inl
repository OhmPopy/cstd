// ���ͼ�������
// �����ͼ��0ֵ��Ϊ���ģ���ȡ(2*BRsize+1) x (2*BRsize+1)��С�Ŀ飬��ȡ���з����ֵ��Ϊ���ĵ�����ֵ
// DI : ԭʼ���ͼ
// BRsize : ��뾶�����������˿��С������BRsizeӦ��С�ڻ��ߵ���12
// ����ĳЩ�߽ǿն��������޷����
// edit time: 2013.07.05
#define DEPTH_TYPE unsigned short
int depthIrepair(int h, int w, const DEPTH_TYPE* DI, int DI_step, int BRsize,
    DEPTH_TYPE* RI, int RI_step)
{
  int kstep[1024], kn = 0, i, j, k, n = h * w;

  if (BRsize > 12) {
    ASSERT("���С�����������趨�����ޣ����СӦ��С�ڻ��ߵ���12");
  }

  if (BRsize < 0) {
    ASSERT("���С������Ҫ�󣬿��СӦ�ô���0");
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      RI[i * RI_step + j] = DI[i * DI_step + j];
    }
  }

  for (i = 0; i < (2 * BRsize + 1); ++i) {
    for (j = 0; j < (2 * BRsize + 1); ++j) {
      kstep[kn] = (i - BRsize) * RI_step + (j - BRsize);
      ++kn;
    }
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      int pos = i * RI_step + j;
      int t = RI[pos], tn = 0;

      if (0 == t || t < 33) {
        for (k = 0; k < kn; ++k) {
          int pos1 = pos + kstep[k];

          if (pos1 >= 0 && pos1 < n && RI[pos1] > 33) {
            t += RI[pos1];
            ++tn;
          }
        }

        if (tn) {
          t /= tn;
        }

        RI[pos] = t;
      }
    }
  }

  return 0;
}
