// based on https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h

#ifndef SILICE3D_COMMON_THREAD_POOL_H
#define SILICE3D_COMMON_THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace std {

template<>
struct less<std::pair<int, std::function<void()>>> {
  constexpr bool operator()(const std::pair<int, std::function<void()>>& lhs,
                            const std::pair<int, std::function<void()>>& rhs) {
    return lhs.first < rhs.first;
  }
};

}

namespace Silice3D {

class ThreadPool {
public:
    ThreadPool(size_t);
    ~ThreadPool();

    void Enqueue(int priority, const std::function<void()>& task);
    void Clear();

private:
    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::priority_queue<std::pair<int, std::function<void()>>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable task_to_do;
    bool stop;
};

}

#endif
