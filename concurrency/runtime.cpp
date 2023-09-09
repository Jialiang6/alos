#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;
typedef unsigned long const ulc;

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
    cout << "max_threads: " << max_threads << endl;
    ulc hardware_threads = thread::hardware_concurrency();
    cout << "hardware_threads: " << hardware_threads << endl;
    ulc num_threads = min(max_threads, hardware_threads != 0 ? hardware_threads : 2);
    cout << "num_threads: " << num_threads << endl;
    ulc block_size = length / num_threads;
    cout << "block_size: " << block_size << endl;

    vector<T> results(num_threads);
    vector<thread> threads(num_threads - 1);

    Iter block_begin = begin;
    for (unsigned long i = 0; i < (num_threads - 1); i++) {
        Iter block_end = block_begin;
        advance(block_end, block_size);
        threads[i] = thread(accumulate_block<Iter, T>(), block_begin, block_end, ref(results[i]));
        block_begin = block_end;
    }
    accumulate_block<Iter, T>()(block_begin, end, results[num_threads - 1]);
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

    return accumulate(results.begin(), results.end(), init);
}

int main() {
    vector<int> nums{3,4,5,6};
    int res = 0;
    cout << "The result is: " << endl;
    cout << parallel_accumulate(nums.begin(), nums.end(), res);
    return 0;
}