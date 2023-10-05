#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

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
        // if (--sem < 0) {
        //     cv.wait(lock, [&](){return wakeups > 0;});
        //     // cv.wait(lock); // 由于虚假唤醒，没条件谓词或者加while就是会比较慢，原因不详
        //     --wakeups;
        // }

        // 节省wakeup的实现方式(但不直观)：
        cv.wait(lock, [&](){return sem > 0;});
        // if (sem <= 0) { // 存在虚假唤醒-> cnt: 2998548
        //     cv.wait(lock);
        // }
        --sem;
    }
    void V() {
        lock_guard<mutex> lock(mtx);
        // if (++sem <= 0) {
        //     ++wakeups;
        //     cv.notify_one();
        // }

        // 节省wakeup的实现方式(但不直观)：
        ++sem;
        cv.notify_one();
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
    auto start_time = chrono::high_resolution_clock::now();
    cout << "Start thread" << endl;
    thread thread_test1( test );
    thread thread_test2( test );
    thread thread_test3( test );

    thread_test1.join();
    thread_test2.join();
    thread_test3.join();

    cout << "All threads joined." << endl;
    cout << "now cnt is: " << cnt << endl;
    auto end_time = chrono::high_resolution_clock::now();
    auto duration_time = chrono::duration_cast<chrono::milliseconds>(end_time-start_time);
    cout << "duration time: " << duration_time.count() << " ms" << endl;
    return 0;
}