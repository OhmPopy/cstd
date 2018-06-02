/******************************************
void wbGW(float** rgbImg, float** chImg, int rows, int cols)
���ܣ�
  ��rbgͼ���Ҷ����������㷨(GW)����ƽ�����
������
  float** rgbImg - ͼ���rgb���ݣ�[rows][cols*3]
  float** chImg - ������ݣ���ƽ�⴦����ͼ�����ݣ�[rows][cols*3]
  int rows - ͼ�������
  int cols - ͼ�������

ʱ�䣺
  2011.9.20
*******************************************/
int wbGW(int rows, int cols, ucahr* rgbImg, int rgbstep, ucahr* chImg, int chstep)
{
  int i = 0;
  int j = 0;

  //1����R��G��B��ɫ������ƽ��ֵ
  float aveR = 0.0f;        //��¼R������ƽ��ֵ
  float aveG = 0.0f;        //��¼G������ƽ��ֵ
  float aveB = 0.0f;        //��¼B������ƽ��ֵ
  float k = 0.0f;          //��¼aveR��aveG��aveB��ƽ��ֵ

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      aveR = aveR + rgbImg[i][3 * j];
      aveG = aveG + rgbImg[i][3 * j + 1];
      aveB = aveB + rgbImg[i][3 * j + 2];
    }
  }

  aveR = aveR / (rows * cols);
  aveG = aveG / (rows * cols);
  aveB = aveB / (rows * cols);
  k = (aveR + aveG + aveB) / 3;

  //3�����ƽ��������ͼ������
  for (i = 0; i < rows; i++) {
    uchar* chLine = chImg + i * chstep;

    for (j = 0; j < cols; j++) {
      int r, g, b;
      r =  rgbImg[i][3 * j]  * k / aveR;  //�������R����
      g =  rgbImg[i][3 * j + 1] * k / aveG; //�������R����
      b =  rgbImg[i][3 * j + 2] * k / aveB; //�������R����

      if (r > 255) {  //����255����255��
        r = 255;
      }

      if (g > 255) {
        g = 255;
      }

      if (b > 255) {
        b = 255;
      }

      chLine[3 * j] = r;
      chLine[3 * j + 1] = g;
      chLine[3 * j + 2] = b;
    }
  }

  return 0;
}
