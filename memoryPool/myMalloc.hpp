#ifndef DEFAULT_MALLOC
#define DEFAULT_MALLOC

#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;
typedef void (*FuncPtr)();

//一级配置器，大于128的直接调用malloc和free
class DefaultMalloc{
public:
    static void* allocate(size_t n);
    static void deallocate(void *p,size_t n);
    static void* reallocate(void *p,size_t old_sz,size_t new_sz);
    // static void (*set_malloc_handler( void (*f)() ) ) (); 
    static FuncPtr set_malloc_handler( FuncPtr f ); 
private:
    static void* oom_malloc(size_t n);
    static void* oom_realloc(void *p,size_t n);
    static void (*__malloc_alloc_oom_handler)(); // 函数指针
};



#endif