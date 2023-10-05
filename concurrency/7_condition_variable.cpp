#include <iostream>
#include <thread>
#include <condition_variable>
using namespace std;
/*
    修改共享资源的部分容易理解；
    wait部分较难理解，这里给出了wait部分的范本：
     1. 【wait前先上锁mtx，wait之前会自动mtx.unlock()】
        这是为了保证判断是否wait和wait这两个操作的原子性，不然可能漏掉消息，
        判断要wait后，如果中间另一线程能修改共享资源，不用wait时，这时还wait就出错
     2. 【推荐不光wait(mtx)，要加上pred】
        提前满足pred->true时不wait提高效率，同时判断是否虚假唤醒
        被唤醒后检查pred，如果pred->false就是虚假唤醒，此时继续等待
        如果逻辑实现正确，虚假唤醒指的是硬件问题或者发送唤醒你的信号后，
        其他线程处理更快把pred改成false，此时你不应该被唤醒
     3. 【从wait往下之前还隐性加mtx，获锁成功执行完代码块剩余代码后才释放mtx】
        这样是为了确保notify_one代码块中剩余代码都执行完才轮到wait后代码的执行
        或者为了保证notify_all实际上真的notify了all，让线程有序执行
     4. 【notify_all唤醒多个线程一个时间只有一个线程获锁，必须一一有序执行剩余代码】

     
*/

condition_variable cv;
mutex mtx;
bool ready = false;

void print_id(int id) {
    unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, []{ return ready; });
    cout << "thread " << id << '\n';
}

void go() {
    unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main() {
    thread threads[10];
    for (int i=0; i<10; ++i)
        threads[i] = thread(print_id, i);

    cout << "10 threads ready to race...\n";
    go();

    for (auto& th : threads) th.join();

    return 0;
}