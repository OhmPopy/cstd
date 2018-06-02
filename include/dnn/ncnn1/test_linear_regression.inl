
#include "cfile.h"
#include "net.inl"
//#include "train.inl"



int test_linear_regression() {
  int i;
  Net net[1] = {0};
  //�����������ݣ����������������x_data��y_data֮��Ĺ�ϵ��
  //x_data = np.linspace(-1,1,300)[:, np.newaxis] //-1��1�ȷ�300���γɵĶ�ά����
  //noise = np.random.normal(0,0.05, x_data.shape) //��������״ͬx_data��0-0.05������̬�ֲ���С��
  //y_data = np.square(x_data)-0.5+noise //x_dataƽ������0.05���ټ�����ֵ
  enum {N = 50};
  float x_datap[N];
  float y_datap[N];
  for (i = 0; i < countof(x_datap); ++i) {
    float x = x_datap[i] = i*2./(countof(x_datap)-1)-1;
    y_datap[i] = x - 0.5 + rng_normal(NULL, 0, 0.05);
  }
  
  //����㣨1����Ԫ��
  int xs = net_addBlob(net, "x_data", 1, 1, 1, countof(x_datap), x_datap);
  int ys = net_addBlob(net, "y_data", 1, 1, 1, countof(y_datap), y_datap);
  
  //���㣨10����Ԫ��
  int W1, b1, W2, b2;
  int Wx_plus_b1 = net_matmul(net, xs, &W1, &b1, 10); //����xs��W1��ˣ�Ȼ�����ƫ��
  int output1 = net_relu(net, Wx_plus_b1); //�����ʹ��tf.nn.relu
  //int output1 = Wx_plus_b1;
  
  //����㣨1����Ԫ��
  int Wx_plus_b2 = net_matmul(net, output1, &W2, &b2, 1);
  int output2 = Wx_plus_b2;
  
  net_rng_gaussian(net, W1, 0, 1);
  net_constant(net, b1, 0.1); //ƫ�ã�1*10�ľ���ʹ��0.1���
  net_rng_gaussian(net, W2, 0, 1);
  net_constant(net, b2, 0.1);
  
#if 0
  //��ʧ
  loss = tf.reduce_mean(tf.reduce_sum(tf.square(ys-output2),reduction_indices=[1])); //�ڵ�һά�ϣ�ƫ��ƽ������ͣ�����ƽ��ֵ����������ʧ
  train_step = tf.train.GradientDescentOptimizer(0.1).minimize(loss); // ʹ���ݶ��½��������ò���0.1������С����ʧ
#endif
  
  //ѵ��
  for (i=0; i<1000; ++i) { //ѵ��1000��
    //_,loss_value = sess.run([train_step,loss],feed_dict={xs:x_data,ys:y_data}) //�����ݶ��½����㣬������ÿһ������ʧ
    Dtype loss_value = net_trainonce(net, output2, ys, 0.1, 1);
    if(i%50==0) {
    }
      printf("%f\n", loss_value); // ÿ50�����һ����ʧ
  }
  return 0;
}
