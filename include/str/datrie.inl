#include "cstd.h"
#include "str.h"
#include "cfile.h"
#if 0
Javaʵ��˫����Trie��(DoubleArrayTrie, DAT)
��ͳ��Trieʵ�ּ򵥣�����ռ�õĿռ�ʵ�������Խ��ܣ��ر��ǵ��ַ�����������Ӣ��26���ַ���ʱ�򣬱�ը�����Ŀռ�����޷����ܡ�˫����Trie�����Ż��˿ռ��Trie����ԭ���ľͲ����ˣ���ο�An Efficient Implementation of Trie Structures��������ı�дҲ�ǲο���ƪ���ĵġ�
��ͳ��Trieʵ�ּ򵥣�����ռ�õĿռ�ʵ�������Խ��ܣ��ر��ǵ��ַ�����������Ӣ��26���ַ���ʱ�򣬱�ը�����Ŀռ�����޷����ܡ�
˫����Trie�����Ż��˿ռ��Trie����ԭ���ľͲ����ˣ���ο�An Efficient Implementation of Trie Structures��������ı�дҲ�ǲο���ƪ���ĵġ�
���ڼ�������û���ἰ��ϸ�ں������Ĳ�һһ�µ�ʵ�֣�
1.���ڲ����ַ����������һ���ַ�������һ���ַ������Ӵ��Ļ������ǽ�������Ҳ��Ϊһ���ߣ�����һ���µĽ�㣬�������½ڵ��Base����Ϊ0
����һ���ַ�������Ҳ��2�������һ����BaseֵΪ�����洢ʣ���ַ�(����ֻ��һ��������)��Tail���飻��һ����BaseΪ0��
�����ڲ�ѯ��ʱ��Ҫ����һ�����������
2.���ڵ�һ�ֳ�ͻ�������е�Case 3��������Ҫ��Tail�е��ַ���ȡ��һ���֣���Ϊ�߷ŵ������С�������ʹ�ý�β�����Ƶķ�ʽ���ҵķ�ʽֱ���޸�Baseֵ���������ƶ�β����
������javaʵ�ֵĴ��룬���Դ�����ͬ�ַ������룬�Ӵ��Ĳ�������
�����ǲ��Խ��������6WӢ�ĵ��ʵ�DAT�������Ҫ20��
�����������ʱ����ÿ�γ������ӵ�2������ʼ1024
Base��Check����ĳ���Ϊ131072
Tail�ĳ���Ϊ262144
#endif
#define END_CHAR '\0'
#define DEFAULT_LEN 1024
typedef struct datrie_t {
  int* Base;
  int* Check;
  uchar* Tail;
  int Base_length;
  int Check_length;
  int Tail_length;
  int Pos;// = 1
  uchar CharMap[256];
  int CharMap_size;
  //int* CharList;
} datrie_t;
void datrie_free(datrie_t* tr)
{
  FREE(tr->Base);
  FREE(tr->Check);
  FREE(tr->Tail);
}
void datrie_init(datrie_t* tr)
{
  int i;
  tr->Pos = 1;
  tr->Base_length = tr->Check_length = tr->Tail_length = DEFAULT_LEN;
  MYREALLOC(tr->Base, tr->Base_length);
  MYREALLOC(tr->Check, tr->Check_length);
  MYREALLOC(tr->Tail, tr->Tail_length);
  //MYREALLOC(tr->CharList, tr->Check_length);
  tr->Base[1] = 1;
  MEMSET(tr->CharMap, 0, 256);
  tr->CharMap[END_CHAR] = ++tr->CharMap_size;
  //CharList.add(END_CHAR);
  //CharList.add(END_CHAR);
  for (i = 0; i < 26; ++i) {
    tr->CharMap['a' + i] = ++tr->CharMap_size;
    //CharList.add((char)('a' + i));
  }
}
void datrie_extend_array(datrie_t* tr)
{
  tr->Base_length *= 2;
  tr->Check_length *= 2;
  MYREALLOC(tr->Base, tr->Base_length);
  MYREALLOC(tr->Check, tr->Check_length);
}
void datrie_extend_tail(datrie_t* tr)
{
  tr->Tail_length *= 2;
  MYREALLOC(tr->Tail, tr->Tail_length);
}
int datrie_getcharcode(datrie_t* tr, uchar c)
{
  if (!tr->CharMap[(uchar)c]) {
    tr->CharMap[(uchar)c] = ++tr->CharMap_size;
  }
  return tr->CharMap[(uchar)c];
}
int datrie_CopyToTailArray(datrie_t* tr, const char* s, int s_length, int p)
{
  int i, _Pos = tr->Pos;
  while (s_length - p + 1 > tr->Tail_length - tr->Pos) {
    datrie_extend_tail(tr);
  }
  for (i = p; i < s_length; ++i) {
    tr->Tail[_Pos] = s[i];
    _Pos++;
  }
  return _Pos;
}
int datrie_x_check(datrie_t* tr, const int* set, int set_length)
{
  int i, j;
  for (i = 1; ; ++i) {
    bool flag = true;
    for (j = 0; j < set_length; ++j) {
      int cur_p = i + set[j];
      if (cur_p >= tr->Base_length) {
        datrie_extend_array(tr);
      }
      if (tr->Base[cur_p] != 0 || tr->Check[cur_p] != 0) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return i;
    }
  }
}
int datrie_GetChildList(datrie_t* tr, int p, int* ret)
{
  int i, j = 0;
  for (i = 1; i <= tr->CharMap_size; ++i) {
    if (tr->Base[p] + i >= tr->Check_length) {
      break;
    }
    if (tr->Check[tr->Base[p] + i] == p) {
      ret[j++] = (i);
    }
  }
  return j;
}
bool datrie_TailContainString(datrie_t* tr, int start, const uchar* s2, int s2_length)
{
  int i;
  for (i = 0; i < s2_length; ++i) {
    if (s2[i] != tr->Tail[i + start]) {
      return false;
    }
  }
  return true;
}
bool datrie_TailMatchString(datrie_t* tr, int start, const char* s2, int s2_length)
{
  int i;
  for (i = 0; i < s2_length; ++i) {
    if (s2[i] != tr->Tail[i + start]) {
      return false;
    }
  }
  if (END_CHAR != tr->Tail[i + start]) {
    return false;
  }
  return true;
}
void datrie_insert(datrie_t* tr, const char* s0, int s_length)
{
  int i, pre_p = 1, cur_p;
  uchar s[256];
  s_length = s_length < 0 ? strlen(s0) : s_length;
  ASSERT(s_length < 256);
  memcpy(s, s0, s_length);
  s[s_length++] = END_CHAR;
  for (i = 0; i < s_length; ++i) {
    //��ȡ״̬λ��
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, s[i]);
    //������ȳ������У���չ����
    if (cur_p >= tr->Base_length) {
      datrie_extend_array(tr);
    }
    //����״̬
    if (tr->Base[cur_p] == 0 && tr->Check[cur_p] == 0) {
      tr->Base[cur_p] = -tr->Pos;
      tr->Check[cur_p] = pre_p;
      tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 1);
      break;
    }
    else {
      //�Ѵ���״̬
      if (tr->Base[cur_p] > 0 && tr->Check[cur_p] == pre_p) {
        pre_p = cur_p;
        continue;
      }
      else {
        //��ͻ 1������ Base[cur_p]С��0�ģ�������һ����ѹ���浽Tail�е��ַ���
        if (tr->Base[cur_p] < 0 && tr->Check[cur_p] == pre_p) {
          int head = -tr->Base[cur_p];
          if (s[i + 1] == END_CHAR && tr->Tail[head] == END_CHAR) { //�����ظ��ַ���
            break;
          }
          //������ĸ���������Ϊ��һ���ж��Ѿ��ų��˽�����������һ����2�������ǽ�����
          if (tr->Tail[head] == s[i + 1]) {
            int cc = datrie_getcharcode(tr, s[i + 1]);
            int avail_base = datrie_x_check(tr, &cc, 1);
            tr->Base[cur_p] = avail_base;
            tr->Check[avail_base + datrie_getcharcode(tr, s[i + 1])] = cur_p;
            tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = -(head + 1);
            pre_p = cur_p;
            continue;
          }
          else {
            //2����ĸ����ͬ�������������һ��Ϊ������
            int avail_base;
            int cc[2] = {datrie_getcharcode(tr, s[i + 1]), datrie_getcharcode(tr, tr->Tail[head]) };
            avail_base = datrie_x_check(tr, cc, 2);
            tr->Base[cur_p] = avail_base;
            tr->Check[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = cur_p;
            tr->Check[avail_base + datrie_getcharcode(tr, s[i + 1])] = cur_p;
            //Tail ΪEND_FLAG �����
            if (tr->Tail[head] == END_CHAR) {
              tr->Base[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = 0;
            }
            else {
              tr->Base[avail_base + datrie_getcharcode(tr, tr->Tail[head])] = -(head + 1);
            }
            if (s[i + 1] == END_CHAR) {
              tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = 0;
            }
            else {
              tr->Base[avail_base + datrie_getcharcode(tr, s[i + 1])] = -tr->Pos;
            }
            tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 2);
            break;
          }
        }
        else {
          //��ͻ2����ǰ����Ѿ���ռ�ã���Ҫ����pre��base
          if (tr->Check[cur_p] != pre_p) {
            int toBeAdjust;
            int list1[256];
            int list[256];
            int list_length;
            int list1_length = datrie_GetChildList(tr, pre_p, list1);
            int origin_base, avail_base, j, k;
            if (true) {
              toBeAdjust = pre_p;
              MEMCPY(list, list1, list1_length);
              list_length = list1_length;
            }
            origin_base = tr->Base[toBeAdjust];
            list[list_length++] = datrie_getcharcode(tr, s[i]);
            avail_base = datrie_x_check(tr, list, list_length);
            list_length--;
            tr->Base[toBeAdjust] = avail_base;
            for (j = 0; j < list_length; ++j) {
              //BUG
              int tmp1 = origin_base + list[j];
              int tmp2 = avail_base + list[j];
              tr->Base[tmp2] = tr->Base[tmp1];
              tr->Check[tmp2] = tr->Check[tmp1];
              //�к���
              if (tr->Base[tmp1] > 0) {
                int subsequence[256];
                int subsequence_length = datrie_GetChildList(tr, tmp1, subsequence);
                for (k = 0; k < subsequence_length; ++k) {
                  tr->Check[tr->Base[tmp1] + subsequence[k]] = tmp2;
                }
              }
              tr->Base[tmp1] = 0;
              tr->Check[tmp1] = 0;
            }
            //�����µ�cur_p
            cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, s[i]);
            if (s[i] == END_CHAR) {
              tr->Base[cur_p] = 0;
            }
            else {
              tr->Base[cur_p] = -tr->Pos;
            }
            tr->Check[cur_p] = pre_p;
            tr->Pos = datrie_CopyToTailArray(tr, s, s_length, i + 1);
            break;
          }
        }
      }
    }
  }
  return ;
}
bool datrie_exists(datrie_t* tr, const char* word0, int word_length)
{
  int pre_p = 1;
  int cur_p = 0;
  int i;
  const uchar* word = (const uchar*)word0;
  word_length = word_length < 0 ? strlen(word0) : word_length;
  for (i = 0; i < word_length; ++i) {
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, word[i]);
    if (tr->Check[cur_p] != pre_p) {
      return false;
    }
    if (tr->Base[cur_p] < 0) {
      if (datrie_TailMatchString(tr, -tr->Base[cur_p], word + i + 1, word_length - i - 1)) {
        return true;
      }
      return false;
    }
    pre_p = cur_p;
  }
  if (tr->Check[tr->Base[cur_p] + datrie_getcharcode(tr, END_CHAR)] == cur_p) {
    return true;
  }
  return false;
}
//�ڲ�����������ƥ�䵥�ʵ�����Base index
int datrie_find(datrie_t* tr, const char* word, int word_length, char* prefix)
{
  int pre_p = 1;
  int cur_p = 0;
  int i, j = 0, p = -1;
  for (i = 0; i < word_length; ++i) {
    // BUG
    if (prefix) {
      prefix[j++] += word[i];
      prefix[j] = 0;
    }
    cur_p = tr->Base[pre_p] + datrie_getcharcode(tr, (uchar)word[i]);
    if (tr->Check[cur_p] != pre_p) {
      p = -1;
      return p;
    }
    if (tr->Base[cur_p] < 0) {
      if (datrie_TailContainString(tr, -tr->Base[cur_p], word + i + 1, word_length - i - 1)) {
        p = cur_p;
        return p;
      }
      p = -1;
      return p;
    }
    pre_p = cur_p;
  }
  p = cur_p;
  return p;
}
#if 0
ArrayList<const char*> GetAllChildWord(int index)
{
  int i;
  ArrayList<const char*> result = new ArrayList<const char*>();
  if (Base[index] == 0) {
    result.add("");
    return result;
  }
  if (Base[index] < 0) {
    const char* r = "";
    for (int i = -Base[index]; Tail[i] != END_CHAR; ++i) {
      r += Tail[i];
    }
    result.add(r);
    return result;
  }
  for (i = 1; i <= CharMap_size(); ++i) {
    if (Check[Base[index] + i] == index) {
for (const char * s: GetAllChildWord(Base[index] + i)) {
        result.add(CharList.get(i) + s);
      }
      //result.addAll(GetAllChildWord(Base[index]+i));
    }
  }
  return result;
}
public ArrayList<const char*> FindAllWords(const char* word)
{
  ArrayList<const char*> result = new ArrayList<const char*>();
  const char* prefix = "";
  FindStruct fs = datrie_find(word);
  int p = fs.p;
  if (p == -1) {
    return result;
  }
  if (Base[p] < 0) {
    const char* r = "";
    for (int i = -Base[p]; Tail[i] != END_CHAR; ++i) {
      r += Tail[i];
    }
    result.add(fs.prefix + r);
    return result;
  }
  if (Base[p] > 0) {
    ArrayList<const char*> r = GetAllChildWord(p);
    for (int i = 0; i < r_size(); ++i) {
      r.set(i, fs.prefix + r.get(i));
    }
    return r;
  }
  return result;
}
#endif
int test_datrie()
{
  int i;
  vstr_t sv[1] = {0};
  datrie_t tr[1] = {0};
  _chdir("E:/book_txt/�ʵ�");
  vstr_load("ţ��߽�Ӣ��ʵ��8��ʱ�.txt", sv);
  datrie_init(tr);
  for (i = 0; i < sv->n; ++i) {
    datrie_insert(tr, sv->v[i].s, -1);
  }
  printf("Base_length = %d\n", tr->Base_length);
  printf("Tail_length = %d\n", tr->Tail_length);
  for (i = 0; i < sv->n; ++i) {
    ASSERT(datrie_exists(tr, sv->v[i].s, -1));
  }
  return 0;
}

