#include "mynet.h"
#include "cstd.h"
#include "IOCPModel.inl"
int test_iocp()
{
  CIOCPModel m_IOCP; // ��Ҫ������ɶ˿�ģ��
  // ��ʼ��Socket��
  if (false == m_IOCP.LoadSocketLib()) {
    printf(_T("����Winsock 2.2ʧ�ܣ����������޷����У�"));
  }
  // ���ñ���IP��ַ
  printf("IP: %s PORT: %d\n", m_IOCP.GetLocalIP(), DEFAULT_PORT);
  // ��������ָ��(Ϊ�˷����ڽ�������ʾ��Ϣ )
  m_IOCP.SetMainDlg(0);
  // ��ʼ����
  if (false == m_IOCP.Start()) {
    printf(_T("����������ʧ�ܣ�"));
    return 0;
  }
  m_IOCP.Stop();
  return 0;
}

