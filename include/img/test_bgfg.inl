#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>


int main(int argc, char** argv)
{
  //����IplImageָ��
  IplImage* pFrame = NULL;
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;

  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;

  CvCapture* pCapture = NULL;

  CvBGStatModel* pBGStatModel = NULL;

  int nFrmNum = 0;

  //��������
  cvNamedWindow("video", 1);
  cvNamedWindow("background", 1);
  cvNamedWindow("foreground", 1);
  //ʹ������������
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);

  //����Ƶ�ļ�
  if (!(pCapture = cvCaptureFromCAM(0))) {
    fprintf(stderr, "Can not open video file %s\n", argv[ 1 ]);
    return -2;
  }

  //��֡��ȡ��Ƶ
  while (pFrame = cvQueryFrame(pCapture)) {
    nFrmNum++;

    //����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
    if (nFrmNum == 1) {
      pBkImg = cvCreateImage(cvSize(pFrame->w, pFrame->h),
          IMG_DEPTH_8U, 1);
      pFrImg = cvCreateImage(cvSize(pFrame->w, pFrame->h),
          IMG_DEPTH_8U, 1);
      pBkMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);
      pFrMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);
      pFrameMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);

      //pBGStatModel = cvCreateFGDStatModel(pFrame, 0);
      pBGStatModel = cvCreateGaussianBGModel(pFrame, 0);
    }
    else {
      //CC_ASSERT(pBGStatModel!=NULL);

      {
        utime_start(_start_time);
        cvUpdateBGStatModel(pFrame, pBGStatModel);
        printf("%f\n", utime_elapsed(_start_time));
      }
      //��ʾͼ��
      cvShowImage("video", pFrame);
      cvShowImage("background", pBGStatModel->background);
      cvShowImage("foreground", pBGStatModel->foreground);

      //����а����¼���������ѭ��
      //�˵ȴ�ҲΪcvShowImage�����ṩʱ�������ʾ
      //�ȴ�ʱ����Ը���CPU�ٶȵ���
      if (cvWaitKey(2) >= 0) {
        break;
      }
    }  // end of if-else
  } // end of while-loop

  cvReleaseBGStatModel(&pBGStatModel);
  //���ٴ���
  cvDestroyWindow("video");
  cvDestroyWindow("background");
  cvDestroyWindow("foreground");

  //�ͷ�ͼ��;���
  cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);

  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);

  return 0;
}
