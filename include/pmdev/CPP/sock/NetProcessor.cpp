//#include "StdAfx.h"
#include "NetProcessor.h"
#include "ThreadPool.h"
#include "MemPool.h"
#include "SockHandle.h"
#include "NetManager.h"
#ifdef WIN32
#include "sftime.hpp"
#endif

extern CNetManager* gNetManager;

CNetProcessor::CNetProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
  CMemPool::GetInstance().CreateMemPool((uMemPoolSize + sizeof(SOCK_DATA_HEADER))*MEMRESERVER_BLOCKS, uMemPoolUnit);

#ifdef WIN32
  SYSTEM_INFO info = { 0 };
  ::GetSystemInfo(&info);
  //��ʱ�����ڴ�صĴ�С���ж��Ƕ��̻߳ص����ǵ��̻߳ص�
  m_pThreadPool = new CThreadPool(uMemPoolSize < 4 * 1024 * 1024 ? 1 : info.dwNumberOfProcessors + 2);
#else
  m_pThreadPool = new CThreadPool(1);
#endif

}

CNetProcessor::~CNetProcessor()
{
  delete m_pThreadPool;
}

void CNetProcessor::StopRecv(SOCKET sock)
{
  gNetManager->OnRecv(sock, NULL, 0);
}

void CNetProcessor::StopListen(SOCKET sock)
{
  gNetManager->OnRecv(sock, NULL, 0);
}

void CNetProcessor::Accept(SOCKET sock, const SOCK_ADDRESS& Addr)
{
  gNetManager->OnListen(sock, Addr);
}
