double pdf(int D, const double* m, const double* v, const double* x)
{
  int i;
  double p = 0, prodv = 1, y = 0;
  for (i = 0; i < D; ++i) {
    double t = x[i] - m[i];
    y += t * t / v[i];
    prodv *= v[i];
  }
  p = exp(-0.5 * y) / sqrt(2 * PI * prodv);
  return p;
}
double mixture(const img_t* mix, const double* x)
{
  //�����������
  //����:
  // mix -- ��ϸ�˹�ṹ
  // x -- ��������, SIZE*1
  //���:
  // prob -- �������
  int j, M = mix->h, D = (mix->w) / 2;
  double prob = 0;
  //imshowmat_f8(mix);cvWaitKey(-1);
  for (j = 0; j < M; ++j) {
    const double* m = mix->tt.f8 + j * mix->w;
    const double* v = m + D;
    const double w = m[2 * D];
    prob += w * pdf(D, m, v, x);
  }
  // ����realmin, �Է�ֹviterbi.m�м���log(prob)ʱ���
  if (prob == 0) {
    prob = DBL_MIN;
  }
  return prob;
}

