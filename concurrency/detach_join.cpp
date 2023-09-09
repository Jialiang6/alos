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
    t.detach();
    if (t.joinable()) // 2次detach直接退出
        t.detach();

    // t.join();
    // if (t.joinable())
    //     t.join();

    cout << "main() Done" << endl;
    return 0;
}