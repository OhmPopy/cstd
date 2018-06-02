/********************************************************************

  iseeio.h

  ----------------------------------------------------------------
    ������֤ �� GPL
  ��Ȩ���� (C) 2003 VCHelp coPathway ISee workgroup.
  ----------------------------------------------------------------
  ��һ����������������������������������������GNU ͨ�ù�����
  ��֤�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ���
  �����ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
  Ŀ�ص������ĵ���������ϸ����������GNUͨ�ù������֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�ù������֤�ĸ�������Ŀ¼
  GPL.txt�ļ����������û�У�д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
  ----------------------------------------------------------------
  �������ʹ�ñ����ʱ��ʲô������飬�������µ�ַ������ȡ����ϵ��

      http://isee.126.com
      http://cosoft.org.cn/projects/iseeexplorer

  ���ŵ���

      isee##vip.163.com
  ----------------------------------------------------------------
  ���ļ���;��  ISee I/O �����ļ������ݱ�׼��ANSI C ��I/O��
  ���ļ���д�ˣ�
          YZ      yzfree##sina.com
          ������    chenjm##netease.com
          lzp     zlan##corp.netease.com

  ���ļ��汾��  30912
  ����޸��ڣ�  2003-9-12

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
    ��ַ�ռ������
  ----------------------------------------------------------------
  ������ʷ��

    2003-9    ģ��ṹ��Ϊ�ں���ָ������ķ�ʽ�����С�
          ���Ӳ����½��ʽӿڵĺ�����ʹģ������ڶ����Ʒ�ʽ�¶�̬֧���½��ʡ�
    2002-11   ����һ��˳�����д�ļ���ʽ�������ò���ϵͳ�ṩ��
          �ļ�IO���ٻ��幦�ܣ�Ŀǰֻ֧��WIN32��(������)
    2002-10   �����ڴ�����ڲ������壬�������ڴ����ϵͳ��
    2002-8    ����������ʵ�ַ��������ģʽ
    2002-4    �����ļ�ȱʡ��ģʽ����Ϊ������ģʽ��
    2001-6    ��һ�����԰淢��


********************************************************************/



#ifndef __ISEEIO_INC__
#define __ISEEIO_INC__


#ifndef WIN32
#if defined(_WIN32) || defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */


#ifdef WIN32  /* Windows OS */

# ifdef ISEEIO_EXPORTS
# define ISEEIO_API __declspec(dllexport)
# else
# define ISEEIO_API __declspec(dllimport)
# endif
# define CALLAGREEMENT  __cdecl

#else     /* Linux OS */

# define ISEEIO_API
# define CALLAGREEMENT

#endif  /*WIN32*/


#define ISIO_MAX_INFACNUM   32        /* ��������32�׽��ʽӿ� */
#define ISIO_OFFIC_INUM     24        /* �ٷ����ʽӿ�������� */
#define ISIO_OEM_INUM     (ISIO_MAX_INFACNUM-ISIO_OFFIC_INUM) /* ���������ʽӿ�������� */


/* isee I/O �����Ͷ��� */
typedef enum _iseeio_confer_type {
  ISEEIO_CONFER_INVALID,            /* ��Ч��������       */
  ISEEIO_CONFER_LOC_FILE,           /* �����ļ�����   @@    */
  ISEEIO_CONFER_LOC_MEMORY,         /* �����ڴ�����   @@    */
  ISEEIO_CONFER_ROT_INTERNET,         /* Զ��internet       */
  ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,      /* ����˳������ļ����� @@  */
  ISEEIO_CONFER_HTTP,             /* HTTPԶ���ļ�����     */
  ISEEIO_CONFER_FTP,              /* FTPԶ���ļ�����      */
  ISEEIO_CONFER_XXX,              /* ������ */
  ISEEIO_CONFER_MAX = ISIO_OFFIC_INUM,    /* �ٷ��������ͱ߽�     */
  ISEEIO_CONFER_OEM0 = ISIO_OFFIC_INUM,   /* ��������������     */
  ISEEIO_CONFER_OEM1,
  ISEEIO_CONFER_OEM2,
  ISEEIO_CONFER_OEM3,
  ISEEIO_CONFER_OEM4,
  ISEEIO_CONFER_OEM5,
  ISEEIO_CONFER_OEM6,
  ISEEIO_CONFER_OEM7,
  ISEEIO_CONFER_OEM8,
  ISEEIO_CONFER_OEM9,
  ISEEIO_CONFER_OEMA,
  ISEEIO_CONFER_OEMB,
  ISEEIO_CONFER_OEMC,
  ISEEIO_CONFER_OEMD,
  ISEEIO_CONFER_OEME,
  ISEEIO_CONFER_OEMF
} ISEEIO_CONFER_TYPE, *PISEEIO_CONFER_TYPE;
/* ע������ö��ֵ�У�ֻ�д�@@ע�͵Ĳ��ֲſ��Ա�����isio_open_param������*/

#define ISIO_MAX_PATH         512

/* isee I/O ���ĵ�ַ�ṹ */
typedef struct _tag_istream_address {
  ISEEIO_CONFER_TYPE  type;         /* Э������ */
  char        name[ISIO_MAX_PATH];  /* ������ */
  unsigned long   param1;         /* ���Ӳ���1 */
  unsigned long   param2;         /* ���Ӳ���2 */
} ISADDR, *PISADDR;


#if defined(_DEBUG)
#define ISFILE_DBG_ID ("+S+")         /* �������ʶ�ִ��������ã�*/
#endif



/* isee I/O ������ṹ */
typedef struct _tag_iseeio_stream {
  void*        stream;
  ISEEIO_CONFER_TYPE  stream_type;
  char*        org_name;
#if defined(_DEBUG)
  char        dbg_iseeio_stream_id[4];
#endif
} ISFILE;


#ifndef _FILE_DEFINED
#include <stdio.h>
#endif


/* �����ļ�����FILE �ṹ�Ѷ����� stdio.h �ļ��� */
typedef FILE  FILE_STREAM;

/* ����˳������ļ��� - FILE_SEQUENTIAL */
typedef HANDLE  FILE_SEQUENTIAL;

/* �����ڴ������ṹ���� */
#define MEMORY_STREAM_UP_SIZE   4096
#define MEMORY_STREAM_UP_STEP   1

typedef struct _tag_memory_stream {
  char*      p_start_pos; /* �ڴ����׵�ַ */
  unsigned long len;      /* �����ݵĳ��� */
  unsigned long cur_pos;    /* ��ǰ�Ķ�дλ�� */
  unsigned long mode;     /* ���Ĵ�ģʽ */

  unsigned long error;      /* �����־ */
  unsigned long total_size;   /* �ڴ����ܳߴ� */
  PISADDR     pisaddr;    /* �������ݵ�ַ��ISADDR��*/
} MEMORY_STREAM;



/* �������������ṹ���� */
typedef struct _tag_internet_stream {
  char*      p_address;
} INTERNET_STREAM;




/* ��ģʽ�֣�λ�����

            32 bit (unsigned long)
  _________________________________________________________________->low bit
  |0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|
                                 | |                         | | | |
                                 | bin                       | | | |
                                 text                        | | | read
                                                             | | write
                                                             | append
                               create
*/
#define ISIO_MODE_READ    0x1
#define ISIO_MODE_WRITE   0x2
#define ISIO_MODE_RDWR    0x3
#define ISIO_MODE_APPEND  0x4
#define ISIO_MODE_CREATE  0x8

#define ISIO_MODE_BIN   0x10000
#define ISIO_MODE_TXT   0x20000   /* ��ANSI��׼��ISIO������֧�� */



#ifdef  WIN32
#define ISIO_ERROR_EOF    _IOEOF
#define ISIO_ERROR_ERR    _IOERR
#else
#define ISIO_ERROR_EOF    0x10
#define ISIO_ERROR_ERR    0x20
#endif /* WIN32 */

/* ISIO �ӿں���ID���� */
typedef enum _tagISIOFNTYPE {
  ISIOFN_MIN = 0,                   /* ��Ч����ID�߽� */

  ISIOFN_OPEN,
  ISIOFN_CLOSE,

  ISIOFN_READ,
  ISIOFN_WRITE,
  ISIOFN_FLUSH,

  ISIOFN_SEEK,
  ISIOFN_REWIND,
  ISIOFN_TELL,
  ISIOFN_LENGTH,

  ISIOFN_EOF,
  ISIOFN_ERROR,
  ISIOFN_CLEARERR,

  ISIOFN_MAX                      /* ��Ч����ID�߽� */
} ISIOFN, *LPISIOFN;

/* �ض����ʵĶ�д�ӿ� */
typedef struct _tagISIOFNSET {
  int     id;                   /* ���ʱ�ʶ */
  char    name[64 - sizeof(int)];       /* �������� */
  void*    pfn[ISIOFN_MAX];           /* ���ʶ�д�ӿں������� */
} ISIOFNSET, *LPISIOFNSET;


#define _ISIOINFAC_MAGIC    0xbbaaccff        /* �ṹ��ǩ */

/* ���ʶ�д�ӿڽṹ */
typedef struct _tagISIOINFAC {
  unsigned long magic;                /* �ṹ��ǩ������_ISIOINFAC_MAGIC��*/
  unsigned long rev;                /* ���� */

  int       total;                /* ���ʽӿڵ������� */
  int       used;               /* ��Ч�ӿ����� */

  ISIOFNSET   ifc[ISIO_MAX_INFACNUM];       /* ���ʼ� */
} ISIOINFAC, *LPISIOINFAC;


#if (defined(__cplusplus) || defined(c_plusplus))
extern  "C" {
#endif

  /* ISIO �ӿں���ԭ�Ͷ��� */
  typedef ISFILE*     (CALLAGREEMENT* ISIOFNT_OPEN)(const char*, const char*);
  typedef int (CALLAGREEMENT* ISIOFNT_CLOSE)(ISFILE*);
  typedef size_t (CALLAGREEMENT* ISIOFNT_READ)(void*, size_t, size_t, ISFILE*);
  typedef size_t (CALLAGREEMENT* ISIOFNT_WRITE)(const void*, size_t, size_t, ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_FLUSH)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_SEEK)(ISFILE*, long, int);
  typedef void (CALLAGREEMENT* ISIOFNT_REWIND)(ISFILE*);
  typedef long(CALLAGREEMENT* ISIOFNT_TELL)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_EOF)(ISFILE*);
  typedef int (CALLAGREEMENT* ISIOFNT_ERROR)(ISFILE*);
  typedef void (CALLAGREEMENT* ISIOFNT_CLEARERR)(ISFILE*);
  typedef unsigned long(CALLAGREEMENT* ISIOFNT_LENGTH)(ISFILE*);


  /* isee I/O �������� */
  extern ISEEIO_API const char*       CALLAGREEMENT isio_open_param(PISADDR psct, ISEEIO_CONFER_TYPE type, const char* name, unsigned long param1, unsigned long param2);
  extern ISEEIO_API ISEEIO_CONFER_TYPE  CALLAGREEMENT isio_insert_oem_io(ISIOFNT_OPEN, ISIOFNT_CLOSE, ISIOFNT_READ, ISIOFNT_WRITE, ISIOFNT_FLUSH, ISIOFNT_SEEK, ISIOFNT_REWIND, ISIOFNT_TELL, ISIOFNT_EOF, ISIOFNT_ERROR, ISIOFNT_CLEARERR, ISIOFNT_LENGTH);

  /* isee I/O ������ */
  extern ISEEIO_API ISFILE*       CALLAGREEMENT isio_open(const char* pstream_name, const char* mode);
  extern ISEEIO_API int           CALLAGREEMENT isio_close(ISFILE* stream);

  extern ISEEIO_API size_t        CALLAGREEMENT isio_read(void* buffer, size_t size, size_t count, ISFILE* stream);
  extern ISEEIO_API size_t        CALLAGREEMENT isio_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
  extern ISEEIO_API int           CALLAGREEMENT isio_flush(ISFILE* stream);

  extern ISEEIO_API int           CALLAGREEMENT isio_seek(ISFILE* stream, long offset, int origin);
  extern ISEEIO_API void          CALLAGREEMENT isio_rewind(ISFILE* stream);
  extern ISEEIO_API long        CALLAGREEMENT isio_tell(ISFILE* stream);
  extern ISEEIO_API unsigned long CALLAGREEMENT isio_length(ISFILE* stream);

  extern ISEEIO_API int           CALLAGREEMENT isio_eof(ISFILE* stream);
  extern ISEEIO_API int           CALLAGREEMENT isio_error(ISFILE* stream);
  extern ISEEIO_API void          CALLAGREEMENT isio_clearerr(ISFILE* stream);

  /*
  �µ�ISIO������ extern ISEEIO_API xxx CALLAGREEMENT isio_xxxx();
  */

#ifdef _DEBUG
  extern ISEEIO_API int      CALLAGREEMENT __get_object_counts(void);
#endif /*_DEBUG*/

#if (defined(__cplusplus) || defined(c_plusplus))
}
#endif


#endif /* __ISEEIO_INC__ */
