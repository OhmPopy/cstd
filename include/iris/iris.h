
#ifndef _IRIS_H_
#define _IRIS_H_

// ��Ĥ��һ����С
#define IRIS_H  30
#define IRIS_W  360
#define IRIS_S  (IRIS_W/8)
#define IRIS_BLOCK_H  16
#define IRIS_BLOCK_W  (64/8)

#define IRIS_BLOCK_TY  2
#define IRIS_BLOCK_TX  1

#define IRIS_BLOCK_NUM  ( ((IRIS_H-IRIS_BLOCK_H)/IRIS_BLOCK_TY + 1) * ((IRIS_S-IRIS_BLOCK_W)/IRIS_BLOCK_TX + 1) )

//����ͼ��С
#define IRIS_OMLEN   (IRIS_H * IRIS_S)
#define IRIS_SHIFT1    8
#define IRIS_SHIFT2    16

#define IR_DEBUG         (1<<0)  // ����ģʽʱʹ��
#define IR_BOOST_DETECT  (1<<1)  // ʹ�� boost����
#define IR_N_DETECT      (1<<2)  // ��Բ���
#define IR_U_DETECT      (1<<3)  // ���������
#define IR_W_DETECT      (1<<4)  // ��Բ���
#define IR_D_DETECT      (1<<5)  // ���������
#define IR_STDIMG        (1<<6)  // ��һ��
#define IR_FEATURE       (1<<7)  // ��ȡ����
#define IR_RECOG         (1<<8)  // �ȶ�
#define IR_FLIP          (1<<9)  // �ȶ�

// ����
#define IR_BG_INIT       (1<<10) // ���³�ʼ��
#define IR_BG_UPDATE     (1<<11) // ���±���

//#define IR_DRAW_DETECT      (1<<10)  // ���ƽ��

#define IR_NU_DETECT     (IR_N_DETECT|IR_U_DETECT)
#define IR_NUW_DETECT    (IR_NU_DETECT|IR_W_DETECT)
#define IR_NUWD_DETECT   (IR_NUW_DETECT|IR_D_DETECT)
#define IR_ALL_DETECT    (IR_BOOST_DETECT|IR_NUWD_DETECT)

#define FR_IRIS_DET  IR_ALL_DETECT
#define FR_IRIS_REG  IR_ALL_DETECT|IR_STDIMG|IR_FEATURE
#define FR_IRIS_REC  IR_ALL_DETECT|IR_STDIMG|IR_FEATURE|IR_RECOG


typedef struct irisrecog_t {
  int in[3]; // ��Բ x=n[0] y=n[1] r=n[2]
  int iw[3]; // ��Բ
  int iu[3]; // ������������ y[0] * (x * x) + y[1] * x + y[2]
  int id[3]; // ������������

  int height, width; // �Ҷ�ͼƬ�߿�
  int color_type; // ��ɫ���� T_GRAY T_UYVY
  uchar* data; // ͼ��������buf��ƫ�� ��С=height * width
  //int std_off; // ��һ��ͼ��������buf��ƫ�� ��С= IRIS_H * IRIS_W
  uchar* buf;  // ������ƫ���� ��С= buflen
  uchar* feature; // ����������ƫ���� ��С=featlen
  uchar* featlib;  // ������
  int mode; // ģʽѡ��
  // ��Ĥ�����
  XRECT rc; // boost������ cx==0 ��ʾû��⵽
  int blurrate; // ģ��ϵ������ʾ�۾�����ģ���̶ȣ�ԽСԽģ������ʾ����ʱҪ���� 256.00����һ��С��26�ͱȽ�ģ���ˡ�
  int cnt_zero; // stdimg ��0�����
  int featlen;  // ��������,mode==0ʱҲ�ܻ�ȡfeatlen��һ�㲻�ᳬ�� 10000 ,�����õ�����==5376
  int distthd;  // �Ƽ���ֵ,���ڼ���Сֵͳ�ƶ���
  int distmax;  // ������,���ڼ����ֵͳ�ƶ���
  int buflen;   // �������ܳ���
  int featnum;  // ��������
  int featstep; // ��������������>=�������� featlen(==5376)
  int maxdist;  // ��С����ֵ
  int maxdist_id; // ��С�����Ӧ��id
  int dbgdata[32]; // �������ģʽ�µ�����
  //int bufused; //buf�Ѿ�ʹ�õĳ��� buflen-bufused==�㷨���õĿռ�
  unsigned char* stdiris;//[IRIS_H*IRIS_W]; // ��һ��ͼƬ
} irisrecog_t;

void* dsp_malloc( size_t n );
void dsp_free( void* p );
// ��dsp����
void iris_recog_open();
void iris_recog_close();
int iris_recog_process( void* p0);
// wline�߿�
int iris_draw(int h, int w, unsigned char* img, int step, int cn, const irisrecog_t* ir, unsigned long color0, char* s, int isflip, int wline);

#endif // _IRIS_H_
