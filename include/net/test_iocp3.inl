
// ����deprecation����
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <stdio.h>
#include <MSWSock.h>
#include "cstd.h"
//#include "auto_tchar.h"
#pragma comment(lib,"ws2_32.lib")
#define _T(x)  (x)
#define TRACE printf

// ÿһ���������ϲ������ٸ��߳�
#define WORKER_THREADS_PER_PROCESSOR 2
// ͬʱͶ�ݵ�AcceptEx���������
#define MAX_POST_ACCEPT 10
// ���ݸ�Worker�̵߳��˳��ź�
#define EXIT_CODE NULL
// �ͷ�ָ��;����Դ�ĺ�
// �ͷ�ָ���
#define RELEASE(x) {if(x != NULL ){free(x);x=NULL;}}
// �ͷž����
#define RELEASE_HANDLE(x) {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// �ͷ�Socket��
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}
//���������� (1024*8)
#define MAX_BUFFER_LEN    (8*1024)
//Ĭ�϶˿�
#define DEFAULT_PORT 12345
//Ĭ��IP��ַ
#define DEFAULT_IP ("127.0.0.1")

#define DEFAULT_THREADS       100                        // Ĭ�ϲ����߳���
#define DEFAULT_MESSAGE       _T("Hello!")   // Ĭ�ϵķ�����Ϣ

class CClient;

// ���ڷ������ݵ��̲߳���
typedef struct _tagThreadParams_WORKER
{
  CClient* pClient;                               // ��ָ�룬���ڵ������еĺ���
  SOCKET   sock;                                  // ÿ���߳�ʹ�õ�Socket
  int      nThreadNo;                             // �̱߳��
  char     szBuffer[MAX_BUFFER_LEN];
  char	 szBuffer1[MAX_BUFFER_LEN];
  
} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;  

// ����Socket���ӵ��߳�
typedef struct _tagThreadParams_CONNECTION
{
  CClient* pClient;                               // ��ָ�룬���ڵ������еĺ���
  
} THREADPARAMS_CONNECTION,*PTHREADPARAM_CONNECTION; 


class CClient
{
public:
  CClient(void);
  ~CClient(void);
  
public:
  
  int Wait() {
    WaitForSingleObject(m_hConnectionThread, -1);
    if (m_phWorkerThreads) {
      WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, 1000000);
    }
    return 0;
  }
  // ����Socket��
  bool LoadSocketLib();
  // ж��Socket�⣬��������
  void UnloadSocketLib() { WSACleanup(); }
  
  // ��ʼ����
  bool Start();
  //	ֹͣ����
  void Stop();
  
  // ��ñ�����IP��ַ
  const char* GetLocalIP();
  
  // ��������IP��ַ
  void SetIP( const char* strIP ) { strcpy(m_strServerIP, strIP); }
  // ���ü����˿�
  void SetPort( const int nPort )   { m_nPort=nPort; }
  // ���ò����߳�����
  void SetThreads(const int& n)      { m_nThreads=n; }
  // ����Ҫ�����͵���Ϣ
  void SetMessage( const char* strMessage ) { strcpy(m_strMessage, strMessage); }
  
  // �����������ָ�룬���ڵ����亯��
  //void SetMainDlg( CDialog* p )      { m_pMain=p; }
  
  // ������������ʾ��Ϣ
  void ShowMessage( const char* strInfo,...);
  
private:
  
  // ��������
  bool EstablishConnections();
  // ���������������
  bool ConnetToServer( SOCKET *pSocket, const char* strServer, int nPort );
  // ���ڽ������ӵ��߳�
  static DWORD WINAPI _ConnectionThread(LPVOID lpParam);
  // ���ڷ�����Ϣ���߳�
  static DWORD WINAPI _WorkerThread(LPVOID lpParam);
  
  // �ͷ���Դ
  void CleanUp();
  
private:
  
  //CDialog*  m_pMain;                                      // ����ָ��
  
  char   m_strServerIP[64];                                // �������˵�IP��ַ
  char   m_strLocalIP[64];                                 // ����IP��ַ
  char   m_strMessage[64];                                 // ��������������Ϣ
  int       m_nPort;                                      // �����˿�
  int       m_nThreads;                                   // �����߳�����
  
  HANDLE    *m_phWorkerThreads;
  HANDLE    m_hConnectionThread;                          // �������ӵ��߳̾��
  HANDLE    m_hShutdownEvent;                             // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�
  
  THREADPARAMS_WORKER      *m_pParamsWorker;              // �̲߳���
};

CClient::CClient(void):
			m_nThreads(DEFAULT_THREADS),
			m_nPort(DEFAULT_PORT),
			m_phWorkerThreads(NULL),
      m_hConnectionThread(NULL),
      m_pParamsWorker(NULL),
			m_hShutdownEvent(NULL)
      {
        strcpy(m_strServerIP, DEFAULT_IP);
        strcpy(m_strLocalIP, DEFAULT_IP);
			strcpy(m_strMessage, DEFAULT_MESSAGE);
}

CClient::~CClient(void)
{
	this->Stop();
}
//////////////////////////////////////////////////////////////////////////////////
//	�������ӵ��߳�
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_CONNECTION* pParams = (THREADPARAMS_CONNECTION*) lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	TRACE("_AccpetThread������ϵͳ������...\n");

	pClient->EstablishConnections();

	TRACE(_T("_ConnectionThread�߳̽���.\n"));

	RELEASE(pParams);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ���ڷ�����Ϣ���߳�
DWORD WINAPI CClient::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER *pParams = (THREADPARAMS_WORKER *)lpParam;
	CClient* pClient = (CClient*) pParams->pClient;

	char szTemp[MAX_BUFFER_LEN];
	memset( szTemp,0,sizeof(szTemp) );
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv,0,MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	// �������������Ϣ
	sprintf( szTemp,("��1����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷���1����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	pClient->ShowMessage("SEND1 �������������Ϣ�ɹ�: %s", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("���󣺽���1����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV1 ��%d���̣߳����յ�1����Ϣ��\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);
	Sleep( 3000 );

	// �ٷ���һ����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("��2����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷��͵�2����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	
	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	pClient->ShowMessage("SEND2 �������������Ϣ�ɹ�: %s", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("���󣺽���2����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV2 ��%d���̣߳����յ�2����Ϣ��\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);
	Sleep( 3000 );
	
	// ����3����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("��3����Ϣ��%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("���󣺷��͵�3����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	pClient->ShowMessage("SEND3 �������������Ϣ�ɹ�: %s", szTemp);
	TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("���󣺽���3����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV3 ��%d���̣߳����յ�3����Ϣ��\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);

	if( pParams->nThreadNo==pClient->m_nThreads )
	{
		pClient->ShowMessage(_T("���Բ��� %d ���߳����."),pClient->m_nThreads);
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
// ��������
bool  CClient::EstablishConnections()
{
	DWORD nThreadID = 0;

	MYREALLOC(m_phWorkerThreads, m_nThreads);
	MYREALLOC(m_pParamsWorker, m_nThreads);

	// �����û����õ��߳�����������ÿһ���߳����������������������̷߳�������
	for (int i=0; i<m_nThreads; i++)
	{
		// �����û���ֹͣ�¼�
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			TRACE(_T("���յ��û�ֹͣ����.\n"));
			return true;
		}
		
		// ���������������
		if( !this->ConnetToServer(&m_pParamsWorker[i].sock,m_strServerIP,m_nPort) )
		{
			ShowMessage(_T("���ӷ�����ʧ�ܣ�"));
			CleanUp();
			return false;
		}

		m_pParamsWorker[i].nThreadNo = i+1;
		_snprintf(m_pParamsWorker[i].szBuffer, MAX_BUFFER_LEN, "%d���߳� �������� %s", i+1, m_strMessage );

		Sleep(10);

		// ������ӷ������ɹ����Ϳ�ʼ�����������̣߳������������ָ������
		m_pParamsWorker[i].pClient = this;
		m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//	�����������Socket����
bool CClient::ConnetToServer( SOCKET *pSocket, const char* strServer, int nPort )
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// ����SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		TRACE("���󣺳�ʼ��Socketʧ�ܣ�������Ϣ��%d\n", WSAGetLastError());
		return false;
	}

	// ���ɵ�ַ��Ϣ
	Server = gethostbyname(strServer);
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		TRACE("������Ч�ķ�������ַ.\n");
		return false; 
	}
	
	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, (char *)Server->h_addr, Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	// ��ʼ���ӷ�����
	if (SOCKET_ERROR == connect(*pSocket, (struct sockaddr *)(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		TRACE("����������������ʧ�ܣ�\n");
		return false; 
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
bool CClient::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		ShowMessage(_T("��ʼ��WinSock 2.2ʧ�ܣ�\n"));
		return false; // ����
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʼ����
bool CClient::Start()
{
	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// ���������߳�
	DWORD nThreadID;
	THREADPARAMS_CONNECTION* pThreadParams = NULL;
  MYREALLOC(pThreadParams, 1);
	pThreadParams->pClient = this;
	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

///////////////////////////////////////////////////////////////////////
//	ֹͣ����
void CClient::Stop()
{
	if( m_hShutdownEvent==NULL ) return ;

	SetEvent(m_hShutdownEvent);
	// �ȴ�Connection�߳��˳�
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	// �ر����е�Socket
	for (int i= 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}

	// �ȴ����еĹ������߳��˳�
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	// �����Դ
	CleanUp();

	TRACE("����ֹͣ.\n");
}

//////////////////////////////////////////////////////////////////////
//	�����Դ
void CClient::CleanUp()
{
	if(m_hShutdownEvent==NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
const char* CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // ��ñ���������

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	strcpy(m_strLocalIP, inet_ntoa(inAddr) );   // ת���ɱ�׼��IP��ַ��ʽ
	return m_strLocalIP;
}

/////////////////////////////////////////////////////////////////////
// ������������ʾ��Ϣ
void CClient::ShowMessage(const char* strInfo,...)
{
	// ���ݴ���Ĳ�����ʽ���ַ���
	char   strMessage[256];
	va_list   arglist;

	va_start(arglist, strInfo);
	_snprintf(strMessage, 256, strInfo,arglist);
	va_end(arglist);

	// ������������ʾ
}

#include "IOCPModel.inl"

int test_iocp3_server() {
  CIOCPModel m_IOCP;
  // ��ʼ��Socket��
  if( false==m_IOCP.LoadSocketLib() )
  {
    printf(_T("����Winsock 2.2ʧ�ܣ����������޷����У�"));
    return 0;
	}
  if( false==m_IOCP.Start() )
  {
    printf(_T("����������ʧ�ܣ�"));
    return 0;	
  }
  m_IOCP.Stop(1);
  return 0;
}
int test_iocp3_client() {
  CClient m_Client;
  int nPort = DEFAULT_PORT;
  int nThreads = 10000;
  char* strIP = "192.168.1.3";
  const char* strMessage = "1111111111111111111111111111";
  printf("strIP = %s\n",strIP);
  printf("strMessage = %s\n",strMessage);
  
  if( false==m_Client.LoadSocketLib() )
  {
    printf(_T("����Winsock 2.2ʧ�ܣ����������޷����У�"));
    return 0;
	}

  // ��CClient���ò���
  m_Client.SetIP(strIP);
  m_Client.SetPort(nPort);
  m_Client.SetThreads(nThreads);
  m_Client.SetMessage(strMessage);
  
  // ��ʼ
  if( !m_Client.Start() )
  {
    printf(_T("����ʧ�ܣ�\n"));
    return 0;
  }
  
	printf(_T("���Կ�ʼ\n"));
  m_Client.Wait();
  return 0;
}
