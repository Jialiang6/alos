#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>
#include <typeinfo>

using namespace std;
typedef unsigned long const ulc;
const int debug = 0;

template<typename Iter, typename T>
struct accumulate_block {
    void operator()(Iter begin, Iter end, T& sum) {
        sum = accumulate(begin, end, sum);
    }
};

template<typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init) {
    ulc length = distance(begin, end);
    if (length == 0)
        return init;
    
    ulc min_per_thread = 25;
    ulc max_threads = (length + min_per_thread - 1) / min_per_thread;
    if (debug) cout << "max_threads: " << max_threads << endl;
    ulc hardware_threads = thread::hardware_concurrency();
    if (debug) cout << "hardware_threads: " << hardware_threads << endl;
    ulc num_threads = min(max_threads, hardware_threads != 0 ? hardware_threads : 2);
    if (debug) cout << "num_threads: " << num_threads << endl;
    ulc block_size = length / num_threads;
    if (debug) cout << "block_size: " << block_size << endl;

    vector<T> results(num_threads);
    vector<thread> threads(num_threads - 1);

    Iter block_begin = begin;
    for (unsigned long i = 0; i < (num_threads - 1); i++) {
        Iter block_end = block_begin + block_size;
        // advance(block_end, block_size);
        threads[i] = thread(accumulate_block<Iter, T>(), 
                            block_begin, block_end, ref(results[i]));
        // 到thread()的args的类型如果希望在函数中是左值引用，得加ref()显示说明，否则报错
        // 因为传入thread()的arg要不被移动为右值引用，要不按值复制
        block_begin = block_end;
    }
    accumulate_block<Iter, T>()(block_begin, end, results[num_threads - 1]);
    for_each(threads.begin(), threads.end(), [](thread& th){th.join();});

    return accumulate(results.begin(), results.end(), init);
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();
    vector<int> nums(100000000, 1);

    unsigned long long res = 0;
    cout << "The result is: " << endl;
    cout << parallel_accumulate(nums.begin(), nums.end(), res) << endl; // parallel
    // cout << accumulate(nums.begin(), nums.end(), res) << endl;       // normal

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "duration time is: " << duration.count() << " ms" << endl;
    return 0;
}