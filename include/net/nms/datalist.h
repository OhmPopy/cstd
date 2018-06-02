#ifndef DataList_H
#define DataList_H

//#include <sys/ipc.h>
#include <pthread.h>
#include <stdlib.h>
//#include <sys/time.h>
/*����ڵ�����*/

typedef struct _ListNode {
  void* data;    /*������*/
  time_t systime;
  struct _ListNode* prev;     /*ָ��ǰ��*/
  struct _ListNode* next;     /*ָ����*/
} ListNode;

/*������������*/
typedef struct  _SList {
  ListNode* head;     /*ָ��ͷ�ڵ�*/
  ListNode* tail;     /*ָ��β�ڵ�*/
  pthread_mutex_t locker;
  int   size;
} DList;

/*��������*/
DList* listInit();

/*�ݻ�����*/
void listDestroy(DList* plist);

/*���������*/
void listClear(DList* plist);

/*������ݵ������ǰ��*/
ListNode* listPushFront(DList* plist, void* pdata, int32_t len);

/*������ݵ�����ĺ���*/
ListNode* listPushEnd(DList* plist, void* pdata, int32_t len);

/*�������еĽڵ�pnodeɾ��*/
ListNode* listDelNote(DList* plist, ListNode* pnode);

/*����ʱ�ڵ�*/
int listDelTimeOutNote(DList* plist, int timeout_s);

/*���ڵ��Ƿ����*/
int listCheckNote(DList* plist, ListNode* pnode);

#include "datalist.c"
#endif

