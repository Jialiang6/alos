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
  unsigned int get() const {
    std::shared_lock lck(mutex_);
    return value_;
  }
 
  // 只有1个线程可以修改 counter 计数
  void increment() {
    std::unique_lock lck(mutex_);
    value_++;
  }
 
  // 只有1个线程可以修改 counter 计数
  void reset() {
    std::unique_lock lck(mutex_);
    value_ = 0;
  }
};
 
int main() {
  ThreadSafeCounter counter;
 
  auto increment_and_print = [&counter]() {
    for (int i = 0; i < 4; i++) {
      counter.increment();
      printf("id: %d, cnt: %d\n", std::this_thread::get_id(), counter.get());
    }
  };

  std::thread threads[5];
  for (int i = 0; i < 3; i++) {
    threads[i] = std::thread(increment_and_print);
  }
  for (int i = 0; i < 3; i++) {
    threads[i].join();
  }
  return 0;
}
// 写有序+1，读可以乱序
/*
id: 2, cnt: 1
id: 2, cnt: 4
id: 2, cnt: 5
id: 2, cnt: 6
id: 4, cnt: 3
id: 3, cnt: 2
id: 3, cnt: 8
id: 4, cnt: 7
id: 4, cnt: 10
id: 3, cnt: 9
id: 4, cnt: 11
id: 3, cnt: 12
*/
