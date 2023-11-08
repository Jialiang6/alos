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
    } else {
        common_work();
    }
}

int main() {
    main_thread = this_thread::get_id();
    cout << "main_thread: " << main_thread << endl;
    cout << "main thread exe: " << endl;
    check();
    cout << "in thread exe: " << endl;
    thread t(check);
    t.join();
    return 0;
}

/*
    main_thread: 1   
    main thread exe: 
    this thread is: 1
    main
    in thread exe:        
    this thread is: 2
    common
*/