// ��֪��ѧϰ�㷨

//�������ǶԳ˻��͵Ĵ����������0�������ֵΪ1��������������ֵΪ-1  
double sigmoid (double val)  
{
  return (val > 0.0) ? 1 : -1;
}
//�����֪��
//singlelayer perceptrons(SLP)  
double slp_calculate_output(const double* inputs, const double* weights, int nInputs)  
{
  int i;
  double sum =0.0;  
  for (i = 0 ; i < nInputs ; ++i) {  
    sum += (weights[i] * inputs[i]);  
  }  
  //�������ǶԳ˻��͵Ĵ����������0�������ֵΪ1��������������ֵΪ-1  
  return sigmoid(sum);
}  
enum { nInputs  = 4,
nOutputs = 3,
 nHiddens = 4,
};

// ����֪����Multi-Layer Perceptrons�� MLP������������
typedef struct mlp
{
  double inputs[nInputs+1];//��һ������ŵ�bias��һ������1  
  double outputs[nOutputs];  
  double hiddens[nHiddens+1]; //��һ������ŵ�bias��һ������1  
  double weight_hiddens_2_inputs[nHiddens+1][nInputs+1];  
  double weight_outputs_2_hiddens[nOutputs][nHiddens+1];  
} mlp;
//���������
int mlp_calculate_outputs(mlp * pMlp)  
{
  int i, h, o;
  double sum =0.0;  
  //���ȼ������ز��е�ÿһ������ֵ  
  for (h = 0 ; h < nHiddens ; ++h)  
  {  
    double sum = 0.0;
    for (i = 0 ; i < nInputs + 1 ; ++i)
    {
      sum += pMlp->weight_hiddens_2_inputs[h][i]*pMlp->inputs[i];  
    }
    pMlp->hiddens[h] = sigmoid(sum);
  }
  //�������ز���Ϊ������㡱�����������  
  for (o = 0 ; o < nOutputs ; ++o)  
  {
    double sum = 0.0;
    for (h = 0 ; h < nHiddens + 1 ; ++h)  
    {
      sum += pMlp->weight_outputs_2_hiddens[o][h]*pMlp->hiddens[h];  
    }
    pMlp->outputs[o] = sigmoid(sum);  
  }  
  return 0;  
}

// ��֪��ѧϰ�㷨
//1.��֪��ѧϰ
//��ʵ��֪��ѧϰ�㷨���������õ�һ�ڽ��ܵĵ����֪�����������ø�����ȷ���ݣ�
//����õ����ֵ�������ֵ����ȷ��ֵ��ȣ��ɴ�������ÿһ��������ϵ�Ȩֵ��
//��ʽ2������������Ȩֵ������ ��һ����ѧϰ��������һ���ҽ������ó�С��1��������T����ѵ�������е���ȷ����� ���ǵ�i������˵�����ֵ�����ǵ�i������������Ȩֵ��


enum {
  nTests   = 4, //ѵ�����ݵ�����  
    nTestInputs  = 2, //����˵�����
  };
//�������ֵ
int compute(double* inputs, double* weights)  
{
  int i;
  double sum = 0.0;
  for (i = 0 ; i < nTestInputs; ++i)  
  {  
    sum += weights[i]*inputs[i];  
  }  
  //bias  
  sum += 1.0 * weights[nInputs];  
  if(sum >0.0)  
    return 1;  
  else  
    return-1;  
}  
//  
int test_slp1()  
{  
  COLOR color_tab[] = {
   CV_RGB(255, 0, 0),
   CV_RGB(0, 255, 0),
   CV_RGB(100, 100, 255),
   CV_RGB(255, 0, 255),
   CV_RGB(255, 255, 0)
  };
  //PerceptronLearning Algorithm(PLA)  
  const double  alpha =0.2;       //ѧϰ����  
  typedef struct slp  
  {  
    double inputs[nTestInputs];  
    double output;  
  } slp; //�����֪��

  //��ȷ��ѵ������
  slp slps[nTests] = {
    {-1.0,-1.0,-1.0},  
    {-1.0, 1.0, 1.0},  
    { 1.0,-1.0, 1.0},  
    { 1.0, 1.0, 1.0}  
  };
  double weights[nTestInputs + 1] = {0.0};
  BOOL bLearningOK = FALSE;
  int i, j, w;
  //��֪��ѧϰ�㷨
  while(!bLearningOK)  
  {
    bLearningOK = TRUE;  
    for (i = 0 ; i < nTests ; ++i)  
    {
      int output = compute(slps[i].inputs, weights);  
      if(output!= (int)slps[i].output)
      {
        for(w = 0 ; w < nTestInputs; ++w)  
        {
          weights[w] += alpha * slps[i].output * slps[i].inputs[w];  
        }
        weights[nTestInputs] += alpha *slps[i].output ;  
        bLearningOK = FALSE;
      }
    }
  }
  {
    img_t im[1] = {0};
    double tx = im->w/3;
    double ty = im->h/3;
    imsetsize(im, 512, 512, 3, 1);
    imzero(im);
    for (i=0; i<im->h; ++i) {
      for (j=0; j<im->w; ++j) {
        double x = (j-(im->w/2))/tx;
        double y = (i-(im->h/2))/ty;
        double inputs[nTestInputs] = {x, y};
        double output = compute(inputs, weights);  
        int cluster_idx = output<0;
        COLOR clr = color_tab[cluster_idx];
        imdraw_SetPixel(im, 0, j, i, _RGB_A(clr, 10));
      }
    }
    for (i=0; i<nTests; ++i) {
      int cluster_idx = slps[i].output<0;
      COLOR clr = color_tab[cluster_idx];
      double x = slps[i].inputs[0];
      double y = slps[i].inputs[1];
      x = x*(im->w/2)+(im->w/2);
      y = y*(im->h/2)+(im->h/2);
      imdrawaa_ellipse(im, 0, x, y, 2, 2, 0, clr, 1);
    }
    imshow(im);
    cvWaitKey(-1);
  }
  for(w = 0 ; w < nTestInputs + 1 ; ++w)
  {
    printf("weight%f:%f\n", w, weights[w]);
  }
  printf("\n");  
  for (i = 0 ;i < nTests ; ++i)  
  {
    double tt;
    printf("rightresult��%d\t", slps[i].output);  
    tt = compute(slps[i].inputs,weights);
    printf("caculateresult:%f\n", tt);  
  }  
  
  return 0;  
}  


