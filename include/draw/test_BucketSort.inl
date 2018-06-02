// �ǱȽ����򣬲���Ҫ�Ƚϣ�������������ʱ�����������ȱ�㣺�ռ�Ҫ��϶࣬����ԭ�����򣬵��͵Ŀռ任ȡʱ�䡣
// ��������
// ������������һ���ص㣺�Ѿ��ź��������С���󣩵������У���i��Ԫ��Ϊx����������һ���У�С�ڵ���x��Ԫ����i��������������Ҫһ���ռ���ۣ�n + max_num.
// Ͱ����
// Ͱ��������������裺�ַ�Ԫ�ص�ÿ��Ͱ�ڣ�˳�����Ͱ��Ԫ�ء�α�������£�
// BUCKET_SORT (A)
// n �� length [A]
// For i = 1 to n do
// Insert A[i] into list B[nA[i]]
// For i = 0 to n-1 do
// Sort list B with Insertion sort
// Concatenate the lists B[0], B[1], . . B[n-1] together in order.
// ����ʵ��
// bucket_sort��ֱ�ӱ���count �ж��Ƿ����1,����Ԫ�أ�counting_sort�ǰ�count������ת��Ϊԭ����������Ԫ��ǰ��Ԫ�صĸ�����ͬ����Ҫ����һ�Ρ�
// ���Խ����
//
// [cpp] view plaincopyprint?
// Test number : 10000000
// count_sort : 112
// BucketSort : 70
// std::sort : 4646
//
//������Ͱ����ʱ���ϵ����Ʋ���˵�ˣ�������Щ�ǱȽ������㷨�Կռ�Ҫ��Ƚϸߣ����Ҹ��������ݵķֲ�Ҳ�йء�����Ĳ��Խ���ǻ�������������ݶ���[0-255]֮��ļ���ǰ�ᡣ���ݷֲ�Խɢ�ң��ռ临�Ӷ�Խ�ߡ�
//
// Ͱ�����˼·�����ʺ��ַ��������������Ϊacsii�ַ���λ��0��255֮�䡣����һ���Ͱ�������ʹ������
void BucketSort(int* array, int length)
{
  int i, k, idx;
  enum { MAX = 256 };
  int bucket[MAX];

  for (i = (0); i < MAX; i++) {
    bucket[i] = 0;
  }

  for (i = 0; i < length; i++) {
    bucket[array[i]]++;
  }

  for (i = 0, idx = 0; i < MAX; i++)
    for (k = bucket[i]; k > 0; k--) { // ����ÿ��Ͱ��Ԫ��
      array[idx++] = i;
    }
}
void counting_sort(int* arr, int* dst, int length)
{
  enum { max_num = 255 };
  int i, count[max_num];

  for (i = (0); i < max_num; i++) {
    count[i] = 0;
  }

  for (i = (0); i < length; i++) {
    count[ arr[i] ]++;
  }

  for (i = (1); i < max_num; i++) {
    count[i] += count[i - 1];
  }

  for (i = length - 1; i >= 0; i--) {
    dst[ count[arr[i]] - 1 ] = arr[i]; // �����һ
    count[ arr[i] ]--;
  }
}
int test_BucketSort()
{
  if (1) {
    int i;
    int myints[] = {10, 20, 30, 5, 15};
    int length = sizeof(myints) / sizeof(myints[0]);
    int result[100];
    counting_sort(myints, result, length);

    for (i = (0); i < length; i++) {
      printf("%d ", result[i]);
    }

    printf("\n");
  }

  if (1) {
    int idx;
    clock_t t;
    enum { test_num = 1000000 }; // 1 million
    int* testPtr = MALLOC(int, test_num);
    int* testPtr1 = MALLOC(int, test_num);
    int* testPtr2 = MALLOC(int, test_num);
    srand((int)time(0));
    printf("Test number \t:\t\t%d\n", test_num);

    for (idx = 0; idx < test_num; idx++) {
      testPtr2[idx] = testPtr1[idx] = testPtr[idx] = rand() % 255;
    }

    t = clock();
    // sort
    // keep the testPtr1, change the testPtr.
    counting_sort(testPtr1, testPtr, test_num);
    printf("count_sort \t:\t\t%d\n" , clock() - t);
    t = clock();
    BucketSort(testPtr1, test_num);
    printf("BucketSort \t:\t\t%d\n", clock() - t);
    t = clock();
    QSORT(testPtr2, testPtr2 + test_num, LESS, int);
    printf("QSORT \t:\t\t %d", clock() - t);

    // Test the result validity
    for (idx = 0; idx < test_num; idx++) {
      if (testPtr[idx] != testPtr1[idx] || testPtr1[idx] != testPtr2[idx]) {
        printf("wrong");
      }
    }

    free(testPtr);
    free(testPtr1);
    free(testPtr2);
    printf("\n");
  }

  system("PAUSE");
  return 0;
}

