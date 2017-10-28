// Copyright (c) Tamas Csala

#include <Silice3D/common/thread_pool.hpp>

namespace Silice3D {

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for(size_t i = 0; i<threads; ++i) {
    workers.emplace_back([this] {
      for(;;) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->task_to_do.wait(lock, [&] { return this->stop || !this->tasks.empty(); });
            if (this->stop) {
              return;
            }
            task = std::move(this->tasks.top().second);
            this->tasks.pop();
        }

        task();
      }
    });
  }
}

// the destructor joins all threads
ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  task_to_do.notify_all();
  for (std::thread &worker: workers) {
    worker.join();
  }
}

// add new work item to the pool
void ThreadPool::Enqueue(int priority, const std::function<void()>& task) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if (stop) {
      throw std::runtime_error("Enqueue on stopped ThreadPool");
    }

    tasks.push(std::make_pair(priority, task));
  }
  task_to_do.notify_one();
}

void ThreadPool::Clear() {
  std::unique_lock<std::mutex> lock(queue_mutex);
  while (!tasks.empty()) {
    tasks.pop();
  }
}

}
