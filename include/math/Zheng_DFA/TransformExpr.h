//TransformExpr.h
#ifndef _TRANSFORMEXPR
#define _TRANSFORMEXPR
#include "LinkedStack.h"
#include <vector>      // ʹ��vectorҪ�� using namespace std;
#include <queue>       
using namespace std;
//-----------  �洢�ṹ  ------------------------
typedef struct EDGE                       // �Զ����ı�
{
	int start;                            // ���
	char input_expr;                      // �����ַ�
	int end;                              // �յ�
}EDGE;

typedef struct NFA                        // �����NFA
{
	int start;                            // ���                
	int end;                              // �յ�
}NFA;
//-----------  �洢�ṹ  ------------------------
class TransformExpr
{
public:
	TransformExpr();
	//-----------------------------ת��Ϊ��׺���ʽ----------------------------------------
	void SetExpr(CString m_expr);                                                          // ��������
	CString GetExpr();                                                                     // ��ñ��ʽ                   
	bool IsLegal(CString m_expr);                                                          // �ж�����ĺϷ���
	CString ToRegular_expr(CString m_expr);                                                // ת���ɿ������  ����ʽ 
	CString ToPostfix();                                                                   // ת���ɺ�׺���ʽ
	//-----------------------------ת��Ϊ��׺���ʽ----------------------------------------

	//-----------------------------��׺���ʽ��NFA------------------------------------------
	void Regular_exprToNFA(const char* m_Regular_expr);                                         // ������ʽ��NFA
	CString ShowNFA();                                                                      // ��ʾNFA�ı�
    //-----------------------------��׺���ʽ��NFA------------------------------------------
    
	//----------------------------- NFA��DFA -----------------------------------------------
	void Find_NULL_Closure(vector<int> input, vector<int>&output, vector<EDGE> edge);       // ����ͨ��$����ļ���
	void Move(vector<int> input, char in_expr, vector<int>&result, vector<EDGE> edge);      // ���ҵ�ǰ����ɵ���ļ���
	void NFA_To_DFA();                                                                      // NFA ת��DFA
	//bool IsEquelState(vector<int> left_state, vector<int> right_state);                     // �ж�����״̬�Ƿ�һ�� 
	//bool IsStackExistState(LinkedStack<vector<int> > state, vector<int> JudgeState);         // �ж�ս���Ƿ����д�״̬
	//int GetDFA_State(LinkedStack<vector<int> > state, vector<int> JudgeState);               // ���DFA״̬���
	int GetState_Gather(vector<vector<int> > state, vector<int> JudgeState);                 // �ж�״̬�������ܼ����еı��
	CString ShowDFA();                                                                      // ��ʾDFA�ı�
	//----------------------------- NFA��DFA -----------------------------------------------

    //----------------------------- ��С��DFA ----------------------------------------------
    void MinimizeDFA();                                                                     // ��С��DFA
	int GetDFAState_Gather(vector<vector<int> > state, int istate, char input);              // �ж�istate״̬����input�� ��״̬  ���ܼ����еı��
	bool HaveSameEdge(vector<EDGE> edge, EDGE e);                                           // �жϼ������Ƿ�����ñ���ͬ�ıߡ�
	void CombinSameState();                                                                 // �ϲ���ͬ״̬
	CString ShowMinimizeDFA();                                                              // ��С����ʾDFA�ı�
	//----------------------------- ��С��DFA ----------------------------------------------

	//-----------------------------��������ж�------------------------------------------
	char OperatePriority(char a,char b);                                                                       // ��������ȼ�
	void MakeNFA_Self(char input,NFA* nfa,vector<EDGE>& edge);                                                 // ��������NFA
	void OperateRule_Binary(NFA Operand_left,char Operator,NFA Operand_right,NFA* result,vector<EDGE>& edge);  // ˫Ŀ�������
	void OperateRule_Unary(NFA Operand,char Operator,NFA* result,vector<EDGE>& edge);                          // ��Ŀ�������
	int type(char  m_expr);                                                                                    // �ж��ַ�����1�����  0������
    //-----------------------------��������ж�-------------------------------------------

	//-----------------------------���������ַ���-------------------------------------------
	bool TestInput_MinimizeNFA(CString TestInput);                                          // ��������
	//-----------------------------���������ַ���-------------------------------------------
	void ClearAll();                                                                        // �����������                                        
	~TransformExpr(){}
private:
	CString expr;                     // �������ı��ʽ
public:
	int state;                        // ����״̬
	vector<char> Regular_Input;       // ����ʽ�������
	vector<int>	startNFA;	          // ��¼NFA��   ��ʼ  ״̬��
	vector<int>	endNFA;		          // ��¼NFA��   ��    ״̬��
	vector<EDGE> NFA_Edge;            // ����NFA���б�
    //////////////////////////////////////////////////////////////////////////////
	vector<int> startDFA;             // ��¼DFA����ʼ״̬��   ���� NFA ���ļ���
	vector<int> endDFA;               // ��¼DFA����̬��       ���� NFA �յ�ļ��ϣ���ƽ���״̬�飩
	vector<int> nonEndDFA;            // ����̬������Ʒǽ���״̬�飩
	vector<EDGE> DFA_Edge;	          // ��NFA������ɵ�DFA�ıߵļ���
    //////////////////////////////////////////////////////////////////////////////
    vector<int> MinimizeStartDFA;     // ��¼��С��DFA����ʼ
    vector<int> MinimizeEndDFA;		  // ��С����DFA����̬��
	vector<int> MinimizeNonEndDFA; 	  // ��С����DFA�ķ���̬��
	vector<EDGE> MinimizeDFA_Edge;    // ��С��DFA �߼�
	vector<vector<int> > MinimizeDFAStateGather;    // ��С��DFA״̬����
};


#endif