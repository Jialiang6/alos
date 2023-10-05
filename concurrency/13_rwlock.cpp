#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <chrono>
using namespace std::chrono_literals;

int data = 0;

class semaphore {
private:
    int sem;
    std::mutex mtx;
    std::condition_variable cv;
public:
    semaphore(): sem(1) {}
    semaphore(int val): sem(val) {}
    ~semaphore() {}
    void P() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return sem > 0; });
        --sem;
    }
    void V() {
        std::unique_lock<std::mutex> lock(mtx);
        ++sem;
        cv.notify_one();
    }
};

/**
 * 1. 读优先锁
 */

class ReadPriLock {
private:
    std::mutex mtx;
    std::condition_variable rCV, wCV;
    int writers, readers;
    int pendreaders;
public:
    ReadPriLock(): writers(0), readers(0), pendreaders(0) {}
    void ReadLock() {
        std::unique_lock<std::mutex> lock(mtx);
        pendreaders++;
        rCV.wait(lock, [&]{ return writers == 0; });
        pendreaders--;
        readers++;
    }
    void ReadUnLock() {
        std::unique_lock<std::mutex> lock(mtx);
        readers--;
        if (readers == 0) {
            wCV.notify_one();
        }
    }
    void WriteLock() {
        std::unique_lock<std::mutex> lock(mtx);
        wCV.wait(lock, [&]{ return readers == 0 && writers == 0; });
        writers++;
    }
    void WriteUnLock() {
        std::unique_lock<std::mutex> lock(mtx);
        writers--;
        if (writers == 0) {
            if (pendreaders > 0) {
                rCV.notify_all();
            } else {
                wCV.notify_one();
            }
        }
    }
};

/**
 * 1.1. 读优先锁 (sem写法)
 */

class ReadPriSem {
private:
    semaphore wMtx;
    semaphore rCntMtx;
    int readers;
public:
    ReadPriSem(): readers(0) {}
    void ReadLock() {
        rCntMtx.P();
        if (readers == 0) {
            wMtx.P();
        }
        readers++;
        rCntMtx.V();
    }
    void ReadUnLock() {
        rCntMtx.P();
        readers--;
        if (readers == 0) {
            wMtx.V();
        }
        rCntMtx.V();
    }
    void WriteLock() {
        wMtx.P();
    }
    void WriteUnLock() {
        wMtx.V();
    }
};

/**
 * 2. 写优先锁
 */

class WritePriLock {
private:
    std::mutex mtx;
    std::condition_variable rCV, wCV;
    int writers, readers;
    int pendwriters;
public:
    WritePriLock(): writers(0), readers(0), pendwriters(0) {}
    void ReadLock() {
        std::unique_lock<std::mutex> lock(mtx);
        rCV.wait(lock, [&]{ return writers == 0 && pendwriters == 0; });
        readers++;
    }
    void ReadUnLock() {
        std::unique_lock<std::mutex> lock(mtx);
        readers--;
        if (readers == 0 && pendwriters > 0) {
            wCV.notify_one();
        }
    }
    void WriteLock() {
        std::unique_lock<std::mutex> lock(mtx);
        pendwriters++;
        wCV.wait(lock, [&]{ return readers == 0 && writers == 0; });
        pendwriters--;
        writers++;
    }
    void WriteUnLock() {
        std::unique_lock<std::mutex> lock(mtx);
        writers--;
        if (writers == 0) {
            if (pendwriters > 0) {
                wCV.notify_one();
            } else {
                rCV.notify_all();
            }
        }
    }
};

/**
 * 2.1. 写优先锁 (sem写法)
 */

class WritePriSem {
private:
    semaphore rCntMtx, wCntMtx, rMtx, wMtx;
    int readers, writers;
public:
    WritePriSem(): readers(0), writers(0) {}
    void ReadLock() {
        rMtx.P();
        rCntMtx.P();
        if (readers == 0) {
            wMtx.P();
        }
        readers++;
        rCntMtx.V();
        rMtx.V();
    }
    void ReadUnLock() {
        rCntMtx.P();
        readers--;
        if (readers == 0) {
            wMtx.V();
        }
        rCntMtx.V();
    }
    void WriteLock() {
        wCntMtx.P();
        if (writers == 0) {
            rMtx.P();
        }
        writers++;
        wCntMtx.V();
        wMtx.P();
    }
    void WriteUnLock() {
        wMtx.V();
        wCntMtx.P();
        writers--;
        if (writers == 0) {
            rMtx.V();
        }
        wCntMtx.V();
    }
};

/**
 * 3.1. 公平锁 (sem写法)
 */

class FairSem {
private:
    semaphore rCntMtx, wMtx;
    semaphore flag;
    int readers;
public:
    FairSem(): readers(0) {}
    void ReadLock() {
        flag.P();
        rCntMtx.P();
        if (readers == 0) {
            wMtx.P();
        }
        readers++;
        rCntMtx.V();
        flag.V();
    }
    void ReadUnLock() {
        rCntMtx.P();
        readers--;
        if (readers == 0) {
            wMtx.V();
        }
        rCntMtx.V();
    }
    void WriteLock() {
        flag.P();
        wMtx.P();
    }
    void WriteUnLock() {
        wMtx.V();
        flag.V();
    }
};

// ReadPriLock rwlock;
// ReadPriSem rwlock;
// WritePriLock rwlock;
// WritePriSem rwlock;
FairSem rwlock;

void writer() {
    while (data < 100) {
        rwlock.WriteLock();
        ++data;
        rwlock.WriteUnLock();
        std::this_thread::sleep_for(10ms);
    }
}

void reader() {
    while (data < 100) {
        rwlock.ReadLock();
        printf("data: %d\n", data);
        rwlock.ReadUnLock();
        std::this_thread::sleep_for(10ms);
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread w1(writer);
    std::vector<std::thread> vec_readers;
    for (int i = 0; i < 10; i++) {
        vec_readers.emplace_back(std::thread(reader));
    }
    w1.join();
    for (int i = 0; i < 10; i++) {
        vec_readers[i].join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    );
    std::cout << "time: " << duration.count() << " ms" << std::endl;
    return 0;
}