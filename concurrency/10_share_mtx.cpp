#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>

// at least c++17
 
class ThreadSafeCounter {
private:
  mutable std::shared_mutex mutex_;
  unsigned int value_;
public:
  ThreadSafeCounter(): value_(0) {}
 
  // 多个线程可以同时读取 counter 计数
  void read() const {
    std::shared_lock lck(mutex_);
    printf("id: %d, cnt: %d\n", std::this_thread::get_id(), value_);
  }
 
  // 只有1个线程可以修改 counter 计数
  void write() {
    std::unique_lock lck(mutex_);
    value_++;
    printf("[w] id: %d, cnt: %d\n", std::this_thread::get_id(), value_);
  }
 
  // 只有1个线程可以修改 counter 计数
  void reset() {
    std::unique_lock lck(mutex_);
    value_ = 0;
  }
};
 
int main() {
  ThreadSafeCounter counter;
 
  auto write = [&counter]() {
    for (int i = 0; i < 1; i++) {
      counter.write();
    }
  };

  auto read = [&]() {
    for (int i = 0; i < 3; i++) {
      counter.read();
    }
  };

  int num_threads = 8;
  std::thread threads[num_threads];
  for (int i = 0; i < num_threads; i++) {
    if ((i&1) == 0) threads[i] = std::thread(write);
    if ((i&1) == 1) threads[i] = std::thread(read);
  }
  for (int i = 0; i < num_threads; i++) {
    threads[i].join();
  }
  return 0;
}
// 写有序互斥+1，读可以并发
/*
[w] id: 2, cnt: 1
id: 3, cnt: 1
[w] id: 4, cnt: 2
id: 5, cnt: 2
[w] id: 6, cnt: 3
id: 3, cnt: 3
id: 7, cnt: 3
[w] id: 8, cnt: 4
id: 5, cnt: 4
id: 5, cnt: 4
id: 3, cnt: 4
id: 7, cnt: 4
id: 7, cnt: 4
id: 9, cnt: 4
id: 9, cnt: 4
id: 9, cnt: 4
*/
