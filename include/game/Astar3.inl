//A_ A�� �㷨 C���� ʵ�ִ���
// ��Ȩ����������Ϊ����ԭ�����£�δ������������ת�ء�
//
// ����A*�㷨���������д��ʲô������ò��������æ����ʲô������ʵ��û����ʲô�����Ǹ��갡�����Խ�������дһ���ܽ�ɣ�
//
// A*�㷨�Ǻܾ����ֻ������ʽ�����㷨������ֻ�������㷨��һ��������㷨(����DFS��BFS֮��)�������������ϵ������Ҿ����á������ȫ���е�һ�仰�����ķǳ��ã�����ʻ�����ﵽĳ�˼ң�д���㷨�ǣ���167�Ÿ�����������Puyallup����XX���ں���ɽ�Ͽ�4.5Ӣ���һ���ӻ����Աߵĺ��̵���ת�������ڵ�һ��·����ת������ߵĺ�ɫ���ӵĳ�����ȥ���ǣ�������ʽ�ǣ��ҳ���һ�����Ǹ���ĵ��ţ��������ϵ�ַ����������򣬵�����������һ�����Ƿ�������������ÿһ���˶���ʶ���ǣ��϶�������Ը������㣬������Ҳ����ˣ�����һ���绰ͤ��绰�����ǣ����ǳ������㡱������ȥ�����е������ô�������Ǿ��忴��A*�㷨��
//
// ע�����ĵ�ͼ���Լ�һЩ˼�������http://www.policyalmanac.org/games/aStarTutorial.htm���ǳ���л�������£�
//
// ����Ҫ�����ɫ�ĵ㵽��ɫ�ĵ㣬��Ҫ����ʽ��ȥ��һ��·����������ôȥ���أ���ʼû�а취��ֻ�ܴӿ�ʼ���������ԣ�������Ҫ����һ��OPEN��OPEN���зŵ���ʲô�أ����ǵ�ǰ���ǵĵ㣬�����ܱߵĵ���Ҫ��ӽ�������Ϊ���ܵ�·���ϵĵ㡣����˵�����е������ô�ȿ����棺
//
// ���Ǵ���ʼ�㿪ʼ������
//
// 1���ӵ�A��ʼ�����Ұ�����Ϊ����������һ��OPEN�����·�����ܻ�ͨ���������ķ���Ҳ���ܲ��ᡣ�����ϣ�����һ������鷽����б�
// 2��Ѱ�������Χ(�ھ�)���пɵ�����߿�ͨ���ķ���������ϰ��﷽����ô�Ͳ���Ҫ�������ǣ���������������ڵ�Ҳ�����Ǽ���OPEN����Щ�ھӽڵ��ϵ�ǰ��Ϊ���ڵ㡣(���ڵ�ı����Ǳ���ģ���Ϊ�������ҵ����һ�����ʱ����Ҫͨ�����ڵ���ݣ��ҵ����е�ֱ����ʼ�ڵ㣡��ô����һ��������·����)
// 3���ӿ����б���ɾ����A���������뵽һ��CLOSE�б��б��б������в���Ҫ�ٴμ��ķ���
//
// ����һ�㣬��Ӧ���γ���ͼ�Ľṹ����ͼ�У�����ɫ����������ʼ��������ġ�������ǳ��ɫ��ߣ��Ա�ʾ�������뵽�ر��б����ˡ����е����ڸ����ڶ��ڿ����б��У����Ǳ���ǳ��ɫ��ߡ�ÿ��������һ����ɫָ�뷴ָ���ǵĸ�����Ҳ���ǿ�ʼ�ķ���
//
// ����������Ӧ��ѡ����һ������������أ�������Ӧ��֪����ν��������������νȨֵ֮��(�˴���ν��Ȩֵ����·���ĳ���)��YES��������ҪOPEN����ȨֵF��С���Ǹ��㣡Ϊʲô�أ���Ȼ��ȨֵԽС��Խ����Ŀ��㿩��
//
// ����Ȩֵ������ΪF����ôF��ô���㵽�ģ������������G��H��
//
// G = �����A�����Ų�����·�����ƶ���������ָ�������·���ķѡ�
// H = ���������Ǹ������ƶ����յ�B��Ԥ���ƶ��ķѡ��⾭������Ϊ����ʽ�ġ������е�ԭ������Ϊ��ֻ�Ǹ��²⡣����û�취����֪��·���ĳ��ȡ�(����������Ҫ֪������Ȼֻ�ǲ²⣬����ֻҪ�ǻ���һ��ͳһ�ı�׼�����Զ���������ǲ���ģ���һ���Ǻ���Ҫ�ģ� )
//
// ���磺Hֵ�Ĺ��Ʋ��á������١�����Ҳ���ǵ�ǰ�ĵ㣬��Ŀ��㣬���������ĸ�������ӣ�����Hֵ��
//
// ( ע�⣺�����ϰ��������ǲ������Ƿ����������⣡�����ϰ���Ҳ����������һ�����ӣ���Ҳ��Hֵ������Ԥ��Ķ��ѵ�ԭ�򣡼���ν����ʽ�� )
//
// ��ô���ڵ�һ��ͼ����ʼ�����յ��Hֵ�ǣ��������4�����ӣ��������0�����ӣ���ôH=4+0=4��
//
// ��ȻҲ�������İ취������ʹ��ֱ�߾��룬sqrt( pow( des.x - src.x ) + pow( des.y - src.y ) )Ҳ�ǿ��Ե�~
//
//
// ����Gֵ����������ӣ���ˮƽ���ߴ�ֱ�ƶ��ĺķ�Ϊ10���Խ��߷���ķ�Ϊ14������ȡ��Щֵ����Ϊ�ضԽ��ߵľ�������ˮƽ��ֱ�ƶ��ķѵĵĸ���2Լ1.414����Ϊ�˼򻯣�������10��14���ơ���ʱ����Ǻ���Ҫ��~
//
// ( ��ʵ����ֻҪ��Ӧ�˻����ı�����ϵ�Ϳ����ˣ� )
//
// ������ʼ���Լ���Χ���Gֵ��Hֵ��Fֵ����ͼ���Ժ������Ŀ�����( ���Ͻ���F�����½���G�����½���H )
//
// ����Gֵ��ֻҪ�Ǻ����������ھӣ���Ϊ+10��б����ھ�+14�Ϳ�����~������ĺ�easy��~�Ǻ�~
//
// ����Hֵ�����������Ӿ��ǿ�~
//
// F = G + H
//
// ע��������ھӽڵ㶼����OPEN����Ŷ~~~ ����OPEN��ɾ��������CLOSE��~
//
// ���ż��㣺
//
// Ȼ���OPEN����ѡ��һ��Fֵ��С��Ȼ��������Χ�ھӣ���ѭ������ֱ���յ����CLOSE�У�Ѱ·����������OPEN���ˣ�û�ҵ�·����
//
// ( ����������ôѡ����С���Ǹ����أ�������ʹ�ö�������ģ�����ѡ����Сֵ�Ǻܿ��~ )
//
// ���Կ�����F��С������ʼ���ұߵ��Ǹ��㣬�������ʾ��~
//
// Ȼ���ٿ������ھӣ�
// ��ʱ�������ھ��м��ֿ����ԣ�
// 1���ھ��Ѿ���CLOSE�����ˣ���ô����Ҫ������~
// 2���ھ����ϰ������Ҫ������e
// 3���ھӲ���OPEN���У���ô���ھӼ���OPEN���������ھӵĸ��ڵ㸳ֵΪ��ǰ�ڵ�
// 4���ھ���OPEN�У���ô��Ҫ�������ھӵ��Gֵ�뵱ǰ���(Gֵ+��ǰ�㵽�ھӵ�ľ���(10����14))�Ĵ�С������Ӵ˵��߸���(��Gֵ��С)����ô���˵�ĸ��ڵ㻻�ɵ�ǰ�㣡(ע�⣺����ͬһ���㣬Gֵ�£���ôF����С����ΪH����ͬ�ģ�)
//
// �����ǽ�һ����ͼʾ��
// ��ô���ǰ��������˼·��֪���յ����CLOSE���� �ռ�ͼʾ���� ��
// �������ǿ��Եõ�·����( ֮ǰ����˵�����ɸ��ڵ���ǰ���ݾͺ�easy�ĵõ�·����~ )
// ˵����ô�࣬Ҳ��֪��˵���в��У�������Ҫ�ܽ�һ�£�
// �ܽ᣺
// 1������ʼ�����OPEN����
// 2��ѭ��ֱ��OPENΪ�ջ����յ����CLOSE����
// �����ҵ�OPEN����Fֵ��С�Ľڵ�(ʹ�ö�����õ�Сֵ��)�����˵��OPENɾ��������CLOSE��
// (��ʱ��С���Ѿ�ȡ������ô��Ҫ��������OPEN���ǵĵ�һ��������СF�ĵ㣡)
// ��8���ھӽ��д���
// ����1���ھ��Ѿ���CLOSE�����ˣ���ô����Ҫ������~
// 2���ھ����ϰ������Ҫ������e
// 3���ھӲ���OPEN���У���ô���ھӼ���OPEN���������ھӵĸ��ڵ㸳ֵΪ��ǰ�ڵ�
// 4���ھ���OPEN�У���ô��Ҫ�������ھӵ��Gֵ�뵱ǰ���(Gֵ+��ǰ�㵽�ھӵ�ľ�
// �� (10����14))�Ĵ�С������Ӵ˵��߸���(��Gֵ��С)����ô���˵�ĸ��ڵ㻻�ɵ�ǰ
// �㣡 (ע�⣺����ͬһ���㣬Gֵ�£���ôF����С����ΪH����ͬ�ģ�)
// ע�⣺�����ڵ�ı��OPEN���е���С����ܻ�仯����ô��Ҫ�ٴ�����õ���С�㣡
// 3�������������˳�ѭ������������֪��������û���ҵ�·������������Ĺ�����easy��~
// ������ԭ�����������~
// �����һ���򵥵�C���Ե���ʾ���룬ֻ��Ϊ����ʾԭ��û��ע���������⣬���Դ��Ī�ְ�~
// ע�⣺������1������㣬2�����յ㣬0�������ͨ����3�����ϰ���
#include <stdio.h>
#include <stdlib.h>
#define STARTNODE 1
#define ENDNODE 2
#define BARRIER 3
typedef struct AStarNode {
  int s_x; // ����(�������·����Ҫ)
  int s_y;
  int s_g; // ��㵽�˵�ľ���( ��g��h���Եõ�f���˴�fʡ�ԣ�f=g+h )
  int s_h; // ��������Ԥ��Ĵ˵㵽�յ�ľ���
  int s_style;// ������ͣ���ʼ�㣬�յ㣬�ϰ���
  struct AStarNode* s_parent; // ���ڵ�
  int s_is_in_closetable; // �Ƿ���close����
  int s_is_in_opentable; // �Ƿ���open����
} AStarNode;
AStarNode map_maze[10][10]; // �������
AStarNode* open_table[100]; // open��
AStarNode* close_table[100]; // close��
int open_node_count; // open���нڵ�����
int close_node_count; // close���н������
AStarNode* path_stack[100]; // ����·����ջ
int top = -1; // ջ��
// ��������Ԫ��
//
void swap(int idx1, int idx2)
{
  AStarNode* tmp = open_table[idx1];
  open_table[idx1] = open_table[idx2];
  open_table[idx2] = tmp;
}
// �ѵ���
void adjust_heap(int nIndex)
{
  int curr = nIndex;
  int child = curr * 2 + 1; // �õ�����idx( �±��0��ʼ��������������curr*2+1 )
  int parent = (curr - 1) / 2; // �õ�˫��idx
  if (nIndex < 0 || nIndex >= open_node_count) {
    return;
  }
  // ���µ���( Ҫ�Ƚ����Һ��Ӻ�cuur parent )
  while (child < open_node_count) {
    // С������˫��ֵС�ں���ֵ
    if (child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h > open_table[child + 1]->s_g + open_table[child + 1]->s_h) {
      ++child; // �ж����Һ��Ӵ�С
    }
    if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h) {
      break;
    }
    else {
      swap(child, curr); // �����ڵ�
      curr = child; // ���жϵ�ǰ���ӽڵ�
      child = curr * 2 + 1; // ���ж�����
    }
  }
  if (curr != nIndex) {
    return;
  }
  // ���ϵ���( ֻ��Ҫ�Ƚ�cuur child��parent )
  while (curr != 0) {
    if (open_table[curr]->s_g + open_table[curr]->s_h >= open_table[parent]->s_g + open_table[parent]->s_h) {
      break;
    }
    else {
      swap(curr, parent);
      curr = parent;
      parent = (curr - 1) / 2;
    }
  }
}
// �ж��ھӵ��Ƿ���Խ���open��
void insert_to_opentable(int x, int y, AStarNode* curr_node, AStarNode* end_node, int w)
{
  int i;
  if (map_maze[x][y].s_style != BARRIER) { // �����ϰ���
    if (!map_maze[x][y].s_is_in_closetable) { // ���ڱձ���
      if (map_maze[x][y].s_is_in_opentable) { // ��open����
        // ��Ҫ�ж��Ƿ���һ�����Ż���·��
        if (map_maze[x][y].s_g > curr_node->s_g + w) { // ������Ż�
          map_maze[x][y].s_g = curr_node->s_g + w;
          map_maze[x][y].s_parent = curr_node;
          for (i = 0; i < open_node_count; ++i) {
            if (open_table[i]->s_x == map_maze[x][y].s_x && open_table[i]->s_y == map_maze[x][y].s_y) {
              break;
            }
          }
          adjust_heap(i); // ���������
        }
      }
      else { // ����open��
        map_maze[x][y].s_g = curr_node->s_g + w;
        map_maze[x][y].s_h = abs(end_node->s_x - x) + abs(end_node->s_y - y);
        map_maze[x][y].s_parent = curr_node;
        map_maze[x][y].s_is_in_opentable = 1;
        open_table[open_node_count++] = &(map_maze[x][y]);
      }
    }
  }
}
// �����ھ�
// ����������8���ھӽ��в���
void get_neighbors(AStarNode* curr_node, AStarNode* end_node)
{
  int x = curr_node->s_x;
  int y = curr_node->s_y;
  // �������8���ھӽ��д���
  if ((x + 1) >= 0 && (x + 1) < 10 && y >= 0 && y < 10) {
    insert_to_opentable(x + 1, y, curr_node, end_node, 10);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && y >= 0 && y < 10) {
    insert_to_opentable(x - 1, y, curr_node, end_node, 10);
  }
  if (x >= 0 && x < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x, y + 1, curr_node, end_node, 10);
  }
  if (x >= 0 && x < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x, y - 1, curr_node, end_node, 10);
  }
  if ((x + 1) >= 0 && (x + 1) < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x + 1, y + 1, curr_node, end_node, 14);
  }
  if ((x + 1) >= 0 && (x + 1) < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x + 1, y - 1, curr_node, end_node, 14);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x - 1, y + 1, curr_node, end_node, 14);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x - 1, y - 1, curr_node, end_node, 14);
  }
}
int test_astar3()
{
  // ��ͼ����Ķ���
  //
  AStarNode* start_node; // ��ʼ��
  AStarNode* end_node; // ������
  AStarNode* curr_node; // ��ǰ��
  int is_found; // �Ƿ��ҵ�·��
  int maze[][10] = { // ����Ϊ�˺ø�ֵ��map_maze
    { 1, 0, 0, 3, 0, 3, 0, 0, 0, 0 },
    { 0, 0, 3, 0, 0, 3, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 3, 0, 3 },
    { 3, 0, 3, 0, 0, 0, 0, 0, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 0, 0, 0, 3 },
    { 3, 0, 0, 3, 0, 0, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 0, 0, 0 },
    { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 0, 0, 3, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 3, 0, 3 },
  };
  int i, j, x;
  // ����׼����
  //
  for (i = 0; i < 10; ++i) {
    for (j = 0; j < 10; ++j) {
      map_maze[i][j].s_g = 0;
      map_maze[i][j].s_h = 0;
      map_maze[i][j].s_is_in_closetable = 0;
      map_maze[i][j].s_is_in_opentable = 0;
      map_maze[i][j].s_style = maze[i][j];
      map_maze[i][j].s_x = i;
      map_maze[i][j].s_y = j;
      map_maze[i][j].s_parent = NULL;
      if (map_maze[i][j].s_style == STARTNODE) { // ���
        start_node = &(map_maze[i][j]);
      }
      else if (map_maze[i][j].s_style == ENDNODE) { // �յ�
        end_node = &(map_maze[i][j]);
      }
      printf("%d ", maze[i][j]);
    }
    printf("\n");
  }
  // ����ʹ��A*�㷨�õ�·��
  //
  open_table[open_node_count++] = start_node; // ��ʼ�����open��
  start_node->s_is_in_opentable = 1; // ����open��
  start_node->s_g = 0;
  start_node->s_h = abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y);
  start_node->s_parent = NULL;
  if (start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y) {
    printf("���==�յ㣡\n");
    return 0;
  }
  is_found = 0;
  while (1) {
    // for test
    if (0) {
      for (x = 0; x < open_node_count; ++x) {
        printf("(%d,%d):%d ", open_table[x]->s_x, open_table[x]->s_y, open_table[x]->s_g + open_table[x]->s_h);
      }
      printf("\n\n");
    }
    curr_node = open_table[0]; // open��ĵ�һ����һ����fֵ��С�ĵ�(ͨ��������õ���)
    open_table[0] = open_table[--open_node_count]; // ���һ����ŵ���һ���㣬Ȼ����жѵ���
    adjust_heap(0); // ������
    close_table[close_node_count++] = curr_node; // ��ǰ�����close��
    curr_node->s_is_in_closetable = 1; // �Ѿ���close������
    if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y) { // �յ���close�У�����
      is_found = 1;
      break;
    }
    get_neighbors(curr_node, end_node); // ���ھӵĴ���
    if (open_node_count == 0) { // û��·������
      is_found = 0;
      break;
    }
  }
  if (is_found) {
    curr_node = end_node;
    while (curr_node) {
      path_stack[++top] = curr_node;
      curr_node = curr_node->s_parent;
    }
    while (top >= 0) { // ���������·������~
      if (top > 0) {
        printf("(%d,%d)-->", path_stack[top]->s_x, path_stack[top--]->s_y);
      }
      else {
        printf("(%d,%d)", path_stack[top]->s_x, path_stack[top--]->s_y);
      }
    }
  }
  else {
    printf("ô���ҵ�·��");
  }
  puts("");
  return 0;
}

