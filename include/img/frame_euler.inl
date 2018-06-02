// ����֡�ľ��룬Frame1��Frame2����Ϊ�ṹ�壬��������ΪCentroid
// ���صľ���d������֮��ľ���
int Frame_Euler(int N1, POINT* Frame1, int N2, POINT* Frame2, double* d, int* Index32)
{
  int i, j;

  for (j = 0; j < N1; ++j) {
    double t = 9999999;
    Index32[j] = -1;

    for (i = 0; i < N2; ++i) {
      int x = Frame2[i].x - Frame1[j].x, y = Frame2[i].y - Frame1[j].y;
      d[i * N1 + j] = sqrt((double)x * x + y * y);

      if (d[i * N1 + j] < t) {
        // ��������Сֵ,����������ʾ��һ֡ͼ���Ŀ����Ŀ
        // ������ʾǰһ֡ͼ���Ŀ����Ŀ�����ص������ǶԵ�3֡��������
        Index32[j] = i;
        t = d[i * N1 + j];
      }
    }
  }

  return 0;
}
