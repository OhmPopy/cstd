#ifndef _STR2F_INL_
#define _STR2F_INL_
// �ַ���ת���ɸ�����
// дһ��ת���������ú�����������һ����ʾ���������ַ������Ѹ��ַ���ת���ɸ������������
// �������뿼�Ǹ�����������Ҵ����е�ѭ�������٣����ܵ���API����crt���еĺ�����
// ���磺�����ַ���"345.7"�������������345.7��
float str2f(char* pstrfloat)
{
  BOOL bNegative = FALSE;
  BOOL bDec = FALSE;
  char* pSor = 0;
  char chByte = '0';
  float fInteger = 0.0;
  float fDecimal = 0.0;
  float fDecPower = 0.1f;
  // check
  if (!pstrfloat) {
    return 0.0;
  }
  // ������λ�жϣ��ж��Ƿ��Ǹ���
  if (pstrfloat[0] == '-') {
    bNegative = TRUE;
    pSor = pstrfloat + 1;
  }
  else {
    bNegative = FALSE;
    pSor = pstrfloat;
  }
  while (*pSor != '\0') {
    chByte = *pSor;
    if (bDec) {
      // С��
      if (chByte >= '0' && chByte <= '9') {
        fDecimal += (chByte - '0') * fDecPower;
        fDecPower = fDecPower * 0.1f;
      }
      else {
        return (bNegative ? -(fInteger + fDecimal) : fInteger + fDecimal);
      }
    }
    else {
      // ����
      if (chByte >= '0' && chByte <= '9') {
        fInteger = fInteger * 10.0f + chByte - '0';
      }
      else if (chByte == '.') {
        bDec = true;
      }
      else {
        return (bNegative ? -fInteger : fInteger);
      }
    }
    pSor++;
  }
  return (bNegative ? -(fInteger + fDecimal) : fInteger + fDecimal);
}
#endif // _STR2F_INL_

