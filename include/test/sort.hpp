/*
��������
*/
template <typename T> inline
void swap(T* list, int low, int high)
{
  T temp = list[low];
  list[low] = list[high];
  list[high] = temp;
}
/*
���������ȶ���
ԭ����������������������ļ�¼��ÿ���¼�¼��ǰ��������������н��бȽϣ�
�������뵽����������ȷ��λ��
������n2
*/
template <typename T, typename Comparator> inline
void inssort(T* list, int n, Comparator lt)
{
  int i, j;
  for (i = 1; i < n; i++) {
    for (j = i; (j > 0) && (lt(list[j], list[j - 1])); j--) {
      swap(list, j, j - 1);
    }
  }
}
/*
ð�������ȶ���
ԭ����һ�ε��ڲ�forѭ���Ӽ�¼����ĵײ��Ƚϵ��������Ƚ����ڵ�ֵ��
�������ŵ�ֵ�ȸ���ŵ�ֵ������߽���˳��
������n2
*/
template <typename T, typename Comparator> inline
void bubsort(T* list, int n, Comparator lt)
{
  int i, j;
  for (i = 0; i < n - 1; i++) {
    for (j = n - 1; j > i; j--) {
      if (lt(list[j], list[j - 1])) {
        swap(list, j, j - 1);
      }
    }
  }
}
/*
ѡ�����򣨲��ȶ���
ԭ��ѡ������ĵ�i���ǡ�ѡ�������е�iС�ļ�¼�������ü�¼�ŵ�����ĵ�i��λ�á�
�����ȴ�δ������������ҵ���С��ֵ�������ҵ���С�ģ���������
������n2
*/
template <typename T, typename Comparator> inline
void selsort(T* list, int n, Comparator lt)
{
  int i, j;
  for (i = 0; i < n - 1; i++) {
    int low_index = i;
    for (j = n - 1; j > i; j--) {
      if (lt(list[j], list[low_index])) {
        low_index = j;
      }
    }
    swap(list, i, low_index);
  }
}
/*
������������shell����ĸ���������
*/
template <typename T, typename Comparator> inline
void inssort_0(T* list, int n, int incr, Comparator lt)
{
  int i, j;
  for (i = incr; i < n; i += incr) {
    for (j = i; (j >= incr) && (lt(list[j], list[j - incr])); j -= incr) {
      swap(list, j, j - incr);
    }
  }
}
/*
shell���򣨲��ȶ���
ԭ�������зֳ������У�Ȼ��ֱ���������ò�������ķ�����������������������������
������n1.5
*/
template <typename T, typename Comparator> inline
void shellsort(T* list, int n, Comparator lt)
{
  int i, j;
  for (i = n / 2; i > 2; i /= 2) {
    for (j = 0; j < i; j++) {
      inssort_0(&list[j], n - j, i, lt);
    }
  }
  inssort_0(list, n, 1, lt);
}
/*
��������(���ȶ�)
ԭ������ѡ��һ����ֵ��С����ֵ�Ľ�㱻�ŵ���ֵ����ߣ�������ֵ�Ľ�㱻�ŵ���ֵ���ұߣ�������ֵ�ͱ���������ȷ��λ���ϡ�
Ȼ�����ֵ��ߺ��ұߵ������н������Ʋ�����
��һ�ε��õ���ʽΪquicksort(list,0,n-1)
������n*logn
*/
template <typename T, typename Comparator> inline
void quicksort_0(T* list, int low, int high, Comparator lt)
{
  int pivot = list[(low + high) / 2 ];
  int left = low - 1;
  int right = high;
  if (low >= high) {
    return ;
  }
  swap(list, (low + high) / 2, high);
  do {
    while (list[++left] < pivot);
    while (right != 0 && list[--right] > pivot);
    swap(list, left, right);
  }
  while (left < right);
  swap(list, left, right);
  swap(list, left, high);
  quicksort_0(list, low, left - 1, lt);
  quicksort_0(list, left + 1, high, lt);
}
template <typename T, typename Comparator> inline
void quicksort(T* list, int n, Comparator lt)
{
  quicksort_0(list, 0, n - 1, lt);
}
// ���ŷǵݹ��
template <typename _Ty2, typename Comparator> inline
void qsort(_Ty2* _F, int _N, Comparator _P)
{
  typedef _Ty2 _Ty;
  _Ty t;
  int sp = 0;
  struct {
    _Ty* lb;
    _Ty* ub;
  } stack[48];
  stack[0].lb = (_F);
  stack[0].ub = (_F) + (_N) - 1;
  while (sp >= 0) {
    _Ty* left = stack[sp].lb;
    _Ty* right = stack[sp--].ub;
    for (;;) {
      int i, n = (int)(right - left) + 1, m;
      if (n <= 64) {
        INSERT_SORT(left, right, _P, _Ty);
        break;
      }
      else {
        _Ty* left0, *left1, *right0, *right1, *pivot, *a, *b, *c;
        int swap_cnt = 0;
        left0 = left;
        right0 = right;
        pivot = left + (n / 2);
        if (n > 40) {
          int d = n / 8;
          a = left, b = left + d, c = left + 2 * d;
          left = MIDLTITER(a, b, c, _P);
          a = pivot - d, b = pivot, c = pivot + d;
          pivot = MIDLTITER(a, b, c, _P);
          a = right - 2 * d, b = right - d, c = right;
          right = MIDLTITER(a, b, c, _P);
        }
        a = left, b = pivot, c = right;
        pivot = MIDLTITER(a, b, c, _P);
        if (pivot != left0) {
          CV_SWAP(*pivot, *left0, t);
          pivot = left0;
        }
        left = left1 = left0 + 1;
        right = right1 = right0;
        for (;;) {
          while (left <= right && !(_P(*pivot, *left))) {
            if (!(_P(*left, *pivot))) {
              if (left > left1) {
                CV_SWAP(*left1, *left, t);
              }
              swap_cnt = 1;
              left1++;
            }
            left++;
          }
          while (left <= right && !(_P(*right, *pivot))) {
            if (!(_P(*pivot, *right))) {
              if (right < right1) {
                CV_SWAP(*right1, *right, t);
              }
              swap_cnt = 1;
              right1--;
            }
            right--;
          }
          if (left > right) {
            break;
          }
          CV_SWAP(*left, *right, t);
          swap_cnt = 1;
          left++;
          right--;
        }
        if (swap_cnt == 0) {
          left = left0, right = right0;
          INSERT_SORT(left, right, _P, _Ty);
          break;
        }
        n = MIN((int)(left1 - left0), (int)(left - left1));
        for (i = 0; i < n; i++) {
          CV_SWAP(left0[i], left[i - n], t);
        }
        n = MIN((int)(right0 - right1), (int)(right1 - right));
        for (i = 0; i < n; i++) {
          CV_SWAP(left[i], right0[i - n + 1], t);
        }
        n = (int)(left - left1);
        m = (int)(right1 - right);
        if (n > 1) {
          if (m > 1) {
            if (n > m) {
              stack[++sp].lb = left0;
              stack[sp].ub = left0 + n - 1;
              left = right0 - m + 1, right = right0;
            }
            else {
              stack[++sp].lb = right0 - m + 1;
              stack[sp].ub = right0;
              left = left0, right = left0 + n - 1;
            }
          }
          else {
            left = left0, right = left0 + n - 1;
          }
        }
        else if (m > 1) {
          left = right0 - m + 1, right = right0;
        }
        else {
          break;
        }
      }
    }
  }
}

