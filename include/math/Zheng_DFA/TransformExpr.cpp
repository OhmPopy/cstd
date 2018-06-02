#include "stdafx.h"
#include "TransformExpr.h"
TransformExpr::TransformExpr()
{
  expr = "";
  state = 1;
}
void TransformExpr::SetExpr(CString m_expr)
{
  expr = m_expr;
  state = 1;
}
CString TransformExpr::GetExpr()
{
  return expr;
}
void TransformExpr::ClearAll()
{
  Regular_Input.clear();           // ����ʽ�������
  startNFA.clear();                // ��¼NFA��   ��ʼ  ״̬��
  endNFA.clear();                  // ��¼NFA��   ��    ״̬��
  NFA_Edge.clear();                // ����NFA���б�
  //////////////////////////////////////////////////////////////////////////////
  startDFA.clear();                // ��¼DFA����ʼ״̬��   ���� NFA ���ļ���
  endDFA.clear();                  // ��¼DFA����̬��       ���� NFA �յ�ļ��ϣ���ƽ���״̬�飩
  nonEndDFA.clear();               // ����̬������Ʒǽ���״̬�飩
  DFA_Edge.clear();                // ��NFA������ɵ�DFA�ıߵļ���
  //////////////////////////////////////////////////////////////////////////////
  MinimizeStartDFA.clear();        // ��¼��С��DFA����ʼ
  MinimizeEndDFA.clear();          // ��С����DFA����̬��
  MinimizeNonEndDFA.clear();       // ��С����DFA�ķ���̬��
  MinimizeDFA_Edge.clear();        // ��С��DFA �߼�
  MinimizeDFAStateGather.clear();  // ��С��DFA״̬����
}
// ����ı��ʽ��  ��׺���ʽ
/*
* ����Ϸ���
*   �û�ֱ�������
*/
bool TransformExpr::IsLegal(CString m_expr)   //�Ϸ��Լ���
{
  /*bool legal = true;*/
  int len = m_expr.GetLength();

  for (int i = 0; i < len; i++) {
    if (m_expr[i] == '(' || m_expr[i] == '*' || m_expr[i] == '|' || m_expr[i] == ')' || (type(m_expr[i]) == 0)) {
      int temp = i + 1;

      switch (m_expr[i]) {
      case '(':
        if (m_expr[temp] == '*' || m_expr[temp] == '|' || m_expr[temp] == ')') {
          return false;
        }

        break;

      case '*':
        if (m_expr[temp] == '*') {
          return false;
        }

        break;

      case '|':
        if (m_expr[temp] == '*' || m_expr[temp] == '|' || m_expr[temp] == ')') {
          return false;
        }
        else if (type(m_expr[temp]) == 0)
        {}
        else {
          return false;
        }

        break;
      }
    }
    else {
      return false;
    }
  }

  return true;
}


/*
* ת���ɿɼ����������ʽ
*  ���롱   �� ^ ��ʾ
*  ����   �� | ��ʾ
*  ���հ��� �� * ��ʾ
*   �������ַ�^����
*/
CString TransformExpr::ToRegular_expr(CString m_expr)
{
  bool legal = true;
  int len = m_expr.GetLength();
  CString Regular_expr = m_expr;            // ������ʽ

  if (len > 1) {
    for (int i = 1; i < len; i++) {
      if (type(Regular_expr[i]) == 0) { // ������
        if (Regular_expr[i - 1] != '(' && Regular_expr[i - 1] != '|') {
          Regular_expr.Insert(i, "^");
          len++;
          i++;
        }
      }

      if (Regular_expr[i] == '(') {
        if (Regular_expr[i - 1] == ')' ||  type(Regular_expr[i - 1]) == 0) {
          Regular_expr.Insert(i, "^");
          len++;
          i++;
        }
      }
    }
  }

  return Regular_expr;
}

// ��������ʽת��Ϊ ��׺���ʽ
CString TransformExpr::ToPostfix()
{
  LinkedStack<CString> s_Figure;    // ������ջ
  LinkedStack<CString> Operator;    // �����ջ
  CString input_expr = expr;
  CString output_expr = "";

  input_expr += "#";   // ��������#��ӵ�ĩβ
  Operator.Add("#");   // ���Ƚ�#ѹջ

  int len = input_expr.GetLength();

  for (int i = 0; i < len; i ++) {
    if (type(input_expr[i]) == 0 && input_expr[i] != '#') { // ��������ջ
      output_expr += input_expr.Mid(i, 1);  // �����׺���ʽ
    }
    else { // �����
      CString top_expr = Operator.Top();       // ջ�������
      CString temp_expr = input_expr.Mid(i, 1); // ��ǰ�ַ�
      CString delete_expr = "";
      char top_char = top_expr[0];   //CString ת char
      char temp_char = temp_expr[0]; //CString ת char

      switch (OperatePriority(top_char, temp_char)) {
      case '<':// ջ����������ȼ�С��  ��������������ջ
        Operator.Add(temp_expr);
        break;

      case '>':
        Operator.Delete(delete_expr);
        output_expr += delete_expr;
        i--;
        break;

      case 'e':   // ���� ������
        break;
        break;

      case 'x':   // ���� ��������
        output_expr = "ERROR";
        break;
        break;

      case '=':
        Operator.Delete(delete_expr);
        break;
      }
    }
  }

  return output_expr;
}
//--------------------- ����ı��ʽ��  ��׺���ʽ---------------------------------------------------------------------

//--------------------- ��׺���ʽ�� NFA ------------------------------------------------------------------------------
// ����׺ʽת����NFA
void TransformExpr::Regular_exprToNFA(const char* m_Regular_expr)
{
  int len = strlen(m_Regular_expr);
  LinkedStack<NFA>  s_NFA;              // ���������NFA

  NFA left_nfa;  // �������
  NFA right_nfa; // �Ҳ�����
  NFA result;    // ����������

  for (int i = 0; i < len; i++) {
    if (type(m_Regular_expr[i]) == 0) { // ������ ����NFA ��ջ
      ////////////  STD::find�÷�
      //find��v.begin����,v.end����,7��!= v.end����;   //�ҵ�
      //find��v.begin����,v.end����,4��== v.end������; //δ�ҵ�
      //   STD::find�÷�
      if (find(Regular_Input.begin(), Regular_Input.end(), m_Regular_expr[i]) == Regular_Input.end()) {
        Regular_Input.push_back(m_Regular_expr[i]);  // δ�ҵ� ���������ĸ����
      }

      NFA temp;                       // ��ʱ��������NFA
      temp.start = 0;
      temp.end = 0;
      MakeNFA_Self(m_Regular_expr[i], &temp, NFA_Edge);
      s_NFA.Add(temp);
    }
    else {                              // ����� ����
      switch (m_Regular_expr[i]) {
      case '|':  // ȡ��2������������
        s_NFA.Delete(right_nfa); // �ұ�������
        s_NFA.Delete(left_nfa);  // ���������
        OperateRule_Binary(left_nfa, m_Regular_expr[i], right_nfa, &result, NFA_Edge);
        s_NFA.Add(result);
        break;

      case '^':  // ȡ��2������������
        s_NFA.Delete(right_nfa); // �ұ�������
        s_NFA.Delete(left_nfa);  // ���������
        OperateRule_Binary(left_nfa, m_Regular_expr[i], right_nfa, &result, NFA_Edge);
        s_NFA.Add(result);
        break;

      case '*':  // ȡ��1������������
        s_NFA.Delete(left_nfa);  // ���������
        OperateRule_Unary(left_nfa, m_Regular_expr[i], &result, NFA_Edge);
        s_NFA.Add(result);
        break;
      }
    }
  }

  if (!s_NFA.IsEmpty()) {                   // ��¼NFA�� �����յ�
    startNFA.push_back(s_NFA.Top().start);// ��¼NFA�� ���
    endNFA.push_back(s_NFA.Top().end);    // ��¼NFA�� �յ�
  }
}
//--------------------- ��׺���ʽ�� NFA ------------------------------------------------------------------------------

//--------------------- NFA �� DFA ------------------------------------------------------------------------------------
//NFA ת�� DFA
void TransformExpr::NFA_To_DFA()
{
  vector<vector<int> > DFAStateGather;          // DFA״̬����
  queue<vector<int> > NFA_state;                // ����״̬
  LinkedStack<vector<int> > DFA_state;          // ����״̬
  vector<int> result;                          // ��ʱ״̬����

  Find_NULL_Closure(startNFA, result, NFA_Edge); // �������㾭��$�ɵ���ļ���

  DFA_state.Add(result);                       // ״̬����ѹջ
  NFA_state.push(result);
  DFAStateGather.push_back(result);            // ״̬���
  startDFA.push_back(GetState_Gather(DFAStateGather, result));     // ���
  ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
  int m;

  for (m = 0; m < endNFA.size(); m++) {
    if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // �ҵ���
      break;
    }
  }

  if (m < endNFA.size()) {
    endDFA.push_back(GetState_Gather(DFAStateGather, result));  // �����  ��̬
  }
  else {
    nonEndDFA.push_back(GetState_Gather(DFAStateGather, result));  // �����  ����̬
  }

  ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
  while (!NFA_state.empty()) {                 // �������� �ҳ����пɵ����״̬����
    vector<int> temp;          // ��ʱ״̬����
    temp = NFA_state.front();  // ȡ�������е�ͷԪ��
    NFA_state.pop();           // ɾ����ͷԪ��

    for (int i = 0; i < Regular_Input.size(); i++) {     // ���������ֵ�
      vector<int> result;                              // ��ʱ״̬����
      result.clear();                                  // ���
      Move(temp, Regular_Input[i], result , NFA_Edge); // ���ҵ�ǰ���� ͨ�� �ɵ���ļ���

      if (result.size() > 0 && GetState_Gather(DFAStateGather, result) == 0) { // ��� DFAStateGather��û�д�״̬  �����DFAStateGather�������
        DFA_state.Add(result);             // ��NFA״̬ ��ջ
        NFA_state.push(result);            // ��״̬ ������
        DFAStateGather.push_back(result);  // ״̬���

        ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
        for (m = 0; m < endNFA.size(); m++) {
          if (find(result.begin(), result.end(), endNFA[m]) != result.end()) { // �ҵ���
            break;
          }
        }

        if (m < endNFA.size()) {
          endDFA.push_back(GetState_Gather(DFAStateGather, result));  // �������NFA��̬   �Ͱ��������  DFA��̬
        }
        else {
          nonEndDFA.push_back(GetState_Gather(DFAStateGather, result));  // ���������  ����̬
        }

        ////////////////////��¼��̬ �� ����̬////////////////////////////////////////////
      }

      if (result.size() > 0) {               // ��DFA����ӵ��߼�
        EDGE edge;                         // DFA��
        edge.start = GetState_Gather(DFAStateGather, temp);
        edge.input_expr = Regular_Input[i];
        edge.end = GetState_Gather(DFAStateGather, result);
        DFA_Edge.push_back(edge);
      }
    }
  }
}
// ��С��DFA
void TransformExpr::MinimizeDFA()
{
  queue<vector<int> >  que_MinimizeDFAGather;     // ����  ����������м�״̬����
  MinimizeDFAStateGather.push_back(endDFA);      // �����̬    --[��Ӽ���ԭ���������״̬�ٵļ���]
  que_MinimizeDFAGather.push(endDFA);            // �����̬
  MinimizeDFAStateGather.push_back(nonEndDFA);   // ��ӷ���̬  {{��̬}��{����̬}}
  que_MinimizeDFAGather.push(nonEndDFA);         // ��ӷ���̬

  //--------- ���ÿ����ָ�����л���  �ҳ�һ����״̬ --------------------
  while (!que_MinimizeDFAGather.empty()) {
    vector<int> temp_state;
    temp_state = que_MinimizeDFAGather.front();                  // ȡ������ͷ�������� ��ʱ����
    que_MinimizeDFAGather.pop();                                 // ɾ��
    bool IsSplitable = false;                                     // �ж� ������ַ��ܷ�����

    if (temp_state.size() > 1) {                                 // ��������1  ���б�Ҫ��
      int current_pos = 0;                                     // ���浱ǰ����  ���ܼ����е�λ��

      for (int index = 0; index < MinimizeDFAStateGather.size(); index++) {
        if (temp_state == MinimizeDFAStateGather[index]) {
          current_pos = index;
          break;
        }
      }

      for (int k = 0; k < Regular_Input.size() && !IsSplitable; k++) { // ������ַ����з��루���A�ܷ��� �Ͳ���B��
        vector<vector<int> > GoadGather;                      // �����ַ��� ����״̬����ļ���
        GoadGather.clear();

        for (int j = 0; j < temp_state.size(); j++) {        // ����Ҫ��ֵļ��� Ԫ��
          GoadGather.resize(MinimizeDFAStateGather.size() + temp_state.size() + 1);      // ���·���ռ�  �����ÿ��״̬����ͬ
          int GoadState = 0;
          GoadState = GetDFAState_Gather(MinimizeDFAStateGather, temp_state[j], Regular_Input[k]);

          if (GoadState >= 0) {
            GoadGather[GoadState].push_back(temp_state[j]);
          }
          else {                                           // ����û������  ����Ŀ��״̬Ϊ��  �����¿ռ�
            GoadGather[MinimizeDFAStateGather.size()].push_back(temp_state[j]);
          }
        }

        for (int stateIndex = 0; stateIndex < GoadGather.size() && GoadGather.size() > 0; stateIndex++) {
          if (GoadGather[stateIndex].size() == temp_state.size()) {
            break;  // ����Ԫ�غ��״̬��һ��   ���������ַ���֤
          }
          else if (GoadGather[stateIndex].size() > 0) {    // ��״̬   ��Ҫ����
            if (MinimizeDFAStateGather[current_pos] == temp_state) {
              MinimizeDFAStateGather.erase(MinimizeDFAStateGather.begin() + current_pos);  // ɾ��ĳλ�õ�����
            }

            IsSplitable = true;                                                            // �Ļ���
            MinimizeDFAStateGather.push_back(GoadGather[stateIndex]);                      // ��ӽ�  ״̬����
            que_MinimizeDFAGather.push(GoadGather[stateIndex]);                            // ��ӽ�����
          }
          else {
            continue;  // �ռ�  ���ù�
          }
        }
      }
    }
  }

  //--------- ���ÿ����ָ�����л���  �ҳ�һ����״̬ ------------------------
}
void TransformExpr::CombinSameState()
{
  //--------- �����С���߼�  ��״̬һ���ļ�����һ��Ԫ�ش��� ----------------
  //ԭ���ü����е�һ�����������ģ���ȥ����״̬X���Ѵ���ȥ״̬X���������Ļ�����������״̬Y��
  int i, j;
  vector<EDGE> tempDFAEdge;
  tempDFAEdge = DFA_Edge;                                    // ��ʱ�߼�  �Ѿ�����ͬ�ı���һ������

  for (i = 0; i < MinimizeDFAStateGather.size(); i++) {  // ���ҿɺϲ��ļ��ϲ�����
    if (MinimizeDFAStateGather[i].size() > 1) {            // �ɺϲ��ļ���
      vector<int> tempState;
      tempState.clear();
      tempState = MinimizeDFAStateGather[i];
      int RepState = MinimizeDFAStateGather[i][0];       // ȡ��һ��Ԫ��Ϊ����

      for (j = 1; j < tempState.size(); j++) {       // �滻Ԫ��
        for (int k = 0; k < tempDFAEdge.size(); k++) {
          //EDGE e;
          if (tempDFAEdge[k].start == tempState[j]) { // ���һ��  �滻
            tempDFAEdge[k].start = RepState;
          }

          if (tempDFAEdge[k].end == tempState[j]) {  // �յ�һ��  �滻
            tempDFAEdge[k].end = RepState;
          }
        }
      }
    }
  }

  for (i = 0; i < tempDFAEdge.size(); i++) {             // ɾ��һ����״̬
    EDGE e;
    e.start = tempDFAEdge[i].start;
    e.input_expr = tempDFAEdge[i].input_expr;
    e.end = tempDFAEdge[i].end;

    if (i == 0) {                                          // ��ӵ�һ��
      MinimizeDFA_Edge.push_back(e);
    }
    else {
      if (!HaveSameEdge(MinimizeDFA_Edge, e)) {          // ����ڼ����в����ڴ�״̬  ���
        MinimizeDFA_Edge.push_back(e);
      }
    }
  }

  //--------- �����С���߼�  ��״̬һ���ļ�����һ��Ԫ�ش��� ----------------

  //--------- ������̬ �� ����̬ --------------------------------------------
  for (i = 0; i < MinimizeDFAStateGather.size(); i++) {
    int state = 0;

    if (find(MinimizeDFAStateGather[i].begin(), MinimizeDFAStateGather[i].end(), startDFA.front()) != MinimizeDFAStateGather[i].end()) {
      MinimizeStartDFA.push_back(MinimizeDFAStateGather[i][0]);   // ѡ�������һ����
    }

    for (j = 0; j < MinimizeDFAStateGather[i].size(); j++) {
      if (find(endDFA.begin(), endDFA.end(), MinimizeDFAStateGather[i][j]) != endDFA.end()) { // ��̬
        if (find(MinimizeEndDFA.begin(), MinimizeEndDFA.end(), MinimizeDFAStateGather[i][0]) == MinimizeEndDFA.end()) { // ������û�д�״̬
          MinimizeEndDFA.push_back(MinimizeDFAStateGather[i][0]);
        }
      }
      else {
        if (find(MinimizeNonEndDFA.begin(), MinimizeNonEndDFA.end(), MinimizeDFAStateGather[i][0]) == MinimizeNonEndDFA.end()) {
          MinimizeNonEndDFA.push_back(MinimizeDFAStateGather[i][0]);
        }
      }
    }
  }

  sort(MinimizeEndDFA.begin(), MinimizeEndDFA.end());
  sort(MinimizeNonEndDFA.begin(), MinimizeNonEndDFA.end());
  //--------- ������̬ �� ����̬ --------------------------------------------
}
//--------------------- ��С�� DFA ------------------------------------------------------------------------------------

bool TransformExpr::TestInput_MinimizeNFA(CString TestInput)
{
  int currentState, nextState;
  nextState = MinimizeStartDFA.front();      // ��ʼ״̬
  int i = 0;

  for (; i < TestInput.GetLength(); i++) {   // ���϶����ַ�
    currentState = nextState;              // Ŀǰ�����
    int j = 0;                             // ����������

    for (; j < MinimizeDFA_Edge.size(); j++) { // ������
      if (MinimizeDFA_Edge[j].start == currentState && MinimizeDFA_Edge[j].input_expr == TestInput[i]) { // �ҵ�ͨ��
        nextState = MinimizeDFA_Edge[j].end; // ��¼�¸���ʼ״̬
        break;
      }
    }

    if (j == MinimizeDFA_Edge.size()) {    // ��������ˡ����ȫ��ƥ�䣬��i����TestInput�ĳ��ȡ�
      break;  //           ������в�ƥ��ģ���i С��TestInput�ĳ��ȡ�
    }
  }

  if (i == TestInput.GetLength() && find(MinimizeEndDFA.begin(), MinimizeEndDFA.end(), nextState) != MinimizeEndDFA.end()) { // ���״̬���� �ɽ���״̬��
    return true;
  }
  else {
    return false;
  }
}
//--------------------- �������  -------------------------------------------------------------------------------------

/* ����ͨ��$�����״̬
*    input  ��㼯��
*    output ����ɵ��Ｏ��
*    EDGE   �߼�
*/
void TransformExpr::Find_NULL_Closure(vector<int> input, vector<int>& output, vector<EDGE> edge)
{
  LinkedStack<int> state;                // ״̬����
  int temp_start = 0;
  output.clear();                        // ���ԭ������

  for (int i = 0; i < input.size(); i++) { //
    state.Add(input[i]);
  }

  while (!state.IsEmpty()) {
    output.push_back(state.Top());     // ���ȼ���
    state.Delete(temp_start);

    for (int i = 0; i < edge.size(); i++) {
      if (edge[i].start == temp_start && edge[i].input_expr == '$') {
        state.Add(edge[i].end);  // ��ӽ�ջ
      }
    }
  }

  sort(output.begin(), output.end());    // �������򣬷�������ж�
}
/*
  inputͨ������in_expr����ļ���
  input   ���뼯��
  in_expr �����ַ�
  result  ��㼯��ͨ�� ��ǰ���� �ɵ���ļ���
*/
void TransformExpr::Move(vector<int> input, char in_expr, vector<int>& result, vector<EDGE> edge)
{
  vector<int> temp_input;
  result.clear();

  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < edge.size(); j++)    // �����߼�
      if (edge[j].start == input[i] && edge[j].input_expr == in_expr) {
        temp_input.push_back(edge[j].end);
      }
  }

  Find_NULL_Closure(temp_input, result, edge); // ����$�ɵ���ļ���
}

// ����input��ļ������ܼ����е�λ��(��0��ʼ��
int TransformExpr::GetDFAState_Gather(std::vector<vector<int> > state, int istate, char input)
{
  int i, GoadState = 0;                            // ����input���Ŀ��״̬

  for (i = 0; i < DFA_Edge.size(); i++) {
    if (DFA_Edge[i].start == istate && DFA_Edge[i].input_expr == input) {
      GoadState = DFA_Edge[i].end;  // �յ�
    }
  }

  for (i = 0; i < state.size(); i++) {
    if (find(state[i].begin(), state[i].end(), GoadState) != state[i].end()) { // �ҵ���
      return i;  // �Ǵ�0��ʼ��
    }
  }

  return -1;                                    // û�ҵ���˵��������input�󵽴�ռ�
}
//  �ж�ĳ�����ڱ߼����Ƿ����
//  ���ڣ�   TRUE
//  �����ڣ� FALSE
bool TransformExpr::HaveSameEdge(vector<EDGE> edge, EDGE e)
{
  for (int i = 0; i < edge.size(); i++) {
    if (edge[i].start == e.start && edge[i].input_expr == e.input_expr && edge[i].end == e.end) {
      return true;
    }
  }

  return false;
}

// ���ĳ��������  �ܼ����е�˳���ţ���1��ʼ��
int TransformExpr::GetState_Gather(std::vector<vector<int> > state, std::vector<int> JudgeState)
{
  int num = 0;

  for (int i = 0; i < state.size(); i++) {
    if (state[i] == JudgeState) {
      return ++i;
    }
  }

  return 0;        // û����
}
// ��������ȼ�
// *��ߣ����ϣ�
// ^��֮
// |���
char TransformExpr::OperatePriority(char a, char b)
{
  int i, j;
  char c[7][7] = {
    {'0', '#', ')', '*', '|', '^', '('},
    {'#', 'e', 'x', '<', '<', '<', '<'},
    {'(', 'x', '=', '<', '<', '<', '<'},
    {'^', '>', '>', '<', '>', '>', '<'},
    {'*', '>', '>', '>', '>', '>', '<'},
    {'|', '>', '>', '<', '>', '<', '<'},
    {')', '>', '>', '>', '>', '>', 'x'},
  };

  for (i = 1; i < 6; i++)if (c[i][0] == a) {
      break;
    }

  for (j = 1; j < 6; j++)if (c[0][j] == b) {
      break;
    }

  return c[i][j];
}
// ÿ�����빹������NFA
// @-->@
void TransformExpr::MakeNFA_Self(char input, NFA* nfa, std::vector<EDGE>& edge)
{
  EDGE e; // �±�

  if (nfa->start == 0 && nfa->end == 0) {
    e.start = nfa->start = state; // ���
    e.input_expr = input;         // ����
    e.end = nfa->end = ++state;   // �ص�
    state++;
  }
  else {
    e.start = nfa->start; // ���
    e.input_expr = input; // ����
    e.end = nfa->end ;    // �ص�
  }

  edge.push_back(e);
}
// ˫Ŀ���������
void TransformExpr::OperateRule_Binary(NFA Operand_left, char Operator, NFA Operand_right, NFA* result, vector<EDGE>& edge)
{
  int i;
  NFA temp;

  switch (Operator) {
  case'^':// ���롱 ����
    result->start = Operand_left.start;
    result->end = Operand_right.end;

    for (i = 0; i < edge.size(); i++) {
      if (edge[i].start == Operand_right.start) {
        edge[i].start = Operand_left.end;    // ������������
      }
    }

    break;

  case'|':// ���� ����
    result->start = temp.start = state;
    temp.end = Operand_left.start;
    MakeNFA_Self('$', &temp, edge);

    temp.start = state;
    temp.end = Operand_right.start;
    MakeNFA_Self('$', &temp, edge);

    temp.start = Operand_left.end;
    temp.end = ++state;
    MakeNFA_Self('$', &temp, edge);

    temp.start = Operand_right.end;
    temp.end = result->end = state;
    MakeNFA_Self('$', &temp, edge);

    state++;
    break;
  };
}
// ���հ��� ����
void TransformExpr::OperateRule_Unary(NFA Operand, char Operator, NFA* result, vector<EDGE>& edge)
{
  NFA temp;
  result->start = temp.start = state;  // ��״̬
  temp.end = Operand.start;
  MakeNFA_Self('$', &temp, edge);

  temp.start = Operand.end;
  result->end = temp.end = ++state;    // ��״̬
  MakeNFA_Self('$', &temp, edge);

  temp.start = Operand.end;
  temp.end = Operand.start;
  MakeNFA_Self('$', &temp, edge);

  temp.start = result->start;
  temp.end = result->end;
  MakeNFA_Self('$', &temp, edge);

  state++;       // ״̬��++

}
// �ж������������ ������
int TransformExpr::type(char m_expr)
{
  if (m_expr == '*' || m_expr == '|' || m_expr == '^') {
    return 1;
  }
  else if (m_expr != '(' && m_expr != ')') {
    return 0;
  }
  else {
    return -1;
  }
}
//--------------------- �������  --------------------------------------------------------------------------------------

//--------------------- ��ʾ      --------------------------------------------------------------------------------------
// NFA ��ʾ
CString TransformExpr::ShowNFA()
{
  CString show_NFA = "";

  for (int i = 0; i < this->NFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->NFA_Edge[i].start);

    //////////////////////////////////////
    char* a;                            //
    a = &this->NFA_Edge[i].input_expr;  //ȡ�������
    a[1] = '\0';                        //������
    input.Format(_T("%s  "), a);        //ת��
    //////////////////////////////////////

    end.Format("%d  ", this->NFA_Edge[i].end);
    show_NFA += ("Index " + num + ":");
    show_NFA += (start + input + end + " \r\n");
  }

  return show_NFA;
}
// DFA ��ʾ
CString TransformExpr::ShowDFA()
{
  CString show_DFA = "";

  for (int i = 0; i < this->DFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->DFA_Edge[i].start);
    //////////////////////////////////////
    char* a;                            //
    a = &this->DFA_Edge[i].input_expr;  //ȡ�������
    a[1] = '\0';                        //������
    input.Format(_T("%s "), a);         //ת��
    //////////////////////////////////////
    end.Format("%d ", this->DFA_Edge[i].end);

    show_DFA += ("Index " + num + ":");
    show_DFA += (start + input + end + "\r\n");
  }

  return show_DFA;
}
// ��С��DFA ��ʾ
CString TransformExpr::ShowMinimizeDFA()
{
  CString show_MinimizeDFA = "";

  for (int i = 0; i < this->MinimizeDFA_Edge.size(); i++) {
    CString num = "";
    CString start = "";
    CString input = "";
    CString end = "";
    num.Format("%d ", i);

    start.Format("%d  ", this->MinimizeDFA_Edge[i].start);
    //////////////////////////////////////
    char* a;                            //
    a = &this->MinimizeDFA_Edge[i].input_expr;  //ȡ�������
    a[1] = '\0';                        //������
    input.Format(_T("%s "), a);         //ת��
    //////////////////////////////////////
    end.Format("%d ", this->MinimizeDFA_Edge[i].end);

    show_MinimizeDFA += ("Index " + num + ":");
    show_MinimizeDFA += (start + input + end + "\r\n");
  }

  return show_MinimizeDFA;
}
//--------------------- ��ʾ      --------------------------------------------------------------------------------------

/* �ж�����״̬�Ƿ�һ��
   һ��    TRUE
   ��һ��  False
*/
//bool TransformExpr::IsEquelState(vector<int> left_state, vector<int> right_state)
//{
//  if (left_state.size()!=right_state.size())
//    return false;
//  else
//  {
//       for (int i = 0;i< left_state.size();i++)
//       if (left_state[i]!= right_state[i])
//         return false;
//  }
//  return true;
//}
///*�жϸ�״̬��ջ���Ƿ����
//  ջ�д���  FALSE
//  ������    TRUE
//*/
//bool TransformExpr::IsStackExistState(LinkedStack<vector<int> > state, std::vector<int> JudgeState)
//{
//  LinkedStack<vector<int> > temp_state;  // Ҫ������ջ
//  LinkedStack<vector<int> > link_state;  // �м����ӵ�ջ
//
//  while(!state.IsEmpty() && state.Top().size()>0)
//  {
//        vector<int> temp;   // ��ʱ����
//    state.Delete(temp);
//    link_state.Add(temp);
//  }
//
//  while(!link_state.IsEmpty() && link_state.Top().size()>0)
//  {
//        vector<int> temp;   // ��ʱ����
//    link_state.Delete(temp);
//    state.Add(temp);
//    temp_state.Add(temp);
//  }
//
//  while(!temp_state.IsEmpty() && temp_state.Top().size()>0)
//  {
//    vector<int> temp;   // ��ʱ����
//    temp_state.Delete(temp);
//        if (temp == JudgeState)
//      return false;
//  }
//
//  return true;
//}
///*
//  ���DFA״̬ ���
//  ��Ű���չ˳��һ�ε�����
//*/
//int TransformExpr::GetDFA_State(LinkedStack<vector<int> > state, vector<int> JudgeState)
//{
//    LinkedStack<vector<int> > temp_state;  // Ҫ������ջ
//  LinkedStack<vector<int> > link_state;  // �м����ӵ�ջ
//    int total = 1;      // ״̬����
//  int pos = 0;        // Ҫ���ҵļ���  ��ջ�е�λ��
//
//  while(!state.IsEmpty() && state.Top().size()>0)
//  {
//        vector<int> temp;   // ��ʱ����
//    state.Delete(temp);
//    link_state.Add(temp);
//
//  }
//
//  while(!link_state.IsEmpty() && link_state.Top().size()>0)
//  {
//        vector<int> temp;   // ��ʱ����
//    link_state.Delete(temp);
//    state.Add(temp);
//    temp_state.Add(temp);
//    if(temp == JudgeState)// ��һ��һ��
//    {
//      pos = total;
//    }
//        total++;
//  }
//  return pos;
//}
//--------------------- NFA �� DFA ------------------------------------------------------------------------------------

//--------------------- ��С�� DFA ------------------------------------------------------------------------------------
// ��С��DFA  �÷ָ
//   ��ʼ���֣���̬�����̬��
//   ���ÿ����ֵĸ���������ѻ����е���Gi��ֱ�������ٷ��ѣ�
//   �����ջ��ֹ���D'���ؼ���ѡ������޸�״̬ת�ƣ�
//   �������ܵ���״̬�Ͳ��ɴ�״̬��
//   ------  ������  ------
//   �����κ�����״̬t��s������һ״̬�������������ַ����أ�
//   ������һ״̬���������ܦأ�
//   ���ߴ�t�����ʹ�s�������ﲻͬ�Ľ���״̬����Ʀض�״̬t��s��
//   �����ֵġ�