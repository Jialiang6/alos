#include <iostream>
#include <thread>

using namespace std;

void do_something(){
    cout << "func" << endl;
}

class X {
public:
    void do_something_in(){
        cout << "func_in_class" << endl;
    }
};

void do_something_p(unique_ptr<X>) {
    cout << "func_using_p" << endl;
}

int main() {
    // thread t(do_something);
    // thread t(&do_something);

    // X x;
    // thread t(X::do_something_in, x);

    thread t(&X::do_something_in, X());

    // thread t(do_something_p, unique_ptr<X>());

    // unique_ptr<X> p(new X);
    // p->do_something_in();
    // thread t(do_something_p, move(p));

    // thread t([](){
    //     cout << "func_lambda" << endl;
    // });

    // thread t0(do_something);
    // thread t(move(t0));

    if (t.joinable()) t.join();
    // std::thread实例可移动，不可复制.
    // 不可复制保性证了在同一时间点，一个std::thread实例只能关联一个执行线程.
    // 可移动性使得程序员可以自己决定，哪个实例拥有实际执行线程的所有权.
    return 0;
}