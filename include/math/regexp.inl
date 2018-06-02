enum {
  REOP_OR = 256, // |
  REOP_CAT, //
  REOP_STAR, // *
  REOP_ANY, // .
};
#define re_isopt(ch) (((ch) == '*' || (ch) == '|' || (ch) == '^'))
#define re_ischr(ch) (!re_isopt(ch) && (ch) != '(' && (ch) != ')' && (ch) != '\0')
//�Ϸ��Լ���
int re_check(const uchar* expr, int len)
{
  int i, prech = 0, ch;
  for (i = 0; i < len - 1; i++) {
    ch = expr[i];
    if (prech == '(' || prech == '*' || prech == '|' || prech == ')') {
      int t1 = i + 1;
      switch (prech) {
      case '(':
        if (ch == '*' || ch == '|' || ch == ')') {
          return FALSE;
        }
        break;
      case '*':
        if (ch == '*') {
          return FALSE;
        }
        break;
      case '|':
        if (ch == '*' || ch == '|' || ch == ')') {
          return FALSE;
        }
        else if (re_ischr(ch)) {
        }
        else {
          return FALSE;
        }
        break;
      }
    }
    else {
      return FALSE;
    }
    prech = ch;
  }
  return TRUE;
}
int re_to_infix(const uchar* expr, int len, uchar* outexpr, int maxlen)
{
  int i, j = 0, prech = 0;
  for (i = 0; i < len;) {
    uchar ch = expr[i++];
    if (re_ischr(ch)) {
      if (re_ischr(prech) || ')' == prech) {
        outexpr[j++] = '^';
      }
      if ('\\' == ch) {
        if (i < len) {
          ch = expr[i++];
        }
        else {
          return 0;
        }
      }
      outexpr[j++] = ch;
    }
    else if ('(' == ch) {
      if (re_ischr(prech)) {
        outexpr[j++] = '^';
      }
      outexpr[j++] = ch;
    }
    else {
      outexpr[j++] = ch;
    }
    prech = ch;
  }
  outexpr[j++] = '#';
  outexpr[j] = 0;
  return j;
}
// ��������ȼ�
// *��ߣ����ϣ�
// ^��֮
// |���
uchar re_opt_priority(int a, int b)
{
  int i, j;
  const uchar* c[] = {
    "0#)*|^(",
    "#ex<<<<",
    "(x=<<<<",
    "^>><>><",
    "*>>>>><",
    "|>><><<",
    ")>>>>>x",
  };
  for (i = 1; i < 6; i++)if (c[i][0] == a) {
      break;
    }
  for (j = 1; j < 6; j++)if (c[0][j] == b) {
      break;
    }
  return c[i][j];
}
// ת���ɿɼ������׺���ʽ
// ��  �� ^ ��ʾ
// ��  �� | ��ʾ
// �հ� �� * ��ʾ
//�������ַ�^����
int re_infix_to_postfix(const uchar* infix, int len, uchar* postfix, int maxlen)
{
  int i, j = 0, ret = 1;
  uchar optstack[256];
  uchar prech = 0, ch = 0;
  int top = 0;
  optstack[top++] = '#';
  for (i = 0; i < len;) {
    ch = infix[i++];
    if (re_ischr(ch) && ch != '#') {
      // ��������ջ
      postfix[j++] = ch;
    }
    else {  // �����
      switch (re_opt_priority(optstack[top - 1], ch)) {
      case '<':// ջ����������ȼ�С�� ��������������ջ
        optstack[top++] = ch;
        break;
      case '>':
        postfix[j++] = optstack[--top];
        i--;
        break;
        // ���� ������
      case 'e':
        postfix[j] = 0;
        return j;
        // ���� ��������
      case 'x':
        postfix[j] = 0;
        return 0;
      case '=':
        --top;
        break;
      }
    }
  }
  postfix[j] = 0;
  return j;
}
typedef struct EDGE {           // �Զ����ı�
  int start, end;             // ��� �յ�
  uchar input_expr;            // �����ַ�
} EDGE;
typedef struct nfa_t {
  EDGE edge[256];
  int start;
  int end;
  int edgelen;
  int stlen;
} nfa_t;
// NFA ��ʾ
int re_print_NFA(const nfa_t* nfa)
{
  int i;
  const EDGE* edge = nfa->edge;
  printf("%d [%d -> %d]\n", nfa->stlen, nfa->start, nfa->end);
  for (i = 0; i < nfa->edgelen; i++) {
    printf("%2d [%2d -> %2d] %c\n", i, edge[i].start, edge[i].end, edge[i].input_expr);
  }
  return 0;
}
#define setedge(x, st, en, input) ((x).start=st,(x).end=en,(x).input_expr=input)
// ����׺ʽת����NFA
int re2nfa(const uchar* expr, int len, nfa_t* nfa)
{
  EDGE s_NFA[256];       // ���������NFA
  int i, k, j = 0, top = 0;
  EDGE l_nfa; // �������
  EDGE r_nfa; // �Ҳ�����
  uchar Regular_Input[256] = {0};
  EDGE* edge = nfa->edge;
  int stage = 0;
  EDGE ed;
  for (i = 0; i < len;) {
    uchar ch = expr[i++];
    if (re_ischr(ch)) { // ������ ����NFA ��ջ
      // ���������ĸ����
      Regular_Input[ch] = 1;
      setedge(ed, stage, stage + 1, ch);
      s_NFA[top++] = ed;
      edge[j++] = ed;
      stage += 2;
    }
    else {               // ����� ����
      switch (ch) {
      case '^': // ȡ��2������������
        r_nfa = s_NFA[--top]; // �ұ�������
        l_nfa = s_NFA[--top]; // ���������
        for (k = 0; k < j; k++) {
          if (edge[k].start == r_nfa.start) {
            edge[k].start = l_nfa.end;  // ������������
          }
        }
        setedge(ed, l_nfa.start, r_nfa.end, '$'), s_NFA[top++] = ed;
        break;
      case '|': // ȡ��2������������
        r_nfa = s_NFA[--top]; // �ұ�������
        l_nfa = s_NFA[--top]; // ���������
        setedge(ed, stage, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, stage, r_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, r_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, stage, stage + 1, '$'), s_NFA[top++] = ed;
        stage += 2;
        break;
      case '*': // ȡ��1������������
        l_nfa = s_NFA[--top]; // ���������
        setedge(ed, stage, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, stage + 1, '$'), edge[j++] = ed;
        setedge(ed, l_nfa.end, l_nfa.start, '$'), edge[j++] = ed;
        setedge(ed, stage, stage + 1, '$'), edge[j++] = ed;
        s_NFA[top++] = ed;
        stage += 2;
        break;
      }
    }
  }
  nfa->edgelen = j;
  nfa->stlen = 0;
  {
    uchar ss[256] = {0};
    uchar id[256] = {0};
    for (i = 0; i < nfa->edgelen; ++i) {
      ss[nfa->edge[i].start] = 1;
      ss[nfa->edge[i].end] = 1;
    }
    for (j = 0, i = 0; i < 256; ++i) {
      if (ss[i]) {
        ss[i] = j++;
      }
    }
    nfa->stlen = j;
    for (i = 0; i < nfa->edgelen; ++i) {
      nfa->edge[i].start = ss[nfa->edge[i].start];
      nfa->edge[i].end = ss[nfa->edge[i].end];
    }
    // ��¼NFA�� �����յ�
    //startNFA.push_back(s_NFA.Top().start);// ��¼NFA�� ���
    //endNFA.push_back(s_NFA.Top().end);  // ��¼NFA�� �յ�
    for (i = 0; i < top; ++i) {
      nfa->start = ss[s_NFA[i].start];
      nfa->end = ss[s_NFA[i].end];
    }
  }
  return j;
}
#undef setedge
typedef struct dfa_table_t {
  uchar tlb[50][128];
  uchar end[50];
} dfa_table_t;
int dfa_table_match(dfa_table_t* dfa, const uchar* str, int slen)
{
  int i, ret;
  int currentState, nextState = 0; // ��ʼ״̬
  for (i = 0; i < slen; i++) { // ���϶����ַ�
    currentState = nextState;       // Ŀǰ�����
    nextState = dfa->tlb[currentState][str[i]];
    ret = dfa->end[nextState];
    if (ret) { // ��������ˡ����ȫ��ƥ�䣬��i����TestInput�ĳ��ȡ�
      return ret;
    }
  }
  return 0;
}
typedef struct dfa_t {
  EDGE edge[256];
  uchar start[50];
  uchar end[50];
  int edgelen;
  int startlen;
  int endlen;
} dfa_t;
int dfa_match(dfa_t* dfa, const uchar* str, int slen)
{
  int i, j, ret;
  int currentState, nextState = 0; // ��ʼ״̬
  for (i = 0; i < slen; i++) {
    currentState = nextState;
    for (j = 0; j < dfa->edgelen; ++j) {
      if (dfa->edge[j].start == currentState && dfa->edge[j].input_expr == str[i]) {
        nextState = dfa->edge[j].end;
        ret = dfa->end[nextState];
        if (ret) {
          return ret;
        }
        break;
      }
    }
  }
  return 0;
}
int nfa_move(const nfa_t* nfa, const uchar* input, int in_expr, uchar* output)
{
  int i, j, temp_start = 0, cnt = 0;
  const EDGE* edge = nfa->edge;
  for (i = 0; i < nfa->stlen; i++) {
    if (input[i]) {
      for (j = 0; j < nfa->edgelen; j++) {
        if (edge[j].start == i && edge[j].input_expr == in_expr && !output[edge[j].end]) {
          output[edge[j].end] = 1; // ��ӽ�ջ
          ++cnt;
        }
      }
    }
  }
  return cnt;
}
// ����ͨ��in_expr�����״̬
//   input ��㼯��
//   output ����ɵ��Ｏ��
//   EDGE  �߼�
int nfa_move_null(const nfa_t* nfa, uchar* input)
{
  int cnt = 0, cntn = 0;
  for (; cnt = nfa_move(nfa, input, '$', input);) {
    cntn += cnt;
  }
  return cnt;
}
#define nfa_find_null_closure(nfa, stset) nfa_move(nfa, stset, '$')
int nfa2dfa(const nfa_t* nfa, dfa_t* dfa)
{
  uchar* sts;
  uchar* sts1;
  int dst[256]; // ����״̬
  int ndst = 0;
  int end[256] = {0}; // ������̬
  int nend = 0;
  int stlen = nfa->stlen;
  int stsize = 2 * nfa->edgelen * stlen;
  uchar chset[256] = {0};
  int i, ist = 0, tt;
  sts = MALLOC(uchar, stsize);
  MEMSET(sts, 0, stsize);
  sts[nfa->start] = 1;
  nfa_move_null(nfa, sts);
  if (sts[nfa->end]) {
    end[nend++] = ist; // �������̬
  }
  dst[ndst++] = ist++;
  //str_printf_arr("%I8u,", stlen, sts, 70);
  //printf("\n");
  for (; ndst > 0;) {
    int cnt, st = dst[--ndst];
    uchar* sts0 = sts + st * stlen;
    int chsetlen = 0;
    for (i = 0; i < nfa->edgelen; ++i) {
      if ('$' != nfa->edge[i].input_expr && sts0[nfa->edge[i].start]) {
        chset[chsetlen++] = nfa->edge[i].input_expr;
      }
    }
    for (i = 0; i < chsetlen; ++i) { // ���������ֵ�
      sts1 = sts + ist * stlen;
      //memcpy(sts1, sts0, stlen);
      cnt = nfa_move(nfa, sts0, chset[i], sts1);
      nfa_move_null(nfa, sts1);
      tt = memfind_block(sts, sts1, ist, stlen, stlen);
      // ��DFA����ӵ��߼�
      //str_printf_arr("%I8u,", stlen, sts1, 70);
      printf("[%d -> %d] %c\n", st, tt < 0 ? ist : tt, chset[i]);
      if (tt < 0) {
        if (sts1[nfa->end]) {
          end[nend++] = ist; // �������̬
        }
        dst[ndst++] = ist++;
      }
    }
  }
  str_printf_arr("%u,", nend, end, 70);
  printf(" <- end stage\n");
  FREE(sts);
  return 0;
}
#if 0
//NFA ת�� DFA
int nfa2dfa(const nfa_t* nfa, dfa_t* dfa)
{
  int* DFAStateGather;      // DFA״̬����
  int* NFA_state;        // ����״̬
  int* DFA_state;        // ����״̬
  int* result;             // ��ʱ״̬����
  Find_NULL_Closure(startNFA, result, NFA_Edge); // �������㾭��$�ɵ���ļ���
  DFA_state.Add(result);            // ״̬����ѹջ
  NFA_state.push(result);
  DFAStateGather.push_back(result);      // ״̬���
  startDFA.push_back(GetState_Gather(DFAStateGather, result));   // ���
  ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
  int m;
  for (m = 0; m < endNFA.size(); m++) {
    if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // �ҵ���
      break;
    }
  }
  if (m < endNFA.size()) {
    endDFA.push_back(GetState_Gather(DFAStateGather, result)); // �������̬
  }
  else {
    nonEndDFA.push_back(GetState_Gather(DFAStateGather, result)); // ����� ����̬
  }
  ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
  while (!NFA_state.empty()) {         // ���������ҳ����пɵ����״̬����
    vector<int> temp;     // ��ʱ״̬����
    temp = NFA_state.front(); // ȡ�������е�ͷԪ��
    NFA_state.pop();      // ɾ����ͷԪ��
    for (int i = 0; i < Regular_Input.size(); i++) {  // ���������ֵ�
      vector<int> result;               // ��ʱ״̬����
      result.clear();                 // ���
      Move(temp, Regular_Input[i], result , NFA_Edge); // ���ҵ�ǰ���� ͨ�� �ɵ���ļ���
      if (result.size() > 0 && GetState_Gather(DFAStateGather, result) == 0) { // ��� DFAStateGather��û�д�״̬ �����DFAStateGather�������
        DFA_state.Add(result);       // ��NFA״̬ ��ջ
        NFA_state.push(result);      // ��״̬ ������
        DFAStateGather.push_back(result); // ״̬���
        ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
        for (m = 0; m < endNFA.size(); m++) {
          if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // �ҵ���
            break;
          }
        }
        if (m < endNFA.size()) {
          endDFA.push_back(GetState_Gather(DFAStateGather, result)); // �������NFA��̬  �Ͱ�������� DFA��̬
        }
        else {
          nonEndDFA.push_back(GetState_Gather(DFAStateGather, result)); // ��������� ����̬
        }
        ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
      }
      if (result.size() > 0) {        // ��DFA����ӵ��߼�
        EDGE edge;             // DFA��
        edge.start = GetState_Gather(DFAStateGather, temp);
        edge.input_expr = Regular_Input[i];
        edge.end = GetState_Gather(DFAStateGather, result);
        DFA_Edge.push_back(edge);
      }
    }
  }
  return 0;
}
#endif

