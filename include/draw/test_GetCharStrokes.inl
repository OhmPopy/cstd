
int test_GetCharStrokes()
{
  union {
    unsigned int ch;
    char szText[4];
  } a;
  int cnt = 0;
  char* szText = "��";
  a.ch = 0;
  a.szText[1] = szText[0];
  a.szText[0] = szText[1];
  //cnt = GetCharStrokes(a.ch);
  printf("\"%s\"�ıʻ�%d\n", szText, cnt);
  return 0;
}
