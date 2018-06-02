


int test_hist1()  
{  
    IplImage* src1=cvLoadImage("aaa.bmp",CC_LOAD_IMAGE_GRAYSCALE);
    //"C:\\Users\\hellmonky\\Desktop\\LeastSquaresMethod\\Debug\\1.bmp"  
    int width=src->width;
    int height=src->height;
    int sum = width*height;
  
    int step=src->widthStep;
    uchar* data=(uchar *)src->imageData;
    int hist[256]={0};
    int CalHist[256] = {0};
    int CH[256] = {0};
    int max1 = 0;
    int max2=0;
  
    int i, j;
    
    //��������ͼ��ĻҶȷֲ�
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)  
        {  
            hist[data[i*step+j]]++;
        }  
    }  
    for(i=0;i<256;i++)  
    {  
        if(hist[i]>max1)  
        {  
            max1=hist[i];
        }  
    }  
    for (i=0;i<256;i++)  
    {
        for (j=0;j<=i;j++)  
        {
            CalHist[i] += (int)(255* (float)hist[j]/sum );
        }
    }
    IplImage* dst1=cvCreateImage(cvSize(400,300),8,1);
    cvSet(dst1,cvScalarAll(255),0);
    double bin_width=(double)dst1->width/256;//������������  
    double bin_unith=(double)dst1->height/max1;
    for(i=0;i<256;i++)
    {  
        IPOINT p0=iPOINT(i*bin_width,dst1->height);
        IPOINT p1=iPOINT((i+1)*bin_width,dst1->height-hist[i]*bin_unith);
        cvRectangle(dst1,p0,p1,cvScalar(1),-1,8,0);
    }  
  
    
    //��ԭʼͼ��������¼���
    for(i=0;i<height;i++)
    {  
        for(j=0;j<width;j++)  
        {  
            data[i*step+j] = CalHist[data[i*step+j]];
        }  
    }  
  
    
    //����任�Ժ��ͼ��ĻҶȷֲ�  
    
    for(i=0;i<height;i++)  
    {  
        for(j=0;j<width;j++)  
        {  
            CH[data[i*step+j]]++;
        }  
    }  
    for(i=0;i<256;i++)  
    {    
        if(CH[i]>max2)  
        {    
            max2=CH[i];
        }    
    }  
    IplImage* you=cvCreateImage(cvSize(400,300),8,1);
    cvSet(you,cvScalarAll(255),0);
    double binwidth=(double)you->width/256;//������������  
    double binunith=(double)you->height/max2;
    for(i=0;i<256;i++)
    {
        IPOINT p0=iPOINT(i*binwidth,you->height);
        IPOINT p1=iPOINT((i+1)*binwidth,you->height-CH[i]*binunith);
        cvRectangle(you,p0,p1,cvScalar(1),-1,8,0);
    }  
  

    cvNamedWindow("ԭʼͼ��Ҷȷֲ�", 0);
    cvShowImage("ԭʼͼ��Ҷȷֲ�",dst1);
    cvSaveImage("ԭʼͼ��Ҷȷֲ�.bmp",dst1);
  
    cvNamedWindow("ֱ��ͼ���⻯ͼ��", 0);
    cvShowImage("ֱ��ͼ���⻯ͼ��",src);
    cvSaveImage("ֱ��ͼ���⻯ͼ��.bmp",src);
  
    cvNamedWindow("���⻯��ֱ��ͼ", 0);
    cvShowImage("���⻯��ֱ��ͼ",you);
    cvSaveImage("���⻯��ֱ��ͼ.bmp",you);
  
    cvWaitKey(0);
    cvDestroyWindow("ԭʼͼ��Ҷȷֲ�");
    cvDestroyWindow("ֱ��ͼ���⻯ͼ��");
    cvDestroyWindow("���⻯��ֱ��ͼ");
    return 0;
}

