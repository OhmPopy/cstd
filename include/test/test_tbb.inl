#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

// ����tbb_debug.lib
#pragma comment(lib, "tbb_debug.lib")

using namespace tbb;

// ��ÿ��Itemִ�иò���
void Foo(float value)
{
    printf("%.2f ", value);
}

class ApplyFoo
{
    float * const my_a;
public:
    void operator () (const blocked_range<size_t> & r) const
    {
        float * a = my_a;
        for (size_t i = r.begin(); i != r.end(); ++ i)
            Foo(a[i]);
    }
    
    ApplyFoo(float a[]) : my_a(a) {}
};

int test_tbb()
{
    // ����task scheduler
    // task_scheduler_init֧��һ����������ָ��ʹ�õ��߳���
    task_scheduler_init init;
    float a[100];
    for (int i = 0; i < 100; i ++)
        a[i] = (float)i;
    // TBB�������ֳ����ɵ�block
    // ��block����ApplyFoo���functor
    parallel_for(blocked_range<size_t>(0, 100), ApplyFoo(a));
    return 0;
}