#pragma comment (lib, "vfw32.lib")
#define RCWIDTH(_PRC) ((_PRC)->right-(_PRC)->left)
#define RCHEIGHT(_PRC) ((_PRC)->bottom-(_PRC)->top)
#define RCOFFSET(_PRC, _L, _T, _R, _B) ((_PRC)->left+=_L, (_PRC)->top+=_T, (_PRC)->right+=_R, (_PRC)->bottom+=_B)
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <vfw.h>
#include <xmmintrin.h>
unsigned char allimdata[] = {
#include "pushbox_allim.txt"
};
#include "GameBase.inl"
typedef struct _PushBoxGame {
  Image* im;
  int level; //��Ϸ����
  int mapslen;
  Image* maps[256];
  int rows, cols, step;
  char* map; //��ͼ��Ϣ
  POINT flagpt[10]; //���ĵ�
  POINT boxpoint[10]; //��������
  POINT babypoint; //��������
  POINT mousepoint; //��갴������
  int babyn; //����״̬
  int flagn; //��������
  int boxn; //���Ӹ���
  int box; //��¼����״̬
  BOOL Lkeydown; //����ȷ�Ͽ��Ʋ���
  BOOL GameOver; //��Ϸ��������
  HDC hdc;
  Image* allim;
  HWND m_hWnd;
} PushBoxGame;
enum {
  PID_BG,
  PID_WALL,
  PID_BASSWALL,
  PID_BOX,
  PID_BOX1,
  PID_FALG,
  PID_BABYF,
  PID_BABYL,
  PID_BABYR,
  PID_BABYB,
};
int SetMap(PushBoxGame* This, int h, int w, char* m)
{
  char* map1 = This->map;
  char* map0 = (char*)malloc(h * w);
  memcpy(map0, m, h * w);
  This->map = map0;
  This->rows = h, This->cols = w, This->step = w;
  if (map1) {
    free(map1);
  }
  return 0;
}
int SetMapi(PushBoxGame* This)
{
  int i, j, h, w;
  if (This->level < 0) {
    This->level = This->mapslen - 1;
  }
  if (This->level >= This->mapslen) {
    This->level = 0;
  }
  if (This->level >= This->mapslen) {
    return 0;
  }
  h = This->maps[This->level]->h, w = This->maps[This->level]->w;
  SetMap(This, h, w, (char*)(This->maps[This->level] + 1));
  This->flagn = 0;
  This->babypoint.x = 1, This->babypoint.y = 2;
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      if (This->map[i * This->step + j] == PID_FALG) {
        This->flagpt[This->flagn].x = j;
        This->flagpt[This->flagn].y = i;
        This->map[i * This->step + j] = 0;
        This->flagn++;
      }
      if (This->map[i * This->step + j] == PID_BABYF) {
        This->babypoint.x = j;
        This->babypoint.y = i;
        This->map[i * This->step + j] = 0;
      }
    }
  }
  //��ʼ������
  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (This->map[j * This->step + i] == PID_BOX) {
        This->boxpoint[This->boxn].x = i;
        This->boxpoint[This->boxn].y = j;
        This->boxn += 1;
      }
    }
  }
  return 1;
}
int AddMap(PushBoxGame* This, Image* im)
{
  This->maps[This->mapslen++] = im;
  return 0;
}
BOOL LoadMap(PushBoxGame* This, const char* fname)
{
  char* buf, *s, *pix;
  int i = 0, k, len, h, w;
  buf = (char*)loadfile(fname, &len);
  if (NULL == buf) {
    return 0;
  }
  s = buf;
  for (i = 0; i < This->mapslen; ++i) {
    free(This->maps[i]);
  }
  for (k = 0; *s && sscanf(s, "%d,%d", &h, &w) > 0;) {
    s = strchr(s, '\n');
    if (NULL == s) {
      break;
    }
    This->maps[k] = newImage(w, h, 1);
    pix = (char*)(This->maps[k] + 1);
    for (i = 0; i < h * w; ++s) {
      if (0 == *s) {
        assert(*s);
      }
      if ('0' <= *s && '9' >= *s) {
        pix[i++] = *s - '0';
      }
    }
    for (; '\r' == *s || '\n' == *s || ' ' == *s || '\t' == *s; ++s);
    ++k;
  }
  This->mapslen = k;
  free(buf);
  return 0;
}
void delPushBoxGame(PushBoxGame* This)
{
  if (This->map) {
    free(This->map);
  }
}
int initPushBoxGame(PushBoxGame* This)
{
  This->babyn = PID_BABYF;
  This->boxn = 0;
  This->Lkeydown = FALSE;
  This->level = 0;
  This->mapslen = 0;
  This->step = 8;
  memset(This->maps, 0, sizeof(This->maps));
  This->hdc = 0;
  LoadMap(This, "pushboxmap1.txt");
  This->allim = (Image*)allimdata;
  return 1;
}
int LMouseDown(PushBoxGame* This, int x, int y)
{
  This->mousepoint.x = x / 40;
  This->mousepoint.y = y / 30;
  This->Lkeydown = TRUE;
  return 0;
}
int DrawObj(PushBoxGame* This, int x, int y, int i)
{
  MapBitBlt(This->im, x, y, 40, 30, This->allim, i);
  DrawFrameToHDC(This->m_hWnd, This->im->w, This->im->h, This->im + 1, 1);
  return 0;
}
//�����յ��������ϡ��¡����ҷ���
static int BackDriect(PushBoxGame* This, POINT strpt, POINT endpt)
{
  int k = 0;
  int dx = endpt.x - strpt.x;
  int dy = endpt.y - strpt.y;
  if ((dx == 1) && (dy == 0)) { //�ҷ�
    return k = PID_BABYR;
  }
  if ((dx == -1) && (dy == 0)) { //��
    return k = PID_BABYL;
  }
  if ((dx == 0) && (dy == 1)) { //�Ϸ�
    return k = PID_BABYF;
  }
  if ((dx == 0) && (dy == -1)) { //�·�
    return k = PID_BABYB;
  }
  return k;
}
//�ƶ�����
POINT BabyMove(PushBoxGame* This, POINT strpt, POINT endpt)
{
  This->babyn = BackDriect(This, strpt, endpt);
  This->babypoint = endpt;
  DrawObj(This, strpt.x, strpt.y, PID_BG);
  DrawObj(This, This->babypoint.x, This->babypoint.y, This->babyn);
  return endpt;
}
//��������,����������û�к��Ӻ�С��ʱ
void FlagShow(PushBoxGame* This)
{
  int i;
  for (i = 0; i < This->flagn; i++) {
    if (This->map[This->flagpt[i].y * This->step + This->flagpt[i].x] != PID_BOX) {
      if (!((This->flagpt[i].x == This->babypoint.x) && (This->flagpt[i].y == This->babypoint.y))) {
        //DrawFlag(flagpt[i]);
        //��������
        DrawObj(This, This->flagpt[i].x, This->flagpt[i].y, PID_FALG);
      }
    }
  }
}
int DrawAll(PushBoxGame* This, HDC hDC)
{
  //����ͼ��Ϣ���ƽ���
  int i, j;
  for (j = 0; j < This->rows; j++) {
    for (i = 0; i < This->cols; i++) {
      MapBitBlt(This->im, i, j, 40, 30, This->allim, PID_BG); //�Ȼ�����
      MapBitBlt(This->im, i, j, 40, 30, This->allim, This->map[j * This->step + i]);
    }
  }
  DrawObj(This, This->babypoint.x, This->babypoint.y, This->babyn); //�������ݵĵ�ͼ��Ϣ���ƽ���
  //DrawAllBox();
  //��������
  FlagShow(This);
  return 0;
}
//��Ϸ��ʼ��
int InitGame(PushBoxGame* This, HWND hWnd)
{
  int h, w;
  This->m_hWnd = hWnd;
  This->babyn = PID_BABYF;
  //��¼����״̬
  This->boxn = 0;
  //��ʼ������
  This->mousepoint = This->babypoint;
  SetMapi(This);
  w = This->cols * 40, h = This->rows * 30;
  This->im = newImage(w, h, 4);
  //��ʼ����ͼ����
  if (This->hdc) {
    ReleaseDC(hWnd, This->hdc);
  }
  This->hdc = GetDC(hWnd);
  //This->allim = loadbmpfile("all.bmp", 32, 1);
  DrawAll(This, This->hdc);
  return 0;
}
//����ʶ��
BOOL IfPass(PushBoxGame* This)
{
  int i, j, k = 0;
  for (i = 0; i < This->flagn; i++) {
    for (j = 0; j < This->boxn; j++) {
      if (((This->flagpt[i].x == This->boxpoint[j].x) && (This->flagpt[i].y == This->boxpoint[j].y))) {
        k += 1;
      }
    }
  }
  if (k == This->flagn) {
    return TRUE;
  }
  return FALSE;
}
BOOL ProLG_MoveBox(PushBoxGame* This)
{
  int i, k;
  for (i = 0; i < This->boxn; i++) {
    if (This->mousepoint.x - This->boxpoint[i].x == 0 && This->mousepoint.y - This->boxpoint[i].y == 0) {
      int dx = This->boxpoint[i].x - This->babypoint.x;
      int dy = This->boxpoint[i].y - This->babypoint.y;
      if (abs(dx) < 2 && abs(dy) < 2 && !This->map[(This->boxpoint[i].y + dy)*This->step + This->boxpoint[i].x + dx]) {
        //������ͼ����
        This->map[This->boxpoint[i].y * This->step + This->boxpoint[i].x] = PID_BG;
        This->map[(This->boxpoint[i].y + dy)*This->step + This->boxpoint[i].x + dx] = PID_BOX;
        DrawObj(This, This->boxpoint[i].x, This->boxpoint[i].y, PID_BG);
        This->boxpoint[i].x += dx;
        This->boxpoint[i].y += dy;
        //��������Ƿ���������
        for (k = 0; k < This->flagn; k++) {
          if ((This->boxpoint[i].x == This->flagpt[k].x) && (This->boxpoint[i].y == This->flagpt[k].y)) {
            This->box = 1;
            k = This->flagn;
          }
          else {
            This->box = 0;
          }
        }
        //���ƺ���
        if (This->box == 0) {
          DrawObj(This, This->boxpoint[i].x, This->boxpoint[i].y, PID_BOX);
        }
        else {
          DrawObj(This, This->boxpoint[i].x, This->boxpoint[i].y, PID_BOX1);
        }
        This->babypoint = BabyMove(This, This->babypoint, This->mousepoint);
        FlagShow(This);
        if (IfPass(This)) {
          This->GameOver = TRUE;
        }
        return TRUE;
      }
    }
  }
  return TRUE;
}
BOOL ProLG_MoveBaby(PushBoxGame* This)
{
  //A*�㷨
  int i, stepn; //����
  POINT* backpoint; //·������
  backpoint = (POINT*)malloc(This->rows * This->cols * sizeof(POINT));
  stepn = AstarGetWay(This->rows, This->cols, This->map, This->step, This->babypoint, This->mousepoint, backpoint); //����A*�㷨·��������
  if (stepn == 0) {
    MessageBeep(MB_OK);
  }
  //��·������
  for (i = 0; i < stepn; i++) {
    This->babypoint = BabyMove(This, backpoint[i], backpoint[i + 1]);
    FlagShow(This);
    Sleep(20);
  }
  free(backpoint);
  return TRUE;
}
//��Ϸ�����߼�
BOOL ProLG(PushBoxGame* This, HWND hWnd)
{
  if (This->Lkeydown) {
    if ((This->map[This->mousepoint.y * This->step + This->mousepoint.x] == PID_BG)
        && ((This->mousepoint.x != This->babypoint.x)
            || (This->mousepoint.y != This->babypoint.y))) {
      //�����㲻����������ʱ�������ڿյ����ƶ�
      ProLG_MoveBaby(This);
    }
    else if (This->map[This->mousepoint.y * This->step + This->mousepoint.x] == PID_BOX) {
      //���ϡ��¡������ĸ�����������
      ProLG_MoveBox(This);
    }
    This->Lkeydown = FALSE;
    if (This->GameOver) {
      MessageBox(0, "��ϲ���أ��������һ��", "congratulation", 0);
      This->level += 1;
      if (This->level >= This->mapslen) {
        MessageBox(0, "��Ȼȫ��ͨ�أ�����ľ��������Խ�ˮ��", "�����´�", 0);
        return 0;
      }
      else {
        This->GameOver = FALSE;
        This->Lkeydown = FALSE;
        InitGame(This, hWnd);
      }
    }
  }
  return 0;
}
BOOL printMap(PushBoxGame* This, FILE* pf)
{
  int i, j;
  fprintf(pf, "%d,%d\r\n", This->rows, This->cols);
  for (i = 0; i < This->rows; ++i) {
    for (j = 0; j < This->cols; ++j) {
      fprintf(pf, "%d", (This->map[i * This->step + j]));
    }
    fprintf(pf, "\r\n");
  }
  return 0;
}

