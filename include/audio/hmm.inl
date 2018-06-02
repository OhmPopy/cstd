#include <math.h>
// �����������������ʹ�����ಿ�ֵĴ����Ϊƽ̨�޹صġ�
// �õ�һ����������ӣ����Ը����Լ�������޸ĸú���
int hmmgetseed(void)
{
  return (17 * rand());
}
// �������������
void hmmsetseed(int seed)
{
  srand(seed);
}
// �õ�0��1֮���һ��˫���������
double hmmgetrand(void)
{
  return (double) rand() / RAND_MAX;
}
//int N; /* ״̬���� Q={1,2,...,N} */
//int M; /* �۲�ֵ��; V={1,2,...,M} */
//double* pi; /* pi[N] pi[i] �ǽ���Markov����״̬�ֲ� */
//double* A; /* A[N*N]. a[i*N+j] ��״̬iת�Ƶ�״̬j��ת�Ƹ���*/
//double* B; /* B[N*M]. b[j*M+k] ��״̬j�۲쵽�۲�ֵk�ĸ��� */
#define HMM_DEF() int N=0, M=0; double* pi=0, *A=0, *B=0
#define HMM_GET_PARAM(phmm) HMM_DEF(); hmm_get_param(phmm, &N, &M, &pi, &A, &B)
int hmm_get_param(void* s, int* pN, int* pM, double** ppi, double** pA, double** pB)
{
  int* p = (int*)(s);
  int N = p[0], M = p[1];
  double* p2 = (double*)(p + 2);
  if (pN) {
    *pN = N;
  }
  if (pM) {
    *pM = M;
  }
  if (ppi) {
    *ppi = p2;
  }
  if (pA) {
    *pA = p2 + N;
  }
  if (pB) {
    *pB = p2 + N + N * N;
  }
  return 0;
}
int hmm_resize(void* pphmm, int N, int M, double** ppi, double** pA, double** pB)
{
  uchar** pphmm1 = (uchar**)pphmm;
  int size = 2 * sizeof(int) + (N + N * N + N * M) * sizeof(double);
  int* p;
  double* p2;
  MYREALLOC(*pphmm1, size);
  p = (int*)(*pphmm1);
  p2 = (double*)(p + 2);
  p[0] = N, p[1] = M;
  if (ppi) {
    *ppi = p2;
  }
  if (pA) {
    *pA = p2 + N;
  }
  if (pB) {
    *pB = p2 + N + N * N;
  }
  return size;
}
// ���е����ɵȺ���
// ������ʼ״̬
int GenInitalState(void* phmm)
{
  double val, accum;
  int i, q_t;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  q_t = N;
  for (i = 0; i < N; ++i) {
    if (val < pi[i] + accum) {
      q_t = i;
      break;
    }
    else {
      accum += pi[i];
    }
  }
  return q_t;
}
// ������һ��״̬
int GenNextState(void* phmm, int q_t)
{
  double val, accum;
  int j, q_next;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  q_next = N;
  for (j = 0; j < N; ++j) {
    if (val < A[q_t * N + j] + accum) {
      q_next = j;
      break;
    }
    else {
      accum += A[q_t * N + j];
    }
  }
  return q_next;
}
// ���ɹ۲�ֵ
int GenSymbol(void* phmm, int q_t)
{
  double val, accum;
  int j, o_t;
  HMM_GET_PARAM(phmm);
  val = hmmgetrand();
  accum = 0.0;
  o_t = M;
  for (j = 0; j < M; ++j) {
    if (val < B[q_t * M + j] + accum) {
      o_t = j;
      break;
    }
    else {
      accum += B[q_t * M + j];
    }
  }
  return o_t;
}
// �������� - GenSequenceArray
// ���� - �������״̬
// ���� - phmm,void�ṹָ��
// seed:���������
// T - �۲����г���
// O[T] - �۲�ֵ����
// q[T] - ����������
void GenSequenceArray(void* phmm, int seed, int T, int* O, int* q) //O q Ϊ��̬��������
{
  int t = 0;
  hmmsetseed(seed);
  q[1] = GenInitalState(phmm);
  O[1] = GenSymbol(phmm, q[1]);
  for (t = 1; t < T; ++t) {
    q[t] = GenNextState(phmm, q[t - 1]);
    O[t] = GenSymbol(phmm, q[t]);
  }
}
// HMM�ļ��Ķ�д�Ȳ���
// �������� - ReadHMM
// ���� - ��ȡvoid�ṹ
// ���� - fp - �ļ�ָ��
// phmm - void�ṹָ�룬����void�ṹ
// ����ֵ - ��
int ReadHMM(FILE* fp, void* pphmm)
{
  int i, j, k, N, M;
  double* pi, *A, *B;
  fscanf(fp, "M= %d\n", &(M));
  fscanf(fp, "N= %d\n", &(N));
  hmm_resize(pphmm, N, M, &pi, &A, &B);
  fscanf(fp, "pi:\n");
  for (i = 0; i < N; ++i) {
    fscanf(fp, "%lf", &(pi[i]));
  }
  fscanf(fp, "A:\n");
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      fscanf(fp, "%lf", &(A[i * N + j]));
    }
    fscanf(fp, "\n");
  }
  fscanf(fp, "B:\n");
  for (j = 0; j < N; ++j) {
    for (k = 0; k < M; ++k) {
      fscanf(fp, "%lf", &(B[j * M + k]));
    }
    fscanf(fp, "\n");
  }
  return 0;
}
// �������� - InitHMM
// ���� - ��ʼ��void�ṹ
// ���� - phmm - void�ṹָ��
// N - ״̬��
// M - �ɹ۲�ֵ�ø���
// seed - ���������
// ����ֵ - ��
void InitHMM(void* pphmm, int N, int M, int seed)
{
  int i, j, k;
  double sum;
  double* pi, *A, *B;
  hmm_resize(pphmm, N, M, &pi, &A, &B);
  /* ��ʼ������������� */
  hmmsetseed(seed);
  for (i = 0; i < N; ++i) {
    sum = 0.0;
    for (j = 0; j < N; ++j) {
      A[i * N + j] = hmmgetrand();
      sum += A[i * N + j];
    }
    for (j = 0; j < N; ++j) {
      A[i * N + j] /= sum;
    }
  }
  for (j = 0; j < N; ++j) {
    sum = 0.0;
    for (k = 0; k < M; ++k) {
      B[j * M + k] = hmmgetrand();
      sum += B[j * M + k];
    }
    for (k = 0; k < M; ++k) {
      B[j * M + k] /= sum;
    }
  }
  sum = 0.0;
  for (i = 0; i < N; ++i) {
    pi[i] = hmmgetrand();
    sum += pi[i];
  }
  for (i = 0; i < N; ++i) {
    pi[i] /= sum;
  }
}
// �������� - PrintHMM
// ���� - ����void�ṹ
// ���� - fp - �ļ�ָ��
// phmm - void�ṹָ��
// ����ֵ - ��
void PrintHMM(FILE* fp, void* phmm)
{
  int i, j, k;
  HMM_GET_PARAM(phmm);
  fprintf(fp, "M= %d\n", M);
  fprintf(fp, "N= %d\n", N);
  fprintf(fp, "A:\n");
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      fprintf(fp, "%f ", A[i * N + j]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "B:\n");
  for (j = 0; j < N; ++j) {
    for (k = 0; k < M; ++k) {
      fprintf(fp, "%f ", B[j * M + k]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "pi:\n");
  for (i = 0; i < N; ++i) {
    fprintf(fp, "%f ", pi[i]);
  }
  fprintf(fp, "\n\n");
}
// �����۲�ֵ���к�voidģ�ͣ�����ǰ���㷨��ȡ�����
// �������� - hmm_forward
// ���� - �����㷨���Ʋ���
// ���� - phmm:ָ��void��ָ��
// T - �۲�ֵ���еĳ���
// O - �۲�ֵ����
// alpha:�������õ�����ʱ����
// pprob:����ֵ����Ҫ��ĸ���**
int hmm_forward(void* phmm, int T, const int* O, double* alpha, double* pprob)
{
  int i, j; //״̬�±�
  int t; //ʱ���±�
  double sum; //�ֲ��ܺ�
  HMM_GET_PARAM(phmm);
  //1,��ʼ��
  for (i = 0; i < N; ++i) {
    alpha[i] = pi[i] * B[i * M + O[0]];
  }
  //2,�ݹ�
  for (t = 1; t < T; ++t) {
    for (j = 0; j < N; ++j) {
      sum = 0.0;
      for (i = 0; i < N; ++i) {
        sum += alpha[(t - 1) * N + i] * (A[i * N + j]);
      }
      alpha[t * N + j] = sum * (B[j * M + O[t]]);
    }
  }
  //3,��ֹ
  if (pprob) {
    sum = 0.0;
    for (i = 0; i < N; ++i) {
      sum += alpha[(T - 1) * N + i];
    }
    *pprob = sum;
  }
  return 0;
}
// ���� - ǰ���㷨���Ʋ���
// ���� - phmm - ָ��void��ָ��
// T - �۲�ֵ���еĳ���
// O - �۲�ֵ����
// alpha - �������õ�����ʱ����
// scale - ��������
// ppmh - ����ֵ����Ҫ��ĸ���
// pprob �Ƕ�������
int hmm_ForwardWithScale(void* phmm, int T, int* O, double* alpha, double* scale, double* pprob)
{
  int i, j; //״̬�±�
  int t; //ʱ���±�
  double sum; //�ֲ��ܺ�
  HMM_GET_PARAM(phmm);
  //1����ʼ��
  sum = 0.0;
  for (i = 0; i < N; ++i) {
    alpha[i] = pi[i] * (B[i * M + O[0]]);
    sum += alpha[i];
  }
  scale[0] = sum;
  for (i = 0; i < N; ++i) {
    alpha[i] /= scale[0];
  }
  //2,�ݹ�
  for (t = 1; t < T; ++t) {
    scale[t] = 0.0;
    for (j = 0; j < N; ++j) {
      sum = 0.0;
      for (i = 0; i < N; ++i) {
        sum += alpha[(t - 1) * N + i] * (A[i * N + j]);
      }
      alpha[t * N + j] = sum * (B[j * M + O[t]]);
      scale[t] += alpha[t * N + j];
    }
    for (j = 0; j < N; ++j) {
      alpha[t * N + j] /= scale[t];
    }
  }
  //3,��ֹ
  if (pprob) {
    sum = 0.0;
    for (t = 0; t < T; ++t) {
      sum += log(scale[t]);
    }
    *pprob = sum;
  }
  return 0;
}
// �����㷨���Ʋ��� �����۲�ֵ���к�voidģ�ͣ�����ǰ������㷨��ȡ�����
// ���� - phmm - ָ��void��ָ��
// T - �۲�ֵ���еĳ���
// O - �۲�ֵ����
// beta - �������õ�����ʱ����
// pprob - ����ֵ����Ҫ��ĸ���
//
int hmm_backward(void* phmm, int T, const int* O, double* beta, double* pprob)
{
  int i, j; /* ״̬ */
  int t; /* ʱ���±� */
  double sum;
  HMM_GET_PARAM(phmm);
  /* 1. ��ʼ�� */
  for (i = 0; i < N; ++i) {
    beta[(T - 1)*N + i] = 1.0;
  }
  /* 2. �ݹ� */
  for (t = T - 2; t >= 0; --t) {
    for (i = 0; i < N; ++i) {
      sum = 0.0;
      for (j = 0; j < N; ++j) {
        sum += A[i * N + j] * (B[j * M + O[t + 1]]) * beta[(t + 1) * N + j];
      }
      beta[t * N + i] = sum;
    }
  }
  /* 3. ��ֹ */
#if 0
  *pprob = 0.0;
  for (i = 0; i < N; ++i) {
    *pprob += beta[i];
  }
#else
  *pprob = 0.0;
  for (i = 0; i < N; ++i) {
    *pprob += beta[i] * pi[i] * B[i * M];
  }
#endif
  return 0;
}
// ������?����Ʋ��������������������?
// phmm - ָ��void��ָ��
// T - �۲�ֵ���еĳ���
// O[T] - �۲�ֵ����
// beta[T*N] - �������õ�����ʱ����
// scale[T] - ������������
// pprob - ����ֵ����Ҫ��ĸ���
int hmm_BackwardWithScale(void* phmm, int T, int* O, double* beta, double* scale, double* pprob)
{
  int i, j; /* ״ָ̬ʾ */
  int t; /* ʱ���±� */
  double sum;
  HMM_GET_PARAM(phmm);
  /* 1. ��ʼ�� */
  for (i = 0; i < N; ++i) {
    beta[(T - 1)*N + i] = 1.0 / scale[T - 1];
  }
  /* 2. �ݹ� */
  for (t = T - 2; t >= 0; --t) {
    for (i = 0; i < N; ++i) {
      sum = 0.0;
      for (j = 0; j < N; ++j) {
        sum += A[i * N + j] * (B[j * M + O[t + 1]]) * beta[(t + 1) * N + j];
      }
      beta[t * N + i] = sum / scale[t];
    }
  }
  return 0;
}
//
#define VITHUGE 100000000000.0
// ����HMM�͹۲����У�������ܵ�״̬
// phmm - void�ṹָ��
// T - �۲�ֵ�ĸ���
// O[T] - �۲�����
// delta[T*N] psi[T*N] Ϊ�м����
// q[T] - ��õ����״̬����
// pprob - ����
//
int hmm_Viterbi(void* phmm, int T, const int* O, double* delta, int* psi, int* q, double* pprob)
{
  int i, j; /* ״̬�±� */
  int t; /* ʱ���±� */
  int maxvalind;
  double maxval, val;
  HMM_GET_PARAM(phmm);
  /* 1. ��ʼ�� */
  for (i = 0; i < N; ++i) {
    delta[i] = pi[i] * (B[i * M + O[0]]);
    psi[i] = 0;
  }
  /* 2. �ݹ� */
  for (t = 1; t < T; ++t) {
    for (j = 0; j < N; ++j) {
      maxval = delta[(t - 1) * N] * (A[j]);
      maxvalind = 0;
      for (i = 1; i < N; ++i) {
        val = delta[(t - 1) * N + i] * (A[i * N + j]);
        if (val > maxval) {
          maxval = val;
          maxvalind = i;
        }
      }
      delta[t * N + j] = maxval * (B[j * M + O[t]]);
      psi[t * N + j] = maxvalind;
    }
  }
  /* 3. ��ֹ */
  *pprob = delta[(T - 1) * N];
  q[T - 1] = 0;
  for (i = 1; i < N; ++i) {
    if (delta[(T - 1)*N + i] > *pprob) {
      *pprob = delta[(T - 1) * N + i];
      q[T - 1] = i;
    }
  }
  /* 4. Path (state sequence) backtracking */
  for (t = T - 1; t > 0; --t) {
    q[t - 1] = psi[t * N + q[t]];
  }
  return 0;
}
/**************************************************************************
** �������� - hmm_ViterbiLog
** ���� - hmm_Viterbi�㷨
** ���� - phmm - void�ṹָ��
** T - �۲�ֵ�ĸ���
** O - �۲�����
** delta��psiΪ�м����
** q - ��õ����״̬����
** pprob - ����
**/
int hmm_ViterbiLog(void* phmm, int T, int* O, double* delta, int* psi, int* q, double* pprob)
{
  int i, j; /* ״̬�±� */
  int t; /* ʱ���±� */
  int maxvalind;
  double maxval, val;
  double* biot = 0;
  HMM_GET_PARAM(phmm);
  /* 0. Ԥ���� */
  for (i = 0; i < N; ++i) {
    pi[i] = log(pi[i]);
  }
  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      A[i * N + j] = log(A[i * N + j]);
    }
  }
  MYREALLOC(biot, N * T);
  for (i = 0; i < N; ++i)
    for (t = 0; t < T; ++t) {
      biot[i * N + t] = log(B[i * M + O[t]]);
    }
  /* 1. ��ʼ�� */
  for (i = 0; i < N; ++i) {
    delta[1 * N + i] = pi[i] + biot[i * N + 1];
    psi[1 * N + i] = 0;
  }
  /* 2. �ݹ� */
  for (t = 2; t <= T; ++t) {
    for (j = 0; j < N; ++j) {
      maxval = -VITHUGE;
      maxvalind = 1;
      for (i = 0; i < N; ++i) {
        val = delta[t - 1 * N + i] + (A[i * N + j]);
        if (val > maxval) {
          maxval = val;
          maxvalind = i;
        }
      }
      delta[t * N + j] = maxval + biot[j * N + t];
      psi[t * N + j] = maxvalind;
    }
  }
  /* 3. ��ֹ */
  *pprob = -VITHUGE;
  q[T] = 1;
  for (i = 0; i < N; ++i) {
    if (delta[T * N + i] > *pprob) {
      *pprob = delta[T * N + i];
      q[T] = i;
    }
  }
  /* 4. ���� */
  for (t = T - 1; t >= 1; t--) {
    q[t] = psi[t * N + q[t]];
  }
  FREE(biot);
  return 0;
}
#if 0
#define DELTA 0.001
// ����Gamma
void ComputeGamma(void* phmm, int T, double* gamma, double* xi)
{
  int i, j;
  int t;
  double denominator;
  HMM_GET_PARAM(phmm);
  for (t = 0; t < T; ++t) {
    for (i = 0; i < N; ++i) {
      gamma[t * N + i] = 0;
    }
  }
  for (t = 0; t < T; ++t) {
    denominator = 0.0;
    for (i = 0; i < N; ++i) {
      for (j = 1; j < N; j++) {
        gamma[t * N + i] += xi[t * N + i * N + j];
      }
      denominator += gamma[t * N + i];
    }
    for (i = 0; i < N; ++i) {
      gamma[t * N + i] = gamma[t * N + i] / denominator;
    }
  }
}
// ����Xi
void ComputeXi(void* phmm, int T, int* O, double* alpha, double* beta, double* xi)
{
  int i, j;
  int t;
  double sum;
  HMM_GET_PARAM(phmm);
  for (t = 0; t < T - 1; ++t) {
    sum = 0.0;
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j) {
        xi[t * N + i * N + j] = alpha[t * N + i] * beta[t * N + j] * (A[i * N + j]) * (B[j * M + O[t]]);
        sum += xi[t * N + i * N + j];
      }
    }
    for (i = 0; i < N; ++i) {
      for (j = 0; j < N; ++j) {
        xi[t * N + i * N + j] /= sum;
      }
    }
  }
}
// phmm - voidģ��ָ��
// T - �۲����г���
// O - �۲�����
// alpha��beta��gamma��pniter��Ϊ�м����
// plogprobinit - ��ʼ����
// plogprobfinal ���ո���
int BaumWelch(void* phmm, int T, const int* O, double* alpha, double* beta,
    double* gamma, int* pniter, double* plogprobinit, double* plogprobfinal)
{
  int i, j, k;
  int t, l = 0;
  double logprobf, logprobb;
  double numeratorA, denominatorA;
  double numeratorB, denominatorB;
  double* xi = 0, *scale = 0;
  double delta, logprobprev, test;
  double deltaprev = 10e-70;
  MYREALLOC(scale, (T + T * N * N)); //����һ��1*T˫����ʵ������
  xi = scale + T; //
  hmm_ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
  // hmm_forward( phmm, T, O, alpha,&logprobf );
  *plogprobinit = logprobf; /* log P(O |��ʼ״̬) */
  *plogprobinit = pow(10, *plogprobinit);
  // hmm_BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
  hmm_backward(phmm, T, O, beta, &logprobb);
  test = logprobb; //��ʱ�ӵ�ֵ�����ڲ���
  ComputeXi(phmm, T, O, alpha, beta, xi);//���û��ӿں���
  ComputeGamma(phmm, T, gamma, xi); //���û��ӿں���
  logprobprev = logprobf;
  do {
    /* ���¹��� t=1 ʱ��״̬Ϊi ��Ƶ�� */
    for (i = 0; i < N; ++i) {
      pi[i] = .001 + .999 * gamma[1 * N + i]; //pi�ĳ�ʼ��
    }
    /* ���¹���ת�ƾ���͹۲���� */
    for (i = 0; i < N; ++i) {
      denominatorA = 0.0;
      for (t = 0; t < T - 1; ++t) {
        denominatorA += gamma[t * N + i];
      }
      for (j = 0; j < N; ++j) {
        numeratorA = 0.0;
        for (t = 0; t < T - 1; ++t) {
          numeratorA += xi[t * N + i * N + j];
        }
        A[i * N + j] = .001 + .999 * numeratorA / denominatorA;
      }
      denominatorB = denominatorA + gamma[T * N + i];
      for (k = 0; k < M; ++k) {
        numeratorB = 0.0;
        for (t = 0; t < T; ++t) {
          if (O[t] == k) {
            numeratorB += gamma[t * N + i];
          }
        }
        B[i * M + k] = .001 + .999 * numeratorB / denominatorB;
      }
    }
    hmm_ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
    hmm_BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
    ComputeXi(phmm, T, O, alpha, beta, xi);
    ComputeGamma(phmm, T, gamma, xi);
    /* ��������ֱ�ӵĸ��ʲ� */
    delta = logprobf - logprobprev;
    logprobprev = logprobf;
    l++;
  }
  while (delta < DELTA); /* �����Ĳ�̫�󣬱����������˳� */
  *pniter = l;
  *plogprobfinal = logprobf; /* log P(O|estimated model) */
  FREE(scale);
  return 0;
}
#endif

