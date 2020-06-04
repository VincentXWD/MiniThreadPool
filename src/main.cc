#include <iostream>
#include <functional>
#include <vector>

#include "thread_pool/thread_pool.h"

int main(int argc, char **argv) {
  std::function<int(int, int)> add = [](int x, int y) {
    return x + y;
  };
  kirai::thread::ThreadPool thread_pool(10);

  std::vector<int> ret;
  for (int i = 0; i < 10; i++) {
    auto tmp = thread_pool.Add([](int answer) { return answer; }, i);
    ret.emplace_back(std::move(tmp));
  }

  return 0;
}
