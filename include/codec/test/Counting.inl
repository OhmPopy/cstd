
// CountingDlg.cpp : implementation file
//

#include <tchar.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include "findfile.h"
#include "fileio.h"
#include "macro.h"

//typedef int BOOL;

// ����������ע����Ϊͬһ��ʱ��ͳ��Ϊ��
// ������1�С�ע����0��
// ������1�С�ע����1��
// ������0�С�ע����1��

#define     IDS_ABOUTBOX        _T("����Դ��ͳ����(&A)...")
#define     IDS_ABORT           _T("Դ��ͳ�Ʊ��û�ȡ��")
#define     IDS_MESSAGE         _T("Դ��ͳ������Ϣ")
#define     IDS_INVALIDFILE     _T("��ȷ���ļ�����ȷ��")
#define     IDS_COUNTING_PATH   _T("������·������")
#define     IDS_WRONGTYPE       _T("�ļ����ʹ���")
#define     IDS_FILENAME        _T("�ļ���")
#define     IDS_PATHNAME        _T("�����ļ���")
#define     IDS_TOTALLINE       _T("������")
#define     IDS_CODELINE        _T("������")
#define     IDS_COMMENTLINE     _T("ע����")
#define     IDS_INPUT_FILE      _T("��������Ҫͳ�Ƶ��ļ������ļ���չ����")
#define     IDS_BLANKLINE       _T("�հ���")
#define     IDS_FILETYPE        _T("����")
#define     IDS_COUNTINGFILE    _T("����ͳ��")
#define     IDS_OPENFAILURE     _T("�ļ���ʧ�ܡ����ܼ��±���������")
#define     IDS_COUNTINGSTOP    _T("Դ��ͳ����ϣ�")
#define     IDS_TOQUIT          _T("��Ҫ�˳�Դ��ͳ������")
#define     IDS_TOTALFILES      _T("���ļ���")
#define     IDS_TOTAL           _T("�ܼ�")
#define     IDS_UNSTARTED       _T("��δ����Դ��ͳ�ƣ�")
#define     IDS_TOTALSIZE       _T("�ܴ�С")
#define     IDS_COUNT           _T("ͳ��")
#define     IDS_STOP            _T("ֹͣ")
#define     IDS_SAVETYPE        _T("�ı��ļ�(*.txt)|*.txt|Excel �ļ�(*.xls)|*.xls|CSV�ļ�(*.csv)|*.csv||")
#define     IDS_WAITING         _T("�ļ�ͳ���У����Ժ򱣴档")
#define     IDS_SAVEOVER        _T("�������")

#define MAX_PATH 260
typedef TCHAR STRPATH[MAX_PATH];

#define SETWINDOWTEXT(_STR)          _tprintf("%s\r", _STR)

#define IsOdd(nQuatos) ((nQuatos)&1)

//#define MESSAGEBOX(_STR) MessageBox(NULL, _STR, IDS_MESSAGE, NULL)
#define MESSAGEBOX(_STR) _tprintf(_STR)

#if 0
#include "strex.h"
#else
int StrTrimLeft(TCHAR* pchData) {
  // find first non-space TCHARacter
  int nNewLength = 0;
  const TCHAR* lpsz = pchData;

  while(_istspace(*lpsz)) {
    // lpsz = _tcsinc(lpsz);
    ++lpsz;
  }

  if (lpsz != pchData) {
    // fix up data and length
    int nDataLength = _tcslen(pchData) - (lpsz - pchData);
    memmove(pchData, lpsz, (nDataLength+1)*sizeof(TCHAR));
    nNewLength = nDataLength;
  }
  return nNewLength;
}

int StrTrimRight(TCHAR* pchData) {
  // find beginning of trailing spaces by starting at beginning (DBCS aware)
  int nNewLength = 0;
  TCHAR* lpsz = pchData;
  TCHAR* lpszLast = NULL;

  while(*lpsz != '\0') {
    if (_istspace(*lpsz)) {
      if (lpszLast == NULL)
        lpszLast = lpsz;
    } else
      lpszLast = NULL;
    //lpsz = _tcsinc(lpsz);
    ++lpsz;
  }

  if (lpszLast != NULL) {
    // truncate at trailing space start
    *lpszLast = '\0';
    nNewLength = lpszLast - pchData;
  }
  return nNewLength;
}

#endif

enum EXTENSION_TYPE {
  TYPE_OTH,
  TYPE_CPP, //FOR extract icon, must map to IDB_FILETYPE
  TYPE_CXX,
  TYPE_H,
  TYPE_HPP, // the same value means the same icon
  TYPE_TXT,
  TYPE_DSW,
  TYPE_RC,
  TYPE_WRI,
  TYPE_HTM,
  TYPE_INI,
  TYPE_HPJ,
  TYPE_C,
  TYPE_JAVA,
  TYPE_VB,
  TYPE_ASP,
  TYPE_JSP,
  TYPE_SQL,
  TYPE_PERL,
  TYPE_SHELL,
};

typedef struct EXTENSION {
  int  nType; // file type defined as following
  const TCHAR* ext; // strings of extension
}
EXTENSION, *PEXTENSION;

static const EXTENSION m_structExtention[] = {
      TYPE_CPP, _T("cpp"),
      TYPE_C, _T("c"),
      TYPE_H, _T("h"),
      TYPE_TXT, _T("txt"),
      TYPE_WRI, _T("wri"),
      TYPE_H, _T("tli"),
      TYPE_H, _T("tlh"),
      TYPE_RC, _T("rc"),
      TYPE_DSW, _T("dsw"),
      TYPE_HPJ, _T("hpj"),
      TYPE_HTM, _T("htm"),
      TYPE_HTM, _T("html"),
      TYPE_CPP, _T("cxx"),
      TYPE_H, _T("hpp"),
      TYPE_OTH, _T("def"),
      TYPE_JAVA, _T("java"),
      TYPE_VB, _T("frm"),
      TYPE_VB, _T("bas"),
      TYPE_VB, _T("ctl"),
      TYPE_VB, _T("cls"),
      TYPE_ASP, _T("asp"),
      TYPE_JSP, _T("jsp"),
      TYPE_SQL, _T("sql"),
      TYPE_PERL, _T("pl"),
      TYPE_SHELL, _T("sh"),
      TYPE_OTH, _T("*"),
    };

int HowManyTCHARsInString(const TCHAR* str, const TCHAR* TCHARs, BOOL bCommentSet) {
  int nCount=0;
  //Ҫ����/*/������
  //  if (str _tcsstr(, "/*/")!=NULL)
  //  {
  //    CString strLeft = str _tcsnccpy(str _tcsstr(, "/*/"));
  //    if (strLeft _tcsstr(, "*/")==-1 && TCHARs == "/*" && bCommentSet)
  //      nCount--;
  //    if (strLeft _tcsstr(, "/*/")==-1 && TCHARs == "*/" && !bCommentSet)
  //      nCount--;
  //  }

  int TCHARsLen = _tcslen(TCHARs);
  for (; str = _tcsstr(str, TCHARs); nCount++) {
    str += TCHARsLen;
  }

  return MAX(nCount, 0);
}

/*
  �Ƚ�strFile�Ƿ�ΪinputFile���ļ�����
*/
BOOL CompareMarkString(const TCHAR* inputFile, const TCHAR* strFile0) {
  const TCHAR* strFile;
  int nLength1;
  int nLength2;
  int i, j;
  strFile = _tcsrchr(strFile0, '\\');
  strFile = strFile ? strFile+1 : strFile0;
  nLength1 = _tcslen(strFile);
  nLength2 = _tcslen(inputFile);

  for(i=0, j=0; i<nLength1 && j<nLength2; i++, j++) {
    if (inputFile[j]=='*') {
      j++;
      if (j==nLength2)
        return TRUE;
      while(j<nLength2&&inputFile[j]=='?') {
        i++;
        j++;
      }
      if (j>=nLength2)
        return TRUE;

      while(i<nLength1&&inputFile[j]!=strFile[i])
        i++;
      if (i>=nLength1)
        return FALSE;

      continue;
    }
    if (inputFile[j]=='?') {
      continue;
    }
    if (inputFile[j]==strFile[i]) {
      continue;
    } else
      return FALSE;
  }

  if (i==nLength1&&j==nLength2)
    return TRUE;
  else
    return FALSE;
}

/*
  strFileName �Ƿ�Ϊ����Ҫͳ�Ƶ��ļ�����
*/
BOOL IsSearchingFor(const TCHAR* strFileName0, const TCHAR* strExts0) {
  int i=0, cnt = 0;
  TCHAR strFileName[256];
  TCHAR strExts[256];
  TCHAR* strOneOfExts; // get one extension in strExts
  TCHAR* strExts1;
  TCHAR* strExtsEnd;
  _tcscpy(strFileName, strFileName0);
  _tcscpy(strExts, strExts0);
  _tcslwr(strFileName);
  _tcslwr(strExts);
  strExts1 = strExts;
  strExtsEnd = strExts1 + _tcslen(strExts);
  for (; strOneOfExts = _tcstok(strExts1, _T(";, : ")); ++cnt) {
    strOneOfExts = _tcstok(strExts1, _T(";, : "));
    strExts1 += _tcslen(strExts1);
    while(strExts1 != strExtsEnd && '\0'==*strExts1++)
      ;
  }

  strExts1 = strExts;
  for (i=0; i<cnt; ++i) {
    strOneOfExts = strExts1;
    if (CompareMarkString(strOneOfExts, strFileName))
      return TRUE;
    strExts1 += _tcslen(strExts1);
    while('\0'==*strExts1++)
      ;
  }

  return FALSE;
}

BOOL m_bIncludeSubfolder = 1;
int m_nStatMethod;  //1.1���¼�
FILE* m_strLogFile; //1.1���¼�

int GetTxtFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetSqlFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetPerlFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetVBFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetCppFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetCppFileLines_1_0_5(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
int GetCppFileLines_1_0(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);

/*
* find file type according to its extension
* for example: strExtension = "xiaogi.c"
*  return: TYPE_C
*/
int FindFileType(const TCHAR* strFile) {
  int i;
  const TCHAR* extNoDot = _tcsrchr(strFile, '.');
  extNoDot = extNoDot ? extNoDot+1 : _T("*");

  for(i=0; i<sizeof(m_structExtention)/sizeof(m_structExtention[0]); i++) {
    if (_tcsicmp(extNoDot, m_structExtention[i].ext)==0)
      return m_structExtention[i].nType;
  }

  return -1;
}

// ��ʼͳ��
int CountLines(const TCHAR* m_strComboFolder, const TCHAR* m_strComboExt) {
  STRPATH  strPath;
  STRPATH  strFolder;
  STRPATH  listPaths[1000];
  STRPATH* stack_top=listPaths;
#define PUSH(_STR) _tcscpy(*stack_top++, _STR), assert((stack_top-listPaths)<1000)
#define POP(_STR)  _tcscpy(_STR, *(--stack_top)), assert(stack_top>=listPaths)
#define ISEMPTY()  (stack_top==listPaths)

  int m_nFiles;
  int m_nSize;
  int m_nTotalLines;
  int m_nCodeLines;
  int m_nCommentLines;
  int m_nBlankLines;
  int bWorking;
  findfile_t hFile;
  fileinfo_t finder;

  m_nTotalLines = 0;//initialize
  m_nCodeLines = 0;
  m_nCommentLines = 0;
  m_nBlankLines = 0;
  m_nFiles = 0;
  m_nSize = 0;

  _tprintf("%6s %6s %6s %6s %22s\n", IDS_TOTALLINE, IDS_CODELINE, IDS_COMMENTLINE, IDS_BLANKLINE, IDS_FILENAME);
#define _CNT_PRINTF_FMT "%6d %6d %6d %6d %s\n"

  _tcscpy(strPath, m_strComboFolder);
  if (m_strComboFolder[_tcslen(m_strComboFolder)-1]=='\\')
    _tcscat(strPath, _T("*.*"));
  else
    _tcscat(strPath, _T("\\*.*"));

  do {
    hFile = findfirst(finder, strPath);
    bWorking = !invalid(hFile);
    while(bWorking) {
      TCHAR* strFilePath;
      bWorking = findnext(finder, hFile);
      strFilePath = fileinfo_filename(finder);
      if (_tcscmp(strFilePath, _T("."))==0)
        continue;
      if (_tcscmp(strFilePath, _T(".."))==0)
        continue;

      if (is_directory(finder)) {
        if (m_bIncludeSubfolder) {
          // listPaths.AddHead(strFilePath);
          PUSH(strFilePath);
        }
        continue;
      } else {
        if (IsSearchingFor(strFilePath, m_strComboExt)) {
          int nType;
          int nLines=0;
          int nCodeLines=0;
          int nCommentLines=0;
          int nBlankLines=0;
          int nLength=0;
          TCHAR str1[100], str2[100];
          _tcscpy(str1, IDS_COUNTINGFILE);
          _sntprintf(str2, 100, _T("%s:%s"), str1, strFilePath);
          SETWINDOWTEXT(str2);

          nType = FindFileType(strFilePath);
          if (nType == -1)
            nType = TYPE_OTH;

          switch(nType) {
          case TYPE_C:
          case TYPE_CPP:
          case TYPE_H:
          case TYPE_JAVA:
            nLines = GetCppFileLines(strFilePath, &nLength, &nCodeLines, &nCommentLines, &nBlankLines);
            break;
          case TYPE_VB:
          case TYPE_ASP:
            nLines = GetVBFileLines(strFilePath, &nLength, &nCodeLines, &nCommentLines, &nBlankLines);
            break;
          case TYPE_SQL:
            //            nLines = pThis->GetSqlFileLines(strFilePath, &nLength, &nCodeLines, &nCommentLines, &nBlankLines);
            break;
          case TYPE_JSP:
            MESSAGEBOX("jsp�ļ�Ŀǰ�����ṩͳ��");
            break;
          case TYPE_PERL:
          case TYPE_SHELL:
            nLines = GetPerlFileLines(strFilePath, &nLength, &nCodeLines, &nCommentLines, &nBlankLines);
            break;
          default:
            nLines = GetTxtFileLines(strFilePath, &nLength, &nCodeLines, &nCommentLines, &nBlankLines);
            break;
          }

          //������1.0�Ժ�İ汾��ɾ����nCodeLines�ɺ�������
          //          nCodeLines = nLines - nCommentLines - nBlankLines;

          //int pos=_tcsrchr(strFilePath, '\\');

          _tprintf(_CNT_PRINTF_FMT, nLines, nCodeLines, nCommentLines, nBlankLines, strFilePath);

          //pThis->m_ctlResult.Update(pThis->m_nFiles);

          m_nFiles++;
          m_nSize += nLength;
          m_nTotalLines += nLines;
          m_nCodeLines += nCodeLines;
          m_nCommentLines += nCommentLines;
          m_nBlankLines += nBlankLines;
        }
      }
    }
    if (ISEMPTY())
      break;

    // _tcscpy(strFolder, listPaths.RemoveHead());
    POP(strFolder);
    //strPath = strFolder + "\\*.*";
    _tcscpy(strPath, strFolder);
    _tcscat(strPath, _T("\\*.*"));
    findclose(hFile);
  } while(1);

  _tprintf(_CNT_PRINTF_FMT, m_nTotalLines, m_nCodeLines, m_nCommentLines, m_nBlankLines, "");

  //pThis->m_ctrlCountingIcon.SendMessage(STM_SETICON, (WPARAM)hIcon, 0);
  return 0;
}

/*
  ͳ��c++�ļ�
*/
int GetCppFileLines(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines) {
  //there are two methods to count lines in files, by CStdioFile or CFile
  FILE* file;
  int i;
  int nLines = 0;
  int nCodeLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;
  __int64 nLength = 0;
  TCHAR bufRead[1024];
  int nLineCommentBegin = 0;

  BOOL bCommentSet = FALSE; //ע����ͳ�Ʊ�ʶ ��"/*"ʱTRUE, "*/"ʱFALSE
  BOOL bQuatoSet = FALSE;   //�ַ���ͳ�Ʊ�ʶ �״�һ����������"ʱTRUE, ��һ����������"ʱFALSE

  *pnLength = 0;
  *pnCodeLines = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;
  FILESIZE(file, nLength);
  *pnLength = (int)nLength;


  while(_fgetts(bufRead, 1024, file)!=NULL) {
    BOOL bStatedComment = FALSE;//������Ϊע�����Ƿ���ͳ�ƹ�
    BOOL bStatedCode = FALSE;   //������Ϊ�������Ƿ���ͳ�ƹ�
    BOOL bDoubleSplashFound = FALSE;
    BOOL bSplashStarFound = FALSE;

    nLines++;

    StrTrimLeft(bufRead); //�Ƚ��ļ�ͷ�Ŀո���Ʊ��ȥ��

    //Ϊ�հ���
    if (_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }

    if (bCommentSet && _tcsstr(bufRead, _T("*/"))==NULL) {
      nCommentLines++;
      continue;
    }

    if ( _tcschr(bufRead, _T('//'))==NULL
         && _tcsstr(bufRead, _T("/*"))==NULL
         && _tcsstr(bufRead, _T("*/"))==NULL) {//������и�������ע�ͷ�����Ҫ����ע�ͷ���Ҫ���Ǵ�����
      if (bCommentSet) {
        nCommentLines++;
        continue;
      } else {
        if (_tcschr(bufRead, '"')==NULL) {
          nCodeLines++;
          continue;
        }
      }
    }

    if (bufRead[0]==_T('//') && !bCommentSet && !bQuatoSet) {
      nCommentLines++;
      continue;
    }

    for(i=0; i<(int)_tcslen(bufRead)-1; i++) {
      TCHAR cTemp = bufRead[i];
      if (bufRead[i]=='/' && bufRead[i+1]=='/' && !bCommentSet && !bQuatoSet) {
        if (!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2)) {
          bStatedComment = TRUE;
          nCommentLines++;
        }
        bDoubleSplashFound = TRUE;
        //i++;//Ӧ��+1����Ҳû��ʲô�ô�
        break;
      } else if (bufRead[i]=='/' && bufRead[i+1]=='*' && !bCommentSet && !bQuatoSet) {
        if (!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2)) {
          bStatedComment = TRUE;
          nCommentLines++;
        }
        bCommentSet = TRUE;
        bSplashStarFound = TRUE;
        i++;
      }
      //��������б�����bCommentSet�ر�֮ǰ
      else if (bufRead[i]!=' ' && bufRead[i]!='\t' && !bCommentSet) {
        if (!bStatedCode) {
          bStatedCode = TRUE;
          nCodeLines++;
        }
        if (bufRead[i]=='\\') {//\֮����ַ�Ҫ����
          i++;
          continue;
        }
        if (bufRead[i]=='\'') {
          if (bufRead[i+1]=='\\')
            i+=2;
          else
            i+=1;
          continue;
        }
        if (bufRead[i]=='"') {//"�����������ӣ���лltzhou
          bQuatoSet = !bQuatoSet;
        }
      } else if (bufRead[i]=='*' && bufRead[i+1]=='/' && bCommentSet && !bQuatoSet) {
        if (!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2)) {
          bStatedComment = TRUE;
          nCommentLines++;
        }
        bCommentSet = FALSE;
        bSplashStarFound = TRUE;
        i++;
      }
    }

    if (bDoubleSplashFound) {
      //���ͳ�Ʒ���Ϊ�����֣���ͬʱ�д�������ע���У���ֻ��ע����
      if (m_nStatMethod==2 && bStatedCode) {
        nCodeLines--;
      }
      //���ͳ�Ʒ���Ϊ��һ�֣���δ��Ϊ������ͳ�ƹ�����ô��Ϊע����
      if (m_nStatMethod==0 && !bStatedCode) {
        nCommentLines++;
      }
      continue;
    }

    //��ĳ�����һ����"����ض������ر�bQuatoSet���Ǵ�����һ�У����򱨴�
    if (bufRead[(int)_tcslen(bufRead)-1]=='"'&&!bCommentSet) {
      bQuatoSet = !bQuatoSet;
      if (!bQuatoSet) {
        if (!bStatedCode) {
          bStatedCode = TRUE;
          nCodeLines++;
        }
      } else {
        if (m_strLogFile) {
          _ftprintf(m_strLogFile, "%s\t%d\t�ַ�������δ��\\\n", strFileName, nLines);
        }
      }
      continue;
    }

    //������һ���ַ��ǿո���Ʊ������ǰ����/*����������ַ�����*/����Ϊ������
    if (bufRead[(int)_tcslen(bufRead)-1]!=' ' && bufRead[(int)_tcslen(bufRead)-1]!='\t' && !bCommentSet
        && bufRead[(int)_tcslen(bufRead)-2]!='*' && bufRead[(int)_tcslen(bufRead)-1]!='/') {
      if (!bStatedCode) {
        bStatedCode = TRUE;
        nCodeLines++;
      }
    }

    if (bSplashStarFound) {
      //���ͳ�Ʒ���Ϊ�����֣���ͬʱ�д�������ע���У���ֻ��ע����
      if (m_nStatMethod==2 && bStatedCode) {
        nCodeLines--;
      }

      //�������޴�����    /*abc*/ //222
      //����ͳ�Ʒ����ǵ�һ�֣�����Ҫ׷��ע���м���һ��
      if (m_nStatMethod==0 && !bStatedCode && !bStatedComment) {
        bStatedComment = TRUE;
        nCommentLines++;
      }
    }

    //������ǰ����/*���ڵ�һ��ͳ�Ʒ����У�δ��Ϊ�����м��������ô���п϶���ע����
    if (!bStatedComment && bCommentSet) {
      if (m_nStatMethod==0 && !bStatedCode) {
        bStatedComment = TRUE;
        nCommentLines++;
      }
    }

    //    if (bQuatoSet && bufRead[(int)_tcslen(bufRead)-1]=='"')
    //    {
    //      bQuatoSet = FALSE;
    //    }

    if (bQuatoSet && bufRead[(int)_tcslen(bufRead)-1]!='\\') {
      if (m_strLogFile) {
        _ftprintf(m_strLogFile, "%s\t%d\t�ַ�������δ��\\\n", strFileName, nLines);
      }
    }

  }

  *pnCodeLines = nCodeLines;
  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);
  return nLines;
}

/*
  ͳ��c++�ļ�
*/
int GetCppFileLines_1_0_5(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines) {
  //there are two methods to count lines in files, by CStdioFile or CFile
  FILE* file;
  int nLines = 0;
  int nCodeLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;
  int nEndComment;
  __int64 nLength = 0;
  TCHAR bufRead[1024];
  int nLineCommentBegin = 0;
  int nDoubleSplashFoundAt;

  BOOL bCommentSet = FALSE; //ע����ͳ�Ʊ�ʶ ��"/*"ʱTRUE, "*/"ʱFALSE
  BOOL bQuatoSet = FALSE;   //�ַ���ͳ�Ʊ�ʶ �״�һ����������"ʱTRUE, ��һ����������"ʱFALSE

  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;

  FILESIZE(file, nLength);
  *pnLength = (int)nLength;

  while(_fgetts(bufRead, 1024, file)!=NULL) {
    int nStartComment;
    nLines++;

    StrTrimLeft(bufRead); //�Ƚ��ļ�ͷ�Ŀո���Ʊ��ȥ��

    //Ϊ�հ���
    if ((int)_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }

    nDoubleSplashFoundAt = _tcschr(bufRead, _T('//'))-bufRead; // ע�ͷ�//�����ںδ�
    if (nDoubleSplashFoundAt>0 /*&& !bCommentSet*/) {
      TCHAR strRight[1024];
      BOOL bCounted = FALSE;
      if ( nDoubleSplashFoundAt==0
           && !bCommentSet
           && !bQuatoSet) {
        nCommentLines++;
        continue;
      }

      _tcscpy(strRight, bufRead);
      do {
        TCHAR strLeft[1024];
        //CString strLeft = strRight _tcsnccpy(nDoubleSplashFoundAt);
        _tcsnccpy(strLeft, strRight, nDoubleSplashFoundAt);
        strLeft[nDoubleSplashFoundAt] = '\0';
        if (_tcschr(strLeft, '\"')!=NULL) {
          int nQuatos = HowManyTCHARsInString(strLeft, _T("\""), bCommentSet);
          //��ǰһ���ַ�������ʱ��ֻҪ//֮ǰ��"��Ϊż��������˵����//Ϊע�ͷ�������Ϊ�����ڲ�//����ע�ͷ�
          //������������˵��//δע�ͷ�
          if ((!bQuatoSet && !IsOdd(nQuatos)) && (bQuatoSet&&IsOdd(nQuatos))) {
            if (m_nStatMethod==0) {
              nCodeLines++;
              bCounted=TRUE;
            } else if (m_nStatMethod==1) {
              nCodeLines++;
              nCommentLines++;
              bCounted=TRUE;
            } else if (m_nStatMethod==2) {
              nCommentLines++;
              bCounted=TRUE;
            }

            if (bQuatoSet&&IsOdd(nQuatos))
              bQuatoSet = FALSE;
          }
        }

        //_tcscpy(strRight, strRight+nDoubleSplashFoundAt+2);
        _tcscpy(strRight, strRight+nDoubleSplashFoundAt+2);
      } while(_tcschr(strRight, _T('//'))!=NULL && !bCounted);
      if (bCounted)
        continue;

      _tcscpy(strRight, bufRead);
      bCounted = FALSE;
      do {
        TCHAR strLeft[1024];
        //CString strLeft = strRight _tcsnccpy(nDoubleSplashFoundAt);
        _tcsnccpy(strLeft, strRight, nDoubleSplashFoundAt);
        strLeft[nDoubleSplashFoundAt] = 0;
        if ( _tcsstr(strLeft, _T("/*"))!=NULL) {
          int nCommB = HowManyTCHARsInString(strLeft, _T("/*"), bCommentSet);
          int nCommE = HowManyTCHARsInString(strLeft, _T("*/"), bCommentSet);
          //��ǰһ���ַ�������ʱ��ֻҪ//֮ǰ��/*��������*/�ĸ�������˵����//Ϊע�ͷ�
          //������/*������*/������һ��˵��//δע�ͷ�
          if ((!bCommentSet&&nCommB==nCommE)&&(bCommentSet&&nCommB==nCommE-1)) {
            if (m_nStatMethod==0) {
              nCodeLines++;
              bCounted=TRUE;
            } else if (m_nStatMethod==1) {
              nCodeLines++;
              nCommentLines++;
              bCounted=TRUE;
            } else if (m_nStatMethod==2) {
              nCommentLines++;
              bCounted=TRUE;
            }

            if (bCommentSet&&nCommB==nCommE-1)
              bCommentSet = FALSE;
          }
        }

        _tcscpy(strRight, strRight+nDoubleSplashFoundAt+2);

      } while( _tcsstr(strRight, _T("/*"))!=NULL&&!bCounted);
      if (bCounted)
        continue;
    }

    nStartComment = _tcsstr(bufRead, _T("/*"))-bufRead;
    if (nStartComment == 0  && !bCommentSet ) // "/*" is the first two TCHARs
    {
      bCommentSet = TRUE;
      //      nLineCommentBegin = nLines;
    } else if (nStartComment != -1 &&!bCommentSet) // "/*" is not the first two TCHARs though found. so it's a code line
    {
      bCommentSet = TRUE;
      nLineCommentBegin = nLines;
      nCommentLines --; //���յ�nCommentLines���1�����Դ˴�Ҫ�ȼ�1

      if (_tcschr(bufRead, '\"')!=NULL) //��ֹ/*������"֮�䣬�� "...../*....."������/*����ע�ͷ�
      {
        TCHAR strTemp[1024];
        int nCountQuota=0;
        int i=1;
        _tcsnccpy(strTemp, bufRead, nStartComment); //��ʼ��/*֮��Ĵ���
        if (strTemp[0]=='\"')
          nCountQuota++;
        while((strTemp[i]=='\"'&&strTemp[i-1]!='\\')&&i<(int)_tcslen(strTemp) ) {
          nCountQuota++;
          i++;
        }
        if ((nCountQuota/2)*2 != nCountQuota)//nCountQuotaΪ��������˵��/*�������У��˴���ע���У���ԭbCommentSet
        {
          bCommentSet = FALSE;
          nCommentLines ++;
        }
      }
    }

    StrTrimRight(bufRead);

    nEndComment = _tcsstr(bufRead, _T("*/"))-bufRead;
    if ((int)_tcslen(bufRead)>=2 && (nEndComment == ((int)_tcslen(bufRead)-2)) && bCommentSet)
      //�����������ַ�        */�ڸ��е����              ǰ����/*
    {
      bCommentSet = FALSE;
      nCommentLines ++;
    } else if (nEndComment != -1 && bCommentSet)// */���ڸ����������Ҫ�����ж�
    {
      TCHAR strRight[1024];
      bCommentSet = FALSE;
      if (nLineCommentBegin == nLines) // /* */ on the same line
        nCommentLines++;
      _tcscpy(strRight, bufRead+ nEndComment+2);
      if ( _tcschr(strRight, _T('//'))!=NULL) //it is very strange!  such as  "code */ (code or blank) // code"
      {
        StrTrimLeft(strRight);
        if ( _tcschr(strRight, _T('//'))==0)
          nCommentLines++;
      } else if (nLineCommentBegin != nLines &&  _tcslen(strRight)==0)
        nCommentLines++;
    }

    if (bCommentSet)
      nCommentLines++;

    if (IsOdd(HowManyTCHARsInString(bufRead, _T("\""), bCommentSet))&&!bCommentSet) {
      //      if (bQuatoSet)
      //      {//���м���ǰһ�е��ַ���
      //        BOOL bCommentBegin = FALSE;
      //        for(int i=0; i<(int)_tcslen(bufRead); i++)
      //        {
      //          if (bufRead[i]=='"' && !bCommentBegin)
      //            bQuatoSet = !bQuatoSet;
      //          if ((!bQuatoSet && bufRead[i]=='/' && bufRead[i+1]=='/')||((!bQuatoSet && bufRead[i]=='/' && bufRead[i+1]=='*')))
      //            bCommentBegin = TRUE;
      //        }
      //      }
      //      else
      //      {//���п�������һ���ַ���
      BOOL bCommentBegin = FALSE;
      int i;
      for(i=0; i<(int)_tcslen(bufRead)-1; i++) {
        if (bufRead[i]=='"' && !bCommentBegin)
          bQuatoSet = !bQuatoSet;
        if ((!bQuatoSet && bufRead[i]=='/' && bufRead[i+1]=='/')||((!bQuatoSet && bufRead[i]=='/' && bufRead[i+1]=='*')))
          bCommentBegin = TRUE;
      }
      if (bQuatoSet && bufRead[(int)_tcslen(bufRead)-1]!='\\')//�ַ�������δ���������������һ���ַ�����\�����������
      {
        //CString strMsg;
        //          strMsg.Format("�ļ�%s�ĵ�%d�п���������");
        //MESSAGEBOX(NULL, strMsg, NULL, NULL);
      }
      //      }
    }
  }

  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);

  return nLines;
}

/*
  ͳ��c++�ļ� 1.0�汾
*/
int GetCppFileLines_1_0(const TCHAR* strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines) {
  //there are two methods to count lines in files, by CStdioFile or CFile
  FILE* file;
  int nLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;
  TCHAR bufRead[1024];
  int nEndComment;
  int nLineCommentBegin = 0;
  int nStartComment;

  BOOL bCommentSet = FALSE; //ע����ͳ�Ʊ�ʶ ��"/*"ʱTRUE, "*/"ʱFALSE

  __int64 nLength;

  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;
  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;

  FILESIZE(file, nLength);
  *pnLength = (int)nLength;


  while(_fgetts(bufRead, 1024, file)!=NULL) {
    nLines++;

    StrTrimLeft(bufRead); //�Ƚ��ļ�ͷ�Ŀո���Ʊ��ȥ��

    if ((int)_tcslen(bufRead)==0) //Ϊ�հ���
    {
      nBlankLines++;
      continue;
    }

    if (bufRead[0]==_T('//') && !bCommentSet) {
      nCommentLines++;
      continue;
    }

    nStartComment = _tcsstr(bufRead, _T("/*"))-bufRead;
    if (nStartComment == 0  && !bCommentSet ) // "/*" is the first two TCHARs
    {
      bCommentSet = TRUE;
      nLineCommentBegin = nLines;
    } else if (nStartComment != -1 &&!bCommentSet) // "/*" is not the first two TCHARs though found. so it's a code line
    {
      bCommentSet = TRUE;
      nLineCommentBegin = nLines;
      nCommentLines --; //���յ�nCommentLines���1�����Դ˴�Ҫ�ȼ�1

      if (_tcschr(bufRead, '\"')!=NULL) //��ֹ/*������"֮�䣬�� "...../*....."������/*����ע�ͷ�
      {
        TCHAR strTemp[1024];
        int nCountQuota=0;
        int i=1;
        _tcsnccpy(strTemp, bufRead, nStartComment); //��ʼ��/*֮��Ĵ���
        if (strTemp[0]=='\"')
          nCountQuota++;
        while((strTemp[i]=='\"'&&strTemp[i-1]!='\\')&&i<(int)_tcslen(strTemp) ) {
          nCountQuota++;
          i++;
        }
        if ((nCountQuota/2)*2 != nCountQuota)//nCountQuotaΪ��������˵��/*�������У��˴���ע���У���ԭbCommentSet
        {
          bCommentSet = FALSE;
          nCommentLines ++;
        }
      }
    }

    StrTrimRight(bufRead);

    nEndComment = _tcsstr(bufRead, _T("*/"))-bufRead;
    if ((int)_tcslen(bufRead)>=2 && (nEndComment == ((int)_tcslen(bufRead)-2)) && bCommentSet)
      //�����������ַ�        */�ڸ��е����              ǰ����/*
    {
      bCommentSet = FALSE;
      nCommentLines ++;
    } else if (nEndComment != -1 && bCommentSet)// */���ڸ����������Ҫ�����ж�
    {
      bCommentSet = FALSE;
      if (nLineCommentBegin == nLines) // /* */ on the same line
        nCommentLines++;
      _tcscpy(bufRead, bufRead+ nEndComment+2);
      if (_tcschr(bufRead, _T('//'))!=NULL) //it is very strange!  such as  "code */ (code or blank) // code"
      {
        StrTrimLeft(bufRead);
        if (bufRead[0]==_T('//'))
          nCommentLines++;
      } else if (nLineCommentBegin != nLines && (int)_tcslen(bufRead)==0)
        nCommentLines++;
    }

    if (bCommentSet)
      nCommentLines++;
  }

  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);

  return nLines;

  //�������벻��ʹ��CStdioFile���д���

  /*  CFile file;

    file.Open(strFileName, CFile::modeRead);

    int nLines = 0;
    int nCodeLines = 0;
    int nCommentLines = 0;
    int nBlankLines = 0;

    BOOL bLastSplash = FALSE;
    BOOL bDoubleSplash = FALSE;
    BOOL bLastStar = FALSE;
    BOOL bCommentCount = FALSE;
    BOOL bBlankCount = TRUE;

    __int64 nLength; FILESIZE(file, nLength);
    m_nSize += nLength;
    *pnLength = (int)nLength;

    TCHAR buf[255];
    int nRead = 0;
    while(nLength>0)
    {
      nRead = file.Read(buf, 250);

      if (buf[0] == '*' && bLastSplash)
      {
        bLastSplash = FALSE;
        bCommentCount = TRUE;
      }

      if (buf[0] == '/' && bLastSplash && !bCommentCount && bBlankCount && !bDoubleSplash)
      {
        bLastSplash = FALSE;
        bDoubleSplash = TRUE;
      }

      if (buf[0] == '/' && bLastStar)
      {
        bLastStar = FALSE;
        bCommentCount = FALSE;
        nCommentLines++;
      }

      for(int i=0; i<nRead; i++)
      {
        if (buf[i] == '/')
        {
          if (i<nRead-1 && buf[i+1]=='*' && !bDoubleSplash )
            bCommentCount = TRUE;
          if (i<nRead-1 && buf[i+1]=='/' && !bCommentCount && bBlankCount  && !bDoubleSplash)
            bDoubleSplash = TRUE;
          if (i==nRead-1)
            bLastSplash=TRUE;
        }


        if (buf[i] == '*')
        {
          if (i<nRead-1 && buf[i+1]=='/')
          {
            bCommentCount = FALSE;
            nCommentLines++;
          }
          if (i==nRead-1)
            bLastStar=TRUE;
        }

        if ( buf[i]!=' ' && buf[i]!='\t' && buf[i]!='\r' && buf[i]!='\n')//&& bBlankCount)
          bBlankCount = FALSE;

        if (buf[i]=='\n')
        {
          nLines++;
          if (bBlankCount)nBlankLines++;
          if (bBlankCount && bCommentCount)nCommentLines--;
          if (bDoubleSplash)
          {
            nCommentLines++;
            bDoubleSplash = FALSE;
          }

          bBlankCount = TRUE;

          if (bCommentCount) nCommentLines++;
        }
      }
      nLength -= nRead;
    }

    if (buf[nRead-1]!='\n')
      nLines++;
    if (bDoubleSplash)
      nCommentLines++;

    *pnCommentLines = nCommentLines;
    *pnBlankLines = nBlankLines;

    fclose(file);

    return nLines;
  */
}

/*
  ͳ��SQL�ļ�������
*/

int GetSqlFileLines(const TCHAR* strFileName, int *pnLength, int *pnCodeLines, int *pnCommentLines, int *pnBlankLines) {

  FILE* file;

  int nLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;
  int nStartComment;
  int nEndComment;

  BOOL bCommentSet = FALSE;
  TCHAR bufRead[1024];

  __int64 nLength;
  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;

  //m_nSize += nLength;
  FILESIZE(file, nLength);
  *pnLength = (int)nLength;

  while(_fgetts(bufRead, 1024, file)!=NULL) {
    nLines++;

    StrTrimLeft(bufRead);

    if ((int)_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }

    if (bufRead[0]=='-' && bufRead[1]=='-' && !bCommentSet) {
      nCommentLines++;
      continue;
    }

    nStartComment = _tcsstr(bufRead, _T("/*"))-bufRead;
    if (nStartComment == 0  && !bCommentSet ) {
      // "/*" is the first two TCHARs
      bCommentSet = TRUE;
    } else if (nStartComment != -1 &&!bCommentSet) {
      // "/*" is not the first two TCHARs though found. so it's a code line
      bCommentSet = TRUE;
      nCommentLines --;
    }

    StrTrimRight(bufRead);

    nEndComment = _tcsstr(bufRead, _T("*/"))-bufRead;
    if ((int)_tcslen(bufRead)>=2 && (nEndComment == (int)_tcslen(bufRead)-2)) {
      bCommentSet = FALSE;
      nCommentLines ++;
    } else if (nEndComment != -1) {
      bCommentSet = FALSE;
      //it is very strange!  such as  " */   -- "
      if (_tcsstr(bufRead, _T("--"))!=NULL) {
        TCHAR sTemp[1024];
        _tcscpy(sTemp, bufRead +nEndComment+3);
        StrTrimLeft(sTemp);
        if (sTemp[0]=='-' && sTemp[1]=='-')
          nCommentLines++;
      }
    }

    if (bCommentSet)
      nCommentLines++;
  }

  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);

  return nLines;
}

int GetVBFileLines(const TCHAR* strFileName, int *pnLength, int *pnCodeLines, int *pnCommentLines, int *pnBlankLines) {

  FILE* file;

  int nLines = 0;
  int nCodeLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;
  TCHAR bufRead[1024];

  __int64 nLength;

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;

  FILESIZE(file, nLength);
  //m_nSize += nLength;
  *pnLength = (int)nLength;


  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  while(_fgetts(bufRead, 1024, file)!=NULL) {
    nLines++;

    StrTrimLeft(bufRead);

    if ((int)_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }

    if (_tcschr(bufRead, '\'')==0) {
      nCommentLines++;
      continue;
    }

    if (_tcschr(bufRead, '\'')!=NULL) {
      if (m_nStatMethod==0)
        nCodeLines++;
      else if (m_nStatMethod==1) {
        nCodeLines++;
        nCommentLines++;
      } else if (m_nStatMethod==2) {
        nCommentLines++;
      }
      continue;
    } else
      nCodeLines++;
  }

  *pnCodeLines = nCodeLines;
  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);

  return nLines;
}

int GetPerlFileLines(const TCHAR* strFileName, int *pnLength, int *pnCodeLines, int *pnCommentLines, int *pnBlankLines) {

  int nLines = 0;
  int nCodeLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;

  __int64 nLength;
  FILE* file;
  TCHAR bufRead[1024];

  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;

  FILESIZE(file, nLength);
  //m_nSize += nLength;
  *pnLength = (int)nLength;


  while(_fgetts(bufRead, 1024, file)!=NULL) {
    nLines++;

    StrTrimLeft(bufRead);

    if ((int)_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }

    if (bufRead[0] == '#') {
      nCommentLines++;
      continue;
    }

    if (_tcschr(bufRead, '#')!=NULL) {
      if (m_nStatMethod==0) {
        nCodeLines++;
      } else if (m_nStatMethod==1) {
        nCodeLines++;
        nCommentLines++;
      } else if (m_nStatMethod==2) {
        nCommentLines++;
      }
      continue;
    } else
      nCodeLines++;
  }

  *pnCodeLines = nCodeLines;
  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);
  return nLines;
}

int GetTxtFileLines(const TCHAR* strFileName, int *pnLength, int *pnCodeLines, int *pnCommentLines, int *pnBlankLines) {

  FILE* file;

  int nLines = 0;
  int nCommentLines = 0;
  int nBlankLines = 0;

  __int64 nLength;
  TCHAR bufRead[1024];

  if ((file = _tfopen(strFileName, _T("r")))==NULL)
    return 0;
  FILESIZE(file, nLength);

  *pnLength = 0;
  *pnCommentLines = 0;
  *pnBlankLines = 0;

  //m_nSize += nLength;
  *pnLength = (int)nLength;


  while(_fgetts(bufRead, 1024, file)!=NULL) {
    nLines++;

    StrTrimLeft(bufRead);

    if ((int)_tcslen(bufRead)==0) {
      nBlankLines++;
      continue;
    }
  }

  *pnCommentLines = nCommentLines;
  *pnBlankLines = nBlankLines;

  fclose(file);

  return nLines;
}
