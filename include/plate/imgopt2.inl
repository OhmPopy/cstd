
//#include "img/imgopt.inl"

// 8��ͨ��Ѱ��
int my_connect_domain(const img_t* I_bw, img_t* I_ind)
{
  int i, ii, jj, row, col, Top_left, ind;
  int ve[4];
  int vect[4];
  row = I_bw->h, col = I_bw->w;
  // ��ΪI_bw���ܱ����о�Ϊ0  ����ǰ���������������

  // �����Ͻǿ�ʼɨ��
  imsetsize(I_ind, row, col, 1, 1);// �洢Ϊ1�ı��
  ind = 1;
  ve[0] = -I_ind->s - 1, ve[1] = -I_ind->s, ve[2] = -I_ind->s + 1, ve[3] = -1;

  for (ii = 2; ii <= row - 1; ++ii) {
    for (jj = 2; jj <= col - 1; ++jj) {
      if (I_bw->tt.data[(ii - 1) * I_bw->s + jj - 1] == 255) {
        uchar* pos = I_ind->tt.data + (ii - 1) * I_ind->s + jj - 1;
        Top_left = pos[ve[0]] + pos[ve[1]] + pos[ve[2]] + pos[ve[3]];

        if (Top_left) {
          for (i = 0; i < 4; ++i) {
            vect[i] = pos[ve[i]];

            if (!vect[i]) {
              vect[i] = 255;
            }
          }

          *pos = MIN4(vect[0], vect[1], vect[2], vect[3]);

          if (255 == *pos) {
            int asdf = 0;
          }
        }
        else {
          *pos = ind;
          ind = ind + 1;
        }
      }
    }
  }

  //imshow(I_ind); cvWaitKey(-1);
  // �����½ǿ�ʼɨ��
  ve[0] = I_ind->s - 1, ve[1] = I_ind->s, ve[2] = I_ind->s + 1, ve[3] = 1;

  for (ii = row - 1; ii >= 2; --ii) {
    for (jj = col - 1; jj >= 2; --jj) {
      if (I_bw->tt.data[(ii - 1) * I_bw->s + jj - 1] == 255) {
        uchar* pos = I_ind->tt.data + (ii - 1) * I_ind->s + jj - 1;
        //Top_left = I_ind(ii+1,jj-1) + I_ind(ii+1,jj) + I_ind(ii+1,jj+1) + I_ind(ii,jj+1);
        Top_left = pos[ve[0]] + pos[ve[1]] + pos[ve[2]] + pos[ve[3]];

        if (Top_left) {
          for (i = 0; i < 4; ++i) {
            vect[i] = pos[ve[i]];

            if (!vect[i]) {
              vect[i] = 255;
            }
          }

          *pos = MIN4(vect[0], vect[1], vect[2], vect[3]);
        }
      }
    }
  }

  //FPRINT2D("aaa.txt", "%1d,", "\r\n", row, col, I_ind->tt.data, col, 1);
  return ind - 1;
}

int juxingkuang(const img_t* e, IRECT* pq, int maxpq)
{
  img_t I_ind[1] = {0};
  int geshu; //���ο����
  int row, col, ii, jj; //
  // ���ο�
  //�����еķ��ϸ����ʹ������Ӿ��ο򷨻��ÿ�����ε��ĸ�����
  //���룺 e ��ֵ���󣬰�ɫ����Ϊ���ϸ��������
  //����� pq ������󣬴�СΪn*4������nΪ���θ�����4��ʾ���ε����������ĸ�����

  // ��ΪI_bw���ܱ����о�Ϊ0  ����ǰ���������������
  row = e->h, col = e->w;
  // �����Ͻǿ�ʼɨ��
  geshu = my_connect_domain(e, I_ind);   // �洢Ϊ1�ı��

  //����˱��ͼƬ����ǵ���ֵ��ʾͼ��ľ��ο����
  if (geshu == 0) {
    //�����ھ��ο�
    imfree(I_ind);
    return 0;
  }

  geshu = MIN(geshu, maxpq);

  //�����������Ը���  �������������Թ���
  for (ii = 0; ii < maxpq; ii++) {
    pq[ii].t = pq[ii].l = 10000;
    pq[ii].b = pq[ii].r = 0;
  }

  for (ii = 2; ii < row; ++ii) {
    for (jj = 2; jj < col; ++jj) {
      int ch = I_ind->tt.data[(ii - 1) * I_ind->s + jj - 1] - 1;

      //���������˱߽磬���Եõ������궼Ҫ��ԭ1��λ��
      if (ch >= 0 && ch < maxpq) {
        pq[ch].l = MIN(pq[ch].l, jj - 2);
        pq[ch].t = MIN(pq[ch].t, ii - 2);
        pq[ch].r = MAX(pq[ch].r, jj);
        pq[ch].b = MAX(pq[ch].b, ii);
      }
    }
  }

  //������ٵı��
  for (ii = 0, jj = 0; ii < geshu; ii++) {
    if (pq[ii].b > pq[ii].t && pq[ii].r > pq[ii].l) {
      pq[jj++] = pq[ii];
    }
  }

  geshu = jj;
  imfree(I_ind);
  return geshu;
}

int juxingkuang2(int h, int w, uchar* img, int al, int cn, CvRect* pq, int maxpq)
{
  img_t im[1] = {0};
  int i, ret = 0;
  IRECT* ss = (IRECT*)pq;
  IMINIT(im, h, w, img, al, cn, 1);
  ret = juxingkuang(im, ss, maxpq);

  for (i = 0; i < ret; ++i) {
    IRECT s0 = ss[i];
    pq[i] = cvRect(s0.l, s0.t, RCW(&s0), RCH(&s0));
  }

  return ret;
}