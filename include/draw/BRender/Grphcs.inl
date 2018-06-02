// ����ͼ�����
#include <math.h>

// ��������
// ��ѧ����
#define PIE   3.14159265358979323846
// ͼ����
// �汾��
#define IMAGE_PROCESS_VERSION_1_0     1
//��ҵ��ɫģ��
#define IMAGE_COLOR_RGB           0X00000000
#define IMAGE_COLOR_RGBA          0X00000001
#define IMAGE_COLOR_INDEX         0X00000002
//ͼ�����ȴ���
#define IMAGE_BRIGHTNESS_PERCENTAGE     0X00000100
#define IMAGE_BRIGHTNESS_REVERSE        0X00000101
//ͼ��Աȶȵ���
#define IMAGE_CONTRAST_ADJUSTMENT       0X00000200
//ͼ��Ҷȴ���
#define IMAGE_GRAY_COLOR_GRAYED_OUT     0X00000300
#define IMAGE_GRAY_PERCENTAGE           0X00000301
#define IMAGE_GRAY_LINEARIZE            0X00000302
#define IMAGE_GRAY_LINEAR_ROUND_OFF     0X00000303
#define IMAGE_GRAY_REVERSE              0X00000304
#define IMAGE_GRAY_HISTOGRAM_BALANCE    0X00000305
#define IMAGE_GRAY_HISTOGRAM_MATCH      0X00000306
#define IMAGE_GRAY_STATISTIC_MATCH      0X00000307
//ͼ����ɫ����
#define IMAGE_COLOR_RENDER          0X00000400
//ͼ���ع⴦��
#define IMAGE_COLOR_SOLARIZE        0X00000500
//ͼ��������
//ͼ��ģ��,
//to blur or smooth image
// 3-order--------5-order
#define IMAGE_BLUR_MEAN_SMOOTH_3      0X00010000
#define IMAGE_BLUR_MEAN_SMOOTH_5      0X00010001
//��ֵģ��,�����Դ���
#define IMAGE_BLUR_MEDIAN_FILTER      0X00010002
//ǰ���ַ���Ϊ��ͨ����,���һ�ַ���Ϊ��ֵ����
#define IMAGE_LOWPASS_NOISE_REMOVE_3    0X00010100
#define IMAGE_LOWPASS_NOISE_REMOVE_5    0X00010101
#define IMAGE_MEDIAN_NOISE_REMOVE       0X00010102
//ͼ���ͨ�˲�
// to sharpen image
#define IMAGE_HIGHPASS_BASIC_SHARPEN      0X00010200
#define IMAGE_HIGHPASS_MODERATE_SHARPEN   0X00010201
#define IMAGE_HIGHPASS_EXCESSIVE_SHARPEN  0X00010202
#define IMAGE_HIGHPASS_OUTLINE_SHARPEN    0X00010203
//��Ե���
//Laplacian ����
//to enhance discontinuities
#define IMAGE_LAPLACIAN_BASIC_DETECT      0X00010300
#define IMAGE_LAPLACIAN_MODERATE_DETECT   0X00010301
#define IMAGE_LAPLACIAN_EXCESSIVE_DETECT  0X00010302
//�ݶȼ��
#define IMAGE_GRADIENT_EAST_DETECT        0X00010400
#define IMAGE_GRADIENT_SOUTH_DETECT       0X00010401
#define IMAGE_GRADIENT_WEST_DETECT        0X00010402
#define IMAGE_GRADIENT_NORTH_DETECT       0X00010403
#define IMAGE_GRADIENT_NORTHEAST_DETECT   0X00010404
#define IMAGE_GRADIENT_SOUTHWEST_DETECT   0X00010405
#define IMAGE_GRADIENT_NORTHWEST_DETECT   0X00010406
#define IMAGE_GRADIENT_SOUTHEAST_DETECT   0X00010407
//ƽ�ƺͲ�ֱ�Ե��ǿ(���)
#define IMAGE_DIFFERENCE_EAST_DETECT      0X00010500
#define IMAGE_DIFFERENCE_SOUTH_DETECT     0X00010501
#define IMAGE_DIFFERENCE_WEST_DETECT      0X00010502
#define IMAGE_DIFFERENCE_NORTH_DETECT     0X00010503
#define IMAGE_DIFFERENCE_NORTHEAST_DETECT 0X00010504
#define IMAGE_DIFFERENCE_SOUTHWEST_DETECT 0X00010505
#define IMAGE_DIFFERENCE_NORTHWEST_DETECT 0X00010506
#define IMAGE_DIFFERENCE_SOUTHEAST_DETECT 0X00010507
//Prewitt��Ե��ǿ(���)
#define IMAGE_PREWITT_EAST_DETECT         0X00010600
#define IMAGE_PREWITT_SOUTH_DETECT        0X00010601
#define IMAGE_PREWITT_WEST_DETECT         0X00010602
#define IMAGE_PREWITT_NORTH_DETECT        0X00010603
#define IMAGE_PREWITT_NORTHEAST_DETECT    0X00010604
#define IMAGE_PREWITT_SOUTHWEST_DETECT    0X00010605
#define IMAGE_PREWITT_NORTHWEST_DETECT    0X00010606
#define IMAGE_PREWITT_SOUTHEAST_DETECT    0X00010607
//�߼��
#define IMAGE_LINE_HORIZONTAL_DETECT      0X00010700
#define IMAGE_LINE_VERTICAL_DETECT        0X00010701
#define IMAGE_LINE_LEFT_DIAGONAL_DETECT   0X00010702
#define IMAGE_LINE_RIGHT_DIAGONAL_DETECT  0X00010703
//�����Ա�Ե���
#define IMAGE_SOBEL_EDGE_DETECT       0X0001FE00
#define IMAGE_KIRSCH_EDGE_DETECT      0X0001FE01
//.................,and so on.
//�Զ�������
#define IMAGE_GENERAL_CONVOLUTION_FILTER  0X0001FF00
//С����ɢ����
#define IMAGE_DIFFUSE_PLOT_FILTER         0X00020000
//�����ֻ�
#define IMAGE_GRANUALATION_PLOT_FILTER    0X00020100
//ͼ��ϳ�
#define IMAGE_TRANSPARENT_COLOR_COMPOSE   0X00030000
//ͼ���Ĵ�������
#define IMAGE_ALGEBRA_ADD         0X00030100
#define IMAGE_ALGEBRA_SUBTRACT        0X00030101
#define IMAGE_ALGEBRA_MULTIPLY        0X00030102
#define IMAGE_ALGEBRA_DIVIDE        0X00030103
#define IMAGE_ALGEBRA_AVERAGE       0X00030104
//ͼ�����߼�����
#define IMAGE_LOGIC_AND           0X00030200
#define IMAGE_LOGIC_OR            0X00030201
#define IMAGE_LOGIC_XOR           0X00030202
//ͼ���ںϴ���
#define IMAGE_BLEND             0X00030300
//ͼ���ں�Դ����
//��������Alpha�ļ�������
#define IMAGE_BLEND_SFACTOR_255         0X00030400
#define IMAGE_BLEND_SFACTOR_ZERO        0X00030401
#define IMAGE_BLEND_SFACTOR_DST_COLOR     0X00030402
#define IMAGE_BLEND_SFACTOR_255_MINUS_DST_COLOR 0X00030403
#define IMAGE_BLEND_SFACTOR_SRC_ALPHA     0X00030404
#define IMAGE_BLEND_SFACTOR_255_MINUS_SRC_ALPHA 0X00030405
#define IMAGE_BLEND_SFACTOR_DST_ALPHA     0X00030406
#define IMAGE_BLEND_SFACTOR_255_MINUS_DST_ALPHA 0X00030407
#define IMAGE_BLEND_SFACTOR_SRC_ALPHA_SATURATE  0X00030408
//ͼ���ں�Ŀ������
//��������Alpha�ļ�������
#define IMAGE_BLEND_DFACTOR_255         0X00030500
#define IMAGE_BLEND_DFACTOR_ZERO        0X00030501
#define IMAGE_BLEND_DFACTOR_SRC_COLOR     0X00030502
#define IMAGE_BLEND_DFACTOR_255_MINUS_SRC_COLOR 0X00030503
#define IMAGE_BLEND_DFACTOR_SRC_ALPHA     0X00030504
#define IMAGE_BLEND_DFACTOR_255_MINUS_SRC_ALPHA 0X00030505
#define IMAGE_BLEND_DFACTOR_DST_ALPHA     0X00030506
#define IMAGE_BLEND_DFACTOR_255_MINUS_DST_ALPHA 0X00030507
//��̬ѧ����
#define IMAGE_MATH_MORPHOLOGY_DILATE      0X00040000
#define IMAGE_MATH_MORPHOLOGY_ERODE       0X00040001
#define IMAGE_MATH_MORPHOLOGY_OPEN        0X00040002
#define IMAGE_MATH_MORPHOLOGY_CLOSE       0X00040003
#define IMAGE_MATH_MORPHOLOGY_PICK_DETECT   0X00040004
#define IMAGE_MATH_MORPHOLOGY_DENOISE     0X00040005
#define IMAGE_MATH_MORPHOLOGY_OPEN_COMPOSE    0X00040006
#define IMAGE_MATH_MORPHOLOGY_GRADIENTIZE   0X00040007
#define IMAGE_MATH_MORPHOLOGY_TOP_HAT     0X00040008
//ͼ�񼸺α任
#define IMAGE_GEOMETRY_TRANSLATE          0X00050000
#define IMAGE_GEOMETRY_SCALE              0X00050001
#define IMAGE_GEOMETRY_ROTATE_CW_90       0X00050002
#define IMAGE_GEOMETRY_ROTATE_CCW_90      0X00050003
#define IMAGE_GEOMETRY_ROTATE_180         0X00050004
#define IMAGE_GEOMETRY_ROTATE_0           0X00050005
#define IMAGE_GEOMETRY_ROTATE             0X00050006
#define IMAGE_GEOMETRY_MIRROR_X           0X00050007
#define IMAGE_GEOMETRY_MIRROR_Y           0X00050008
//���α任������
#define IMAGE_GEOMETRY_NEAREST_NEIGHBOR_INTERPOLATE 0X00050100
#define IMAGE_GEOMETRY_BILINEAR_INTERPOLATE         0X00050101
#define IMAGE_GEOMETRY_THREE_ORDER_INTERPOLATE      0X00050102
//ͼ��һ��任
#define IMAGE_GEOMETRY_GENERAL_TRANSFORM      0X00050200
//Ť��
#define IMAGE_GEOMETRY_WARP_X           0X00050300
#define IMAGE_GEOMETRY_WARP_Y           0X00050301
// �ṹ����
//ͼ����ɫ:RGB
typedef struct tagPIXELCOLORRGB {
  BYTE red;
  BYTE green;
  BYTE blue;
} PIXELCOLORRGB;
//ͼ����ɫ:RGBA
typedef struct tagPIXELCOLORRGBA {
  BYTE red;
  BYTE green;
  BYTE blue;
  BYTE alpha;
} PIXELCOLORRGBA;
//��̬ѧ�ṹ
//�ṹԪ��
typedef struct tagMORPHOLOGYELEMENT {
  int x;        // Ԫ�ص�X����
  int y;        // Ԫ�ص�Y����
  short red;    // ��Ԫ�ص���ɫ:��ɫ����������Ϊ��ֵ
  short green;  // ��Ԫ�ص���ɫ:��ɫ����������Ϊ��ֵ
  short blue;   // ��Ԫ�ص���ɫ:��ɫ����������Ϊ��ֵ
} MORPHOLOGYELEMENT;
//��ά��������
typedef struct tagFLOATPOINT {
  float x;
  float y;
} FLOATPOINT;
//*******************   ��������       *********************
//�ضϺ���
//#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
//����ֵ����
//#define ABS(x)  (((x) < 0) ? -(x) : (((x) > 0) ? (x) : 0))
//#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
//#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
// ͼ�����
/* �汾�� */
#define GRAPHICS_DESIGN_VERSION_1_0     1
//��ҵ��ɫģ��
#define G3D_PIXEL_COLOR_RGB           0X00000000
#define G3D_PIXEL_COLOR_RGBA          0X00000001
#define G3D_PIXEL_COLOR_INDEX         0X00000002
//ֱ������������������λ�ù�ϵ,ע������ֻ�������ҹ�ϵ,���������¹�ϵ
//������ɨ�����
#define G3D_TRIANGLE_ON_LINE_RIGHT        0X00000100
#define G3D_TRIANGLE_ON_LINE_LEFT         0X00000101
//����˷����ڲ����㽻����,�����ڴ˶�����˻��ҳ˲���
#define G3D_MATRIX_MULTIPLIER_PRECONCAT     0X00000200
#define G3D_MATRIX_MULTIPLIER_POSTCONCAT    0X00000201

#define G3D_VERTICES             0X00000001
#define G3D_WIRE                 0X00000002
#define G3D_FACE                 0X00000004
#define G3D_TEX                  0X00000008

//GL_LINES            //���� �����ÿ�����㽫�������������һ��ֱ�ߣ���N/2��
//GL_LINE_STRIP       //�����еĵ����ӱ��һ������
//GL_LINE_LOOP        //��GL_LINE_STRIP���ɵ�����ͷβ�������γɱպ�ͼ��
//GL_TRIANGLES            //�����ÿ�����㽫����������Ϳ��һ�������Σ���N/3��
//GL_QUADS                �����ÿ�ĸ��㽫����������Ϳ��һ���ı��Σ���N/4��

#define G3D_POINTS                         0x4000
#define G3D_LINES                          0x4001
#define G3D_LINE_LOOP                      0x4002
#define G3D_LINE_STRIP                     0x4003
#define G3D_TRIANGLES                      0x4004
#define G3D_TRIANGLE_STRIP                 0x4005
#define G3D_TRIANGLE_FAN                   0x4006
#define G3D_QUADS                          0x4007
#define G3D_QUAD_STRIP                     0x4008
#define G3D_POLYGON                        0x4009
#define G3D_RENDER_ELEM_MARK               0x400f


#define G3D_FLAT                 0X00000010
#define G3D_GOURAUD              0X00000020
//������ɫģ��
#define G3D_RENDER_VERTICES             (G3D_VERTICES)
#define G3D_RENDER_WIRE                 (G3D_FLAT|G3D_WIRE)  //�߿�ģ��
#define G3D_RENDER_WIRE_LIGHTED         (G3D_GOURAUD|G3D_WIRE)
#define G3D_RENDER_FLAT                 (G3D_FLAT|G3D_FACE)  //����ģ��
#define G3D_RENDER_FLAT_TEX             (G3D_FLAT|G3D_TEX)   //��������ģ��
#define G3D_RENDER_GOURAUD_SMOOTH       (G3D_GOURAUD|G3D_FACE)  //�⻬����ģ��
#define G3D_RENDER_GOURAUD_SMOOTH_TEX   (G3D_GOURAUD|G3D_TEX)  //�⻬����ģ��
//���ʺ͹�Դ�Ļ�������:
#define G3D_AMBIENT               0X00000400
#define G3D_DIFFUSE               0X00000401
#define G3D_SPECULAR              0X00000402
//���λ��
#define G3D_POSITION              0X00000403
//�۹�����
#define G3D_SPOT_DIRECTION          0X00000404
#define G3D_SPOT_CUTOFF             0X00000405
#define G3D_SPOT_EXPONENT           0X00000406
//���˥������
#define G3D_CONSTANT_ATTENUATION        0X00000407
#define G3D_LINEAR_ATTENUATION          0X00000408
#define G3D_QUADRATIC_ATTENUATION       0X00000409
#define G3D_LIGHT_ON_OFF                0X0000040A
//����ר��
#define G3D_AMBIENT_AND_DIFFUSE   0X00000501
#define G3D_EMISSION              0X00000502
#define G3D_SHININESS             0X00000503
//����ģ��
#define G3D_LIGHT_MODEL_AMBIENT         0X00000600 //ȫ�ֻ�����
#define G3D_LIGHT_MODEL_LOCAL_VIEWER    0X00000601 //���ӵ�������Զ�ӵ�
#define G3D_LIGHT_MODEL_TWO_SIDE        0X00000602 //˫�����
//ָ������ǰ�桢���棬����˫��
#define G3D_FRONT                 0X00000700
#define G3D_BACK                  0X00000701
#define G3D_FRONT_AND_BACK        0X00000702
//��Ȳ�������
#define G3D_LESS                  0X00000800
#define G3D_LEQUAL                0X00000801
#define G3D_EQUAL                 0X00000802
#define G3D_GEQUAL                0X00000803
#define G3D_GREATER               0X00000804
#define G3D_NOTEQUAL              0X00000805
#define G3D_NEVER                 0X00000806
#define G3D_ALWAYS                0X00000807
// �ṹ����
//������ɫ:RGBA
#define FRGBASET(FRGBA_, R, G, B, A)   ((FRGBA_).r=(float)(R), (FRGBA_).g=(float)(G), (FRGBA_).b=(float)(B), (FRGBA_).a=(A))
#define FRGBASETv(FRGBA_, _V)          FRGBASET(FRGBA_, (_V)[0], (_V)[1], (_V)[2], (_V)[3])
#define FRGBSET(FRGBA_, R, G, B)       FRGBASET(FRGBA_, R, G, B, 1.f)
#define ROUND01(_A)   (_A=(_A<0.0f)?0.0f:((_A>1.0f)?1.0f:_A))
#define FRGBROUND01(_C)    (ROUND01(_C.r), ROUND01(_C.g), ROUND01(_C.b))
#define FRGBOPTv(_C, _OP)    ((_C).r _OP, (_C).g _OP, (_C).b _OP)
#define FRGBOPTvv(_C, _OP1, _C1)    ((_C).r _OP1 _C1.r, (_C).g _OP1 _C1.g, (_C).b _OP1 _C1.b)
#define FRGBOPTvvv(_C, _OP1, _C1, _OP2, _C2)    ((_C).r _OP1 _C1.r _OP2 _C2.r, (_C).g _OP1 _C1.g _OP2 _C2.g, (_C).b _OP1 _C1.b _OP2 _C2.b)
typedef struct _FRGBA {
  float r, g, b, a;
} FRGBA;
typedef struct _IRGBA {
  int r, g, b, a;
} IRGBA;

IRGBA IRGBA_FiI(FRGBA f)
{
  IRGBA i;
  i.r = F2FIX(f.r);
  i.g = F2FIX(f.g);
  i.b = F2FIX(f.b);
  i.a = F2FIX(f.a);
  return i;
}
#define V4SET(_C, X, Y, Z, W)    ((_C).x=(float)(X), (_C).y=(float)(Y), (_C).z=(float)(Z), (_C).w=(W))
#define V4SETv(_C, _V)           V4SET(_C, (_V)[0], (_V)[1], (_V)[2], (_V)[3])
#define V3SET(_C, X, Y, Z)       V4SET(_C, X, Y, Z, 1.f)
#define V3OPTv(_C, OP)          ((_C).x OP, (_C).y OP, (_C).z OP)
#define V3OPTvv(_C, _OP1, _C1)    ((_C).x _OP1 _C1.x, (_C).y _OP1 _C1.y, (_C).z _OP1 _C1.z, (_C).w=1.f)
#define V3OPTvvv(_C, _OP1, _C1, _OP2, _C2)    ((_C).x _OP1 _C1.x _OP2 _C2.x, (_C).y _OP1 _C1.y _OP2 _C2.y, (_C).z _OP1 _C1.z _OP2 _C2.z, (_C).w=1.f)
//��ά��������
typedef struct _VECTOR3D {
  float x, y, z, w;
} VECTOR3D;

// ������
typedef struct _Vector3 {
  float x, y, z;
} Vector3;
typedef struct _Vector2 {
  float x, y;
} Vector2;
//�򵥵�͸��ͶӰ,��������豸����任,�����۲����
//��ȡ����(���ڹ۲�����)
typedef struct _VIEWFINDER {
  float fDistView; // �Ӿ࣬�ӵ㵽ͶӰ��Ļ֮��ľ���
  float xScale;    // �����ӿڱ任ʱ,������ű���
  float yScale;    // �����ӿڱ任ʱ,�߶����ű���
  POINT ptCenter;  // ����,��������ϵ����ԭ��ͶӰ����Ļ�ϵ�λ��
} VIEWFINDER;
// ��������
//�ضϺ���
//����ֵ����
#undef ABS
#define ABS(x)  (((x) < 0) ? -(x) : (((x) > 0) ? (x) : 0))
#undef MAX
#undef MIN
#define MAX(a,b)  (((a) > (b)) ? (a) : (b))
#define MIN(a,b)  (((a) < (b)) ? (a) : (b))
void* mymalloc(int n)
{
  void* p2 = malloc(n);
  memset(p2, 0, n);
  return p2;
}
void* memdup1(const void* p, int n)
{
  void* p2 = mymalloc(n + 1);
  memcpy(p2, p, n);
  ((char*)p2)[n] = 0;
  return p2;
}
//#undef MALLOC
#define MEMDUP1(T, P, N)  (T*)memdup(P, (N)*sizeof(T))
#define MALLOC1(T, N)  (T*)mymalloc((N)*sizeof(T))
//#define MALLOC(T, N)  new T[N]

#define zb_error printf



#define T_CHAR     0x1400
#define T_UCHAR    0x1401
#define T_SHORT    0x1402
#define T_USHORT   0x1403
#define T_INT      0x1404
#define T_UINT     0x1405
#define T_FLOAT    0x1406
#define T_2_BYTES  0x1407
#define T_3_BYTES  0x1408
#define T_4_BYTES  0x1409
#define T_DOUBLE   0x140A
typedef struct _MatC {
  int h, w, bw, t;
  unsigned char* data;
} MatC,  *HMC;
#define SETHMC(hMC, W, H, BW, T, _P)  ((hMC)->h=H, (hMC)->w=W, (hMC)->bw=BW, (hMC)->t=T, (hMC)->data=(unsigned char*)(_P) )
#define GetMatElemSize(t)  (t<T_SHORT?1:(t<T_INT?2:(t<T_DOUBLE?4:8)))
HMC newMatC(int w, int h, int t, int esz)
{
  //int esz=GetMatElemSize(t);
  HMC hMC = (HMC)malloc(h * w * esz + sizeof(MatC));
  hMC->h = h, hMC->w = w, hMC->t = t, hMC->bw = w * esz, hMC->data = (uchar*)(hMC + 1);
  return hMC;
}

#define GetMatElem(_out, hMC, _x, _y, T)  {switch(hMC->t) { \
    case T_CHAR   : _out=(T)_PixelT(hMC, _x, _y, char   ); break; \
    case T_UCHAR  : _out=(T)_PixelT(hMC, _x, _y, uchar  ); break; \
    case T_SHORT  : _out=(T)_PixelT(hMC, _x, _y, short  ); break; \
    case T_USHORT : _out=(T)_PixelT(hMC, _x, _y, ushort ); break; \
    case T_INT    : _out=(T)_PixelT(hMC, _x, _y, int    ); break; \
    case T_UINT   : _out=(T)_PixelT(hMC, _x, _y, uint   ); break; \
    case T_FLOAT  : _out=(T)_PixelT(hMC, _x, _y, float  ); break; \
    case T_DOUBLE : _out=(T)_PixelT(hMC, _x, _y, double ); break; \
}}
#define _PixelTV(_out, T, hMC, _y, T2)           {T* _outf=(T*)&(_out);int _x=0;for (;_x<hMC->w; ++_x) _outf[_x]=(T)_PixelT(hMC, _x, _y, T2); }
#define GetMatElemV(_out, hMC, _y, T)  {switch(hMC->t) { \
    case T_CHAR   : _PixelTV(_out, T, hMC, _y, char   ); break; \
    case T_UCHAR  : _PixelTV(_out, T, hMC, _y, uchar  ); break; \
    case T_SHORT  : _PixelTV(_out, T, hMC, _y, short  ); break; \
    case T_USHORT : _PixelTV(_out, T, hMC, _y, ushort ); break; \
    case T_INT    : _PixelTV(_out, T, hMC, _y, int    ); break; \
    case T_UINT   : _PixelTV(_out, T, hMC, _y, uint   ); break; \
    case T_FLOAT  : _PixelTV(_out, T, hMC, _y, float  ); break; \
    case T_DOUBLE : _PixelTV(_out, T, hMC, _y, double ); break; \
}}
