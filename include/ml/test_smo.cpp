
#include "smo.h"
#include "algrand.hpp"


namespace test {

//��������������ʼ��������ѡ������Ȩֵ����ѭ��
int test_smo() {
  svm_smo<learned_func_nonlinear<xlib::rbf_kernel<double> > > sv;
  const string data_file_name="svm.data";//�����ļ�
  const string svm_file_name="svm.model";//ģ���ļ�
  sv.train(data_file_name,svm_file_name);
  //#else
  sv.load_svm(svm_file_name);
  vector<double> x;
  x.resize(2);
  x[0]=-11;
  x[1]=0;
  double y;
  y=sv.predict_func(x.begin(), x.end());
  assert(y<0);
  x[0]=1;
  x[1]=0;
  y=sv.predict_func(x.begin(), x.end());
  assert(y>0);
  return 0;
}

}

int main(int argc,char* argv[]) {
  //test_rand_fill();
  test::test_smo();
  return 0;
}

