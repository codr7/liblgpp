#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <thread>

namespace lgpp {
  using namespace std;
  
  struct Thread {
    using Id = thread::id;

    template <typename T, typename...Args>
    void emit(Args&&...args) {
      ops.emplace_back(ops.size(), T(forward<Args>(args)...));
    }

    vector<Op> ops;
  };
}

#endif
