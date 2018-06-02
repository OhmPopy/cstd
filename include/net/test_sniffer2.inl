/* June 2nd��2002
* Project for graduation qualification By Bby Team 19 */
#include <stdio.h>
#include <conio.h>
//�����·���������ͷ�ļ�packet32.h������ȥ
#include "..\..\Include\packet32.h"
#include "..\..\Include\ntddndis.h"
#define Max_Num_Adapter 10
// Prototypesԭ��
//����
void PrintPackets(LPPACKET lpPacket);
//�豸�б�
char AdapterList[Max_Num_Adapter][1024];
// ������ʼ
int main()
{
  ��//define a pointer to an ADAPTER strUCture�豸ָ��
  ��LPADAPTER lpAdapter = 0;
  ��//define a pointer to a PACKET structure��ָ��
  ��LPPACKET lpPacket;
  ��int i;
  ��DWord dwErrorCode;
  ��DWORD dwVersion;
  ��DWORD dwWindowsMajorVersion;
  ��//Unicode strings (WinNT)
  ��WCHAR AdapterName[8192]; //�����������豸�б�
  ��WCHAR* temp, *temp1;
  ��//ASCII strings (Win9x)
  ��char AdapterNamea[8192]; //�����������豸�б�
  ��char* tempa, *temp1a;
  ��int AdapterNum = 0, Open;
  ��ULONG AdapterLength;
  ��char buffer[256000]; // �������������������ݵĻ�����
  ��struct bpf_stat stat;
  ��// ��ñ���������
  ��AdapterLength = 4096;
  ��printf("Packet.dll test application. Library version:%s\n", PacketGetVersion());
  ��printf("Adapters installed:\n");
  ��i = 0;
  ����������δ����������ڲ�ͬ�汾�µõ���������������
  ����Win9x ��WinNT�е����������Ƿֱ���ASCII��UNICODEʵ�ֵģ���������Ҫ�õ����ز���ϵͳ�İ汾�ţ�
  dwVersion = GetVersion();
  dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
  �������������õ���Packet.dll������PacketGetAdapterNames��PTSTR pStr��PULONG BufferSize, ͨ����������������ͨ�Ų������õĵ�һ���������������ص��û�����ϵͳ�а�װ�����������������ַ��ڻ�����pStr�У�BufferSize�ǻ������ĳ��ȣ�
  if (!(dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4)) {
    ��//��Windows NT
    ��// �Ҳ����豸�б�
    ��if (PacketGetAdapterNames(AdapterName, &AdapterLength) == FALSE) {
      ����printf("Unable to retrieve the list of the adapters!\n");
      ����return -1;
      ��
    }
    ��// �ҵ��豸�б�
    ��temp = AdapterName;
    ��temp1 = AdapterName;
    ��while ((*temp != '\0')(*(temp - 1) != '\0'))
      �� {
      ����if (*temp == '\0')
        ����{
        ������memcpy(AdapterList[i], temp1, (temp - temp1) * 2);
        ������temp1 = temp + 1;
        ������i++;
        ����
      }
      ����temp++;
      ��
    }
    ��// ��ʾ�������б�
    ��AdapterNum = i;
    ��for (i = 0; i < AdapterNum; i++) {
      ����wprintf(L"\n%d- %s\n", i + 1, AdapterList[i]);
    }
    ����printf("\n");
    ��
  }
  ��else //�������windows 9x����ȡ���������ķ���ͬWinNT��
    �� {
    ����if (PacketGetAdapterNames(AdapterNamea, &AdapterLength) == FALSE) {
    ������printf("Unable to retrieve the list of the adapters!\n");
      ������return -1;
      ��
    }
  ��tempa = AdapterNamea;
  ��temp1a = AdapterNamea;
  ��while ((*tempa != '\0')(*(tempa - 1) != '\0'))
  �� {
  ����if (*tempa == '\0')
      ����{
      ������memcpy(AdapterList[i], temp1a, tempa - temp1a);
      ������temp1a = tempa + 1;
      ������i++;
      ����
    }
    ����tempa++;
    ��
  }
  ��AdapterNum = i;
  ��for (i = 0; i < AdapterNum; i++) {
  ����printf("\n%d- %s\n", i + 1, AdapterList[i]);
  }
  ����printf("\n");
  }
  ����������δ���������û�ѡ������������������ţ�
  // ѡ���豸
  do {
    ��printf("Select the number of the adapter to open : ");
    ��scanf("%d", &Open);
    ��if (Open > AdapterNum) {
      ����printf("\nThe number must be smaller than %d", AdapterNum);
    }
  }
  while (Open > AdapterNum);
  ����Ȼ�󣬽���ѡ����豸�򿪣������������Ϊ�����ӡ�ģʽ�򿪣�Ҳ�����ǡ�ֱ�ӡ�ģʽ�򿪡��������£�
  // ���豸
  lpAdapter = PacketOpenAdapter(AdapterList[Open - 1]);
  // ���豸�޷���ʱ����ʾ������Ϣ��
  if (!lpAdapter(lpAdapter->hFile == INVALID_HANDLE_VALUE)) {
    ��dwErrorCode = GetLastError();
    ��printf("Unable to open the adapter, Error Code : %lx\n", dwErrorCode);
    ��return -1;
  }
  ��������������Ϊ�����ӡ�ģʽ���������£�
  ���������õ�����PacketSetHwFilter��LPADAPTER AdapterObject��ULONG Filter�������ڵ����İ���������һ��Ӳ����������������ɹ�������TRUE��AdapterObject�ǹ��������ڵ������豸ָ�룻�������ĳ���Filter������ͷ�ļ�ntddndis.h �У������У�
  ������NDIS - PACKET - TYPE - PROMISCUOUS�����û���ģʽ��ÿ�������İ����ᱻ�������ܣ�
  ������NDIS - PACKET - TYPE - DIRECTED��ֻ��ֱ�ӵ����������İ��Żᱻ���ܣ�
  ������NDIS - PACKET - TYPE - BROADCAST��ֻ���ܹ㲥����
  ������NDIS - PACKET - TYPE - MULTICAST��ֻ���ܵ��������ڵ���Ķಥ����
  ������NDIS - PACKET - TYPE - ALL - MULTICAS������ÿ���ಥ�İ���
  // set the network adapter in promiscuous mode
  // �������ģʽ����ʧ�ܣ���ʾ����
  if (PacketSetHwFilter(lpAdapter, NDIS_PACKET_TYPE_PROMISCUOUS) == FALSE) {
    printf("Warning: unable to set promiscuous mode!\n");
  }
  ����Ȼ����driver����512K�Ļ��壺
  ���������õ�����PacketSetBuff��LPADAPTER AdapterObject��int dim����������������AdapterObjectָ�����������������Ļ��������ɹ��򷵻�TRUE��Dim���µĻ������Ĵ�С���������趨ʱ���ɻ������е����ݽ������������д洢�İ�Ҳ��ʧȥ��
  ������Ҫע�صĵط����������������Ĵ�С�����Ƿ�ǡ������Ӱ��ذ����̵����ܣ�����Ӧ�ܱ�֤���п��Ҳ��ᶪ�����������õ���512000Byte��
  // set a 512K buffer in the driver
  // ���޷����û�����ʱ����ʾ����
  if (PacketSetBuff(lpAdapter, 512000) == FALSE) {
    ��printf("Unable to set the kernel buffer!\n");
    ��return -1;
  }
  ����PacketSetReadTimeout��LPADAPTER AdapterObject��int timeout�������Ĺ����ǣ�������AdapterObjectָ�������󶨵Ķ�������ʱ��ֵ��timeout�Ժ���Ϊ��λ��0��ʾû�г�ʱ����û�а���ʱ��read�Ͳ����ء�
  // set a 1 second read timeout
  // ����1��Ķ�ȡ������ʱ
  if (PacketSetReadTimeout(lpAdapter, 1000) == FALSE) {
    ��printf("Warning: unable to set the read tiemout!\n");
  }
  ��������������λ�豸���������£�
  ���������õ�����PacketAllocatePacket��Void�������ڴ��з���һ��PACKET�ṹ������һ��ָ������ָ�룬������ṹ��Buffer�ֶλ�û���趨������Ӧ�ٵ���PacketInitPacket������������г�ʼ����
  //allocate and initialize a packet structure that will be used to
  //receive the packets.
  // ����λʧ��ʱ����ʾ����
  if ((lpPacket = PacketAllocatePacket()) == NULL) {
    ��printf("\nError: failed to allocate the LPPACKET structure.");
    ��return (-1);
  }
  ����Ȼ�󣬾Ϳ��Գ�ʼ���豸����ʼ����������ˣ�
  �����ú���PacketInitPacket��LPPACKET lpPacket��PVOID Buffer��UINT Length������ʼ��PACKET�ṹ��lpPacket��Ҫ����ʼ����ָ�룻BufferΪָ���û�����İ����������ݵĻ�������ָ�룻LengthΪ���������ȡ�
  ������Ҫע�صĵط���PACKET�ṹ�����Ļ������洢��packet capture driver �ػ�İ���������������������С�����ƣ���󻺳����Ĵ�С����Ӧ�ó������������һ���ܶ��������ݵĶ��١��������ô�Ļ������ɼ���ϵͳ���õĴ�������߽ػ�Ч�ʡ��������õ���256K��
  PacketInitPacket(lpPacket, (char*)buffer, 256000);
  �������������ǽذ���ѭ����
  //main capture loop
  �����������õ�����PacketReceivePacket��LPADAPTER AdapterObject��LPPACKET lpPacket��BOOLEAN Sync�����������ܣ��ػ�һ�����ļ��ϡ���������һ��ָ������ָ���ذ���������ADAPTER�ṹָ�롢һ��ָ���������ɰ���PACKET�ṹ��һ��ָ����ͬ�������첽��ʽ�����ı�ǡ�������ͬ��ʱ�������������򣻵������첽ʱ���������������򣬱������PacketWaitPacket����������Ƿ���ȷ��ɡ�һ�����ͬ��ģʽ��
  // ֱ���м��̼��룺
  while (!kbhit()) {
    ��// capture the packets ��׽��
    ��// ��׽��ʧ��ʱ����ʾ����
    ��if (PacketReceivePacket(lpAdapter, lpPacket, TRUE) == FALSE) {
      ����printf("Error: PacketReceivePacket failed");
      ����return (-1);
      ��
    }
    ��// ��ӡ���е����ݣ������Զ��庯��PrintPackets����
    ��PrintPackets(lpPacket);
  }
  ������󽫵õ���ͳ�����ݴ�ӡ�������������£�
  ���������õ�����PacketGetStats��LPADAPTER AdapterObject��struct bpf_star* s�����Եõ���������������ڲ�������ֵ���ӵ���PacketOpenAdapter��ʼ���Ѿ���ָ���������յİ���Ŀ���Լ��Ѿ����������յ����ں˶����İ���Ŀ��������ֵ���������򿽱���Ӧ���ṩ��bpf_stat�ṹ�С�
      //print the capture statistics
      // �õ�ͳ��ֵ
      // ���޷����ں˶�ȡ״̬ʱ����ʾ����
      if (PacketGetStats(lpAdapter, &stat) == FALSE) {
      ��printf("Warning: unable to get stats from the kernel!\n");
    }
  // ��ӡ��XX������ȡ��XX������������
    else {
      printf("\n\n%d packets received.\n%d Packets lost", stat.bs_recv, stat.bs_drop);
    }
  ���������ú���PacketFreePacket(LPPACKET lpPacket)���ͷ���lpPacketָ��Ľṹ��
  // �ͷſռ�
  PacketFreePacket(lpPacket);
  �ú���PacketCloseAdapter(LPADAPTER lpAdapter)���ͷ�ADAPTER�ṹlpAdapter�����ر�����ָ�룺
  // close the adapter and exit
  // �ر��豸�˳�
  PacketCloseAdapter(lpAdapter);
  return (0);
} // ���������

