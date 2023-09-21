 
// file:	thread_pool.cpp
//
// summary:	定长n线程池。n个线程，一个任务队列
/**************************************************************************************************
C++11中的std::future是一个模板类。
std::future提供了一种用于访问异步操作结果的机制。
std::future所引用的共享状态不能与任何其它异步返回的对象共享(与std::shared_future相反)
一个future是一个对象，它可以从某个提供者的对象或函数中检索值,如果在不同的线程中，则它可以正确地同步此访问
 **************************************************************************************************/
 

 
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>

using namespace std;
 
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args> //返回一个基于F的函数对象，其参数被绑定到Args上。
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
	int get_workers_num()
	{
		return workers.size();
	}
	queue< std::function<void()> > get_tasks()
	{
		return tasks;
	}
	bool get_stop()
	{
		return stop;
	}
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // 存放task的阻塞队列
    std::queue< std::function<void()> > tasks;
    
    // synchronization 同步互斥
    std::mutex queue_mutex; //锁
    std::condition_variable condition; //条件变量
    bool stop;
};
 
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads): stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this] {
                std::function<void()> task;
                for(;;) {
 
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
 
                    task();
                }
            }
        );
}
 
// Add new task item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
 
    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
 
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
 
        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
 
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}
 
// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
 



int st(int i) //线程函数
{
	printf("hello world %d\n", i);
	// std::cout << "hello world " << i << std::endl;
	// std::this_thread::sleep_for(std::chrono::seconds(1));
	return i * i + 10;
}
 
int main()
{
	ThreadPool pool(4); //池中创建四个线程
	std::vector< std::future<int> > results; //std::future 提供访问异步操作结果的机制
 
    pool.enqueue(st, 1);
	pool.enqueue(st, 2);
 
	for (int i = 0; i < 5; ++i) {
		results.emplace_back(
			pool.enqueue([i](){ return st(i); })
		);
	}

    printf("results: \n");
    // std::cout << "results: " << std::endl;
	for (auto && result : results)
		std::cout << result.get() << ' ';
	std::cout << std::endl;
 
 
	return 0;
}
