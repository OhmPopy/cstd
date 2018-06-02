//��������
#include <stdio.h>
#include <WinSock2.h>
#pragma comment( lib, "mswsock.lib")
#pragma comment( lib, "ws2_32.lib" )
#define BUFFER_SIZE 1024
#define OP_READ 18
#define OP_WRITE 28
#define OP_ACCEPT 38
#define CHECK_CODE 0x010110
BOOL bStopThread = false;
typedef struct _PER_HANDLE_DATA {
  SOCKET s;
  sockaddr_in addr; // �ͻ��˵�ַ
  char buf[BUFFER_SIZE];
  int nOperationType;
} PER_HANDLE_DATA, *PPER_HANDLE_DATA;
#pragma pack(1)
typedef struct MsgAsk {
  int iCode;
  int iBodySize;
  char szBuffer[32];
} MSG_ASK, *PMSG_ASK;
typedef struct MsgBody {
  int iBodySize;
  int iOpType;
  char szBuffer[64];
} MSG_BODY, *PMSG_BODY;
typedef struct MsgAck {
  int iCheckCode;
  char szBuffer[32];
} MSG_ACK, *PMSG_ACK;
#pragma pack()
DWORD WINAPI ServerThread(LPVOID lpParam)
{
  // �õ���ɶ˿ھ��
  HANDLE hCompletion = (HANDLE)lpParam;
  DWORD dwTrans;
  PPER_HANDLE_DATA pPerHandle;
  OVERLAPPED* pOverLapped;
  while (!bStopThread) {
    // �ڹ���������ɶ˿ڵ������׽����ϵȴ�I/O���
    BOOL bOK = ::GetQueuedCompletionStatus(hCompletion,
        &dwTrans, (PULONG_PTR)&pPerHandle, &pOverLapped, WSA_INFINITE);
    if (!bOK) {
      ::closesocket(pPerHandle->s);
      ::GlobalFree(pPerHandle);
      ::GlobalFree(pOverLapped);
      continue;
    }
    switch (pPerHandle->nOperationType) {
    case OP_READ: {
      MSG_ASK msgAsk = {0};
      memcpy(&msgAsk, pPerHandle->buf, sizeof(msgAsk));
      if (msgAsk.iCode != CHECK_CODE
          || msgAsk.iBodySize != sizeof(msgAsk)) {
        printf("error\n");
      }
      else {
        msgAsk.szBuffer[strlen(msgAsk.szBuffer) + 1] = '\n';
        printf(msgAsk.szBuffer);
        printf("Recv bytes = %d, msgAsk.size = %d\n", dwTrans, msgAsk.iBodySize);
      }
      MSG_BODY msgBody = {0};
      memcpy(&msgBody, pPerHandle->buf + msgAsk.iBodySize, sizeof(MSG_BODY));
      if (msgBody.iOpType == OP_READ && msgBody.iBodySize == sizeof(MSG_BODY)) {
        printf("msgBody.szBuffer = %s\n", msgBody.szBuffer);
      }
      MSG_ACK msgAck = {0};
      msgAck.iCheckCode = CHECK_CODE;
      memcpy(msgAck.szBuffer, "This is the ack package",
          strlen("This is the ack package"));
      // ����Ͷ�ݷ���I/O����
      pPerHandle->nOperationType = OP_WRITE;
      WSABUF buf;
      buf.buf = (char*)&msgAck;
      buf.len = sizeof(MSG_ACK);
      OVERLAPPED* pol = (OVERLAPPED*)::GlobalAlloc(GPTR, sizeof(OVERLAPPED));
      DWORD dwFlags = 0, dwSend = 0;
      ::WSASend(pPerHandle->s, &buf, 1, &dwSend, dwFlags, pol, NULL);
    }
    break;
    case OP_WRITE: {
      if (dwTrans == sizeof(MSG_ACK)) {
        printf("Transfer successfully\n");
      }
      // Ȼ��Ͷ�ݽ���I/O����
    }
    break;
    case OP_ACCEPT:
      break;
    }
  }
  return 0;
}
DWORD InitWinsock()
{
  DWORD dwRet = 0;
  WSADATA wsaData;
  dwRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (dwRet != NO_ERROR) {
    printf("error code = %d\n", GetLastError());
    dwRet = GetLastError();
  }
  return dwRet;
}
void UnInitWinsock()
{
  WSACleanup();
}
int test_iocp2_server()
{
  int nPort = 5000;
  InitWinsock();
  // ������ɶ˿ڶ���
  HANDLE hCompletion = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
  if (hCompletion == NULL) {
    DWORD dwRet = GetLastError();
    return dwRet;
  }
  ::CreateThread(NULL, 0, ServerThread, (LPVOID)hCompletion, 0, 0);
  // ���������׽���
  SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  SOCKADDR_IN si;
  si.sin_family = AF_INET;
  si.sin_port = ::htons(nPort);
  si.sin_addr.s_addr = INADDR_ANY;
  ::bind(sListen, (sockaddr*)&si, sizeof(si));
  ::listen(sListen, 10);
  while (TRUE) {
    SOCKADDR_IN saRemote;
    int nRemoteLen = sizeof(saRemote);
    printf("Accepting...\n");
    SOCKET sNew = ::accept(sListen, (sockaddr*)&saRemote, &nRemoteLen);
    //SOCKET sNew = ::accept(sListen, NULL, NULL);
    if (sNew == INVALID_SOCKET) {
      continue;
    }
    printf("Accept one!\n");
    // ���������Ӻ󣬴���һ��per-handle���ݣ�����������ɶ˿ڶ���
    PPER_HANDLE_DATA pPerHandle = (PPER_HANDLE_DATA)::GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA));
    pPerHandle->s = sNew;
    memcpy(&pPerHandle->addr, &saRemote, nRemoteLen);
    pPerHandle->nOperationType = OP_READ;
    ::CreateIoCompletionPort((HANDLE) pPerHandle->s, hCompletion, (ULONG_PTR)pPerHandle, 0);
    // Ͷ��һ����������
    OVERLAPPED* pol = (OVERLAPPED*)::GlobalAlloc(GPTR, sizeof(OVERLAPPED));
    WSABUF buf;
    buf.buf = pPerHandle->buf;
    buf.len = BUFFER_SIZE;
    DWORD dwRecv = 0;
    DWORD dwFlags = 0;
    ::WSARecv(pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, pol, NULL);
  }
  return 0;
}
//�ͻ��ˣ�û����ɶ˿ڣ�
DWORD SendAll(SOCKET& clientSock, char* buffer, int size)
{
  DWORD dwStatus = 0;
  char* pTemp = buffer;
  int total = 0, count = 0;
  while (total < size) {
    count = send(clientSock, pTemp, size - total, 0);
    if (count < 0) {
      dwStatus = WSAGetLastError();
      break;
    }
    total += count;
    pTemp += count;
  }
  return dwStatus ;
}
DWORD RecvAll(SOCKET& sock, char* buffer, int size)
{
  DWORD dwStatus = 0;
  char* pTemp = buffer;
  int total = 0, count = 0;
  while (total < size) {
    count = recv(sock, pTemp, size - total, 0);
    if (count < 0) {
      dwStatus = WSAGetLastError();
      break;
    }
    total += count;
    pTemp += count;
  }
  return dwStatus;
}
int test_iocp2_client()
{
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) {
    printf("error code = %d\n", GetLastError());
    return -1;
  }
  sockaddr_in clientAddr;
  clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port = htons(5000);
  SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientSock == INVALID_SOCKET) {
    printf("Create socket failed, error code = %d\n", WSAGetLastError());
    return -1;
  }
  //connect
  while (connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
    printf("Connecting...\n");
    Sleep(1000);
  }
  MSG_ASK msgAsk = {0};
  msgAsk.iBodySize = sizeof(MSG_ASK);
  msgAsk.iCode = CHECK_CODE;
  memcpy(msgAsk.szBuffer, "This is a header", strlen("This is a header"));
  // ����ͷ��
  SendAll(clientSock, (char*)&msgAsk, msgAsk.iBodySize);
  MSG_BODY msgBody = {0};
  msgBody.iBodySize = sizeof(MSG_BODY);
  msgBody.iOpType = OP_READ;
  memcpy(msgBody.szBuffer, "This is the body", strlen("This is the body"));
  // ����body
  SendAll(clientSock, (char*)&msgBody, msgBody.iBodySize);
  MSG_ACK msgAck = {0};
  RecvAll(clientSock, (char*)&msgAck, sizeof(msgAck));
  if (msgAck.iCheckCode == CHECK_CODE) {
    printf("The process is successful, msgAck.szBuffer = %s \n", msgAck.szBuffer);
  }
  else {
    printf("failed\n");
  }
  closesocket(clientSock);
  WSACleanup();
  return 0;
}

