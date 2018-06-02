#ifndef CPMINTERFACE_H
#define CPMINTERFACE_H
////////////////////////////////////////////////////////////////////*/
//��������: CPMDB.lib
//��    ��: ���ڽ�����DSP��ʹ��U����������������
//˵    ��: ʵ��U���û�����, U�̼�¼����
//            ��U���д��ж�̨DSP�е��û����ݺͼ�¼����
//ʱ    ��: 2011/04/14
////////////////////////////////////////////////////////////////////*/
#ifdef WIN32
#ifdef CPMDB_EXPORTS
#define CPMDB_API __declspec(dllexport)
#else
#define CPMDB_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C"
{
#endif
  typedef void (*OperCallBack)(void* handle, const char* buf, unsigned int len, void* param);
  //���ܣ��� flag = true���ر� flag = false���ݿ�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API bool __stdcall CPM_DBOpen(bool flag , const char* DB_AbsPath);
  //���ܣ�ע��ص�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API void __stdcall CPM_DBSetOperCallBack(OperCallBack cb , void* param);
  //���ܣ��� flag = true���ر� flag = false���ݿ�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API void __stdcall OperDB(const char* buf, unsigned int len, const char* DB_AbsPath);
#ifdef __cplusplus
};
#endif
#else
typedef void (*OperCallBack)(void* handle, const char* buf, unsigned int len, void* param);
//���ܣ��� flag = true���ر� flag = false���ݿ�
//���������ݿ������·����ȫ��
//���أ��ɹ�true��ʧ��false
bool  CPM_DBOpen(bool flag , const char* DB_AbsPath)
{
  return true;
}
//���ܣ�ע��ص�
//���������ݿ������·����ȫ��
//���أ��ɹ�true��ʧ��false
void  CPM_DBSetOperCallBack(OperCallBack cb , void* param)
{
  return ;
}
//���ܣ��� flag = true���ر� flag = false���ݿ�
//���������ݿ������·����ȫ��
//���أ��ɹ�true��ʧ��false
void  OperDB(const char* buf, unsigned int len, const char* DB_AbsPath)
{
  return ;
}
#endif
#endif

