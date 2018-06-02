
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int DWORD;
typedef int BOOL;

#define SeekToBegin(fp)  fseek(fp, 0, SEEK_SET)
#define SeekToEnd(fp)    fseek(fp, 0, SEEK_END)

#define FALSE 0
#define TRUE  1

//�������ĺ���
__int64 epass( const char* ppass ) {
  int i;
  __int64 mc = 8757735233305;
  
  int plen = strlen( ppass );
  
  for ( i = 0; i < ( int ) plen; i++ ) {
    mc ^= ppass[ i ] | ( 128 );
  }
  return mc;
}

int ecode( int flen, char* data, __int64 m_password ) {
  int i;
  for ( i = 0; i < ( int ) flen; i++ ) {
    data[ i ] ^= m_password;
    data[ i ] ^= flen;
  }
  return 0;
}

//���ļ����ܵĺ���
// type = 0 ����
// type = 1 ����
BOOL edcfile( const char* infile, const char* oufile, const char* ppass, int type ) {
  int i, j;
  char *data;
  FILE *file;
  DWORD flen;
  char cpass[ 5 ] = "love";
  __int64 m_password = epass( ppass );
  if ( !oufile ) {
    oufile = infile;
  }
  
  file = fopen( infile, "rb" );
  if ( !file ) {
    return FALSE;
  }
  
  SeekToEnd( file );
  flen = ftell( file );
  SeekToBegin( file );
  if ( type ) {
    char love[ 5 ];
    //���������ǲ�����ȷ
    fread( &love, 5, 1, file );
    for ( i = 0; i < 5; i++ ) {
      love[ i ] ^= m_password;
    }
    if ( strcmp( love, cpass ) != 0 ) {
      return FALSE;
    }
    flen -= 5;
  }
  
  data = malloc( sizeof( char ) * ( int ) flen );
  fread( data, flen, 1, file );
  fclose( file );
  
  //���ܻ����
  ecode( flen, data, m_password );
  
  file = fopen( oufile, "wb" );
  if ( !type ) {
    //���������֤��Ϣ
    for ( j = 0; j < 5; j++ ) {
      cpass[ j ] ^= m_password;
    }
    SeekToBegin( file );
    fwrite( &cpass, 5, 1, file );
  }
  fwrite( data, flen, 1, file );
  free( data );
  
  fclose( file );
  return TRUE;
}

