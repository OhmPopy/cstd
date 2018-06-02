
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include "svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

// ��������:    k_function
// ��������:    ����RBF�˺��������ڻ�
// ����:        �������������x,��������ڻ���֧������,ģ�Ͳ���
// ����ֵ��     �ڻ�ֵ
// ��ע��       ��
double k_function(const svm_node* x, const svm_node* y, const svm_parameter* param)   //�������������x,��i��֧������,����
{
  //RBF�˼����ڻ�
  double sum = 0;

  while (x->index != -1 && y->index != -1) { //�ж������Ƿ��ȡ����,��Ҳ��Ϊʲô��ǰ�����ڴ��ʱ��������
    //l���ڵ���ڴ��Լ�Ϊʲôÿ�ζ�ȡ��һ��֮��ͻ�ѽ�������һ��
    if (x->index == y->index) {       //�ڵ��index��Ϊ-1��ԭ��
      double d = x->value - y->value;
      sum += d * d;
      ++x;
      ++y;
    }
    else {                    //���ĳ������������ֵΪ0,�ͻ�ʡ�����index,��ʱֱ������һ������ֵ��ƽ��
      if (x->index > y->index) {
        sum += y->value * y->value;
        ++y;
      }
      else {
        sum += x->value * x->value;
        ++x;
      }
    }
  }

  while (x->index != -1) { // �����ĳ����������������һ���̾�ִ����������whileѭ��
    sum += x->value * x->value;
    ++x;
  }

  while (y->index != -1) {
    sum += y->value * y->value;
    ++y;
  }

  return exp(-param->gamma * sum); //exp������math.h����

}

// ��������:    svm_predict_values
// ��������:    �ж������x�������������ĸ�����
// ����:        ģ�ͽṹ��,��������x,dec_values����0����һ��,С��0������һ��
// ����ֵ��     labelֵ
// ��ע��       ��
double svm_predict_values(const svm_model* model, const struct svm_node* x, double* dec_values)
{

  int i, j, p;
  int nr_class = model->nr_class;
  int l = model->l;  //֧�������ĸ���
  int* start;
  int* vote;
  int vote_max_idx;
  double* kvalue = Malloc(double, l);

  for (i = 0; i < l; i++) {
    kvalue[i] = k_function(x, model->SV[i], model->param);  //���ú˺��������ڻ�,��������x,��i��֧������
  }

  //SV[i]�����i��֧��������ָ��
  start = Malloc(int, nr_class);
  start[0] = 0;

  for (i = 1; i < nr_class; i++) { //nSV[i] ÿһ��֧�������ĸ���
    start[i] = start[i - 1] + model->nSV[i - 1];  //start[i]���ŵ��ǵ�i������һ��֧������
  }

  vote = Malloc(int, nr_class);

  for (i = 0; i < nr_class; i++) {
    vote[i] = 0;  //ÿ�����ͶƱ��������Ϊ0
  }

  p = 0;

  for (i = 0; i < nr_class; i++) { //��������forѭ�������ÿ�����ͶƱ,��nr_class*(nr_class-1)/2��ͶƱ
    for (j = i + 1; j < nr_class; j++) {
      double sum = 0;
      int si = start[i];  //��i���ĵ�һ��֧������
      int sj = start[j];  //��j���ĵ�һ��֧������
      int ci = model->nSV[i]; //��i���֧�������ĸ���
      int cj = model->nSV[j]; //��j���֧�������ĸ���

      int k;
      double* coef1 = model->sv_coef[j - 1]; //sv_coef[m][l],mΪ�ܵ��������1,lΪ֧�������ĸ���
      double* coef2 = model->sv_coef[i];

      for (k = 0; k < ci; k++) {
        sum += coef1[si + k] * kvalue[si + k];  //��i��֧�������ĺ��ڻ�����Ӧ��alpha���
      }

      for (k = 0; k < cj; k++) {              //�����alphaʵ������alpha��yi�ĳ˻�,����Ҳ�и�ֵ
        sum += coef2[sj + k] * kvalue[sj + k];  //��j��֧�������ĺ��ڻ�����Ӧ��alpha���
      }

      sum -= model->rho[p];                  //��ȥ��Ӧ��b,b�Ĵ��˳��Ϊ��0����1����������0����2.....
      dec_values[p] = sum;                   //��ȥb����Ϊ�����ŵ���bȡ�������

      if (dec_values[p] > 0) {    //��������������i��,��������j��
        ++vote[i];  //����0������i�໹��j��ѵ����ʱ����Ѿ�������(������Ϊ1,������Ϊ-1)
      }
      else {                     //�����Ͻ�Ӧ���Ǵ���1����һ��,С��-1������һ��
        ++vote[j];
      }

      p++;
    }
  }

  vote_max_idx = 0;

  for (i = 1; i < nr_class; i++) {
    if (vote[i] > vote[vote_max_idx]) {
      vote_max_idx = i;  //�õ�ͶƱ���������
    }
  }


  free(kvalue);
  free(start);
  free(vote);
  return model->label[vote_max_idx]; //������Ӧ��label
}

// ��������:    svm_predict_values
// ��������:    �ж������x�������������ĸ�����
// ����:        ģ�ͽṹ��,��������x
// ����ֵ��     labelֵ
// ��ע��       ��
double svm_predict(const svm_model* model, const struct svm_node* x)  //����Ԥ���labelֵ
{
  int nr_class = model->nr_class;
  double* dec_values;
  double pred_result;

  dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
  pred_result = svm_predict_values(model, x, dec_values);
  free(dec_values);
  return pred_result;
}

// ��������:    sigmoid_predict
// ��������:    ���㹫ʽ���rij
// ����:        Ԥ��ֵ,A��B
// ����ֵ��     rij
// ��ע��       ����Ԥ��Ԥ���ʱ����ú���
static double sigmoid_predict(double decision_value, double A, double B)
{
  double fApB = decision_value * A + B;

  if (fApB >= 0) {
    return exp(-fApB) / (1.0 + exp(-fApB));
  }
  else {
    return 1.0 / (1 + exp(fApB)) ;
  }
}

// ��������:    multiclass_probability
// ��������:    �������
// ����:        Ԥ��ֵ,A��B
// ����ֵ��     rij
// ��ע��       ����Ԥ��Ԥ���ʱ����ú���
static void multiclass_probability(int k, double** r, double* p)
{
  int t, j;
  int iter = 0, max_iter = max(100, k);
  double** Q = Malloc(double*, k);
  double* Qp = Malloc(double, k);
  double pQp, eps = 0.005 / k;

  for (t = 0; t < k; t++) { //�����Q����
    p[t] = 1.0 / k; // Valid if k = 1
    Q[t] = Malloc(double, k);
    Q[t][t] = 0;

    for (j = 0; j < t; j++) {
      Q[t][t] += r[j][t] * r[j][t];
      Q[t][j] = Q[j][t];
    }

    for (j = t + 1; j < k; j++) {
      Q[t][t] += r[j][t] * r[j][t];
      Q[t][j] = -r[j][t] * r[t][j];
    }
  }

  for (iter = 0; iter < max_iter; iter++) {
    double max_error;
    // stopping condition, recalculate QP,pQP for numerical accuracy
    pQp = 0;

    for (t = 0; t < k; t++) {
      Qp[t] = 0;

      for (j = 0; j < k; j++) {
        Qp[t] += Q[t][j] * p[j];
      }

      pQp += p[t] * Qp[t];
    }

    max_error = 0;

    for (t = 0; t < k; t++) {
      double error = fabs(Qp[t] - pQp);

      if (error > max_error) {
        max_error = error;
      }
    }

    if (max_error < eps) {
      break;  //�ж��Ƿ�������ֹ����
    }

    for (t = 0; t < k; t++) {
      double diff = (-Qp[t] + pQp) / Q[t][t];
      p[t] += diff;
      pQp = (pQp + diff * (diff * Q[t][t] + 2 * Qp[t])) / (1 + diff) / (1 + diff);

      for (j = 0; j < k; j++) {
        Qp[j] = (Qp[j] + diff * Q[t][j]) / (1 + diff);
        p[j] /= (1 + diff);
      }
    }
  }

  for (t = 0; t < k; t++) {
    free(Q[t]);
  }

  free(Q);
  free(Qp);
}

// ��������:    svm_predict_probability
// ��������:    ���ʹ���,���ʹ��ƵĽ�������prob_estimates��
// ����:        ģ�ͽṹ��,��������x,���ʹ��ƵĽ������
// ����ֵ��     labelֵ
// ��ע��       ��
double svm_predict_probability(const svm_model* model, const svm_node* x, double* prob_estimates)
{
  if ((model->param->svm_type == C_SVC) &&
      model->probA != NULL && model->probB != NULL) {
    int i, j;
    int nr_class = model->nr_class;
    double* dec_values = Malloc(double, nr_class * (nr_class - 1) / 2);
    double min_prob;
    double** pairwise_prob;
    int k;
    int prob_max_idx;

    svm_predict_values(model, x, dec_values);

    min_prob = 1e-7;
    pairwise_prob = Malloc(double*, nr_class);

    for (i = 0; i < nr_class; i++) {
      pairwise_prob[i] = Malloc(double, nr_class);
    }

    k = 0;

    for (i = 0; i < nr_class; i++) { //�õ�rij
      for (j = i + 1; j < nr_class; j++) {
        pairwise_prob[i][j] = min(max(sigmoid_predict(dec_values[k], model->probA[k], model->probB[k]), min_prob), 1 - min_prob);
        pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
        k++;
      }
    }

    multiclass_probability(nr_class, pairwise_prob, prob_estimates);

    prob_max_idx = 0;

    for (i = 1; i < nr_class; i++) {
      if (prob_estimates[i] > prob_estimates[prob_max_idx]) {
        prob_max_idx = i;
      }
    }

    for (i = 0; i < nr_class; i++) {
      free(pairwise_prob[i]);
    }

    free(dec_values);
    free(pairwise_prob);
    return model->label[prob_max_idx];
  }
  else {
    return svm_predict(model, x);
  }
}

static const char* svm_type_table[] = { //ģ�͵�����
  "c_svc", NULL
};

static const char* kernel_type_table[] = { //�˺���������
  "rbf", NULL
};


static char* line = NULL;  //�洢ÿ�ζ�ȡ���ַ���
static int max_line_len;   //һ��������Ϊ1024

// ��������:    readline
// ��������:    �������file�ļ��ж�ȡһ���ַ���
// ����:        file�ļ�
// ����ֵ��     �ַ���
// ��ע��       ��
static char* readline(FILE* input)
{
  int len;

  //fgets�ķ���ֵΪ��ȡ�ַ�����ָ��,���û�ж�ȡ���ַ�����ΪNULL
  if (fgets(line, max_line_len, input) == NULL) { //��input�ж�ȡ�������Ϊmax_line_len���ַ�����line��
    return NULL;  //�������з�����(�����а������з�)
  }

  while (strrchr(line, '\n') == NULL) { //strrchr�������ڲ���'\n'����ַ���line�ַ������濪ʼ��һ�γ��ֵ�λ��
    //��Ϊ�գ�˵��1024�Ŀռ�洢��������ַ���,��������������Ŀռ�
    max_line_len *= 2;
    line = (char*) realloc(line, max_line_len);
    len = (int) strlen(line);

    if (fgets(line + len, max_line_len - len, input) == NULL) { //�ж��ַ����Ƿ��ȡ��,û�еĻ������ѭ�����Ӵ洢�ռ�
      break;
    }
  }

  return line;
}

// ��������:    svm_load_model
// ��������:    ��.model�ļ��м���ģ��
// ����:        ģ�͵��ļ���
// ����ֵ��     ģ��
// ��ע��       ��
svm_model* svm_load_model(const char* model_file_name)
{
  int m, l, i, j, k;
  int elements;
  long pos;
  svm_model* model;
  svm_parameter* param;
  svm_node* x_space;
  char cmd[81];
  FILE* fp;
  char* p, *endptr, *idx, *val;

  fp = fopen(model_file_name, "rb");

  if (fp == NULL) {
    return NULL;
  }

  // read parameters
  model = Malloc(svm_model, 1); //�����ڴ�
  param = model->param;
  model->rho = NULL;  //�൱���б����е�b
  model->probA = NULL;
  model->probB = NULL;
  model->label = NULL; //��� label[n]
  model->nSV = NULL;  //ÿһ�����֧�������ĸ���

  ////////����c_svcģ�Ͳ�ʹ��RBF�˺����ķ���������,����whileѭ����ģ�͵�����,ģ�͵ĺ˺�������,////////////////
  ////////////RBF�˺�����gamma����,�ܵ������nr_class,�ܵ�֧����������total_sv,rhoҲ�����б�ʽ��b��ֵ///////////
  /////////////һ��n*(n-1)/2��,�������������,ÿ�����֧�������ĸ���,����ֵ��ģ�Ͷ�Ӧ����////
  while (1) {
    fscanf(fp, "%80s", cmd); //��fpģ���ж�ȡ80���ַ���cmd��,�����ո������ȡ

    if (strcmp(cmd, "svm_type") == 0) { //�����ȡ�����ַ���Ϊsvm_type,�������Ͷ�ȡ��ʲô���͵�ģ��
      int i;
      fscanf(fp, "%80s", cmd); //��fpģ�����ٶ�ȡ80���ַ�,�����ո������ȡ

      for (i = 0; svm_type_table[i]; i++) {
        if (strcmp(svm_type_table[i], cmd) == 0) { //svm_type_table�Ǹ��ַ�ָ������
          //ָ��"c_svc","nu_svc","one_class","epsilon_svr","nu_svr"
          param->svm_type = i;   //�Ѷ�ȡ����ģ�����͸�ֵ��ģ�͵Ĳ���
          break;
        }
      }

      if (svm_type_table[i] == NULL) { //���û�ж�ȡ���κ�ģ��,�ͷ���
        fprintf(stderr, "unknown svm type.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if (strcmp(cmd, "kernel_type") == 0) { //�ڶ���ѭ������ȡ80���ַ���cmd��,����kernel_type�Ƚ�
      int i;
      fscanf(fp, "%80s", cmd);

      for (i = 0; kernel_type_table[i]; i++) {
        if (strcmp(kernel_type_table[i], cmd) == 0) {
          param->kernel_type = i;     //��ȡģ���ļ��е�kernel_type,��ʼ��ģ��
          break;
        }
      }

      if (kernel_type_table[i] == NULL) {
        fprintf(stderr, "unknown kernel function.\n");
        free(model->rho);
        free(model->label);
        free(model->nSV);
        free(model);
        return NULL;
      }
    }
    else if (strcmp(cmd, "probA") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->probA = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->probA[i]);
      }
    }
    else if (strcmp(cmd, "probB") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->probB = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->probB[i]);
      }
    }
    else if (strcmp(cmd, "gamma") == 0) {
      fscanf(fp, "%lf", &param->gamma);
    }
    else if (strcmp(cmd, "nr_class") == 0) {
      fscanf(fp, "%d", &model->nr_class);
    }
    else if (strcmp(cmd, "total_sv") == 0) {
      fscanf(fp, "%d", &model->l);
    }
    else if (strcmp(cmd, "rho") == 0) {
      int i, n;
      n = model->nr_class * (model->nr_class - 1) / 2;
      model->rho = Malloc(double, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%lf", &model->rho[i]); //ÿһ������������b
      }
    }
    else if (strcmp(cmd, "label") == 0) {
      int i, n = model->nr_class;
      model->label = Malloc(int, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%d", &model->label[i]);  //ÿ��������õ�label
      }
    }


    else if (strcmp(cmd, "nr_sv") == 0) {
      int i, n = model->nr_class;
      model->nSV = Malloc(int, n);

      for (i = 0; i < n; i++) {
        fscanf(fp, "%d", &model->nSV[i]);  //ÿ�����֧�������ĸ���
      }
    }
    else if (strcmp(cmd, "SV") == 0) { //���else ifֻ���ö�ȡ����SV��һ��
      while (1) {
        int c = getc(fp);   //��fp�ж�ȡһ���ַ�

        if (c == EOF || c == '\n') {
          break;
        }
      }

      break;
    }
    else {
      fprintf(stderr, "unknown text in model file: [%s]\n", cmd);
      free(model->rho);
      free(model->label);
      free(model->nSV);
      free(model);
      return NULL;
    }
  }

  // read sv_coef and SV
  //��ȡ֧���������������ճ��Ӻ�֧������
  elements = 0;
  pos = ftell(fp);   //��ȡ�ļ�fp�ĳ���,���ļ���ʼ����ǰλ��
  max_line_len = 1024;
  line = Malloc(char, max_line_len);

  while (readline(fp) != NULL) { //readline���������fp��ȡ����λ�ý������¶�,����'\n'����,�ַ�������line��
    //ÿһ��whileѭ����ȡһ���ַ���
    p = strtok(line, ":"); //�ֽ��ַ���,�ָ���Ϊ':'��ע��:ִ��һ��ֻ�ڵ�һ��:���ָ�,�ѵ�һ��:ǰ���ַ�����ȡ����

    //printf("%s",p);
    while (1) {
      p = strtok(NULL, ":"); //�ڶ���ִ��,��ѵ�һ���͵ڶ���:֮����ַ�����ȡ����

      //printf("%s",p);
      if (p == NULL) {
        break;
      }

      ++elements;  //ͳ���������ܸ���=������ά��*֧���������ܸ���l
    }
  }

  elements += model->l;  //elements�ĸ�������֧�������ĸ���

  fseek(fp, pos, SEEK_SET); //SEEK_SET=0,�����ļ�ָ��stream��λ��,��SEEK_SET��ʼ��posΪƫ����,ִ����֮��fpָ���ļ�β
  //SEEK_SET��ʾ�ļ���ͷ
  m = model->nr_class - 1 ;
  l = model->l;
  model->sv_coef = Malloc(double*, m);  //sv_coef�൱���б�ʽ�е�alpha

  for (i = 0; i < m; i++) {
    model->sv_coef[i] = Malloc(double, l); //sv_coef[i]��Ӧ��i��alpha
  }

  model->SV = Malloc(svm_node*, l);    //SV����֧��������ָ��
  x_space = NULL;

  if (l > 0) {
    x_space = Malloc(svm_node, elements);
  }

  j = 0;

  for (i = 0; i < l; i++) {
    readline(fp);     //��ȡ��һ���ַ���line��
    model->SV[i] = &x_space[j];
    p = strtok(line, " \t");   //�Կո�ָ��ַ���
    //strtod�����ַ���ת���ɸ������� ,���ת����ȷ�ͻ��endptr��ֵΪNULL
    //������ַ������һ����ַ�ĵ�ַ������
    model->sv_coef[0][i] = strtod(p, &endptr); //��Pת����float��ֵ��sv_coef[0][i]

    for (k = 1; k < m; k++) {
      p = strtok(NULL, " \t");
      model->sv_coef[k][i] = strtod(p, &endptr); //sv_coef[m][l],mΪ�ܵ��������1,lΪ֧�������ĸ���
    }

    while (1) {
      idx = strtok(NULL, ":");//��ȡ������
      val = strtok(NULL, " \t");//��ȡ������

      if (val == NULL) {
        break;
      }

      x_space[j].index = (int) strtol(idx, &endptr, 10);
      x_space[j].value = strtod(val, &endptr);

      ++j;
    }

    x_space[j++].index = -1;//ÿ��ȡ��һ�е����������ݾͰѽ�����һ���ڵ��������Ϊ-1
  }

  free(line);

  if (ferror(fp) != 0 || fclose(fp) != 0) {
    return NULL;
  }

  model->free_sv = 1; // XXX
  return model;  //����ģ��
}

// ��������:    svm_destroy_model
// ��������:    �ͷ�ģ��
// ����:        ģ����
// ����ֵ��     ��
// ��ע��       ��
void svm_destroy_model(svm_model* model)  //�ͷ�ģ��
{
  int i;

  if (model->free_sv && model->l > 0) {
    free((void*)(model->SV[0]));
  }

  for (i = 0; i < model->nr_class - 1; i++) {
    free(model->sv_coef[i]);
  }

  free(model->SV);
  free(model->sv_coef);
  free(model->rho);
  free(model->label);
  free(model->nSV);
  free(model->probA);
  free(model->probB);
  free(model);
}
