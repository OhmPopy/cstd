int getparam(hmm_t* hmm, const img_t* O, param_t* param)
{
  //�����������O, ����ǰ�����alpha, �������beta, �궨ϵ��c, ��ksai,gama
  //����:
  // hmm -- HMMģ�Ͳ���
  // O -- n*d �۲�����
  //���:
  // param -- �������ֲ����Ľṹ
  int T = O->h;  //���еĳ���
  int N = hmm->N;                //HMM״̬��
  int D = hmm->D;
  int i = 0, j, t, maxM;
  img_t* init = hmm->init;       //��ʼ����
  img_t* trans = hmm->trans;     //ת�Ƹ���
  img_t* mix = hmm->mix; //��˹���
  img_t im[10] = {0};
  img_t* pab = im + i++;
  img_t* prob = im + i++;
  img_t* c = param->c;
  img_t* alpha = param->alpha;
  img_t* beta = param->beta;
  img_t* ksai = param->ksai;
  img_t* gama = param->gama;
  double* x, sa;
  // �����۲�����O, ����ǰ�����alpha
  imsetsize_f8(alpha, T, N, 1);
  //imshowmat_f8(O);cvWaitKey(-1);
  //imshowmat_f8(mix);cvWaitKey(-1);
  // t=1��ǰ�����
  x = O->tt.f8;
  sa = 0;
  for (i = 0; i < N; ++i) {
    alpha->tt.f8[i] = init->tt.f8[i] * mixture(mix + i, x);
    sa += alpha->tt.f8[i];
  }
  // �궨t=1��ǰ�����
  imsetsize_f8(c, T, 1, 1);
  c->tt.f8[0] = 1. / sa;
  for (i = 0; i < N; ++i) {
    alpha->tt.f8[i] = c->tt.f8[0] * alpha->tt.f8[i];
  }
  //imshowmat_f8(trans);cvWaitKey(-1);
  //imshowmat_f8(alpha);cvWaitKey(-1);
  // t=2:T��ǰ����ʺͱ궨
  for (t = 1; t < T; ++t) {
    double* alpha1 = alpha->tt.f8 + (t - 1) * alpha->w;
    double* alpha0 = alpha->tt.f8 + t * alpha->w;
    const double* Ot = O->tt.f8 + t * O->w;
    sa = 0;
    for (i = 0; i < N; ++i) {
      double temp = 0;
      for (j = 0; j < N; ++j) {
        temp += alpha1[j] * trans->tt.f8[j * trans->w + i];
      }
      alpha0[i] = temp * mixture(mix + i, Ot);
      sa += alpha0[i];
    }
    c->tt.f8[t] = 1. / sa;
    for (i = 0; i < N; ++i) {
      alpha0[i] = c->tt.f8[t] * alpha0[i];
    }
  }
  //imshowmat_f8(c);cvWaitKey(-1);
  // �����۲�����O, ����������beta
  imsetsize_f8(beta, T, N, 1);
  // t=T�ĺ�����ʼ��궨
  for (i = 0; i < N; ++i) {
    beta->tt.f8[(T - 1)*beta->w + i] = c->tt.f8[T - 1];
  }
  // t=T-1:1�ĺ�����ʺͱ궨
  for (t = T - 2; t >= 0; --t) {
    double* beta1 = beta->tt.f8 + (t + 1) * beta->w;
    double* beta0 = beta->tt.f8 + t * beta->w;
    double mm[HMMMAXMIX];
    x = O->tt.f8 + (t + 1) * O->w;
    for (j = 0; j < N; ++j) {
      mm[j] = mixture(mix + j, x);
    }
    for (i = 0; i < N; ++i) {
      double temp = 0;
      for (j = 0; j < N; ++j) {
        temp += beta1[j] * trans->tt.f8[i * trans->w + j] * mm[j];
      }
      beta0[i] = temp;
    }
    for (i = 0; i < N; ++i) {
      beta0[i] = c->tt.f8[t] * beta0[i];
    }
  }
  //imshowmat_f8(beta);cvWaitKey(-1);
  //���ɸ���ksai
  imsetsize_f8(ksai, (T - 1), N * N, 1);
  for (t = 0; t < T - 1; ++t) {
    //const double* alpha1 = alpha->tt.f8+(t-1)*alpha->w;
    const double* alpha0 = alpha->tt.f8 + t * alpha->w;
    const double* beta1 = beta->tt.f8 + (t + 1) * beta->w;
    const double* beta0 = beta->tt.f8 + t * beta->w;
    const double* O1 = O->tt.f8 + (t + 1) * O->w;
    double* ksai0 = ksai->tt.f8 + t * ksai->w;
    double denom = 0;
    for (i = 0; i < N; ++i) {
      denom += alpha0[i] * beta0[i];
    }
    for (i = 0; i < N - 1; ++i) {
      for (j = i; j < (i + 2); ++j) {
        double nom = alpha0[i] * trans->tt.f8[i * trans->w + j] * mixture(mix + j, O1) * beta1[j];
        ksai0[i * N + j] = c->tt.f8[t] * nom / denom;
      }
    }
  }
  //imshowmat_f8(ksai);cvWaitKey(-1);
  maxM = hmm->M[0];
  for (i = 1; i < N; ++i) {
    maxM = MAX(maxM, hmm->M[i]);
  }
  //����������:gama
  imsetsize_f8(gama, T, N * maxM, 1);
  imsetsize_f8(pab, N, 1, 1);
  imsetsize_f8(prob, maxM, 1, 1);
  //gama = zeros(T,N,max(hmm->M));
  for (t = 0; t < T; ++t) {
    const double* alpha0 = alpha->tt.f8 + t * alpha->w;
    const double* beta0 = beta->tt.f8 + t * beta->w;
    double* gama0 = gama->tt.f8 + t * gama->w;
    const double* x = O->tt.f8 + (t) * O->w;
    double* prob0 = prob->tt.f8;
    double tmp, spab = 0;
    //pab = zeros(N,1);
    for (i = 0; i < N; ++i) {
      pab->tt.f8[i] = alpha0[i] * beta0[i];
      spab += pab->tt.f8[i];
    }
    for (i = 0; i < N; ++i) {
      int mi = hmm->M[i];
      double sprob = 0;
      for (j = 0; j < mi; ++j) {
        prob0[j] = 0;
      }
      for (j = 0; j < mi; ++j) {
        double* m = hmm->mix[i].tt.f8 + j * (D + D + 1);
        double* v = m + D;
        double weight = v[D];
        prob0[j] = weight * pdf(D, m, v, x);
        sprob += prob0[j];
      }
      tmp = pab->tt.f8[i] / spab;
      for (j = 0; j < mi; ++j) {
        gama0[i * maxM + j] = tmp * prob0[j] / sprob;
      }
    }
  }
  //imshowmat_f8(gama);cvWaitKey(-1);
  imfrees2(im);
  return 0;
}

