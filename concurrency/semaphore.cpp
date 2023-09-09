#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class semaphore {
private:
    int sem;
    int wakeups;
    mutex mtx;
    condition_variable cv;
public:
    semaphore(int val): sem(val), wakeups(0) {}
    void P() {
        unique_lock<mutex> lock(mtx);
        if (--sem < 0) {
            cv.wait(lock, [&](){return wakeups > 0;});
            --wakeups;
        }
    }
    void V() {
        lock_guard<mutex> lock(mtx);
        if (++sem <= 0) {
            ++wakeups;
            cv.notify_one();
        }
    }
};

int i = 0;
semaphore sp(1);

void test(){
    int num = 1e6;
    for (int k = 0; k < num; k++) {
        sp.P();
        i = i + 1;
        sp.V();
    }
}

int main(int argc, const char* argv[]) {
    cout << "Start thread" << endl;
    thread thread_test1( test );
    thread thread_test2( test );

    thread_test1.join();
    thread_test2.join();

    cout << "All threads joined." << endl;
    cout << "now i is: " << i << endl;
    return 0;
}