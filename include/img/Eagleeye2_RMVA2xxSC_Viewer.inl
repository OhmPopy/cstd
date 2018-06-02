
#include <winsock.h>
#include <process.h>
#include "cstd.h"
#include "Eagleeye2_RMVA2xxSC_Viewer.h"

//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")

//Recv image data thread
void  RecvTrgImagedata(LPVOID pParam);  //����ģʽ��������������ݽ����������̺߳�������
void  RecvConImagedata(LPVOID pParam);  //ȫ�ֱ�������ģʽ��������������ݽ����������̺߳�������
void  RecvHfconImagedata(LPVOID pParam);  //������ģʽ��������������ݽ����������̺߳�������
void  RecvHfBinconImagedata(LPVOID pParam);//������ģʽ��������������ݽ����������̺߳�������

//global variables
UINT      uiJpgDataLen;        //���ܵ���jpgͼ�����ݵĳ���
BUFFNODE  listJpgBuff[NUM_BUFF];    //װ��JPGͼ�����ݵĻ��λ�������
LONG      PrevBuff;          //���λ����ǰһ��ʹ������
LONG      CurrBuff;          //���λ���ĵ�ǰʹ������
volatile BYTE  TargetIPAdd1, TargetIPAdd2; //
volatile BYTE  TargetIPAdd3, TargetIPAdd4; //ά����ǰĿ�����IP��ַ��4������
char      cRcvBuf[4096];         //��Ŀ���������������Ľ��ܻ�����
char      cmdbuf[1024 * 5];      //��Ŀ���������������ķ��ͻ�����
volatile unsigned char  pImgBuf8[IMGLENGTH * 3];
EagleEye2ImgInfStruct   EagleEye2_ImgInf = {
  {'\0'},      //ui8LocusInf[101]
  {0},      //ui8TimeInf[7]
  0,        //ui8WayNum
  0,        //ui8TrgMode
  0.0        //fSpeed
};;


/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc construction/destruction

void CEagleeye2_ViewerDoc(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  int nBuff = 0;
  //һЩͨ�ÿ��Ʊ����ĳ�ʼ��
  ev->m_bIsTrgMode  = FALSE;    //�Ƿ񴥷�ģʽ
  ev->m_bIsFconMode  = FALSE;    //�Ƿ�ȫ�ֱ�������ģʽ
  ev->m_bIsHFRMode  = FALSE;    //�Ƿ������ģʽ
  ev->m_bIsHFRBinMode = FALSE;    //�Ƿ�����ʸ�������ģʽ
  ev->m_bIsEnsureTgtIP = FALSE;    //�Ƿ�ȷ��Ŀ�������IP��ַ
  ev->m_bRelayIsOpen = TRUE;      //����̵����Ƿ�Ͽ�
  ev->m_bLUTEnable = TRUE;      //����Ƿ�ʹ��ͼ���LUTӳ��任
  ev->m_bAutoTrgEnable = FALSE;    //�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
  ev->m_bFirstShow = TRUE ;      //��������Ƿ�Ϊ������ʾ������ά����ͼ��ˢ��
  ev->m_bIsDataFromCamera = FALSE;  //��־��ǰ��ͼ��ʾ��ͼ���Ƿ�������������������Ҳ�ɴ�Ӳ���еľ�̬ͼ��
  ev->m_bIsJpeg = FALSE;        //��־��ǰ��ͼ��ʾ��ͼ���Ƿ���JPG��ʽ
  ev->m_bIsHighFR = FALSE;      //��־��ǰ��Ҫ��ʾ��ͼ���Ƿ����Ը�����ģʽ���Դ���������������ģʽ��������OnDraw�����н�����ʾ����
  ev->m_bIsRGBArray = TRUE;      //����ɼ���ͼ���ʽ�Ƿ�ΪRGB��ʽ
  ev->m_bIsYUVArray = FALSE;      //����ɼ���ͼ���ʽ�Ƿ�ΪYUV��ʽ
  ev->m_bIsBAYERArray = FALSE;    //����ɼ���ͼ���ʽ�Ƿ�ΪBAYER��ʽ
  ev->m_bTrgShutterIsManual = TRUE;  //����ģʽ�����ٶ��Ƿ����ֶ�ģʽ

  //��ʼ�����λ�����
  for (nBuff = 0; nBuff < NUM_BUFF; nBuff++) {
    listJpgBuff[nBuff].sizeBuff = 0;
    listJpgBuff[nBuff].bIsLocked = FALSE;
  }

  CurrBuff = 0;
  PrevBuff = -1;
}

#define AfxMessageBox printf
///////////////////////////////////////////////////////////////////////////////
//              Sub Threads
///////////////////////////////////////////////////////////////////////////////
void RecvTrgImagedata(LPVOID pParam)
{
  //����DSPͼ�����ݵ��߳�
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockTrgClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  static char    strFileNamebak[256] = "\0";
  static int      iIndex = 0;
  //����ץ��ͼ��
  char strFileName[256], strDirectory[256];
  char strFolderName[256];
  char tmpStr[20];

  //��ȡĿ�������IP��ַ
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
    return;
  }

  sockTrgClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockTrgClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(TRGIMGPORT);

  //����Ŀ�����
  if (connect(sockTrgClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //��ʼ��������ϴ�ͼ������
  while (1) {
    //����JPG��������
    i = 0;

    while (i < 4) {
      //CFileFind   f;
      cnt = recv(sockTrgClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }

      //���������ͻ�0xffff���ʾ��������ͨѶ
      if (recvdatalen == 0x00) {
        goto leave;
      }
    }

    //����ץ����Ϣ
    i = 0;

    while (i < IMGINFLEN) {
      cnt = recv(sockTrgClient, (char*)&EagleEye2_ImgInf + i, IMGINFLEN - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    //����ͼ������
    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockTrgClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;
    //֪ͨ��ͼ������ʾ
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);

    //�ص��ʱ��
    if (strlen(EagleEye2_ImgInf.cLocusInf) > 0) {
      strcat(strFileName, EagleEye2_ImgInf.cLocusInf);

      if (EagleEye2_ImgInf.byTimeInf[0] > 9) {
        strcat(strFileName , "_20");
        strcat(strFolderName , "20");
      }
      else {
        strcat(strFileName , "_200");
        strcat(strFolderName , "200");
      }
    }
    else {
      if (EagleEye2_ImgInf.byTimeInf[0] > 9) {
        strcat(strFileName , "20");
        strcat(strFolderName , "20");
      }
      else {
        strcat(strFileName , "200");
        strcat(strFolderName , "200");
      }
    }

    strcat(strFileName , itoa(EagleEye2_ImgInf.byTimeInf[0], tmpStr, 10));
    strcat(strFolderName , itoa(EagleEye2_ImgInf.byTimeInf[0], tmpStr, 10));

    if (EagleEye2_ImgInf.byTimeInf[1] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[1], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[2] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[2], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[4] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFolderName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
      strcat(strFolderName, itoa(EagleEye2_ImgInf.byTimeInf[4], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[5] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[5], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[5], tmpStr, 10));
    }

    if (EagleEye2_ImgInf.byTimeInf[6] > 9) {
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[6], tmpStr, 10));
    }
    else {
      strcat(strFileName , "0");
      strcat(strFileName, itoa(EagleEye2_ImgInf.byTimeInf[6], tmpStr, 10));
    }

    //������
    strcat(strFileName , "_");
    strcat(strFileName , itoa(EagleEye2_ImgInf.byWayNum, tmpStr, 10));
    strcat(strFileName , "����");

    //������
    if (!strcmp(strFileName, strFileNamebak)) {
      //�����ļ���
      strcpy(strFileNamebak , strFileName);
      //����������
      strcat(strFileName , "_");
      strcat(strFileName , itoa(++iIndex, tmpStr, 10));
    }
    else {
      //�����ļ���
      strcpy(strFileNamebak , strFileName);
      iIndex = 0;
    }

    //��������
    switch (EagleEye2_ImgInf.byTrgMode) {
    case 0://����
      strcat(strFileName , "_����");
      break;

    case 1://����Ȧ
      strcat(strFileName , "_����Ȧ����");
      break;

    case 2://˫��Ȧ
      strcat(strFileName , "_˫��Ȧ��������Ϊ");

      if (EagleEye2_ImgInf.fSpeed - (int)EagleEye2_ImgInf.fSpeed < 0.0001) {
        strcat(strFileName , itoa((int)EagleEye2_ImgInf.fSpeed, tmpStr, 10));
      }
      else {
        int tttt;
        strcat(strFileName , _gcvt(EagleEye2_ImgInf.fSpeed, 5, tmpStr));
        tttt = strlen(strFileName);

        if (strFileName[(strlen(strFileName) - 1)] == '.') {
          strcat(strFileName , "00");
        }
      }

      strcat(strFileName , "����");
      break;

    case 3://ר��Ӳ����
      strcat(strFileName , "_ר��Ӳ����");
      break;

    default:
      break;
    }

    //Creat Directory
    strcpy(strDirectory, "D:\\Picture\\");
    CreateDirectory(strDirectory, NULL);
    strcat(strDirectory, strFolderName);
    strcat(strDirectory, "\\");
    CreateDirectory(strDirectory, NULL);
    //end Creat Directory
    strcat(strDirectory , strFileName);
    strcat(strDirectory , ".jpg");
    imfree(pDoc->m_fPic1);
    pDoc->m_fPic1 = imread2_mem((BYTE*)listJpgBuff[PrevBuff].pBuff, listJpgBuff[PrevBuff].sizeBuff, 0);
    imwrite2(strDirectory, pDoc->m_fPic1);
  }

leave:
  //���������˳��߳�
  closesocket(sockTrgClient);
  WSACleanup();
}

void RecvConImagedata(LPVOID pParam)
{
  //����DSPͼ�����ݵ��߳�
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockFconClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //��ȡĿ�������IP��ַ
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
    return;
  }

  sockFconClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockFconClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(FCONIMGPORT);

  //����Ŀ�����
  if (connect(sockFconClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //��ʼ��������ϴ�ͼ������
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockFconClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockFconClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //��ȫ�ֱ���ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
    if (!pDoc->m_bIsFconMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockFconClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockFconClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //֪ͨ��ͼ������ʾ
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //���������˳��߳�
  closesocket(sockFconClient);
  WSACleanup();
}

void RecvHfconImagedata(LPVOID pParam)
{
  //����DSPͼ�����ݵ��߳�
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockHfrClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //��ȡĿ�������IP��ַ
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
    return;
  }

  sockHfrClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockHfrClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(HFRIMGPORT);

  //����Ŀ�����
  if (connect(sockHfrClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //��ʼ��������ϴ�ͼ������
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockHfrClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockHfrClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //�ڸ�����ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
    if (!pDoc->m_bIsHFRMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockHfrClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockHfrClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //֪ͨ��ͼ������ʾ
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //���������˳��߳�
  closesocket(sockHfrClient);
  WSACleanup();
}

void RecvHfBinconImagedata(LPVOID pParam)
{
  //����DSPͼ�����ݵ��߳�
  Eagleeye2_RMVA2xxSC_Viewer* pDoc = (Eagleeye2_RMVA2xxSC_Viewer*) pParam;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockHfrBinClient;
  struct sockaddr_in  addr;
  int          i, cnt;
  BOOL        flag = TRUE;
  UINT        recvdatalen;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  UINT        tmp;
  BYTE        replyBuf[2];

  //��ȡĿ�������IP��ַ
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("�޷���ʼ��ȫ�ֱ�������ģʽ�׽ӿڶ���");
    return;
  }

  sockHfrBinClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockHfrBinClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(HFRBINIMGPORT);

  //����Ŀ�����
  if (connect(sockHfrBinClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //��ʼ��������ϴ�ͼ������
  while (1) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockHfrBinClient, (char*)&uiJpgDataLen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = uiJpgDataLen;
      }
    }

    i = 0;
    listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
    pDoc->m_bIsJpeg = TRUE;

    while (i < (int)recvdatalen) {
      cnt = recv(sockHfrBinClient, listJpgBuff[CurrBuff].pBuff + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto leave;
      }

      i += cnt;
    }

    CurrBuff++;

    if (CurrBuff == 5) {
      CurrBuff = 0;
    }

    PrevBuff++;

    if (PrevBuff == 5) {
      PrevBuff = 0;
    }

    pDoc->m_bIsDataFromCamera = TRUE;

    //�ڸ�����ģʽ�»ظ�����Ƿ������ǰ���ӣ����ظ�0xffff����������������������
    if (!pDoc->m_bIsHFRBinMode) {
      replyBuf[0] = 0xff;
      replyBuf[1] = 0xff;

      if (send(sockHfrBinClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }

      goto leave;
    }
    else {
      replyBuf[0] = 0x0;
      replyBuf[1] = 0x0;

      if (send(sockHfrBinClient, (char*)replyBuf, 2, 0) < 0) {
        goto leave;
      }
    }

    //֪ͨ��ͼ������ʾ
    PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
  }

leave:
  //���������˳��߳�
  closesocket(sockHfrBinClient);
  WSACleanup();
}

//////////////////////////////////////////////////////////////////////////
//��Ŀ�������������
BOOL SendCMDtoDSP(Eagleeye2_RMVA2xxSC_Viewer* ev, char* cmdbuf)
{
  char*        buf = cmdbuf;
  WORD        wVersionRequested;
  WSADATA        wsaData;
  SOCKET        sockCMDClient;
  struct sockaddr_in  addr;
  UINT        i, cnt, tmp;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  u_long        bNonblock = 1;
  UINT        datalen, recvdatalen;

  //����ʹ��TCPЭ�����Ŀ���������ָ����׽ӿڶ���
  tmp1 = TargetIPAdd1;
  tmp2 = TargetIPAdd2;
  tmp3 = TargetIPAdd3;
  tmp4 = TargetIPAdd4;
  tmp =  tmp1;
  tmp |=  tmp2 << 8;
  tmp |=  tmp3 << 16;
  tmp |=  tmp4 << 24;
  wVersionRequested = MAKEWORD(1, 1);
  i = WSAStartup(wVersionRequested, &wsaData);

  if (i != 0) {
    AfxMessageBox("�޷���ʼ�����������׽ӿڶ���");
    return FALSE;
  }

  sockCMDClient = socket(AF_INET, SOCK_STREAM, 0);

  if (sockCMDClient < 0) {
    goto leave;
  }

  addr.sin_family    = AF_INET;
  addr.sin_addr.s_addr  = tmp;
  addr.sin_port    = htons(COMMANDPORT);

  //����Ŀ�����
  if (connect(sockCMDClient, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
    goto leave;
  }

  //��������ID�Ͳ����ĳ���(�ֽ���)
  if (send(sockCMDClient, buf, 6, 0) < 0) {
    goto leave;
  }

  //�����������
  if (send(sockCMDClient, buf + 6, *(UINT*)(buf + 2), 0) < 0) {
    goto leave;
  }

  //�������Ӧ�𣬽��������Ӧ
  if (buf[1]) {
    i = 0;

    while (i < 4) {
      cnt = recv(sockCMDClient, (char*)&datalen + i, 4 - i, 0);

      if (cnt <= 0) {
        goto  leave;
      }

      i += cnt;

      if (i == 4) {
        recvdatalen = datalen;
      }
    }

    i = 0;

    while (i < recvdatalen) {
      cnt = recv(sockCMDClient, cRcvBuf + i, recvdatalen - i, 0);

      if (cnt <= 0) {
        goto  leave;
      }

      i += cnt;
    }
  }

  //�ر��׽ӿڶ��󲢷�����ֵ
  closesocket(sockCMDClient);
  WSACleanup();
  return TRUE;

  //�������󣬹ر��׽ӿڶ��󲢷���
leave:
  closesocket(sockCMDClient);
  WSACleanup();
  ev->m_bIsEnsureTgtIP = FALSE;
  return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//����Ŀ�������IP��ַ��ͬʱ��������ʹĿ���������ȫ�ֱ�������ģʽ�µ������ϴ�
void OnConfigTargetip(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  UINT    tmp1 = 0, tmp2 = 0;

  {
    TargetIPAdd1 = ev->m_byIPAdd1;
    TargetIPAdd2 = ev->m_byIPAdd2;
    TargetIPAdd3 = ev->m_byIPAdd3;
    TargetIPAdd4 = ev->m_byIPAdd4;
    //��λ����ģʽ�ı�־��ȷ��Ŀ��IP��־
    ev->m_bIsTrgMode  = FALSE;
    ev->m_bIsFconMode  = FALSE;
    ev->m_bIsHFRMode  = FALSE;
    ev->m_bIsHFRBinMode = FALSE;
    ev->m_bIsEnsureTgtIP = FALSE;
    //��ⴥ��ģʽ�������Ƿ�û���˳����ӣ���û����Ͽ�
    cmdbuf[0] = 0x23;
    cmdbuf[1] = 0x00;//��Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������ʧ��!");
      return;
    }

    //��ѯĿ������Ĺ���ģʽ�������ӳɹ�������Ӧ�Ŀͻ����߳�
    cmdbuf[0] = 0x03;
    cmdbuf[1] = 0x01;//��Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (SendCMDtoDSP(ev, cmdbuf)) {
      ev->m_bIsEnsureTgtIP = TRUE;
      //��ȡ��ز������ý���
      //���ÿɱ��I/O�Ŀڷ�������ֵ�Ը��½���
      tmp1 = *(UINT*)(cRcvBuf + 16);
      tmp2 = *(UINT*)(cRcvBuf + 20);

      //port1����
      if ((tmp1 & 0x001) == 0) {
        //����
        ev->m_bIsPort1IN = TRUE;
        ev->m_pioIndicator.pio1 = 0;
      }
      else {
        //���
        ev->m_bIsPort1IN = FALSE;
        ev->m_pioIndicator.pio1 = 1;

        if ((tmp2 & 0x001) == 0) {
          ev->m_bPort1OUTValue = FALSE;
        }
        else {
          ev->m_bPort1OUTValue = TRUE;
        }
      }

      //port2����
      if ((tmp1 & 0x002) == 0) {
        //����
        ev->m_bIsPort2IN = TRUE;
        ev->m_pioIndicator.pio2 = 0;
      }
      else {
        //���
        ev->m_bIsPort2IN = FALSE;
        ev->m_pioIndicator.pio2 = 1;

        if ((tmp2 & 0x002) == 0) {
          ev->m_bPort2OUTValue = FALSE;
        }
        else {
          ev->m_bPort2OUTValue = TRUE;
        }
      }

      //port3����
      if ((tmp1 & 0x004) == 0) {
        //����
        ev->m_bIsPort3IN = TRUE;
        ev->m_pioIndicator.pio3 = 0;
      }
      else {
        //���
        ev->m_bIsPort3IN = FALSE;
        ev->m_pioIndicator.pio3 = 1;

        if ((tmp2 & 0x004) == 0) {
          ev->m_bPort3OUTValue = FALSE;
        }
        else {
          ev->m_bPort3OUTValue = TRUE;
        }
      }

      //port4����
      if ((tmp1 & 0x008) == 0) {
        //����
        ev->m_bIsPort4IN = TRUE;
        ev->m_pioIndicator.pio4 = 0;
      }
      else {
        //���
        ev->m_bIsPort4IN = FALSE;
        ev->m_pioIndicator.pio4 = 1;

        if ((tmp2 & 0x008) == 0) {
          ev->m_bPort4OUTValue = FALSE;
        }
        else {
          ev->m_bPort4OUTValue = TRUE;
        }
      }

      //port5����
      if ((tmp1 & 0x010) == 0) {
        //����
        ev->m_bIsPort5IN = TRUE;
        ev->m_pioIndicator.pio5 = 0;
      }
      else {
        //���
        ev->m_bIsPort5IN = FALSE;
        ev->m_pioIndicator.pio5 = 1;

        if ((tmp2 & 0x010) == 0) {
          ev->m_bPort5OUTValue = FALSE;
        }
        else {
          ev->m_bPort5OUTValue = TRUE;
        }
      }

      //port6����
      if ((tmp1 & 0x020) == 0) {
        //����
        ev->m_bIsPort6IN = TRUE;
        ev->m_pioIndicator.pio6 = 0;
      }
      else {
        //���
        ev->m_bIsPort6IN = FALSE;
        ev->m_pioIndicator.pio6 = 1;

        if ((tmp2 & 0x020) == 0) {
          ev->m_bPort6OUTValue = FALSE;
        }
        else {
          ev->m_bPort6OUTValue = TRUE;
        }
      }

      //port7����
      if ((tmp1 & 0x040) == 0) {
        //����
        ev->m_bIsPort7IN = TRUE;
        ev->m_pioIndicator.pio7 = 0;
      }
      else {
        //���
        ev->m_bIsPort7IN = FALSE;
        ev->m_pioIndicator.pio7 = 1;

        if ((tmp2 & 0x040) == 0) {
          ev->m_bPort7OUTValue = FALSE;
        }
        else {
          ev->m_bPort7OUTValue = TRUE;
        }
      }

      //port8����
      if ((tmp1 & 0x080) == 0) {
        //����
        ev->m_bIsPort8IN = TRUE;
        ev->m_pioIndicator.pio8 = 0;
      }
      else {
        //���
        ev->m_bIsPort8IN = FALSE;
        ev->m_pioIndicator.pio8 = 1;

        if ((tmp2 & 0x080) == 0) {
          ev->m_bPort8OUTValue = FALSE;
        }
        else {
          ev->m_bPort8OUTValue = TRUE;
        }
      }

      //port9����
      if ((tmp1 & 0x100) == 0) {
        //����
        ev->m_bIsPort9IN = TRUE;
        ev->m_pioIndicator.pio9 = 0;
      }
      else {
        //���
        ev->m_bIsPort9IN = FALSE;
        ev->m_pioIndicator.pio9 = 1;

        if ((tmp2 & 0x100) == 0) {
          ev->m_bPort9OUTValue = FALSE;
        }
        else {
          ev->m_bPort9OUTValue = TRUE;
        }
      }

      //port10����
      if ((tmp1 & 0x200) == 0) {
        //����
        ev->m_bIsPort10IN = TRUE;
        ev->m_pioIndicator.pio10 = 0;
      }
      else {
        //���
        ev->m_bIsPort10IN = FALSE;
        ev->m_pioIndicator.pio10 = 1;

        if ((tmp2 & 0x200) == 0) {
          ev->m_bPort10OUTValue = FALSE;
        }
        else {
          ev->m_bPort10OUTValue = TRUE;
        }
      }

      //����ͼ��ɼ���ʽ
      switch (cRcvBuf[15]) {
      case RGB_ARRAY:
        ev->m_bIsRGBArray = TRUE;
        ev->m_bIsYUVArray = FALSE;
        ev->m_bIsBAYERArray = FALSE;
        break;

      case YUV_ARRAY:
        ev->m_bIsRGBArray = FALSE;
        ev->m_bIsYUVArray = TRUE;
        ev->m_bIsBAYERArray = FALSE;
        break;

      case BAYER_ARRAY:
        ev->m_bIsRGBArray = FALSE;
        ev->m_bIsYUVArray = FALSE;
        ev->m_bIsBAYERArray = TRUE;
        break;

      default:
        break;
      }

      //��������ģʽ
      switch (cRcvBuf[14]) {
      case TrgMode:
        ev->m_bIsTrgMode = TRUE;
        _beginthread(RecvTrgImagedata, 0, (LPVOID)ev);
        break;

      case FconMode:
        ev->m_bIsFconMode = TRUE;
        _beginthread(RecvConImagedata, 0, (LPVOID)ev);
        break;

      case HfrMode:
        ev->m_bIsHFRMode = TRUE;
        _beginthread(RecvHfconImagedata, 0, (LPVOID)ev);
        break;

      case HfrBinMode:
        ev->m_bIsHFRBinMode = TRUE;
        _beginthread(RecvHfBinconImagedata, 0, (LPVOID)ev);
        break;

      default:
        break;
      }

      //���ð�ƽ�⻷��ѡ��Ľ�����ʾ
      switch (cRcvBuf[13]) {
      case DAYLIGHT:
        ev->m_uiWBType = 0;
        break;

      case FLUORESCENCE:
        ev->m_uiWBType = 1;
        break;

      case WITHOUTWB:
        ev->m_uiWBType = 2;
        break;

      default:
        break;
      }

      //���������ͬ�����״̬
      if (cRcvBuf[8]) {
        ev->m_bIsFlashSynEnable = TRUE;
      }
      else {
        ev->m_bIsFlashSynEnable = FALSE;
      }

      //�����Ƿ�����ģʽ�л�������ģʽ���Ƿ������Զ�����ץȡһ��ͼ��
      if (cRcvBuf[9]) {
        ev->m_bAutoTrgEnable = TRUE;
      }
      else {
        ev->m_bAutoTrgEnable = FALSE;
      }

      //�����Ƿ�ʹ��ͼ��LUTӳ��任
      if (cRcvBuf[10]) {
        ev->m_bLUTEnable = TRUE;
      }
      else {
        ev->m_bLUTEnable = FALSE;
      }

      //���ü̵����Ƿ�պ�
      if (cRcvBuf[11]) {
        ev->m_bRelayIsOpen = FALSE;
      }
      else {
        ev->m_bRelayIsOpen = TRUE;
      }

      //�����Ƿ�ʹ�ܿ��Ź������ÿ��Ź���ʱ��ʱ�������ʾֵ
      if (cRcvBuf[12]) {
        ev->m_bIsWDTEnable = TRUE;
      }
      else {
        ev->m_bIsWDTEnable = FALSE;
      }

      ev->m_iWDT = *(unsigned int*)(cRcvBuf + 4);
      //���ô���������ź����������ʾֵ
      ev->m_fAfeVGAGain = *(float*)(cRcvBuf);
      //////////////////////////////////////////////////////////////////////////
      //����������Ĵ��ڶ���,֪ͨ����������״̬��
      //pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
      //m_hMainFrameWnd = pMain->GetSafeHwnd();
      //PostMessage(m_hMainFrameWnd, WM_SWICHNEWCAMERA, 0, 0);
    }
    else {
      ev->m_bIsEnsureTgtIP = FALSE;
      AfxMessageBox("����Ŀ�����ʧ�ܣ���˶�Ŀ�������IP��ַ");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//            �������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л����������ģʽ
void OnOmcTrigger(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bIsTrgMode  = TRUE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = FALSE;
  //���������л����������ģʽ
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  cmdbuf[6] = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
  else {
    _beginthread(RecvTrgImagedata, 0, (LPVOID)ev);
  }
}
//��������ڴ���ģʽ�µĹ���״̬
void OnTrgmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    int cmdlen = 0;
    //����ģʽ�µ��������
    cmdbuf[0] = 0x04;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    //ץ�ĵص�
    cmdlen += strlen(ev->m_strLocation);

    if (cmdlen > 0) {
      memcpy(cmdbuf + 6, ev->m_strLocation, cmdlen);
    }

    memset(cmdbuf + 6 + cmdlen, '\0', 101 - cmdlen);
    cmdlen = 101;
    //������
    cmdbuf[cmdlen + 6] = ev->m_iWayIndex;
    cmdlen += 1;

    //������ʽ
    if (ev->m_bIsSingalCoin) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //��Ϊ˫��Ȧ��������Ȧ���
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fInterval;
    cmdlen += 4;

    //�����������
    if (ev->m_bIsManualCtrl) {
      cmdbuf[cmdlen + 6] = 0;
    }
    else {
      cmdbuf[cmdlen + 6] = 1;
    }

    cmdlen += 1;
    //�ֶ�ģʽ�µĿ�������
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_uiTrgShutterTime;
    cmdlen += 4;
    //�ֶ�ģʽ�µ���������
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fTrgPga;
    cmdlen += 4;
    //�Զ�ģʽ�µĿ�����������
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iShutLimit;
    cmdlen += 4;
    //�Զ�ģʽ�µ�������������
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fPGALimit;
    cmdlen += 4;
    //�Զ�ģʽ�µĵ�ǰ��������
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iCurShut;
    cmdlen += 4;
    //�Զ�ģʽ�µĵ�ǰ��������
    *(float*)(cmdbuf + cmdlen + 6) = ev->m_fCurPga;
    cmdlen += 4;
    //�Զ�ģʽ�µĲ��������ʼ��
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iSRow;
    cmdlen += 4;
    //�Զ�ģʽ�µĲ��������ʼ��
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iSCol;
    cmdlen += 4;
    //�Զ�ģʽ�µĲ��������ֹ��
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iERow;
    cmdlen += 4;
    //�Զ�ģʽ�µĲ��������ֹ��
    *(UINT*)(cmdbuf + cmdlen + 6) = ev->m_iECol;
    cmdlen += 4;
    //�Զ�ģʽ�µĲ����������ƽ������
    cmdbuf[cmdlen + 6] = ev->m_byYMean;
    cmdlen += 1;

    //�Զ�ģʽ���Ƿ������Բ�ⴥ��
    if (ev->m_bEnableADLI) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //�Զ�ģʽ�����Բ�ⴥ������
    cmdbuf[cmdlen + 6] = ev->m_iADLIP;
    cmdlen += 1;

    //�Ƿ񱣴����
    if (ev->m_bSaveParam) {
      cmdbuf[cmdlen + 6] = 1;
    }
    else {
      cmdbuf[cmdlen + 6] = 0;
    }

    cmdlen += 1;
    //����������������ڴ���ģʽ�µ��ع�ʱ��
    cmdbuf[0] = 0x04;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = cmdlen;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//�������ץȡһ��ͼ��
void OnOpeGrab(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //������������ץȡһ��ͼ��
  cmdbuf[0] = 0x02;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}

//////////////////////////////////////////////////////////////////////////
//          ���ȫ�ֱ�����ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л������ȫ�ֱ�������ģʽ
void OnOmcMjpeg(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = TRUE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = FALSE;

  //�Ƿ�ֹͣ����˴���ģʽ�ķ���
  if (bStopTrgSvr) {
    //����ֹͣ����ģʽ�·���ı�־
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
      return;
    }
  }

  Sleep(200);
  //�л������ȫ�ֱ�������ģʽ
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
  else {
    _beginthread(RecvConImagedata, 0, (LPVOID)ev);
  }
}

//////////////////////////////////////////////////////////////////////////
//          ���������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л������������ģʽ
void OnOmcHfr(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = TRUE;
  ev->m_bIsHFRBinMode = FALSE;

  //�Ƿ�ֹͣ����˴���ģʽ�ķ���
  if (bStopTrgSvr) {
    //����ֹͣ����ģʽ�·���ı�־
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
      return;
    }
  }

  //�л������������ģʽ
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x02;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
  else {
    _beginthread(RecvHfconImagedata, 0, (LPVOID)ev);
  }
}

//��������ڸ�����ģʽ�µĹ���״̬
void OnHfrmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    if (ev->m_bIsShutPgaManualCtrl) {
      //��������ڸ�����ģʽ��Ϊ�ֶ�����ģʽ
      cmdbuf[0] = 0x11;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 0;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ���ֶ����з�ʽ�µ��ع�ʱ��
      cmdbuf[0] = 0x06;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_uiTrgShutterTime;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
    else {
      //��������ڸ�����ģʽ��Ϊ�Զ�����ģʽ
      cmdbuf[0] = 0x11;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 1;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ��ź������Զ����ڵ���������
      cmdbuf[0] = 0x13;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x04;
      *(float*)(cmdbuf + 6) = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ��ع�ʱ���Զ����ڵ���������
      cmdbuf[0] = 0x12;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_bIsShutPgaAutoCtrl;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ�ͼ��ƽ��������ֵ
      cmdbuf[0] = 0x15;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = ev->m_byYMean;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//          ��������ʸ�������ģʽ����ؿ���
//////////////////////////////////////////////////////////////////////////
//�л�����������ʸ�������ģʽ
void OnOmcHfrb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  BOOL  bStopTrgSvr = FALSE;

  if (ev->m_bIsTrgMode) {
    bStopTrgSvr = TRUE;
  }

  ev->m_bIsTrgMode  = FALSE;
  ev->m_bIsFconMode  = FALSE;
  ev->m_bIsHFRMode  = FALSE;
  ev->m_bIsHFRBinMode = TRUE;

  //�Ƿ�ֹͣ����˴���ģʽ�ķ���
  if (bStopTrgSvr) {
    //����ֹͣ����ģʽ�·���ı�־
    cmdbuf[0] = 0x21;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x00;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
      return;
    }
  }

  //�л������������ģʽ
  cmdbuf[0] = 0x01;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 1;
  cmdbuf[6] = 0x03;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
  else {
    _beginthread(RecvHfBinconImagedata, 0, (LPVOID)ev);
  }
}

void OnHfrbinmodeConfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    if (ev->m_bIsShutPgaManualCtrl) {
      //��������ڸ�����ģʽ��Ϊ�ֶ�����ģʽ
      cmdbuf[0] = 0x25;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 0;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ���ֶ����з�ʽ�µ��ع�ʱ��
      cmdbuf[0] = 0x24;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_uiTrgShutterTime;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
    else {
      //��������ڸ�����ģʽ��Ϊ�Զ�����ģʽ
      cmdbuf[0] = 0x25;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = 1;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ��ź������Զ����ڵ���������
      cmdbuf[0] = 0x13;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x04;
      *(float*)(cmdbuf + 6) = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;
      ev->m_fPGALimit = ev->m_fPGALimit;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ��ع�ʱ���Զ����ڵ���������
      cmdbuf[0] = 0x26;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x03;
      *(UINT*)(cmdbuf + 6) = ev->m_bIsShutPgaAutoCtrl;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //��������ڸ�����ģʽ�µ�ͼ��ƽ��������ֵ
      cmdbuf[0] = 0x27;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      *(UINT*)(cmdbuf + 6) = ev->m_byYMean;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//          �������������
//////////////////////////////////////////////////////////////////////////
//��������Ŀ�������IP��ַ���������������������µ�IP��ַ����
void OnConfigNewip(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  int          IPAddress;
  BYTE        tmp1, tmp2, tmp3, tmp4;
  //char        macadd[256];

  //if(ev->DoModal()==IDOK)
  {
    tmp1 = ev->m_byIPAdd1;
    tmp2 = ev->m_byIPAdd2;
    tmp3 = ev->m_byIPAdd3;
    tmp4 = ev->m_byIPAdd4;
    IPAddress =  tmp1;
    IPAddress |=  tmp2 << 8;
    IPAddress |=  tmp3 << 16;
    IPAddress |=  tmp4 << 24;
    //����Ŀ���������IP��ַ
    cmdbuf[0] = 0x1a;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x04;
    *(UINT*)(cmdbuf + 6) = IPAddress;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//��������Ŀ�������MAC��ַ���������������������µ�MAC��ַ����
void OnConfigNewmac(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  char        macadd[256];
  int          i, j;

  //if(m_MACCfgDlg.DoModal()==IDOK)
  {
    //Set new MAC Address
    //macadd = m_MACCfgDlg.m_strMacAdd4 + m_MACCfgDlg.m_strMacAdd5 + m_MACCfgDlg.m_strMacAdd6;
    i = strlen(macadd);

    if (i != 6) {
      if (i == 0) {
        return;
      }

      AfxMessageBox("MAC address is invaliable!");
      return;
    }

    for (i = 0; i < 3; i++) {
      unsigned char tmp[2];

      for (j = 0; j < 2; j++) {
        if ((macadd[i * 2 + j] >= 48 && macadd[i * 2 + j] <= 57) || (macadd[i * 2 + j] >= 65 && macadd[i * 2 + j] <= 70) || (macadd[i * 2 + j] >= 97 && macadd[i * 2 + j] <= 102)) {
          if (macadd[i * 2 + j] >= 48 && macadd[i * 2 + j] <= 57) {
            tmp[j] = macadd[i * 2 + j] - 48;
          }

          if (macadd[i * 2 + j] >= 65 && macadd[i * 2 + j] <= 70) {
            tmp[j] = macadd[i * 2 + j] - 55;
          }

          if (macadd[i * 2 + j] >= 97 && macadd[i * 2 + j] <= 102) {
            tmp[j] = macadd[i * 2 + j] - 87;
          }
        }
        else {
          AfxMessageBox("MAC address is invaliable!");
          return;
        }
      }

      cmdbuf[i + 9] = (tmp[0] * 16 + tmp[1]);
    }

    //���������޸�Ŀ�������MAC��ַ
    cmdbuf[0] = 0x1b;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x06;
    cmdbuf[6] = 0x08;
    cmdbuf[7] = 0x00;
    cmdbuf[8] = 0x28;  //MAC code for TI

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//CCD�źŵ��������(0 - 35.5dB �ɵ�)
void OnConfigAfegainctrl(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(m_AfeConfigDlg.DoModal()==IDOK)
  {
    //���ô���������źŵ�����
    cmdbuf[0] = 0x07;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x04;
    *(float*)(cmdbuf + 6) = ev->m_fAfeVGAGain;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//��ƽ�����ѡ��
void OnConfigWhitebalance(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    //����Ŀ�������ƽ��У���Ĺ�������
    cmdbuf[0] = 0x08;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x01;
    *(UINT*)(cmdbuf + 6) = *(UINT*)(&ev->m_uiWBType);

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//�ɱ��IO�ں������ͬ�����ʹ�ܿ���
void OnConfigGioconfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {
    //����Ŀ�����ʮ���ɱ��I/O�ķ���
    cmdbuf[0] = 0x0d;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x02;
    *(UINT*)(cmdbuf + 6) = *(UINT*)(&ev->m_pioIndicator) & 0x3ff;
    ev->m_bIsPortDirAdj = FALSE;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }

    //����Ŀ�����ʮ���ɱ��I/O�������ƽ�������������Ч
    cmdbuf[0] = 0x0e;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x02;
    *(UINT*)(cmdbuf + 6) = (*(UINT*)(&ev->m_pioIndicator) & 0xffc00) >> 10;
    ev->m_bIsPortOutAdj = FALSE;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }

    //����Ŀ������������ͬ������Ƿ�ʹ��
    cmdbuf[0] = 0x0c;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x01;

    if (ev->m_bIsFlashSynEnable) {
      *(UINT*)(cmdbuf + 6) = 1; //ʹ��
    }
    else {
      *(UINT*)(cmdbuf + 6) = 0; //��ֹ
    }

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//����ڲ���ʵʱʱ������
void OnConfigRtcconfig(Eagleeye2_RMVA2xxSC_Viewer* ev, RTCConfig* rtc)
{
  //if(rtc->DoModal()==IDOK)
  {
    //����Ŀ�������RTCʱ��
    cmdbuf[0] = 0x18;
    cmdbuf[1] = 0x00;//����Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0x07;
    cmdbuf[6] = rtc->m_iYear % 100;
    cmdbuf[7] = rtc->m_iMonth;
    cmdbuf[8] = rtc->m_iDay;
    cmdbuf[9] = rtc->m_iWeek;
    cmdbuf[10] = rtc->m_iHour;
    cmdbuf[11] = rtc->m_iMinute;
    cmdbuf[12] = rtc->m_iSecond;

    if (!SendCMDtoDSP(ev, cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }

    //��ȡĿ�������RTCʱ��
    cmdbuf[0] = 0x19;
    cmdbuf[1] = 0x01;//��Ҫ����ش�����
    *(UINT*)(cmdbuf + 2) = 0;

    if (!SendCMDtoDSP(ev,  cmdbuf)) {
      AfxMessageBox("�������������ʧ��");
    }
  }
}

//����Ŀ��Ź�����
void OnConfigWdtconfig(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //if(ev->DoModal()==IDOK)
  {

    if (ev->m_bIsWDTEnable) {
      //ʹ��Ŀ������Ŀ��Ź�
      cmdbuf[0] = 0x16;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = 0x01;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }

      //����Ŀ������Ŀ��Ź���ʱ��(1~256��)
      cmdbuf[0] = 0x17;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = ev->m_iWDT;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
    else {
      //��ֹĿ������Ŀ��Ź�
      cmdbuf[0] = 0x16;
      cmdbuf[1] = 0x00;//����Ҫ����ش�����
      *(UINT*)(cmdbuf + 2) = 0x01;
      cmdbuf[6] = 0x00;

      if (!SendCMDtoDSP(ev, cmdbuf)) {
        AfxMessageBox("�������������ʧ��");
      }
    }
  }
}

//�������ץȡһ����ǰͼ���������Ӧ��ƽ��У��
void OnOpeAutowb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�������ץȡһ����ǰͼ���������Ӧ��ƽ��У��
  cmdbuf[0] = 0x09;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�����������������ģʽ�л�������ģʽ���Ƿ��Զ�����ץȡһ��ͼ��
void OnOpeAutotrgEnable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //ʹ���л����Զ���������
  ev->m_bAutoTrgEnable = TRUE;
  cmdbuf[0] = 0x1f;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
void OnOpeAutotrgDisable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //��ֹ�л����Զ���������
  ev->m_bAutoTrgEnable = FALSE;
  cmdbuf[0] = 0x1f;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�Ƿ�ʹ��ͼ��LUT�任����
void OnOpeLutEnable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //ʹ��LUT�任����
  ev->m_bLUTEnable = TRUE;
  cmdbuf[0] = 0x0a;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0x01;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
void OnOpeLutDisable(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //��ֹLUT�任����
  ev->m_bLUTEnable = FALSE;
  cmdbuf[0] = 0x0a;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x02;
  cmdbuf[6] = 0x00;
  cmdbuf[7] = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�̵�������
void OnOpeRelayOpen(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�Ͽ��̵���
  ev->m_bRelayIsOpen = TRUE;
  cmdbuf[0] = 0x1d;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
void OnOpeRelayClose(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�պϼ̵���
  ev->m_bRelayIsOpen = FALSE;
  cmdbuf[0] = 0x1d;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�������ͼ��ɼ���ʽΪRGB����
void OnOpeImgformatRgb(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�������ͼ��ɼ���ʽΪRGB����
  ev->m_bIsRGBArray = TRUE;
  ev->m_bIsYUVArray = FALSE;
  ev->m_bIsBAYERArray = FALSE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�������ͼ��ɼ���ʽΪYUV����
void OnOpeImgformatYuv(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�������ͼ��ɼ���ʽΪYUV����
  ev->m_bIsRGBArray = FALSE;
  ev->m_bIsYUVArray = TRUE;
  ev->m_bIsBAYERArray = FALSE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//�������ͼ��ɼ���ʽΪBAYER����
void OnOpeImgformatBayer(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //�������ͼ��ɼ���ʽΪYUV����
  ev->m_bIsRGBArray = FALSE;
  ev->m_bIsYUVArray = FALSE;
  ev->m_bIsBAYERArray = TRUE;
  cmdbuf[0] = 0x20;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 2;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//���ô���ģʽ�Ŀ����ٶȵ��ֶ�ģʽ
void OnOpeTrgshutterctrlManual(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bTrgShutterIsManual = TRUE;
  cmdbuf[0] = 0x22;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 0;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//���ô���ģʽ�Ŀ����ٶȵ��Զ�ģʽ
void OnOpeTrgshutterctrlAuto(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  ev->m_bTrgShutterIsManual = FALSE;
  cmdbuf[0] = 0x22;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x01;
  *(UINT*)(cmdbuf + 6) = 1;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//��λ���
void OnOpeResetsys(Eagleeye2_RMVA2xxSC_Viewer* ev)
{
  //Զ������Ŀ�����
  cmdbuf[0] = 0x00;
  cmdbuf[1] = 0x00;//����Ҫ����ش�����
  *(UINT*)(cmdbuf + 2) = 0x00;

  if (!SendCMDtoDSP(ev, cmdbuf)) {
    AfxMessageBox("�������������ʧ��");
  }
}
//Զ�̸����û�����
void OnOpeRemoteupdatedspprrom()
{
  char CmdLine[] = "EagleeyeDSPRemoteUpdate";
  UINT RES = WinExec(CmdLine, SW_SHOW);
}
