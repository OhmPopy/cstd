
#include <string.h>

#define NUM_CHARS 256   // number of characters in the character set
#define MAX_SIZE 100000   // maximal buffer size for input from file

#ifndef TRUE
#define TRUE 1
#endif // TRUE
#ifndef FALSE
#define FALSE 0
#endif // FALSE

//Huffman ����
typedef struct HuffCode {
  int used;
  unsigned char bits[2];
  unsigned int size;
} HuffCode;

//Huffman Tree �ڵ�
typedef struct HuffNode {
  char c;             // for leaves, the corresponding character from the character set
  unsigned int freq;  // frequency of the node (for non-leaves sum of frequencies of child nodes)
  struct HuffNode *left, *right;    // ���ӽ��
} HuffNode;


// This class is used in the construction of the Huffman tree.
// ���ȼ�����
  HuffNode g_huffNode[NUM_CHARS*2];
  HuffNode* g_HuffNodes[NUM_CHARS*2];
  unsigned int g_size;
  int g_Nodeused;

  HuffNode* new_HuffNode() {
    return g_huffNode + g_Nodeused++;
  }
  void init() {
    g_size=0;
    g_Nodeused=0;
  }

  void heapify(int i)
  {
    int l,r,smallest;
    HuffNode* tmp;
    
    l=2*i; /*left child*/
    r=2*i+1; /*right child*/
    
    if ((l < g_size)&&(g_HuffNodes[l]->freq < g_HuffNodes[i]->freq))
      smallest=l;
    else
      smallest=i;
    if ((r < g_size)&&(g_HuffNodes[r]->freq < g_HuffNodes[smallest]->freq))
      smallest=r;
    
    if (smallest!=i) 
    {
      /*exchange to maintain heap property*/
      tmp=g_HuffNodes[smallest];
      g_HuffNodes[smallest]=g_HuffNodes[i];
      g_HuffNodes[i]=tmp;
      heapify(smallest);
    }
  }

  void addItem(HuffNode* node) 
  {
    unsigned int i,parent;  
    g_size = g_size+1;
    i = g_size-1;
    parent = i/2;

    /*find the correct place to insert*/
    while ( (i > 0) && (g_HuffNodes[parent]->freq > node->freq) )
    {
      g_HuffNodes[i] = g_HuffNodes[parent];
      i = parent;
      parent = i/2;
    }
    g_HuffNodes[i]=node;
  }
  
  int isEmpty(void) {
    return g_size==0;
  }
  
  int isFull(void) {
    return g_size >= NUM_CHARS;
  }

  HuffNode* extractMin(void) 
  {
    HuffNode* max;
    if (isEmpty())
      return 0;
    max=g_HuffNodes[0];
    g_HuffNodes[0]=g_HuffNodes[g_size-1];
    g_size=g_size-1;
    heapify(0);
    return max;
  }


// set the bit at position pos in the array bits to the value state
void set_bit(unsigned char* bits, unsigned int pos, unsigned int state)
{
  unsigned int i;
  unsigned char mask = 0x80;  // = 128 dec = 10000000 bin
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;  // shift bitmask to right
  
  if (state)
    bits[pos/8] = bits[pos/8] | mask;
  else
    bits[pos/8] = bits[pos/8] & (~mask);
  
  return;
}

// get the state of the bit at position pos in the array bits
unsigned int get_bit(unsigned char* bits, unsigned int pos) {
  unsigned int i;
  unsigned char mask = 0x80;  // = 128 dec = 10000000 bin
  for (i = 0; i < (pos % 8); i++)
    mask = mask >> 1;  // shift bitmask to right
  
  return (((mask & bits[(int)(pos/8)]) == mask) ? 1 : 0);
}


// create the Huffman tree from the array of frequencies
// returns a pointer to the root node of the Huffman tree
// �����ַ�Ƶ�����飬����һ��huffman�������ظ��ڵ㡣
HuffNode* build_Huffman_tree(unsigned int freqs[NUM_CHARS])
{
  unsigned int i;
  // create priority queue
  //HuffNodePriorityQueue priority_queue;
  init();

  for (i = 0; i < NUM_CHARS; i++)
  {
    if (freqs[i] > 0)
    {
      HuffNode* node = new_HuffNode();
      node->c = i;
      node->freq = freqs[i];
      node->left = NULL;
      node->right = NULL;
      addItem(node);
    }
  }

  //printf("number of characters: %d\n", size);

  // create the Huffman tree
  while (g_size > 1)
  {
    HuffNode* left = extractMin();
    HuffNode* right = extractMin();

    HuffNode* root = new_HuffNode();
    root->freq = left->freq + right->freq;
    root->left = left;
    root->right = right;
    addItem(root);
  }

  // return pointer to the root of the Huffman tree
  return extractMin();
}


// recursively free memory of the Huffman tree
// �ݹ麯�����ͷ�huffman�������нڵ㣩ռ�õ��ڴ�

void delete_Huffman_tree(HuffNode* root)
{
  HuffNode* node = root;

  // base case at leaf
  if (node->left == NULL && node->right == NULL)
  {
    //delete node;
    return;
  }

  if (node->left)
    delete_Huffman_tree(node->left);
  if (node->right)
    delete_Huffman_tree(node->right);

  //delete node;
  return;
}



//�ݹ麯����������ȱ���huffman��������ÿ��Ҷ�ӽڵ�ı������õ���Ӧ�� HuffCode ������Ӧ����
//code: ׷�ټ�¼��ǰ��HuffCode������Ҷ�ӽڵ��·����
//pos: code�ĵ�ǰ����

void create_table_rec(HuffCode table[NUM_CHARS], HuffNode* node, unsigned char* code, unsigned int pos) {
  if(node->left) {
    set_bit(code, pos, 0);
    create_table_rec(table, node->left, code, pos+1);
  }
  
  if(node->right) {
    set_bit(code, pos, 1);
    create_table_rec(table, node->right, code, pos+1);
  }
  
  //����Ҷ�ӽڵ㣿��洢��ǰ�ڵ��HuffCode
  if(node->left == 0 && node->right == 0) {
    table[node->c].used = TRUE;
    table[node->c].bits[0] = code[0];
    table[node->c].bits[1] = code[1];
    table[node->c].size = pos;
  }
}

HuffNode* ntable[NUM_CHARS];
unsigned char npos[NUM_CHARS];
unsigned char ncode0[NUM_CHARS];
unsigned char ncode1[NUM_CHARS];
int create_table_rec1(HuffCode table[NUM_CHARS], HuffNode* node, unsigned char* code, unsigned int pos) {
  int nlen=0;
  ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos, ntable[nlen++]=node;
  while(nlen>0 && nlen<NUM_CHARS) {
    code[0] = ncode0[--nlen], code[1] = ncode1[nlen], pos = npos[nlen], node = ntable[nlen];
    if(node->left) {
      set_bit(code, pos, 0);
      //create_table_rec(table, node->left, code, pos+1);
      ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos+1, ntable[nlen++]=node->left;
    }
    
    if(node->right) {
      set_bit(code, pos, 1);
      //create_table_rec(table, node->right, code, pos+1);
      ncode0[nlen] = code[0], ncode1[nlen] = code[1], npos[nlen]=pos+1, ntable[nlen++]=node->right;
    }
    
    //����Ҷ�ӽڵ㣿��洢��ǰ�ڵ��HuffCode
    if(node->left == 0 && node->right == 0) {
      table[node->c].used = TRUE;
      table[node->c].bits[0] = code[0];
      table[node->c].bits[1] = code[1];
      table[node->c].size = pos;
    }
  }
  return 0;
}


// create table of Huffman codes
// calls create_table_rec to do the actual work
int create_table(HuffCode table[NUM_CHARS], HuffNode* tree) {
  unsigned int i;
  unsigned char code[2];
  code[0]=0, code[1]=0;
  // initialize table data so that there are no undefined values
  for (i = 0; i < NUM_CHARS; i++) {
    table[i].used = FALSE;
    table[i].size = 0;
    table[i].bits[0] = 0;
    table[i].bits[1] = 0;
  }

  //code�б���ڵ��huffman���롣
  create_table_rec(table, tree, code, 0);
  //for (i = 0; i < NUM_CHARS; i++) {
  //  printf("%d, %d, %d, %d,\n", table[i].used, table[i].size, table[i].bits[0], table[i].bits[1]);
  //}
  return 0;
}

// �����ļ�ͷ
void create_header(unsigned int freqs[NUM_CHARS], HuffCode table[NUM_CHARS], unsigned char* header, unsigned int size)
{
  unsigned int i;
  // store the size of the string at the beginning of the header
  memcpy(header, &size, sizeof(int));

  // store the scaled frequencies for all symbols in the character set
  for (i = 0; i < NUM_CHARS; i++)
    header[sizeof(int) + i] = (unsigned char)freqs[i];

  return;
}

//����һ���ַ��������ַ��ĳ���Ƶ�ʱ��浽freqs������
//Hint: Be carefull that you don��t scale any frequencies to zero for symbols that do appear in the string!
void create_freq_array(unsigned int freqs[NUM_CHARS], unsigned char* string, unsigned int size)
{
  int i, maxfreq = 0;
  
  //��ʼ����0
  memset(freqs, 0, sizeof(unsigned int) * NUM_CHARS);
  
  for(i=0; i<size; i++)
  {
    freqs[string[i]]++;
    
    if(freqs[string[i]] > maxfreq)
      maxfreq = freqs[string[i]];
  }
  
  //���ַ�Ƶ��ѹ����һ���ֽڡ� scaled freqs to (0~255)
  if(maxfreq > 0xff) {
    for(i=0; i<NUM_CHARS; i++) {
      if(freqs[i]) {
        freqs[i] = (int)(freqs[i] * 255.0 / maxfreq + 0.5);
        
        //Ҫȷ�����ᱻ��С��0��
        if(freqs[i] == 0) {
          freqs[i] = 1;
        }
      }
    }
  }
}

//��һ��ָ�����ȵ��ַ�������Huffman����
//compressed: ������
//string: ������ַ���
//size: �ַ�������
//���� compressed ���ֽڳ��ȡ�
unsigned int compress(HuffCode table[NUM_CHARS], unsigned char* compressed, unsigned char* string, unsigned int size)
{
  unsigned int i, j, pos = 0;
  unsigned char c;
  
  for(i = 0; i < size; i++) {
    c=string[i];
    for(j = 0; j < table[c].size; j++) {
      set_bit(compressed, pos+j, get_bit(table[c].bits, j));
    }
    pos += table[c].size;
  }
  
  //bits = (pos+1)
  return (pos/8 + 1);
}

//huffman���룺
//root: �Ѿ��ؽ��õ�Huffman���ĸ��ڵ�
//comprerssed: huffman������bits
//string: �������ַ���
//size: ԭʼ�ַ���������ǰ�������ַ��������ȣ����Ǵ�header�ж�ȡ��
void uncompress(HuffNode* root, unsigned char* compressed, unsigned char* string, unsigned int size)
{
  unsigned int length = 0;   //�Ѿ�������ַ���
  unsigned int pos = 0;     //��¼��bits�е�λ��
  unsigned int bit;
  HuffNode *node = NULL; //��ǰ�ڵ�
  
  while(length < size)
  {
    //�õ����ڵ�
    node = root; 
    
    //Ѱ��Ҷ�ӽڵ�
    while(node->left != 0 || node->right != 0)
    {
      bit = get_bit(compressed, pos++);
    
      if(bit) node = node->right;  //goto right child
      else node = node->left;     //goto left child
    }
    
    string[length++] = node->c;    
  }
}

#define HSIZE (sizeof(int) + NUM_CHARS)

HuffCode table[NUM_CHARS];
unsigned char header[HSIZE];
unsigned int freqs[NUM_CHARS];
// compress the size-many symbols in string into *compressed
// returns the size of the compressed data (in bytes)
unsigned int HUFF_Encode(unsigned char* string, unsigned int size, unsigned char* compressed)
{
  HuffNode* huff_tree;
  unsigned char* comp = compressed+HSIZE;
  unsigned int codesize;
  // create array of frequencies for all ascii characters
  create_freq_array(freqs, string, size);

  //��ӡ�ַ�Ƶ��
  //print_freqs(freqs);

  // create Huffman tree
  huff_tree = build_Huffman_tree(freqs);

  // create symbol table
  create_table(table, huff_tree);

  //print_table(table);

  //create header
  //unsigned int HSIZE = sizeof(int) + NUM_CHARS;  // size of the header
  create_header(freqs, table, header, size);
  
  // ��Header �� ������ �ϲ���һ��(compressed_tmp)��
  memcpy(compressed, header, HSIZE);

  // create compressed text
  codesize = compress(table, comp, string, size);

  //printf("compressed string: (size: %d bit)\n", 8*codesize);
  //for (unsigned int i = 0; i < 8*codesize; i++)
  //  printf("%d", get_bit(comp, i));
  //printf("\n");

  // free Huffman tree
  delete_Huffman_tree(huff_tree);

  return HSIZE + codesize;
}

// uncompress the data in compressed into *string
// returns the size of the uncompressed text (in bytes)
unsigned int HUFF_Decode(unsigned char* compressed, unsigned int size0, unsigned char* str)
{
  unsigned int size = 0;  // size of the string
  unsigned int freqs[NUM_CHARS];  // frequencies of the ascii characters
  //unsigned int HSIZE = sizeof(int) + NUM_CHARS;  // size of the header
  unsigned int i;
  HuffNode* huff_tree;

  // get number of symbols in compressed
  // ��ȡԭʼ�ַ����ĳ��ȣ��洢��ǰ4���ֽ��У�
  memcpy(&size, compressed, sizeof(int));

  //printf("size of compressed string: %d\n", size);

  // restore frequency table
  for (i = 0; i < NUM_CHARS; i++)
    freqs[i] = compressed[sizeof(int) + i];


  // create Huffman tree
  huff_tree = build_Huffman_tree(freqs);

  /*
  HuffCode table[NUM_CHARS];
  create_table(table, huff_tree);
  print_table(table);
  */

  // uncompress the data
  uncompress(huff_tree, &compressed[HSIZE], str, size);

  // free Huffman tree
  delete_Huffman_tree(huff_tree);

  //printf("uncompressed string: (size: %d bit)\n", 8*sizeof(char)*size);
  //for(i = 0; i < size; i++)
  //  printf("%c", (*string)[i]);
  //printf("\n");

  return size;
}

