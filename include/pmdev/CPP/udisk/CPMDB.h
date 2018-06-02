#ifndef CPMINTERFACE_H
#define CPMINTERFACE_H

/********************************************************************/
/*��������: CPMDB.lib
/*��    ��: ���ڽ�����DSP��ʹ��U����������������
/*˵    ��: ʵ��U���û�����, U�̼�¼����
            ��U���д��ж�̨DSP�е��û����ݺͼ�¼����
/*ʱ    ��: 2011/04/14
/********************************************************************/
#ifdef CPMDB_EXPORTS
#define CPMDB_API
#else
#define CPMDB_API
#endif


#ifdef __cplusplus
extern "C"
{
#endif

  typedef void (*OperCallBack)(void* handle, const char* buf, unsigned int len, void* param);

  //���ܣ��� flag = true���ر� flag = false���ݿ�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API BOOL  CPM_DBOpen(BOOL flag , const char* DB_AbsPath);

  //���ܣ�ע��ص�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API void  CPM_DBSetOperCallBack(OperCallBack cb , void* param);

  //���ܣ��� flag = true���ر� flag = false���ݿ�
  //���������ݿ������·����ȫ��
  //���أ��ɹ�true��ʧ��false
  CPMDB_API void  OperDB(const char* buf, unsigned int len, const char* DB_AbsPath);

#ifdef __cplusplus
};
#endif

#endif