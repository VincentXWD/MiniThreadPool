#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>

#include "guard.h"


namespace kirai {
namespace thread {

class ThreadPool {
 public:
  using Task = std::function<void()>;

  explicit ThreadPool(const int thread_pool_size);

  ~ThreadPool();

  template <class Function, class... Args>
  std::future<typename std::result_of<Function(Args...)>::type>
  Add(Function&& fcn, Args&&... args);

 private:
  // Those constructors are not allowed.
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;

  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

 private:
  int thread_pool_size_;

  std::mutex mtx_;
  std::atomic<bool> stop_;
  std::condition_variable cond_;

  ThreadsGuard guard_;
  std::vector<std::thread> threads_;
  std::queue<Task> tasks_;
};

} // namespace thread
} // namespace kirai


#endif // THREAD_POOL_H
