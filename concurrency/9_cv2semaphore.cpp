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

int cnt = 0;
semaphore sp(1);
mutex a_mtx;

void test(){
    int num = 1e6;
    for (int k = 0; k < num; k++) {
        sp.P();
        // a_mtx.lock();
        cnt = cnt + 1;
        // a_mtx.unlock();
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
    cout << "now cnt is: " << cnt << endl;
    return 0;
}