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
    t.detach(); // detach的作用是将t线程完全与main分离，即使main返回，t还能运行且不报错
    if (t.joinable())
        t.detach();

    // t.join();  // join的作用是阻塞main线程，直到t线程完成
    // if (t.joinable())
    //     t.join();

    /**
     * std::thread对象可以处于以下三种状态之一：
     * 1. joinable：thread对象代表了一个用户线程
     * 2. not joinable：thread对象的线程已经被连接或分离
     * 3. invalid: 默认构造的thread对象，无效，不能代表一个用户线程
     */
    cout << "main() Done" << endl;
    return 0;
}