CString GetfolderPath()  
{  
  CString folderPath;  
  BROWSEINFO bi;  
  char buf[MAX_PATH];  
  
  //��ʼ����ڲ���bi  
  bi.hwndOwner = NULL;  
  bi.pidlRoot = NULL;  
  bi.pszDisplayName = buf;    //�˲�����ΪNULL������ʾ�Ի���  
  bi.lpszTitle = "ѡ��·��";  
  bi.ulFlags = BIF_RETURNONLYFSDIRS;  
  bi.lpfn = NULL;  
  bi.iImage = 0;  
  
  LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);  //������ʾѡ��Ի���  
  if(pIDList)  
  {  
    SHGetPathFromIDList(pIDList,buf);           //ȡ���ļ���·����buf��  
    folderPath = buf;       //��·��������һ��CString������  
  }  
  folderPath += ("\\");  
  
  return folderPath;  
}  