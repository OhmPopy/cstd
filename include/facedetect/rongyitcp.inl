
#ifndef _RONGYITCP_INL_
#define _RONGYITCP_INL_
// ���
int set32_b(void* buf, int& i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x>>24)&0xff;
  p[i++] = (x>>16)&0xff;
  p[i++] = (x>>8)&0xff;
  p[i++] = (x)&0xff;
  return i;
}
int set16_b(void* buf, int i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x>>8)&0xff;
  p[i++] = (x)&0xff;
  return i;
}
int set8_b(void* buf, int i, uint x) {
  unsigned char* p = (unsigned char*)buf;
  p[i++] = (x)&0xff;
  return i;
}
int set8_n(void* buf, int i, uint x, uint n) {
  unsigned char* p = (unsigned char*)buf;
  memset(p+i, x, n);
  return i+n;
}
int setp_n(void* buf, int i, const void* x, uint n) {
  unsigned char* p = (unsigned char*)buf;
  memcpy(p+i, x, n);
  return i+n;
}

int get32_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = (p[i]<<24)|
    (p[i+1]<<16)|
    (p[i+2]<<8)|
    (p[i+3]);
  return i+4;
}
int get16_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = (p[i]<<8)|
    (p[i+1]);
  return i+2;
}
int get8_b(const void* buf, int i, uint* x) {
  const unsigned char* p = (const unsigned char*)buf;
  *x = p[i++];
  return i;
}
int getp_n(const void* buf, int i, void* x, uint n) {
  const unsigned char* p = (const unsigned char*)buf;
  memcpy(x, p+i, n);
  return i+n;
}
typedef struct ID {
  char data[16];
}ID;

int set_pack_data(void* buf, int i, int buflen, uint head, uint type, ID sguid, ID dguid, uint smod, uint dmod, uint index, uint len, const void* data, uint datalen) {
  int len1 = i + 8 + 16+16+4+4+4+len;
  if (buflen>=len1) {
    i = set16_b(buf, i, head);
    i = set8_b(buf, i, type);
    i = set8_n(buf, i, 0, 5);
    i = setp_n(buf, i, sguid.data, 16);
    i = setp_n(buf, i, dguid.data, 16);
    i = set16_b(buf, i, smod);
    i = set16_b(buf, i, dmod);
    i = set32_b(buf, i, index);
    i = set32_b(buf, i, len);
    i = setp_n(buf, i, data, len);
  }
  return i;
}

int get_pack_data(const void* buf, int i, int buflen, uint* head, uint* type, ID* sguid, ID* dguid, uint* smod, uint* dmod, uint* index, uint* len, void* data, uint datalen) {
  i = get16_b(buf, i, head);
  i = get8_b(buf, i, type);
  i+=5;
  i = getp_n(buf, i, sguid->data, 16);
  i = getp_n(buf, i, dguid->data, 16);
  i = get16_b(buf, i, smod);
  i = get16_b(buf, i, dmod);
  i = get32_b(buf, i, index);
  i = get32_b(buf, i, len);
  if (*len<=datalen) {
    i = getp_n(buf, i, data, *len);
  }
  return i;
}

/* 
* ���ַ�ת��Ϊ��ֵ 
* */  
int c2i(char ch)  
{  
  // ��������֣��������ֵ�ASCII���ȥ48, ���ch = '2' ,�� '2' - 48 = 2  
  if(isdigit(ch))  
    return ch - 48;  
  
  // �������ĸ��������A~F,a~f�򷵻�  
  if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )  
    return -1;  
  
  // ����Ǵ�д��ĸ���������ֵ�ASCII���ȥ55, ���ch = 'A' ,�� 'A' - 55 = 10  
  // �����Сд��ĸ���������ֵ�ASCII���ȥ87, ���ch = 'a' ,�� 'a' - 87 = 10  
  if(isalpha(ch))  
    return isupper(ch) ? ch - 55 : ch - 87;  
  
  return -1;  
}  

/* 
* ���ܣ���ʮ�������ַ���ת��Ϊ����(int)��ֵ 
* */  
uint hex2dec(const char *hex)  
{  
  int len;  
  int num = 0;  
  int temp;  
  int bits;  
  int i;  
  
  // ������ hex = "1de" ����Ϊ3, hex��main�������ݵ�  
  len = strlen(hex);  
  
  for (i=0, temp=0; i<len; i++, temp=0)  
  {  
    // ��һ�Σ�i=0, *(hex + i) = *(hex + 0) = '1', ��temp = 1  
    // �ڶ��Σ�i=1, *(hex + i) = *(hex + 1) = 'd', ��temp = 13  
    // �����Σ�i=2, *(hex + i) = *(hex + 2) = 'd', ��temp = 14  
    temp = c2i( *(hex + i) );  
    // �ܹ�3λ��һ��16����λ�� 4 bit����  
    // ��һ�Σ�'1'Ϊ���λ������temp���� (len - i -1) * 4 = 2 * 4 = 8 λ  
    // �ڶ��Σ�'d'Ϊ�θ�λ������temp���� (len - i -1) * 4 = 1 * 4 = 4 λ  
    // �����Σ�'e'Ϊ���λ������temp���� (len - i -1) * 4 = 0 * 4 = 0 λ  
    bits = (len - i - 1) * 4;  
    temp = temp << bits;  
    
    // �˴�Ҳ������ num += temp;�����ۼ�  
    num = num | temp;  
  }  
  
  // ���ؽ��  
  return num;  
}
int hex2data(const char *hex, void* data, int datalen)   {
  int num = 0;  
  int temp;  
  int i, j=0;
  unsigned char* p = (unsigned char* )data;
  
  // ������ hex = "1de" ����Ϊ3, hex��main�������ݵ�  
  int len = strlen(hex);  
  memset(data, 0, datalen);
  for (i=0, temp=0; (j/2)<datalen && hex[i]; i++, temp=0)  
  {
    uint temp = (uint)c2i( hex[i] );  
    if (temp<16) {
      int bits = (!(j&1))*4;
      p[j/2]|=(temp<<bits);
      j++;
    }
  }
  num = (j+1)/2;
  if (num<datalen) {
    int off = datalen-num;
    memmove(p+off, p, num);
    memset(p, 0, off);
  }
  return num;
}
int data2hex(char *hex, int hexmaxlen, const void* data, int len)   {
  const unsigned char* p = (const unsigned char* )data;
  int i=0, j;
  for (j=0; j<len; ++j) {
    hex[i++] = "0123456789abcdef"[p[j]>>4];
    hex[i++] = "0123456789abcdef"[p[j]&0xf];
    hex[i++] = ' ';
  }
  hex[i] = 0;
  return i;
}


typedef enum {
  RYT_NO_NEED_CONFIRMATION = 0x71,//���ͱ��ģ��ñ��Ĳ���Ҫ���շ��յ�ȷ��
    RYT_NEED_CONFIRMATION = 0x51,//���ͱ��ģ��ñ�����Ҫ���շ��յ�ȷ��
    RYT_RESPONSE = 0x41,//���շ�Ӧ���ģ������յ��˱��ģ�����ʾҵ�����ݵĴ���������˸����ͱ�������������Ϊ0
    RYT_FAULT = 0x45,//���ϱ��ġ�
} RYT_MSG_TYPE;

typedef struct appid_t {
  ID guid;
  uint mod;
} appid_t;

int appid_find(const appid_t* ids, int nid, const appid_t* id) {
  int i;
  for (i=0; i<nid; ++i) {
    if (0==memcmp(ids+i, id, sizeof(appid_t))) {
      return i;
    }
  }
  return -1;
}
#define MAXAPPID  100

int appid_add(appid_t* ids, int nid, const appid_t* id, int* pnid) {
  if (nid<MAXAPPID) {
    int pos = appid_find(ids, nid, id);
    if (pos<0) {
      ids[nid++] = *id;
      *pnid = nid;
    }
    return 1;
  }
  return 0;
}
int appid_del(appid_t* ids, int nid, const appid_t* id, int* pnid) {
  if (nid>0) {
    int pos = appid_find(ids, nid, id);
    if (pos>0) { // 0�Ƿ����� ����ɾ��
      ids[pos] = ids[--nid];
      *pnid = nid;
    }
    return 1;
  }
  return 0;
}


#include "cjson.inl"
#include "net.h"

typedef struct rongyitcp_t {
  char recvbuf[1<<16];
  char sendbuf[1<<16];
  bool logined;// = false;
  bool facedetected;// = true;
  uint index;// = 0;
  appid_t appid[MAXAPPID];// = {0};
  int nid;// = 1;
  socket_info_t* si;
  addr_in addr[1];
  select_thd_t s[1];
} rongyitcp_t;
#define FACEDETECTMODE  2

int sock_open_tcp_server(const char* ip, int port, SOCK_TYPE type, addr_in* addr);
int sock_sendstr(int sock, char* str) {
  int len = strlen(str);
  return sock_send(sock, str, len, NULL);
}

int rongyitcp_send(rongyitcp_t* r, int sock, uint type, appid_t* appid, uint len, const char* data) {
  appid_t myappid = {0};
  uint head = 0x5231;
  myappid.mod = FACEDETECTMODE;
  int len1 = set_pack_data(r->sendbuf, 0, countof(r->sendbuf), head, type, myappid.guid, appid->guid, myappid.mod, appid->mod, r->index, len, data, countof(data));
  r->index++;
  return sock_send(sock, r->sendbuf, len1, NULL);
}

int rongyitcp_callback(int sock, void* user) {
  rongyitcp_t* r = (rongyitcp_t*)user;
  int ret = sock_recv(sock, r->recvbuf, countof(r->recvbuf), 0);
  uint head=0, type=0, index=0, len=0;
  appid_t sapp = {0}, dapp = {0};
  char data[1<<12];
  char retdata[1<<12];
  char sguid_str[64] = {0};
  char dguid_str[64] = {0};
  r->recvbuf[ret] = 0;
  *retdata = 0;
  get_pack_data(r->recvbuf, 0, ret, &head, &type, &sapp.guid, &dapp.guid, &sapp.mod, &dapp.mod, &index, &len, data, countof(data));
  //printf("%s\n", recvbuf);
  
  int len1 = len;
  if (len>=500) {
    len = 0;
  }
  data[len] = 0;
  data2hex(sguid_str, 64, sapp.guid.data, 16);
  data2hex(dguid_str, 64, dapp.guid.data, 16);
  printf("-----------\ntype=%02x \n sguid=%s \n dguid=%s \n smod=%d, dmod=%d index=%d\n len=%d data=%s\n",
    type, sguid_str, dguid_str, sapp.mod, dapp.mod, index, len1, data);
  
  {
    cJSON *pJsRoot = NULL;
    ret = 0;
    pJsRoot = cJSON_Parse(data);
    if (pJsRoot) {
      cJSON* pJsCmd = cJSON_GetObjectItem(pJsRoot, "cmd");
      if( pJsCmd && pJsCmd->type == cJSON_String ) {
        char* cmd = pJsCmd->valuestring;
        if (0==strcmp(cmd, "login")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "ret");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            r->logined = 0==pJsRet->valueint;
            printf("login ok\n");
          }
        }
        else if (0==strcmp(cmd, "facedetect")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "en");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            r->facedetected = pJsRet->valueint;
          }
          _snprintf(retdata, countof(retdata), "{\"cmd\":\"facedetect\",\"en\":%d}", r->facedetected);
        }
        else if (0==strcmp(cmd, "facedetectlisten")) {
          cJSON* pJsRet = cJSON_GetObjectItem(pJsRoot, "en");
          if( pJsRet && pJsRet->type == cJSON_Number ) {
            int facedetectlistened = pJsRet->valueint;
            if (facedetectlistened) {
              appid_add(r->appid, r->nid, &sapp, &r->nid);
            } else {
              appid_del(r->appid, r->nid, &sapp, &r->nid);
            }
            _snprintf(retdata, countof(retdata), "{\"cmd\":\"facedetectlisten\",\"en\":%d}", facedetectlistened);
          }
        }
      }
      cJSON_Delete(pJsRoot);
    }
  }
  if (type==RYT_NEED_CONFIRMATION) {
    rongyitcp_send(r, sock, RYT_NO_NEED_CONFIRMATION, &sapp, len, retdata);
  }
  //sock_sendstr(sock, recvbuf);
  return 0;
}

int memiszero(void* p, int len) {
  int i;
  char* c = (char*)p;
  for (i=0; i<len; ++i) {
    if (c[i]) return 0;
  }
  return 1;
}

rongyitcp_t r[1] = {0};

int rongyitcp_login(rongyitcp_t* r) {
  char data[256];
  _snprintf(data, countof(data), "{\"cmd\":\"login\",\"model\":\"%d\"}", FACEDETECTMODE);
  int len = strlen(data);
  rongyitcp_send(r, r->si->sock, RYT_NO_NEED_CONFIRMATION, r->appid, len, data);
  return 0;
}

int rongyitcp_open(rongyitcp_t* r, const char* ip, int port) {
  memset(r, 0, sizeof(*r));
  r->logined = false;
  r->facedetected = true;
  r->nid = 1;
  r->appid[0].guid.data[15] = 1; // ������
  r->appid[0].mod = 1;
  r->si = select_client_create(r->s, ip, port, rongyitcp_callback, r, r->addr);
  if (r->si!=NULL) {
    rongyitcp_login(r);
  }
  return r->si!=NULL;
}

int rongyitcp_facedetectinfo(rongyitcp_t* r, int faceindex = -1, int faceid = -1, int sideface = -1, int age = -1, int gender = -1, int glass = -1, int smile = -1) {
  int ctime = time(NULL);
  int ret = 0;
  char data[256];
  _snprintf(data, countof(data), "{\"cmd\": \"facedetectinfo\", \"faceindex\":%d, \"time\":%d, \"faceid\":-1, \"sideface\":-1, \"age\":-1, \"gender\":-1, \"glass\":-1, \"smile\":-1}", faceid, ctime);
  int i, len = strlen(data);
  for (i=0; i<r->nid; ++i) {
    if (!memiszero(r->appid+i, sizeof(appid_t))) {
      ret = rongyitcp_send(r, r->si->sock, RYT_NEED_CONFIRMATION, r->appid+i, len, data);
    }
  }
  return ret;
}

#endif // _RONGYITCP_INL_
