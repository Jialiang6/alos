#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "pool.hpp"
using namespace std;


//初始化函数指针(static成员变量要在类外初始化)
void (*DefaultMalloc::__malloc_alloc_oom_handler)()=0;

//分配内存，直接调用malloc
void* DefaultMalloc::allocate(size_t n){
    void* result=malloc(n);
    if(result==0) result=oom_malloc(n);
    return result;
}

//释放内存
void DefaultMalloc::deallocate(void *p,size_t n) {
    free(p);
}

//重新分配内存
void* DefaultMalloc::reallocate(void *p,size_t old_sz,size_t new_sz){
    void* result=realloc(p,new_sz);
    if(result==0) result=oom_realloc(p,new_sz);
    return result;
}

//设置内存不在时的处理函数
// void (*DefaultMalloc::set_malloc_handler(void (*f)()))(){
FuncPtr DefaultMalloc::set_malloc_handler(FuncPtr f) {
    void (*old)()=__malloc_alloc_oom_handler;
    __malloc_alloc_oom_handler=f;
    return old;
}

//malloc失败时调用此函数
void* DefaultMalloc::oom_malloc(size_t n){
    void (*my_malloc_handler)();
    void* result;
    for(;;){
        my_malloc_handler=__malloc_alloc_oom_handler;
        if(my_malloc_handler==0) exit(-1);
        (*my_malloc_handler)();
        result=malloc(n);
        if(result) return result;
    }
}

//realloc失败时调用此函数
void* DefaultMalloc::oom_realloc(void *p,size_t n){
    void (*my_malloc_handler)();
    void* result;
    for(;;){
        my_malloc_handler=__malloc_alloc_oom_handler;
        if(my_malloc_handler==0) exit(-1);
        (*my_malloc_handler)();
        result=realloc(p,n);
        if(result) return result;
    }
    return result;
}




MemoryPool::obj* volatile MemoryPool::free_list[__NUM_FREELISTS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char* MemoryPool::start_cand=0;
char* MemoryPool::end_cand=0;
size_t MemoryPool::pool_size=0;

MemoryPool::MemoryPool(size_t n){
    start_cand=(char*)DefaultMalloc::allocate(n);
    end_cand=start_cand+n;
    pool_size=n;
}

void* MemoryPool::allocate(size_t n){
    obj* volatile *this_free_list; // volatile限定的是*this_free_list,即obj*,不是obj也不是obj**
    obj* result;
    //当申请的内存大于128bytes或小于等于0，直接调用一级配置器，即malloc
    if(n > (size_t)__MAX_BYTES || n<=0){
        return DefaultMalloc::allocate(n);
    }
    //找到长度为n的内存要从第几个链表上来申请
    this_free_list=free_list+FREELIST_INDEX(n);
    result=*this_free_list; // 类似于：result = free_list[FREELIST_INDEX(n)];
    
    //如果链表上没有相应大小的内存了，就调用refill从内存池申请
    if(result==0){
        void *r=refill(ROUND_UP(n));
        return r;
    }
    *this_free_list=result->list_next; // 直接从链表上申请一块区域，链表就往下走
    return result;
}

void MemoryPool::deallocate(void* p,size_t n){
    obj* volatile *this_free_list;
    obj* q=(obj*)p;
    if(n > (size_t)__MAX_BYTES || n<=0){
        DefaultMalloc::deallocate(p,n);
        return ;
    }
    this_free_list=free_list+FREELIST_INDEX(n);
    //将p指向的内存归还到相应的链表上(放在链表头部)
    q->list_next=*this_free_list;
    *this_free_list=q;
}

void* MemoryPool::reallocate(void* p,size_t old_sz,size_t new_sz){
    void* result;
    size_t copy_size;
    if(old_sz > (size_t)__MAX_BYTES || new_sz<=0 || old_sz<=0){
        return DefaultMalloc::reallocate(p,old_sz,new_sz);
    }
    if(ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
    result=allocate(new_sz);
    copy_size=new_sz>old_sz ? old_sz : new_sz;
    //申请完新内存之后，把旧地址的数据复制到新地址
    memcpy(result,p,copy_size);
    deallocate(p,old_sz);
    return result;
}



// 当某一规格空间对应的链表为空，从内存池申请X块空间到这个链表上，并返回首个空闲空间
// 这里的n应该是8的倍数
void* MemoryPool::refill(size_t n){
    //默认每次返回20个长度为n的块，其中返回给用户一块，剩下的连到相应大小的链表上。
    int nobjs=20;
    //从内存池中申请20块大小为n的内存。
    char* chunk=chunk_alloc(n,nobjs);
    obj* volatile *this_free_list;
    obj* result, *current_obj, *next_obj;
    //上面chunk_alloc中的nobjs是引用传值，可能会改变nobjs的值。
    if(nobjs==1) return chunk;
    this_free_list=free_list+FREELIST_INDEX(n);
    result=(obj*)chunk;
    *this_free_list=current_obj=(obj*)(chunk+n);
    //把剩余的块都连到链表上
    for(int i=0; i<nobjs-1;i++){
        next_obj=(obj*)((char*)current_obj+n);
        current_obj->list_next=next_obj;
        current_obj=next_obj;
    }
    current_obj->list_next=0;
    return result;
}
// 负责从空闲内存区取内存的工作，负责内存池最核心的工作
// 这里的n应该是8的倍数
char* MemoryPool::chunk_alloc(size_t size,int &objs){
    char* result;
    //计算要申请到内存总数
    size_t total_bytes=size* objs;
    //计算内存池中未连接的备用内存还有多少
    size_t bytes_left=end_cand-start_cand;
    //如果剩余内存足够申请20块内存
    if(bytes_left>=total_bytes){
        result=start_cand;
        start_cand+=total_bytes;
        return result;
    //如果不够20块，但是能提供一块或以上
    }else if(bytes_left>=size){
        objs=bytes_left/size;
        total_bytes=size*objs;
        result=start_cand;
        start_cand+=total_bytes;
        return result;
    //如果一块都提供不了，就需要另想办法了
    }else{
        //内存池不够了就从堆中申请，从堆中申请的数量这样计算：
        //要申请的大小的2倍+一个随申请次数增大的值
        //(heap_size一直记录申请内存总数,右移四位相当于除以16，应该是有十六个规格的原因？
        size_t bytes_to_get=total_bytes*2+ROUND_UP(pool_size>>4);
        //如果剩余内存无法提供一块size，但是还剩下一点，就先把这个块内存连到链表上
		//剩下的这点bytes_left应当保证是8的倍数，所以刚开始初始化end_free和start_free时要注意。
        if(bytes_left>0){
            obj* volatile *this_free_list=free_list+FREELIST_INDEX(bytes_left);
            ((obj*)start_cand)->list_next=*this_free_list;
            *this_free_list=(obj*)start_cand;
        }
        //向堆中再申请新的内存
        start_cand=(char*)malloc(bytes_to_get);
        //如果堆中内存也没了
        if(0==start_cand){
            obj *volatile *this_free_list,*p;
            //再遍历一下链表，看被管理起来的内存中有没有能够本次使用的
            for(int new_size=size;new_size<=__MAX_BYTES;new_size+=__ALIGN){
                this_free_list=free_list+FREELIST_INDEX(new_size);
                p=*this_free_list;
                if(p!=0){
                    *this_free_list=p->list_next;
                    start_cand=(char*)p;
                    end_cand=start_cand+new_size;
                    //再递归调用自身，看刚回收的那点内存能使用不
                    return chunk_alloc(size,objs);
                }
            }
            end_cand=0;
			//最后实在没有了，让malloc再尝试一下。
            start_cand=(char*)DefaultMalloc::allocate(bytes_to_get);
        }
        pool_size+=bytes_to_get;
        end_cand=start_cand+bytes_to_get;
        //如果从堆中申请到了内存，再递归调用自身
        return chunk_alloc(size,objs);
    }
}




#define USEPOOL

int main(){
    MemoryPool pool(10);
    clock_t start,end;
    int sum=0;
    vector<int> randnum(200005);
    for(int i=0;i<200000;i++){
        randnum[i]=rand()%128;
    }

#ifdef USEPOOL
    for(int i=0;i<100;i++){
        start=clock();
        for(int i=0;i<200000;i++){
            char *p=(char*)MemoryPool::allocate(randnum[i]);
            MemoryPool::deallocate(p,randnum[i]);
        }
        end=clock();
        sum+=(end-start);
    }
    cout<<"MemoryPool: malloc-free memory 20000 times (0-128bytes):"<<sum/100<<" ms"<<endl;
#else
    sum=0;
    for(int i=0;i<100;i++){
        start=clock();
        for(int i=0;i<200000;i++){
            char* p = new char(randnum[i]);
            delete p;
            p = nullptr;
        }
        end=clock();
        sum+=(end-start);
    }
    cout<<"OS: malloc-free memory 20000 times (0-128bytes):"<<sum/100<<" ms"<<endl;

#endif

    return 0;
}