
// �ú���������Ϊ�򵥵ķ������и��٣�ǰ����ǰ���������׼ȷ������֡Ƶ����
// Edit Time: 2013.08.06
// �������壺
// ���룺
//       Video                  ������Ѿ�����õ�ǰ������ͼƬ
//       Frame                  ÿһ֡��ǰ�������������꣬������һ���ṹ�壬������֡
//       Trajectory1            ��һ֡������ͱ��
//       Trajectory2            �ڶ�֡������ͱ��
//       Trajectory3            ����֡������ͱ��
//       NumPeopleIn            ͳ��ĳһ��ʱ���ȥ������
//       NumPeopleOut           ͳ��ĳһ��ʱ�����������
//       NumEmpty45             ͳ��ĳһ��ʱ����������֡û��Ŀ����ֵ�֡��������ĳЩ������ʱ����
//       FlgOut                 �Ƿ�ִ��ͳ�Ƽ�����ǣ�ÿһ֡��ʼʱ���븳ֵΪ0
//       Startcoord             ���ڼ�¼ĳһĿ����γ��ֵ�ʱ������꣬���ﶨ��Ϊһ��1*2*100����ά����
//       Endcoord               ���ڼ�¼ĳһĿ����ʧ��ʱ������꣬���ﶨ��Ϊһ��1*2*100����ά����
//       RE                     ���ڼ�¼ÿһ�γ�����Ŀ���ǣ������ã�����ȷ���Ƿ���ȷ����
//       RI                     ���ڼ�¼ÿһ�ν�����Ŀ���ǣ������ã�����ȷ���Ƿ���ȷ����
//       DIN                    ������Ŀ����ʼ�������ʧ������ľ��룬������
//       DE                     ��ȥ��Ŀ����ʼ�������ʧ������ľ��룬������
//       DA                     �����ܹ���ͳ�Ƶ�Ŀ�����ʼ�������ʧ������ľ���
//       CountsStartcoordIn     ͳ�Ʊ�ʶ
//       CountsEndcoordOut      ͳ�Ʊ�ʶ
// �����������ͬ
//

#include "Frame_Euler.inl"
#define MAXMAN 16

// �����ƥ��
// id1==0 ��ʾ�Ѿ�ƥ��
int ptMatch(int n1, POINT* pt1, int* id1, int n2, POINT* pt2, int* id2)
{
  int i, j, n = 0;

  if (2 == n2) {
    int adsf = 0;
  }

  for (; 1;) {
    int mind = 100, mini = -1, minj = -1;

    for (i = 0; i < n1; ++i) {
      for (j = 0; j < n2; ++j) {
        if (id1[i] > 0 && id2[j] == 0) {
          int x = pt2[j].x - pt1[i].x, y = pt2[j].y - pt1[i].y;
          int d = (int)sqrt((double)x * x + y * y);

          if (d < mind) {
            mind = d, mini = i, minj = j;
            ++n;
          }
        }
      }
    }

    if (mini >= 0 && minj >= 0) {
      id2[minj] = id1[mini];
      id1[mini] = 0; // ��ʾ�Ѿ�ƥ����ˣ��´β���ƥ��
    }
    else {
      return n;
    }
  }

  return 0;
}

typedef struct TrackingAndCaculateData {
  int RE, RI, DIN, DE, DA;
  int Startcoord, Endcoord, NumPeopleOut, NumPeopleIn, NumEmpty45;
  int CountsStartcoordIn, CountsEndcoordOut;
  int FlgOut;
  int Trajectory1[MAXMAN][3], Trajectory2[MAXMAN][3], Trajectory3[MAXMAN][3];
  POINT S[MAXMAN];
  POINT Frame[6][MAXMAN];
  int NumS;
  int NumFrame1;
  int NumFrame2;
  int NumFrame3;
  int NumFrame4;
  int NumFrame5;
} TrackingAndCaculateData;

int TrackingAndCaculate(TrackingAndCaculateData* td)
{
  int i, j = 0;
  double d32[MAXMAN * MAXMAN];
  int Index32[MAXMAN];
  td->NumFrame5 = td->NumS;
  memcpy(td->Frame[5], td->S, sizeof(POINT)*td->NumFrame5);

  for (i = 0; i < td->NumFrame3; ++i) { // ��¼��ǰ����֡�����꣬������Ƿ��ű��Ϊ0
    td->Trajectory3[i][0] = td->Frame[3][i].x;
    td->Trajectory3[i][1] = td->Frame[3][i].y;
    td->Trajectory3[i][2] = 0;
  }

  Frame_Euler(td->NumFrame2, td->Frame[2], td->NumFrame2, td->Frame[3], d32, Index32); // ��֮֡���ŷ�Ͼ���


#if 0

  if NumFrame1 == 0 && NumFrame2 == 0 && NumFrame3~ = 0 // ǰ��֡Ϊ�գ�����֡(��ǰ����֡)Ϊ�ǿյ�ʱ����֮Ϊ��ʼ���
    for ij = 1:
            NumFrame3
            CountsStartcoordIn = CountsStartcoordIn + 1; // ��Ǳ�ʶ�������ۼӵķ�ʽ���б��

Startcoord(1, : , CountsStartcoordIn) = Frame {3}(ij).Centroid; // ��������Ŀ�����ʼ����
  Trajectory3(ij, 1: 2) = Frame {3}(ij).Centroid; // ��¼��ǰ����֡��Ŀ������
  Trajectory3(ij, 3) = CountsStartcoordIn; // ���
  end
  Frame {1} = Frame {2}; // ����
  Frame {2} = Frame {3};
  Frame {3} = Frame {4};
  Frame {4} = Frame {5};

  Trajectory1 = Trajectory2; // ����
  Trajectory2 = Trajectory3;
  Trajectory3 = [];
  elseif NumFrame1 + NumFrame2 + NumFrame3~ = 0 // ����֡����һ֡�ǿգ���ִ����Ӧ�Ĵ���

      if isempty(d32) // ���d32Ϊ��
        if NumFrame2~ = 0 // ���ڶ�֡�ǿգ�����֡Ϊ�յ�ʱ��˵����ʱ���һ���߳���Ŀ��
        for ijh = 1:
                  NumFrame2
      disp('ע��Ŀ���߳������,�������һ��') {
        CountsEndcoordOut = CountsEndcoordOut + 1;  // �߳�ͼ��ļ���������ȷ����ʧ�������ŵ���
      }

  FlgOut = 1; // ָʾ��Ǹ�ֵΪ1����ʾ��Ŀ����ʧ
Endcoord(1, : , Trajectory2(ijh, 3)) = Trajectory2(ijh, 1: 2); // ���浱ǰ��ʧĿ�����ʧ����
  disp(['CountsEndcoordOut: ', num2str(CountsEndcoordOut), '  Trajectory2(IndexList(ij),1:2): ', num2str(Trajectory2(ijh, 3))])
  end
  end
  elseif ~isempty(d32)  // �ڶ�֡�͵���֡��Ϊ�ǿյ�ʱ��
  TempList32 = Index32;
  UniqueList32 = unique(Index32); // ����Index32�е����ݷ�ӳ�У���ʾ����֡��Ŀ��λ�ã���Index32����������ӳ���ǵڶ�֡Ŀ��λ�ã�������ȡΨһ������Ϣ
  NU32 = size(UniqueList32, 2); // ����������Ŀ����ʵ���ǵ���֡��Ŀ����Ŀ

for ij = 1:
      NU32 // �ڵ���֡��Ѱ�����ƥ���Ŀ�꣬������ǣ������Ѿ�������Ŀ�겻�����������
      IndexEqual = find(TempList32 == UniqueList32(ij)); // ����Ѱ��

  NumEqual = sum(IndexEqual~ = 0);

  if NumEqual == 1 // ֻ��һ��ƥ��Ŀ��
  if NumFrame3 == 1 // ������ֻ֡��һ��Ŀ��ʱ�������޷����ɶ�ά�ľ������Ե�������
    Trajectory3(IndexEqual, 3) {
        = Trajectory2(Index32(IndexEqual), 3);
    }
    else { // �ڶ�֡Ŀ����ĿΪ1������֡Ŀ����Ŀ����Ϊ1
      Trajectory3(Index32(IndexEqual), 3) = Trajectory2(IndexEqual, 3);
    }

  end
  elseif NumEqual > 1 // ���ڶ��ƥ�䣬Ѱ�����ƥ��
  TempValue = zeros(1, NumEqual);

for ijk = 1:
      NumEqual // Ѱ��ƥ�����Сֵ
  TempValue(1, ijk) {
      = d32(Index32(IndexEqual(ijk)), IndexEqual(ijk));
  }

  end
  [CValue CIndex] = min(TempValue); // ���ƥ��
  Trajectory3(Index32(IndexEqual(CIndex)), 3) = Trajectory2(IndexEqual(CIndex), 3); // �Ӷ��ƥ����Ѱ�����ƥ��
  end
  end

NumMatch3 = sum((Trajectory3(: , 3)~ = 0)); // ����֡�͵ڶ�֡ƥ���Ŀ����Ŀ
  TempLeftList3 = Trajectory3;

  if NumMatch3 < NumFrame3 // ��ƥ����ĿС�ڵ���֡Ŀ����Ŀ����ʾ���³��ֵ�Ŀ��
IndexList3 = find(Trajectory3(: , 3) == 0); // Ѱ���³��ֵ�Ŀ����Trajectory3�ж�Ӧ����

  Nempty3 = size(IndexList3, 1); // �³���Ŀ�����Ŀ

for ij = 1:
      Nempty3 // ���³��ֵ�Ŀ������ǣ�����������ʼ����
      CountsStartcoordIn = CountsStartcoordIn + 1;

Trajectory3(IndexList3(ij), 3) = CountsStartcoordIn;
Startcoord(1, : , CountsStartcoordIn) = Trajectory3(IndexList3(ij), 1: 2);
  end
  end

  if NumMatch3 < NumFrame2 // ��ƥ�����ĿС�ڵڶ�֡Ŀ����Ŀ��ʱ�򣬱�ʾ��Ŀ����ʧ
IndexList32 = Trajectory2(: , 3);

for ij = 1:
      NumFrame3 // Ѱ����ʧ��Ŀ����
  IndexList32(IndexList32 == TempLeftList3(ij, 3)) {
      = [];
  }

  end
  IndexList = IndexList32;

for ij = 1:
  size(IndexList, 1) { // ��¼��ʧĿ�����ʧ������
    CountsEndcoordOut = CountsEndcoordOut + 1;
  }

  FlgOut = 1;
Tij = find(Trajectory2(: , 3) == IndexList(ij));
Endcoord(1, : , Trajectory2(Tij, 3)) = Trajectory2(Tij, 1: 2);
  disp(['CountsEndcoordOut: ', num2str(CountsEndcoordOut), '  Trajectory2(IndexList(ij),3): ', num2str(Trajectory2(Tij, 3))])
  end
  end
  //             if NumMatch3==NumFrame3 && NumFrame3==NumFrame2
  //                 for ij=1:NumFrame3
  //                     Trajectory3(ij,3)=Trajectory2(Index32(ij),3);
  //                 end
  //             else
  //                 disp('û���ǵ������')
  //             end
  end

  if FlgOut == 1 // �����Ŀ����ʧ
  if NumFrame3 > 0 // ����֡����Ŀ��
TempList3 = Trajectory3(: , 3);

TempList2 = Trajectory2(: , 3);

for ij = 1:
      NumFrame3 // �ҳ���ʧ��Ŀ����
  TempList2(TempList2 == TempList3(ij)) {
      = [];
  }

  end
  elseif NumFrame3 == 0 // ����֡û��Ŀ��
TempList2 = Trajectory2(: , 3);
  end

  NOut = size(TempList2, 1); // ��ʧ��Ŀ����Ŀ

for ij = 1:
      NOut
    DStartEnd = norm(Startcoord(: , : , TempList2(ij)) - Endcoord(: , : , TempList2(ij))); // ������ʧĿ�����ʼ�����ʧ��ľ���

  DA = [DA DStartEnd]; // ������룬�����ڲ���
TStart = Startcoord(: , : , TempList2(ij)); // ��ʼ������
TEnd = Endcoord(: , : , TempList2(ij)); // ��ʧ������

  if DStartEnd > 100 && TEnd(1) > 200 && TStart(1) < 40 // �����ж�
    NumPeopleIn = NumPeopleIn + 1; // ����������ͳ��

  RI = [RI TempList2]; // ��ʶ���棬������
  DIN = [DIN DStartEnd]; // ���뱣�棬������
  end

  if DStartEnd > 100 && TEnd(1) < 40 && TStart(1) > 200 // �����ж�
    NumPeopleOut = NumPeopleOut + 1; // ��ȥ������ͳ��

  RE = [RE TempList2]; // ��ʶ���棬������
  DE = [DE DStartEnd]; // ���뱣�棬������
  end
  end
  end

  if NumFrame4~ = NumFrame3 && NumFrame3 == NumFrame5 // ���ڵ���֡Ŀ����Ŀ�͵���֡Ŀ����Ŀ��ͬ������֡Ŀ����Ŀ�͵���֡Ŀ����Ŀ��ͬ����ɾ��������
      Frame {4} = []; // ɾ��������
  Frame {1} = Frame {2}; // ����
  Frame {2} = Frame {3};
  Frame {3} = Frame {5};
  Frame {4} = Frame {5};

  Trajectory1 = Trajectory2; // ����
  Trajectory2 = Trajectory3;
  Trajectory3 = [];

  else
    Frame {1} = Frame {2}; // һ������µĸ���
  Frame {2} = Frame {3};
  Frame {3} = Frame {4};
  Frame {4} = Frame {5};

  Trajectory1 = Trajectory2; // һ������µĸ���
  Trajectory2 = Trajectory3;
  Trajectory3 = [];
  end
  else
    disp('������֡Ϊ0����ʼ��')
    if NumFrame3 == 0 && NumFrame4 == 0 && NumFrame5 == 0 || NumFrame1 == 0 && NumFrame2 == 0 && NumFrame3 == 0 // ͳ��������֡Ϊ0���������������Ŀ����һ����Ŀʱ����ĳЩ�������г�ʼ��
    NumEmpty45 = NumEmpty45 + 1; // ͳ��������֡Ϊ0�Ĵ���

if NumEmpty45 > 10 //������������һ����ֵ��ʱ�򣬶�Startcoord,Endcoord,CountsStartcoord,CountsStartcoord,NumEmpty45���и���
  Startcoord = zeros(1, 2, 100); // Ҫ���ö�ʱ����

  CountsStartcoordIn = 0;
  CountsEndcoordOut = 0;
  Endcoord = zeros(1, 2, 100); // Ҫ���ö�ʱ����
  NumEmpty45 = 0;
  end
  end
  Frame {1} = Frame {2}; // ����
  Frame {2} = Frame {3};
  Frame {3} = Frame {4};
  Frame {4} = Frame {5};

  Trajectory1 = Trajectory2; // ����
  Trajectory2 = Trajectory3;
  Trajectory3 = [];
  end


#endif
  return 0;
}