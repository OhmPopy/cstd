// A*�㷨��
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<iostream.h>
#define N 4
typedef struct QNode {
  int data[N][N];
  int ancent; //��Ƿ����������·ֱ�Ϊ 1234 5Ϊ�������ⷽ��
  int x;
  int y;
  int gone; //�Ƿ�����ýڵ㣬0δ������1������
  int value; //��Ŀ���״̬��=����λ���ƾ����+���
  int deep;
  struct QNode* father; //���ǰһ�ڵ���"store"�����е�λ��
  struct QNode* next;
} QNode, *QueuePtr;
typedef struct {
  QueuePtr head;
  QueuePtr rear;
} LinkQueue;
int A1[N][N] = {
  {1, 2, 3, 4},
  {5, 10, 6, 8},
  {0, 9, 7, 12},
  {13, 14, 11, 15}
};
int A[N][N] = {
  1, 2, 11, 7,
  5, 6, 0, 3,
  9, 10, 15, 4,
  13, 14, 12, 8,
};
int B[N][N] = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 0}
};
int x, y;
QueuePtr min;//�����С�Ľ��
bool check()
{
  //�ж��Ƿ���·�������ݳ�ʼ̬��Ŀ��̬����������ͬΪ������ͬΪż��������·��
  int temp = A[x][y];
  int i, j, sum2 = 0, sum1 = 0;
  int a[N * N], b[N * N];
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      a[i * N + j] = A[i][j];
    }
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      b[i * N + j] = B[i][j];
    }
  }
  for (i = 0; i < N * N - 1; i++) {
    for (j = i + 1; j < N * N; j++) {
      if (a[i] != temp && a[j] != temp && a[i] > a[j]) {
        sum1++;
      }
    }
  }
  for (i = 0; i < N * N - 1; i++) {
    for (j = i + 1; j < N * N; j++) {
      if (b[i] != temp && b[j] != temp && b[i] > b[j]) {
        sum2++;
      }
    }
  }
  if ((sum1 % 2 == 0 && sum2 % 2 == 1) || (sum1 % 2 == 1 && sum2 % 2 == 0)) {
    return false;
  }
  return true;
}
bool begin_opint()
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (A[i][j] == 0) {
        x = i;
        y = j;
        return true;
      }
    }
  }
  return false;
}
bool compare(int a[N][N])
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (a[i][j] != B[i][j]) {
        return false;
      }
    }
  }
  return true;
}
bool moveleft(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (y == 0) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x][y - 1];
  (*b)->data[x][y - 1] = k;
  (*b)->x = x;
  (*b)->y = y - 1;
  return true;
}
bool moveup(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (x == 0) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x - 1][y];
  (*b)->data[x - 1][y] = k;
  (*b)->x = x - 1;
  (*b)->y = y;
  return true;
}
bool movedown(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (x == N - 1) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x + 1][y];
  (*b)->data[x + 1][y] = k;
  (*b)->x = x + 1;
  (*b)->y = y;
  return true;
}
bool moveright(int a[N][N], QueuePtr* b, int x, int y)
{
  int k, i, j;
  if (y == N - 1) {
    return false;
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*b)->data[i][j] = a[i][j];
    }
  }
  k = (*b)->data[x][y];
  (*b)->data[x][y] = (*b)->data[x][y + 1];
  (*b)->data[x][y + 1] = k;
  (*b)->x = x;
  (*b)->y = y + 1;
  return true;
}
bool copy(QueuePtr* a)
{
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      (*a)->data[i][j] = A[i][j];
    }
  }
  return true;
}
void output(QueuePtr* p)
{
  int i, j;
  long int n = 0;
  for (; (*p)->father != NULL; (*p) = (*p)->father, n++) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
        printf(" %d", (*p)->data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
  printf("step is %d\n", n - 1);
}
int getvalue(QueuePtr* p)
{
  int count = 0; //�������
  bool test = true; //�����ҵ�һ��λ�õ�ֵ���������һ��
  //���㲻��λ�ľ����
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      test = true;
      for (int k = 0; k < N; k++) {
        for (int l = 0; l < N; l++) {
          if ((i != (*p)->x || j != (*p)->y) && (*p)->data[i][j] == B[k][l]) {
            count = count + abs(i - k) + abs(j - l);
            test = false;
          }
          if (test == false) {
            break;
          }
        }
        if (test == false) {
          break;
        }
      }
    }
  }
  count = count + (*p)->deep; //�������ֵ
  return count;
}
void test_15astar()
{
  QueuePtr closed, p, q;
  LinkQueue open;
  if (!begin_opint()) {
    printf("no 0 opint!!\n"); //ȷ��0��
    exit(0);
  }
  if (!check()) {
    printf("no answer!!\n"); //ȷ���Ƿ��н�
    exit(0);
  }
  open.head = open.rear = (QueuePtr)malloc(sizeof(QNode)); //ͷ���
  open.head->father = NULL;
  open.rear->next = open.head->next = NULL;
  closed = (QueuePtr)malloc(sizeof(QNode)); //ͷ���
  closed->next = NULL;
  closed->father = NULL;
  p = (QueuePtr)malloc(sizeof(QNode)); //S0��open��
  copy(&p);
  p->x = x;
  p->y = y;
  p->ancent = 5;
  p->deep = 0; //s0�����Ϊ0
  p->gone = 0;
  p->father = open.head;
  p->value = getvalue(&p); //
  p->next = open.head->next;
  open.head->next = p;
  open.rear = open.head;
  //min=p; //��ʼΪs0���
  if (compare(p->data)) {
    output(&p);
    exit(0);
  }
  while (open.head->next != NULL) {
    /*q=open.head->next; //open��closed
    open.head->next=q->next; //�Ƴ�open��ͷ���
    q->next=closed->next; //����closed��ı�ͷ
    closed->next=q;
    */
    //Ѱ����С״̬
    for (min = q = open.head->next; q != NULL; q = q->next) {
      if (q->value <= min->value && q->gone == 0) {
        min = q;
        break;
      }
    }
    min->gone = 1; //����С״̬�ѱ���
    min->father->next = min->next; //��open����ɾ���ҵ�����С̬
    min->next = closed->next; //����closed��ı�ͷ
    //min->father=closed; //��
    closed->next = min;
    //�ո���4�������ƶ�
    switch (closed->next->ancent) {
    case 1:
      p = (QueuePtr)malloc(sizeof(QNode)); //���Ƚ�������
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 2:
      p = (QueuePtr)malloc(sizeof(QNode)); //���Ƚ�������
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 3:
      p = (QueuePtr)malloc(sizeof(QNode)); //���Ƚ�������
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    case 4:
      p = (QueuePtr)malloc(sizeof(QNode)); //���Ƚ��������
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    default:
      p = (QueuePtr)malloc(sizeof(QNode)); //��ʼ���
      if (moveleft(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 1;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveup(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 2;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (movedown(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 3;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      p = (QueuePtr)malloc(sizeof(QNode)); //
      if (moveright(closed->next->data, &p, closed->next->x, closed->next->y)) {
        p->father = closed->next;
        p->ancent = 4;
        p->gone = 0;
        p->deep = min->deep + 1;
        p->value = getvalue(&p);
        p->next = open.rear->next;
        open.rear->next = p;
        open.rear = p;
        //�Ƚ�������
        if (compare(p->data)) {
          output(&p);
          exit(0);
        }
      }
      else {
        free(p);
      }
      break;
    }
  }
  printf("error: no answer!\n");
}

