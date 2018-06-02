// �Լ�д�ļ���Ⱥ�۹���
// 2016-03-18 14:15 339���Ķ� ����(1) �ղ� �ٱ�
// ���ࣺ
// ��ѧ�ʼǣ�39��
// 
// ��Ȩ����������Ϊ����ԭ�����£�δ������������ת�ء�
// ��������û�Ⱥ2015-09-26 09:55:59??
// ���������û�Ⱥ���ߵ�C++����ʵ�֣�2014-6-28��2014-07-23 15:47:25
// ��������Ⱥ�����ġ���λ��Ⱥ2014-06-06 10:10:40
// �������ϵľ���˷�������ʽ����������2014-04-25 10:44:52
// �ж�nԪ�û������Ƿ񹹳�S_n����Ⱥ��C++����ʵ��2013-07-16 22:00:46
// 
// ����n�׶Գ�ȺȺԪ���ϵ��ȶ��㷨�������������
// 12!=479001600
#if 0
using System;
namespace Sn

����n�׶Գ�ȺȺԪ���ϵĲ��ȶ��㷨
// abcdef.exe
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;
vector<int> v6;
vector<string> s6;
// ����ת��Ϊ�ַ���
string itos(int i)
{
  stringstream s;
  s << i;
  return s.str();
}
int GetRand(int a,int b){
  if(a>=b)
    return a;
  int iRet=rand()%(b-a+1)+a;
  return iRet;
}
string GenS6Element()
{
  string str;
  vector<int> v6c=v6;
  while(v6c.size()>0)
  {
    int i=GetRand(0,v6c.size()-1);
    if(v6c.size()==v6.size())
      str=str+"\"";
    str=str+itos(v6c[i]);
    if(v6c.size()==1)
      str=str+"\"";
    str=str+",";
    v6c.erase(v6c.begin()+i);
  }
  return str;
}
bool GenS6()
{
  while(s6.size()<720)
  {
    string str=GenS6Element();
    vector<string>::iterator iter = find(s6.begin(),s6.end(),str);
    if(iter!=s6.end()){
      //�ҵ���
    }
    else{
      //�Ҳ���
      s6.push_back(str);
    }
  }
  return true;
}

int main() {
  time_t now=time(0);
  srand(now);
  for(int i=0;i<6;i++)
    v6.push_back(i+1);
  bool bRet=GenS6();
  string fn="abcdef"+itos(now)+".txt";
  ofstream fout(fn);
  fout << "static char *abcdef[] = {"<<endl;
  for(int i=0;i<s6.size();i++){
    fout<<s6[i]<<endl;
  }
  fout<<"};"<<endl;
  cout<<"bRet="<<bRet<<endl;
  system("pause");
  return 0;
}
// S6_2_14.9.txt
// 0,0,0,0,0,0
// 4,2,3,6,1,5
// 3,5,4,1,6,2
// ��FG.exe��������һ��24��Ⱥ�����õ����Ŀ��������S6_2_14.9.txt.txt
// ����calG.exe�������õ�S6_2_14.9.txt_ElementToOrder.txt
// G24ElementToOrder(0)=1
// G24ElementToOrder(1)=4
// G24ElementToOrder(2)=3
// G24ElementToOrder(3)=2
// G24ElementToOrder(4)=4
// G24ElementToOrder(5)=4
// G24ElementToOrder(6)=3
// G24ElementToOrder(7)=4
// G24ElementToOrder(8)=3
// G24ElementToOrder(9)=3
// G24ElementToOrder(10)=2
// G24ElementToOrder(11)=3
// G24ElementToOrder(12)=2
// G24ElementToOrder(13)=2
// G24ElementToOrder(14)=2
// G24ElementToOrder(15)=3
// G24ElementToOrder(16)=2
// G24ElementToOrder(17)=2
// G24ElementToOrder(18)=2
// G24ElementToOrder(19)=4
// G24ElementToOrder(20)=3
// G24ElementToOrder(21)=4
// G24ElementToOrder(22)=3
// G24ElementToOrder(23)=2
// S6_2_14.9.txt��1��1��Ԫ��9��2��Ԫ��8��3��Ԫ��6��4��Ԫ��0��6��Ԫ��0��8��Ԫ��0��12��Ԫ��0��24��Ԫ
// ��
// ֤��ż����Ⱥ�غ�2��Ԫ��
// ���취֤����Ⱥ��Ϊż������Ϊ2n��,��Ⱥ�б���һԪ��a��a��2n��Ϊe, a ��1�ף�2�ף�һֱ��2n�ױ���Ⱥ�У�a��n�׼�Ϊ��Ϊ2��Ԫ�ء�
// ��������������Sylow��һ����G������Ⱥ��p�����������p^k||G|��k>=0����ôG��һ����һ����Ϊp^k����Ⱥ����p=2��k=1����G��һ��2����Ⱥ������G��һ����2��Ԫ��
// ��
// RandMakeS6SubGroup.exe
#include <time.h>
#include <fstream>
using namespace std;

static char *abcdef[] = {
  "1,4,6,5,2,3",
    "1,5,6,3,4,2",
    "1,6,5,2,4,3",
    "1,6,4,3,2,5",
    "2,3,5,6,5,1",
    "2,5,3,4,1,6",
    "2,6,4,1,5,3",
    "3,2,6,5,4,1",
    "3,4,5,2,6,1",
    "3,5,4,1,6,2",
    "3,6,2,1,4,5",
    "4,1,5,6,3,2",
    "4,3,2,5,1,6",
    "4,5,1,2,3,6",
    "4,2,3,6,1,5",
    "5,1,3,6,2,4",
    "5,2,4,3,6,1",
    "5,3,1,4,2,6",
    "5,4,2,1,6,3",
    "6,1,3,4,5,2",
    "6,1,2,5,3,4",
    "6,2,1,4,3,5",    
    "6,3,1,2,5,4"
};
int GetRand(int a,int b){
  if(a>=b)
    return a;
  int iRet=rand()%(b-a+1)+a;
  return iRet;
}
int main() {
  srand(time(0));
  int n=sizeof(abcdef)/sizeof(abcdef[0]);
  int m=GetRand(2,4);
  int* p=new int[m];
  char sz[100]={0};
  sprintf(sz,"S6_%d_",m);
  for(int i=0;i<m;i++){
    p[i]=GetRand(0,n-1);
    char sz1[100]={0};
    sprintf(sz1,"%d.",p[i]);
    strcat(sz,sz1);
  }
  strcat(sz,"txt");
  ofstream fout(sz);
  fout << "0,0,0,0,0,0"<<endl;
  for(int i=0;i<m;i++){
    fout<<abcdef[p[i]]<<endl;
  }
  delete[] p;
  puts(sz);
  system("pause");
  return 0;
}

// S_3=<(1,4,3,2,5),(1,2,3,5,4)>,S_3����ΪS_4
// 
// ----24�׷ǽ���ȺS_4ȺԪ�Ľ�----
// 
// S_4=<(2,3,5,4,1),(1,3,2,4,5)>=<(2,4,6,1,3,5),(1,3,5,2,4,6)>=<(1,4,3,2,5),(1,2,3,5,4),(2,1,3,4,5)>��1��1��Ԫ��9��2��Ԫ��8��3��Ԫ��6��4��Ԫ��0��6��Ԫ��0��8��Ԫ��0��12��Ԫ��0��24��Ԫ
// 
// ----24�׷ǽ���ȺA_4��C_2=<(6,5,1,2,3,4),(1,2,3,4,6,5)>=<(6,5,1,2,3,4),(2,1,3,4,5,6)>==<(6,5,1,2,3,4),(1,2,4,3,5,6)>ȺԪ�Ľ�----

//1��1��Ԫ��7��2��Ԫ��8��3��Ԫ��8��6��Ԫ
// G20_4=F_20(Frobenius group F_20)=<(1,3,2,5,4),(2,3,4,1,5)>��1��1��Ԫ��5��2��Ԫ��10��4��Ԫ��4��5��Ԫ��0��10��Ԫ��0��20��Ԫ
// S_4=<(1,2,3,5,4),(1,3,4,2,5)>��1��1��Ԫ��9��2��Ԫ��8��3��Ԫ��6��4��Ԫ��0��6��Ԫ��0��8��Ԫ��0��12��Ԫ��0��24��Ԫ

//20140628����11������R��N���û�����Ԫ�������������û�Ⱥ�Ŀ������С����FG.exe
/*
#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;
*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

std::vector<string> split( const std::string& str, const std::string& delims, unsigned int maxSplits = 0)
{
  std::vector<string> ret;
  unsigned int numSplits = 0;
  // Use STL methods
  size_t start, pos;
  start = 0;
  do
  {
    pos = str.find_first_of(delims, start);
    if (pos == start)
    {
      // Do nothing
      start = pos + 1;
    }
    else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
    {
      // Copy the rest of the std::string
      ret.push_back( str.substr(start) );
      break;
    }
    else
    {
      // Copy up to delimiter
      ret.push_back( str.substr(start, pos - start) );
      start = pos + 1;
    }
    // parse up to next real data
    start = str.find_first_not_of(delims, start);
    ++numSplits;
  } while (pos != std::string::npos);
  return ret;
}

// S_N���û��˷�m*n
vector<int> Mul(int N,const vector<int> & m,const vector<int> & n)
{
  vector<int> tArr(N);
  vector<int> aArr(N);
  vector<int> taArr(N);
  memcpy(&tArr[0],&m[0],sizeof(tArr[0])*N);
  memcpy(&aArr[0],&n[0],sizeof(aArr[0])*N);
  for(int i=0;i<N;i++)
    taArr[i]=aArr[tArr[i]-1];
  vector<int> ret(N);
  memcpy(&ret[0],&taArr[0],sizeof(taArr[0])*N);
  return ret;
}

vector<vector<int>> Order(int N,const vector<int> & m)
{
  vector<vector<int>> ret;
  vector<int> mi=m;
  vector<int> m0(N);
  for(int i=0;i<N;i++)
  {
    m0[i]=i+1;
  }
  while(memcmp(&mi[0],&m0[0],sizeof(int)*N)!=0)
  {
    ret.push_back(mi);
    mi=Mul(N,mi,m);
  }
  ret.push_back(mi);
  return ret;
}

int IsInFG(int N,const vector<vector<int>> FG,const vector<int> & m)
{
  for(int i=0;i<FG.size();i++)
  {
    if(memcmp(&m[0],&FG[i][0],sizeof(int)*N)==0)
      return i;
  }
  return -1;
}

//M9����2���������û�Ⱥ��ʾ
//int g_FG[72][9]=
//{
//{1,2,3,4,5,6,7,8,9},
//{4,5,6,9,3,2,7,1,8},//4��Ԫa
//{6,1,7,4,2,5,9,3,8},//4��Ԫb
//{9,3,2,8,6,5,7,4,1},//2��Ԫa^2
//{8,6,5,1,2,3,7,9,4},//4��Ԫa^3
//{5,6,9,4,1,2,8,7,3},//2��Ԫb^2
//{2,5,8,4,6,1,3,9,7},//4��Ԫb^3
//{4,2,5,8,7,1,9,6,3},//4��Ԫab
//{4,1,2,3,9,6,8,5,7},//4��Ԫabb
//{4,6,1,7,8,5,3,2,9},//4��Ԫabbb
//{8,7,1,3,5,2,9,4,6},//4��Ԫaab
//{3,9,6,7,2,1,8,4,5},//3��Ԫaabb
//{7,8,5,9,1,6,3,4,2},//4��Ԫaabbb
//{3,5,2,6,1,7,9,8,4},//4��Ԫaaab
//{7,2,1,5,6,9,8,3,4},//4��Ԫaaabb
//{9,1,6,2,5,8,3,7,4},//4��Ԫaaabbb
//{2,4,7,9,5,3,8,6,1},//4��Ԫba=aaabbbaaabbbaaabbb
//{5,9,7,8,3,6,1,2,4},//4��Ԫbaa=aabbbaabbbaabbb
//{3,8,7,1,6,2,4,5,9},//4��Ԫbaaa=abbbabbbabbb
//{3,2,8,9,4,5,1,7,6},//4��Ԫbba=aaabbaaabbaaabb
//{6,5,1,8,9,3,4,7,2},//3��Ԫbbaa=aabbaabb
//{2,3,4,1,8,6,9,7,5},//4��Ԫbbaaa=abbabbabb
//{5,3,1,9,2,4,6,8,7},//4��Ԫbbba=aaabaaabaaab
//{3,6,4,8,5,9,2,1,7},//4��Ԫbbbaa=aabaabaab
//{6,2,9,1,3,8,5,4,7},//4��Ԫbbbaaa=ababab
//{8,2,7,6,9,4,3,1,5},//2��Ԫabab=aaabbaaabb
//{3,4,1,2,7,6,5,9,8},//2��Ԫabbabb=aabbbaabbb
//{7,5,4,3,2,8,1,6,9},//2��Ԫabbbabbb
//{4,9,8,1,5,7,6,3,2},//2��Ԫaabaab=aaabbbaaabbb
//{2,1,5,7,3,9,4,8,6},//2��Ԫaaabaaab
//{9,5,3,1,7,4,8,2,6},//30=1*16
//{8,3,6,4,7,9,1,5,2},//31=1*17
//{1,6,2,9,7,8,4,3,5},//32=1*18
//{9,4,5,6,8,2,1,3,7},//33=1*19
//{8,9,3,2,1,5,4,6,7},//34=1*20
//{1,8,6,5,4,3,9,2,7},//35=1*21
//{9,2,4,7,1,3,6,5,8},//36=1*22
//{8,5,9,7,4,6,2,3,1},//37=1*23
//{1,3,8,7,9,2,5,6,4},//38=1*24
//{6,9,4,5,7,2,3,8,1},//39=1*25
//{2,7,6,8,1,4,5,3,9},//40=1*26
//{1,5,7,2,8,9,6,4,3},//41=1*28
//{7,3,9,6,5,1,4,2,8},//42=1*29
//{1,4,9,8,2,7,3,5,6},//43=2*7
//{6,4,8,3,1,9,7,2,5},//44=2*8
//{5,4,3,7,6,8,9,1,2},//45=2*9
//{2,8,9,3,7,5,6,1,4},//46=2*10
//{6,7,3,9,8,1,2,5,4},//47=2*12
//{9,7,8,5,2,6,4,1,3},//48=2*14
//{4,8,3,6,2,9,5,7,1},//49=2*25
//{6,3,5,2,4,7,8,1,9},//50=2*26
//{7,4,6,1,9,5,2,8,3},//51=2*28
//{1,7,4,6,3,5,8,9,2},//52=3*16
//{4,7,9,2,6,3,1,8,5},//53=3*17
//{6,8,2,7,5,4,1,9,3},//54=3*19
//{7,1,3,8,4,2,6,9,5},//55=3*22
//{7,9,2,4,8,3,5,1,6},//56=3*24
//{5,7,2,1,4,9,3,6,8},//57=3*25
//{8,1,4,9,6,7,5,2,3},//58=3*26
//{2,6,3,5,9,7,1,4,8},//59=4*17
//{5,2,6,3,8,7,4,9,1},//60=4*18
//{7,6,8,2,3,4,9,5,1},//61=4*21
//{8,4,2,5,3,1,6,7,9},//62=4*22
//{1,9,5,3,6,4,2,7,8},//63=4*23
//{9,6,7,3,4,1,5,8,2},//64=4*26
//{3,7,5,4,9,8,6,2,1},//65=4*28
//{5,8,4,2,9,1,7,3,6},//66=5*15
//{5,1,8,6,7,3,2,4,9},//67=6*13
//{2,9,1,6,4,8,7,5,3},//68=6*25
//{3,1,9,5,8,4,7,6,2},//69=7*8
//{9,8,1,4,3,7,2,6,5},//70=7*12
//{4,3,7,5,1,8,2,9,6},//71=10*12
//};

/*
int main()
{
vector<vector<int>> FG;
int N=9;
int R=2;
int S[2][9]=
{
//{1,2,3,4,5,6,7,8,9},
{4,5,6,9,3,2,7,1,8},//4��Ԫa
{6,1,7,4,2,5,9,3,8},//4��Ԫb
};
vector<int> E;
for(int i=0;i<N;i++)
{
E.push_back(i+1);
}
FG.push_back(E);

  for(int i=0;i<R;i++)
  {
  vector<int> I(N);
  memcpy(&I[0],&S[i][0],sizeof(int)*N);
  FG.push_back(I);
  }
  
    int cnt=R+1;
    int cnt1=R+1;
    do{
    cnt=FG.size();
    for(int i=1;i<cnt;i++)
    {
    for(int j=1;j<cnt;j++)
    {
    vector<int> IJ=Mul(N,FG[i],FG[j]);
    int bIn=IsInFG(N,FG,IJ);
    if(bIn==-1)
    {
    //cout<<FG.size()<<"="<<i<<"*"<<j<<endl;
    FG.push_back(IJ);
    }
    }
    }
    cnt1=FG.size();
    }while(cnt1>cnt);
    
      int n=FG.size();
      for(int i=0;i<n;i++)
      {
      for(int j=0;j<n;j++)
      {
      vector<int> IJ=Mul(N,FG[i],FG[j]);
      int bIn=IsInFG(N,FG,IJ);
      cout<<bIn+1<<" ";
      }
      cout<<endl;
      }
      
        system("pause");
        return 0;
        }
*/

int main(int argc, char* argv[])
{
  char sz[100]={0};
  char sz1[100]={0};
  if(argc<2)
  {
    printf("���������������û�ȺA������Ԫ�ļ�����");
    scanf("%s",sz);
  }
  else
    strcpy(sz,argv[1]);
  strcpy(sz1,sz);
  strcat(sz1,".txt");
  
  ifstream fin(sz);
  if( !fin ) {
    cerr << "Error opening input stream" << endl;
    system("pause");
    return -1;
  }
  else
  {
    string strLine;
    fin >> strLine;
    
    vector<string> vN=split(strLine,",");
    int N=vN.size();
    
    vector<vector<int>> FG;
    vector<int> E;
    for(int i=0;i<N;i++)
    {
      E.push_back(i+1);
    }
    FG.push_back(E);
    
    while(strLine!="")
    {
      strLine="";
      fin >> strLine;
      
      vector<string> vN1=split(strLine,",");
      int N1=vN1.size();
      if(N1==N)
      {
        vector<int> viN1(N);
        for(int i=0;i<N;i++)
        {
          viN1[i]=atoi(vN1[i].c_str());
        }
        if(memcmp(&E[0],&viN1[0],sizeof(int)*N)!=0)
        {
          FG.push_back(viN1);
        }
      }
    }
    
    int R=FG.size()-1;
    int cnt=R+1;
    int cnt1=R+1;
    do{
      cnt=FG.size();
      for(int i=1;i<cnt;i++)
      {
        for(int j=1;j<cnt;j++)
        {
          vector<int> IJ=Mul(N,FG[i],FG[j]);
          int bIn=IsInFG(N,FG,IJ);
          if(bIn==-1)
          {
            //cout<<FG.size()<<"="<<i<<"*"<<j<<endl;
            FG.push_back(IJ);
          }
        }
      }
      cnt1=FG.size();
    }while(cnt1>cnt);
    
    ofstream fout(sz1);
    int n=FG.size();
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
      {
        vector<int> IJ=Mul(N,FG[i],FG[j]);
        int bIn=IsInFG(N,FG,IJ);
        fout<<bIn+1<<" ";
      }
      fout<<endl;
    }
    cout<<FG.size()<<"��Ⱥ�Ŀ�����������ϣ�"<<endl;
  }
  
  system("pause");
  return 0;
}

// Ⱥ��С����6��������Ⱥ�Ŀ�������������ġ���λ��Ⱥ��С����ZDOfG.exe������CenterG.exeֻ�������ģ�
// 20140620����6�Ľ�:
// ����Ⱥ�Ŀ�������������ġ���λ��Ⱥ��С����ZDTableOfG.exe����Ⱥ��ʽ�Ŀ�������ZDTableOfG_N.exe���淶��ʽ�Ŀ�����
// ����Ⱥ�Ŀ������������Ӱ���ġ���Ӱ��λ��Ⱥ��С����PZDTableOfG.exe
// ������ȺA�������ļ�����A4.txt
// 0
// |Z(A4)|=1
// 0
// 3
// 8
// 11
// |(A4)'|=4=>A_4�ǿɽ�ǵ�Ⱥ
// 1 4 9 12
// 4 1 12 9
// 9 12 1 4
// 12 9 4 1
// ���4��Ⱥ��C_2��C_2
// ������ȺA�������ļ�����Q12.txt
// 0
// 3
// |Z(Q12)|=2
// 0
// 1
// 2
// |(Q12)'|=3=>Q_12�ǿɽ�ǵ�Ⱥ
// ������ȺA�������ļ�����D3C2.txt
// 0
// 1
// |Z(D3C2)|=2
// 0
// 6
// 8
// |(D3C2)'|=3=>D3C2�ǿɽ�ǵ�Ⱥ
// ������ȺA�������ļ�����A5.txt
// 0
// |Z(A5)|=1
// 
// |(A5)'|=60=>A_5�ǲ��ɽ�Ⱥ��2014��6��5��23:55:04�ü����֤����
// ������ȺA�������ļ�����Q60.txt
// 0
// 16
// |Z(Q60)|=2
// 0
// 3
// 5
// 7
// 9
// 11
// 13
// 15
// 17
// 19
// 21
// 23
// 25
// 27
// 29
// |(Q60)'|=15=>Q_60�ǿɽ�ǵ�Ⱥ
// ������ȺA�������ļ�����D15C2.txt
// 0
// 1
// |Z(D15C2)|=2
// 0
// 6
// 10
// 14
// 16
// 20
// 26
// 30
// 34
// 36
// 40
// 46
// 50
// 54
// 56
// |(D15C2)'|=15=>D15C2�ǿɽ�ǵ�Ⱥ
// ������ȺA�������ļ�����D15.txt
// 0
// |Z(D15)|=1
// 0
// 3
// 5
// 7
// 8
// 10
// 13
// 15
// 17
// 18
// 20
// 23
// 25
// 27
// 28
// |(D15)'|=15=>D15�ǿɽ�ǵ�Ⱥ
// ȺG��������G��������Ⱥ��
// Z(D_4)=C_2,Z(Q_8)=C_2������D_4,Q_8��2��8�׷�AbelȺ�����ǵ�Ⱥ��
// Z(D_3)=C_1
// (D_3)'=C_3���������6�׷�AbelȺ�ǿɽ�Ⱥ��
// D_4'=C_2��Q_8'=C_2������D_4,Q_8��2��8�׷�AbelȺ���ǿɽ�Ⱥ��
// ������ȺA�������ļ�����A5.txt
// 0
// |Z(A5)|=1
// ������ȺA�������ļ�����Q60.txt
// 0
// 16
// |Z(Q60)|=2
// ������ȺA�������ļ�����Q12.txt
// 0
// 3
// |Z(Q12)|=2
// ������ȺA�������ļ�����A4.txt
// 0
// |Z(A4)|=1
// ������ȺA�������ļ�����D3C2.txt
// 0
// 1
// |Z(D3C2)|=2
// ������ȺA�������ļ�����G24_4.txt
// 0
// 3
// 6
// 9
// |Z(G24_4)|=4
// 1 4 7 10
// 4 1 10 7
// 7 10 4 1
// 10 7 1 4
// ����C_4�Ŀ�����������C_2��C_2�Ŀ�����
// 1 2 3 4
// 2 1 4 3
// 3 4 2 1
// 4 3 1 2
// ������ȺA�������ļ�����C4.txt
// G4ElementToOrder(0)=1
// G4ElementToOrder(1)=2
// G4ElementToOrder(2)=4
// G4ElementToOrder(3)=4
// C4��1��1��Ԫ��1��2��Ԫ��2��4��Ԫ
// ������ȺA�������ļ�����G24_5.txt
// 0
// 3
// |Z(G24_5)|=2
// ������ȺA�������ļ�����SL(2,3).txt
// 0
// 3
// |Z(SL(2,3))|=2
// 0
// 3
// 6
// 9
// 14
// 17
// 20
// 23
// |(SL(2,3))'|=8
// ���8��Ⱥ��
// 1 4 7 10 15 18 21 24
// 4 1 10 7 18 15 24 21
// 7 10 4 1 24 21 15 18
// 10 7 1 4 21 24 18 15
// 15 18 21 24 4 1 10 7
// 18 15 24 21 1 4 7 10
// 21 24 18 15 7 10 4 1
// 24 21 15 18 10 7 1 4
// ���8��Ⱥ��Q_8
// ������ȺA�������ļ�����G8.txt
// G8ElementToOrder(0)=1
// G8ElementToOrder(1)=2
// G8ElementToOrder(2)=4
// G8ElementToOrder(3)=4
// G8ElementToOrder(4)=4
// G8ElementToOrder(5)=4
// G8ElementToOrder(6)=4
// G8ElementToOrder(7)=4
// G8��1��1��Ԫ��1��2��Ԫ��6��4��Ԫ��0��8��Ԫ
// ������ȺA�������ļ�����G24_6.txt
// 0
// 3
// |Z(G24_6)|=2
// ������ȺA�������ļ�����G24_7.txt
// 0
// 6
// 12
// 18
// |Z(G24_7)|=4
// 1 7 13 19
// 7 1 19 13
// 13 19 7 1
// 19 13 1 7
// <=>
// 1 2 3 4
// 2 1 4 3
// 3 4 2 1
// 4 3 1 2
// ���4��Ⱥ��C_4
// ������ȺA�������ļ�����C4.txt
// G4ElementToOrder(0)=1
// G4ElementToOrder(1)=2
// G4ElementToOrder(2)=4
// G4ElementToOrder(3)=4
// C4��1��1��Ԫ��1��2��Ԫ��2��4��Ԫ
// ������ȺA�������ļ�����G24_8.txt
// 0
// 6
// |Z(G24_8)|=2
// ������ȺA�������ļ�����D12.txt
// 0
// 6
// |Z(D12)|=2
// 0
// 3
// 4
// 6
// 9
// 10
// |(D12)'|=6
// 1 4 5 7 10 11
// 4 5 1 10 11 7
// 5 1 4 11 7 10
// 7 10 11 1 4 5
// 10 11 7 4 5 1
// 11 7 10 5 1 4
// ���6��Ⱥ��C_6
// ������ȺA�������ļ�����G6.txt
// G6ElementToOrder(0)=1
// G6ElementToOrder(1)=3
// G6ElementToOrder(2)=3
// G6ElementToOrder(3)=2
// G6ElementToOrder(4)=6
// G6ElementToOrder(5)=6
// G6��1��1��Ԫ��1��2��Ԫ��2��3��Ԫ��2��6��Ԫ
// ������ȺA�������ļ�����G24_9.txt
// 0
// 3
// 6
// 9
// |Z(G24_9)|=4
// 1 4 7 10
// 4 1 10 7
// 7 10 1 4
// 10 7 4 1
// <=>
// 1 2 3 4
// 2 1 4 3
// 3 4 1 2
// 4 3 2 1
// ���4��Ⱥ��C_2��C_2
// ������ȺA�������ļ�����C2C2.txt
// G4ElementToOrder(0)=1
// G4ElementToOrder(1)=2
// G4ElementToOrder(2)=2
// G4ElementToOrder(3)=2
// C2C2��1��1��Ԫ��3��2��Ԫ��0��4��Ԫ
// ������ȺA�������ļ�����G24_10.txt
// 0
// 6
// |Z(G24_10)|=2
// ������ȺA�������ļ�����G24_11.txt
// 0
// 3
// 4
// 7
// 8
// 11
// |Z(G24_11)|=6
// ���6��Ⱥ��C_6
// 1 4 5 8 9 12
// 4 1 8 5 12 9
// 5 8 9 12 1 4
// 8 5 12 9 4 1
// 9 12 1 4 5 8
// 12 9 4 1 8 5
// ������ȺA�������ļ�����G24_12.txt
// 0
// 1
// 2
// 3
// 4
// 5
// |Z(G24_12)|=6
// ���6��Ⱥ��C_6
// 1 2 3 4 5 6
// 2 1 4 3 6 5
// 3 4 5 6 1 2
// 4 3 6 5 2 1
// 5 6 1 2 3 4
// 6 5 2 1 4 3
// ������ȺA�������ļ�����G24_13.txt
// 0
// |Z(G24_13)|=1
// ������ȺA�������ļ�����G24_14.txt
// 0
// 12
// |Z(G24_14)|=2
// ������ȺA�������ļ�����G24_15.txt
// 0
// 6
// 12
// 18
// |Z(G24_15)|=4
// ���4��Ⱥ��C_2��C_2
// 1 7 13 19
// 7 1 19 13
// 13 19 1 7
// 19 13 7 1
// ����AbelȺG,��λ��[a,b]=aba^-1b^-1=aa^-1bb^-1=1�����Ի�λ��ȺG'=1
// �ʣ����AbelȺG�Ļ�λ��ȺG'���ж�G�Ƿ��ǿɽ�Ⱥ��
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int g_D3Mul[6][6]=
{
  //D_3
  {1,2,3,4,5,6},
  {2,1,4,3,6,5},
  {3,5,1,6,2,4},
  {4,6,2,5,1,3},
  {5,3,6,1,4,2},
  {6,4,5,2,3,1}
};
int g_D4Mul[8][8]=
{
  //D_4
  //{1,2,3,4,5,6,7,8},
  //{2,1,4,3,6,5,8,7},
  //{3,4,1,2,7,8,5,6},
  //{4,3,2,1,8,7,6,5},
  //{5,7,6,8,1,3,2,4},
  //{6,8,5,7,2,4,1,3},
  //{7,5,8,6,3,1,4,2},
  //{8,6,7,5,4,2,3,1}
  //Q_8
  {1,2,3,4,5,6,7,8},
  {2,1,4,3,6,5,8,7},
  {3,4,2,1,8,7,5,6},
  {4,3,1,2,7,8,6,5},
  {5,6,7,8,2,1,4,3},
  {6,5,8,7,1,2,3,4},
  {7,8,6,5,3,4,2,1},
  {8,7,5,6,4,3,1,2}
};
bool IsInCenterOfG(const vector<vector<int>> &vvG,int j)
{
  int N=vvG.size();
  for(int i=0;i<N;i++)
  {
    int ij=vvG[i][j]-1;
    int ji=vvG[j][i]-1;
    if(ij==ji)
      continue;
    else
      return false;
  }
  return true;
}
vector<int> CenterOfG(const vector<vector<int>> &vvG)
{
  vector<int> ret;
  int N=vvG.size();
  for(int i=0;i<N;i++)
  {
    if(IsInCenterOfG(vvG,i))
      ret.push_back(i);
    else
      continue;
  }
  return ret;
}

int Inv(const vector<vector<int>> &vvG,int j)
{
  int N=vvG.size();
  for(int i=0;i<N;i++)
  {
    int ij=vvG[i][j]-1;
    if(ij==0)
      return i;
  }
  return -1;
}
vector<int> commutatorOfG(const vector<vector<int>> &vvG)
{
  vector<int> ret;
  ret.push_back(0);
  int N=vvG.size();
  for(int i=0;i<N;i++)
  {
    int I=Inv(vvG,i);
    for(int j=i+1;j<N;j++)
    {
      int ij=vvG[i][j]-1;
      int J=Inv(vvG,j);
      //int JI=vvG[J][I]-1;
      //int ijJI=vvG[ij][JI]-1;
      //ret.push_back(ijJI);
      int IJ=vvG[I][J]-1;
      int ijIJ=vvG[ij][IJ]-1;
      ret.push_back(ijIJ);
    }
  }
  sort(ret.begin(),ret.end());
  ret.erase(unique(ret.begin(),ret.end()),ret.end());
  return ret;
}
int main()
{
  vector<vector<int>> vvD3;
  {
    int n=6;
    for(int a=0;a<n;a++)
    {
      vector<int> iRow(&g_D3Mul[a][0],&g_D3Mul[a][0]+n);
      vvD3.push_back(iRow);
    }
  }
  vector<int> ZD3=CenterOfG(vvD3);//0��ʾZ(D_3)={1}=C_1
  vector<int> DD3=commutatorOfG(vvD3);//0 3 4��ʾ(D_3)'={1,4,5}=C_3
  vector<vector<int>> vvD4;
  {
    int n=8;
    for(int a=0;a<n;a++)
    {
      vector<int> iRow(&g_D4Mul[a][0],&g_D4Mul[a][0]+n);
      vvD4.push_back(iRow);
    }
  }
  vector<int> ZD4=CenterOfG(vvD4);//0 3��ʾZ(D_4)={1,4}=C_2
  vector<int> DD4=commutatorOfG(vvD4);//0 7��ʾ(D_4)'={1 8}=C_2��Ӧ����0 3��ʾ(D_4)'={1 4}=C_2
  //vector<int> ZQ8=CenterOfG(vvD4);//0 1��ʾZ(Q_8)={1,2}=C_2
  //vector<int> DQ8=commutatorOfG(vvD4);//0 1��ʾ(Q_8)'={1,2}=C_2
  system("pause");
  return 0;
}
// ����1������ȺG�Ŀ������G��������ȺN��ȺԪ��������ȺG/N��ȺԪ�Ϳ�����
// ����2������ȺG�Ŀ������G���Ӽ�H���ж�H�Ƿ���G����Ⱥ���Ƿ���G��������Ⱥ��
// 20140606����ȺG/N���Կ�����G��һ����Ⱥ���ĺ����ԣ�
// ��������һ��G����ȺH��ʹ��H=G/N����H={a_1,��,a_h}��G/N={~a_1,��,~a_h}
// H�ĳ˷���f_H(a_i,a_j)=a_k
// G/N�ĳ˷���f_G/N(~a_i,~a_j)=~a_k=~f_H(a_i,a_j)
//     vector<vector<int>> PDD3Table=quotientGNTable(vvD3,DD3);//C_3
//  //   vector<vector<int>> PDD4Table=quotientGNTable(vvD4,ZD4);//C_2��C_2=D_4/Z(D_4)=D_4/(D_4)'={~0=~3,~1=~2,~4=~7,~5=~6}
//  vector<vector<int>> PZQ8Table=quotientGNTable(vvD4,ZQ8);//C_2��C_2=Q_8/Z(Q_8)=Q_8/(Q_8)'={~0=~1,~2=~3,~4=~5,~6=~7}
// D_4����Ⱥ={1,4,5,8}={1,4,6,7}
// Q_8����Ⱥ={1,2,5,6}=C_4
// 1 2 5 6
// 2 1 6 5
// 5 6 2 1
// 6 5 1 2
// <=>
// 1 2 3 4
// 2 1 4 3
// 3 4 2 1
// 4 3 1 2
// ���4��Ⱥ��C_4
// ������ȺA�������ļ�����G4.txt
// G4ElementToOrder(0)=1
// G4ElementToOrder(1)=2
// G4ElementToOrder(2)=4
// G4ElementToOrder(3)=4
// G4��1��1��Ԫ��1��2��Ԫ��2��4��Ԫ
// Q_8/Z(Q_8)=Q_8/(Q_8)'={~0=~1,~2=~3,~4=~5,~6=~7}
// ={~1=~2,~3=~4,~5=~6,~7=~8}
// ~1 ~4 ~5 ~7
// ~4 ~1 ~7 ~5
// ~5 ~7 ~1 ~4
// ~7 ~5 ~4 ~1
// <=>
// 1 2 3 4
// 2 1 4 3
// 3 4 1 2
// 4 3 2 1
// ���4��Ⱥ��C_2��C_2
// ������ȺA�������ļ�����C2C2.txt
// G4ElementToOrder(0)=1
// G4ElementToOrder(1)=2
// G4ElementToOrder(2)=2
// G4ElementToOrder(3)=2
// C2C2��1��1��Ԫ��3��2��Ԫ��0��4��Ԫ
vector<vector<int>> quotientGN(const vector<vector<int>> &vvG,const vector<int> &vN)
{
  vector<vector<int>> ret;
  int G=vvG.size();
  int N=vN.size();
  for(int i=0;i<G;i++)
  {
    vector<int> I;
    for(int j=0;j<N;j++)
    {
      int ij=vvG[i][vN[j]]-1;
      I.push_back(ij);
    }
    bool bNew=true;
    for(int k=0;k<ret.size();k++)
    {
      //�ж�I�е�Ԫ���Ƿ���ret��
      vector<int>::iterator p;
      p=std::find(ret[k].begin(),ret[k].end(),I[0]);
      if(p!=ret[k].end())
      {
        bNew=false;
        break;
      }
    }
    if(bNew)
    {
      ret.push_back(I);
    }
  }
  return ret;
}
vector<vector<int>> quotientGNTable(const vector<vector<int>> &vvG,const vector<int> &vN)
{
  vector<vector<int>> ret1=quotientGN(vvG,vN);
  int G=vvG.size();
  int H=ret1.size();
  vector<vector<int>> ret(H);
  for(int i=0;i<H;i++)
  {
    vector<int> I(H);
    for(int j=0;j<H;j++)
    {
      int ij=vvG[ret1[i][0]][ret1[j][0]]-1;
      int IJ=-1;
      for(int k=0;k<ret1.size();k++)
      {
        vector<int>::iterator p;
        p=std::find(ret1[k].begin(),ret1[k].end(),ij);
        if(p!=ret1[k].end())
        {
          IJ=k+1;
          break;
        }
      }
      I[j]=IJ;
    }
    ret[i]=I;
  }
  return ret;
}
bool IsSubG(const vector<vector<int>> &vvG,const vector<int> &vH)
{
  int H=vH.size();
  //�ж�1�Ƿ���H��
  vector<int>::const_iterator p;
  p=std::find(vH.begin(),vH.end(),1);
  if(p==vH.end())
  {
    return false;
  }
  for(int i=0;i<H;i++)
  {
    int I=Inv(vvG,vH[i]-1);
    p=std::find(vH.begin(),vH.end(),I+1);
    if(p==vH.end())
    {
      return false;
    }
    for(int j=0;j<H;j++)
    {
      int ij=vvG[vH[i]-1][vH[j]-1];
      p=std::find(vH.begin(),vH.end(),ij);
      if(p==vH.end())
      {
        return false;
      }
    }
  }
  return true;
}
//vector<int> ZD4=CenterOfG(vvD4);//0 3��ʾZ(D_4)={1,4}=C_2
//vector<int> DD4=commutatorOfG(vvD4);//0 7��ʾ(D_4)'={1 8}=C_2��Ӧ����0 3��ʾ(D_4)'={1 4}=C_2
vector<int> ZQ8=CenterOfG(vvD4);//0 1��ʾZ(Q_8)={1,2}=C_2
vector<int> DQ8=commutatorOfG(vvD4);//0 1��ʾ(Q_8)'={1,2}=C_2
vector<vector<int>> PZD3=quotientGN(vvD3,ZD3);//0:0 1:1 2:2 3:3 4:4 5:5
vector<vector<int>> PDD3=quotientGN(vvD3,DD3);//0:0,3,4 1:1,2,5
//vector<vector<int>> PZD4=quotientGN(vvD4,ZD4);//0:0,3 1:1,2 2:4,7 3:5,6
//vector<vector<int>> PDD4=quotientGN(vvD4,DD4);//0:0,3 1:1,2 2:4,7 3:5,6
vector<vector<int>> PZQ8=quotientGN(vvD4,ZQ8);//0:0,1 1:2,3 2:4,5 3:6,7
////Q_8��4����Ⱥ
//vector<int> vH;
//vH.push_back(1);
//vH.push_back(2);
//vH.push_back(5);
//vH.push_back(6);
//bool bret=IsSubG(vvD4,vH);
//{1,4,5,8}={1,4,6,7},����8���Ӽ��е�2��������D_4��4����Ⱥ
vector<vector<int>> vvG=vvD4;
{
  int SG1[4]={1,3,5,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,3,5,8};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,3,6,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,3,6,8};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,4,5,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,4,5,8};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,4,6,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,4,6,8};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
//����8���Ӽ��ж�������D_4��4����Ⱥ
vector<vector<int>> vvG=vvD4;
{
  int SG1[4]={1,2,5,6};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,2,5,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,2,8,6};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,2,8,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,3,5,6};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,3,5,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}
{
  int SG1[4]={1,3,8,6};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}  
{
  int SG1[4]={1,3,8,7};
  vector<int> vH(SG1,SG1+4);
  bool bret=IsSubG(vvG,vH);
  int a=0;
}

//�û�Ⱥ������֤������ѧ��е�� http://www.docin.com/p-388825048.html
/*
�ó���֤��������ȺD_n���Ĵν���ȺA_4�����ζԳ�ȺS_3�ǳ��ɽ�Ⱥ��
��һ���ж���Щ����Ⱥ�Ƿ�������Ⱥ��Nilpotent group����
�ó���֤����Щ����Ⱥ�Ƕ���ѭ��Ⱥ���ɽ�Ⱥ��
�ó���֤��A_3��S_3��A_4��S_4���ǿɽ�Ⱥ��
�ó���֤��A_5����СAbel��Ⱥ��Ҳ����С���ɽ�Ⱥ��
���û�ȺA���û�ȺS��������Ⱥ������ȺS/A��
������ȺԪ���࣬ʵ�ֳ˷����㡢�������㡣
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
using namespace std;
/*
http://wenku.baidu.com/view/ddcb56ab960590c69ec376f8.html
*/
struct S4
{
public:
  S4(int a1=1,int a2=2,int a3=3,int a4=4)
  {
    m_a1=a1;
    m_a2=a2;
    m_a3=a3;
    m_a4=a4;
  }
  bool operator==(const S4 &a)
  {
    return (m_a1==a.m_a1 && m_a2==a.m_a2 && m_a3==a.m_a3 && m_a4==a.m_a4);
  }
  S4 operator*(const S4 &a)
  {
    int aArr[4]={a.m_a1,a.m_a2,a.m_a3,a.m_a4};
    S4 ret;
    ret.m_a1=aArr[this->m_a1-1];
    ret.m_a2=aArr[this->m_a2-1];
    ret.m_a3=aArr[this->m_a3-1];
    ret.m_a4=aArr[this->m_a4-1];
    return ret;
  }
  S4 InvMul()const
  {
    static S4 gS4[]={S4(1,2,3,4),S4(2,1,3,4),S4(1,2,4,3),S4(3,2,1,4),S4(1,4,3,2),S4(4,2,3,1),S4(1,3,2,4),S4(2,1,4,3),S4(3,4,1,2),S4(4,3,2,1),S4(2,3,1,4),S4(3,1,2,4),S4(3,2,4,1),S4(4,2,1,3),S4(2,4,3,1),S4(4,1,3,2),S4(1,3,4,2),S4(1,4,2,3),S4(2,3,4,1),S4(2,4,1,3),S4(3,4,2,1),S4(3,1,4,2),S4(4,3,1,2),S4(4,1,2,3)};
    for(int i=0;i<24;i++)
    {
      if(gS4[i]*(*this)==gS4[0])
        return gS4[i];
    }
    return S4(0,0,0,0);
  }
  const char *getName()
  {
    static S4 gS4[]={S4(1,2,3,4),S4(2,1,3,4),S4(1,2,4,3),S4(3,2,1,4),S4(1,4,3,2),S4(4,2,3,1),S4(1,3,2,4),S4(2,1,4,3),S4(3,4,1,2),S4(4,3,2,1),S4(2,3,1,4),S4(3,1,2,4),S4(3,2,4,1),S4(4,2,1,3),S4(2,4,3,1),S4(4,1,3,2),S4(1,3,4,2),S4(1,4,2,3),S4(2,3,4,1),S4(2,4,1,3),S4(3,4,2,1),S4(3,1,4,2),S4(4,3,1,2),S4(4,1,2,3)};
    static const char *szName[]={"(1)","(12)","(34)","(13)","(24)","(14)","(23)","(12)(34)","(13)(24)","(14)(23)","(123)","(132)","(134)","(143)","(124)","(142)","(234)","(243)","(1234)","(1243)","(1324)","(1342)","(1423)","(1432)"};
    for(int i=0;i<24;i++)
    {
      if((*this)==gS4[i])
        return szName[i];
    }
    return "Error";
  }
  //�ܷ�ӳԪ�ؽ׵��û����ʽ(���������߹��̡�)��������˫���û����ʽ(���������߽����)
  const char * getOrderForm()
  {
    static S4 gS4[]={S4(1,2,3,4),S4(2,1,3,4),S4(1,2,4,3),S4(3,2,1,4),S4(1,4,3,2),S4(4,2,3,1),S4(1,3,2,4),S4(2,1,4,3),S4(3,4,1,2),S4(4,3,2,1),S4(2,3,1,4),S4(3,1,2,4),S4(3,2,4,1),S4(4,2,1,3),S4(2,4,3,1),S4(4,1,3,2),S4(1,3,4,2),S4(1,4,2,3),S4(2,3,4,1),S4(2,4,1,3),S4(3,4,2,1),S4(3,1,4,2),S4(4,3,1,2),S4(4,1,2,3)};
    static const char *szName[]={"(1)","(12)","(34)","(13)","(24)","(14)","(23)","(12)(34)","(13)(24)","(14)(23)","(123)","(132)","(134)","(143)","(124)","(142)","(234)","(243)","(1234)","(1243)","(1324)","(1342)","(1423)","(1432)"};
    for(int i=0;i<24;i++)
    {
      if((*this)==gS4[i])
        return szName[i];
    }
    return "Error";
  }
  //ȺԪ�Ľ�
  static int getOrder(const S4 &idx,const S4 &m)
  {
    S4 t=idx;
    for(int i=1;i<=24;i++)
    {
      t=t*m;
      if(t==idx)
        return i;
    }
    return -1;
  }
  static S4 createS4(const char * szOrderForm)
  {
    static S4 gS4[]={S4(1,2,3,4),S4(2,1,3,4),S4(1,2,4,3),S4(3,2,1,4),S4(1,4,3,2),S4(4,2,3,1),S4(1,3,2,4),S4(2,1,4,3),S4(3,4,1,2),S4(4,3,2,1),S4(2,3,1,4),S4(3,1,2,4),S4(3,2,4,1),S4(4,2,1,3),S4(2,4,3,1),S4(4,1,3,2),S4(1,3,4,2),S4(1,4,2,3),S4(2,3,4,1),S4(2,4,1,3),S4(3,4,2,1),S4(3,1,4,2),S4(4,3,1,2),S4(4,1,2,3)};
    static const char *szName[]={"(1)","(12)","(34)","(13)","(24)","(14)","(23)","(12)(34)","(13)(24)","(14)(23)","(123)","(132)","(134)","(143)","(124)","(142)","(234)","(243)","(1234)","(1243)","(1324)","(1342)","(1423)","(1432)"};
    for(int i=0;i<24;i++)
    {
      if(strcmp(szOrderForm,szName[i])==0)
        return gS4[i];
    }
    return S4(0,0,0,0);
  }
  friend ostream& operator<<(ostream& os,S4& a);
public:
  int m_a1,m_a2,m_a3,m_a4;//˫���û����ʽ
};
ostream& operator<<(ostream& os,S4& a)
{
  cout<<a.getName();
  return os;
}
bool IsGroup(vector<S4> &vec_gS4,S4 id)
{
  if(find(vec_gS4.begin(),vec_gS4.end(),id)==vec_gS4.end())
  {
    return false;
  }
  //�˷������
  for(int i=0;i<vec_gS4.size();i++)
  {
    for(int j=i;j<vec_gS4.size();j++)
    {
      S4 ij=vec_gS4[i]*vec_gS4[j];
      S4 ji=vec_gS4[j]*vec_gS4[i];
      if(find(vec_gS4.begin(),vec_gS4.end(),ij)==vec_gS4.end())
      {
        return false;
      }
      if(find(vec_gS4.begin(),vec_gS4.end(),ji)==vec_gS4.end())
      {
        return false;
      }
    }
  }
  //�г˷���Ԫ
  for(int i=0;i<vec_gS4.size();i++)
  {
    S4 inv=vec_gS4[i].InvMul();
    if(find(vec_gS4.begin(),vec_gS4.end(),inv)==vec_gS4.end())
    {
      return false;
    }
  }
  //id�ǳ˷���λԪ
  for(int i=0;i<vec_gS4.size();i++)
  {
    if(id*vec_gS4[i]==vec_gS4[i] && vec_gS4[i]*id==vec_gS4[i])
      continue;
    else
      return false;
  }
  return true;
}
//0��ѭ��Ⱥ���������Ϊѭ��Ⱥ������Ԫ
//1��Ⱥ������ѭ��Ⱥ
//2����Ⱥ
int IsCircleGroup(vector<S4> &vec_gA,vector<S4> &vec_Out)
{
  if(!IsGroup(vec_gA,S4(1,2,3,4)))
    return 2;
  vec_Out.clear();
  for(int i=0;i<vec_gA.size();i++)
  {
    if(S4::getOrder(S4(1,2,3,4),vec_gA[i])==vec_gA.size())
    {
      vec_Out.push_back(vec_gA[i]);
    }
  }
  if(vec_Out.size()>0)
    return 0;
  else
    return 1;
  return 0;
}
//0��AbelȺ
//1��Ⱥ������AbelȺ
//2����Ⱥ
int IsAbelianGroup(vector<S4> &vec_gA)
{
  if(!IsGroup(vec_gA,S4(1,2,3,4)))
    return 2;
  //�˷�������
  for(int i=0;i<vec_gA.size();i++)
  {
    for(int j=i+1;j<vec_gA.size();j++)
    {
      S4 ij=vec_gA[i]*vec_gA[j];
      S4 ji=vec_gA[j]*vec_gA[i];
      if(ij==ji)
        continue;
      else
        return 1;
    }
  }
  return 0;
}
/*
ȺG������Z(G)��������G�к�G������Ԫ�ؿɽ�����Ԫ�صļ��ϣ�Z(G)={z��G|gz=zg��������g��G}��G��һ���ɽ�����������Ⱥ��
���壺��G��һ��Ⱥ������C(G)={a��G|ag=ga,��g��G}��ΪG�����ġ�
��GΪȺ����G������C_1(G)=C(G)ΪG��������Ⱥ��
��C_2(G)��������̬ͬG->G/C_1(G)֮��C(G/C_1(G))��ԭ������C_2(G){<|}G��
һ��أ���C_n(G)������̬ͬG->G/C_(n-1)(G)֮��C(G/C_(n-1)(G))��ԭ������������������Ⱥ��{1}<=C_1(G)<=C_2(G)<=C_n(G){<|}G�����G���������С�
���壺ȺG��������Ⱥ��nilpotent group������ָ����n>=1��ʹ��C_n(G)=G��
*/
bool IsInCenterOfG(vector<S4> &vec_G,S4 j)
{
  for(int i=0;i<vec_G.size();i++)
  {
    S4 ij=vec_G[i]*j;
    S4 ji=j*vec_G[i];
    if(ij==ji)
      continue;
    else
      return false;
  }
  return true;
}
vector<S4> CenterOfG(vector<S4> &vec_G)
{
  vector<S4> ret;
  if(!IsGroup(vec_G,S4(1,2,3,4)))
    return ret;
  for(int i=0;i<vec_G.size();i++)
  {
    if(IsInCenterOfG(vec_G,vec_G[i]))
      ret.push_back(vec_G[i]);
    else
      continue;
  }
  return ret;
}
//0��������Ⱥ
//1����Ⱥ������������Ⱥ
//2������Ⱥ
int IsNormalSubgroup(vector<S4> &vec_gA,vector<S4> &vec_gS4)
{
  if(!IsGroup(vec_gS4,S4(1,2,3,4)))
    return 2;
  if(!IsGroup(vec_gA,S4(1,2,3,4)))
    return 2;
  for(int i=0;i<vec_gA.size();i++)
  {
    if(find(vec_gS4.begin(),vec_gS4.end(),vec_gA[i])==vec_gS4.end())
    {
      return 2;
    }
  }
  //��һ���ж��Ƿ���������Ⱥ
  for(int i=0;i<vec_gS4.size();i++)
  {
    for(int j=0;j<vec_gA.size();j++)
    {
      S4 ghg1=vec_gS4[i]*vec_gA[j]*vec_gS4[i].InvMul();
      if(find(vec_gA.begin(),vec_gA.end(),ghg1)==vec_gA.end())
      {
        return 1;
      }
    }
  }
  return 0;
}

static S4 gS4[]={S4(1,2,3,4),S4(2,1,3,4),S4(1,2,4,3),S4(3,2,1,4),S4(1,4,3,2),S4(4,2,3,1),S4(1,3,2,4),S4(2,1,4,3),S4(3,4,1,2),S4(4,3,2,1),S4(2,3,1,4),S4(3,1,2,4),S4(3,2,4,1),S4(4,2,1,3),S4(2,4,3,1),S4(4,1,3,2),S4(1,3,4,2),S4(1,4,2,3),S4(2,3,4,1),S4(2,4,1,3),S4(3,4,2,1),S4(3,1,4,2),S4(4,3,1,2),S4(4,1,2,3)};
vector<S4> vec_gS4(gS4,gS4+24);
//1��1��Ԫ��3��2��Ԫ��8��3��Ԫ
static S4 gA4[]={S4::createS4("(1)"),S4::createS4("(123)"),S4::createS4("(132)"),S4::createS4("(134)"),S4::createS4("(143)"),S4::createS4("(124)"),S4::createS4("(142)"),S4::createS4("(234)"),S4::createS4("(243)"),S4::createS4("(12)(34)"),S4::createS4("(13)(24)"),S4::createS4("(14)(23)")};
vector<S4> vec_gA4(gA4,gA4+12);
//C_2
static S4 gC2[]={S4::createS4("(1)"),S4::createS4("(12)(34)")};
vector<S4> vec_gC2(gC2,gC2+2);
//��������ԪȺV_4
static S4 gV4[]={S4::createS4("(1)"),S4::createS4("(12)(34)"),S4::createS4("(13)(24)"),S4::createS4("(14)(23)")};
vector<S4> vec_gV4(gV4,gV4+4);
//C_4
static S4 gC4[]={S4::createS4("(1)"),S4::createS4("(1234)"),S4::createS4("(13)(24)"),S4::createS4("(1432)")};
vector<S4> vec_gC4(gC4,gC4+4);
//S_3
static S4 gS3[]={S4::createS4("(1)"),S4::createS4("(12)"),S4::createS4("(13)"),S4::createS4("(23)"),S4::createS4("(123)"),S4::createS4("(132)")};
vector<S4> vec_gS3(gS3,gS3+6);
//D_4={E=(1),E1=(12),E2=(34),E3=(12)(34),E4=(13)(24),E5=(1423),E6=(1324),E7=(14)(23)}
//D_4
static S4 gD4[]={S4::createS4("(1)"),S4::createS4("(1234)"),S4::createS4("(13)(24)"),S4::createS4("(1432)"),S4::createS4("(13)"),S4::createS4("(12)(34)"),S4::createS4("(24)"),S4::createS4("(14)(23)")};
vector<S4> vec_gD4(gD4,gD4+8);
int main()
{
  //����ż�û�
  bool ret=IsGroup(vec_gA4,S4(1,2,3,4));
  cout<<"ret="<<ret<<endl;
  bool ret1=IsGroup(vec_gS4,S4(1,2,3,4));
  cout<<"ret1="<<ret1<<endl;
  bool ret2=IsGroup(vec_gV4,S4(1,2,3,4));
  cout<<"ret2="<<ret2<<endl;
  bool ret3=IsGroup(vec_gC4,S4(1,2,3,4));
  cout<<"ret3="<<ret3<<endl;
  bool ret4=IsGroup(vec_gS3,S4(1,2,3,4));
  cout<<"ret4="<<ret4<<endl;
  bool ret5=IsGroup(vec_gD4,S4(1,2,3,4));
  cout<<"ret5="<<ret5<<endl;
  //C_2��V_4��������Ⱥ��V_4��S_4��������Ⱥ����C_2����S_4��������Ⱥ
  int ret6=IsNormalSubgroup(vec_gC2,vec_gV4);
  cout<<"ret6="<<ret6<<endl;
  int ret7=IsNormalSubgroup(vec_gV4,vec_gS4);
  cout<<"ret7="<<ret7<<endl;
  int ret8=IsNormalSubgroup(vec_gC2,vec_gS4);
  cout<<"ret8="<<ret8<<endl;
  system("pause");
  return 0;
}

#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
using namespace std;
/*
��
����ȺGǶ��S_n����Сn=��Ϊm(G)={n��N|G<S_n}
��m(C_2)=2,m(C_3)=3
,m(C_4)=4,m(C_2��C_2)=4
�ʣ��Թ�������ͽ�ȺG���û���ʾ�������m(G)��
��
�ʣ���������Ⱥ���û���ʾ��A_5��Ⱥ�����
��n������ȺG������p�׿�����Ⱥ��
�����ζԳ�ȺS_3��������Ⱥ
��6����
H1={(1)}
H2={(1),(12)}
H3={(1),(13)}
H4={(1),(23)}
H5={(1),(123),(132)}
H6=S_3
֤����A_3��S_3��������Ⱥ��
֤����S_3�ǿɽ�Ⱥ��solvable group����
֤����n��5��S_n���ɽ⡣
֤���������׵�Ⱥû�з�ƽ��������Ⱥ�������з�ƽ����Ⱥ��
֤����S_3��2������Ԫ����������ʱ����ת120��r=(2,3,1)��������תf=(3,2,1)��
����S_n�ɶԻ�{(1 2)��(2 3)����(n 1)}���ɡ�
��S_n����Ⱥ��ʾC_6=U_6=����x^6=1��٤����Ⱥ��
���C_6��������Ⱥ��
2004.12.15-2009.9.2
C_4�ķ�ƽ����Ⱥ��C_2��
C_3={I,a,a^2}��1������Ԫa=(a^(-1))^2��a^2=a^(-1)
S_6��2������Ԫ
���Ό��QȺS_3Ҳ�ǿɽ�Ⱥ��һϵ�е���Ⱥ��{e}��A_3��S_3����ȺS_3/H�����Ԫ�صĽ��QȺ��A_3/{e}�䌍�cA_3��ƣ���A_3������Ԫ�صĽ��QȺ��
�ĴΌ��QȺS4߀�ǿɽ�Ⱥ��һϵ�е���Ⱥ��{e}��V��A_4��S_4��
A_4��4�ν��eȺ����{(1), (123), (132), (124), (142), (134), (143), (234), (243), (12)(34), (13)(24), (14)(23)}
V={(1),(12)(34),(13)(24),(14)(23)}
A_4��S_4����Ҏ��Ⱥ��V��A_4����Ҏ��Ⱥ��{(1)}��V����Ҏ��Ⱥ
����S_4/A_4�Ƕ���Ԫ�صĽ��QȺ��A_4/V������Ԫ�صĽ��QȺ��V/{e}�t���Ă�Ԫ�صĽ��QȺ�����S_4�ǿɽ�Ⱥ��
A5�Ǻ���Ⱥ��Ҳ�����fA_5����{e}�c�Լ�������Ҳ�]����������Ҏ��Ⱥ��
��S_5����{e}���Լ��Լ�A_5��A_5��S_5֮��Ҏ��Ⱥ�����A_5����ǡ��S_5֮������һ�룩����Ҳ�]��������Ҏ��Ⱥ��
S_5����{e},A_5,S_5������Ҳ�]����������Ҏ��Ⱥ������Ψһ���ҵ�һϵ�е���Ⱥ
�mȻS_5/A_5�ǽ��QȺ��A_5/{e}�cA_5���ȴ�ǽ��QȺ�����S_5�Ƿǿɽ�Ⱥ��
S_n[��n��Ԫ�صļ���A�ϵ��û�ȫ���ΪS]ͨ�����Q��n�Ό��QȺ��Symmetric Group of degree n����������Ⱥ�ֳ�Ϊn���û�Ⱥ��permutation group����
���У�Permutation��
ѭ�h��cycle��
���Q��transposition��
���Ա�ʾ���攵�����Q�ĺϳɵ����оͽ��������У����һ�����п��Ա��ż�������Q�ĺϳɣ��t�Q��ż���С�
����(123)(456)=(13)(12)(46)(45)����ż���С�
���(1)=(12)(12)�����Ԇ�λԪ��(1)Ҳ��ż���С�
һ����ԣ�������������n�� S_n������ż�������ɵļ��Ϙ���һ����ȺA_n��A_n�Q��n�ν��eȺ/����Ⱥ��Alternating Group of degree n����
*/
struct S3
{
public:
  S3(int a1=1,int a2=2,int a3=3)
  {
    m_a1=a1;
    m_a2=a2;
    m_a3=a3;
  }
  bool operator==(const S3 &a)
  {
    return (m_a1==a.m_a1 && m_a2==a.m_a2 && m_a3==a.m_a3);
  }
  S3 operator*(const S3 &a)
  {
    //int tArr[3]={this->m_a1,this->m_a2,this->m_a3};
    int aArr[3]={a.m_a1,a.m_a2,a.m_a3};
    S3 ret;
    ret.m_a1=aArr[this->m_a1-1];
    ret.m_a2=aArr[this->m_a2-1];
    ret.m_a3=aArr[this->m_a3-1];
    return ret;
    ////�����Ǵ�����û�Ⱥ�˷����룺
    //for(int i=0;i<3;i++)
    //{
    // for(int j=0;j<3;j++)
    // {
    //  if(aArr[j]==tArr[i])
    //   tArr[i]=aArr[j];
    //  break;
    // }
    // return S3(tArr[0],tArr[1],tArr[2]);
    //}
  }
  S3 InvMul()const
  {
    static S3 gS3[]={S3(1,2,3),S3(2,3,1),S3(3,1,2),S3(3,2,1),S3(1,3,2),S3(2,1,3)};
    for(int i=0;i<6;i++)
    {
      if(gS3[i]*(*this)==gS3[0])
        return gS3[i];
    }
    return S3(0,0,0);
  }
  const char *getName()
  {
    static S3 gS3[]={S3(1,2,3),S3(2,3,1),S3(3,1,2),S3(3,2,1),S3(1,3,2),S3(2,1,3)};
    static const char *szName[]={"I","r","rr","f","fr","frr"};
    for(int i=0;i<6;i++)
    {
      if((*this)==gS3[i])
        return szName[i];
    }
    return "Error";
  }
  //�ܷ�ӳԪ�ؽ׵��û����ʽ(���������߹��̡�)��������˫���û����ʽ(���������߽����)
  int getOrderForm()
  {
    static S3 gS3[]={S3(1,2,3),S3(2,3,1),S3(3,1,2),S3(3,2,1),S3(1,3,2),S3(2,1,3)};
    static const char *szName[]={"I","r","rr","f","fr","frr"};
    static int OrderForm[]={1,231,132,13,23,12};
    for(int i=0;i<6;i++)
    {
      if((*this)==gS3[i])
        return OrderForm[i];
    }
    return 0;
  }
  friend ostream& operator<<(ostream& os,S3& a);
public:
  int m_a1,m_a2,m_a3;//˫���û����ʽ
};
ostream& operator<<(ostream& os,S3& a)
{
  cout<<a.getName();
  return os;
}
bool IsGroup(vector<S3> &vec_gS3,S3 id)
{
  if(find(vec_gS3.begin(),vec_gS3.end(),id)==vec_gS3.end())
  {
    return false;
  }
  //�˷������
  for(int i=0;i<vec_gS3.size();i++)
  {
    for(int j=i;j<vec_gS3.size();j++)
    {
      S3 ij=vec_gS3[i]*vec_gS3[j];
      S3 ji=vec_gS3[j]*vec_gS3[i];
      if(find(vec_gS3.begin(),vec_gS3.end(),ij)==vec_gS3.end())
      {
        return false;
      }
      if(find(vec_gS3.begin(),vec_gS3.end(),ji)==vec_gS3.end())
      {
        return false;
      }
    }
  }
  //�г˷���Ԫ
  for(int i=0;i<vec_gS3.size();i++)
  {
    S3 inv=vec_gS3[i].InvMul();
    if(find(vec_gS3.begin(),vec_gS3.end(),inv)==vec_gS3.end())
    {
      return false;
    }
  }
  //id�ǳ˷���λԪ
  for(int i=0;i<vec_gS3.size();i++)
  {
    if(id*vec_gS3[i]==vec_gS3[i] && vec_gS3[i]*id==vec_gS3[i])
      continue;
    else
      return false;
  }
  return true;
}
static S3 gS3[]={S3(1,2,3),S3(2,3,1),S3(3,1,2),S3(3,2,1),S3(1,3,2),S3(2,1,3)};
vector<S3> vec_gA3(gS3,gS3+3);
vector<S3> vec_gS3(gS3,gS3+6);
vector<S3> vec_ge(gS3,gS3+1);
vector<S3> vec_gA(gS3,gS3+4);
int main()
{
  //����ż�û�
  cout<<S3(1,2,3)<<endl;
  cout<<S3(2,3,1)<<endl;
  cout<<S3(3,1,2)<<endl;
  cout<<S3(1,2,3)*S3(1,2,3)<<endl;
  cout<<S3(1,2,3)*S3(2,3,1)<<endl;
  cout<<S3(1,2,3)*S3(3,1,2)<<endl;
  bool ret=IsGroup(vec_gA3,S3(1,2,3));
  cout<<"ret="<<ret<<endl;
  bool ret1=IsGroup(vec_gS3,S3(1,2,3));
  cout<<"ret1="<<ret1<<endl;
  bool ret2=IsGroup(vec_ge,S3(1,2,3));
  cout<<"ret2="<<ret2<<endl;
  vector<S3> vec_gH2;
  vec_gH2.push_back(S3(1,2,3));
  vec_gH2.push_back(S3(2,1,3));
  bool ret3=IsGroup(vec_gH2,S3(1,2,3));
  cout<<"ret3="<<ret3<<endl;
  bool ret4=IsGroup(vec_gA,S3(1,2,3));
  cout<<"ret4="<<ret4<<endl;
  system("pause");
  return 0;
}
// ���������˷�

#include<string.h>
#include<iostream>
using namespace std;
#include"ZnElement.h"
bool Brmul(ZnElement *a,ZnElement *b,int m,int n,int k,ZnElement *c)
{
  int p=a[0].m_mod;
  if(p<=0)
    return false;
  if(p!=b[0].m_mod)
    return false;
  int u;
  for (int i=0; i<=m-1; i++)
    for (int j=0; j<=k-1; j++)
    {
      u=i*k+j;
      c[u]=ZnElement(p,0);
      for(int l=0; l<=n-1; l++)
        c[u]=c[u]+a[i*n+l]*b[l*k+j];
      //����ҲOK
      //{
      // c[u].m_k=c[u].m_k+(a[i*n+l]*b[l*k+j]).m_k;
      // c[u].m_k=c[u].m_k%p;
      //}
    }
    return true;
}
int main(void)
{
  int mtxA[3][3]={{1,2,3},{0,5,6},{0,0,9}};
  int mtxB[3][3]={{1,3,9},{0,7,1},{0,0,2}};
  int mtxC[3][3]={0};
  ZnElement a[3][3]={0};
  ZnElement b[3][3]={0};
  ZnElement c[3][3]={0};
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
    {
      a[i][j]=ZnElement(17,mtxA[i][j]);
      b[i][j]=ZnElement(17,mtxB[i][j]);
    }
    bool bret=Brmul(&a[0][0],&b[0][0],3,3,3,&c[0][0]);
    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
      {
        mtxC[i][j]=c[i][j].m_k;
      }
      system("pause");
      return 0;
}

http://www.docin.com/p-706641170.html
���ǧ���������޾���Ⱥ
ժҪ
��RΪ�е�λԪ�Ľ�������GL(n,R)ΪR�����е�n�׿������ļ��ϣ���GL(n,R)���ھ���ĳ˷�����һ��Ⱥ��GL(n,R)��ΪR�ϴ�Ϊn��һ������Ⱥ��
������Ҫ�о���������Q�ϵ�һ������ȺGL(n,Q)��������Ⱥ�Ľṹ��
�ؼ��ʣ�һ������Ⱥ������Ⱥ������Ⱥ�����ڣ�������

#endif
#endif
