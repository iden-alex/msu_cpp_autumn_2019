#ifndef POOL_THR
#define POOL_THR

#include <thread>
#include <pthread.h>
#include <future>
#include <queue>
#include <vector>
#include <functional>

class ThreadPool
{
	bool f;
	std::mutex mut;
	std::vector <std::thread> threads;
	std::queue <std::function<void()>> tasks;
	std::condition_variable condition_; 

public:
    explicit ThreadPool(size_t poolSize): f(true) {
    	for (size_t i = 0; i < poolSize; ++i) {
    		threads.emplace_back(
    			[this]() {
    					while(f) {
		    				std::unique_lock <std::mutex> lock(mut);
				            while (f && tasks.empty()) {
				            	condition_.wait(lock);
				            }
				            if (!f) {
				                break;
				            }
				            auto task = tasks.front();
				            tasks.pop();
				            task();
				        }
		    	}
		    );
    	}
    }
    

    ~ThreadPool() {
    	f = false;
    	condition_.notify_all();
    	for (auto &thr: threads) {
    		thr.join();
    	}
    }


    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
    	auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
            (std::bind(func, args...));
        std::unique_lock<std::mutex> lock(mut);
        tasks.push([task]() { (*task)(); });
        condition_.notify_one();
        return task->get_future();
    }
};

#endif