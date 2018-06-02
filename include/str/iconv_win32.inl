#include <windows.h>
#include <tchar.h>
#include <stdio.h>
void GBKSimplified2GBKTraditional(LPTSTR szSimplified)
{
  LCID dwLocale;
  WORD wLangID;
  int nLength;
  char* pBuffer;
  wLangID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
  dwLocale = MAKELCID(wLangID, SORT_CHINESE_PRC);
  nLength = LCMapString(dwLocale, LCMAP_TRADITIONAL_CHINESE, (LPCTSTR)szSimplified, strlen(szSimplified), NULL, 0);
  pBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (nLength + 1));
  pBuffer[nLength] = 0;
  LCMapString(dwLocale, LCMAP_TRADITIONAL_CHINESE, (LPCTSTR)szSimplified, strlen(szSimplified),
      pBuffer, nLength);
  strcpy(szSimplified, pBuffer);
  free(pBuffer);
  return;
}
// ��ת��
char* j2f(char* S)
{
  char* buf = (S);
  int n = strlen(buf);
  char* BS = S;
  int r = LCMapString(
      MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC),
      LCMAP_TRADITIONAL_CHINESE,
      BS, n, NULL, 0);
  TCHAR* ft = (TCHAR*)malloc(sizeof(TCHAR) * (n + 1));
  TCHAR* getbg = (TCHAR*)malloc(sizeof(TCHAR) * (n + 1));
  r = LCMapString(
      MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC),
      LCMAP_TRADITIONAL_CHINESE,
      BS, n, ft, n); //���apiԛ��
  _tcsncpy(getbg, ft, n);
  free(ft);
  return getbg;
}
//WCHAR ת���� CHAR����
void ConvertUnicodeToAnsi(WCHAR* Source, char* obj)
{
  DWORD dwNum = WideCharToMultiByte(950, 0, Source, -1, NULL, 0, NULL, FALSE);
  WideCharToMultiByte(950, 0, Source, -1, obj, dwNum, NULL, FALSE);
}
//CHAR TO WCHAR
void ConvertAnsiTounicode(char* source, WCHAR* obj)
{
  int nLength = 0;
  nLength = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
  MultiByteToWideChar(CP_ACP, 0, source, -1, obj, nLength);
}
// ����Big5�ַ�������Gb�����ַ�
char* Big2Gb(char* sBig)
{
  char* pszBig5 = NULL; //Big5������ַ�
  wchar_t* wszUnicode = NULL; //Unicode������ַ�
  char* pszGbt = NULL; //Gb����ķ����ַ�
  char* pszGbs = NULL; //Gb����ļ����ַ�
  char* sGb; //���ص��ַ���
  int iLen = 0; //��Ҫת�����ַ���
  pszBig5 = sBig; //������Ҫת�����ַ�����
  //����ת�����ַ���
  iLen = MultiByteToWideChar(950, 0, pszBig5, -1, NULL, 0) ;
  //��wszUnicode�����ڴ�
  wszUnicode = (wchar_t*)malloc(sizeof(wchar_t) * (iLen + 1));
  //ת��Big5�뵽Unicode�룬ʹ����API����MultiByteToWideChar
  MultiByteToWideChar(950, 0, pszBig5, -1, wszUnicode, iLen);
  //����ת�����ַ���
  iLen = WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //��pszGbt�����ڴ�
  pszGbt = (char*)malloc(sizeof(char) * (iLen + 1));
  //��pszGbs�����ڴ�
  pszGbs = (char*)malloc(sizeof(char) * (iLen + 1));
  //ת��Unicode�뵽Gb�뷱�壬ʹ��API����WideCharToMultiByte
  WideCharToMultiByte(936, 0, (PWSTR) wszUnicode, -1, pszGbt, iLen, NULL, NULL) ;
  //ת��Gb�뷱�嵽Gb����壬ʹ��API����LCMapString
  LCMapString(0x0804, LCMAP_SIMPLIFIED_CHINESE, pszGbt, -1, pszGbs, iLen);
  //����Gb������ַ�
  sGb = pszGbs;
  //�ͷ��ڴ�
  free(wszUnicode);
  free(pszGbt);
  //free(pszGbs);
  return sGb;
}
//��������Gb�ַ�������Big5�ַ�
char* Gb2Big(char* sGb)
{
  char* pszGbt = NULL; //Gb����ķ����ַ�
  char* pszGbs = NULL; //Gb����ļ����ַ�
  wchar_t* wszUnicode = NULL; //Unicode������ַ�
  char* pszBig5 = NULL; //Big5������ַ�
  char* sBig5; //���ص��ַ���
  int iLen = 0; //��Ҫת�����ַ���
  pszGbs = sGb; //������Ҫת�����ַ�����
  //����ת�����ַ���
  iLen = MultiByteToWideChar(936, 0, pszGbs, -1, NULL, 0) ;
  //��pszGbt�����ڴ�
  pszGbt = (char*)malloc(sizeof(char) * (iLen * 2 + 1));
  //ת��Gb����嵽Gb�뷱�壬ʹ��API����LCMapString
  LCMapString(0x0804, LCMAP_TRADITIONAL_CHINESE, pszGbs, -1, pszGbt, iLen * 2);
  //��wszUnicode�����ڴ�
  wszUnicode = (wchar_t*)malloc(sizeof(wchar_t) * (iLen + 1));
  //ת��Gb�뵽Unicode�룬ʹ����API����MultiByteToWideChar
  MultiByteToWideChar(936, 0, pszGbt, -1, wszUnicode, iLen);
  //����ת�����ַ���
  iLen = WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, NULL, 0, NULL, NULL) ;
  //��pszBig5�����ڴ�
  pszBig5 = (char*)malloc(sizeof(char) * (iLen + 1));
  //ת��Unicode�뵽Big5�룬ʹ��API����WideCharToMultiByte
  WideCharToMultiByte(950, 0, (PWSTR) wszUnicode, -1, pszBig5, iLen, NULL, NULL) ;
  //����Big5���ַ�
  sBig5 = pszBig5;
  //�ͷ��ڴ�
  free(wszUnicode);
  free(pszGbt);
  free(pszBig5);
  return sBig5;
}
// �ж��Ƿ�gb2312�����ַ�
int isgb(const void* pzInfo, int length)
{
  //int nWSize = MultiByteToWideChar( 936,MB_ERR_INVALID_CHARS,pzInfo,-1,NULL,0 );
  int error, nWSize;
  nWSize = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, (const char*)pzInfo, length, NULL, 0);
  if (0 == nWSize) {
    error = GetLastError();
    if (error == ERROR_NO_UNICODE_TRANSLATION) {
      return FALSE;
    }
  }
  //�ж��Ƿ���CP_UTF8,ֻҪ��936��CP_UTF8���漴��
  return TRUE;
}
int isuni(const void* pzInfo, int length)
{
  //�ж��Ƿ���CP_UTF8,ֻҪ��936��CP_UTF8���漴��
  return TRUE;
}
int isutf(const void* pzInfo, int length)
{
  int error, nWSize;
  nWSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (const char*)pzInfo, length, NULL, 0);
  if (0 == nWSize) {
    error = GetLastError();
    if (error == ERROR_NO_UNICODE_TRANSLATION) {
      return FALSE;
    }
  }
  //�ж��Ƿ���gb2312,ֻҪ�� CP_UTF8 ��936���漴��.
  return TRUE;
}
// �ж��ַ����Ƿ�UTF8����
// UTF8����8bits��1BytesΪ������������λ����ȻҲ�����л���16bits��32bits����ʽ���ֱ��ΪUTF16��UTF32����Ŀǰ�õò��࣬��UTF8�򱻹㷺Ӧ�����ļ���������紫���С�
// ����ԭ��
// �ȿ����ģ�壺
// UCS-4 range (hex.) UTF-8 octet sequence (binary)
// 0000 0000-0000 007F 0xxxxxxx
// 0000 0080-0000 07FF 110xxxxx 10xxxxxx
// 0000 0800-0000 FFFF 1110xxxx 10xxxxxx 10xxxxxx
// 0001 0000-001F FFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 0020 0000-03FF FFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 0400 0000-7FFF FFFF 1111110x 10xxxxxx ... 10xxxxxx
// ���벽�裺
// 1) ����ȷ����Ҫ���ٸ�8bits(octets)
// 2) ��������ģ�����ÿ��octets�ĸ�λbits
// 3) ���ַ���bits�����x�У��ַ�˳�򣺵�λ����λ��UTF8˳�����һ��octet����ĩλx����һ��octet���λx
// ����UTF8����,������6���ֽ����,����UTF8��1-6�ֽڱ������
int IsTextUTF8(const char* str, int length)
{
  int i;
  DWORD nBytes = 0; //UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
  UCHAR chr;
  BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8
  for (i = 0; i < length; i++) {
    chr = *(str + i);
    if ((chr & 0x80) != 0) { // �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
      bAllAscii = FALSE;
    }
    if (nBytes == 0) { //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
      if (chr >= 0x80) {
        if (chr >= 0xFC && chr <= 0xFD) {
          nBytes = 6;
        }
        else if (chr >= 0xF8) {
          nBytes = 5;
        }
        else if (chr >= 0xF0) {
          nBytes = 4;
        }
        else if (chr >= 0xE0) {
          nBytes = 3;
        }
        else if (chr >= 0xC0) {
          nBytes = 2;
        }
        else {
          return FALSE;
        }
        nBytes--;
      }
    }
    else { //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
      if ((chr & 0xC0) != 0x80) {
        return FALSE;
      }
      nBytes--;
    }
  }
  if (nBytes > 0) { //Υ������
    return FALSE;
  }
  if (bAllAscii) { //���ȫ������ASCII, ˵������UTF-8
    return FALSE;
  }
  return TRUE;
}
typedef unsigned char uint8_t;
int isutf8(const char* s, int ns)
{
  uint8_t x = 0, i = 0, j = 0, nbytes = 0, n = 0;
  for (i = 1; i < 7; i++) {
    x = (uint8_t)(255 << i);
    if (((uint8_t)*s & x) == x) {
      n = nbytes = (8 - i);
      for (j = 0; (j < nbytes && j < ns); j++) {
        if ((uint8_t)s[j] <= 0x80 && (uint8_t)s[j] >= 0xc0) {
          break;
        }
        else {
          n--;
        }
      }
      if (n == 0) {
        return nbytes;
      }
    }
  }
  return 0;
}
//
int isgbk(const char* s, size_t ns)
{
  if (ns > 2 && (uint8_t)*s >= 0x81 && (uint8_t)*s <= 0xfe
      && (
          ((uint8_t) * (s + 1) >= 0x80 && (uint8_t) * (s + 1) <= 0x7e)
          || ((uint8_t) * (s + 1) >= 0xa1 && (uint8_t) * (s + 1) <= 0xfe)
      )
     ) {
    return 1;
  }
  return 0;
}
//
int isGBKCode(const char* s, size_t ns)
{
  unsigned char ch1;
  unsigned char ch2;
  if (ns >= 2) {
    ch1 = (unsigned char)s[0];
    ch2 = (unsigned char)s[1];
    if (ch1 >= 129 && ch1 <= 254 && ch2 >= 64 && ch2 <= 254) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}
// �Ƿ�gb2312
int IsGB(const char* pText)
{
  unsigned char* sqChar = (unsigned char*)pText;
  if (sqChar[0] >= 0xa1) {
    if (sqChar[0] == 0xa3) {
      return 1;//ȫ���ַ�
    }
    else {
      return 2;//����
    }
  }
  else {
    return 0;//Ӣ�ġ����֡�Ӣ�ı��
  }
}
/***************************************************
* Function: IsChinese
* Description: �Ƿ�������
* Input: pzInfo�����ж��ַ�
* Output:
* Return:
* Others:
***************************************************/
BOOL IsChinese(const char* pzInfo)
{
  int i;
  char* pText = (char*)pzInfo;
  while (*pText != '\0') {
    i = IsGB(pText);
    switch (i) {
    case 0:
      pText++;
      break;
    case 1:
      pText++;
      pText++;
      break;
    case 2:
      pText++;
      pText++;
      return TRUE;
      break;
    }
  }
  return FALSE;
}
int gb2uni(const void* src, int srclen, void* dst)
{
  int utflen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)dst;
  //unilen = utf2uni((char*)src, srclen, uni);//ff fe e3 4e
  *uni++ = 0xfeff;
  unilen = 1 + MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (const char*)src, srclen, uni, unilen);
  return 2 * unilen;
}
int utf2uni(const void* src, int srclen, void* dst)
{
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)dst;
  unilen = 2 * MultiByteToWideChar(CP_UTF8, 0, (const char*)src, srclen, uni, unilen);
  return unilen;
}
int uni2gb(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  gblen = WideCharToMultiByte(CP_ACP , 0, (const unsigned short*)src, srclen / 2, (char*)dst, gblen, NULL, NULL);
  return gblen;
}
int uni2utf(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  gblen = WideCharToMultiByte(CP_UTF8 , 0, (const unsigned short*)src, srclen / 2, (char*)dst, gblen, NULL, NULL);
  return gblen;
}
// UTF8��ʽת��ΪASCII��ʽ�ַ���
int utf2gb(const void* src, int srclen, void* dst)
{
  int gblen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)malloc(unilen);
  //memset(uni, 0, unilen);
  //unilen = utf2uni((char*)src, srclen, uni);
  unilen = MultiByteToWideChar(CP_UTF8, 0, (const char*)src, srclen, uni, unilen);
  gblen = WideCharToMultiByte(CP_ACP , 0, uni, unilen, (char*)dst, gblen, NULL, NULL);
  free(uni);
  return gblen;
}
// ��acii�ַ���ת��Ϊutf8��ʽ; ����ת����ĳ���
// szBuf:ԭ�ַ���������
// szDest:utf-8Ŀ��
int gb2utf(const void* src, int srclen, void* dst)
{
  int utflen = 2 * srclen;
  int unilen = 2 * srclen;
  unsigned short* uni = (unsigned short*)malloc(unilen);
  //unilen = utf2uni((char*)src, srclen, uni);
  unilen = MultiByteToWideChar(CP_ACP, 0, (const char*)src, srclen, uni, unilen);
  utflen = WideCharToMultiByte(CP_UTF8 , 0, uni, unilen, (char*)dst, utflen, NULL, NULL);
  free(uni);
  return utflen;
}
// Big5 => GBK��
// ���ؤH���@�M�� --> ���A���񹲺͇�
int big2gbk(const char* szBuf, int nStrLen, char* pws)
{
  if (NULL != pws) {
    BOOL bValue = FALSE;
    int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, (wchar_t*)pws, nStrLen);
    nReturn = WideCharToMultiByte(936, 0, (wchar_t*)pws, nReturn, pws, nStrLen, "?", &bValue);
  }
  return sizeof(wchar_t) * nStrLen;
}
// GBK => Big5
// ���A���񹲺͇� --> ���ؤH���@�M��
int gbk2big(const char* szBuf, int nStrLen, char* pws)
{
  if (NULL != pws) {
    BOOL bValue = FALSE;
    MultiByteToWideChar(936, 0, szBuf, nStrLen, (wchar_t*)pws, nStrLen);
    WideCharToMultiByte(950, 0, (wchar_t*)pws, nStrLen, pws, nStrLen, "?", &bValue);
  }
  return sizeof(wchar_t) * nStrLen;
}
// GB2312 => GBK
// �л����񹲺͹� --> ���A���񹲺͇�
int gb2gbk(const TCHAR* szBuf, int nStrLen, TCHAR* pcBuf)
{
  WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
  int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
  if (NULL != pcBuf) {
    wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
    LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
  }
  return sizeof(char) * nReturn;
}
// GBK =��GB2312
// ���A���񹲺͇� --> �л����񹲺͹�
int gbk2gb(const TCHAR* szBuf, int nStrLen, TCHAR* pcBuf)
{
  WORD wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
  int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
  if (NULL != pcBuf) {
    wLCID = (WORD)MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
    LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
  }
  return sizeof(char) * nReturn;
}
int gb2big(const char* szBuf, int nStrLen, char* pcBuf)
{
  int len = gb2gbk(szBuf, nStrLen, 0);
  TCHAR* gbk = (TCHAR*)malloc(len);
  gb2gbk(szBuf, nStrLen, gbk);
  len = gbk2big(gbk, len, pcBuf);
  free(gbk);
  return len;
}
int big2gb(const char* szBuf, int nStrLen, char* pcBuf)
{
  int len = big2gbk(szBuf, nStrLen, 0);
  TCHAR* gbk = (TCHAR*)malloc(len);
  len = big2gbk(szBuf, nStrLen, gbk);
  len = gbk2gb(gbk, len, gbk);
  free(gbk);
  return len;
}
//1.str תGBK����
//2.str תUNICODE
//3.STR תBIG5
int test_GBK2()
{
  char p[200] = { 0 };
  WCHAR buf[200] = { 0 };
  strcpy(p, "����");
  GBKSimplified2GBKTraditional(p);
  ConvertAnsiTounicode(p, buf);
  strcpy(p, "");
  ConvertUnicodeToAnsi(buf, p);
  return 0;
}

