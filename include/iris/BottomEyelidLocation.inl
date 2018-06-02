

int BottomEyelidLocation(buf_t* bf, const img_t* img, int nr, int wr, int polar_nx, int polar_ny, int ss, double* canshu123)
{
  img_t BottomEyelidROI[1] = {0}, I5[1] = {0};
  int ii, jj, kk, length_point, num_point, ROI_row, ROI_col;
  int h = img->h, w = img->w;

  length_point = 0;           // ѡȡ����iris �ڲ��ĵ�ĸ���
  num_point = 0;              // �ж��Ƿ����ڵ��ļ���

  bf_imsetsize(bf, BottomEyelidROI, h, w, 1, 1);
  imclone(img, BottomEyelidROI);

  ROI_row = BottomEyelidROI->h, ROI_col = BottomEyelidROI->w;

  horizontal_filter(bf, img, 0, I5);

  // I4Ϊdouble  I5Ϊlogical��
  // 6_1 �����ж�����I4ͼ���pupil��iris���ڲ������ж��Ƿ������������ڵ�
  // ע�⣺ ���ڵ�ʱ�ض��к�ǿ��canny���߼�⵽��Ҳ���ǿ��Զ�I4����(nr+10,wr+2)��Χ�ڽ���I4==1ͳ�Ƽ���
  for (jj = 5; jj <= ROI_col - 5; ++jj) {
    for (kk = round(2 * ROI_row / 3.); kk <= ROI_row; ++kk) {
      if (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128) {
        double diffdist = sqrt(SQR(kk - polar_ny) + SQR(jj - polar_nx));

        if ((diffdist > nr + 10) && (diffdist < wr + 2)) {
          num_point = num_point + 1;
        }
      }
    }
  }

  //imshow(I5);cvWaitKey(-1);
  if (num_point <= 70) {              // �����и���Ҫ�Ĳ����趨��
    //z = 1;
    //a[0].x=1, a[0].y=1;
    canshu123[0] = canshu123[1] = canshu123[2] = 0;
  }
  else {
    IPOINT* a_pt = 0;
    BFMALLOC(bf, a_pt, 1000);

    // ÿ��ѡ��һ����Ե��   û�еı�Ե�� all_edgepointΪ0
    for (jj = 5; jj <= ROI_col - 5; ++jj) {
      for (kk = (2 * ROI_row / 3); kk <= ROI_row; ++kk) {
        if (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128) {
          double diffdist = FastDistance2D((kk - polar_ny), (jj - polar_nx));

          if (length_point < 1000 && (diffdist > nr + 10) && (diffdist < wr + 5)) {
            // diffabs(1,jj) = kk;
            // all_edgepoint(1,jj) = kk;
            a_pt[length_point].x = jj;
            a_pt[length_point].y = kk;
            BottomEyelidROI->tt.data[BottomEyelidROI->s * (kk - 1) + jj - 1] = 255;
            length_point = length_point + 1;
            break;
          }
        }

        I5->tt.data[I5->s * (kk - 1) + jj - 1] = 0;
      }
    }

#if 0
    {
      int ii;
      imshow(BottomEyelidROI);
      cvWaitKey(-1);
      my_two_polyfit(a_pt, length_point, canshu123);

      for (ii = 0; ii < ROI_row; ++ii) {
        double x = ii + 1;
        int z = (int)(canshu123[0] * (x * x) + canshu123[1] * x + canshu123[2]);

        if (z > 0 && z < ROI_row) {
          //z[ii] = ROI_row;
          BottomEyelidROI->tt.data[BottomEyelidROI->s * z + ii] = 255;
        }
      }
    }
    imshow(BottomEyelidROI);
    cvWaitKey(-1);

#endif

    for (ii = 0; ii < jj; ++ii) {
      a_pt[ii].x = a_pt[ii].x * ss >> 8;
      a_pt[ii].y = a_pt[ii].y * ss >> 8;
    }

    my_two_polyfit(a_pt, length_point, canshu123);
    BFFREE(bf, a_pt, 1000);
  }


  //imshow(img); imshow(I5); //cvWaitKey(-1);
  bf_imfree(bf, I5);
  bf_imfree(bf, BottomEyelidROI);

  return 0;
}
