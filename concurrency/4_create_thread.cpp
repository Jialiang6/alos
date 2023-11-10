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

class Y {
public:
    void operator()(int x) {
        cout << "func_" << endl;
    }
};

class Z {
public:
    static void do_something_in(){
        cout << "static_func_in_class" << endl;
    }
};

int main() {
    /**
     * 1. function
     * 2. function pointer
     * 3. non-static member function
     * 4. static member function
     * 5. pseudo-function
     * 6. lambda function
     */

    // thread t(do_something);             // function

    // thread t(&do_something);            // function pointer
    // typedef void (*func_ptr)();
    // func_ptr fp = &do_something;        // == void (*fp)() = &do_something;
    // thread t(fp);                       // function pointer

    // X x;
    // thread t(X::do_something_in, x);    // non-static member function (type 1)

    // thread t(&X::do_something_in, X()); // non-static member function (type 2)

    // thread t(&Z::do_something_in);      // static member function

    // thread t(Y(), 10);                  // pseudo-function

    // thread t([](){                      // lambda function
    //     cout << "func_lambda" << endl;
    // });

    /**
     * move thread
     */

    // thread t0(do_something);
    // thread t(move(t0));

    /**
     * use Pointers to point to objects
     */

    unique_ptr<X> p(new X); // 深入：this指针实际是函数调用的一个参数
    p->do_something_in();  // 用指针调用对象的成员函数
    thread t(do_something_p, move(p)); // 独占
    // thread t(do_something_p, unique_ptr<X>()); 

    if (t.joinable()) t.join();
    /*
        std::thread实例可移动，不可复制.
        1) 不可复制保性证了在同一时间点，一个std::thread实例只能关联一个执行线程.
        2) 可移动性使得程序员可以自己决定，哪个实例拥有实际执行线程的所有权.
    */

    return 0;
}