// ����L0����ƽ����ͼ��������Ч�����㷨
// ͼ��������Ч�㷨����Ҫ˼·���Է������֣���һ����ͼ�񣬼�ȥ��ͼ��ϸ�ڣ�������ͼ����Ҫ��Ϣ���ڶ������ɺ��ʵı�Ե������������ߺϳɼ��ɣ��ϳ���ʵ���ǽ����߼�һ�˼��ɡ��ڼ�ͼ���裬����meanshift�㷨�⣬��ҿ϶����뵽�ܶ��Ե���ֵ�ͼ��ƽ���㷨�����羭���˫���˲��������˲��ȵȡ��������ݿ��Բ���������������ṩ�˶��ֱ�Ե����ƽ���㷨����Ҫ����˫���˲�����任��WLS�������˲���L0����ƽ���� ��Ч���Ա��Լ�matlab���룬�㷨�ܶ࣬���Ǳ�����ΪL0����ƽ���㷨�Ƚ��ʺ���������Ч��ƽ���Ľ��ͼ���Ÿ��в�θС�����L0����ƽ���㷨����ҿ��Բο�ԭ�������Լ���ƪ���� �������ﲻ��׸����L0����ƽ���㷨ԭ����ʽ�Ƶ��ȽϺ���⣬����ʵ�������Ƚϸ��ӣ���Ҫ�õ�����Ҷ�任�Լ�������ⷽ�̡�
// ͼ�񾭹�ƽ���򻯺󣬱�Ե���Ч��Ҳ����á��������µ������Ե�����������٣���⵽��ͼ���Ե������ᣬ�����С����˼�ͼ�ͱ�Եͼ����ô�����Ϳ�������������Ч�ˡ���ͳ����Ϊֱ�ӽ����ߺϳɣ�������ͼ�������Ե�����ֱ�Ӻϳɡ����ǣ����ڼ��ı�Ե��������Ӳ��ȱ��ƽ���ıʴ�Ч��������ֱ�Ӻϳɵ�������Ч���������Ǻ����롣���㷨����Ҫ���µ��������ȡ�ı�Ե�����ϣ��������߻��־����LIC�㷨���˲����������㷨�������ȡ�ı�Ե����ǳ�Բ��ƽ�����Ӿ��ϸ��ӽ������ֻ�Ч����
// ���������㷨��������߼����룬���������˽Ȿ�������������Ч�㷨˼·��


int ConvertToDouble(const img_t* src, double* rdata, double* gdata, double* bdata) {
  int i, j, k=0;
  for (i=0; i<src->h; ++i) {
    const uchar* src1 = img_row(uchar, src, i);
    for (j=0; j<src->w; ++j, src1 += src->c) {
      bdata[k] = src1[0];
      gdata[k] = src1[1];
      rdata[k] = src1[2];
      ++k;
    }
  }
  return 0;
}

int CartoonStylization(const img_t* src, img_t* dst, int block_count) {
  int width = src->w;
  int height=  src->h;  
  int size = width*height;  
  int mem_size = size * sizeof(double);  
  double *rdata = (double *)malloc(mem_size);  
  double *gdata = (double *)malloc(mem_size);  
  double *bdata = (double *)malloc(mem_size);  
  
  // ����ת��  
  ConvertToDouble(src, rdata, gdata, bdata);  
  
  // ��ϸ��  
  double *rgb_data[3] = { rdata, gdata, bdata };  
  L0ImageSmoothing(rgb_data, width, height, block_count, 0.01, 2.0);  
  
  // ��ȡ��Ե  
  rdata = rgb_data[0];  
  gdata = rgb_data[1];  
  bdata = rgb_data[2];  
  double *gray_data = (double *)malloc(mem_size);  
  double *hor_edgedata = (double *)malloc(mem_size);  
  double *ver_edgedata = (double *)malloc(mem_size);  
  for (int i = 0; i < size; i++)  
  {  
    gray_data[i] = (rdata[i]*0.299f + gdata[i]*0.587f + bdata[i]*0.114f);  
  }  
  EdgeDetectionFilter(gray_data, width, height, hor_edgedata, ver_edgedata);  
  
  for (int i = 0; i < size; i++)  
  {  
    double edge_val = (fabs(hor_edgedata[i]) + fabs(ver_edgedata[i]))*1.2;  
    edge_val = edge_val < 1.0 ? 1.0 - edge_val : 0.0;  
    gray_data[i] = edge_val;  
  }  
  
  double *edge_data = (double *)malloc(mem_size);  
  ExtractEdge(gray_data, edge_data, width, height, 15.7, 0.017, 75.5f);  
  
  // ����ʸ����  
  double *vec_x = (double *)malloc(mem_size);  
  double *vec_y = (double *)malloc(mem_size);  
  CalcVectorField(gray_data, width, height, vec_x, vec_y, 1.0);  
  
  // �߻��־��  
  LICFilter(edge_data, gray_data, vec_x, vec_y, width, height);  
  
  free(vec_x);  
  free(vec_y);  
  vec_x = NULL;  
  vec_y = NULL;  
  free(edge_data);  
  edge_data = NULL;  
  
  // ����ϳ�  
  for (int i = 0; i < size; i++)  
  {  
    rdata[i] *= gray_data[i];  
    gdata[i] *= gray_data[i];  
    bdata[i] *= gray_data[i];  
  }  
  
  // ����ת��  
  ConvertToUchar(rdata, gdata, bdata, src);  
  
  // Ч��΢��  
  ImageAdjust(src);  
  
  free(gray_data);  
  free(hor_edgedata);  
  free(ver_edgedata);  
  gray_data = NULL;  
  hor_edgedata = NULL;  
  ver_edgedata = NULL;  
  
  free(rdata);  
  free(gdata);  
  free(bdata);  
  rdata = NULL;  
  gdata = NULL;  
  bdata = NULL;  
  
  return NULL;  
}  

��Ȼ�����㷨Ҳ���о����Եģ������˾�һ�����е��ʺ������е��ʺϷ羰�������㷨���ڱ�����ǰ����һЩ��ͼЧ���ȽϺã����ͼ�񱳾�����ǰ���Ƚ����ң����ɵĽ�����ܲ�̫���롣������һЩЧ��ͼ��