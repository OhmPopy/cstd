
#ifndef _FACETRACKING_H_
#define _FACETRACKING_H_

#ifdef __cplusplus
extern "C" {
#endif

  struct XRECT;
  
  typedef struct TRACKER {
    size_t count;   // ���ο���Ŀ
    size_t maxcount; // �����ο�����
    size_t baseid; // ��С���ο�id��
    real zoom; // ����ϵ��
    real mindist; // �ϲ���С������
    real minarea; // ��С���
    real ssmin, ss, thd;
    int stepxy, mincnt;   // ������
    real** phist; // ֱ��ͼָ��
    PXRECT ptrack; // �����ٵľ��ο���
  }
  TRACKER, *PTRACKER;
  
  // ����������
  // maxcount - �����ο��������������ɸ��ٵ�Ŀ������
  PTRACKER new_tracker( size_t maxcount );
  
  // �ͷŸ�����
  void free_tracker( PTRACKER tk );
  
  //
  // [h, w, A, al, ai] - BGR��ɫͼƬ[��, ��, ���Ͻ�ָ��, �в���, �в���]
  // tk  - new_tracker �����ĸ�����ָ��
  // cas - ����������
  // mark, mark_step -����ָ��,����step,
  // detectface - �Ƿ���ȫ�ּ�� 1=��� 0=�����
  int camtrack( int h, int w, unsigned char * img, int al, int ai,
                unsigned char * mark, int mark_step, PTRACKER tk,
                const void * cas, int detectface );
                
#ifdef __cplusplus
}
#endif

#endif // _FACETRACKING_H_
