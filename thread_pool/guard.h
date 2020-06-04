#ifndef GUARD_H
#define GUARD_H

#include <vector>
#include <thread>

namespace kirai {
namespace thread {

class ThreadsGuard {
 public:
  ThreadsGuard(std::vector<std::thread>& threads);

  /**
   *  @brief: Deconstructor of ThreadsGuard. Make sure we can call join() for
   *          all threads.
   */
  ~ThreadsGuard();

 private:
  // Those constructors are not allowed.
  ThreadsGuard(const std::vector<std::thread>&) = delete;
  ThreadsGuard(std::vector<std::thread>&&) = delete;

  ThreadsGuard& operator=(const ThreadsGuard&) = delete;
  ThreadsGuard& operator=(ThreadsGuard&&) = delete;

 private:
  std::vector<std::thread> &threads_;
};

} // namespace thread
} // namespace kirai


#endif // GUARD_H
