#include <stdio.h>
//���������end��ʾ֮ǰ��Ԫ�ض��ҵ���ƥ�䣬��endԪ��֮���Ԫ�ض�δ��ƥ��
int inter_i23(int size1, int* array1, int size2, int* array2)
{
  int end = size1;
  int i, j;
  bool swap = false;//��ʶ��������ʾ��������е���һ��
  for (i = 0 ; i < end;) {
    swap = false;//��ʼ�����ǵ�һ�����
    for (j = i ; j < size2; j++) { //�ҵ����Ԫ�ش�����ͬ��Ԫ�أ��������ͬ��Ԫ�ؽ��������Ԫ����ͬ�±��λ����
      if (array1[i] == array2[j]) { //�ڶ���������ҵ�����ȵ�Ԫ��
        int tmp = array2[i];//������2���н���
        array2[i] = array2[j];
        array2[j] = tmp;
        swap = true;//���ñ�־
        break;
      }
    }
    if (swap != true) { //��һ�������û����ͬԪ�ش���ʱ�������Ԫ�ؽ�������δ���бȽϵ�β��
      int tmp = array1[i];
      array1[i] = array1[--end];
      array1[end] = tmp;
    }
    else {
      i++;
    }
  }
  return end;
}
int test_inter()
{
  int i;
  //������������
  int array1[] = {7, 1, 2, 5, 4, 3, 5, 6, 3, 4, 5, 6, 7, 3, 2, 5, 6, 6};
  int size1 = 18;
  int array2[] = {8, 2, 1, 3, 4, 5, 3, 2, 4, 5, 3, 2, 1, 3, 5, 4, 6, 9};
  int size2 = 18;
  int end = inter_i23(size1, array1, size2, array2);
  //������
  printf("only in array1\n");
  for (i = end ; i < size1; i++) {
    printf("%d ", array1[i]);
  }
  printf("\n");
  printf("only in array2\n");
  for (i = end ; i < size2; i++) {
    printf("%d ", array2[i]);
  }
  printf("\n");
  //�������
  printf("elements in array1 and array2\n");
  for (i = 0 ; i < end ; i++) {
    printf("%d ", array1[i]);
  }
  printf("\n");
  //�������
  printf("elements in array1 or array2\n");
  for (i = 0 ; i < end ; i++) {
    printf("%d ", array1[i]);
  }
  for (i = end ; i < size1; i++) {
    printf("%d ", array1[i]);
  }
  for (i = end ; i < size2; i++) {
    printf("%d ", array2[i]);
  }
  printf("\n");
  return 0;
}

