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
int wbGW(int rows, int cols, uchar* rgbImg, int rgb_step, uchar* chImg, int ch_step)
{
	int i = 0;
	int j = 0;
	
	//1����R��G��B��ɫ������ƽ��ֵ
	float aveR = 0.0f;				//��¼R������ƽ��ֵ
	float aveG = 0.0f;				//��¼G������ƽ��ֵ
	float aveB = 0.0f;				//��¼B������ƽ��ֵ
	float k = 0.0f;					//��¼aveR��aveG��aveB��ƽ��ֵ
	for(i = 0; i < rows; i++)
	{
    uchar* rgbLine = rgbImg + i*rgb_step;
		for (j = 0; j < cols; j++)
		{
			aveR = aveR + rgbLine[3*j+0];
			aveG = aveG + rgbLine[3*j+1];
			aveB = aveB + rgbLine[3*j+2];
		}
	}
	aveR = aveR / (rows * cols);
	aveG = aveG / (rows * cols);
	aveB = aveB / (rows * cols);
	k = (aveR + aveG + aveB) / 3;

	//3�����ƽ��������ͼ������
	for (i = 0; i < rows; i++)
	{
    uchar* rgbLine = rgbImg + i*rgb_step;
    uchar* chLine = chImg + i*ch_step;
		for (j = 0; j < cols; j++)
		{
      int r, g, b;
			r = (int)(rgbLine[3*j]   * k / aveR);			//�������R����
			g = (int)(rgbLine[3*j+1] * k / aveG);		//�������R����
			b = (int)(rgbLine[3*j+2] * k / aveB);		//�������R����
        //����255����255��
      if (r > 255) { r = 255;	}
      if (g > 255) { g = 255; }
      if (b > 255) { b = 255; }
      chLine[3*j+0]=r;
      chLine[3*j+1]=g;
      chLine[3*j+2]=b;
		}
	}
  return 0;
}

