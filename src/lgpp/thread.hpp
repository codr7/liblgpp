#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <thread>

namespace lgpp {
  using namespace std;
  
  struct Thread {
    using Id = thread::id;

    template <typename T, typename...Args>
    const T& emit(Args&&...args) { return ops.emplace_back(ops.size(), T(forward<Args>(args)...)).template as<T>(); }

    vector<Op> ops;
  };
}

#endif
