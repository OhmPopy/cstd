#include "inithmm.inl"
#include "mixture.inl"
#include "getparam.inl"
#include "baum.inl"
#include "viterbi.inl"
//����:
// samples -- �����ṹ
// M -- Ϊÿ��״ָ̬��pdf����,��:[3 3 3 3]
//���:
// hmm -- ѵ����ɺ��hmm
int trainhmm(hmm_t* hmm, int K, const img_t* samples, int N, const int* M)
{
  int i = 0, k;
  img_t im[10] = {0};
  img_t* q = im + i++;
  double pout1 = 0;
  double pout = 0;
  int loop;
  // ������������
  //disp('���ڼ�����������');
  //inithmm(hmm, K, samples, N, M);
  for (loop = 0; loop < 40; ++loop) {
    printf("��%d��ѵ��\n", loop + 1);
    baum(hmm, K, samples);
    //�������������
    pout = 0;
    for (k = 0; k < K; ++k) {
      pout += viterbi(hmm, samples + k, q);
    }
    printf("�ܺ��������(log)=%f\n", pout);
    //�Ƚ�����HMM�ľ���
    if (loop > 1) {
      double ss = fabs((pout - pout1) / pout);
      if (ss < 5e-6) {
        printf("����!\n");
        return 0;
      }
    }
    pout1 = pout;
  }
  printf("����40���Բ�����, �˳�\n");
  imfrees2(im);
  return 0;
}

