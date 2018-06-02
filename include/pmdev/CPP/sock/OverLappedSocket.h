/**********************************************************************/
/*˵   ��:���ص���̳���select��
          �����ݽ��ܵ�ʱ��ʹ���ص�IO
      ���ڷ������ݵ�ʱ��ʹ��selectģ��
/***********************************************************************/

#ifndef __OVERLAPPED_SOCKET_
#define __OVERLAPPED_SOCKET_

#include "Socket.h"
#include "SelectSocket.h"

#ifdef WIN32
typedef struct _IO_OPERATION_CONTEXT {
  WSAOVERLAPPED OverLapped;
  WSABUF      WsaBuf;
  long      flag ;
  long      nOperBytes;
  char*     mOperBuf;
} IO_OPERATION_CONTEXT, *PIO_OPERATION_CONTEXT;

class COverLappedSocket : public CSelectSocket
{
public:
  COverLappedSocket();
  virtual ~COverLappedSocket();
public:
  BOOL Recv();
  void HandleReadIO(int nRecvBytes);
public:
  virtual int send(const char* buf, int len);
  virtual BOOL recv(SOCK_DATA_NODE** pNode);
private:
  IO_OPERATION_CONTEXT m_RecvIOOper;

};
#endif

#endif