
// ��֪��ϣ�㷨
__int64 perceptual_hash(int h, int w, const uchar* A, int al, int ai)
{
  int sum[64] = {0};
  int cnt[64] = {0};
  int i, j, i1, j1, avg = 0;
  __int64 ret = 0;

  // ��һ������С�ߴ硣
  // ��ͼƬ��С��8x8�ĳߴ磬�ܹ�64�����ء���һ����������ȥ��ͼƬ��ϸ�ڣ�ֻ�����ṹ�������Ȼ�����Ϣ��������ͬ�ߴ硢����������ͼƬ���졣
  for (i = 0; i < h; ++i) {
    const uchar* A0 = A + i * al;
    i1 = i * 8 / h;

    for (j = 0; j < w; ++j) {
      j1 = i1 * 8 + j * 8 / w;
      sum[j1] += (A0[0] + A0[1] + A0[1] + A0[2]) >> 2;
      cnt[j1]++;
    }
  }

  // �ڶ�������ɫ�ʡ�
  // ����С���ͼƬ��תΪ64���Ҷȡ�Ҳ����˵���������ص��ܹ�ֻ��64����ɫ��
  // ������������ƽ��ֵ��
  // ��������64�����صĻҶ�ƽ��ֵ��
  for (i = 0; i < 64; ++i) {
    sum[i] /= cnt[i];
    avg += sum[i];
  }

  avg /= 64;

  // ���Ĳ����Ƚ����صĻҶȡ�
  // ��ÿ�����صĻҶȣ���ƽ��ֵ���бȽϡ����ڻ����ƽ��ֵ����Ϊ1��С��ƽ��ֵ����Ϊ0��
  for (i = 0; i < 64; ++i) {
    ret |= ((__int64)(sum[i] > avg)) << (i);
  }

  return ret;
}

int hamming_distance_i64(__int64 a, __int64 b)
{
  int i = 0, d = 0;
  __int64 c = a ^ b;

  for (i = 0; i < 64; ++i) {
    d += !!(c & (1 << i));
  }

  return d;
}

//#include "../fmtimg/fmtimg.inl"

int test_perceptual_hash()
{
  img_t im[1] = {0};
  FILE* pf;
  char buf[256];
  int i = 0, d;
  __int64 sig[100];
  _chdir("D:/code/c/ͼ��ͼ��/����ͼƬ����/SimilarImageSearch/images");
  pf = fopen("list.txt", "rb");

  for (; fgets(buf, 256, pf) > 0; ++i) {
    strtrim(buf, "\n\r\t ");
    imread(buf, 3, 1, im);
    {
      utime_start(_start_time);
      sig[i] = perceptual_hash(im->h, im->w, im->data, im->s, im->c);
      d = hamming_distance_i64(sig[i], sig[0]);
      printf("%f\n", utime_elapsed(_start_time));
    }
    // �������� ���
    printf("%d %I64X  %s\n", d, sig[i], buf);
  }

  imfree(im);
  return 0;
}
