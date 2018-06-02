//�Զ������Թ�
//�����Թ���ʼ��ȫ��Ϊǽ
//Ȼ�����ѡ����м�һ���㿪ʼ��
//��ʼ�ݹ飬���ѡ�������ƶ��������ǽ�����Ҳ���������·��ͨ���Ͱ�ǽ���ó�·��
//ʹ��������ȵķ��������µĵ�����ݹ飬�����Χȫ���޷���ͨ������˵��ϴνڵ㣬ѡ����������
//���һֱ�ݹ飬ֱ�����еĵ㶼̽���ꡣ���յ�Ч��ͼ���£�
//�����о��±��˵��㷨�����ȼ����ͼ���������ĵ㣬Ȼ����Щ����д�ͨ��
//ֻҪ������ǹ����ģ��Ϳ�������������ͨ���������㷨����ÿ�Щ�������ϻ�򵥺ܶ࣬
//���ɵ�ͼ��Ҳû��һ������С�顣
#include "cstd.h"
// �ݹ�汾��ջ�������
//���ѡ����м�һ���㿪ʼ�ݹ飬
//���ѡ�������ƶ��������ǽ�����Ҳ���������·��ͨ���Ͱ�ǽ���ó�·��
//ʹ��������ȵķ��������µĵ�����ݹ飬�����Χȫ���޷���ͨ������˵��ϴνڵ㣬ѡ����������
//���һֱ�ݹ飬ֱ�����еĵ㶼̽���ꡣ
int maze_search_path(uint* pseed, char* zmap, int* dir)
{
  int next, turn, i;
  *zmap = 0;
  turn = myrand32(pseed) % 2 ? 1 : 3;
  for (i = 0, next = myrand32(pseed) % 4; i < 4; ++i, next = (next + turn) % 4) {
    if (zmap[2 * dir[next]] == 0) {
      zmap[dir[next]] = 1;
      maze_search_path(pseed, zmap + 2 * dir[next], dir);
    }
  }
  return 0;
}
#define PUSH(P) (dts[ptslen++]=P, zmap+=2*dir[P])
#define POP() (zmap-=2*dir[dts[--ptslen]])
// �ǵݹ�汾��û��С����
int maze_search_path2(uint* pseed, char* zmap, const int* dir, char* dts)
{
  int ptslen = 0;
  PUSH(4);
  for (; ptslen > 0;) {
    int next, turn, i, candel = ptslen;
    //maze_print("asdf.txt", 11, 11, map, 11);
    *zmap = 1;
    turn = myrand32(pseed) % 2 ? 1 : 3;
    for (i = 0, next = myrand32(pseed) % 4; i < 4; ++i, next = (next + turn) % 4) {
      if (zmap[2 * dir[next]] == 0) {
        zmap[dir[next]] = 1;
        PUSH(next);
        break;
      }
    }
    if (candel == ptslen) {
      POP();
    }
  }
  return 0;
}
#undef PUSH
#undef POP
#define PUSH(P, pp) (nodes[ptslen].zmap=zmap+dir[P], nodes[ptslen++].parent = pp, ASSERT(ptslen<len))
#define POP() (zmap=nodes[--ptslen].zmap)
typedef struct mazenode_t_ mazenode_t;
struct mazenode_t_ {
  mazenode_t* parent;
  char* zmap;
};
char* g_map;
int g_h, g_w;
mazenode_t* maze_search_path3(char* zmap, char* end, const int* dir, char* dts, int len)
{
  int ptslen = 0;
  mazenode_t* nodes = (mazenode_t*)dts;
  mazenode_t* parent = NULL;
  len /= sizeof(mazenode_t);
  PUSH(4, NULL);
  for (; ptslen > 0;) {
    int i, candel = ptslen;
    mazenode_t* parent = nodes + ptslen - 1;
    zmap = nodes[ptslen - 1].zmap;
    *zmap = 2;
    if (end == zmap) {
      return parent;
    }
    for (i = 0; i < 4; ++i) {
      if (1 == zmap[dir[i]]) {
        PUSH(i, parent);
      }
    }
    if (candel == ptslen) {
      POP();
    }
  }
  return NULL;
}
#undef PUSH
#undef POP
//�߽���v
int maze_fill_bord(int h, int w, char* map, int step, int v)
{
  int i, j;
  for (i = 0, j = w - 1; i < h; ++i) {
    map[i * step + 0] = v;
    map[i * step + j] = v;
  }
  for (i = 0, j = h - 1; i < w; ++i) {
    map[0 * step + i] = v;
    map[j * step + i] = v;
  }
  return 0;
}
int maze_rand(uint* pseed, int h, int w, char* map, int step)
{
  //�����Թ���ʼ��ȫ��Ϊǽ
  int i, j, dir[5] = {1, step, -1, -step, 0};
  char* dts = NULL;
  //assert((h & 1) && (w & 1));
  assert((h > 3) && (w > 3));
  h = (h - 1) | 1;
  w = (w - 1) | 1;
  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      map[i * step + j] = 0;
    }
  }
  //�߽���0������Խ���ж�
  maze_fill_bord(h, w, map, step, 1);
  map[2 * step + 1] = 1;
  map[(h - 3)*step + w - 2] = 1;
  //maze_print("asdf.txt", h, w, map, step);
  srand((unsigned)time(NULL));
  i = myrand32(pseed) % ((h - 3) / 2) + 1;
  j = myrand32(pseed) % ((w - 3) / 2) + 1;
  dts = (char*)malloc(h * w * 4);
  maze_search_path2(pseed, map + (i * step + j) * 2, dir, dts);
  maze_fill_bord(h, w, map, step, 0);
  {
    mazenode_t* p = maze_search_path3(map + 2 * step + 1, map + (h - 3) * step + w - 2, dir, dts, h * w * 4);
    for (i = 0; i < h; ++i) {
      for (j = 0; j < w; ++j) {
        map[i * step + j] = !!map[i * step + j];
      }
    }
    for (; p; p = p->parent) {
      *p->zmap = 2;
    }
  }
  free(dts);
  //maze_search_path(2, 1);
  return 0;
}
int maze_print(const char* fn, int h, int w, char* map, int step)
{
  int i, j;
  FILE* pf = fn ? fopen(fn, "wb") : stdout;
  char* aa[] = {"��", "��", "��", "��"};
  if (fn) {
    for (i = 1; i < h - 1; i++) {
      for (j = 1; j < w - 1; j++) {
        fputs(aa[map[i * step + j] & 3], pf);
      }
      fputs("\r\n", pf);
    }
    fclose(pf);
  }
  return 0;
}

