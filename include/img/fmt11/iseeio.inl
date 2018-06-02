/********************************************************************

  iseeio.c

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
  ���ļ���;��  ISee I/O ʵ���ļ������ݱ�׼��ANSI C ��I/O��
  ���ļ���д�ˣ�
          YZ      yzfree##sina.com
          ������    chenjm##netease.com
          ����    mssoft##sina.com
          lzp     zlan##corp.netease.com

  ���ļ��汾��  30912
  ����޸��ڣ�  2003-09-12

  ע������E-Mail��ַ�е�##����@�滻����������Ϊ�˵��ƶ����E-Mail
    ��ַ�ռ������
    ----------------------------------------------------------------
  ������ʷ��

    2003-9    ģ��ṹ��Ϊ�ں���ָ������ķ�ʽ�����С�
          ���Ӳ����½��ʽӿڵĺ�����ʹģ������ڶ����Ʒ�ʽ�¶�̬֧���½��ʡ�
    2003-7    �޸��ڴ���������ʽ���ɶ���������Ϊ��ָ���������Լ����ط����ڴ�Ĵ�����
    2003-7    ����isio_error����ISEEIO_CONFER_LOC_FILE_SEQUENTIAL���ʹ�����
          ��BUG��ԭ���뽫���ļ����д����Ҳ��Ϊ�ļ�����󷵻أ�
    2003-6    ����isio_seek�е�lDistanceToMoveΪ����ʱ���ֵĴ���(���� mssoft#sina.com)
          (��������:SetFilePointer��lpDistanceToMoveHigh�������ó�ָ��0��ָ�룬
          ��lDistanceToMoveΪ����ʱSetFilePointer�϶��û�ָ����Ϊ64λ����
          ����ʱ��32λ��Ϊ0�������Ǻǡ�)
    2002-11   ����һ��˳�����д�ļ���ʽ�������ò���ϵͳ�ṩ��
          �ļ�IO���ٻ��幦�ܣ�Ŀǰֻ֧��WIN32��(������)
    2002-10   �����ڴ�����ڲ������壬�������ڴ����ϵͳ��
    2002-8    ����������ʵ�ַ��������ģʽ
    2002-4    �����ļ�ȱʡ��ģʽ����Ϊ������ģʽ��
    2001-6    ������һ�����԰�


********************************************************************/


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeio.h"


#ifdef _DEBUG
static int __isfile_object_counts = 0;
#endif

#if defined(WIN32) && defined(_MSC_VER)
#include <fcntl.h>  /* �趨�ļ���ģʽʱ��Ҫ��һЩ�� */
static int old_fmode;
#endif

/* ���ʽӿ���Ϣ�ṹ */
static ISIOINFAC  _isio_ifc;


#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER 0XFFFFFFFF
#endif

/* �ڴ����չ���� */
#define MVA     2


/* �ڲ����� */
void        CALLAGREEMENT _openio(void);
void        CALLAGREEMENT _closeio(void);

unsigned long   CALLAGREEMENT _analyst_for_mode(const char* mode);
int         CALLAGREEMENT _add_memory_block(MEMORY_STREAM* memstream, int add_size);
int         CALLAGREEMENT _dec_memory_block(MEMORY_STREAM* memstream);
ISFILE*       CALLAGREEMENT _alloc_ISFILE(void);
ISFILE*       CALLAGREEMENT _free_ISFILE(ISFILE* pfile);
MEMORY_STREAM*    CALLAGREEMENT _alloc_MEMORY_STREAM(void);
MEMORY_STREAM*    CALLAGREEMENT _free_MEMORY_STREAM(MEMORY_STREAM* pmem);

/* �����ļ����ʶ�д�ӿں��� */
ISFILE*       CALLAGREEMENT LOC_FILE_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_FILE_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_FILE_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_FILE_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_error(ISFILE* stream);
void        CALLAGREEMENT LOC_FILE_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_FILE_length(ISFILE* stream);

/* �����ڴ���ʶ�д�ӿں��� */
ISFILE*       CALLAGREEMENT LOC_MEMORY_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_MEMORY_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_MEMORY_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_MEMORY_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_MEMORY_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_MEMORY_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_error(ISFILE* stream);
void        CALLAGREEMENT LOC_MEMORY_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_MEMORY_length(ISFILE* stream);

/* Զ���ļ����ʶ�д�ӿں��� */
ISFILE*       CALLAGREEMENT ROT_INTERNET_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT ROT_INTERNET_close(ISFILE* stream);
size_t        CALLAGREEMENT ROT_INTERNET_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT ROT_INTERNET_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_flush(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT ROT_INTERNET_rewind(ISFILE* stream);
long        CALLAGREEMENT ROT_INTERNET_tell(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_eof(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_error(ISFILE* stream);
void        CALLAGREEMENT ROT_INTERNET_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT ROT_INTERNET_length(ISFILE* stream);

/* �����ļ����ʿ��ٶ�д�ӿں��� */
ISFILE*       CALLAGREEMENT LOC_FILE_SEQUENTIAL_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_SEQUENTIAL_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_SEQUENTIAL_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_FILE_SEQUENTIAL_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_FILE_SEQUENTIAL_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_error(ISFILE* stream);
void        CALLAGREEMENT LOC_FILE_SEQUENTIAL_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_FILE_SEQUENTIAL_length(ISFILE* stream);



/* ################################################################

  ע��* ��ǰ�汾ֻʵ�����ļ������ڴ����Ĵ��룬������δʵ�֡�
    * ISIO���������趨ԭ���Ǽ���ANSI C�����������ڱ����������Լ�
      ��չ�����������ڿ��Ƿ�Χ֮�ڡ�

   ################################################################*/





/*
  ���ܣ�׼������isio_open������ַ����

  ��ڣ�
  PISADDR psct        - ��������ṹ

  ISEEIO_CONFER_TYPE type   - �������͡��μ�ö������ISEEIO_CONFER_TYPE��

  const char *name      - �����ơ�  �ļ����ͣ�ָ��·������ָ��
                        �ڴ����ͣ�ָ���ڴ��ļ�������������ļ�����д��
                            ����ͬ��
                        �������ͣ���
                      ˳�����ͣ�ָ��·������ָ��

  unsigned long param1    - ���Ӳ������ļ����ͣ���
                      �ڴ����ͣ�ָ���ڴ���׵�ַ��ָ�루���ڴ�����
                            �Ƕ����ģ���������ĳһ�ڴ���м��
                            ��ַ�����Ҹ��ڴ�������ɶ�д������
                            �����ڴ�ӿڷ���ģ�
                      �������ͣ���
                      ˳�����ͣ���

  unsigned long param2    - ���Ӳ������ļ����ͣ���
                      �ڴ����ͣ��ڴ��ĳߴ�
                      �������ͣ���
                      ˳�����ͣ���


  ���أ���ǿ��ת����psct��ַ��
*/
ISEEIO_API const char* CALLAGREEMENT isio_open_param(
    PISADDR psct,
    ISEEIO_CONFER_TYPE type,
    const char* name,
    unsigned long param1,
    unsigned long param2
)
{
  assert(psct);

  strcpy((char*)psct->name, (const char*)name);
  psct->type   = type;
  psct->param1 = param1;
  psct->param2 = param2;

  return (const char*)psct;
}


/*
  ���ܣ�������������ʶ�д�ӿ�

  ��ڣ�
    �½��ʶ�д�����ĵ�ַ

  ���ڣ�
    ISEEIO_CONFER_INVALID - ��ʾû���㹻���ڴ�����µĽ��ʶ�д�ӿڣ�����ʧ��
    ����ֵ          - ����ɹ�������ֵΪ�½��ʵ�ID

*/
ISEEIO_API ISEEIO_CONFER_TYPE CALLAGREEMENT isio_insert_oem_io(ISIOFNT_OPEN ioiopen,
    ISIOFNT_CLOSE ioiclose,
    ISIOFNT_READ ioiread,
    ISIOFNT_WRITE ioiwrite,
    ISIOFNT_FLUSH ioiflush,
    ISIOFNT_SEEK ioiseek,
    ISIOFNT_REWIND ioirewind,
    ISIOFNT_TELL ioitell,
    ISIOFNT_EOF ioieof,
    ISIOFNT_ERROR ioierror,
    ISIOFNT_CLEARERR ioiclearerr,
    ISIOFNT_LENGTH ioilength)
{
  int   i;

  /* ����δ��ռ�õĽ��ʽӿڼ� */
  for (i = ISEEIO_CONFER_MAX; i < (ISEEIO_CONFER_MAX + ISIO_OEM_INUM); i++) {
    if (_isio_ifc.ifc[i].id != i) {
      break;
    }
  }

  /* ���OEM���������򷵻���Ч���� */
  if (i == (ISEEIO_CONFER_MAX + ISIO_OEM_INUM)) {
    return ISEEIO_CONFER_INVALID;
  }

  /* ���нӿں�������ȫ���ṩ */
  assert(ioiopen && ioiclose && ioiread && ioiwrite && ioiflush && ioiseek && ioirewind && ioitell && ioilength && ioieof && ioierror && ioiclearerr);
  assert(_isio_ifc.ifc[i].id == 0);

  /* ���ýӿڼ����� */
  _isio_ifc.ifc[i].id = i;

  strcpy((char*)_isio_ifc.ifc[i].name, (const char*)"OEM����");

  _isio_ifc.ifc[i].pfn[ISIOFN_MIN]       = (void*)NULL;
  _isio_ifc.ifc[i].pfn[ISIOFN_OPEN]    = (void*)ioiopen;
  _isio_ifc.ifc[i].pfn[ISIOFN_CLOSE]     = (void*)ioiclose;
  _isio_ifc.ifc[i].pfn[ISIOFN_READ]    = (void*)ioiread;
  _isio_ifc.ifc[i].pfn[ISIOFN_WRITE]     = (void*)ioiwrite;
  _isio_ifc.ifc[i].pfn[ISIOFN_FLUSH]     = (void*)ioiflush;
  _isio_ifc.ifc[i].pfn[ISIOFN_SEEK]    = (void*)ioiseek;
  _isio_ifc.ifc[i].pfn[ISIOFN_REWIND]    = (void*)ioirewind;
  _isio_ifc.ifc[i].pfn[ISIOFN_TELL]    = (void*)ioitell;
  _isio_ifc.ifc[i].pfn[ISIOFN_LENGTH]    = (void*)ioilength;
  _isio_ifc.ifc[i].pfn[ISIOFN_EOF]     = (void*)ioieof;
  _isio_ifc.ifc[i].pfn[ISIOFN_ERROR]     = (void*)ioierror;
  _isio_ifc.ifc[i].pfn[ISIOFN_CLEARERR]  = (void*)ioiclearerr;
  _isio_ifc.ifc[i].pfn[ISIOFN_MAX]     = (void*)NULL;

  _isio_ifc.used++;

  return (ISEEIO_CONFER_TYPE)i;
}




/*
  ���ܣ���ָ����ISIO��

  ��ڣ�
  pstream_name  �� �����ƽṹ��ַ��

  �ò����������������ͣ�һ����ISADDR�ṹ��ַ��ǿ��ת��Ϊchar*����������
  �������£�

        �ļ�����  �ڴ�����  ��������  ˳������ļ�
  ---------------------------------------------------------------------
  *name   �ļ�·��  �ڴ��ļ�  ��      ͬ�ļ�����
        ��      �������
              ���ļ���
              ͬ����

  param1    ��      �ڴ����  ��      ��
              ��ַ��
              д����ʱ
              �ɲ�����
  param2    ��      �ڴ����  ��      ��
              ����ֵ��
              д����ʱ
              �ɲ�����
  ---------------------------------------------------------------------
                          ע��"��"��ʾδ��ʹ��

  mode      �� �����ķ�ʽ��

  ��Ч��������£�r,r+,rb,r+b,w,w+,wb,w+b,a,a+,ab,a+b�����ǵĺ���ɲμ�
  ANSI C��fopen��ע�͡�
  ע������˳�����������˵��ֻ֧��r��a��w����ģʽ��

  ע������isio_open��һ���ڴ�������ڲ���pstream_name��ָ���ISADDR
    �ṹ������ɾ������Ϊ��������ʱ��ϵͳ����������д������ݡ���ϸ˵��
    �μ�isio_close������

  ���أ�  0 �� ��ʧ��
    ��0 �� �򿪳ɹ�������ֵ������������ISIO����
*/
ISEEIO_API ISFILE* CALLAGREEMENT isio_open(const char* pstream_name, const char* mode)
{
  assert((pstream_name) && (mode));

  return (*(ISIOFNT_OPEN)_isio_ifc.ifc[((PISADDR)pstream_name)->type].pfn[ISIOFN_OPEN])(pstream_name, mode);
}




/*
  ���ܣ��ر�ָ����ISIO��

  ��ڣ�
    stream  �� ��isio_open()�����򿪵�ISIO�����

  ��ע��

  isee I/O �����رպ�isio_close����������isio_open���������PISADDR�ṹ
  ��д��Ҫ�����ݣ������������£�

        �ļ�����  �ڴ�����  ��������  ˳������
  ---------------------------------------------------------------------
  *name   ��      �ڴ��ļ�  ��      ��
              ��
  param1    ��      ������  ��      ��
              ����׵�ַ
  param2    ��      �ڴ������  ��      ��
              ����ֵ
  ---------------------------------------------------------------------
                          ע��"��"��ʾδ���ı�

  �ڴ����ͱ�ע������һ���ڴ�����ԭ�����ڴ�飨���ָ���˵Ļ���������
    ��ʹ�ã�����ڴ��ܿ��ܻᱻISIO�����ı�����λ�ã�������������д��
    �ݵ��������ֻ�йر�����ڴ����������������ṹ�з��صĵ�ַ��name
    ���ſ��Ա���ȫ��ʹ�á�


  ���أ�  0 �� �رճɹ�
    EOF �� �ر�ʧ��
*/
ISEEIO_API int CALLAGREEMENT isio_close(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_CLOSE)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_CLOSE])(stream);
}




/*
  ���ܣ���ISIO��������

  ��ڣ�
    buffer  �� ���ڴ�Ŷ������ݵĻ������׵�ַ
    size  �� ������ĳߴ�
    count �� ������ĸ���
    stream  �� ISIO�����

  ���أ��ɹ���������������
*/
ISEEIO_API size_t CALLAGREEMENT isio_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  assert(buffer && size && count);
  assert(stream);

  return (*(ISIOFNT_READ)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_READ])(buffer, size, count, stream);
}




/*
  ���ܣ���ISIO��д����

  ��ڣ�
    buffer  �� ��д�����ݻ������׵�ַ
    size  �� ������ĳߴ�
    count �� ������ĸ���
    stream  �� ISIO�����

  ���أ��ɹ�д������������
*/
ISEEIO_API size_t CALLAGREEMENT isio_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  assert(buffer && size && count);
  assert(stream);

  return (*(ISIOFNT_WRITE)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_WRITE])(buffer, size, count, stream);
}



/*
  ���ܣ�ˢ��ISIO����ע���ڴ���û��ˢ�����ԣ�

  ��ڣ�
    stream  �� ISIO�����

  ���أ� 0  �� ˢ�³ɹ�
    EOF �� ʧ��
*/
ISEEIO_API int CALLAGREEMENT isio_flush(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_FLUSH)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_FLUSH])(stream);
}




/*
  ���ܣ�ISIO��ָ�붨λ��ע������λ�ã�

  ��ڣ�
    stream  �� ISIO�����
    offset  �� �����origin��ƫ���������ֽ�Ϊ��λ��
    origin  �� ��λ�õĲο��㣨SEEK_SET��SEEK_CUR��SEEK_END��

  ���أ� 0 �� ��λ�ɹ�
    -1 �� ʧ��
*/
ISEEIO_API int CALLAGREEMENT isio_seek(ISFILE* stream, long offset, int origin)
{
  assert(stream);

  return (*(ISIOFNT_SEEK)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_SEEK])(stream, offset, origin);
}




/*
  ���ܣ�ISIO��ָ�붨λ��ע����λ����ͷ����

  ��ڣ�
    stream  �� ISIO�����

  ���أ���
*/

ISEEIO_API void CALLAGREEMENT isio_rewind(ISFILE* stream)
{
  assert(stream);

  (*(ISIOFNT_REWIND)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_REWIND])(stream);
}




/*
  ���ܣ���ȡISIO����ǰָ��λ��

  ��ڣ�
    stream  �� ISIO�����

  ���أ�  -1 �� ����ִ��ʧ��
    ��-1 �� λ��ֵ
*/
ISEEIO_API long CALLAGREEMENT isio_tell(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_TELL)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_TELL])(stream);
}




/*
  ���ܣ��ж�ISIO����ǰָ���Ƿ�������β��

  ��ڣ�
    stream  �� ISIO�����

  ���أ�  0 �� ����β��
    ��0 �� ��β��
*/
ISEEIO_API int CALLAGREEMENT isio_eof(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_EOF)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_EOF])(stream);
}




/*
  ���ܣ��ж�ISIO���Ƿ����˴���

  ��ڣ�
    stream  �� ISIO�����

  ���أ�  0 �� û�д���
    ��0 �� �д���
*/
ISEEIO_API int CALLAGREEMENT isio_error(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_ERROR)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_ERROR])(stream);
}




/*
  ���ܣ����ISIO���Ĵ����־��EOF��־

  ��ڣ�
    stream  �� ISIO�����

  ���أ���
*/
ISEEIO_API void CALLAGREEMENT isio_clearerr(ISFILE* stream)
{
  assert(stream);

  (*(ISIOFNT_CLEARERR)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_CLEARERR])(stream);
}




/*
  ���ܣ�ȡ��ISIO���ĳ���

  ��ڣ�
    stream  �� ISIO�����

  ���أ������ȣ����ֽ�Ϊ��λ��
*/
ISEEIO_API unsigned long CALLAGREEMENT isio_length(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_LENGTH)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_LENGTH])(stream);
}





/***********************************************************************************/
/* ϵͳ�ڲ����� */


/* ����ISIO��Openģʽ */
unsigned long CALLAGREEMENT _analyst_for_mode(const char* mode)
{
  unsigned long result = 0UL;
  int count = 0;

  assert(mode);

  switch (*mode) {
  case 'r':
    result = ISIO_MODE_READ;
    break;

  case 'w':
    result = ISIO_MODE_CREATE | ISIO_MODE_WRITE | ISIO_MODE_READ;
    break;

  case 'a':
    result = ISIO_MODE_CREATE | ISIO_MODE_APPEND | ISIO_MODE_READ;
    break;

  default:
    assert(0);            /* �Ƿ���ģʽ��ʶ�� ANSI (r/w/a)*/
    return 0UL;
  }

  result |= ISIO_MODE_BIN;      /* ANSI ��ģʽΪ������ */

  while (*++mode) {
    switch (*mode) {
    case '+':
      if (result & (ISIO_MODE_WRITE | ISIO_MODE_APPEND))
        ;
      else {
        result |= ISIO_MODE_WRITE;
      }

      break;

    case 'b':
      break;

    default:
      assert(0);          /* �Ƿ���ģʽ��ʶ�� */
      count = 2;
      break;
    }

    /* ģʽ����󳤶�Ϊ3���ַ����磺"r+b"�ȵ� */
    if (++count >= 2) {
      break;
    }
  }

  return result;
}


/* Ϊ�ڴ������ӿռ䣨���ӵĳߴ�add_size��*/
int CALLAGREEMENT _add_memory_block(MEMORY_STREAM* memstream, int add_size)
{
  char* ptmp;
  int new_size;

  assert(memstream);
  assert(memstream->mode & ISIO_MODE_CREATE);

  /* ���add_size��ָ��Ϊ0����������ϵͳȱʡ���ڴ�ߴ磺MEMORY_STREAM_UP_SIZE*MEMORY_STREAM_UP_STEP */
  new_size = memstream->total_size + ((add_size == 0) ? (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP) : add_size);

  ptmp = (char*)im_realloc((void*)(memstream->p_start_pos), new_size);

  if (!ptmp) {
    return -1;  /* �ڴ治�� */
  }

  memstream->p_start_pos = ptmp;
  memstream->total_size  = new_size;

  return 0;       /* �ɹ� */
}


/* �ͷ��ڴ��β���Ŀ����ڴ� */
int CALLAGREEMENT _dec_memory_block(MEMORY_STREAM* memstream)
{
  char* ptmp;

  assert(memstream);
  assert(memstream->total_size >= memstream->len);

  if ((memstream->p_start_pos) && (memstream->total_size > memstream->len)) {
    ptmp = (char*)im_realloc((void*)(memstream->p_start_pos), memstream->len);

    if (!ptmp) {
      return -1;  /* realloc����ʧ�ܣ��ƺ�������:�� */
    }

    memstream->p_start_pos = ptmp;
    memstream->total_size  = memstream->len;
    return 0;     /* �ɹ� */
  }

  return 1;       /* û�п��е��ڴ����� */
}


/* ����һ���µ��ڴ����ṹ */
MEMORY_STREAM* CALLAGREEMENT _alloc_MEMORY_STREAM()
{
  MEMORY_STREAM* pmem = (MEMORY_STREAM*)im_malloc(sizeof(MEMORY_STREAM));

  if (!pmem) {
    return (MEMORY_STREAM*)0;
  }

  pmem->p_start_pos = (char*)0;

  pmem->len = pmem->cur_pos = pmem->total_size = 0UL;

  pmem->mode    = 0UL;
  pmem->error   = 0UL;
  pmem->pisaddr = 0;

  return pmem;
}


/* �ͷ�һ���ڴ����ṹ���ú��������ͷ�����Ӧ���ڴ�飩 */
MEMORY_STREAM* CALLAGREEMENT _free_MEMORY_STREAM(MEMORY_STREAM* pmem)
{
  assert(pmem);       /* ���ϸ����ڲ������ */

  im_free(pmem);

  return (MEMORY_STREAM*)0;
}


/* ����һ���µ�ISFILE�ṹ */
ISFILE* CALLAGREEMENT _alloc_ISFILE()
{
  ISFILE* pfile = (ISFILE*)im_malloc(sizeof(ISFILE));

  if (!pfile) {
    return (ISFILE*)0;
  }

#ifdef _DEBUG
  strcpy((char*)pfile->dbg_iseeio_stream_id, ISFILE_DBG_ID);
#endif

  pfile->org_name = (char*)0;
  pfile->stream   = (void*)0;
  pfile->stream_type = ISEEIO_CONFER_INVALID;

#ifdef _DEBUG
  __isfile_object_counts++;
#endif

  return pfile;
}


/* �ͷ�һ��ISFILE�ṹ */
ISFILE* CALLAGREEMENT _free_ISFILE(ISFILE* pfile)
{
  assert(pfile);        /* ���ϸ����ڲ������ */

  if (!pfile) {
    return (ISFILE*)0;
  }

  assert(!strcmp((const char*)pfile->dbg_iseeio_stream_id, ISFILE_DBG_ID));

  im_free(pfile);

#ifdef _DEBUG
  __isfile_object_counts--;
#endif

  return (ISFILE*)0;
}


/* ����ר�ã���ȡISFILEʵ��ĸ��� */
#ifdef _DEBUG
ISEEIO_API int CALLAGREEMENT __get_object_counts(void)
{
  return __isfile_object_counts;
}
#endif /*_DEBUG*/


/* ��ISeeIOϵͳ����ʼ��������*/
void CALLAGREEMENT _openio()
{
  memset((void*)&_isio_ifc, 0, sizeof(ISIOINFAC));

  _isio_ifc.magic = _ISIOINFAC_MAGIC;
  _isio_ifc.rev   = 0;
  _isio_ifc.total = ISIO_MAX_INFACNUM;
  _isio_ifc.used  = ISEEIO_CONFER_XXX - 1;


  /* ������Ч���͵Ľӿ����ݣ����ڲ������*/
  _isio_ifc.ifc[ISEEIO_CONFER_INVALID].id = ISEEIO_CONFER_INVALID;
  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_INVALID].name, "��Ч����");
  memset((void*)_isio_ifc.ifc[ISEEIO_CONFER_INVALID].pfn, 0, sizeof(void*)*ISIOFN_MAX);


  /* ���ñ����ļ���д�ӿ����� */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].id = ISEEIO_CONFER_LOC_FILE;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].name, "�����ļ�");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_OPEN]     = (void*)LOC_FILE_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_CLOSE]    = (void*)LOC_FILE_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_READ]     = (void*)LOC_FILE_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_WRITE]    = (void*)LOC_FILE_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_FLUSH]    = (void*)LOC_FILE_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_SEEK]     = (void*)LOC_FILE_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_REWIND]   = (void*)LOC_FILE_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_TELL]     = (void*)LOC_FILE_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_LENGTH]   = (void*)LOC_FILE_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_EOF]      = (void*)LOC_FILE_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_ERROR]    = (void*)LOC_FILE_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_CLEARERR] = (void*)LOC_FILE_clearerr;


  /* ���ñ����ڴ��д�ӿ����� */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].id = ISEEIO_CONFER_LOC_MEMORY;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].name, "�����ڴ�");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_OPEN]     = (void*)LOC_MEMORY_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_CLOSE]    = (void*)LOC_MEMORY_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_READ]     = (void*)LOC_MEMORY_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_WRITE]    = (void*)LOC_MEMORY_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_FLUSH]    = (void*)LOC_MEMORY_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_SEEK]     = (void*)LOC_MEMORY_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_REWIND]   = (void*)LOC_MEMORY_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_TELL]     = (void*)LOC_MEMORY_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_LENGTH]   = (void*)LOC_MEMORY_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_EOF]      = (void*)LOC_MEMORY_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_ERROR]    = (void*)LOC_MEMORY_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_CLEARERR] = (void*)LOC_MEMORY_clearerr;


  /* ���ñ����ļ����ٶ�д�ӿ����� */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].id = ISEEIO_CONFER_LOC_FILE_SEQUENTIAL;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].name, "�����ڴ�");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_OPEN]     = (void*)LOC_FILE_SEQUENTIAL_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_CLOSE]    = (void*)LOC_FILE_SEQUENTIAL_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_READ]     = (void*)LOC_FILE_SEQUENTIAL_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_WRITE]    = (void*)LOC_FILE_SEQUENTIAL_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_FLUSH]    = (void*)LOC_FILE_SEQUENTIAL_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_SEEK]     = (void*)LOC_FILE_SEQUENTIAL_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_REWIND]   = (void*)LOC_FILE_SEQUENTIAL_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_TELL]     = (void*)LOC_FILE_SEQUENTIAL_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_LENGTH]   = (void*)LOC_FILE_SEQUENTIAL_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_EOF]      = (void*)LOC_FILE_SEQUENTIAL_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_ERROR]    = (void*)LOC_FILE_SEQUENTIAL_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_CLEARERR] = (void*)LOC_FILE_SEQUENTIAL_clearerr;


  /* ����ISEEIO_CONFER_ROT_INTERNET���͵Ľӿ����ݣ���δʵ�֣�*/
  _isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].id = ISEEIO_CONFER_ROT_INTERNET;
  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].name, "Զ���ļ�����");
  memset((void*)_isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].pfn, 0, sizeof(void*)*ISIOFN_MAX);

}


/* �ر�ISeeIOϵͳ��������Դ��*/
void CALLAGREEMENT _closeio()
{
  return;
}



/**************************************************************************************
 * ISEEIO_CONFER_LOC_FILE - �����ļ����ʶ�д�ӿ�
 *************************************************************************************/

ISFILE* CALLAGREEMENT LOC_FILE_open(const char* pstream_name, const char* mode)
{
  PISADDR       pname = (PISADDR)pstream_name;
  ISFILE*        pfile = (ISFILE*)0;
  void*        pstream = (void*)0;

  /* ����ISFILE�ṹ�� */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;              /* �ڴ治�� */
  }

  /* ���ļ� */
  if ((pstream = (void*)fopen((const char*)pname->name, mode)) == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream      = pstream;
  pfile->stream_type = ISEEIO_CONFER_LOC_FILE;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_FILE_close(ISFILE* stream)
{
  int result;

  /* �ر��ļ������ͷ���Ӧ�Ľṹ */
  result = fclose((FILE*)(stream->stream));

  _free_ISFILE(stream);

  return result;
}


size_t CALLAGREEMENT LOC_FILE_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return fread(buffer, size, count, (FILE*)(stream->stream));
}


size_t CALLAGREEMENT LOC_FILE_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return fwrite(buffer, size, count, (FILE*)stream->stream);
}


int CALLAGREEMENT LOC_FILE_flush(ISFILE* stream)
{
  return fflush((FILE*)(stream->stream));
}


int CALLAGREEMENT LOC_FILE_seek(ISFILE* stream, long offset, int origin)
{
  return fseek((FILE*)stream->stream, offset, origin);
}


void CALLAGREEMENT LOC_FILE_rewind(ISFILE* stream)
{
  rewind((FILE*)(stream->stream));
}


long CALLAGREEMENT LOC_FILE_tell(ISFILE* stream)
{
  return ftell((FILE*)(stream->stream));
}


int CALLAGREEMENT LOC_FILE_eof(ISFILE* stream)
{
  return feof((FILE*)stream->stream);
}


int CALLAGREEMENT LOC_FILE_error(ISFILE* stream)
{
  return ferror((FILE*)stream->stream);
}


void CALLAGREEMENT LOC_FILE_clearerr(ISFILE* stream)
{
  clearerr((FILE*)(stream->stream));
}


unsigned long CALLAGREEMENT LOC_FILE_length(ISFILE* stream)
{
  long      pos;
  unsigned long len;

  pos = ftell((FILE*)(stream->stream));
  fseek((FILE*)stream->stream, 0, SEEK_END);
  len = (unsigned long)ftell((FILE*)(stream->stream));
  fseek((FILE*)stream->stream, pos, SEEK_SET);

  return len;
}










/**************************************************************************************
 * ISEEIO_CONFER_LOC_MEMORY - �����ڴ���ʶ�д�ӿ�
 *************************************************************************************/
ISFILE* CALLAGREEMENT LOC_MEMORY_open(const char* pstream_name, const char* mode)
{
  PISADDR       pname = (PISADDR)pstream_name;
  ISFILE*        pfile = (ISFILE*)0;
  MEMORY_STREAM*   pmemory;

  /* ����ISFILE�ṹ�� */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;              /* �ڴ治�� */
  }

  /* �����ڴ����ṹ */
  if ((pmemory = _alloc_MEMORY_STREAM()) == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream = (void*)pmemory;

  pmemory->mode = _analyst_for_mode(mode);

  if (pname->param1) { /* �Ѿ����ڵ��ڴ��� */
    pmemory->p_start_pos = (char*)(pname->param1);
    /* �����"w"��"w+"��ʽ�򿪵��ڴ������򳤶ȱ���Ϊ0 */
    pmemory->len         = ((pmemory->mode & (ISIO_MODE_WRITE | ISIO_MODE_CREATE)) == (ISIO_MODE_WRITE | ISIO_MODE_CREATE)) ? 0 : pname->param2;
    pmemory->total_size  = pname->param2;
    pmemory->cur_pos     = 0;
    pmemory->error       = (pmemory->len == pmemory->cur_pos) ? ISIO_ERROR_EOF : 0;
  }
  else {      /* �����ڵ��ڴ��� */
    if (!(pmemory->mode & ISIO_MODE_CREATE)) {
      _free_MEMORY_STREAM((MEMORY_STREAM*)(pfile->stream));
      return _free_ISFILE(pfile);   /* û�д������� */
    }

    /* �����ڴ�����һ�����䵥λ��������*/
    if (_add_memory_block(pmemory, 0) == -1) {
      _free_MEMORY_STREAM((MEMORY_STREAM*)(pfile->stream));
      return _free_ISFILE(pfile);   /* �ڴ治�� */
    }

    pmemory->len     = 0UL;
    pmemory->cur_pos = 0UL;
    pmemory->error   = ISIO_ERROR_EOF;
  }

  pmemory->pisaddr   = pname;       /* ��д�ṹ��ַ */
  pfile->stream_type = ISEEIO_CONFER_LOC_MEMORY;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_MEMORY_close(ISFILE* stream)
{
  /* ѹ���ڴ�β���Ŀ����ڴ��Խ�ʡ�ռ� */
  _dec_memory_block((MEMORY_STREAM*)stream->stream);

  assert(((MEMORY_STREAM*)(stream->stream))->p_start_pos);

  /* ��д�ڴ�����Ϣ

     ע�⣺* �������ر���ǰ������Open���ĵ�ַ�ṹ���ܱ�ɾ����
       * �ڴ��������ݿ鲻�ᱻClose�����������ͷţ��������ص�����
  */
  ((MEMORY_STREAM*)(stream->stream))->pisaddr->param1 = (unsigned long)(((MEMORY_STREAM*)(stream->stream))->p_start_pos);   /* �ڴ���׵�ַ */
  ((MEMORY_STREAM*)(stream->stream))->pisaddr->param2 = (unsigned long)(((MEMORY_STREAM*)(stream->stream))->len);       /* �ڴ�����ݵĳߴ� */

  /* �ͷ��ڴ����ṹ */
  _free_MEMORY_STREAM((MEMORY_STREAM*)(stream->stream));
  /* �ͷ����ṹ */
  _free_ISFILE(stream);

  return 0;
}


size_t CALLAGREEMENT LOC_MEMORY_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  MEMORY_STREAM* pmemory;
  int       i, step;

  pmemory = (MEMORY_STREAM*)stream->stream;

  if (pmemory->error & ISIO_ERROR_EOF) {
    assert(pmemory->len == pmemory->cur_pos);
    return 0;             /* �ѵ����ļ�β�� */
  }

  assert(pmemory->cur_pos < pmemory->len);

  /* �������ݵ�ָ���Ļ����� */
  for (i = 0; i < (int)count; i++) {
    if ((pmemory->cur_pos + size) > pmemory->len) {
      step = (int)(pmemory->len - pmemory->cur_pos);
    }
    else {
      step = size;
    }

    memcpy((void*)buffer, (const void*)(pmemory->p_start_pos + pmemory->cur_pos), step);

    /* ˢ������ǰָ�� */
    pmemory->cur_pos += (unsigned long)step;
    ((char*)buffer) += step;

    if (pmemory->cur_pos == pmemory->len) {
      pmemory->error |= ISIO_ERROR_EOF;

      if (step == (int)size) {
        i++;
      }

      break;
    }
  }

  return i;
}


size_t CALLAGREEMENT LOC_MEMORY_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  MEMORY_STREAM* pmemory;
  int       i, addsize;

  pmemory = (MEMORY_STREAM*)stream->stream;

  /* �ж�ָ���ڴ���Ƿ���д��׷�ӣ�Ȩ�� */
  if (!(pmemory->mode & (ISIO_MODE_WRITE | ISIO_MODE_APPEND))) {
    pmemory->error |= ISIO_ERROR_ERR;
    return 0;             /* û��дȨ�� */
  }

  /* ׷������������д���������ݶ�׷�ӵ��ļ�β�� */
  if (pmemory->mode & ISIO_MODE_APPEND) {
    pmemory->cur_pos = pmemory->len;
    pmemory->error |= ISIO_ERROR_EOF;
  }

  /* �ж��Ƿ���Ҫ�����ڴ�� */
  if ((pmemory->cur_pos + (unsigned long)(size * count)) > pmemory->total_size) {
    /* ����ʵ����Ҫ�������� */
    addsize = (pmemory->cur_pos + size * count) - pmemory->total_size;
    /* �ڴ��ߴ簴ָ������ */
    addsize = max(addsize, (int)pmemory->total_size);
    /* �߽���� */
    addsize = ((addsize + (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP - 1)) / (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP)) * (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP);

    /* �����ڴ��ߴ� */
    if (_add_memory_block(pmemory, addsize) == -1) {
      pmemory->error |= ISIO_ERROR_ERR;
      return 0;     /* �ڴ治�� */
    }

    assert((pmemory->cur_pos + (unsigned long)(size * count)) <= pmemory->total_size);
  }

  /* �������ݵ�ָ���Ļ����� */
  for (i = 0; i < (int)count; i++) {
    memcpy((void*)(pmemory->p_start_pos + pmemory->cur_pos), (const void*)buffer, size);

    pmemory->cur_pos += (unsigned long)size;
    ((char*)buffer) += (int)size;

    if (pmemory->cur_pos >= pmemory->len) {
      pmemory->len = pmemory->cur_pos;
      pmemory->error |= ISIO_ERROR_EOF;
    }

    assert(pmemory->cur_pos <= pmemory->len);
    assert(pmemory->len <= pmemory->total_size);
  }

  return i;
}


int CALLAGREEMENT LOC_MEMORY_flush(ISFILE* stream)
{
  return 0; /* �޻������������ˢ�� */
}


int CALLAGREEMENT LOC_MEMORY_seek(ISFILE* stream, long offset, int origin)
{
  MEMORY_STREAM* pmemory;
  unsigned long new_off = 0UL;

  pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  switch (origin) {
  case  SEEK_SET:
    new_off = (unsigned long)offset;
    break;

  case  SEEK_CUR:
    new_off = (unsigned long)((long)(pmemory->cur_pos) + offset);
    break;

  case  SEEK_END:
    new_off = (unsigned long)((long)(pmemory->len) + offset);
    break;

  default:
    assert(0);              /* �����ƴ��� */
    break;
  }

  if (new_off > pmemory->len) {     /* �Ƿ�λ�ã���ǰλ�ò��ı� */
    return -1;
  }

  if (new_off == pmemory->len) {
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~(ISIO_ERROR_EOF);
  }

  pmemory->cur_pos = new_off;

  return 0;
}


void CALLAGREEMENT LOC_MEMORY_rewind(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  pmemory->cur_pos = 0UL;

  if (pmemory->cur_pos == pmemory->len) {
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~(ISIO_ERROR_EOF);
  }

  return;
}


long CALLAGREEMENT LOC_MEMORY_tell(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  return (long)pmemory->cur_pos;
}


int CALLAGREEMENT LOC_MEMORY_eof(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* �ڴ�������� */

  return (pmemory->error & ISIO_ERROR_EOF) ? ISIO_ERROR_EOF : 0;
}


int CALLAGREEMENT LOC_MEMORY_error(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* �ڴ�������� */

  return (pmemory->error & ISIO_ERROR_ERR) ? ISIO_ERROR_ERR : 0;
}


void CALLAGREEMENT LOC_MEMORY_clearerr(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* �ڴ�������� */

  pmemory->error &= ~(ISIO_ERROR_ERR);  /* �������ָʾ�� */

  if (pmemory->cur_pos == pmemory->len) { /* ���ô����־ */
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~ISIO_ERROR_EOF;
  }

  return;
}


unsigned long CALLAGREEMENT LOC_MEMORY_length(ISFILE* stream)
{
  return ((MEMORY_STREAM*)(stream->stream))->len;
}









/**************************************************************************************
 * ISEEIO_CONFER_ROT_INTERNET - ���ʶ�д�ӿ�
 *************************************************************************************/
ISFILE* CALLAGREEMENT ROT_INTERNET_open(const char* pstream_name, const char* mode)
{
  return (ISFILE*)0;                  /* �ݲ�֧�� */
}


int CALLAGREEMENT ROT_INTERNET_close(ISFILE* stream)
{
  return EOF;
}


size_t CALLAGREEMENT ROT_INTERNET_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return 0;
}


size_t CALLAGREEMENT ROT_INTERNET_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return 0;
}


int CALLAGREEMENT ROT_INTERNET_flush(ISFILE* stream)
{
  return EOF;
}


int CALLAGREEMENT ROT_INTERNET_seek(ISFILE* stream, long offset, int origin)
{
  return -1;
}


void CALLAGREEMENT ROT_INTERNET_rewind(ISFILE* stream)
{
  return;
}


long CALLAGREEMENT ROT_INTERNET_tell(ISFILE* stream)
{
  return -1L;
}


int CALLAGREEMENT ROT_INTERNET_eof(ISFILE* stream)
{
  return 0;
}


int CALLAGREEMENT ROT_INTERNET_error(ISFILE* stream)
{
  return 0;
}


void CALLAGREEMENT ROT_INTERNET_clearerr(ISFILE* stream)
{
  return;
}


unsigned long CALLAGREEMENT ROT_INTERNET_length(ISFILE* stream)
{
  return 0UL;
}










/**************************************************************************************
 * ISEEIO_CONFER_LOC_FILE_SEQUENTIAL - �����ļ����ʿ��ٶ�д�ӿ�
 *************************************************************************************/
ISFILE* CALLAGREEMENT LOC_FILE_SEQUENTIAL_open(const char* pstream_name, const char* mode)
{
  PISADDR         pname = (PISADDR)pstream_name;
  ISFILE*          pfile = (ISFILE*)0;
  void*          pstream = (void*)0;
  LPCTSTR         lpFileName;
  DWORD         dwDesiredAccess;
  DWORD         dwShareMode;
  LPSECURITY_ATTRIBUTES lpSecurityAttributes;
  DWORD         dwCreationDisposition;
  DWORD         dwFlagsAndAttributes;
  HANDLE          hTemplateFile;

  /* ����ISFILE�ṹ�� */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;                /* �ڴ治�� */
  }

  lpFileName           = (const char*)pname->name;
  lpSecurityAttributes = NULL;
  dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN; /*���ٻ����Ż�*/
  hTemplateFile        = NULL;

  switch (mode[0]) {
  case 'a':
    dwDesiredAccess       = GENERIC_WRITE;
    dwShareMode           = FILE_SHARE_READ;
    dwCreationDisposition = OPEN_ALWAYS;
    break;

  case 'w':
    dwDesiredAccess       = GENERIC_WRITE;
    dwShareMode           = 0;
    dwCreationDisposition = CREATE_ALWAYS;
    break;

  case 'r':
    dwDesiredAccess       = GENERIC_READ;
    dwShareMode           = FILE_SHARE_READ;
    dwCreationDisposition = OPEN_EXISTING;
    break;

  default:
    assert(!"��֧�ֵ�ģʽ");
    return _free_ISFILE(pfile);
    break;
  }

  /* ��˳����ʷ�ʽ���ļ� */
  pstream = (void*)CreateFile(
      lpFileName,
      dwDesiredAccess,
      dwShareMode,
      lpSecurityAttributes,
      dwCreationDisposition,
      dwFlagsAndAttributes,
      hTemplateFile);

  if (pstream == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream      = pstream;
  pfile->stream_type = ISEEIO_CONFER_LOC_FILE_SEQUENTIAL;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_close(ISFILE* stream)
{
  int result;

  assert(stream->stream);
  result = !CloseHandle((HANDLE)stream->stream);
  _free_ISFILE(stream);

  return result;
}


size_t CALLAGREEMENT LOC_FILE_SEQUENTIAL_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  unsigned long nNumberOfBytesRead;

  if (!ReadFile((HANDLE)stream->stream, buffer, size * count, &nNumberOfBytesRead, NULL)) {
    nNumberOfBytesRead = 0;
  }
  else {
    nNumberOfBytesRead /= size;
  }

  return nNumberOfBytesRead;
}


size_t CALLAGREEMENT LOC_FILE_SEQUENTIAL_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  unsigned long nNumberOfBytesWritten;

  if (!WriteFile((HANDLE)stream->stream, buffer, size * count, &nNumberOfBytesWritten, NULL)) {
    nNumberOfBytesWritten = 0;
  }
  else {
    nNumberOfBytesWritten /= size;
  }

  return nNumberOfBytesWritten;
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_flush(ISFILE* stream)
{
  if (FlushFileBuffers((HANDLE)stream->stream)) {
    return 0;
  }
  else {
    return EOF;
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_seek(ISFILE* stream, long offset, int origin)
{
  long      lDistanceToMove;
  unsigned long dwMoveMethod;

  switch (origin) {
  case SEEK_SET:
    dwMoveMethod = FILE_BEGIN;
    break;

  case SEEK_CUR:
    dwMoveMethod = FILE_CURRENT;
    break;

  case SEEK_END:
    dwMoveMethod = FILE_END;
    break;

  default:
    assert(!"��֧�ֵĶ�λԭ��");
    return -1;
  }

  lDistanceToMove = offset;

  if (INVALID_SET_FILE_POINTER == SetFilePointer((HANDLE)stream->stream, lDistanceToMove, NULL, dwMoveMethod)) {
    return -1L;
  }
  else {
    return 0;
  }
}


void CALLAGREEMENT LOC_FILE_SEQUENTIAL_rewind(ISFILE* stream)
{
  SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_BEGIN);
}


long CALLAGREEMENT LOC_FILE_SEQUENTIAL_tell(ISFILE* stream)
{
  long  lDistanceToMove;

  lDistanceToMove = SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_CURRENT);

  if (INVALID_SET_FILE_POINTER ==  lDistanceToMove) {
    return -1L;
  }
  else {
    return lDistanceToMove;
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_eof(ISFILE* stream)
{
  long  lDistanceToMove;

  lDistanceToMove = SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_CURRENT);

  if (INVALID_SET_FILE_POINTER ==  lDistanceToMove) {
    return -1;
  }
  else {
    return (GetFileSize((HANDLE)stream->stream, NULL) == (unsigned long)lDistanceToMove);
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_error(ISFILE* stream)
{
  long  err = GetLastError();

  switch (err) {
  case  ERROR_FILE_NOT_FOUND:           /* ֻʶ�����ļ������йصĴ��� */
  case  ERROR_PATH_NOT_FOUND:
  case  ERROR_TOO_MANY_OPEN_FILES:
  case  ERROR_WRITE_PROTECT:
  case  ERROR_NOT_READY:
  case  ERROR_SEEK:
  case  ERROR_NOT_DOS_DISK:
  case  ERROR_WRITE_FAULT:
  case  ERROR_READ_FAULT:
  case  ERROR_SHARING_VIOLATION:
  case  ERROR_HANDLE_EOF:
  case  ERROR_HANDLE_DISK_FULL:
  case  ERROR_OPEN_FAILED:
  case  ERROR_BUFFER_OVERFLOW:
  case  ERROR_DISK_FULL:
  case  ERROR_INVALID_NAME:
  case  ERROR_SEEK_ON_DEVICE:
  case  ERROR_FILENAME_EXCED_RANGE:
    break;

  default:
    err = 0;                    /* ���Է��ļ���д����Ĵ��� */
    break;
  }

  return err;
}


void CALLAGREEMENT LOC_FILE_SEQUENTIAL_clearerr(ISFILE* stream)
{
  SetLastError(0);
}


unsigned long CALLAGREEMENT LOC_FILE_SEQUENTIAL_length(ISFILE* stream)
{
  return GetFileSize((HANDLE)stream->stream, NULL);
}



/**************************************************************************************
 * ISEEIO_CONFER_XXXXXX - ���ʶ�д�ӿ�
 *************************************************************************************/
/*
ISFILE * CALLAGREEMENT XXX_open(const char *pstream_name, const char *mode)
{
}


int CALLAGREEMENT XXX_close(ISFILE *stream)
{
}


size_t CALLAGREEMENT XXX_read(void *buffer, size_t size, size_t count, ISFILE *stream)
{
}


size_t CALLAGREEMENT XXX_write(const void *buffer, size_t size, size_t count, ISFILE *stream)
{
}


int CALLAGREEMENT XXX_flush(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_seek(ISFILE *stream, long offset, int origin)
{
}


void CALLAGREEMENT XXX_rewind(ISFILE *stream)
{
}


long CALLAGREEMENT XXX_tell(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_eof(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_error(ISFILE *stream)
{
}


void CALLAGREEMENT XXX_clearerr(ISFILE *stream)
{
}


unsigned long CALLAGREEMENT XXX_length(ISFILE *stream)
{
}

*/



