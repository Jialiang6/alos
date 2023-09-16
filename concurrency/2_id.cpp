#include <iostream>
#include <thread>

using namespace std;

thread::id main_thread;

void main_work() {cout << "main" << endl;}
void common_work() {cout << "common" << endl;}

void check() {
    cout << "this thread is: " << this_thread::get_id() << endl;
    if (this_thread::get_id() == main_thread) {
        main_work();
    }
    common_work();
}

int main() {
    main_thread = this_thread::get_id();
    cout << "main_thread: " << main_thread << endl; // 1
    cout << "main thread exe: " << endl;
    check();  // 1
    cout << "in thread: " << endl;
    thread t(check);
    t.join(); // 2
    return 0;
}