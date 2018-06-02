//#include"datalist.h"
//#include"common.h"

static ListNode* listMakeNode(void* pdata, int32_t len);
static void listFreeNode(ListNode* p);

/*����ֵΪi�Ľڵ㣬�����ؽڵ��ַ*/
static ListNode* listMakeNode(void* pdata, int32_t len)
{
  ListNode* p;
  p = (ListNode*)malloc(sizeof(ListNode));

  if (!p) {
    return NULL;
  }

  p->systime = time(NULL);

  if (pdata && len) {
    p->data = malloc(len + 1);

    if (!p->data) {
      free(p);
      return NULL;
    }

    memcpy(p->data, pdata, len);
    *((char*)p->data + len) = 0;
  }
  else {
    p->data = NULL;
  }

  p->prev = NULL;
  p->next = NULL;

  return p;
}
//#define FreePointer(p)   if (p) {free(p);(p)=NULL;}
/*�ͷ�p��ָ�Ľڵ�*/
static void listFreeNode(ListNode* p)
{
  if (p && p->data) {
    FreePointer(p->data);
  }

  FreePointer(p);
}
/*����һ���յ�˫������*/
DList* listInit()
{
  DList* plist = (DList*)malloc(sizeof(DList));

  if (!plist) {
    return NULL;
  }

  pthread_mutex_init(&plist->locker, NULL);
  plist->head = NULL;
  plist->tail = NULL;
  plist->size = 0;

  return plist;
}

/*�ݻ�һ��˫������*/
void listDestroy(DList* plist)
{
  if (!plist) {
    return;
  }

  listClear(plist);
  free(plist);
  pthread_mutex_destroy(&plist->locker);
}

/*��һ��������Ϊ�ձ��ͷ�ԭ����ڵ�ռ�*/
void listClear(DList* plist)
{
  ListNode* temp, *p;

  if (!plist) {
    return;
  }

  p = plist->tail;

  while (p) {
    temp = p->prev;
    listFreeNode(p);
    p = temp;
  }

  plist->head = NULL;
  plist->tail = NULL;
  plist->size = 0;
}

/*������ݵ������ǰ��*/
ListNode* listPushFront(DList* plist, void* pdata, int32_t len)
{
  ListNode* pnode;

  if (!plist || !pdata || !len) {
    return NULL;
  }

  pnode = listMakeNode(pdata, len);

  if (!pnode) {
    return NULL;
  }

  pthread_mutex_lock(&plist->locker);
  pnode->next = plist->head;
  pnode->prev = NULL;

  if (plist->head) {
    plist->head->prev = pnode;
  }

  plist->head = pnode;

  if (!plist->tail) {
    plist->tail = pnode;
  }

  plist->size++;
  pthread_mutex_unlock(&plist->locker);

  return pnode;
}

/*������ݵ�����ĺ���*/
ListNode* listPushEnd(DList* plist, void* pdata, int32_t len)
{
  ListNode* pnode;

  if (!plist || !pdata || !len) {
    return NULL;
  }

  pnode = listMakeNode(pdata, len);

  if (!pnode) {
    return NULL;
  }

  pthread_mutex_lock(&plist->locker);
  pnode->next = NULL;
  pnode->prev = plist->tail;

  if (plist->tail) {
    plist->tail->next = pnode;
  }

  plist->tail = pnode;

  if (!plist->head) {
    plist->head = pnode;
  }

  plist->size++;
  pthread_mutex_unlock(&plist->locker);

  return pnode;
}

/*�������һ���ڵ�ɾ��,���ظýڵ�ĵ�ַ*/
ListNode* listDelNote(DList* plist, ListNode* pnode)
{
  ListNode* pret = NULL;

  if (!plist || !pnode) {
    return NULL;
  }

  pthread_mutex_lock(&plist->locker);

  if (pnode == plist->head) {
    if (plist->head == plist->tail) {
      plist->head = NULL;
      plist->tail = NULL;
    }
    else {
      plist->head = plist->head->next;
      plist->head->prev = NULL;
      pret = plist->head;
    }

    plist->size--;
  }
  else if (pnode == plist->tail) {
    if (plist->head == plist->tail) {
      plist->head = NULL;
      plist->tail = NULL;
    }
    else {
      plist->tail = plist->tail->prev;
      plist->tail->next = NULL;
    }

    plist->size--;
  }
  else {
    ListNode* p;

    for (p = plist->head; p && p->next && p->next->next; p = p->next) {
      if (p->next == pnode) {
        p->next = p->next->next;
        p->next->prev = p;
        pret = p->next;
        plist->size--;
        break;
      }
    }
  }

  pthread_mutex_unlock(&plist->locker);
  listFreeNode(pnode);
  return pret;
}

/*���ڵ��Ƿ����*/
int listCheckNote(DList* plist, ListNode* pnode)
{
  ListNode* p;

  if (!plist || !pnode) {
    return -1;
  }

  for (p = plist->head; p ; p = p->next) {
    if (p == pnode) {
      return 0;
    }
  }

  return -1;
}

/*����ʱ�ڵ�*/
int listDelTimeOutNote(DList* plist, int timeout_s)
{
  ListNode* p, *ptmp;
  time_t systime;

  if (!plist) {
    return -1;
  }

  systime = time(NULL);
  systime -= timeout_s;
  p = plist->tail;

  while (p) {
    ptmp = p->prev;

    if (p->systime < systime) {
      listDelNote(plist, p);
    }

    p = ptmp;
  }

  return 0;
}


