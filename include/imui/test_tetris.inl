#if 0
C���Զ���˹���飨���װ棩
������˹����ȫ���û�ͼ����ʵ�ַ���Ļ��ƣ�û��һ����Դ�����Դ�����С�ɣ���������������Ӻã�Ҳ��10K��ɡ��ǳ�С�ɵĶ���˹���顣
���˼�룺
1������Ϸ���򻮷�Ϊ18�� * 10�е����̣�����һ�������͵Ķ�ά����������Ա�ʾ�����ϸ����ط��Ƿ��з��顣
2����4�����������ֲ�ͬ��״�ķ��飬һ����������Ϸ�����У��ͰѶ�Ӧ�Ĳ����Ͷ�ά���������Ϊ�棬��ʾ�÷����Ѿ��з����ˡ�
3�����������㷽���ƶ��Ƿ����ײ���жϡ�
4�������Ѿ�������һ��СBUG��
5��ѹ�����е��ļ���δ���޸ĵ�Դ���룬�˴��Ĵ���Ϊ���¡�
6���������Ϊ�ı���״����Ϊֱ���䵽�ײ���p��Ϊ��ͣ������Pause���� ��ǩ�� < �� >
#endif
#define BLOCKWIDTH 20 //���������С
#define NUMLINEBLOCKS 18 //����
#define NUMCOLUMNBLOCKS 10 //����
#define ID_TIMER 1 //��ʱ��ID
#define BLOCKSTYLES (sizeof (tetris_blocks) / sizeof (tetris_blocks[0])) //�����������
#define GAME_STOP 0
#define GAME_RUN 1
typedef struct test_tetris_t {
  int inited;
  int cxChar, cyChar;
  IPOINT Block[4], NextBlock[4];
  BOOL pause;
  int score; //�÷�
  int game_time;
  int stage;
  //��Ϸ�������񶥵㲼��ֵ������÷����Ƿ��з���
  BOOL client[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
  int F, S, cF, cS; //�������ͼ�ζ�Ӧ�ĵ�һ����γ
  double start_time;
} test_tetris_t;
//�����������״���Ե��ʾ
const IPOINT tetris_blocks[][4][4] = {
  //��7
  0, 0, 1, 0, 1, 1, 1, 2, 2, 0, 0, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 2, 2, 2, 0, 1, 1, 1, 2, 1, 0, 2,
  //��7
  1, 0, 2, 0, 1, 1, 1, 2, 0, 1, 1, 1, 2, 1, 2, 2, 1, 0, 1, 1, 0, 2, 1, 2, 0, 0, 0, 1, 1, 1, 2, 1,
  //1
  1, 0, 1, 1, 1, 2, 1, 3, 0, 1, 1, 1, 2, 1, 3, 1, 1, 0, 1, 1, 1, 2, 1, 3, 0, 1, 1, 1, 2, 1, 3, 1,
  //Z
  0, 0, 1, 0, 1, 1, 2, 1, 2, 0, 1, 1, 2, 1, 1, 2, 0, 0, 1, 0, 1, 1, 2, 1, 2, 0, 1, 1, 2, 1, 1, 2,
  //��Z
  1, 0, 2, 0, 0, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2, 1, 0, 2, 0, 0, 1, 1, 1, 1, 0, 1, 1, 2, 1, 2, 2,
  //����
  0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1,
  //��ͷ
  1, 0, 0, 1, 1, 1, 2, 1, 0, 0, 0, 1, 1, 1, 0, 2, 0, 0, 1, 0, 2, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 2
};
//�жϷ����Ƿ��������
BOOL CanDown(test_tetris_t* s, const IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //���������ڸ����ȼ���ָ��Ϊ�޷���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //����������³����µױ߽磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
    if (pt[i].y + 1 == NUMLINEBLOCKS || s->client[pt[i].x][pt[i].y + 1]) {
      result = FALSE;
      break;
    }
  }
  //�ָ��������ڸ���Ϊ�з���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//�ж��Ƿ��������
BOOL CanLeft(test_tetris_t* s, IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //���������ڸ����ȼ���ָ��Ϊ�޷���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //����������Ƴ�����߽߱磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
    if (!pt[i].x || s->client[pt[i].x - 1][pt[i].y]) {
      result = FALSE;
      break;
    }
  }
  //�ָ��������ڸ���Ϊ�з���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//�ж��Ƿ��������
BOOL CanRight(test_tetris_t* s, IPOINT* pt)
{
  int i;
  BOOL result = TRUE;
  //���������ڸ����ȼ���ָ��Ϊ�޷���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  for (i = 0; i < 4; ++i) {
    //����������Ƴ�����߽߱磬����FALSE�����߼����С��������һ���Ѿ��з��飬���ΪFALSE
    if (pt[i].x + 1 == NUMCOLUMNBLOCKS || s->client[pt[i].x + 1][pt[i].y]) {
      result = FALSE;
      break;
    }
  }
  //�ָ��������ڸ���Ϊ�з���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
//�ж��Ƿ������ת
BOOL CanChange(test_tetris_t* s, IPOINT* pt)
{
  int i, k, t;
  BOOL result = TRUE;
  //���������ڸ����ȼ���ָ��Ϊ�޷���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
  }
  t = (s->cS + 1) % 4;
  for (k = 0; k < 4; ++k) {
    int x = tetris_blocks[s->cF][t][k].x - tetris_blocks[s->cF][s->cS][k].x,
        y = tetris_blocks[s->cF][t][k].y - tetris_blocks[s->cF][s->cS][k].y;
    if (s->client[pt[k].x + x][pt[k].y + y] || //�÷����Ѿ��з���
        pt[k].x + x > NUMCOLUMNBLOCKS - 1 || //x���곬Խ���ұ߽�
        pt[k].x + x < 0 || //x���곬Խ����߽�
        pt[k].y + y > NUMLINEBLOCKS - 1) { //y���곬Խ���µױ߽�
      result = FALSE;
      break;
    }
  }
  //�ָ��������ڸ���Ϊ�з���
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  return result;
}
int GameClient1[NUMCOLUMNBLOCKS* NUMLINEBLOCKS];
//ʵ����ת
int Change(test_tetris_t* s, IPOINT* pt)
{
  int i;
  int t = (s->cS + 1) % 4;
  for (i = 0; i < 4; ++i) {
    int x = tetris_blocks[s->cF][t][i].x - tetris_blocks[s->cF][s->cS][i].x,
        y = tetris_blocks[s->cF][t][i].y - tetris_blocks[s->cF][s->cS][i].y;
    s->client[pt[i].x][pt[i].y] = FALSE;
    pt[i].x += x;
    pt[i].y += y;
    s->client[pt[i].x][pt[i].y] = TRUE;
  }
  s->cS = t;
  if (0) {
    for (i = 0; i < NUMLINEBLOCKS; ++i) {
      int j;
      for (j = 0; j < NUMCOLUMNBLOCKS; ++j) {
        GameClient1[i * NUMCOLUMNBLOCKS + j] = s->client[j][i];
      }
    }
    //cvShowMat("GameClient1", "%d", NUMLINEBLOCKS, NUMCOLUMNBLOCKS, GameClient1, NUMCOLUMNBLOCKS*sizeof(BOOL), sizeof(BOOL), -1);
    //cvWaitKey(-1);
  }
  return 0;
}
//ʵ������
int Right(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    ++pt[i].x;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//ʵ������
int Left(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    --pt[i].x;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//ʵ�ַ��������
int Down(test_tetris_t* s, IPOINT* pt)
{
  int i, k;
  for (i = 0; i < 4; ++i) {
    s->client[pt[i].x][pt[i].y] = FALSE;
    ++pt[i].y;
  }
  for (k = 0; k < 4; ++k) {
    s->client[pt[k].x][pt[k].y] = TRUE;
  }
  return 0;
}
//������������ư�
unsigned Random(int n)
{
  SYSTEMTIME st;
  GetLocalTime(&st);
  srand(st.wMilliseconds);
  return rand() % n;
}
//���д����Լ���������
int DelSqure(test_tetris_t* s)
{
  int x, y;
  int line = 0, temp;
  for (x = NUMLINEBLOCKS - 1; x >= 0; --x) {
    BOOL result = TRUE;
    for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
      if (!s->client[y][x]) {
        result = FALSE;
        break;
      }
    }
    //�ж��Ƿ��������
    if (result) {
      temp = x;
      ++line;
      while (x > 0) {
        for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
          s->client[y][x] = s->client[y][x - 1];
        }
        --x;
      }
      for (y = 0; y < NUMCOLUMNBLOCKS; ++y) {
        s->client[y][0] = FALSE;
      }
      x = temp + 1;
    }
  }
  if (line) {
    s->score += (line - 1) * 2 + 1;
  }
  return 0;
}
double dtime()
{
  return sys_utime_counter() * 1. / sys_utime_frequency();
}
static int test_tetris()
{
  GETGUIIO2();
  static test_tetris_t s[1] = {0};
  IRECT rc;
  int x, y;
  int i;
  int m_text_height = 0;
  if (!s->inited) {
    s->inited = 1;
    s->stage = GAME_RUN;
    //��ʼ����һ�����ֵķ���
    s->cS = Random(4);
    s->cF = Random(BLOCKSTYLES);
    for (i = 0; i < 4; ++i) {
      s->Block[i].x = tetris_blocks[s->cF][s->cS][i].x + 4;
      s->Block[i].y = tetris_blocks[s->cF][s->cS][i].y;
      s->client[s->Block[i].x][s->Block[i].y] = TRUE;
    }
    s->S = Random(4);
    s->F = Random(BLOCKSTYLES);
    for (i = 0; i < 4; ++i) {
      s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
      s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
    }
    s->start_time = dtime();
  }
  s->pause = FALSE; //��ͣ
  if (1) {
    ISIZE sz = imuimeasureText("H", NULL, io->FontID, io->layoutflags, NULL);
    x = (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH;
    y = (NUMLINEBLOCKS + 3) * BLOCKWIDTH;
    if (!calc_rect(x, y, io->layoutflags, &rc)) {
      return 0;
    }
    m_text_height = sz.h + 2;
  }
  if (1) {
    double d = dtime();
    if (d - s->start_time > 0.1) {
      s->start_time = d;
      s->game_time++;
      if (s->stage == GAME_RUN && 0 == (s->game_time) % 5) {
        if (s->pause) {
          return 0;
        }
        if (CanDown(s, s->Block)) {
          Down(s, s->Block);
        }
        else {
          //�������ƣ���Ҫ���������жϣ���Ϸ�����������Ҫ������һ����ʾ���͵�ǰ��ʾ�ķ���
          DelSqure(s);
          for (i = 0; i < 4; ++i) {
            s->Block[i].x = s->NextBlock[i].x + 4;
            s->Block[i].y = s->NextBlock[i].y;
            if (s->client[s->Block[i].x][s->Block[i].y]) {
              s->stage = GAME_STOP;
            }
            else {
              s->client[s->Block[i].x][s->Block[i].y] = TRUE;
            }
          }
          s->cS = s->S;
          s->cF = s->F;
          s->S = Random(4);
          s->F = Random(BLOCKSTYLES);
          for (i = 0; i < 4; ++i) {
            s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
            s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
          }
        }
      }
    }
  }
  if (ISHOVEREDWIN2()) {
    switch (io->lastMsg) {
    case MSG_KEYDOWN:
      if (s->pause && io->lastChar != VK_PAUSE) {
      }
      else {
        switch (io->lastChar) {
        case VK_LEFT:
          if (CanLeft(s, s->Block)) {
            Left(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_RIGHT:
          if (CanRight(s, s->Block)) {
            Right(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_UP:
          printf("111111111\n");
          if (CanChange(s, s->Block)) {
            Change(s, s->Block);
          }
          printf("2222222222\n");
          force_redraw(c);
          break;
        case VK_DOWN:
          while (CanDown(s, s->Block)) {
            Down(s, s->Block);
          }
          force_redraw(c);
          break;
        case VK_PAUSE:
          s->pause = !s->pause;
          break;
        default:
          break;
        }
      }
      break;
    case MSG_CHAR:
      if (io->lastChar == 'p') {
        s->pause = !s->pause;
      }
      else if (io->lastChar == 'r') {
        s->score = 0;
        for (x = 0; x < NUMCOLUMNBLOCKS; ++x) {
          for (y = 0; y < NUMLINEBLOCKS; ++y) {
            s->client[x][y] = FALSE;
          }
        }
        s->cS = Random(4);
        s->cF = Random(BLOCKSTYLES);
        for (i = 0; i < 4; ++i) {
          s->Block[i].x = tetris_blocks[s->cF][s->cS][i].x + 4;
          s->Block[i].y = tetris_blocks[s->cF][s->cS][i].y;
          s->client[s->Block[i].x][s->Block[i].y] = TRUE;
        }
        s->S = Random(4);
        s->F = Random(BLOCKSTYLES);
        for (i = 0; i < 4; ++i) {
          s->NextBlock[i].x = tetris_blocks[s->F][s->S][i].x;
          s->NextBlock[i].y = tetris_blocks[s->F][s->S][i].y;
        }
        s->pause = FALSE;
        force_redraw(c);
      }
    }
  }
  if (1) {
    //if(pause) return 0;
    char szBufferScore[32];
    int fmt = TF_CENTER;
    IRECT rc1 = iRECT2(BLOCKWIDTH, BLOCKWIDTH, NUMCOLUMNBLOCKS * BLOCKWIDTH, NUMLINEBLOCKS * BLOCKWIDTH);
    ImDraw_edgeRect1(ctx, rc, Color_White, 0, 0);
    //������˹������Ϸ�ı߿�
    ImDraw_edgeRect1(ctx, rc1, Color_White, Color_Black, 1);
    //�������һ�����ַ���
    rc1 = iRECT2((NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, BLOCKWIDTH, 400, 300);
    gcSolidText(ctx, rc1, "��һ����", 0, io->FontID, fmt, Color_Black);
    //������÷֡��ַ���
    _snprintf(szBufferScore, 32, "�÷֣� %d", s->score);
    rc1 = iRECT2((NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, 6 * BLOCKWIDTH, 400, 300);
    gcSolidText(ctx, rc1, szBufferScore, 0, io->FontID, fmt, Color_Black);
    //��ʾ��Ϸ���ķ���
    for (x = 0; x < NUMCOLUMNBLOCKS; ++x) {
      for (y = 0; y < NUMLINEBLOCKS; ++y) {
        if (s->client[x][y]) {
          rc1 = iRECT2((x + 1) * BLOCKWIDTH, (y + 1) * BLOCKWIDTH, BLOCKWIDTH, BLOCKWIDTH);
          ImDraw_edgeRect1(ctx, rc1, Color_Black, 0, 0);
        }
      }
    }
    //��ʾ��һ����������ķ���
    for (i = 0; i < 4; ++i) {
      rc1 = iRECT2((s->NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, (s->NextBlock[i].y + 2) * BLOCKWIDTH, BLOCKWIDTH, BLOCKWIDTH);
      ImDraw_edgeRect1(ctx, rc1, Color_Black, 0, 0);
    }
  }
  return 0;
}

