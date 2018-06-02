
#include "./dspdatestruct.h"

#undef GET_ERR
#define GET_ERR( state )   (DEV_OPER_RESULT)( (state) & 0xff )
//˵��: M��ʾ����, ����0x01 - 0x80, S��ʾ������, ����0x81-0xff

#define M_WPA_SET                0x16

#if 1

#define CMD_M_USER               0x01            //�û�
#define CMD_M_VERIFY             0x02            //��֤��¼
#define CMD_M_ALARM              0x03            //������¼
#define CMD_M_OPTLOG             0x04            //��־��¼
#define CMD_M_CIMAGE             0x05            //���ע���ռ�¼
#define CMD_M_RANGE              0x06            //���伯����������
#define CMD_M_AUTH               0x07            //�ͻ��������֤
#define CMD_M_TIME               0x08            //ϵͳʱ��
#define CMD_M_HEART              0x09            //������
#define CMD_M_DEVINFO            0x0a            //�豸������Ϣ
#define CMD_M_STATIS             0x0b            //ͳ����Ϣ
#define CMD_M_DEVSET             0x0c            //�豸��������
#define CMD_M_FORMAT             0x0d            //�豸��ʽ��
#define CMD_M_DOOR_STATE         0x0e            //��״̬���� ,��3��״̬��0����״̬ �� 1����״̬�� 2����״̬ CMD_M_EOPEN
#define CMD_M_REMOTE_OPEN        0x0f            //Զ�̿�����,
#define CMD_M_TGROUP             0x10            //ʱ����
#define CMD_M_DSPRIGHT           0x11            //Ȩ��
#define CMD_M_UGROUP             0x12            //�û���
#define CMD_M_MCAST              0x13            //�ಥ��ַ
#define CMD_M_APPUPDATE          0x14            //�������
#define CMD_M_IMGCHANGE          0x15            //��������ͼƬ����
#define CMD_M_VIDEO_OUT          0x23   //UDP������Ƶ���� NET_ADDRESS
#define CMD_M_USER_EX            0x24   //�����û��ϴ�����
#define CMD_M_REBOOT             0x25   //�豸����
#define CMD_M_RANGE_EX          0x26   //��¼�������� CMD_M_RANGE_EX
#define CMD_M_SET_IP             0x27   //Զ���޸�IP��ַ
#define CMD_M_NOPEN              0x28   //N+1��������
#define CMD_M_IOCTRL             0x29   //IO����
#define CMD_M_NETPRO             0x30   //��������
#define CMD_M_RESETSCREEN        0x32   //��ĻУ��
#define CMD_M_ID                 0xA1
#else
#define CMD_M_USER          M_USER                         //�û�
#define CMD_M_VERIFY        M_VERIFY                       //��֤��¼
#define CMD_M_ALARM         M_ALARM                        //������¼
#define CMD_M_OPTLOG        M_OPTLOG                       //��־��¼
#define CMD_M_CIMAGE        M_CIMAGE                       //���ע���ռ�¼
#define CMD_M_RANGE         M_RANGE                        //���伯����������
#define CMD_M_AUTH          M_AUTH                         //�ͻ��������֤
#define CMD_M_TIME          M_TIME                         //ϵͳʱ��
#define CMD_M_HEART         M_HEART                        //������
#define CMD_M_DEVINFO       M_DEVINFO                      //�豸������Ϣ
#define CMD_M_STATIS        M_STATIS                       //ͳ����Ϣ
#define CMD_M_DEVSET        M_DEVSET                       //�豸��������
#define CMD_M_FORMAT        M_FORMAT                       //�豸��ʽ��
#define CMD_M_DOOR_STATE    M_DOOR_STATE                   //��״̬���� ,��3��״̬��0����״̬ �� 1����״̬�� 2����״̬ CMD_M_EOPEN
#define CMD_M_REMOTE_OPEN   M_REMOTE_OPEN                  //Զ�̿�����,
#define CMD_M_TGROUP        M_TGROUP                       //ʱ����
#define CMD_M_DSPRIGHT      M_DSPRIGHT                     //Ȩ��
#define CMD_M_UGROUP        M_UGROUP                       //�û���
#define CMD_M_MCAST         M_MCAST                        //�ಥ��ַ
#define CMD_M_APPUPDATE     M_APPUPDATE                    //�������
#define CMD_M_IMGCHANGE     M_IMGCHANGE                    //��������ͼƬ����
#define CMD_M_VIDEO_OUT     M_VIDEO_OUT           //UDP������Ƶ���� NET_ADDRESS
#define CMD_M_USER_EX       M_USER_EX             //�����û��ϴ�����
#define CMD_M_REBOOT        M_REBOOT              //�豸����
#define CMD_M_RANGE_EX      M_RANGE_EX           //��¼�������� CMD_M_RANGE_EX
#define CMD_M_SET_IP        M_SET_IP              //Զ���޸�IP��ַ
#define CMD_M_NOPEN         M_NOPEN               //N+1��������
#define CMD_M_IOCTRL        M_IOCTRL              //IO����
#define CMD_M_NETPRO        M_NETPRO              //��������
#define CMD_M_RESETSCREEN   M_RESETSCREEN         //��ĻУ��
#define CMD_M_ID            M_ID

#endif
#define CMD_S_SET                0x81          //�ϴ�\����
#define CMD_S_DEL                0x82          //ɾ��
#define CMD_S_GET                0x83          //����\���
#define CMD_S_REALTIME_SEND      0x84          //�ɼ�ʵʱ����
#define CMD_S_REALTIME_LOAD      0x85          //ʵʱ����
#define CMD_S_REALTIME_CAP       0x86          //ʵʱ�û��ɼ�
#define CMD_S_REALTIME_CAP_INFO  0x88          //Զ�̲ɼ�ʵ�ǽ�����Ϣ
#define CMD_S_UPDATE_SEND        0x87          //����ʵʱ����
#define CMD_S_CLEAR_INOUT        0x88          //���ͳ�Ƽ���

