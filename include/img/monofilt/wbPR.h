
/******************************************
void fQuickSort(float* data, int* ind, int n)
���ܣ�
	���ÿ������򷨶����齵������
������
	float* data - ��������		
	int* ind - λ����������˳��
	int iLeft - ��ʼλ��
	int iRight - ����λ��
*******************************************/
void fQuickSort(float* data, int* ind, int iLeft, int iRight)
{
	if (iLeft >= iRight)
		return;
	
	int mid = iLeft;

	//�����һ��Ԫ��Ϊ���գ���ķ���ߣ�С�ķ��ұ�
	for (int i = iLeft; i < iRight; i++)
	{
		if (data[i] >= data[iRight])
		{			
			float temp0 = data[mid];
			data[mid] = data[i];
			data[i] = temp0;

			int temp1 = ind[mid];
			ind[mid] = ind[i];
			ind[i] = temp1;

			mid = mid + 1;
		} 
		
	}
	
	float temp2 = data[mid];
	data[mid] = data[iRight];
	data[iRight] = temp2;

	int temp3 = ind[mid];
	ind[mid] = ind[iRight];
	ind[iRight] = temp3;

	fQuickSort(data, ind, iLeft, mid-1);
	fQuickSort(data, ind, mid+1, iRight);
}

/******************************************
void wbPR(float** rgbImg, float** chImg, int rows, int cols)
���ܣ�
	��rbgͼ��ȫ���������㷨(PR)����ƽ�����
������
	float** rgbImg - ͼ���rgb���ݣ�[rows][cols*3]		
	float** chImg - ������ݣ���ƽ�⴦����ͼ�����ݣ�[rows][cols*3]
	int rows - ͼ�������
	int cols - ͼ�������	
*******************************************/
int wbPR(int rows, int cols, uchar* rgbImg, int rgb_step, uchar* chImg, int ch_step)
{
	int i, j;

	//1���׵�����ֵ
	int rw = 255;
	int gw = 255;
	int bw = 255;	

	//2�����������R��G��Bֵ��Ϊ�����ձ��ԣ����������һ�������������ƽ��ֵ
	float rate = 0.15f;									//���ʣ���ȫͼ��ȡ������ı���
	float rmax = 0.0f;									//������rate�����������R������ƽ��ֵ
	float gmax = 0.0f;									//������rate�����������G������ƽ��ֵ
	float bmax = 0.0f;									//������rate�����������B������ƽ��ֵ
	float* grayImg = new float[rows * cols];					//��¼rgbImg�ĻҶ�ֵ
	float* tempImg = new float[rows * cols];			//��grayImg����ת��Ϊһά���飬��������
  int gray_step = cols;
  int temp_step = cols;
	int* ind = new int[rows * cols];					//��¼˳��
	for (i = 0; i < rows*cols; i++)						//����ֵ
	{
		ind[i] = i;
	}
	
	//rgb2gray(rgbImg, grayImg, rows, cols);				//rgbתgray

	for (i = 0; i < rows; i++)
	{
    int r, g, b;
    uchar* rgbLine = rgbImg + i*rgb_step;
    float* grayLine = grayImg + i*gray_step;
		for (j = 0; j < cols; j++)
		{
      r = rgbLine[j*3+0];
      g = rgbLine[j*3+1];
      b = rgbLine[j*3+2];
			grayLine[j] = (float)((r+g+g+b)>>2);
		}
	}

	for (i = 0; i < rows; i++)
	{
    float* grayLine = grayImg + i*gray_step;
    float* tempLine = tempImg + i*temp_step;
		for (j = 0; j < cols; j++)
		{
			tempLine[j] = grayLine[j];
		}
	}


	// fSort(tempImg, ind, rows*cols);		//��������
	fQuickSort(tempImg, ind, 0, rows*cols-1);

	//��rmax��gmax��bmax
	for (i = 0; i < int(rate*rows*cols); i++)
	{
		int row, col;									//�С��ж�λ
		row = int(ind[i] / cols);						//��
		col = ind[i] - cols * row;						//��
		rmax = rmax + rgbImg[row*rgb_step+3*col];
		gmax = gmax + rgbImg[row*rgb_step+3*col+1];
		bmax = bmax + rgbImg[row*rgb_step+3*col+2];
	}
	rmax = rmax / (int(rate*rows*cols));
	gmax = gmax / (int(rate*rows*cols));
	bmax = bmax / (int(rate*rows*cols));


	//3������ԭͼ��R��G��B����ֵ���ó��������ͼ������
	for (i = 0; i < rows; i++)
	{
    uchar* rgbLine = rgbImg + i*rgb_step;
    uchar* chLine = chImg + i*ch_step;
		for (j = 0; j < cols; j++)
		{
			chLine[3*j+0] = (uchar)(rgbLine[3*j+0] * rw / rmax);
			chLine[3*j+1] = (uchar)(rgbLine[3*j+1] * gw / gmax);
			chLine[3*j+2] = (uchar)(rgbLine[3*j+2] * bw / bmax);
		}
	}

	//4���ͷ��ڴ�
	delete[] grayImg;
	grayImg = NULL;

	delete[] tempImg;
	tempImg = NULL;

	delete[] ind;
	ind = NULL;
  return 0;
}

