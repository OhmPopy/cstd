
#ifndef _GMM3DTRACK_H_
#define _GMM3DTRACK_H_
#ifdef __cplusplus
extern "C" {;
#endif
enum {
  GT_START,  // ��ʼ��Ƶ����͸�˹������� void*
    GT_STOP,  // ������Ƶ����͸�˹������� void*
    GT_GETHEIGHT,  // ��ȡ��Ƶ�� int*
    GT_GETWIDTH,   // ��ȡ��Ƶ�� int*
    GT_GETPICH,    // ��ȡ��Ƶÿ���ֽ��� int*
    GT_GETCHANNEL, // ��ȡ��Ƶͨ���� int*
    GT_GETTRANSN,  // ��ȡ��Ƶ���� [0~4] ��ʾ��Ƶ��תn*90�� int*
    GT_GETDATA,    // ��ȡ��Ƶ�ֽ��� unsigned char*
    GT_GETNEXT,    // ��ȡ��һ֡����� unsigned char*
    GT_GETBK,    // ��ȡ��Ƶ�����ֽ��� unsigned char*
    GT_GETFG,    // ��ȡ��Ƶǰ���ֽ��� unsigned char*
    GT_GETSHOWWINDOW,    // ��ȡ�Ƿ���ʾ���� int*
    GT_GETMINPIX,    // ��ȡ�������С���ظ�����С�ڴ˸��������򽫱����� int*
    GT_GEWAITMS,    // ������Ƶÿ֡ʱ�������� int*
    GT_GETRECT,    // ��ȡ Ŀ���������� int*  ���ϵ�x,���ϵ�y,���µ�x,���µ�y,Ŀ��������ظ���
    GT_GETRECTN,    // ��ȡ Ŀ��������� int*
    GT_GETFGPIX,    // ��ȡ ǰ�����ظ��� int*
};

void* GMM3DTrack(int mode);
int GMM3DTrackInt(int mode);
int saveToImg(const char* fn, int h, int w, const unsigned char* img, int al, int ai);
int GMM3DTrackStart();
int GMM3DTrackGetNext();
int GMM3DTrackGetfgpix();
int GMM3DTrackStop();


#ifdef TEST_GMM3DTRACK
#include <stdio.h>
#include <stdlib.h>
// ���Ӵ���
int test_GMM3DTrack() {
  char buf[256];
  int i, j, h, w, al, ai, n, fgpix;
  unsigned char* img;
  int* rt;

  GMM3DTrack(GT_START);
  *(int*)GMM3DTrack(GT_GETSHOWWINDOW)=1; // ��ʾ����
  *(int*)GMM3DTrack(GT_GETMINPIX)=20; // ������С���ظ���Ϊ20
  *(int*)GMM3DTrack(GT_GEWAITMS)=10; // 
  
  for (i=0;;++i) {
    if (0==GMM3DTrack(GT_GETNEXT)) {
      break;
    }
    fgpix = GMM3DTrackInt(GT_GETFGPIX);
    printf("%3d : ǰ�����ظ���\n", fgpix);
    n = GMM3DTrackInt(GT_GETRECTN);
    rt = (int*)GMM3DTrack(GT_GETRECT);
    for (j=1; j<(n>2?2:n); ++j) {
      printf("%3d: (%3d, %3d)-(%3d, %3d) pix:%3d\n", j, rt[j*5], rt[j*5+1], rt[j*5+2], rt[j*5+3], rt[j*5+4]);
    }
    h = GMM3DTrackInt(GT_GETHEIGHT);
    w = GMM3DTrackInt(GT_GETWIDTH);
    al = GMM3DTrackInt(GT_GETPICH);
    ai = GMM3DTrackInt(GT_GETCHANNEL);
    // ������ƵԭͼƬ
    img = (unsigned char*)GMM3DTrack(GT_GETDATA);
    _snprintf(buf, 256, "im%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
    // ����ǰ��ͼƬ
    img = (unsigned char*)GMM3DTrack(GT_GETFG);
    _snprintf(buf, 256, "fg%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
    // ���汳��ͼƬ
    img = (unsigned char*)GMM3DTrack(GT_GETBK);
    _snprintf(buf, 256, "bk%d.jpg", i);
    saveToImg(buf, h, w, img, al, ai);
  }
  GMM3DTrack(GT_STOP);
  return 0;
}

int test_GMM3DTrack2() {
  int i, fgpix;
  
  if (0== GMM3DTrackStart()) {
    printf("����ʧ��\n"); return 0;
  }
  for (i=0;;++i) {
    if (0==GMM3DTrackGetNext()) {
      printf("��ȡͼ��ʧ��\n");
      break;
    }
    fgpix = GMM3DTrackGetfgpix();
    printf("%3d : ǰ�����ظ���\n", fgpix);
  }
  GMM3DTrackStop();
  return 0;
}

#endif

#ifdef __cplusplus
}
#endif
#endif // _GMM3DTRACK_H_
