// �߻��־���㷨(Line Integral Convolution��LIC��)
// ��һ�־����ʸ�������ӻ���������Cabral��Leedom ��1993���SIGGRAPH����������
// ���㷨������Ч�ر�����άʸ�������������ֱ�۵ط�ӳ��ÿ����ٶȷ������ܱ��ʸ������ϸ�ڡ�
// ���㷨���������ʸ�������ӻ����й㷺����Զ�����壬�ѱ��ɹ�Ӧ�õ��ܶ�����
// �䷢չ�����Ѿ��漰������ѡȡ���ź��˲���������㡢����ӳ�估����������ȶ��ּ��������й����ķ�չǰ����
//  �þ������ʾʸ�����ķ���Դ��һ���˶�ģ����˼�롣LIC�㷨ԭ��ɸ���Ϊ����һά��ͨ�����
// ˫��ԳƵ������߷������������������պϳ�ʸ��������ͼΪLIC�㷨����ʾ��ͼ��
// http://www.zhanpingliu.org/Research/FlowVis/LIC/LIC.htm

#define DISCRETE_FILTER_SIZE 2048      //����˵Ķ�Ӧ����̫С�Ļ���Щ�������û�б������ȥ��Ӱ��ͼ��Ч��
#define LOWPASS_FILTR_LENGTH 32.00000f
#define LINE_SQUARE_CLIP_MAX 100000.0f //ֻҪ���ڵ���0���У�����0ʱ����ֺ�ɫ����
#define VECTOR_COMPONENT_MIN 0.050000f

/*----------------------------------------------------------------------------------------------*/


/*�ϳ�����----------------------------------------------------*/
void SyntheszSaddle(int _row, int _col, float* pVectr)
{
	int row = 400, col = 400, index = 0;
	float vec_x = 0.0f, vec_y = 0.0f, vcMag = 0.0f, scale = 0.0f;
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			// ��������
			index = i * col + j;// ����ʸ����
			// index = (row - 1 - i) * col + j;// ��ʸ����
			vec_x = -(float)i / row + 0.5f;
			vec_y =  (float)j / col - 0.5f;

			// ���ݹ���
			vcMag = float(sqrt(vec_x * vec_x + vec_y * vec_y));
			scale = (vcMag < 0.001f) ? 0.0f : 1.0f / vcMag;
			vec_x *= scale;
			vec_y *= scale;

			pVectr[2*index]     = vec_x;
			pVectr[2*index + 1] = vec_y;
		}
	}
}
/*------------------------------------------------------------*/


/*���ݹ���------------------------------------------------------------------------------------------------------------*/
void NormalizVectrs(int w, int h, float* pVectr)
{
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			int   index = (j * w + i) << 1;
			float vcMag = float( sqrt( double(pVectr[index] * pVectr[index] + pVectr[index + 1] * pVectr[index + 1]) ) );
			float scale = (vcMag == 0.0f) ? 0.0f : 1.0f / vcMag;
			pVectr[index    ] *= scale;//ʸ�������ķŴ�����Խ���ٶ�Խ�󣬳��������ٶȻ��������Ч���������
			pVectr[index + 1] *= scale;
		}//for
	}//for
}
/*--------------------------------------------------------------------------------------------------------------------*/


/*���ɰ�����----------------------------------------------------*/
void MakeWhiteNoise(int w, int h, unsigned char* pNoise)
{
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			int r = rand();
			r = ( (r & 0xff) + ( (r & 0xff00) >> 8 ) ) & 0xff;//
			pNoise[j * w + i] = (unsigned char) r;
		}
	}
}
/*--------------------------------------------------------------*/


/*�����LUTsiz = DISCRETE_FILTER_SIZE ----------------------*/
void GenBoxFiltrLUT(int LUTsiz, float* p_LUT0, float* p_LUT1)
{
	for(int i = 0; i < LUTsiz; i++)
	{
		p_LUT0[i] = p_LUT1[i] = (float)i;
	}
}
/*----------------------------------------------------------*/


/*д�ļ�----------------------------------------------------------------------*/
void WriteImage2PPM(int w, int h, unsigned char* pImage, char* f_name)
{
	FILE* o_file;
	if( (o_file = fopen(f_name, "w") ) == NULL )
	{
		printf("Can't open output file\n");
		return;
	}

	fprintf(o_file, "P6\n%d %d\n255\n", w, h);

	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i< w; i++)
		{
			unsigned char unchar = pImage[j * w + i];
			fprintf(o_file, "%c%c%c", unchar, unchar, unchar);
		}
	}

	fclose(o_file);
	o_file = NULL;
}
/*-----------------------------------------------------------------------------*/


/*�������ص�------------------------------------------------------------------------------------------------*/
void FlowImagingLIC(int    w, int h,    float* pVectr, unsigned char* pNoise, unsigned char* pImage,
					float* p_LUT0, float* p_LUT1, float krnlen)
{
    int     vec_id = 0;
	int     advDir = 0;  //����
	int     advcts = 0;	 //׷�ٵĲ���
	int     ADVCTS = int(krnlen * 3);//׷�ٵ������

	float   vctr_x = 0;  //�ٶ�x����
	float   vctr_y = 0;  //�ٶ�y����
	float   clp0_x = 0;  //��ǰ��x����
	float   clp0_y = 0;  //��ǰ��y����
	float   clp1_x = 0;  //��һ��x����
	float   clp1_y = 0;  //��һ��y����
	float   samp_x = 0;  //������x����
	float   samp_y = 0;  //������y����
	float   tmpLen = 0;  //��ʱ����
	float   segLen = 0;  //ÿ�γ���
	float   curLen = 0;  //��ǰ�����߳���
	float   prvLen = 0;  //��һ�����߳���
	float   W_ACUM = 0;  //����Ȩ��֮��
	float   texVal = 0;  //����ĻҶ�ֵ
	float   smpWgt = 0;  //��ǰ�������Ȩ��ֵ
	float   t_acum[2];   //���������Ӧ�����ϵĻҶ�ֵ֮��
	float   w_acum[2];   //Ȩ��֮��
	float*  wgtLUT = NULL;//Ȩ�ز��ұ�
	float   len2ID = (DISCRETE_FILTER_SIZE - 1) / krnlen;//�����߳���ӳ�䵽����˵�ĳһ��
    //krnlen����LOWPASS_FILTR_LENGTH
	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			t_acum[0] = t_acum[1] = w_acum[0] = w_acum[1] = 0.0f;
            
			//׷�ٷ������������뷴������
			for(advDir = 0; advDir < 2; advDir++)
			{
				//**��ʼ��׷�ٲ�����׷�ٳ��ȡ����ӵ�λ��
				advcts = 0;
				curLen = 0.0f;
				clp0_x = i + 0.5f;
				clp0_y = j + 0.5f;

				//**��ȡ��Ӧ�ľ����
				wgtLUT = (advDir == 0) ? p_LUT0 : p_LUT1;
                
				//**ѭ����ֹ����������׷�ٵ��㹻�����ߵ���������������
				while(curLen < krnlen && advcts < ADVCTS)
				{
					//**��ò������ʸ������
					vec_id = ( int(clp0_y) * w + int(clp0_x) ) << 1;
					vctr_x = pVectr[vec_id    ];
					vctr_y = pVectr[vec_id + 1];
                    
					//**��Ϊ�ؼ��㼴һ�������Ϊ����������ʱ����������׷��
					if( vctr_x == 0 && vctr_y == 0)
					{
						t_acum[advDir] = (advcts == 0) ? 0.0f : t_acum[advDir];
					    w_acum[advDir] = (advcts == 0) ? 1.0f : w_acum[advDir];
						break;
					}
					
					//**����׷�ٻ���׷��
					vctr_x = (advDir == 0) ? vctr_x : -vctr_x;
					vctr_y = (advDir == 0) ? vctr_y : -vctr_y;

					segLen = LINE_SQUARE_CLIP_MAX;
					segLen = (vctr_x < -VECTOR_COMPONENT_MIN) ? ( int(     clp0_x         ) - clp0_x ) / vctr_x : segLen;
					segLen = (vctr_x >  VECTOR_COMPONENT_MIN) ? ( int( int(clp0_x) + 1.5f ) - clp0_x ) / vctr_x : segLen;

					segLen = (vctr_y < -VECTOR_COMPONENT_MIN) ?
												  ( ( (tmpLen = ( int(     clp0_y         ) - clp0_y ) / vctr_y) < segLen) ? tmpLen : segLen) : segLen;
					segLen = (vctr_y >  VECTOR_COMPONENT_MIN) ?
						                          ( ( (tmpLen = ( int( int(clp0_y) + 1.5f ) - clp0_y ) / vctr_y) < segLen) ? tmpLen : segLen) : segLen;

					prvLen = curLen;
					curLen += segLen;
					segLen += 0.0004f;//��β����ӵĻ������ǻ��������
					
					//**�жϳ���
					segLen = (curLen > krnlen) ? ( (curLen = krnlen) - prvLen ) : segLen;
                    
					//**��ȡ��һ��׷�ٵ�λ��
					clp1_x = clp0_x + vctr_x * segLen;
					clp1_y = clp0_y + vctr_y * segLen;
					
					//**���������λ��
					samp_x = (clp0_x + clp1_x) * 0.5f;
					samp_y = (clp0_y + clp1_y) * 0.5f;

					///��ȡ���������ĻҶ�ֵ->����������ò�ֵ�Ļ�����Ч��Ӧ��������һ������
					texVal = pNoise[ int(samp_y) * w + int(samp_x) ];

					W_ACUM = wgtLUT[ int(curLen * len2ID) ];
					smpWgt = W_ACUM - w_acum[advDir];
					w_acum[advDir] = W_ACUM;
					t_acum[advDir] += texVal * smpWgt;

					advcts ++;
					clp0_x = clp1_x;
					clp0_y = clp1_y;
                    
					if( clp0_x < 0.0f || clp0_x >= w || clp0_y < 0.0f || clp0_y >= h)
					{
						break;
					}
				}//while
			}//for

			texVal = (t_acum[0] + t_acum[1]) / (w_acum[0] + w_acum[1]);

			texVal = (texVal <   0.0f) ?   0.0f : texVal;
			texVal = (texVal > 255.0f) ? 255.0f : texVal;

			pImage[j * w + i] = (unsigned char)texVal;
		}//for
	}//for
}
/*----------------------------------------------------------------------------------------------------------*/
void test_lic1()
{
  int SQUARE_FLOW_FIELD_SZ = 400;
  int            w = SQUARE_FLOW_FIELD_SZ;
  int            h = SQUARE_FLOW_FIELD_SZ;
  float*         pVectr = (float*        )malloc( sizeof(float) * w * h * 2 );
  float*         p_LUT0 = (float*        )malloc( sizeof(float) * DISCRETE_FILTER_SIZE);
  float*         p_LUT1 = (float*        )malloc( sizeof(float) * DISCRETE_FILTER_SIZE);
  unsigned char* pNoise = (unsigned char*)malloc( w * h     );
  unsigned char* pImage = (unsigned char*)malloc( w * h     );
  
  //SyntheszSaddle(h, w, pVectr);
  // NormalizVectrs(w, h, pVectr);//���ݲ�����һ�µĻ����ٶ�ԽС�ĵط�����Խǿ
  //MakeWhiteNoise(h, w, pNoise, w);
  //GenBoxFiltrLUT(DISCRETE_FILTER_SIZE, p_LUT0, p_LUT1);
  FlowImagingLIC(h, w, pVectr, pNoise, pImage, p_LUT0, p_LUT1, LOWPASS_FILTR_LENGTH);
  //WriteImage2PPM(w, h, pImage, "LIC.ppm");
  cvShowImage("pNoise", h, w, pNoise, w, 1);
  cvShowImage("pImage", h, w, pImage, w, 1);
  cvWaitKey(-1);
  

  free(pVectr);
  free(p_LUT0);
  free(p_LUT1);
  free(pNoise);
  free(pImage);
}


/*----------------------------------------------------------------------------------------------*/
void test_lic()
{
  int SQUARE_FLOW_FIELD_SZ = 400;
  int            w = SQUARE_FLOW_FIELD_SZ;
  int            h = SQUARE_FLOW_FIELD_SZ;
  float*         pVectr = (float*        )malloc( sizeof(float        ) * w * h * 2 );
  float*         p_LUT0 = (float*        )malloc( sizeof(float        ) * DISCRETE_FILTER_SIZE);
  float*         p_LUT1 = (float*        )malloc( sizeof(float        ) * DISCRETE_FILTER_SIZE);
  unsigned char* pNoise = (unsigned char*)malloc( sizeof(unsigned char) * w * h     );
  unsigned char* pImage = (unsigned char*)malloc( sizeof(unsigned char) * w * h     );
  
  SyntheszSaddle(w, h, pVectr);
  // NormalizVectrs(w, h, pVectr);//���ݲ�����һ�µĻ����ٶ�ԽС�ĵط�����Խǿ
  MakeWhiteNoise(w, h, pNoise);
  GenBoxFiltrLUT(DISCRETE_FILTER_SIZE, p_LUT0, p_LUT1);
  FlowImagingLIC(w, h, pVectr, pNoise, pImage, p_LUT0, p_LUT1, LOWPASS_FILTR_LENGTH);
  //WriteImage2PPM(w, h, pImage, "LIC.ppm");
  
  cvShowImage("pNoise", h, w, pNoise, w, 1);
  cvShowImage("pImage", h, w, pImage, w, 1);
  cvWaitKey(-1);

  free(pVectr);  pVectr = NULL;
  free(p_LUT0);  p_LUT0 = NULL;
  free(p_LUT1);  p_LUT1 = NULL;
  free(pNoise);  pNoise = NULL;
  free(pImage);  pImage = NULL;
}