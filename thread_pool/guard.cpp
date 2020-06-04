#include "guard.h"

namespace kirai {
namespace thread {

ThreadsGuard::ThreadsGuard(std::vector<std::thread>& threads) :
                         threads_(threads) {}

ThreadsGuard::~ThreadsGuard() {
  for (auto& thread : threads_) {
    if (thread.joinable()) {
        thread.join();
    }
  }
}

} // namespace thread
} // namespace kirai
