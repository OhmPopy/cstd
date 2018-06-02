
////////////////////////////////////////////////////////////////
//BOOL WINAPI GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped, DWORD dwMilliseconds);
//lpCompletionKey [out] ��Ӧ��PER_SOCKET_CONTEXT�ṹ������CreateIoCompletionPort���׽��ֵ���ɶ˿�ʱ���룻
//A pointer to a variable that receives the completion key value associated with the file handle whose I/O operation has completed.
//A completion key is a per-file key that is specified in a call to CreateIoCompletionPort.
//lpOverlapped [out] ��Ӧ��PER_IO_CONTEXT�ṹ���磺����accept����ʱ������AcceptEx����ʱ���룻
//A pointer to a variable that receives the address of the OVERLAPPED structure that was specified when the completed I/O operation was started.
/////////////////////////////////////////////////////////////////
// ����ɶ˿���Ͷ�ݵ�I/O����������
typedef enum OPERATION_TYPE {
  ACCEPT_POSTED, // ��־Ͷ�ݵ�Accept����
  SEND_POSTED, // ��־Ͷ�ݵ��Ƿ��Ͳ���
  RECV_POSTED, // ��־Ͷ�ݵ��ǽ��ղ���
  NULL_POSTED // ���ڳ�ʼ����������
} OPERATION_TYPE;
//ÿ���׽��ֲ���(�磺AcceptEx, WSARecv, WSASend��)��Ӧ�����ݽṹ��OVERLAPPED�ṹ(��ʶ���β���)���������׽��֣����������������ͣ�
typedef struct PER_IO_CONTEXT {
  OVERLAPPED m_Overlapped; // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)
  SOCKET m_sockAccept; // ������������ʹ�õ�Socket
  WSABUF m_wsaBuf; // WSA���͵Ļ����������ڸ��ص�������������
  char m_szBuffer[MAX_BUFFER_LEN]; // �����WSABUF�������ַ��Ļ�����
  OPERATION_TYPE m_OpType; // ��ʶ�������������(��Ӧ�����ö��)
  DWORD m_nTotalBytes; //�����ܵ��ֽ���
  DWORD m_nSendBytes; //�Ѿ����͵��ֽ�������δ��������������Ϊ0
} PER_IO_CONTEXT;

//���캯��
void PER_IO_CONTEXT_init(PER_IO_CONTEXT* s) {
  ZeroMemory(&s->m_Overlapped, sizeof(s->m_Overlapped));
  ZeroMemory(s->m_szBuffer, MAX_BUFFER_LEN);
  s->m_sockAccept = INVALID_SOCKET;
  s->m_wsaBuf.buf = s->m_szBuffer;
  s->m_wsaBuf.len = MAX_BUFFER_LEN;
  s->m_OpType = NULL_POSTED;
  s->m_nTotalBytes = 0;
  s->m_nSendBytes = 0;
}
//��������
void PER_IO_CONTEXT_free(PER_IO_CONTEXT* s) {
  if (s->m_sockAccept != INVALID_SOCKET) {
    closesocket(s->m_sockAccept);
    s->m_sockAccept = INVALID_SOCKET;
  }
}
//���û���������
void PER_IO_CONTEXT_ResetBuffer(PER_IO_CONTEXT* s) {
  ZeroMemory(s->m_szBuffer, MAX_BUFFER_LEN);
  s->m_wsaBuf.buf = s->m_szBuffer;
  s->m_wsaBuf.len = MAX_BUFFER_LEN;
}

//ÿ��SOCKET��Ӧ�����ݽṹ(����GetQueuedCompletionStatus����)��-
//SOCKET����SOCKET��Ӧ�Ŀͻ��˵�ַ�������ڸ�SOCKET��������(��Ӧ�ṹPER_IO_CONTEXT)��
typedef struct PER_SOCKET_CONTEXT {
  SOCKET m_Socket; //���ӿͻ��˵�socket
  SOCKADDR_IN m_ClientAddr; //�ͻ��˵�ַ
  PER_IO_CONTEXT** m_arrayIoContext; //�׽��ֲ�����������WSARecv��WSASend����һ��PER_IO_CONTEXT
  int m_arrayIoContext_Count;
} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

//���캯��
void PER_SOCKET_CONTEXT_init(PER_SOCKET_CONTEXT* s) {
  s->m_Socket = INVALID_SOCKET;
  memset(&s->m_ClientAddr, 0, sizeof(s->m_ClientAddr));
  s->m_arrayIoContext_Count = 0;
  s->m_arrayIoContext = NULL;
}
//��������
void PER_SOCKET_CONTEXT_free(PER_SOCKET_CONTEXT* s) {
  if (s->m_Socket != INVALID_SOCKET) {
    closesocket(s->m_Socket);
    s->m_Socket = INVALID_SOCKET;
  }
  // �ͷŵ����е�IO����������
  for (int i = 0; i < s->m_arrayIoContext_Count; i++) {
    PER_IO_CONTEXT* pContext = s->m_arrayIoContext[i];
    PER_IO_CONTEXT_free(pContext);
    free(pContext);
  }
  free(s->m_arrayIoContext);
  s->m_arrayIoContext = NULL;
  s->m_arrayIoContext_Count = 0;
}
//�����׽��ֲ���ʱ�����ô˺�������PER_IO_CONTEX�ṹ
PER_IO_CONTEXT* PER_SOCKET_CONTEXT_GetNewIoContext(PER_SOCKET_CONTEXT* s) {
  PER_IO_CONTEXT* p = NULL;
  MYREALLOC(p, 1);
  PER_IO_CONTEXT_init(p);
  ++s->m_arrayIoContext_Count;
  MYREALLOC(s->m_arrayIoContext, s->m_arrayIoContext_Count);
  s->m_arrayIoContext[s->m_arrayIoContext_Count - 1] = p;
  return p;
}
// ���������Ƴ�һ��ָ����IoContext
void PER_SOCKET_CONTEXT_RemoveContext(PER_SOCKET_CONTEXT* s, PER_IO_CONTEXT* pContext) {
  ASSERT(pContext != NULL);
  for (int i = 0; i < s->m_arrayIoContext_Count; i++) {
    if (pContext == s->m_arrayIoContext[i]) {
      PER_IO_CONTEXT_free(pContext);
      FREE(pContext);
      pContext = NULL;
      memdel(s->m_arrayIoContext, s->m_arrayIoContext_Count, i, 1, sizeof(void*));
      s->m_arrayIoContext_Count--;
      break;
    }
  }
}
// �������̵߳��̲߳���
class CIOCPModel;
typedef struct IOCPTHREADPARAMS_WORKER {
  CIOCPModel* pIOCPModel; //��ָ�룬���ڵ������еĺ���
  int nThreadNo; //�̱߳��
} IOCPTHREADPARAMS_WORKER;
// CIOCPModel��
class CIOCPModel
{
public:
  CIOCPModel(void);
  ~CIOCPModel(void);
public:
  // ����Socket��
  bool LoadSocketLib();
  // ж��Socket�⣬��������
  void UnloadSocketLib() {
    WSACleanup();
  }
  // ����������
  bool Start();
  // ֹͣ������
  void Stop(BOOL wait);
  // ��ñ�����IP��ַ
  const char* GetLocalIP();
  // ���ü����˿�
  void SetPort(const int& nPort) {
    m_nPort = nPort;
  }
  // �����������ָ�룬���ڵ�����ʾ��Ϣ��������
  void SetMainDlg(void* p) {
    m_pMain = p;
  }
  //Ͷ��WSASend�����ڷ�������
  bool PostWrite(PER_IO_CONTEXT* pAcceptIoContext);
  //Ͷ��WSARecv���ڽ�������
  bool PostRecv(PER_IO_CONTEXT* pIoContext);
protected:
  // ��ʼ��IOCP
  bool _InitializeIOCP();
  // ��ʼ��Socket
  bool _InitializeListenSocket();
  // ����ͷ���Դ
  void _DeInitialize();
  //Ͷ��AcceptEx����
  bool _PostAccept(PER_IO_CONTEXT* pAcceptIoContext);
  //���пͻ��������ʱ�򣬽��д���
  bool _DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);
  //���ӳɹ�ʱ�����ݵ�һ���Ƿ���յ����Կͻ��˵����ݽ��е���
  bool _DoFirstRecvWithData(PER_IO_CONTEXT* pIoContext);
  bool _DoFirstRecvWithoutData(PER_IO_CONTEXT* pIoContext);
  //���ݵ����������pIoContext������
  bool _DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);
  //���ͻ���socket�������Ϣ�洢��������
  void _AddToContextList(PER_SOCKET_CONTEXT* pSocketContext);
  //���ͻ���socket����Ϣ���������Ƴ�
  void _RemoveContext(PER_SOCKET_CONTEXT* pSocketContext);
  // ��տͻ���socket��Ϣ
  void _ClearContextList();
  // ������󶨵���ɶ˿���
  bool _AssociateWithIOCP(PER_SOCKET_CONTEXT* pContext);
  // ������ɶ˿��ϵĴ���
  bool HandleError(PER_SOCKET_CONTEXT* pContext, const DWORD& dwErr);
  //�̺߳�����ΪIOCP�������Ĺ������߳�
  static DWORD WINAPI _WorkerThread(LPVOID lpParam);
  //��ñ����Ĵ���������
  int _GetNoOfProcessors();
  //�жϿͻ���Socket�Ƿ��Ѿ��Ͽ�
  bool _IsSocketAlive(SOCKET s);
  //������������ʾ��Ϣ
  void _ShowMessage(const char* szFormat, ...) const;
private:
  HANDLE m_hShutdownEvent; // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�
  HANDLE m_hIOCompletionPort; // ��ɶ˿ڵľ��
  HANDLE* m_phWorkerThreads; // �������̵߳ľ��ָ��
  int m_nThreads; // ���ɵ��߳�����
  char m_strIP[64]; // �������˵�IP��ַ
  int m_nPort; // �������˵ļ����˿�
  void* m_pMain; // ������Ľ���ָ�룬����������������ʾ��Ϣ
  CRITICAL_SECTION m_csContextList; // ����Worker�߳�ͬ���Ļ�����
  PER_SOCKET_CONTEXT** m_arrayClientContext; // �ͻ���Socket��Context��Ϣ
  int m_arrayClientContext_Count;
  PER_SOCKET_CONTEXT* m_pListenContext; // ���ڼ�����Socket��Context��Ϣ
  LPFN_ACCEPTEX m_lpfnAcceptEx; // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
  LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
};
CIOCPModel::CIOCPModel(void):
  m_nThreads(0),
  m_hShutdownEvent(NULL),
  m_hIOCompletionPort(NULL),
  m_phWorkerThreads(NULL),
  m_nPort(DEFAULT_PORT),
  m_pMain(NULL),
  m_lpfnAcceptEx(NULL),
  m_arrayClientContext(NULL),
  m_arrayClientContext_Count(0),
  m_pListenContext(NULL)
{
  strcpy(m_strIP, DEFAULT_IP);
}
CIOCPModel::~CIOCPModel(void)
{
  // ȷ����Դ�����ͷ�
  this->Stop(0);
}
//////////////////////////////////////////////////////////////////////
//�������ܣ��̺߳���������GetQueuedCompletionStatus����������д���
//����������lpParam��IOCPTHREADPARAMS_WORKER����ָ�룻
//����˵����GetQueuedCompletionStatus��ȷ����ʱ��ʾĳ�����Ѿ���ɣ�
//�ڶ�������lpNumberOfBytes��ʾ�����׽��ִ�����ֽ�����
//����lpCompletionKey��lpOverlapped������Ҫ����Ϣ�����ѯMSDN�ĵ���
////////////////////////////////////////////////////////////////////*/
DWORD WINAPI CIOCPModel::_WorkerThread(LPVOID lpParam)
{
  IOCPTHREADPARAMS_WORKER* pParam = (IOCPTHREADPARAMS_WORKER*)lpParam;
  CIOCPModel* pIOCPModel = (CIOCPModel*)pParam->pIOCPModel;
  int nThreadNo = (int)pParam->nThreadNo;
  pIOCPModel->_ShowMessage("�������߳�������ID: %d.", nThreadNo);
  OVERLAPPED* pOverlapped = NULL;
  PER_SOCKET_CONTEXT* pSocketContext = NULL;
  DWORD dwBytesTransfered = 0;
  //ѭ����������ֱ�����յ�Shutdown��ϢΪֹ
  while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0)) {
    BOOL bReturn = GetQueuedCompletionStatus(
        pIOCPModel->m_hIOCompletionPort,
        &dwBytesTransfered,
        (PULONG_PTR)&pSocketContext,
        &pOverlapped,
        INFINITE);
    //����EXIT_CODE�˳���־����ֱ���˳�
    if (EXIT_CODE == (DWORD)pSocketContext) {
      break;
    }
    //����ֵΪ0����ʾ����
    if (!bReturn) {
      DWORD dwErr = GetLastError();
      // ��ʾһ����ʾ��Ϣ
      if (!pIOCPModel->HandleError(pSocketContext, dwErr)) {
        break;
      }
      continue;
    }
    else {
      // ��ȡ����Ĳ���
      PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);
      // �ж��Ƿ��пͻ��˶Ͽ���
      if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType || SEND_POSTED == pIoContext->m_OpType)) {
        pIOCPModel->_ShowMessage(_T("�ͻ��� %s:%d �Ͽ�����."), inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));
        // �ͷŵ���Ӧ����Դ
        pIOCPModel->_RemoveContext(pSocketContext);
        continue;
      }
      else {
        switch (pIoContext->m_OpType) {
          // Accept
        case ACCEPT_POSTED: {
          pIoContext->m_nTotalBytes = dwBytesTransfered;
          pIOCPModel->_DoAccpet(pSocketContext, pIoContext);
        }
        break;
        // RECV
        case RECV_POSTED: {
          pIoContext->m_nTotalBytes = dwBytesTransfered;
          pIOCPModel->_DoRecv(pSocketContext, pIoContext);
        }
        break;
        case SEND_POSTED: {
          pIoContext->m_nSendBytes += dwBytesTransfered;
          if (pIoContext->m_nSendBytes < pIoContext->m_nTotalBytes) {
            //����δ�ܷ����꣬������������
            pIoContext->m_wsaBuf.buf = pIoContext->m_szBuffer + pIoContext->m_nSendBytes;
            pIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes - pIoContext->m_nSendBytes;
            pIOCPModel->PostWrite(pIoContext);
          }
          else {
            pIOCPModel->PostRecv(pIoContext);
          }
        }
        break;
        default:
          // ��Ӧ��ִ�е�����
          TRACE(_T("_WorkThread�е� pIoContext->m_OpType �����쳣.\n"));
          break;
        } //switch
      }//if
    }//if
  }//while
  TRACE(_T("�������߳� %d ���˳�.\n"), nThreadNo);
  // �ͷ��̲߳���
  RELEASE(lpParam);
  return 0;
}
//�������ܣ���ʼ���׽���
bool CIOCPModel::LoadSocketLib()
{
  WSADATA wsaData;
  int nResult;
  nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  // ����(һ�㶼�����ܳ���)
  if (NO_ERROR != nResult) {
    this->_ShowMessage(_T("��ʼ��WinSock 2.2ʧ�ܣ�\n"));
    return false;
  }
  return true;
}
//�������ܣ�����������
bool CIOCPModel::Start()
{
  // ��ʼ���̻߳�����
  InitializeCriticalSection(&m_csContextList);
  // ����ϵͳ�˳����¼�֪ͨ
  m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  // ��ʼ��IOCP
  if (false == _InitializeIOCP()) {
    this->_ShowMessage(_T("��ʼ��IOCPʧ�ܣ�\n"));
    return false;
  }
  else {
    this->_ShowMessage("\nIOCP��ʼ�����\n.");
  }
  // ��ʼ��Socket
  if (false == _InitializeListenSocket()) {
    this->_ShowMessage(_T("Listen Socket��ʼ��ʧ�ܣ�\n"));
    this->_DeInitialize();
    return false;
  }
  else {
    this->_ShowMessage("Listen Socket��ʼ�����.");
  }
  this->_ShowMessage(_T("ϵͳ׼���������Ⱥ�����....\n"));
  return true;
}
//
// ��ʼ����ϵͳ�˳���Ϣ���˳���ɶ˿ں��߳���Դ
void CIOCPModel::Stop(BOOL wait)
{
  if (wait) {
    WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);
  }
  if (m_pListenContext != NULL && m_pListenContext->m_Socket != INVALID_SOCKET) {
    // ����ر���Ϣ֪ͨ
    SetEvent(m_hShutdownEvent);
    for (int i = 0; i < m_nThreads; i++) {
      // ֪ͨ���е���ɶ˿ڲ����˳�
      PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
    }
    // �ȴ����еĿͻ�����Դ�˳�
    WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);
    // ����ͻ����б���Ϣ
    this->_ClearContextList();
    // �ͷ�������Դ
    this->_DeInitialize();
    this->_ShowMessage("ֹͣ����\n");
  }
}
//////////////////////////////////////////////////////////////
//�������ܣ�Ͷ��WSARecv����
//����������
//PER_IO_CONTEXT* pIoContext: ���ڽ���IO���׽����ϵĽṹ����ҪΪWSARecv������WSASend������
///////////////////////////////////////////////////////////////
bool CIOCPModel::PostRecv(PER_IO_CONTEXT* pIoContext)
{
  // ��ʼ������
  DWORD dwFlags = 0;
  DWORD dwBytes = 0;
  WSABUF* p_wbuf = &pIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pIoContext->m_Overlapped;
  PER_IO_CONTEXT_ResetBuffer(pIoContext);
  pIoContext->m_OpType = RECV_POSTED;
  pIoContext->m_nSendBytes = 0;
  pIoContext->m_nTotalBytes = 0;
  // ��ʼ����ɺ󣬣�Ͷ��WSARecv����
  int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);
  // �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
  if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError())) {
    this->_ShowMessage("Ͷ�ݵ�һ��WSARecvʧ�ܣ�");
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////
//�������ܣ�Ͷ��WSASend����
//����������
//PER_IO_CONTEXT* pIoContext: ���ڽ���IO���׽����ϵĽṹ����ҪΪWSARecv������WSASend����
//����˵��������PostWrite֮ǰ��Ҫ����pIoContext��m_wsaBuf, m_nTotalBytes, m_nSendBytes��
///////////////////////////////////////////////////////////////
bool CIOCPModel::PostWrite(PER_IO_CONTEXT* pIoContext)
{
  // ��ʼ������
  DWORD dwFlags = 0;
  DWORD dwSendNumBytes = 0;
  WSABUF* p_wbuf = &pIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pIoContext->m_Overlapped;
  pIoContext->m_OpType = SEND_POSTED;
  //Ͷ��WSASend���� -- ��Ҫ�޸�
  int nRet = WSASend(pIoContext->m_sockAccept, &pIoContext->m_wsaBuf, 1, &dwSendNumBytes, dwFlags,
      &pIoContext->m_Overlapped, NULL);
  // �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
  if ((SOCKET_ERROR == nRet) && (WSA_IO_PENDING != WSAGetLastError())) {
    this->_ShowMessage("Ͷ��WSASendʧ�ܣ�");
    return false;
  }
  return true;
}
////////////////////////////////
// ��ʼ����ɶ˿�
bool CIOCPModel::_InitializeIOCP()
{
  // ������һ����ɶ˿�
  m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
  if (NULL == m_hIOCompletionPort) {
    this->_ShowMessage(_T("������ɶ˿�ʧ�ܣ��������: %d!\n"), WSAGetLastError());
    return false;
  }
  // ���ݱ����еĴ�����������������Ӧ���߳���
  m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();
  // Ϊ�������̳߳�ʼ�����
  MYREALLOC(m_phWorkerThreads, m_nThreads);
  // ���ݼ�����������������������߳�
  DWORD nThreadID;
  for (int i = 0; i < m_nThreads; i++) {
    IOCPTHREADPARAMS_WORKER* pThreadParams = NULL;
    MYREALLOC(pThreadParams, 1);
    pThreadParams->pIOCPModel = this;
    pThreadParams->nThreadNo = i + 1;
    m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void*)pThreadParams, 0, &nThreadID);
  }
  TRACE(" ���� _WorkerThread %d ��.\n", m_nThreads);
  return true;
}
/////////////////////////////////////////////////////////////////
// ��ʼ��Socket
bool CIOCPModel::_InitializeListenSocket()
{
  // AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
  GUID GuidAcceptEx = WSAID_ACCEPTEX;
  GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
  // ��������ַ?�Ϣ�����ڰ�Socket
  struct sockaddr_in ServerAddress;
  // �������ڼ�����Socket����Ϣ
  MYREALLOC(m_pListenContext, 1);
  PER_SOCKET_CONTEXT_init(m_pListenContext);
  // ��Ҫʹ���ص�IO�������ʹ��WSASocket������Socket���ſ���֧���ص�IO����
  m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == m_pListenContext->m_Socket) {
    this->_ShowMessage("��ʼ��Socketʧ�ܣ��������: %d.\n", WSAGetLastError());
    return false;
  }
  else {
    TRACE("WSASocket() ���.\n");
  }
  // ��Listen Socket������ɶ˿���
  if (NULL == CreateIoCompletionPort((HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort, (DWORD)m_pListenContext, 0)) {
    this->_ShowMessage("�� Listen Socket����ɶ˿�ʧ�ܣ��������: %d/n", WSAGetLastError());
    RELEASE_SOCKET(m_pListenContext->m_Socket);
    return false;
  }
  else {
    TRACE("Listen Socket����ɶ˿� ���.\n");
  }
  // ����ַ��Ϣ
  ZeroMemory((char*)&ServerAddress, sizeof(ServerAddress));
  ServerAddress.sin_family = AF_INET;
  // �����?԰��κο��õ�IP��ַ�����߰�һ��ָ����IP��ַ
  ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  //ServerAddress.sin_addr.s_addr = inet_addr(m_strIP);
  ServerAddress.sin_port = htons(m_nPort);
  // �󶨵�ַ�Ͷ˿�
  if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr*) &ServerAddress, sizeof(ServerAddress))) {
    this->_ShowMessage("bind()����ִ�д���.\n");
    return false;
  }
  else {
    TRACE("bind() ���.\n");
  }
  // ��ʼ���м���
  if (SOCKET_ERROR == listen(m_pListenContext->m_Socket, SOMAXCONN)) {
    this->_ShowMessage("Listen()����ִ�г��ִ���.\n");
    return false;
  }
  else {
    TRACE("Listen() ���.\n");
  }
  // ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
  // ������Ҫ�����ȡһ�º�����ָ�룬
  // ��ȡAcceptEx����ָ��
  DWORD dwBytes = 0;
  if (SOCKET_ERROR == WSAIoctl(m_pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
      &GuidAcceptEx, sizeof(GuidAcceptEx), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, NULL, NULL)) {
    this->_ShowMessage("WSAIoctl δ�ܻ�ȡAcceptEx����ָ�롣�������: %d\n", WSAGetLastError());
    this->_DeInitialize();
    return false;
  }
  // ��ȡGetAcceptExSockAddrs����ָ�룬Ҳ��ͬ��
  if (SOCKET_ERROR == WSAIoctl(m_pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
      sizeof(GuidGetAcceptExSockAddrs), &m_lpfnGetAcceptExSockAddrs, sizeof(m_lpfnGetAcceptExSockAddrs), &dwBytes, NULL, NULL)) {
    this->_ShowMessage("WSAIoctl δ�ܻ�ȡGuidGetAcceptExSockAddrs����ָ�롣�������: %d\n", WSAGetLastError());
    this->_DeInitialize();
    return false;
  }
  // ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
  //����10���׽��֣�Ͷ��AcceptEx���󣬼�����10���׽��ֽ���accept������
  for (int i = 0; i < MAX_POST_ACCEPT; i++) {
    // �½�һ��IO_CONTEXT
    PER_IO_CONTEXT* pAcceptIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(m_pListenContext);
    if (false == this->_PostAccept(pAcceptIoContext)) {
      PER_SOCKET_CONTEXT_RemoveContext(m_pListenContext, pAcceptIoContext);
      return false;
    }
  }
  this->_ShowMessage(_T("Ͷ�� %d ��AcceptEx�������"), MAX_POST_ACCEPT);
  return true;
}
////////////////////////////////////////////////////////////
// ����ͷŵ�������Դ
void CIOCPModel::_DeInitialize()
{
  // ɾ���ͻ����б�Ļ�����
  DeleteCriticalSection(&m_csContextList);
  // �ر�ϵͳ�˳��¼����
  RELEASE_HANDLE(m_hShutdownEvent);
  // �ͷŹ������߳̾��ָ��
  for (int i = 0; i < m_nThreads; i++) {
    RELEASE_HANDLE(m_phWorkerThreads[i]);
  }
  RELEASE(m_phWorkerThreads);
  // �ر�IOCP���
  RELEASE_HANDLE(m_hIOCompletionPort);
  // �رռ���Socket
  RELEASE(m_pListenContext);
  this->_ShowMessage("�ͷ���Դ���.\n");
}
//
// Ͷ����ɶ˿�����
//
#pragma comment( lib, "mswsock.lib")
char* GetErrorMsg(int dw)
{
  // Retrieve the system error message for the last-error code
  static char buf[256];
  LPTSTR lpMsgBuf = NULL;
  if (0 == dw) {
    dw = GetLastError();
  }
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
  _snprintf(buf, 256, "err: %d %s", dw, lpMsgBuf);
  return buf;
}
// Ͷ��Accept����
bool CIOCPModel::_PostAccept(PER_IO_CONTEXT* pAcceptIoContext)
{
  ASSERT(INVALID_SOCKET != m_pListenContext->m_Socket);
  // ׼������
  DWORD dwBytes = 0;
  pAcceptIoContext->m_OpType = ACCEPT_POSTED;
  WSABUF* p_wbuf = &pAcceptIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pAcceptIoContext->m_Overlapped;
  // Ϊ�Ժ�������Ŀͻ�����׼����Socket( ������봫ͳaccept�������� )
  pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept) {
    _ShowMessage("��������Accept��Socketʧ�ܣ��������: %d", WSAGetLastError());
    return false;
  }
#define M_ACCEPTEX m_lpfnAcceptEx
  //#define M_ACCEPTEX AcceptEx
  // Ͷ��AcceptEx
  if (FALSE == M_ACCEPTEX(m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
      sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, p_ol)) {
    if (WSA_IO_PENDING != WSAGetLastError()) {
      int err = WSAGetLastError();
      _ShowMessage("Ͷ�� AcceptEx ����ʧ�ܣ��������: %d %s", err, GetErrorMsg(err));
      printf("%s", strerror(err));
      return false;
    }
  }
  return true;
}
////////////////////////////////////////////////////////////////////*
//�������ܣ��������пͻ��˽��봦��
//����˵����
//PER_SOCKET_CONTEXT* pSocketContext: ����accept������Ӧ���׽��֣����׽�������Ӧ�����ݽṹ��
//PER_IO_CONTEXT* pIoContext: ����accept������Ӧ�����ݽṹ��
//DWORD dwIOSize: ���β�������ʵ�ʴ����?ֽ��?
/////////////////////////////////////////////////////////////////////
bool CIOCPModel::_DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
  if (pIoContext->m_nTotalBytes > 0) {
    //�ͻ�����ʱ����һ�ν���dwIOSize�ֽ�����
    _DoFirstRecvWithData(pIoContext);
  }
  else {
    //�ͻ��˽���ʱ��û�з������ݣ���Ͷ��WSARecv���󣬽�������
    _DoFirstRecvWithoutData(pIoContext);
  }
  // 5. ʹ�����֮�󣬰�Listen Socket���Ǹ�IoContext���ã�Ȼ��׼��Ͷ���µ�AcceptEx
  PER_IO_CONTEXT_ResetBuffer(pIoContext);
  return this->_PostAccept(pIoContext);
}
/////////////////////////////////////////////////////////////////
//�������ܣ����н��յ����ݵ����ʱ�򣬽��д���
bool CIOCPModel::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
  //������յ�����
  SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;
  this->_ShowMessage(_T("�յ� %s:%d ��Ϣ��%s"), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);
  //��������
  pIoContext->m_nSendBytes = 0;
  pIoContext->m_nTotalBytes = pIoContext->m_nTotalBytes;
  pIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes;
  pIoContext->m_wsaBuf.buf = pIoContext->m_szBuffer;
  return PostWrite(pIoContext);
}
//////////////////////////////////////////////////////////////
//�������ܣ�AcceptEx���տͻ����ӳɹ������տͻ���һ�η��͵����ݣ���Ͷ��WSASend����
//����������
//PER_IO_CONTEXT* pIoContext: ���ڼ����׽����ϵĲ���
///////////////////////////////////////////////////////////////
bool CIOCPModel::_DoFirstRecvWithData(PER_IO_CONTEXT* pIoContext)
{
  SOCKADDR_IN* ClientAddr = NULL;
  SOCKADDR_IN* LocalAddr = NULL;
  int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);
  //1. ����ȡ������ͻ��˵ĵ�ַ��Ϣ
  this->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
      sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);
  //��ʾ�ͻ�����Ϣ
  this->_ShowMessage(_T("�ͻ��� %s:%d ��Ϣ��%s."), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);
  //2.Ϊ�½�����׽Ӵ���PER_SOCKET_CONTEXT���������׽��ְ󶨵���ɶ˿�
  PER_SOCKET_CONTEXT* pNewSocketContext = NULL;
  MYREALLOC(pNewSocketContext, 1);
  PER_SOCKET_CONTEXT_init(pNewSocketContext);
  pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
  memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));
  // ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
  if (false == this->_AssociateWithIOCP(pNewSocketContext)) {
    PER_SOCKET_CONTEXT_free(pNewSocketContext);
    RELEASE(pNewSocketContext);
    return false;
  }
  //3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
  PER_IO_CONTEXT* pNewIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(pNewSocketContext);
  pNewIoContext->m_OpType = SEND_POSTED;
  pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;
  pNewIoContext->m_nTotalBytes = pIoContext->m_nTotalBytes;
  pNewIoContext->m_nSendBytes = 0;
  pNewIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes;
  memcpy(pNewIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.buf, pIoContext->m_nTotalBytes); //�������ݵ�WSASend�����Ĳ���������
  //��ʱ�ǵ�һ�ν������ݳɹ�����������Ͷ��PostWrite����ͻ��˷�������
  if (false == this->PostWrite(pNewIoContext)) {
    PER_SOCKET_CONTEXT_RemoveContext(pNewSocketContext, pNewIoContext);
    return false;
  }
  //4. ���Ͷ�ݳɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
  this->_AddToContextList(pNewSocketContext);
  return true;
}
//////////////////////////////////////////////////////////////
//�������ܣ�AcceptEx���տͻ����ӳɹ�����ʱ��δ���յ����ݣ���Ͷ��WSARecv����
//����������
//PER_IO_CONTEXT* pIoContext: ���ڼ����׽����ϵĲ���
///////////////////////////////////////////////////////////////
bool CIOCPModel::_DoFirstRecvWithoutData(PER_IO_CONTEXT* pIoContext)
{
  //Ϊ�½�����׽��ִ���PER_SOCKET_CONTEXT�ṹ�����󶨵���ɶ˿�
  PER_SOCKET_CONTEXT* pNewSocketContext = NULL;
  SOCKADDR_IN ClientAddr;
  int Len = sizeof(ClientAddr);
  MYREALLOC(pNewSocketContext, 1);
  PER_SOCKET_CONTEXT_init(pNewSocketContext);
  getpeername(pIoContext->m_sockAccept, (sockaddr*)&ClientAddr, &Len);
  pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
  memcpy(&(pNewSocketContext->m_ClientAddr), &ClientAddr, sizeof(SOCKADDR_IN));
  //�����׽��ְ󶨵���ɶ˿�
  if (false == this->_AssociateWithIOCP(pNewSocketContext)) {
    PER_SOCKET_CONTEXT_free(pNewSocketContext);
    RELEASE(pNewSocketContext);
    return false;
  }
  //Ͷ��WSARecv���󣬽�������
  PER_IO_CONTEXT* pNewIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(pNewSocketContext);
  //��ʱ��AcceptExδ���յ��ͻ��˵�һ�η��͵����ݣ������������PostRecv���������Կͻ��˵�����
  if (false == this->PostRecv(pNewIoContext)) {
    PER_SOCKET_CONTEXT_RemoveContext(pNewSocketContext, pNewIoContext);
    return false;
  }
  //���Ͷ�ݳɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
  this->_AddToContextList(pNewSocketContext);
  return true;
}
/////////////////////////////////////////////////////
// �����(Socket)�󶨵���ɶ˿���
bool CIOCPModel::_AssociateWithIOCP(PER_SOCKET_CONTEXT* pContext)
{
  // �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
  HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);
  if (NULL == hTemp) {
    this->_ShowMessage(("ִ��CreateIoCompletionPort()���ִ���.������룺%d"), GetLastError());
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////
// ���ͻ��˵������Ϣ�洢��������
void CIOCPModel::_AddToContextList(PER_SOCKET_CONTEXT* pHandleData)
{
  EnterCriticalSection(&m_csContextList);
  MEMPUSH(m_arrayClientContext, m_arrayClientContext_Count);
  m_arrayClientContext[m_arrayClientContext_Count - 1] = pHandleData;
  LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
// �Ƴ�ĳ���ض���Context
void CIOCPModel::_RemoveContext(PER_SOCKET_CONTEXT* pSocketContext)
{
  EnterCriticalSection(&m_csContextList);
  for (int i = 0; i < m_arrayClientContext_Count; i++) {
    if (pSocketContext == m_arrayClientContext[i]) {
      PER_SOCKET_CONTEXT_free(pSocketContext);
      RELEASE(pSocketContext);
      MEMDEL(m_arrayClientContext, m_arrayClientContext_Count, i, 1);
      m_arrayClientContext_Count--;
      break;
    }
  }
  LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
// ��տͻ�����Ϣ
void CIOCPModel::_ClearContextList()
{
  EnterCriticalSection(&m_csContextList);
  for (int i = 0; i < m_arrayClientContext_Count; i++) {
    PER_SOCKET_CONTEXT* pContext = m_arrayClientContext[i];
    PER_SOCKET_CONTEXT_free(pContext);
    free(pContext);
  }
  FREE(m_arrayClientContext);
  m_arrayClientContext = NULL;
  m_arrayClientContext_Count = 0;
  LeaveCriticalSection(&m_csContextList);
}
//
// ��ñ�����IP��ַ
const char* CIOCPModel::GetLocalIP()
{
  // ��ñ���������
  char hostname[MAX_PATH] = {0};
  gethostname(hostname, MAX_PATH);
  struct hostent FAR* lpHostEnt = gethostbyname(hostname);
  if (lpHostEnt == NULL) {
    return DEFAULT_IP;
  }
  // ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP(��Ϊһ̨�������ܻ�󶨶��IP)
  LPSTR lpAddr = lpHostEnt->h_addr_list[0];
  // ��IP��ַת�����ַ�����ʽ
  struct in_addr inAddr;
  memmove(&inAddr, lpAddr, 4);
  strcpy(m_strIP, inet_ntoa(inAddr));
  return m_strIP;
}
// ��ñ����д�����������
int CIOCPModel::_GetNoOfProcessors()
{
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwNumberOfProcessors;
}
///
// ������������ʾ��ʾ��Ϣ
void CIOCPModel::_ShowMessage(const char* szFormat, ...) const
{
  // ���ݴ���Ĳ�����ʽ���ַ���
  char strMessage[256];
  va_list arglist;
  // ����䳤����
  va_start(arglist, szFormat);
  _vsnprintf(strMessage, 256, szFormat, arglist);
  va_end(arglist);
  // ������������ʾ
  TRACE(strMessage);
  TRACE("\n");
}
///
// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ���������һ����Ч��Socket��Ͷ��WSARecv����������쳣
// ʹ�õķ����ǳ��������socket�������ݣ��ж����socket���õķ���ֵ
// ��Ϊ����ͻ��������쳣�Ͽ�(����ͻ��˱������߰ε����ߵ�)��ʱ�򣬷����������޷��յ��ͻ��˶Ͽ���֪ͨ��
bool CIOCPModel::_IsSocketAlive(SOCKET s)
{
  int nByteSent = send(s, "", 0, 0);
  if (-1 == nByteSent) {
    return false;
  }
  return true;
}
//�������ܣ���ʾ��������ɶ˿��ϵĴ���
bool CIOCPModel::HandleError(PER_SOCKET_CONTEXT* pContext, const DWORD& dwErr)
{
  // ����ǳ�ʱ�ˣ����ټ����Ȱ�
  if (WAIT_TIMEOUT == dwErr) {
    // ȷ�Ͽͻ����Ƿ񻹻���...
    if (!_IsSocketAlive(pContext->m_Socket)) {
      this->_ShowMessage(_T("��⵽�ͻ����쳣�˳���"));
      this->_RemoveContext(pContext);
      return true;
    }
    else {
      this->_ShowMessage(_T("���������ʱ��������..."));
      return true;
    }
  }
  // �����ǿͻ����쳣�˳���
  else if (ERROR_NETNAME_DELETED == dwErr) {
    this->_ShowMessage(_T("��⵽�ͻ����쳣�˳���"));
    this->_RemoveContext(pContext);
    return true;
  }
  else {
    this->_ShowMessage(_T("��ɶ˿ڲ������ִ����߳��˳���������룺%d"), dwErr);
    return false;
  }
}

