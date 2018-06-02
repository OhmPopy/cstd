
#include "sqlite.inl"

static int _sql_callback(void* notused, int argc, char** argv, char** szColName)
{
  int i = 0;

  printf("notused:0x%x, argc:%d\n", notused, argc);

  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", szColName[i], argv[i] == 0 ? "NULL" : argv[i]);
  }

  printf("\n");
  return 0;
}

#include <direct.h>

int test_sqlite()
{
  const char* sSQL1 = "create table users(userid varchar(20) PRIMARY KEY, age int, birthday datetime);";
  const char* sSQL2 = "insert into users values('wang', 20, '1989-5-4');";
  const char* sSQL3 = "select * from users;";

  sqlite3* db = 0;
  char* pErrMsg = 0;
  int ret = 0;
  _chdir("E:/soft/���ݿ�/sqlitebrowser_200_b1_win");

  //�������ݿ�
  ret = sqlite3_open("./1.db", &db);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "�޷������ݿ⣺%s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  printf("���ݿ����ӳɹ�\n");

  sSQL1 = "select * from TWORD;";
  //ִ�н���SQL
  ret = sqlite3_exec(db, sSQL1, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL create error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg); //���Ҫ��Ŷ��Ҫ��Ȼ���ڴ�й¶��Ŷ������
    sqlite3_close(db);
    return 1;
  }

  printf("���ݿ⽨��ɹ�����\n");

  //ִ�в�������
  ret = sqlite3_exec(db, sSQL2, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL insert error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg); //���Ҫ��Ŷ��Ҫ��Ȼ���ڴ�й¶��Ŷ������
    sqlite3_close(db);
    return 1;
  }

  printf("���ݿ�������ݳɹ���\n");

  //ִ�в�ѯ����
  ret = sqlite3_exec(db, sSQL3, _sql_callback, 0, &pErrMsg);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", pErrMsg);
    sqlite3_free(pErrMsg);
    sqlite3_close(db);
    return 1;
  }

  printf("���ݿ��ѯ�ɹ�����\n");

  //�ر����ݿ�
  sqlite3_close(db);
  db = 0;

  return 0;
}

int test_db()
{
  test_sqlite();
  return 0;
}
