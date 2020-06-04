#include "thread_pool.h"

namespace kirai {
namespace thread {

ThreadPool::ThreadPool(const int thread_pool_size) :
                       thread_pool_size_(thread_pool_size),
                       guard_(threads_),
                       stop_(false) {
  // No limit.
  if (thread_pool_size_ <= 0) {
    thread_pool_size_ = std::thread::hardware_concurrency();
    thread_pool_size_ = (thread_pool_size_ == 0 ? 1 : thread_pool_size_);
  }
  threads_.resize(thread_pool_size_);

  for (auto& thread : threads_) {
    thread = std::thread([&]() {
      while (!stop_.load(std::memory_order_acquire)) {
        Task task;
        std::unique_lock<std::mutex> ulk(mtx_);
        cond_.wait(ulk, [&]() {
          return stop_.load(std::memory_order_acquire) || tasks_.empty();
        });
        if (stop_.load(std::memory_order_acquire)) {
          return;
        }
        task = std::move(tasks_.front());
        tasks_.pop();
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  stop_.store(true, std::memory_order_release);
  cond_.notify_all();
  guard_.~ThreadsGuard();
}

template <class Function, class... Args>
std::future<typename std::result_of<Function(Args...)>::type> ThreadPool::Add(
    Function&& fcn, Args&&... args) {

  using Type = typename std::result_of<Function(Args...)>::type;
  using Task = std::packaged_task<Type()>;

  auto t = std::make_shared<Task>(std::bind(std::forward<Function>(fcn),
                                  std::forward<Args>(args)...));
  auto ret = t->get_future();
  std::lock_guard<std::mutex> lg(mtx_);
  if (stop_.load(std::memory_order_acquire)) {
    throw std::runtime_error("thread pool has stopped");
  }
  tasks_.emplace([t] { (*t)(); });
  cond_.notify_one();
  return ret;
}

} // namespace thread
} // namespace kirai
