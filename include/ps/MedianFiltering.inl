// ����뾶��ֵ�˲�����չ���ٷֱ��˲�����O(1)ʱ�临�Ӷ��㷨
// ͼ������չ���������Ҹ���r������
// bָ��
int imext(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int cn, int r)
{
  int i = 0, j = 0, i1 = 0, j1 = 0;

  for (i = -r; i < h + r;) {
    //i1=i<0?-i: i>=h?(h+h-i);
    //memcpy();
  }

  return 0;
}

// �����㷨
// Input: Image X of size m*n, kernel radius r
// Output: Image Y OF the same size as X
//   Initialize kernel histogram H
//   for i=1 to m do
//     for j=1 to n do
//       for k=-r to r do
//         Remove X[i+k,j-r-1] from H
//         Add x[i+k,j+r] to H
//       end for
//       Y[i,j]<-median(H)
//     end for
//   end for
int MedianFiltering_Huang(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int r)
{
  int i, j, k, i1, j1;
  int H[256] = {0};
  int H_line[256] = {0};

  for (i = 0; i < r; ++i) {
    for (j = 0; j < r; ++j) {
      H_line[a[i * al + j]]++;
    }
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      for (k = -r; k <= r; ++k) {
        i1 = i + k;

        if (i1 >= 0 && i1 < h) {
          j1 = j - r - 1;

          if (j1 >= 0 && j1 < w) {
            H_line[a[i1 * al + j1]]--;
          }

          j1 = j + r;

          if (j1 >= 0 && j1 < w) {
            H_line[a[i1 * al + j1]]++;
          }
        }
      }
    }
  }

  return 0;
}
// ���8λ�Ҷ�ͼ��
// ��1�����Ժ����Ҳ����ֱ��ͼִ��һ�μӷ���
// ��2������ͬһ��ֱ��ͼִ��һ�μ�����ȥ�������������Ϣ��
// ��3���������º����ֱ��ͼ���ݼӵ���ֱ��ͼ�У��������256�μӷ���
// ��4��������Ч����ֱ��ͼ���ݴӺ�ֱ��ͼ�м�ȥ������Ҫ256�μ�����
// ��5����Ϊ�ҵ���ֱ��ͼ����ֵ��ƽ����Ҫ128�αȽϺ�127�μӷ���

// Input: Image X of size m*n, kernel radius r
// Output: Image Y OF the same size as X
//   Initialize kernel histogram H and column histograms h[1...n]
//   for i=1 to m do
//     for j=1 to n do
//       Remove X[i-r-1,j+r] from h[j+r]
//       Add x[i+r,j+r] to h[j+r]
//       H<-H+h[j+r]-h[j-r-1]
//       H<-H+h[j+r]-h[j-r-1]
//       Y[i,j]<-median(H)
//     end for
//   end for

