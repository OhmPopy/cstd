// ��ģʽ�ַ���ƥ���㷨AC�Զ���
1 #include<stdio.h>
2 #include<stdlib.h>
3 #include<ctype.h>
4
5 struct node {
  6 node* child[26];
  7 node* fail;
  8 int flag;
  9
};
10 node* create_node()
11 {
  12 int* p = (int*)(malloc(sizeof(node)));
  13 if (p != NULL) {
    14 for (int i = 0, j = sizeof(node) / sizeof(int); i < j; ++i) {
      15 p[i] = 0;
    }
    16 return (node*)p;
    17
  }
  18 else
  { 19 exit(-1); }
  20
}
21
22 char* modes[10] = {"Word", "shi", "worship", "or", "oral",
    23 "ship", "hiphop", "Hop", "hip", "sword"
                     };
24
25 node* build_trie(char* modes[], int n)
26 {
  27 node* root = create_node();
  28 for (int i = 0; i < n; ++i) {
    29 node* temp = root;
    30 for (int j = 0, c = (modes[i])[j]; c != '\0'; ++j, c = (modes[i])[j]) {
      31 if (c >= 'A' && c <= 'Z') { /* convert all uppercases to lowercases */
        32 c += 32;
      }
      33 int index = c - 'a';
      34 if (temp->child[index] == NULL) {
        35 temp->child[index] = create_node();
      }
      36 temp = temp->child[index];
      37
    }
    38 temp->flag = 1; /* this moment, we reach the end of a word, so set flag 1 */
    39
  }
  40 return root;
  41
}
42
43 void BSF(node* root) /* breadth-first search */
44 {
  45 if (root == NULL) {
    46 return;
  }
  47 node* queue[100];
  48 int head = 0;
  49 int tail = 0;
  50 node* temp = root;
  51 queue[tail++ % 100] = temp;
  52 printf("R");
  53 while (head % 100 != tail % 100) {
    54 temp = queue[head % 100];
    55 for (int i = 0; i < 26; ++i) {
      56 if (temp->child[i] != NULL) {
        57 queue[tail++ % 100] = temp->child[i];
        58 printf(" %c", i + 'a');
        59
      }
      60
    }
    61 ++head;
    62
  }
  63 printf("\n");
  64
}
65
66 void DFS(node* root) /* depth-first search */
67 {
  68 if (root == NULL) {
    69 return;
  }
  70 node* stack[20];
  71 int childs[20];
  72 int top = -1;
  73 stack[++top] = root;
  74 childs[top] = 'R';
  75 while (top != -1) {
    76 node* temp = stack[top];
    77 printf("%c ", childs[top]);
    78 --top;
    79 for (int i = 25; i >= 0; --i) {
      80 if (temp->child[i] != NULL) {
        81 stack[++top] = temp->child[i];
        82 childs[top] = i + 'a';
        83
      }
      84
    }
    85
  }
  86 putchar('\n');
  87
}
88
89 void build_automation(node* root) /* build AC_automation(add fail pointer for every node) */
90 {
  91 if (root == NULL) {
    92 return;
  }
  93 node* queue[100];
  94 int head = 0;
  95 int tail = 0;
  96 root->fail = NULL; /* root's fail pointer is NULL*/
  97 queue[tail++ % 100] = root;
  98 while (head % 100 != tail % 100) {
    99 node* temp = queue[head];
    100 for (int i = 0; i < 26; ++i) {
      101 if (temp->child[i] != NULL) {
        102 node* p = temp->fail;
        103 while (p != NULL) { /* go on search, until p is NULL or found child[i]'s fail pointer */
          104 if (p->child[i] != NULL) { /* p also has child[i]? */
            105 temp->child[i]->fail = p->child[i];
            106 break;
            107
          }
          108 p = p->fail; /* if p has not child[i], go on search */
          109
        }
        110 if (p == NULL) {
          111 temp->child[i]->fail = root;
        }
        112 queue[tail++ % 100] = temp->child[i]; /* push child[i] into queue */
        113
      }
      114
    }
    115 ++head; /* pop temp */
    116
  }
  117
}
118
119 void AC(node* root, FILE* file) /* imaging the file has only letter characters, and has no '\n'*/
120 {
  121 int c;
  122 node* temp = root;
  123 while ((c = fgetc(file)) != EOF) {
    124 c = tolower(c);
    125 while (temp != NULL && c != EOF) {
      126 int i = c - 'a';
      127 if (temp->child[i] != NULL) {
        128 if (temp->child[i]->flag) {
          129 printf("Match\n");
          130
        }
        131 temp = temp->child[i];
        132 c = fgetc(file);
        133 c = tolower(c);
        134
      }
      135 else {
        136 temp = temp->fail;
        137 if (temp && temp->flag) {
          138 printf("Match\n");
        }
        139
      }
      140
    }
    141 if (temp == NULL) {
      142 temp = root; /* not match, make temp root to research */
    }
    143
  }
  144
}
145 void main(int argc, char* argv[])
146 {
  147 node* root = build_trie(modes, 10);
  148 BSF(root);
  149 DFS(root);
  150 build_automation(root);
  151 FILE* file = fopen("tt.txt", "r");
  152 AC(root, file);
  153
}
���ļ�tt.txtֻ���ڵ�һ����д���ݣ������л��з�����������ֻ���Ǵ�Сд��ĸ��
build_trie:
�����ֵ�����
BSF����ȱ�������� R h o s w i o r h w o p p a i o o r h l p p r d l s o d d h p i p
DSF����ȱ�������� R h i p h o p o p o r a l s h i p o p w o r d w o r d l d s h i p
��֤�ֵ���������ȷ��
bulid_automation�����ֵ����Ͻ���AC�Զ�����ʧ��ָ��
AC��AC�Զ���ƥ�䡣
ע�����������ַ���A��B����A��B�����ֹ�ϵ��A��B���Խ�ɫ��������
��B������ȫ������A��
��B��A��ǰ׺;
��B��A�ĺ�׺;
��B��A���м�һ���֣���A = ��*** B*** ��;
��A��Bͬʱ������ģʽ������ʱ���Ҵ����Ĵ�����A��
�ڢ٣���, ������£�AC�㷨����ȷƥ�����, ������Ӧ�������������ʾ����
�ڢ�����£�AC�㷨������ȷ��ʾ�����ǿ������ⲿ�жϣ���Ȼ���Aƥ����B�ض�ƥ�䣻
��һ�����ڢ٢ڢۣ�A = "ship", B = "hiphop", c = "hip", D = "hop"�����ABCD����ģʽ�����У�
    �������ABCD���ܼ�������
    ����һ��������ڢ٢ڢۢܣ�A = "shipx", B = "hipxhop", c = "hip", D = "hop", E = "x";
����������ܼ���ABCD��E������ȷ���������������ⲿ�жϡ�

