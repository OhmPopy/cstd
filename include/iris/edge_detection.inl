
//#include "../cv/cvcanny.inl"
int edge_detection(buf_t* bf, const img_t* polarv, int Rmax, IPOINT* p)
{
  img_t I2[1] = {0}, I3[1] = {0}, orient_img[1] = {0}, polarv_bw[1] = {0};
  img_t polarv_op[1] = {0}, I_edge[1] = {0}, I_edge_bw[1] = {0};
  double scaling = 1.0,    sigma = 1,    vert = 1.0,    horz = 0.0;
  int i, j, n, edge_number, point, Number_sample_edge, i_cs, avg_x;
  IPOINT* Final_good_edge = 0;
  IPOINT* edge_location = 0;
  int* duan_dian = 0;
  int* line_length = 0;
  int canshu2;
  int maxpts = 0;
  int h = polarv->h, w = polarv->w;
  int buflen = bf->len;

  // p_col Ϊ�������Բ��ȡ��ļ�����������
  // p_row Ϊ�������Բ��ȡ��ļ�����������
  // Number_sample_edge Ϊ�������Բ��ȡ��ĸ���
  // polarv Ϊ������ͼ��, RmaxΪ������뾶���ֵ

  bf_imsetsize(bf, polarv_bw, h, w, 1, 1);

  // ��Ե��⼰��ʾ
  //FPRINT2D("polarv.txt", "%3d,", "\r\n", polarv->h, polarv->w, polarv->u8, polarv->w, 1);
  im2bw_adapt(polarv, 40, polarv_bw, 0, 255);
  // polarv1 = bwareaopen(polarv, 400, 4);    // 7_20
  my_areaopen(bf, polarv_bw, 1400, polarv_op);
  n = polarv_bw->h * polarv_bw->w;
  //imshow(polarv);imshow(polarv_bw);imshow(polarv_op);cvWaitKey(-1);
  //FPRINT2D("polarv_bw.txt", "%3d,", "\r\n", polarv_bw->h, polarv_bw->w, polarv_bw->u8, polarv_bw->w, 1);
  // I_edge = edge(polarv,'canny','vertical');   // 7_20
  // generate the edge image
  bf_imsetsize(bf, I_edge_bw, h, w, 1, 1);

  if (1) {
#if 0
    canny(polarv_op, sigma, scaling, vert, horz, I2, orient_img);    // edge detection 4-19
    adjgamma(I2, 1.9, I3);       // ��I2 ��ǿ ��normalize to [0, 1]  4-19
    nonmaxsup(I3, orient_img, 1.5, I_edge);
    //imsetsize(I_edge_bw, I_edge->h, I_edge->w, 1, 1);

    for (i = 0; i < n; ++i) {
      I_edge_bw->tt.data[i] = I_edge->tt.f8[i] < 0.3 ? 0 : 255;
    }

#else
    canny_fix(bf, polarv_op, sigma, scaling, vert, horz, 1.9, 1.5, I_edge);
    //imsetsize(I_edge_bw, I_edge->h, I_edge->w, 1, 1);

    for (i = 0; i < n; ++i) {
      I_edge_bw->tt.data[i] = I_edge->tt.u1[i] < (0.3 * 255) ? 0 : 255;
    }

#endif
  }

  //FPRINT2D("I_edge.txt", "%13.13f,", "\r\n", I_edge->h, I_edge->w, I_edge->tt.f8, I_edge->w, 1);
  // I_edge_bw = im2bw(I_edge, (int)(255*0.3));

  //imshow(polarv); imshow(polarv_bw); imshow(polarv_op); imshow(I_edge_bw); imshow(I_edge); cvWaitKey(-1);
  //��ͫ�ױ�Ե��
  edge_number = 0;          // 4_11 edge_number Ϊȡ�ı�Ե�������Ҳ����ȡ�ĽǶȸ���

  maxpts = 400;

  BFMALLOC(bf, Final_good_edge, 2 * maxpts);
  edge_location = Final_good_edge + maxpts;

  MEMSET(Final_good_edge, 0, 2 * maxpts);

  for (i = 0; i < 135; ++i) {       // 4_11 �����е�����һ���ֱ�Ե ÿ��Ҳ����ÿ���Ƕ���ֻȡһ����Ե��
    for (j = 5; j < Rmax - 5; ++j) {
      if (I_edge_bw->tt.data[I_edge_bw->s * i + j] == 255 && edge_number < maxpts) {
        edge_number = edge_number + 1;
        edge_location[edge_number - 1].y = i; // 4_8 �����ڼ������ϵ�λ��
        edge_location[edge_number - 1].x = j; // 4_8 �����ڼ������ϵ�λ��
        break;
      }
    }
  }

  for (i = 225; i < 360; ++i) { // 4_11 �����е�����һ���ֱ�Ե ÿ��Ҳ����ÿ���Ƕ���ֻȡһ����Ե��
    for (j = 5; j < Rmax - 5; ++j) {
      if (I_edge_bw->tt.data[I_edge_bw->s * i + j] == 255 && edge_number < maxpts) {
        edge_number = edge_number + 1;
        edge_location[edge_number - 1].y = i; // 4_8 �����ڼ������ϵ�λ��
        edge_location[edge_number - 1].x = j; // 4_8 �����ڼ������ϵ�λ��
        break;
      }
    }
  }

  //imshow(polarv_bw);imshow(polarv_op);imshow(polarv);imshow(I_edge_bw); cvWaitKey(-1);

  // ����ϵ�
  BFMALLOC(bf, duan_dian, 2 * maxpts);
  line_length = duan_dian + maxpts;
  // duan_dian   ��������߶ε������ ����Ϊ��׼ �ڶ����õ���p_row_y2��������������2��Ϊһ���ϵ�
  // line_length ��ŵ�ǰ�߶εĳ���
  MEMSET(duan_dian, 0, 2 * maxpts);
  point = 0;
  duan_dian[point] = 0;
  avg_x = 0;

  for (i = 0; i < edge_number; ++i) {
    avg_x += edge_location[i].x;
  }

  avg_x /= edge_number;

  for (i = 1; i < edge_number; ++i) {
    if (abs((edge_location[i].x - edge_location[i - 1].x)) > 2 && point < maxpts) {
      point = point + 1;
      duan_dian[point - 1] = i;
      line_length[point - 1] = i - duan_dian[point - 1];
    }
  }

  duan_dian[point + 1] = edge_number + 1;
  line_length[point] = edge_number + 1 - duan_dian[point];
  n = 0;  // 4-13 ѡ�еı�Ե��

  for (i = 0; i < point + 1; ++i) {
    if (line_length[i] > 50 * polarv->h / 350) { // 8 Ϊ�������ȣ�С��8���ȵ��߶ν�������
      for (j = duan_dian[i]; j < MIN(maxpts, duan_dian[i + 1] - 1); ++j) {
        if (abs(edge_location[j].x - avg_x) < 5) {
          Final_good_edge[n++] = edge_location[j];
        }
      }
    }
  }

#if 0
  {
    int k;
    img_t polarv2[1] = {0};
    bf_imsetsize(bf, polarv2, h, w, 1, 1);
    imclone(polarv, polarv2);

    for (k = 0; k < n; ++k) {
      i = Final_good_edge[k].y, j = Final_good_edge[k].x;
      polarv2->tt.data[polarv2->s * i + j] = 255;
    }

    imshow(polarv2);
    imshow(I_edge_bw);
    cvWaitKey(-1);
    bf_imfree(bf, polarv2);
  }
#endif
  // ȡ�ڱ�Ե��
  Number_sample_edge = 0;

  canshu2 = 5;

  if (n < canshu2) {
    i_cs = 0;
  }
  else {
    i_cs = canshu2;
  }

  //logclear();
  for (i = i_cs; i < n; i += canshu2) {     // 20 �ǿɵ���  ��Ҫ���ڸı�sample��Ķ���
    p[Number_sample_edge++] = Final_good_edge[i];
    //logprintf("%3d, %3d\n", Final_good_edge[i].x, Final_good_edge[i].y);
  }

  BFFREE(bf, duan_dian, 2 * maxpts);
  BFFREE(bf, Final_good_edge, 2 * maxpts);

  bf_imfree(bf, I2);
  bf_imfree(bf, I3);
  bf_imfree(bf, orient_img);
  bf_imfree(bf, I_edge);
  bf_imfree(bf, I_edge_bw);
  bf_imfree(bf, polarv_op);
  bf_imfree(bf, polarv_bw);

  ASSERT(buflen == bf->len);
  return Number_sample_edge;
}
