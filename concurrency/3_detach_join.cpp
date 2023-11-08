#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void do_something(int len) {
    for (int i = 0; i < len; i++) {
        cout << i << " ";
    }
    cout << endl;
    this_thread::sleep_for(chrono::seconds(3));
}

int main() {
    cout << "main()" << endl;
    thread t(do_something, 10);
    t.detach(); // detach将t线程与main分离，即使main返回，t还能运行
    if (t.joinable())
        t.detach();

    // t.join();  // join阻塞main线程，直到t线程完成
    // if (t.joinable())
    //     t.join();

    cout << "main() Done" << endl;
    return 0;
}

/**
 * std::thread对象可以处于以下三种状态之一：
 * 1. joinable：thread对象代表了一个用户线程
 * 2. not joinable：thread对象的线程已经被连接或分离
 * 3. invalid: 默认构造的thread对象，无效，不能代表一个用户线程
 * 
 * 建议在main()函数之前将所有thread对象join()或detach()
 * 1. joinable的thread在main线程执行完后还没执行完，会被终止
 * 2. joinable的thread在main线程执行完前执行完，资源还未释放完全
 * 
 * 概述两种特殊的进程状态：
 * 孤儿进程：父进程先结束，操作系统将其父进程设为init进程，确保孤儿进程被正确回收。（还算正常）
 * 僵尸进程：子进程先结束，父进程没调用wait()/waitpid()来回收其PCB信息。（过多导致资源泄漏）
 */