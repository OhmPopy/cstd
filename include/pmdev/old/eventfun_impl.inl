{

  int i;
  BOOL bRet = FALSE;

  switch (eResult) {
  case OPER_ERR_BUSY:
    PRINTF("�豸%s:�豸æ��\n", sIP);
    return TRUE;//�豸æ

  case OPER_ERR_LIMIT:
    PRINTF("�豸%s:�Ѵ����ޡ�\n", sIP);
    return TRUE;//�Ѵ�����

  case OPER_ERR_NOFIND: //û���ҵ���Ӧ����
    PRINTF("�豸%s:û���ҵ���Ӧ����\n", sIP);
    return TRUE;

  case OPER_ERR_SAVEFAIL:
    PRINTF("�豸%s:���ݱ���ʧ��\n", sIP);
    return TRUE;//���ݱ���ʧ��

  case OPER_ERR_SETFAIL:
    PRINTF("�豸%s:����ʧ��\n", sIP);
    return TRUE;//����ʧ��

  case OPER_ERR_FROMAT:
    PRINTF("�豸%s:��ʽ��ʧ��\n", sIP);
    return TRUE;//��ʽ��ʧ��

  case OPER_ERR_PARAM:
    PRINTF("�豸%s:��������\n", sIP);
    return TRUE; //��������

  case OPER_ERR_DISABLE:
    PRINTF("�豸%s:Ҫ��ִ�еĹ���û��ʹ��\n", sIP);
    return TRUE; //

  case OPER_ERR_EXECUT:
    PRINTF("�豸%s:ִ��ʧ��\n", sIP);
    return TRUE; //ʧ��

  case OPER_ERR_SUPPORT:
    PRINTF("�豸%s:��֧�ֵ�����\n", sIP);
    return TRUE; //��֧�ֵ�����

  case OPER_ERR_INPUTDATA:
    PRINTF("�豸%s:����˴�����������쳣\n", sIP);
    return TRUE; //����˴�����������쳣

  case OPER_BATCH_DL_COMPLETE:
    PRINTF("�豸%s:�����������\n", sIP);
    break;

  case OPER_SUCCESS:
    break;

  default:
    break;
  }

  //Ȩ����֤��

  switch (eType) {
  case DEV_CONNECT_SUCCESS:
    PRINTF("���豸%s���ӳɹ���\n", sIP);
    break;

  case DEV_CONNECT_FAILUE:
    PRINTF("���豸%s����ʧ�ܡ�\n", sIP);
    break;

  case DEV_CONNECT_CUT:
    PRINTF("���豸%s�Ͽ����ӡ�\n", sIP);

    //Connect(pv);
    switch (eResult) {
    case -3:
      PRINTF("err = %d У�����\n", eResult);
      break;

    case -2:
      PRINTF("err = %d recv��ʱ\n", eResult);
      break;
    }

    if (0) {
      closesocket(pv->m_sHost);
      pv->m_sHost = 0;
      pv->m_exitThread = 1;
    }

    break;

  case DEV_AUTH_OPER:
    if (0 == eResult) {
      PRINTF("�豸%s:Ȩ����֤ͨ����\n", sIP);

#if 0
      PRINTF("�豸%s:��������ʶ���¼...\n", sIP);
      bRet = CPM_DLAllIdentifyRecord(pv);

      PRINTF("�豸%s:���������û�...\n", sIP);
      //bRet = CPM_DLAllUser(pv);

      PRINTF("�豸%s:��ȡ�豸�������...\n", sIP);
      bRet = CPM_NetPro(pv, NULL);

      PRINTF("�豸%s:��ȡ������Ϣ...\n", sIP);
      bRet = CPM_DLDevBaseInfo(pv);
      PRINTF("�豸%s:��ȡͳ����Ϣ...\n", sIP);
      bRet = CPM_DLDevStatisInfo(pv);
      PRINTF("�豸%s:��ȡ������Ϣ...\n", sIP);
      bRet = CPM_DLDevWorkAttInfo(pv);
      PRINTF("�豸%s:��ȡ�豸ʱ��...\n", sIP);
      bRet = CPM_DLDevTime(pv);

#endif
    }

    break;

  case DEV_BASEINFO_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_BASEINFO* dev_baseinfo = (DEV_BASEINFO*)pvContent;
      PRINTF("�豸%s:��ȡ������Ϣ�ɹ�\n", sIP);
      PRINTF("<< �豸������Ϣ \n");
      PRINTF("�豸���� %d\n", dev_baseinfo->m_DevType);
      PRINTF("��ʹ���������� %d\n", dev_baseinfo->m_LimitUser);
      PRINTF("1N�û����� %d\n", dev_baseinfo->m_Limit1NUser);
      PRINTF("�豸��� %s\n", dev_baseinfo->m_SN.m_ID);
      PRINTF("DSP����汾 %s\n", dev_baseinfo->m_Ver.m_ID);
      PRINTF("����������Ϣ %s\n", dev_baseinfo->m_Space.m_ID);
      PRINTF(">>\n");
    }

    break;

  case DEV_STATISINFO_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_STATIS* dev_statis = (DEV_STATIS*)pvContent;
      PRINTF("�豸%s:��ȡͳ����Ϣ�ɹ�\n", sIP);
      PRINTF("<< �豸ͳ����Ϣ \n");
      PRINTF("��ǰ�豸�����û��� %d\n", dev_statis->m_TotalUsers);
      PRINTF("û�вɼ������������û��� %d\n", dev_statis->m_NoFeatUser);
      PRINTF("ʶ���¼���������� %d\n", dev_statis->m_TotalDays);
      PRINTF("�ܼ�¼�� %d\n", dev_statis->m_TotalRecords);
      PRINTF("�ܱ�����¼�� %d\n", dev_statis->m_TotalAlarm);
      PRINTF("�ܲ�����־��Ŀ %d\n", dev_statis->m_TotalDspLog);
      PRINTF("�ܱ��ע���� %d\n", dev_statis->m_TotalChangeImage);
      PRINTF(">>\n");
    }

    break;

  case DEV_WORKATT_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_WORKATT* dev_workatt = (DEV_WORKATT*) pvContent;
      PRINTF("�豸%s:��ȡ�豸������Ϣ�ɹ�\n", sIP);
      PRINTF("<< �豸������Ϣ \n");
      PRINTF("�μ��豸���ܱ�Ƕ��� %d\n", dev_workatt->m_BaseSet);
      PRINTF("�Ŵ���ʱʱ�䣬��λ�� %d\n", dev_workatt->m_DoorMangetTime);
      PRINTF("��������ʱ�䣬��λ�� %d\n", dev_workatt->m_LockTime);
      PRINTF("�û������֤ģʽ %d\n", dev_workatt->m_VerifyMode);
      PRINTF("Τ��Э������ %d\n", dev_workatt->m_nWGType);
      PRINTF("Τ��������� %d\n", dev_workatt->m_nWGOutType);
      PRINTF("���ID���� %d\n", dev_workatt->m_nWGOutIDType);
      PRINTF("WG������� %d\n", dev_workatt->m_nWGOutContent);
      PRINTF("�Ƿ�WG��� %d\n", dev_workatt->m_bWGOutPut);
      PRINTF("�������� %s\n", dev_workatt->m_szSuperPWD.m_ID);
      PRINTF("�豸Ĭ��Ȩ�� %d\n", dev_workatt->m_DefaultRight);
      PRINTF(">>\n");

      if (0) {
        PRINTF("�豸%s:���ù�����Ϣ\n", sIP);
        bRet = CPM_ULDevWorkAttInfo(pv, dev_workatt);
      }
    }

    break;

  case DEV_SYS_TIME_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DATETIME* dev_datetime = (DEV_DATETIME*) pvContent;
      PRINTF("�豸%s:��ȡ�豸ʱ��ɹ�\n", sIP);
      PRINTF("<< �豸���ں�ʱ�� \n");
      PRINTF("%d-%d-%d %d:%d:%d:%d\n"
          , dev_datetime->m_Date.m_Year
          , dev_datetime->m_Date.m_Month
          , dev_datetime->m_Date.m_Day
          , dev_datetime->m_Time.m_Hour
          , dev_datetime->m_Time.m_Minute
          , dev_datetime->m_Time.m_Second
          , dev_datetime->m_Time.m_Msec
            );
      PRINTF(">>\n");

      if (0) {
        PRINTF("�豸%s:�����豸ʱ��\n", sIP);
        bRet = CPM_ULDevTime(pv, dev_datetime);
      }
    }

    break;

  case DEV_USER_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_USER* dev_user = (DEV_USER*) pvContent;
      PRINTF("�豸%s:��ȡ�û��ɹ�\n", sIP);
      PRINTF("�豸%s:�ϴ��û�:%s\n", sIP, dev_user->m_UserID.m_ID);
      bRet = CPM_ULUser(pv, dev_user);

      PRINTF("�豸%s:Զ���û������ɼ�:%s\n", sIP, dev_user->m_UserID.m_ID);
      bRet = CPM_ULRealTimeUserCap(pv, dev_user);
    }

    break;

  case DEV_USER_EX_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag && OPER_BATCH_DL_COMPLETE == eResult) {
      DEV_BATCH_USER* pUserBatch = (DEV_BATCH_USER*) pvContent;

      //if (pUserBatch->m_nCount>0)
      if (0) {
        int i;
        //PRINTF("�豸%s:���������û��ɹ�\n", sIP);
        PRINTF("m_nCount = %d\n", pUserBatch->m_nCount);

        for (i = 0; i < pUserBatch->m_nCount; ++i) {
          PRINTF("m_ID = %s\n", pUserBatch->m_pUserArray[i].m_UserID.m_ID);
        }
      }

      //PRINTF("�豸%s:�����ϴ��û�....", sIP);
      //bRet = CPM_ULUserEx(pv, pUserBatch, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
    }

    break;

  case DEV_USER_RANGE_OPER:
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DL_USER_RANGE* dev_dl_user_range = (DEV_DL_USER_RANGE*) pvContent;
      int nCount = dev_dl_user_range->m_Count;
      PRINTF("�豸%s:��ȡ�û��������صĴ�����ɹ�\n", sIP);

      if (nCount > 0) {
        enum { maxnn = 190 };
        DEV_CID arrID[maxnn] = {0};
        nCount = MIN(maxnn, nCount);
        PRINTF("�豸%s:�Ѵ����仮�ֳɼ���С���������û�\n", sIP);

        for (i = 0; i < nCount; ++i) {
          strcpy(arrID[i].m_ID, dev_dl_user_range->m_pArray[i].m_UserID.m_ID);
        }

        bRet = CPM_DLUserEx(pv, arrID, nCount, DL_DEV_PIC | DL_DEV_USER_FEAT | DL_DEV_USER_TEXT);
      }
    }

    break;

  case DEV_USER_RT_OPER: { //Զ�̼���
    DEV_USER_REAL_LOAD* pDevUserRealLoad = (DEV_USER_REAL_LOAD*) pvContent;
    DEV_USER_REAL_LOAD stDevUserRealLoad;
    memset(&stDevUserRealLoad, 0, sizeof(DEV_USER_REAL_LOAD));
    memcpy(&stDevUserRealLoad, pDevUserRealLoad, sizeof(DEV_USER_REAL_LOAD));
    //PRINTF("�豸%s:ʵʱ�û�����:%s\n", sIP, m_TempUser.m_UserID.m_ID);
    //bRet = CPM_ULRealTimeUser(pv, m_TempUser, stDevUserRealLoad);
  }
  break;

  case DEV_RECORD_RANGE_OPER:
  case DEV_ALARM_RANGE_OPER: {
    //��ȡ��¼�ĸ��������䣬����������Ϊ��λ��
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_DL_RECORD_ARRAY* dev_dl_record_array = (DEV_DL_RECORD_ARRAY*) pvContent;
      PRINTF("�豸%s:��ȡ��¼�������صĸ���������ɹ� m_Count = %d\n", sIP, dev_dl_record_array->m_Count);

      for (i = 0; i < dev_dl_record_array->m_Count; i++) {
        //DEV_DL_RECORD_RANGE stDevRange[1] = {0};
        //��ÿ����������в�֣���ֳ�ÿ50����¼��С���С����,û��С����ĵ�һ����¼��ID������һ��С��������һ����¼ID����˴������µ�С������봮�����ء�����ֻһ�����ش������ǰ50����
        //int nSBatchCount = dev_dl_user_range->m_Count / 50;
        //int nLastCount = dev_dl_user_range->m_Count % 50;
        //*stDevRange = dev_dl_user_range->m_pRange;
        //stDevRange->m_Count = stDevRange.m_Count > 50 ? 50 : stDevRange.m_Count;
        PRINTF("%d %s\n", dev_dl_record_array->m_pRange[i].m_Count, dev_dl_record_array->m_pRange[i].m_SID.m_ID);
        bRet = CPM_DLRecEx(pv, dev_dl_record_array->m_pRange + i);
      }
    }
  }
  break;

  case DEV_RECORD_OPER:
  case DEV_ALARM_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_RECORD* pRecord = (DEV_RECORD*) pvContent;
      PRINTF("�豸%s:���ؼ�¼%s\n", sIP, pRecord->m_SerialID.m_ID);
    }
  }
  break;

  case DEV_RECORD_EX_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) {
      DEV_BATCH_RECORD* pBatchRecord = (DEV_BATCH_RECORD*) pvContent;
      PRINTF("�豸%s:�������ؼ�¼%d���ɹ�\n", sIP, pBatchRecord->m_nCount);
    }
  }
  break;

  case DEV_USER_REMOTE_CAP: { //Զ�̲ɼ����
    if (eFlag == DEV_OPER_UPLOAD) { //
      DEV_USER* dev_user = (DEV_USER*) pvContent;
      PRINTF("�豸%s:Զ�̲ɼ��û�%s�ɹ�\n", sIP, dev_user->m_UserID.m_ID);
    }
  }
  break;

  case DEV_NOPEN_OPER: {
    if (DEV_OPER_DOWNLOAD == eFlag) { //
      DEV_NETPRO* pNOPEN = (DEV_NETPRO*) pvContent;
      PRINTF("�豸%s:��ȡ������Ϣ�ɹ�", sIP);
      PRINTF("�豸%s:����������Ϣ...", sIP);
      bRet = CPM_NetPro(pv, pNOPEN);
    }
  }
  break;

  default:
    break;
  }
}

#undef PRINTF