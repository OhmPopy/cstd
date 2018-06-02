#ifndef __VENUS_BOX_H_
#define __VENUS_BOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagProp {
  UINT Flags;        // ���Ա�ʶ
  UINT Count;
  UINT Value;
  UINT Hash;
  union {
    SPOT Role;      // ��ɫλ��
    struct {
      UINT RoleX;    // ��ɫˮƽ����
      UINT RoleY;    // ��ɫ��ֱ����
    };
  };
  union {
    SPOT Size;      // ��ɫλ��
    struct {
      UINT SizeX;    // ��ɫˮƽ����
      UINT SizeY;    // ��ɫ��ֱ����
    };
  };
  PSPOT Spots;
} PROP, *PPROP;

// Stage property flags
#define SPF_SPOT_ARRAY    0x00000001    // Ҫ�󷵻���������

// �٢ڢ�
// �ܡ��
// �ޢߢ�
#pragma pack(1)
typedef union tagSquare {
  struct {
    BYTE UpperLeft;    // ��
    BYTE UpperTop;   // ��
    BYTE UpperRight; // ��
    BYTE LeftSide;   // ��
    BYTE RightSide;    // ��
    BYTE BottomLeft; // ��
    BYTE BottomDown; // ��
    BYTE BottomRight;  // ��
  };
  struct {
    BYTE Left;     // Ŀ�����
    BYTE Core;     // Ŀ������
    BYTE Right;      // Ŀ���Ҳ�
    BYTE Front;      // Ŀ����ǰ
    BYTE UpHill;   // Ŀ����ǰ
    BYTE DownHill;   // Ŀ����ǰ
    //WORD Reserved;
  };
  __int64 Value;      // ���潩�־Ź�����, �����жϽ���: ��λ��������������ߴ�
} SQUARE, *PSQUARE;

//typedef struct tagIdle{
//
//} IDLE, *PIDLE;
#pragma pack()

typedef struct tagMove {
  UINT Index;        // ��������
  UINT Value;        // ��λ����
  union {
    SPOT Porter;    // ��ɫλ��
    struct {
      UINT PortX;    // ��ɫˮƽ����
      UINT PortY;    // ��ɫ��ֱ����
    };
  };
  union {
    SPOT Movement;    // Ѱ·λ��
    struct {
      UINT MoveX;    // Ѱ·ˮƽ����
      UINT MoveY;    // Ѱ·��ֱ����
    };
  };
  union {
    SPOT Object;    // ����λ��
    struct {
      UINT ObjX;   // Ѱ·ˮƽ����
      UINT ObjY;   // Ѱ·��ֱ����
    };
  };
  UINT DestId;     // Ŀ�굥Ԫ����
  UINT CubeId;     // ���ӵ�Ԫ����
  UINT RoleId;     // ��ɫ��Ԫ����
  union {
    UINT Flags;      // ��ʶ��ֵ��ѡ
    struct {
      BYTE Dest;   // Ŀ��λ�õ�ֵ(AL)
      BYTE Cube;   // ����λ�õ�ֵ(AX)
      BYTE Move;   // Ѱ·λ�õ�ֵ
      BYTE Role;   // ��ɫλ�õ�ֵ
    };
  };
} MOVE, *PMOVE;

#pragma pack(4)       // SPOT.ValueĬ����64λ����
typedef struct tagSnap {
  union {
    SPOT Position;    // �����ɫλ��
    struct {
      UINT PosX;   // ��ɫˮƽ����
      UINT PosY;   // ��ɫ��ֱ����
    };
  };
  UINT Hash;       // ���ղ���ָ��
  SPOT Stars[1];      // ����λ���б�(0Ԫ������Ϊ�Ǳ�׼��չ)
} SNAP, *PSNAP;
#pragma pack()

typedef struct tagShot {
  UINT Count;        // ���վ�������
  UINT Value;        // *** ��̬�ڴ�Ƶ���ط���Ч�ʵ�, Ԥ���������� ***
  UINT Size;       // ��λ���ճߴ�
  PSNAP Snaps;      // ���վ����б�
} SHOT, *PSHOT;

typedef struct tagStack {
  UINT Count;        // ��ջ��Ԫ����
  PSPOT Stars;      // ��ջ�����б�(����Ϊ��Ԫ��, ����ɨ���߼���)
} STACK, *PSTACK;

// Path finding callback
typedef int (* PFCB)(void* lpVoid);

#pragma pack(4)   // SPOT.Value(__int64)Ĭ����64λ����
typedef struct tagStage {
  UINT Volume;     // �ṹ��ʵ�ʴ�С(���ټ���)
  UINT Flags;        // ������ʶ
  PSPOT Stars;      // ����λ���б�(�ڲ�ָ��, ���ͷ�, ����Ϊ.Count)
  PSPOT Series;     // ������������(�ڲ�ָ��, ���ͷ�, ����Ϊ.Count)
  UINT Count;        // ��������(Ŀ������)
  UINT Value;        // ��λ����
  UINT Hash;       // ����ָ��(�������������ϣֵ)
  tagStage* Prev;     // �ϸ�����(��Ϸ�����Ӷ��в���, αָ��, ���ͷ�)
  tagStage* Next;     // �¸�����(��Ϸ�����Ӷ��в���, αָ��, ���ͷ�)
  tagStage* Host;     // ��������(���ʱ���򸸼������õ��ⷨ·��, αָ��, ���ͷ�)
  union {
    SPOT Size;      // �����ߴ�
    struct {
      long SizeX;   // �������(����)
      long SizeY;   // �����߶�(����)
    };
  };
  union {
    SPOT Position;    // ��ɫ��ǰλ��
    struct {
      long PosX;    // ��ɫˮƽλ��
      long PosY;    // ��ɫ��ֱλ��
    };
  };
  union {
    SPOT Automation;  // �Զ�Ѱ·λ��
    struct {
      long AutoX;   // Ѱ·ˮƽ����
      long AutoY;   // Ѱ·��ֱ����
    };
  };
  PMOVE Moves;      // �����߷��б�(�ڲ�ָ��, ���ͷ�, ����Ϊ.Count * 4: �ĸ�����)
  UINT Range;        // �����߷�����
  UINT Index;        // ��ǰ�����߷�
  UINT Slaves;     // ʣ��δ�������ӳ�������
  UINT Layer;        // ��ǰ����
  union {
    BYTE Matrix[1];    // ��������
    long Data;
  };
} STAGE, *PSTAGE;
#pragma pack()

// 100x100ռ10000�ֽ�, 1000��ռ10000000B Լ10M
// 256x256ռ65536�ֽ�, 1000��ռ65536000B Լ63M
// ȡ���ض�λ ^ 010;
// ��λ�ض�λ | 010;
// �����ض�λ & 101;
#define GetMatrix(s, X, Y)  s->Matrix[Y * s->SizeX + X]

// Stage file header
#define SFH_HEADER    0x786F622E  // .box�ļ�ͷ��

// Stage global flags
#define SGF_PRIME   0x00000000  // ԭʼ����(�½��༭, û����������)
#define SGF_BASIC   0x00000001  // ��������(��������, ������Ϸ)
#define SGF_EXTRA   0x00000002  // ��չ����(��չ����, �������)
#define SGF_READY   0x00000003  // ��������(��������)
#define SGF_ARRAY   0x00000004  // �Ӽ�����(����Ԫ��, �����ͷ�)

#define SGF_CRC32   0x10000000  // ��ϣУ��
#define SGF_DEBUG   0x80000000  // �������

// Stage matrix types
#define SMT_UNKNOW    0x00    // ��λ
#define SMT_OBJECT    0x01    // ����
#define SMT_TARGET    0x02    // Ŀ��
#define SMT_BORDER    0x04    // �߽�
#define SMT_PORTER    0x08    // ��ɫ
#define SMT_FILTER    0x0F    // ����

#define SMT_OPENED    0x10    // ���Ե��ﵱǰ��Ԫ

#define SMT_MASKED    0x05    // ���ӱ߽粻��ͨ��
#define SMT_OPNMSK    0x15    // ..
#define SMT_MATRIX    0x0F0F0F0F  // ��������

// Path pushing callback
typedef int (__cdecl* PFNPUSH)(PSTAGE pStage, UINT dwIndex, UINT dwFlags);

// Stage move flags
#define SMF_MOVE_NONE 0x00000000  // ��(Ӧ�ö���)

#define SMF_MOVE_RISE 0x00000001  // ��
#define SMF_MOVE_DROP 0x00000002  // ��
#define SMF_MOVE_BACK 0x00000003  // ��
#define SMF_MOVE_FORE 0x00000004  // ��
#define SMF_MOVE_MASK 0x0000000F  // ����(����ֻ����һ��)

#define SMF_MOVE_USED 0x00000010  // ����(�жϱ��ƶ���Ч)
#define SMF_MOVE_TEST 0x00000020  // ����(��Ϸʱ����ƶ�)

#define SMF_FLAG_HASH 0x10000000  // ʹ�ù�ϣ(ʹ��CRC32����У������ظ�)
#define SMF_FLAG_TEST 0x80000000  // �������
#define SMF_FLAG_MASK 0xFF000000  // ��ʶ����

// Stage prop flags
#define SPF_SAFE_CHECK    -1
#define SPF_UNIT_VALUE    -2
#define SPF_LAST_ERROR    -3
#define SPF_PREV_STAGE    -4
#define SPF_NEXT_STAGE    -5
#define SPF_HEAP_STAGE    -6
#define SPF_CALC_STAGE    -7
#define SPF_PUSH_STAGE    -8
#define SPF_PATH_STAGE    -9

typedef struct tagQueue { // ���ջ��ͬ, �Ƚ��ȳ�
  UINT Volume;     // ��������(�ֽ���)
  UINT Size;       // Ԫ���ڴ��С
  UINT Count;        // Ԫ����������
  UINT Value;        // ��ǰԪ�ظ���(�¸�����)
  UINT Used;       // ����Ԫ�ظ���
  UINT Step;       // �������
  PSTAGE Active;      // �׸������(�Ӵ˵���)
  PSTAGE Backup;      // ĩβ�����(���ѹ��)
  PSTAGE Stages;      // ���ڳ���(ѹ�뵯��)
  PSHOT Shots;      // ʧ�ܿ����б�(�ⲿָ��, �ⲿ�ͷ�)
  PSTACK Stacks;      // ɨ�������б�(�ⲿָ��, �ⲿ�ͷ�)
  union {
    BYTE Dummy[1];
    UINT Data;
  };
} QUEUE, *PQUEUE;

// Solution error codes
// �������ּ�: ����ļ�, ����㺯�����ø�λ
#define SEC_ERROR_NONE    0x00000000  // �ɹ����

#define SEC_FILE_OPEN   0x00000001  // �ļ���ʧ��
#define SEC_FILE_SIGN   0x00000002  // �ļ�������Ч
#define SEC_FILE_SIZE   0x00000003  // �ļ���С��Ч

#define SEC_CACHE_NULL    0x00000010  // �ڴ治��

#define SEC_PARAM_NULL    0x00000020  // ������Ч
#define SEC_PARAM_MANY    0x00000030  // ����Խ��

#define SEC_STAGE_ROLE    0x00000100  // ������ɫ���ָ���
#define SEC_STAGE_VARY    0x00000200  // ��������Ŀ�겻��
#define SEC_STAGE_SAVE    0x00000300  // �����½����ܱ���
#define SEC_STAGE_FLAG    0x00000400  // ����Ԫ�ز����ͷ�
#define SEC_STAGE_GAME    0x00000400  // �����½�������Ϸ

#define SEC_STAGE_NULL    1     // ��Чָ��
#define SEC_SOLVE_DONE    3     // �����Ѿ���λ
#define SEC_STAGE_ECHO    5     // ��������

#define SEC_ERROR_DUMP    0xFFFFFFFF  // ֻ����д

// ******** API ********
V32API PSTAGE  StageInit(void* lpVoid, PSPOT pSpot, UINT dwFlags);
V32API int  StageEdit(PSTAGE pStage, void* lpVoid, long dwPosX, long dwPosY);
V32API int  StageMove(PSTAGE pStage, void* lpVoid, UINT dwIndex, UINT dwFlags);
V32API int  StageProp(PSTAGE pStage, void* lpVoid, long dwPropId, UINT dwFlags);
V32API int  StageSave(PSTAGE pStage, void* lpVoid, UINT dwFlags);
V32API int  StageExit(PSTAGE pStage, UINT dwFlags);

#endif