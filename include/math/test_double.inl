//#include "test_panorama.inl"
// "�쳣", // FPE_DENORMAL
// "����Ϊ��", // FPE_ZERODIVIDE
// "�����", // FPE_OVERFLOW
// "�����", // FPE_UNDERFLOW
// "������ʧ", // FPE_INEXACT
// "�������", // FPE_UNEMULATED
// "��������", // FPE_SQRTNEG
// "δ֪", // Unknown error
// "δ֪", // Unknown error
// "��ջ����", // FPE_STACKOVERFLOW
// "��ջ����", // FPE_STACKUNDERFLOW
// "�û�����", // FPE_EXPLICITGEN
//  1.#INF00 0x7FF0000000000000 �������1.0/0.0�����һ��INF�����
// -1.#INF00 0xFFF0000000000000 �������-1.0/0.0�����һ���������
//  1.#QNAN0 0x7fffffffffffffff ָQuiet Not a Number
// ��������ƽ�������������Ч����ͻ����һ��NAN����ı�ʾֵ
// IND ���ʾ����С������ȷ��
// ���⸡����1.#IND 1.#INF���ж�
//һ�����⸡�����ĺ���
// 1.#INF / inf�����ֵ��ʾ������� (infinity ����д)�����������˼�������Ա�ʾ�ĸ����������Χ������˵������ double ���͵����ֵ�������磬���� 0 ��һ������ʱ���õ�һ��1.#INF / infֵ����Ӧ�ģ������ 0 ��һ��������Ҳ��õ� -1.#INF / -inf ֵ��
// -1.#IND / nan���������������ӣ�һ����˵�������������κ�δ���������Ƿ����ĸ��������㡣"IND"�� indeterminate ����д����"nan"�� not a number ����д���������ֵ�ĳ��������У��Ը�����ƽ�����Ը���ȡ������0.0/0.0��0.0*��, ��/�� �ȡ�
// �������⸡�������ж�
// C ���ṩ��һ�麯�������ж�һ���������Ƿ��������� NaN��
// int _isnan(double x) ���������ж�һ���������Ƿ��� NaN���� int _finite(double x) �����ж�һ���������Ƿ��������
// ʹ��ʱҪ #include "float.h"
// Ҳ����������Ĵ���ʵ�֣�
// ����ıȽϲ�������ȥ���ǻ�õ� true
// ����Ȥ���Ƕ��ڸ����� NaN ��˵�ܻ�õ� false!
// bool IsNumber(double x) { return (x == x); }
// ������Ĵ�������ж�һ���������Ƿ������޵ģ�finite, ���Ȳ��� NaN �ֲ��� infinite����
// bool IsFiniteNumber(double x) { return (x <= DBL_MAX && x >= -DBL_MAX); }
// ���У�DBL_MAX �� <float.h> ��Ԥ����ĳ����� #define DBL_MAX         1.7976931348623158e+308
int test_double()
{
  int t = 0;
  union {
    uint64 i;
    double d;
  } num;
  num.i = 0x7fefffffffffffffu; // DBL_MAX
  t = _isnan(num.d);
  t = _finite(num.d);
  //printf("%f\n", num.d);
  num.i = 0x7fffffffffffffff;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // 1.#QNAN0
  num.i = 0x7FF0000000000000;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // 1.#INF00
  num.i = 0xfff8000000000000;
  t = _isnan(num.d);
  t = _finite(num.d);
  printf("%f\n", num.d); // -1.#IND00
  return 0;
}

