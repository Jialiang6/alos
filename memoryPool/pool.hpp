#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "myMalloc.hpp"
using namespace std;

enum{__ALIGN=8};
enum{__MAX_BYTES=128};
enum{__NUM_FREELISTS=16};

//二级配置器，当函数小于128bytes时，使用此类来管理内存
class MemoryPool{
public:
    MemoryPool(){}; //无参构造
    MemoryPool(size_t n); //带参构造，可以先给内存池分配n个bytes
    static void* allocate(size_t n);
    static void deallocate(void* p,size_t n);
    static void* reallocate(void* p,size_t old_sz,size_t new_sz);

private:
    //链表的union，用来串联每块内存
    union obj{
        union obj *list_next;
        char data[1];
    };
    //这种结构的好处是，在内存空闲不用时，
    // 就把它当做obj*free_list_link来用，可以让它指向下一块内存，这样就能把空闲内存都串起来
    // 当内存被申请后，当client_data来对待，可以全部用来储存数据，省去了传统链表所占用的空间，有效地利用了内存。

    //向上取到8的整数倍
    static inline size_t ROUND_UP(size_t bytes){
        // return ( (bytes+__ALIGN-1) / __ALIGN * __ALIGN);
        return ( (bytes+__ALIGN-1) & ~(__ALIGN-1) ); // 骚吧
    }
    //分了16个规格来管理内存，分别是8 16...128，此函数用来判断此次申请的bytes个字节要到哪个规格的链表去申请内存
    static inline size_t FREELIST_INDEX(size_t bytes){
        // return ( (bytes+__ALIGN-1)/__ALIGN -1 );
        return ( bytes / __ALIGN );
    }

    //当链表上的空间不足时，refill会调用chunk_alloc向内存池申请空间，连到链表上
    static void* refill(size_t n);
    //用来向内存池申请空间，执行具体的操作
    static char* chunk_alloc(size_t size,int &objs);//此处要用引用

    //长度为16的数组，每个元素都索引一条链表，用来管理相应大小内存
    static obj* volatile free_list[__NUM_FREELISTS];
    //用来标示内存池的起止位置，这里面是未被链表连起来的内存，链表上的内存不够用时，会从这块内存池中申请内存
    static char *start_cand,*end_cand;
    //内存池申请到的总空间大小，包括被free_list串起来的。
    static size_t pool_size; 
};



#endif

/*
    内存池的理想作用（小于128 B的内存才可能用内存池管理）：
    1. 减少内存碎片
    2. 当需要频繁分配和释放内存时减少分配和释放的开销

    核心思想：
    1. 内存池初始化申请大块内存，有空闲内存时直接从内存池里分配，而不需要malloc系统调用；不够再调用。
    2. 内存池内有两种内存，申请内存时首先找的是用链表连接的空间（每条链表初始化为空）
    3. 其次是找备用区域，并把这些区域连接到对应链表
    3. 链表内的节点用union来节省空间，区域未申请时节点存储下一区域的地址，申请后存储对象的数据
    4. 对不同尺寸的空间用不同链表进行管理（16种规格），减小内存碎片
    5. 在对象前预留空间做越界检测

    内存池初始化：
    1. 用malloc之类的系统调用申请大块内存作备用空间（未被链表连起来的）
    2. 并初始化几种规格空间对应的链表为空：obj* volatile free_list[__NUM_FREELISTS]
    3. 总大小用pool_size记录，初始化为n。备用空间用start_cand和end_cand指向头尾。

    可选优化：
    1. 重载new和delete来优化分配和释放
    2. 为了减少线程同步开销，对重要线程单独开内存池
*/