
#include "cstd.h"
#include "str_format_len.inl"
#include "cfile.h"

int test_find_next_file_utf8() {
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  const char* path = "G:/Youku";
  path = "G:/Youku";
  _chdir(path);
  for (; find_next_file_utf8(finfo, path, "*.mp4", info, 0);) {
    char* name = GetFileNameExt(info->name);
    utf8_printf("%s\n", info->name);
    if (0) {
      WCHAR wbuf[256] = {0};
      int len = MultiByteToWideChar( CP_UTF8, 0, info->name, -1, wbuf, 256 );
      HANDLE hFile;//�ļ����
      FILE* pf = _wfopen(wbuf, L"rb");
      hFile=CreateFileW(
        wbuf,//������򿪵��ļ����豸������(������txt�ļ�)��
        GENERIC_READ,// �ļ�����Ȩ��,д
        0,//����ģʽ,��������0��ֹ�������̴��ļ����豸
        NULL,//SECURITY_ATTRIBUTES�ṹ����ȫ����������NULL����Ĭ�ϰ�ȫ����
        OPEN_EXISTING,//���ڴ��ڻ򲻴��ڵ�����ִ�еĲ�����������ʼ�մ���
        FILE_ATTRIBUTE_NORMAL,//�����ļ������ԣ������и��ٻ����ѡ��
        NULL);
      printf("%s\n", GetErrorMsg(0));
    }
  }
  return 0;
}
int test_codeformat() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  _chdir("E:/book");
  test_find_next_file_utf8();
  dirlist(dir, "E:/book", "*.pdf", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}
