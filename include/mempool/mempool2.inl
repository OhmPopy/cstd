typedef unsigned char EigthByte;
//�ڴ�ص�Ĭ�ϴ�С�ͷ���ڵ��Ĭ�ϴ�С
static  const size_t DEFAULTMEMEPOOLSIZE = 1000;
static  const size_t DEFAULTMEMENODESIZE = 100;
//�ڴ��ʼ��������   ������λ1111 1111
static  const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF;
//�ڴ����ڵ㣨˫������
typedef struct TMemeNode
{
  //ָ��ǰһ�ڵ�
  TMemeNode *first;
  //ָ���һ�ڵ�
  TMemeNode *next;
  //�ڵ��С
  size_t idataSize;
  //�ڵ��Ƿ�ʹ��
  bool isUsed;
  //����Ľڵ�ĺ�һ�ڵ�
  TMemeNode *pEndNode;
  //��¼�ڴ�ط�����׵�ַ
  bool isMemeBegin;
  //���������ڴ��ַ
  EigthByte *Data;
  //ʹ���߶���ĵ�ַ
  void **pUser;
} TMemeLinkNode;

//�ڴ�ص�ʵ��
class  CMemePool
{
public:
  //�̳߳ع��캯��
  CMemePool(const size_t &sInitialMemoryPoolSize = DEFAULTMEMEPOOLSIZE,
    const size_t &sMemoryChunkSize = DEFAULTMEMENODESIZE);
  ~CMemePool();
  //�����ڴ�
  void *GetMemeroy(void **p,const size_t &sSize);
  //�ͷŷ����ڴ�
  void FreeAllocMemeroy(void *p,const size_t &sSize);
  //�ͷ��ڴ�������ڴ�
  void FreeAllMemeroy();
  //չʾ�ڴ�ص�ʹ�����
  void ShowTheMemePoolStatue();
  //��ȡ������Ϣ
  void GetErrorInfo();
private:
  //��ֹ�����빹�죬Ҫ���ݾ������ð�
  CMemePool(CMemePool *tCMemePool);
  CMemePool& operator =(CMemePool &tCMemePool);
  void AllocPoolMemeroy();
  void CalLinkNodeNum();
  void CalMemeSize();
  void LinkMemeryToNode(EigthByte *PAlloc);
  void UpdateLinkNodeSize(TMemeNode *PNode);
  void CalNeetLinkNumber(const size_t &sSize);
  void* FindMemeNode(const size_t &sSize);
  TMemeNode * SearchAllocNode(void *p);
  void CleanAllMemeDate();
  void CleatAllLinkNode();
  void ResetLinkToMemery();
  //˫�������ͷ�ڵ�
  TMemeLinkNode *m_Head;
  //˫������ĵ�ǰ�ڵ�
  TMemeLinkNode *m_Current;
  //˫����������ڵ�
  TMemeLinkNode *m_LastNode;
  EigthByte *m_PAlloc;
  //�����һ������ͷ��ַ
  bool m_isFirst;
  //�ڴ�������Ŀ
  size_t m_Number;
  //�ڴ���ܵ���Ŀ
  size_t m_AllNumber;
  //ÿһ���ڴ��Ĵ�С
  size_t m_MemLinkSize;
  //�ڴ�ط���Ĵ�С
  size_t m_MemePollSize;
  //�ڴ���ܷ����С
  size_t m_AllAloctsize;
  //�ڴ��ʹ�õĴ�С
  size_t m_MemePoolUseSize;
  //�ڴ�ؿ��еĴ�С
  size_t m_MemePoolFreeSize;
  //�����˶��ٸ�����
  size_t m_iUseObject;
  //���������Ϣ
  //������������ڴ��û���Ϣ
  void **m_User;
};

//---------------------------------------------------------------------------
//recalloc�������ڴ��֮ǰָ����ڴ��ָ���ʧЧ��
//��Ҫ���¶�λ��֮ǰ��������ָ��ҲҪ���¶�λ

/*****************************************
�ڴ�ع��캯��

*****************************************/
CMemePool::CMemePool(const size_t &sInitialMemoryPoolSize,const size_t &sMemoryChunkSize)
{
  //��ʼ���ڴ�صĴ�С
  m_MemePollSize =  sInitialMemoryPoolSize;
  //��ʼ��ÿ���ڴ��Ĵ�С
  m_MemLinkSize = sMemoryChunkSize;
  //��ʼ��һЩ����
  m_MemePoolFreeSize = 0;
  m_MemePoolUseSize = 0;
  m_Current = NULL;
  m_LastNode = NULL;
  m_PAlloc = NULL;
  m_Number = 0;
  m_AllAloctsize = 0;
  m_AllNumber = 0;
  m_iUseObject = 0;
  m_Head = new TMemeLinkNode;
  m_Head->next = NULL;
  m_Head->first = NULL;
  m_Head->Data = NULL;
  m_isFirst = true;
  //�����̳߳غ���
  AllocPoolMemeroy();
}
/*****************************************
�ڴ����������

*****************************************/
CMemePool::~CMemePool()
{
  FreeAllMemeroy();
}
/*****************************************
�ڴ�ط����ڴ溯��

*****************************************/
void CMemePool::AllocPoolMemeroy()
{
  //������Ҫ������ڵ���Ŀ
  CalLinkNodeNum();
  //��������Ҫ������ڴ��С
  CalMemeSize();
  m_AllNumber = m_AllNumber + m_Number;
  m_AllAloctsize += m_MemePollSize;
  m_MemePoolFreeSize += m_MemePollSize;
  //׷�ӷ����ڴ棬ԭ���ڴ�����ݲ����ܵ�Ӱ��
  m_PAlloc = (EigthByte *)realloc(m_PAlloc,(m_AllAloctsize)*sizeof(EigthByte));
  //�ڴ����ʧ��
  if(NULL == m_PAlloc)
  {
#define m_sError printf
    m_sError("Alloc Memeroy Pool Failture");
    return;
  }
  //���ǵ�һ�η����ڴ�
  if(false ==  m_isFirst)
  {   //�·����ڴ��ԭ��ָ��ȫʧЧ����Ҫ�ض���
    ResetLinkToMemery();
  }
  //������ڴ����ݳ�ʼ��
  // memset(((void *) PAlloc), NEW_ALLOCATED_MEMORY_CONTENT, m_MemePollSize);
  //��������̳߳��ڴ�������ڵ����
  LinkMemeryToNode(&m_PAlloc[m_AllAloctsize - m_MemePollSize]);
}
/*****************************************
��ԭ�ڴ��ָ������ض���Alloc��ԭ���ڴ���ܱ��ͷ��ˣ�

*****************************************/
void CMemePool::ResetLinkToMemery()
{
  TMemeLinkNode *pTemp = m_Head->next;
  int iIndex = 0;
  while(NULL != pTemp)
  {
    //�ض���ָ�������ָ��
    pTemp->Data = &m_PAlloc[iIndex * m_MemLinkSize];
    if(NULL != pTemp->pUser)
    {
      //�ض����û�ָ���ָ��
      *pTemp->pUser = pTemp->Data;
    }
    iIndex++;
    pTemp = pTemp->next;
  }
}
/*****************************************
������Ҫ���ڴ�����ڵ���Ŀ
*****************************************/
void CMemePool::CalLinkNodeNum()
{
  float fTempValue =   m_MemePollSize /  m_MemLinkSize;
  //����ȡ����Ҫ�Ľڵ���Ŀ
  m_Number = ceil(fTempValue);
}
/*****************************************
�����ڴ������������ڴ�Ĵ�С
*****************************************/
void CMemePool::CalMemeSize()
{
  m_MemePollSize = (size_t)(m_Number * m_MemLinkSize);
}
/*****************************************
��������ڴ������ڵ������
*****************************************/
void CMemePool::LinkMemeryToNode(EigthByte *PAlloc)
{
  TMemeLinkNode *PNode;
  //����ÿһ���ڵ����ռ�
  for(size_t iIndex = 0;  iIndex < m_Number; iIndex ++)
  {
    PNode = new TMemeLinkNode();
    if(NULL == m_LastNode)
    {
      PNode->next =  m_Head->next;
      m_Head->next = PNode;
      PNode->first = m_Head;
      m_LastNode = PNode;
    }
    else
    {
      PNode->next =  m_LastNode->next;
      m_LastNode->next = PNode;
      PNode->first = m_LastNode;
      m_LastNode = PNode;
    }
    m_LastNode->isUsed = false;
    m_LastNode->idataSize = m_MemePollSize - iIndex * m_MemLinkSize;
    m_LastNode->Data = &PAlloc[iIndex * m_MemLinkSize];
    m_LastNode->isMemeBegin = false;
    m_LastNode->pUser = NULL;
    //��¼�ڴ����׵�ַ���ͷ�ʱʹ��
    if(true == m_isFirst && 0 == iIndex)
    {
      m_LastNode->isMemeBegin = true;
      m_isFirst = false;
    }
  }
  UpdateLinkNodeSize(m_LastNode);
}
/*****************************************
���µ�ǰ�ڵ��ǰ���Сֵ
*****************************************/
void CMemePool::UpdateLinkNodeSize(TMemeNode *PNode)
{
  TMemeNode *PTemp;
  PTemp = PNode->next;
  int iDateSize = 0;
  //��ǰ�ڵ�ĺ�һ���ڵ�û���䣬�õ�����DataSizeֵ
  if(NULL != PTemp && false == PTemp->isUsed)
  {
    iDateSize = PTemp->idataSize;
  }
  //�����һ���ڵ�����ǰ�������������еĽڵ��Сֵ
  int iIndex = 1;
  while(PNode != m_Head && false == PNode->isUsed)
  {
    PNode->idataSize =  iIndex * m_MemLinkSize + iDateSize;
    iIndex++;
    PNode = PNode->first;
  }
  m_Current = PNode->next;
}
/*****************************************
�����ڴ�ռ�

*****************************************/
void *CMemePool::GetMemeroy(void **p,const size_t &sSize)
{
  m_MemePoolFreeSize -= sSize;
  m_MemePoolUseSize +=  sSize;
  //���������ڴ������û���Ϣ
  m_User = p;
  //���ӷ��������Ŀ
  m_iUseObject++;
  //�к��ʵ��ڴ��
  void *pFind = FindMemeNode(sSize);
  if(NULL != pFind)
  {
    return pFind;
  }
  TMemeNode *PTemp;
  PTemp = m_Current;
  m_Current = m_Current->next;
  //�����ڴ���ҵ����ʵ��ڴ�
  while(PTemp != m_Current)
  {
    //�ߵ���β����ͷ��
    if(NULL == m_Current)
    {
      m_Current = m_Head->next;
    }
    //�����Ѿ�����Ľڵ�
    if(true == m_Current->isUsed)
    {
      m_Current = m_Current->pEndNode;
      //  m_Current = m_Current->first;
    }
    pFind = FindMemeNode(sSize);
    if(NULL != pFind)
    {
      return pFind;
    }
    if(PTemp == m_Current)
    {
      break;
    }
    m_Current = m_Current->next;
  }
  //�ڵ�ǰ�����нڵ�������û�к��ʵģ��·����
  m_MemePollSize = sSize;
  AllocPoolMemeroy();
  return FindMemeNode(sSize);
}
/*****************************************
����������ڴ����Ŀ

*****************************************/
void CMemePool::CalNeetLinkNumber(const size_t &sSize)
{
  float fTempValue =   sSize /  m_MemLinkSize;
  //����ȡ����Ҫ�Ľڵ���Ŀ
  m_Number = ceil(fTempValue);
  if(0 == m_Number)
  {
    m_Number = 1;
  }
}
/*****************************************
�ҵ����ʵ��ڴ����ڵ���

*****************************************/
void* CMemePool::FindMemeNode(const size_t &sSize)
{
  if(m_Current->idataSize >= sSize && false == m_Current->isUsed)
  {
    CalNeetLinkNumber(sSize);
    size_t iIndex = 0;
    TMemeLinkNode *pTemp = m_Current;
    while(iIndex < m_Number)
    {
      m_Current->isUsed = true;
      m_Current = m_Current->next;
      iIndex++;
    }
    //��������ڴ���û���Ϣ
    pTemp->pUser = m_User;
    //��¼�����˼����ڴ棬�����������
    pTemp->pEndNode = m_Current->next;
    return pTemp->Data;
  }
  else
  {
    return NULL;
  }
}
/*****************************************
�ջط�����ڴ��

*****************************************/
void CMemePool::FreeAllocMemeroy(void *p,const size_t &sSize)
{
  //���ٷ�����Ŀ
  m_iUseObject--;
  m_MemePoolUseSize -= sSize;
  m_MemePoolFreeSize += sSize;
  TMemeNode *pFind = SearchAllocNode(p);
  if(NULL == pFind)
  {
    m_sError("can not find the alloc point,you may use wrong");
  }
  //�ָ��ڴ�Ϊ��ʼ��
  //memset(((void *) pFind->Data), NEW_ALLOCATED_MEMORY_CONTENT, sSize);
  //��������ƶ������ڴ��
  CalNeetLinkNumber(sSize);
  size_t iIndex = 0;
  while(iIndex != m_Number)
  {
    pFind->isUsed = false;
    pFind = pFind->next;
    iIndex++;
  }
  //�ڴ�黹�󣬸���ǰ��ڵ�Ĵ�С
  UpdateLinkNodeSize(pFind->first);
}
/*****************************************
����֮ǰ�����ڴ�Ľڵ�

*****************************************/
TMemeNode * CMemePool::SearchAllocNode(void *p)
{
  TMemeNode *PTemp = m_Head->next;
  while(NULL != PTemp)
  {
    if(PTemp->Data == (EigthByte *) p)
    {
      //�ͷ��ڴ���û����֮ǰ������û���Ϣ
      PTemp->pUser = NULL;
      return PTemp;
    }
    PTemp = PTemp->next;
  }
  return NULL;
}
/*****************************************
����̳߳�

*****************************************/
void CMemePool::FreeAllMemeroy()
{
  //�����ڴ�����ͷ���
  if(0 != m_iUseObject)
  {
    m_sError("warning there is some object not release");
  }
  CleanAllMemeDate();
  CleatAllLinkNode();
}
/*****************************************
�����ϵͳ������ڴ�

*****************************************/
void CMemePool::CleanAllMemeDate()
{
  TMemeLinkNode *pTemp = m_Head->next;
  while(NULL != pTemp)
  {
    //�ڴ汻������һ�飬���׵�ַ�Ϳ���ȫ��ɾ��
    if(pTemp->isMemeBegin&&NULL != pTemp->Data)
    {
      delete []pTemp->Data;
      return;
    }
    pTemp = pTemp->next;
  }
}
/*****************************************
���˫������

*****************************************/
void CMemePool::CleatAllLinkNode()
{
  TMemeLinkNode *pTemp = m_Head->next;
  while(NULL != pTemp)
  {
    TMemeLinkNode *qTemp = pTemp;
    pTemp = pTemp->next;
    delete qTemp;
  }
  if(NULL != m_Head)
  {
    delete m_Head;
  }
}
/*****************************************
��ʾ�ڴ������״̬

*****************************************/
void CMemePool::ShowTheMemePoolStatue()
{
  printf("\n\n\t\t\t�ڴ��ʹ��״�����\t\t\t\n\n");
  printf("\t\t���ڴ�ش�С:%dʹ�ô�С:%d���д�С:%d\n", m_AllAloctsize, m_MemePoolUseSize, m_MemePoolFreeSize);
  TMemeLinkNode *pTemp = m_Head->next;
  int iIndex = 1;
  while(NULL != pTemp)
  {
    printf("\n�ڴ�غ�:%d\t\t��С��%d\t\t�Ƿ�ʹ��:%d\n", iIndex, pTemp->idataSize, pTemp->isUsed);
    iIndex++;
    pTemp = pTemp->next;
  }
}



int test_mempool()
{
  CMemePool *g_ptrMemPool = new CMemePool();
  char *ptrCharArray = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArray,700);
  g_ptrMemPool->ShowTheMemePoolStatue();
  char * ptrCharArrayB = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArrayB,80);
  g_ptrMemPool->ShowTheMemePoolStatue();
  char * ptrCharArrayC = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArrayC,400);
  g_ptrMemPool->ShowTheMemePoolStatue();
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArray, 700);
  g_ptrMemPool->ShowTheMemePoolStatue();
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArrayC, 400);
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArrayB, 80);
  ptrCharArray = (char *) g_ptrMemPool->GetMemeroy((void**)&ptrCharArray,1300);
  g_ptrMemPool->ShowTheMemePoolStatue();
  for(int i=0; i< 1200; i++)
  {
    ptrCharArray[i] = 'a';
  }
  g_ptrMemPool->ShowTheMemePoolStatue();
  char c = ptrCharArray[800];
  g_ptrMemPool->FreeAllocMemeroy(ptrCharArray, 1300);
  g_ptrMemPool->ShowTheMemePoolStatue();
  delete g_ptrMemPool;
  printf("MemoryPool Program finished...\n");
  //system("PAUSE");
  return 0;
}
