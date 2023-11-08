
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

void some_function() {}
void some_other_function() {}

// std::thread不支持拷贝语义。
// std::thread支持移动语义。

class scoped_thread {
    std::thread t;
public:
    explicit scoped_thread(std::thread t_): t(std::move(t_)) {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }

    ~scoped_thread() {
        t.join();                                         
    }

    scoped_thread(scoped_thread const &) = delete;
    scoped_thread &operator=(scoped_thread const &) = delete;
};


int main() {
    // 构造一个thread对象t1
    std::thread t1(some_function);
    // 并非复制，调用的是operator=(std::thread&&)，剥夺原先实例的资源。
    std::thread t2 = std::move(t1);
    // 并非复制，调用的是operator=(std::thread&&)，剥夺原先实例的资源。
    t1 = std::thread(some_other_function);

    std::thread t3;
    t3 = std::move(t2);
   
    // t1 = std::move(t3);
    // 把t3的资源给t1，非法。因为`t1`已经有了一个相关的线程
    // 会调用`std::terminate()`来终止程序。

    t1.join();
    t3.join();
    return 0;
}