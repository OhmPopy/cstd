// ��ʾ������Ҫ��
//
// 1. ����
// 1.1 ���������Զ�����ʶ����棬��ʵ��Ƶ����ʵʶ�������Ϣ
// 1.2 ������Ļ -> û����������һ���ɼ����ǹ���Ա , �ɼ������߼����ý���
// -> �й���Ա���� ���������Ա��֤ ����֤�ɹ�����߼����ý���
//
// 1.3 �߼����ý������
// <�û�����> �� ��ʾ�û�ע����Ƭ ������ɾ���û�
// <��¼����> �� ��ʾ�û�ʶ�����Ƭ��ʱ��
// <�ɼ�����> �� �Զ����ɱ�ţ�ע������
void imput_strf(img_t* im, int x, int y, COLOR color, const char* fmt, ...)
{
  char buf[256];
  va_list ARGLIST;
  va_start(ARGLIST, fmt);
  _vsnprintf(buf, countof(buf), fmt, ARGLIST);
  imput_str(im, x, y, buf, -1, color);
  va_end(ARGLIST);
}

