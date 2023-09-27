
/*
    select(): #include<sys/select.h>
    poll(): #include<pool.h>
    epoll(): #include<sys/epoll.h>
    这些函数都在C的头文件中声明，并且不能在win中直接使用


    1. select()
    int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
    void FD_ZERO(fd_set *set);    //清空文件描述符集合
    void FD_SET(int fd, fd_set *set);    //将待监听的文件描述符添加到监听集合中
    void FD_CLR(int fd, fd_set *set);    //将文件描述符从监听集合中移除
    int FD_ISSET(int fd, fd_set *set);    //判断文件描述符是否在监听集合中

    nfds：最大描述符+1，告诉内核检测多少个描述符的状态。
    *fds: 监听有读就绪、写就绪、异常就绪文件描述符的集合,传入传出参数。
    timeout：定时阻塞监听时间，填NULL将永远等下去至少有一个事件就绪才会返回，设置timeval里的事件均为0则检查完所有描述符后立即返回，若不为0则为设置监听的超时时间，超时时间如果到了还没有事件就绪就返回。
    返回值是所有监听的描述符中有事件就绪的描述符个数，返回-1为出错。


    2. poll()
    int poll(struct pollfd *fds, unsigned int nfds, int timeout);
    struct pollfd 
    {
        ...
        short events;         // 待监听的事件
        short revents;       // 实际就绪的事件
    } ;

    fds：监听的pollfd结构体数组。
    nfds：传入的pollfd结构体数组中需要监听的个数，需要监听多少个文件描述符。
    事件取值POLLIN、POLLOUT、POLLERR分别代写、读、异常。


    3. epoll()
    int epoll_create(int size);  // size为供内核参考的创建监听节点的数量。
                                    返回指向新创建的红黑树根节点fd，失败返回-1。

    int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); // 返回0成功，-1失败。 
    typedef union epoll_data{
        void    *ptr;
        int       fd;
        uint32_t u32;
        uint64_t u64;
    }
    struct epoll_event{
        uint32_t       events;
        epoll_data       data;
    }

    epfd: epoll_create返回值。
    op: EPOLL_CTL_ADD添加新fd监听
        EPOLL_CTL_MOD修改监听事件
        EPOLL_CTL_DEL将一个fd从监听红黑树删除取消监听。
    fd: 被操作的套接字描述符。
    event: epoll_event结构体地址
        events主要有EPOLLIN、EPOLLOUT、EPOLLERR
        默认是水平触发，(EPOLLIN|EPOLLET)则表示边缘触发监听读就绪事件
        data是一个联合体
    
    int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);

    epfd: epoll_create返回值。
    events: 传出参数，传出满足监听事件结构体数组。
    maxevents: 数组的最大长度。
    timeout: 超时时长ms。
            传-1则阻塞等待至少有一个描述符事件就绪才返回；
            传0表示轮询完fd红黑树立即返回，不阻塞进程
            传大于0的指定毫秒数超时时长，超时且还未有描述符事件就绪就返回。
    返回值是所有监听的描述符中有事件就绪的描述符个数，返回-1为出错。

*/