/***************************************************************************
C++11中的std::future是一个模板类。
std::future提供了一种用于访问异步操作结果的机制。
std::future所引用的共享状态不能与任何其它异步返回的对象共享(与std::shared_future相反)
一个future是一个对象，它可以从某个提供者的对象或函数中检索值,
如果在不同的线程中，则它可以正确地同步此访问
 **************************************************************************/

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
using namespace std;

/**
 * future是线程结果的消费者，需要绑定生产者来初始化
 * 三种生产者对应future三种不同的使用方式
 * 生产者即对一个线程的包装
 */

void testPromise() { // 需要显式启动线程并把promise对象传递给线程
    promise<int> p; // 生产者
    future<int> fut = p.get_future(); // 消费者
    thread t([&p](){
        this_thread::sleep_for(chrono::seconds(1));
        p.set_value(1);
    });

    int x = fut.get();
    cout << "[P] value: " << x << '\n';

    t.join();
}

void testPackagedTask() {
    packaged_task<int()> pTask([](){
        this_thread::sleep_for(chrono::seconds(1));
        return 2;
    });
    future<int> fut = pTask.get_future();
    thread t(move(pTask));

    int x = fut.get();
    cout << "[PT] value: " << x << '\n';

    t.join();
}

void testAsync() {
    future<int> fut = async([](){
        this_thread::sleep_for(chrono::seconds(1));
        return 3;
    });

    int x = fut.get();
    cout << "[A] value: " << x << '\n';
}

int main() {
    testPromise();
    testPackagedTask();
    testAsync();
    return 0;
}