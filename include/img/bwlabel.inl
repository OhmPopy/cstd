
// minareas �������� 4����8
// minareas ��С��� ȥ��С��minareas�Ŀ�
int bwlabel(int h, int w, const unsigned char* A, int al, int* tmp, int bl, int neighbours,
    int* areas, int areanum, int minareas, const unsigned char* map, unsigned char* C, int cl)
{
  int* queue;
  int m = h, n = w, i, j, k;
  const unsigned char* s = A;
  int label = 1;
  int neighbour[8];
  int neighbour8[][2] = { -1, 0, 0, -1, 0, 1, 1, 0, -1, -1, -1, 1, 1, -1, 1, 1}; //�͵�ǰ����������ӵõ��˸���������
  ASSERT(neighbours <= 8);
  queue = (int*)pmalloc(h * w * sizeof(int));

  for (k = 0; k < neighbours; ++k) {
    neighbour[k] = neighbour8[k][0] * al + neighbour8[k][1];
  }

  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j) {
      tmp[i * bl + j] = 0;
    }
  }

  for (i = 1; i < m - 1; ++i) {
    for (j = 1; j < n - 1; ++j) {
      if (s[i * al + j] && tmp[i * bl + j] == 0) {
        int* queue_head = queue;     //����ͷ
        int* queue_tail = queue;     //����β
        tmp[i * bl + j] = label;
        *queue_tail++ = i * al + j;  //��Ԫ��ģ����У���ǰ��������

        while (queue_head != queue_tail) {
          int pix = *--queue_tail;

          for (k = 0; k < neighbours; ++k) {          //n��������
            int pix1 = pix + neighbour[k];

            if (pix1 >= 0 && pix1 < h * w) {
              int pix2 = (pix1 / al) * bl + (pix1 % al);

              if (s[pix1] && tmp[pix2] == 0) { //�����ǰ������������Ϊ1���ұ��ͼ��������������û�б���ǣ���ô���
                tmp[pix2] = label;
                *queue_tail++ = pix1;
                ASSERT(queue_tail - queue_head < h * w);
              }
            }
          }
        }

        label = label + 1;
      }
    }
  }

  if (areas) {
    for (i = 0; i < areanum; ++i) {
      areas[i] = 0;
    }

    for (i = 0; i < m; ++i) {
      for (j = 0; j < n; ++j) {
        int t = tmp[i * bl + j];

        if (t < areanum) {
          areas[t]++;
        }
      }
    }

    if (minareas > 0) {
      k = 1;

      for (i = 0; i < label + 1; ++i) {
        queue[i] = 0;
      }

      for (i = 1; i < areanum; ++i) {
        queue[i] = 0;

        if (areas[i] > minareas) {
          queue[i] = k++;
        }
      }

      for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
          int t = tmp[i * bl + j];
          tmp[i * bl + j] = queue[t];
          ASSERT(tmp[i * bl + j] < 300);
        }
      }
    }

    if (map) {
      for (i = 0; i < m; ++i) {
        for (j = 0; j < n; ++j) {
          int t = tmp[i * bl + j];
          C[i * cl + j] = map[t];
        }
      }
    }
  }

  pfree(queue);
  return label;
}
