double viterbi(hmm_t* hmm, const img_t* O, img_t* q)
{
  double prob = 0;
  //Viterbi�㷨
  //����:
  // hmm -- hmmģ��
  // O -- ����۲�����, N*D, NΪ֡��,DΪ����ά��
  //���:
  // prob -- �������
  // q -- ״̬����
  int N = hmm->N;                //HMM״̬��
  int T = O->h;  //����֡��
  int i = 0, j, t;
  img_t im[10] = {0};
  img_t* init = im + i++; //��ʼ����
  img_t* trans = im + i++; //ת�Ƹ���
  img_t* delta = im + i++; //
  img_t* fai = im + i++; //
  img_t* mix = hmm->mix;  //��˹���
  const double inf = Inf_f64();
  double* x = NULL;
  imclone2(hmm->init, init);
  imclone2(hmm->trans, trans);
  // ����log(init);
  for (i = 0; i < init->h * init->w; ++i) {
    init->tt.f8[i] = init->tt.f8[i] <= 0 ? -inf : log(init->tt.f8[i]);
  }
  // ����log(trans);
  for (i = 0; i < trans->h * trans->w; ++i) {
    trans->tt.f8[i] = trans->tt.f8[i] <= 0 ? -inf : log(trans->tt.f8[i]);
  }
  // ��ʼ��
  imsetsize_f8(delta, T, N, 1);
  imsetsize_i4(fai, T, N, 1);
  imsetsize_f8(q, T, 1, 1);
  // t=1
  x = O->tt.f8;
  for (i = 0; i < N; ++i) {
    delta->tt.f8[i] = init->tt.f8[i] + log(mixture(mix + i, x));
  }
  // t=2:T
  for (t = 1; t < T; ++t) {
    double* delta1 = delta->tt.f8 + (t - 1) * delta->w;
    double* delta0 = delta->tt.f8 + (t) * delta->w;
    int* fai0 = fai->tt.s4 + (t) * fai->w;
    x = O->tt.f8 + t * O->w;
    for (j = 0; j < N; ++j) {
      double de = delta1[0] + trans->tt.f8[j];
      int fa = 0;
      for (i = 1; i < N; ++i) {
        double d = delta1[i] + trans->tt.f8[i * trans->w + j];
        if (d > de) {
          de = d;
          fa = i;
        }
      }
      delta0[j] = de;
      fai0[j] = fa;
      delta0[j] += log(mixture(mix + j, x));
    }
  }
  //imshowmat_f8(delta);cvWaitKey(-1);
  imsetsize_i4(q, T, 1, 1);
  // ���ո��ʺ����ڵ�
  {
    double* delta1 = delta->tt.f8 + (T - 1) * delta->w;
    double de = delta1[0];
    int fa = 0;
    for (i = 1; i < N; ++i) {
      double d = delta1[i];
      if (d > de) {
        de = d;
        fa = i;
      }
    }
    prob = de;
    q->tt.s4[T - 1] = fa;
  }
  // �������״̬·��
  for (t = T - 2; t >= 0; --t) {
    q->tt.s4[t] = fai->tt.s4[(t + 1) * fai->w + q->tt.s4[t + 1]];
  }
  //imshowmat_u4(q);cvWaitKey(-1);
  imfrees2(im);
  return prob;
}

