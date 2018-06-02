/* Ҫִ�е��������� */
typedef struct tpool_work {
  thread_cb fun; /* ������ */
  void* arg; /* �����������Ĳ��� */
  struct tpool_work* next;
} tpool_work_t;
#define MAX_THR_NUM 4
typedef struct pool {
  int shutdown; /* �̳߳��Ƿ����� */
  thread_t thr_id[MAX_THR_NUM]; /* �߳�ID���� */
  tpool_work_t* queue_head; /* �߳����� */
  critical_section_t queue_lock[1];
  cond_t queue_ready[1];
} tpool_t;
/* �������̺߳���, ������������ȡ������ִ�� */
static void* thread_routine(void* arg)
{
  tpool_t* pool = (tpool_t*)arg;
  tpool_work_t* work;
  while (1) {
    /* ����̳߳�û�б�������û������Ҫִ�У���ȴ� */
    while (!pool->queue_head && !pool->shutdown) {
      cond_wait(pool->queue_ready, INFINITE);
    }
    critical_section_lock(pool->queue_lock);
    if (pool->shutdown) {
      critical_section_unlock(pool->queue_lock);
      break;
    }
    work = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    critical_section_unlock(pool->queue_lock);
    work->fun(work->arg);
    free(work);
  }
  return NULL;
}
tpool_t* tpool_get(tpool_work_t* work, int opt);
/* �����̳߳� */
int tpool_destroy()
{
  tpool_get(NULL, 3);
  return 0;
}
tpool_t* tpool_get(tpool_work_t* work, int opt)
{
  static tpool_t pool[1] = {0};
  int i;
  static int inited = 0;
  if (3 != opt) {
    if (!inited) {
      /* ��ʼ�� */
      pool->shutdown = 0;
      pool->queue_head = NULL;
      critical_section_init(pool->queue_lock);
      cond_init(pool->queue_ready);
      /* �����������߳� */
      for (i = 0; i < MAX_THR_NUM; ++i) {
        thread_create(pool->thr_id + i, thread_routine, pool);
      }
      inited = 1;
    }
    if (2 == opt) {
      tpool_work_t* member;
      critical_section_lock(pool->queue_lock);
      member = pool->queue_head;
      if (!member) {
        pool->queue_head = work;
      }
      else {
        while (member->next) {
          member = member->next;
        }
        member->next = work;
      }
      /* ֪ͨ�������̣߳������������ */
      critical_section_unlock(pool->queue_lock);
      cond_signal(pool->queue_ready);
    }
  }
  else {
    if (inited) {
      tpool_work_t* member;
      if (pool->shutdown) {
        return 0;
      }
      pool->shutdown = 1;
      /* ֪ͨ�������ڵȴ����߳� */
      critical_section_lock(pool->queue_lock);
      cond_signal(pool->queue_ready);
      critical_section_unlock(pool->queue_lock);
      for (i = 0; i < MAX_THR_NUM; ++i) {
        thread_join(pool->thr_id + i);
      }
      free(pool->thr_id);
      while (pool->queue_head) {
        member = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(member);
      }
      critical_section_destroy(pool->queue_lock);
      cond_destroy(pool->queue_ready);
      free(pool);
    }
  }
  return pool;
}
/* ���̳߳�������� */
int tpool_create(void* fun, void* arg)
{
  tpool_work_t* work;
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
  tpool_get(work, 2);
  return 0;
}

