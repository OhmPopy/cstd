/* Ҫִ�е��������� */
typedef struct tpool_work {
  thread_cb fun; /* ������ */
  void* arg; /* �����������Ĳ��� */
  struct tpool_work* next;
} tpool_work_t;
typedef struct tpool {
  int shutdown; /* �̳߳��Ƿ����� */
  int max_thr_num; /* ����߳��� */
  thread_t* thr_id; /* �߳�ID���� */
  tpool_work_t* queue_head; /* �߳����� */
  critical_section_t queue_lock[1];
  cond_t queue_ready[1];
} tpool_t1;
static tpool_t1* tpool = NULL;
/* �������̺߳���, ������������ȡ������ִ�� */
static void* thread_routine(void* arg)
{
  tpool_work_t* work;
  while (1) {
    /* ����̳߳�û�б�������û������Ҫִ�У���ȴ� */
    while (!tpool->queue_head && !tpool->shutdown) {
      cond_wait(tpool->queue_ready, INFINITE);
    }
    critical_section_lock(tpool->queue_lock);
    if (tpool->shutdown) {
      critical_section_unlock(tpool->queue_lock);
      break;
    }
    work = tpool->queue_head;
    tpool->queue_head = tpool->queue_head->next;
    critical_section_unlock(tpool->queue_lock);
    work->fun(work->arg);
    free(work);
  }
  return NULL;
}
/*
 * �����̳߳�
 */
int tpool_init(int max_thr_num)
{
  int i;
  tpool = (tpool_t1*)calloc(1, sizeof(tpool_t1));
  if (!tpool) {
    printf("%s: calloc failed\n", __FILE__);
    return 0;
  }
  /* ��ʼ�� */
  tpool->max_thr_num = max_thr_num;
  tpool->shutdown = 0;
  tpool->queue_head = NULL;
  critical_section_init(tpool->queue_lock);
  cond_init(tpool->queue_ready);
  /* �����������߳� */
  tpool->thr_id = (thread_t*)calloc(max_thr_num, sizeof(thread_t));
  for (i = 0; i < max_thr_num; ++i) {
    thread_create(tpool->thr_id + i, thread_routine, NULL);
  }
  return 0;
}
/* �����̳߳� */
void tpool_destroy()
{
  int i;
  tpool_work_t* member;
  if (tpool->shutdown) {
    return;
  }
  tpool->shutdown = 1;
  /* ֪ͨ�������ڵȴ����߳� */
  critical_section_lock(tpool->queue_lock);
  cond_signal(tpool->queue_ready);
  critical_section_unlock(tpool->queue_lock);
  for (i = 0; i < tpool->max_thr_num; ++i) {
    thread_join(tpool->thr_id + i);
  }
  free(tpool->thr_id);
  while (tpool->queue_head) {
    member = tpool->queue_head;
    tpool->queue_head = tpool->queue_head->next;
    free(member);
  }
  critical_section_destroy(tpool->queue_lock);
  cond_destroy(tpool->queue_ready);
  free(tpool);
}
/* ���̳߳�������� */
int tpool_create(void* fun, void* arg)
{
  tpool_work_t* work, *member;
  if (!fun) {
    printf("%s:Invalid argument\n", __FILE__);
    return -1;
  }
  work = (tpool_work_t*)malloc(sizeof(tpool_work_t));
  if (!work) {
    printf("%s:malloc failed\n", __FILE__);
    return -1;
  }
  work->fun = (thread_cb)fun;
  work->arg = arg;
  work->next = NULL;
  critical_section_lock(tpool->queue_lock);
  member = tpool->queue_head;
  if (!member) {
    tpool->queue_head = work;
  }
  else {
    while (member->next) {
      member = member->next;
    }
    member->next = work;
  }
  /* ֪ͨ�������̣߳������������ */
  critical_section_unlock(tpool->queue_lock);
  cond_signal(tpool->queue_ready);
  return 0;
}

