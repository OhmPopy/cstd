// ������������㷨
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// �������
#define RANDPERM(n, arr, t) {int i;for(i=0;i<n;++i){int p=rand()%n;t=arr[p],arr[p]=arr[i],arr[i]=t;}}
#define SetBit(x,y) (x|=(1<<(y)))
#define ClearBit(x,y) (x&=~(1<<(y)))
#define TestBit(x,y) (x&(1<<(y)))
static BOOL sudoku_set_str(int* m_nGivens, LPCTSTR lpszString)
{
  BOOL ok = FALSE;
  int nCount, dwIndex, dwLen = strlen(lpszString);
  nCount = 0; // count of values read
  ASSERT(lpszString && (lpszString[0] != ('\0')));
  if ((dwLen >= 81) && lpszString && (lpszString[0] != ('\0'))) {
    int row = 0, col = 0;
    for (dwIndex = 0; dwIndex < dwLen; dwIndex++) {
      TCHAR c = lpszString[dwIndex];
      int nValue = 0;
      if (c == ('\0')) { // check for eol
        break;
      }
      if (c == ('.')) { // . = 0
        nValue = 0;
      }
      else if (isdigit(c)) {
        nValue = c - ('0');
      }
      else {
        continue;
      }
      m_nGivens[row * 9 + col++] = nValue;
      //TRACE(("%d: nValue=%d\n"), nCount, nValue);
      nCount++;
      if (nCount == 81) {
        break;
      }
      if (col >= 9) {
        row++;
        col = 0;
      }
    } // for
  }
  //TRACE(("nCount=%d\n"), nCount);
  return ok;
}
// GetRowCandidates()
// Purpose: Get candidates for a row
// Parameters: row - row number (0 - 8)
// Returns: WORD - lower 8 bits represent the candidate values that are
// possible for a cell (bit 0 is never set). A value is
// possible if no other cell in the row contains the value
// (a given, a user entered value, or a hint).
static WORD GetRowCandidates(const int* src, int row)
{
  WORD wBitFlags = 0x3FE;
  int col;
  for (col = 0; col < 9; col++) {
    int nGiven = src[row * 9 + col];
    if (nGiven) {
      ClearBit(wBitFlags, nGiven);
    }
  }
  return wBitFlags;
}
// GetColCandidates()
// Purpose: Get candidates for a column
// Parameters: col - column number (0 - 8)
// Returns: WORD - lower 8 bits represent the candidate values that are
// possible for a cell (bit 0 is never set). A value is
// possible if no other cell in the column contains the
// value (a given, a user entered value, or a hint).
static WORD GetColCandidates(const int* src, int col)
{
  WORD wBitFlags = 0x3FE;
  int row;
  for (row = 0; row < 9; row++) {
    int nGiven = src[row * 9 + col];
    if (nGiven) {
      ClearBit(wBitFlags, nGiven);
    }
  }
  return wBitFlags;
}
// GetRegionCandidates()
// Purpose: Get candidates for a 3x3 region
// Parameters: row - row number (0 - 8)
// col - column number (0 - 8)
// Returns: WORD - lower 8 bits represent the candidate values that are
// possible for a cell (bit 0 is never set). A value is
// possible if no other cell in the region contains the
// value (a given, a user entered value, or a hint).
static WORD GetRegionCandidates(const int* src, int row, int col)
{
  int i, j;
  WORD wBitFlags = 0x3FE;
  int nStartRow = (row / 3) * 3;
  int nStartCol = (col / 3) * 3;
  for (i = nStartRow; i < (nStartRow + 3); i++) {
    for (j = nStartCol; j < (nStartCol + 3); j++) {
      int nGiven = src[i * 9 + j];
      if (nGiven) {
        ClearBit(wBitFlags, nGiven);
      }
    }
  }
  return wBitFlags;
}
// GetCandidates()
// Purpose: Get candidates for a row, column, and 3x3 region
// Parameters: row - row number (0 - 8)
// col - column number (0 - 8)
// Returns: WORD - lower 8 bits represent the candidate values that are
// possible for a cell (bit 0 is never set). A value is
// possible if no other cell in the row, column, or region
// contains the value (a given, a user entered value, or
// a hint).
static WORD GetCandidates(const int* src, int row, int col)
{
  WORD wRowCandidates = GetRowCandidates(src, row);
  WORD wColCandidates = GetColCandidates(src, col);
  WORD wRegionCandidates = GetRegionCandidates(src, row, col);
  WORD wBitFlags = (WORD)(wRowCandidates & wColCandidates & wRegionCandidates);
  //TRACE(("Candidates for %d,%d: 0x%X\n"), row, col, wBitFlags);
  return wBitFlags;
}
//���������㷨
//1. �������м�ľŹ���
//2. ͨ�������С��У�����м�ľŹ�����ϡ��¡������ĸ��Ź���
//3. ͬ����ʽ����ĸ����ϵľŹ���
//4. �����Ѷ������ȥ��
static int sudoku_ouput(int* dst)
{
  int i, j;
  for (i = 0; i < 9; ++i) {
    for (j = 0; j < 9; ++j) {
      printf("%2d", dst[(i * 9 + j)]);
    }
    printf("\n");
  }
  return 0;
}
// ��ʼ������
// ����������м�ľŹ�����ͨ�������������������ĸ��Ź���
// ���ͨ�����������ĸ����ϵľŹ���
/*
����˳�����£�
9 7 8 3 1 2 6 4 5
3 1 2 6 4 5 9 7 8
6 4 5 9 7 8 3 1 2
7 8 9 1 2 3 4 5 6
1 2 3 4 5 6 7 8 9
4 5 6 7 8 9 1 2 3
8 9 7 2 3 1 5 6 4
2 3 1 5 6 4 8 9 7
5 6 4 8 9 7 2 3 1
*/
static int sudoku_init1(int* dst, int difficulty)
{
  int i, j, k;
  k = time(0);
  k = 12;
  srand(k);
  memset(dst, 0, sizeof(dst));
  // ��ʼ���м�ľŹ���
  {
    int src[9];
    for (i = 0; i < 9; ++i) {
      src[(i)] = (i + 1);
    }
    //random_shuffle(src.begin(), src.end()); // ���������
    RANDPERM(9, src, k);
    k = 0;
    for (i = 3; i < 6; ++i) {
      for (j = 3; j < 6; ++j) {
        dst[i * 9 + j] = src[k++];
      }
    }
  }
  // ���м�ľŹ��񽻲�任����ʼ�����������ĸ��Ź���
  {
    for (i = 3; i < 6; ++i) {
      int l = 0;
      for (j = 3; j < 6; ++j) {
        if (i == 3) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i + 2) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 4) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 5) {
          dst[(i - 2) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
      }
    }
    for (j = 3; j < 6; ++j) {
      int l = 0;
      for (i = 3; i < 6; ++i) {
        if (j == 3) {
          dst[(l + 0) * 9 + (j + 1)] = dst[(i) * 9 + (j)];
          dst[(l + 6) * 9 + (j + 2)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (j == 4) {
          dst[(l + 0) * 9 + (j + 1)] = dst[(i) * 9 + (j)];
          dst[(l + 6) * 9 + (j - 1)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (j == 5) {
          dst[(l + 0) * 9 + (j - 2)] = dst[(i) * 9 + (j)];
          dst[(l + 6) * 9 + (j - 1)] = dst[(i) * 9 + (j)];
          ++l;
        }
      }
    }
  }
  // ��ʼ���ĸ����ϵ��ĸ��Ź���
  {
    for (i = 0; i < 3; ++i) {
      int l = 0;
      for (j = 3; j < 6; ++j) {
        if (i == 0) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i + 2) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 1) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 2) {
          dst[(i - 2) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
      }
    }
    for (i = 6; i < 9; ++i) {
      int l = 0;
      for (j = 3; j < 6; ++j) {
        if (i == 6) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i + 2) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 7) {
          dst[(i + 1) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
        else if (i == 8) {
          dst[(i - 2) * 9 + (l + 0)] = dst[(i) * 9 + (j)];
          dst[(i - 1) * 9 + (l + 6)] = dst[(i) * 9 + (j)];
          ++l;
        }
      }
    }
  }
  // �����趨���Ѷ��������
  {
    difficulty *= 20;
    while (difficulty--) {
      dst[(rand() % 9) * 9 + (rand() % 9)] = 0;
    }
  }
  return 0;
}
//������Ϸ�������㷨
// �����Զ�����������Ϸ������
// 1. ����һ�л��һ�������1-9������
// 2. ���ý������㷨�õ�һ�����
// 3. ÿ������ڵ�1-8�����֡������Ҫ�ϴ���Ѷȣ�Ҳ���Խ�1-8��Ϊ2-8��3-8���ȵȡ�
// ��ͼ����9*9��char��ÿ��char��0-9��0��ʾ����
// �����ݣ�ͬʱ�����������ԡ����ݡ���int* a; ��a[(0)]Ϊ��ʱ��ʾ��ǰλ�ÿ���1��a[(1)]Ϊ��ʱ��ʾ����2���Դ�����
int sudoku_fill_line(int* dst, int line);
// ����һ�����ӡ����еĿ��������ڵ���ǰ��ã�Ҫ���ǵ����еĿ����Ժ;Ź���Ŀ�����
/* nums_possible : array (0-8) means possible of number (1-9) */
static int sudoku_fill_pos(int* dst, int* nums_possible, int line, int pos)
{
  int i, j;
  int m, n, nine_failed;
  int x, y, vetical_failed;
  if (pos >= 9) {
    return sudoku_fill_line(dst, line + 1);
  }
  if (dst[(pos) * 9 + (line)] != 0) {
    return sudoku_fill_pos(dst, nums_possible, line, pos + 1);
  }
  for (i = 0; i < 9; i++) {
    if (!nums_possible[i]) {
      continue;
    }
    /* ��鱾���Ƿ��ظ� */
    vetical_failed = 0;
    for (j = 0; j < 9; j++)
      if (dst[(pos) * 9 + (j)] == i + 1) {
        vetical_failed = 1;
        break;
      }
    if (vetical_failed) {
      continue;
    }
    /* ���Ź����Ƿ��ظ� */
    nine_failed = 0;
    m = pos / 3;
    n = line / 3;
    m *= 3;
    n *= 3;
    for (y = n; y < n + 3; y++) {
      for (x = m; x < m + 3; x++) {
        if (dst[(x) * 9 + (y)] == i + 1) {
          nine_failed = 1;
          break;
        }
      }
      if (nine_failed) {
        break;
      }
    }
    if (nine_failed) {
      continue;
    }
    /* all ok, try next position */
    dst[(pos) * 9 + (line)] = i + 1;
    nums_possible[i] = 0;
    if (sudoku_fill_pos(dst, nums_possible, line, pos + 1)) {
      // ������ȫ��OK��������һ��
      if (sudoku_fill_line(dst, line + 1)) {
        return 1;
      }
      // ��һ��ʧ�ܣ����³��Ա�λ�õ�ʣ�������
    }
    nums_possible[i] = 1;
    dst[(pos) * 9 + (line)] = 0;
  }
  return 0;
}
// ����һ��
static int sudoku_fill_line(int* dst, int line)
{
  int i;
  int nums[9];
  int saveline[9];
  if (line >= 9) {
    return 1;
  }
  //calc possibility(for the current line)
  for (i = 0; i < 9; i++) {
    nums[i] = 1; // all can be
  }
  for (i = 0; i < 9; i++) {
    char n = dst[(i) * 9 + (line)];
    // save line
    saveline[i] = dst[(i) * 9 + (line)];
    if (n != 0) {
      nums[(n - 1)] = 0; // appears
    }
  }
  if (!sudoku_fill_pos(dst, nums, line, 0)) {
    // restore line
    for (i = 0; i < 9; i++) {
      dst[(i) * 9 + (line)] = saveline[i];
    }
    return 0;
  }
  return 1;
}
static int sudoku_init2(int* dst, int d)
{
  int i, j, k;
  char t, ch[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  k = time(0);
  k = 12;
  srand(k);
  memset(dst, 0, 9 * 9 * sizeof(int));
  // �������һ��
  RANDPERM(9, ch, t);
  for (i = 0; i < 9; i++) {
    dst[(i) * 9 + (0)] = ch[i];
  }
  if (sudoku_fill_line(dst, 0)) {
    // �޼���һЩ��
    for (i = 0; i < 9; i++) {
      // ����n��ȡֵ��Χ�ɸı��Ѷ� %6 + 3�ǱȽ��ѵ�
      int n = (rand() % (9 - d)) + d;
      for (j = 0; j < 9; j++) {
        ch[j] = j; /* ch: index to erase */
      }
      RANDPERM(9, ch, t);
      for (j = 0; j < n; j++) {
        dst[(ch[(j)]) * 9 + (i)] = 0;
      }
    }
  }
  return 0;
}
static int test_sudoku_init()
{
  int dst[9 * 9];
  int d = 3;
  //sudoku_init1(dst, d);
  sudoku_init2(dst, d);
  sudoku_ouput(dst);
  return 0;
}
#define HEAD (0)
#if 0
#define NN (10)
#define MM (10)
typedef struct dlx_t {
  int idx; //�У���
  int L[NN* MM], R[NN* MM], U[NN* MM], D[NN* MM];
  int X[NN* MM], Y[NN* MM];
  int cntc[NN];
  int O[MM];
} dlx_t;
int dlx_free(dlx_t* d)
{
  return 0;
}
int dlx_setsize(dlx_t* d, int m, int n, const int* mx)
{
  return 0;
}
#else
typedef struct dlx_t {
  int idx; //�У���
  int* L, *R, *U, *D;
  int* X, *Y;
  int* cntc;
  int* O;
} dlx_t;
int dlx_free(dlx_t* d)
{
  free(d->O);
  memset(d, 0, sizeof(dlx_t));
  return 0;
}
int dlx_setsize(dlx_t* d, int m, int n, const int* mx)
{
  int sz = 0;
  int i, t;
  int* p;
  sz = m + n + 6 * m * n;
  t = n + 1;
  for (i = 0; i < m * n; i++) {
    t += mx[i] > 0;
  }
  sz = m + n + 6 * m * n;
  sz = m + n + 6 * t;
  p = (int*)realloc(d->O, sz * sizeof(int));
  memset(p, 0, sz * sizeof(int));
  d->O = p, p += m;
  d->cntc = p, p += n;
  d->L = p, p += t;
  d->R = p, p += t;
  d->U = p, p += t;
  d->D = p, p += t;
  d->X = p, p += t;
  d->Y = p, p += t;
  return 0;
}
#endif
//ɾ���м�����Ӧ����
int dlx_remove(dlx_t* d, int c)
{
  int i, j;
  d->L[d->R[c]] = d->L[c];
  d->R[d->L[c]] = d->R[c];
  for (i = d->D[c]; i != c; i = d->D[i]) {
    for (j = d->R[i]; j != i; j = d->R[j]) {
      d->U[d->D[j]] = d->U[j];
      d->D[d->U[j]] = d->D[j];
      d->cntc[d->X[j]]--;
    }
  }
  return 0;
}
//�ָ��м�����Ӧ����
int dlx_resume(dlx_t* d, int c)
{
  int i, j;
  d->R[d->L[c]] = c;
  d->L[d->R[c]] = c;
  for (i = d->D[c]; i != c; i = d->D[i]) {
    for (j = d->R[i]; j != i; j = d->R[j]) {
      d->U[d->D[j]] = j;
      d->D[d->U[j]] = j;
      d->cntc[d->X[j]]++;
    }
  }
  return 0;
}
BOOL dlx_dfs(dlx_t* d)
{
  int i, j, c, min;
  if (d->R[HEAD] == HEAD) {
    return TRUE;
  }
  min = 0xffffff;
  for (i = d->R[HEAD]; i != HEAD; i = d->R[i]) {
    if (d->cntc[i] < min) {
      min = d->cntc[i];
      c = i;
    }
  }
  dlx_remove(d, c);
  for (i = d->D[c]; i != c; i = d->D[i]) {
    //i��ĳ�����ţ����õ������е��кű���
    //d->O[d->idx++] = (i - 1) / n;
    d->O[d->idx++] = d->Y[i];
    for (j = d->R[i]; j != i; j = d->R[j]) {
      dlx_remove(d, d->X[j]);
    }
    if (dlx_dfs(d)) {
      return TRUE;
    }
    for (j = d->L[i]; j != i; j = d->L[j]) {
      dlx_resume(d, d->X[j]);
    }
    d->idx--;
  }
  dlx_resume(d, c);
  return FALSE;
}
BOOL dlx_build(dlx_t* d, int m, int n, const int* mx0)
{
  int i, j, t, pre, first;
  int* mx = (int*)malloc(m * n * sizeof(int));
  dlx_setsize(d, m, n, mx0);
  t = n + 1;
  for (i = 0; i < m * n; i++) {
    mx[i] = 0;
    if (mx0[i]) {
      mx[i] = t++;
    }
  }
  d->idx = 0;
  for (i = 0; i < n; i++) {
    d->R[i] = i + 1;
    d->L[i + 1] = i;
  }
  d->R[n] = 0;
  d->L[0] = n;
  //��˫������
  for (j = 1; j <= n; j++) {
    pre = j;
    d->cntc[j] = 0;
    for (i = 1; i <= m; i++) {
      t = mx[(i - 1) * n + (j - 1)];
      if (t) {
        d->cntc[j]++;
        d->X[t] = j;
        d->Y[t] = i;
        d->D[pre] = t;
        d->U[t] = pre;
        pre = t;
      }
    }
    d->U[j] = pre;
    d->D[pre] = j;
    if (d->cntc[j] == 0) {
      free(mx);
      return FALSE;
    }
  }
  //��˫������
  for (i = 1; i <= m; i++) {
    pre = first = -1;
    for (j = 1; j <= n; j++) {
      t = mx[(i - 1) * n + (j - 1)];
      if (t) {
        if (pre == -1) {
          first = t;
        }
        else {
          d->R[pre] = t;
          d->L[t] = pre;
        }
        pre = t;
      }
    }
    if (first != -1) {
      d->R[pre] = first;
      d->L[first] = pre;
    }
  }
#if 0
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      t = mx[(i - 1) * n + (j - 1)];
      if (t) {
        printf("%2d %2d %2d %2d %2d %2d %2d %2d\n", i, j, t, d->L[t], d->R[t], d->U[t], d->D[t], d->X[t]);
      }
    }
  }
#endif
  free(mx);
  return TRUE;
}
int dlx_print(dlx_t* d)
{
  int i;
  for (i = 0; i < d->idx; i++) {
    int r = d->O[i];
    printf("%d\n", r);
  }
  return 0;
}
int dlx_sudoku_out(int idx, const int* O, int* ans)
{
  int i;
  int x, y, k, num;
  for (i = 0; i < idx; i++) {
    int r = O[i];
    k = r % 9;
    if (k == 0) {
      k = 9;
    }
    num = (r - k) / 9 + 1;
    y = num % 9;
    if (y == 0) {
      y = 9;
    }
    x = (num - y) / 9;
    ans[x * 9 + y - 1] = k;
  }
  if (idx == 0) {
    //printf("impossible\n");
  }
  if (idx == 0) {
    printf("impossible\n");
  }
  return idx != 0;
}
//DLX���9*9���������⣬ת��Ϊ729*324�ľ�ȷ��������
//�У�һ��9 * 9 * 9 == 729�С�һ��9 * 9С��ÿһ����9�ֿ�����(1 - 9)��ÿһ�ֿ��ܶ���Ӧ��һ�С�
//�У�һ��(9 + 9 + 9) * 9 + 81 == 324 ��ǰ������9�ֱ������9��9�к�9С�飬����9����˼��9�ֿ���(1 - 9)����Ϊÿ�ֿ���ֻ����ѡ��һ����
//81������81��С��������ÿһ��С��ֻ��һ�����֡�
//�������ݺ����Ϊ'.'����9�У�����1-9�ֿ��ܣ�����һ�У���ʾĳС��ֻ�ܷ�ȷ����ĳ�����֡�
int sudoku_solve_dlx(dlx_t* d, const char* s, int* ans)
{
  int i, j, k, m, n;
  int mx[324 * 729];
  memset(mx, 0, sizeof(mx));
  n = 324;
  m = 729;
  for (i = 1; i <= 9; i++) {
    for (j = 1; j <= 9; j++) {
      int t = (i - 1) * 9 + j;
      if (s[t - 1] == '0') {
        for (k = 1; k <= 9; k++) {
          mx[(9 * (t - 1) + k - 1)*n + t - 1] = 1; //81grid ÿ��С��ֻ�ܷ�һ������
          mx[(9 * (t - 1) + k - 1)*n + 81 + (i - 1) * 9 + k - 1] = 1; //9row ÿ������kֻ�ܳ���һ��
          mx[(9 * (t - 1) + k - 1)*n + 162 + (j - 1) * 9 + k - 1] = 1; //9col ÿ������kֻ�ܳ���һ��
          mx[(9 * (t - 1) + k - 1)*n + 243 + ((i - 1) / 3 * 3 + (j + 2) / 3 - 1) * 9 + k - 1] = 1; //subgrid ÿ��3*3��������kֻ�ܳ���һ��
        }
      }
      else {
        k = s[t - 1] - '0';
        mx[(9 * (t - 1) + k - 1)*n + t - 1] = 1; //81grid
        mx[(9 * (t - 1) + k - 1)*n + 81 + (i - 1) * 9 + k - 1] = 1; //9row
        mx[(9 * (t - 1) + k - 1)*n + 162 + (j - 1) * 9 + k - 1] = 1; //9col
        mx[(9 * (t - 1) + k - 1)*n + 243 + ((i - 1) / 3 * 3 + (j + 2) / 3 - 1) * 9 + k - 1] = 1; //subgrid
      }
    }
  }
  dlx_build(d, m, n, mx);
  dlx_dfs(d);
  dlx_sudoku_out(d->idx, d->O, ans);
  //dlx_print(d);
  return 0;
}
int sudoku_solve_dlx_i32(dlx_t* d, const int* src, int* dst)
{
  char s[100] = {0};
  int j;
  for (j = 0; j < 9 * 9; ++j) {
    s[j] = '0' + src[j];
  }
  return sudoku_solve_dlx(d, s, dst);
}

