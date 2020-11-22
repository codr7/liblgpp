#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <thread>

#include "pc.hpp"

namespace lgpp {
  using namespace std;
  
  struct Thread {
    using Id = thread::id;

    template <typename T, typename...Args>
    const T& emit(Args&&...args) { return ops.emplace_back(ops.size(), T(forward<Args>(args)...)).template as<T>(); }

    void push_ret(PC pc) { ret.push_back(pc); }

    PC pop_ret() {
      if (!ret.size()) { throw runtime_error("Return stack is empty"); }
      PC pc = ret.back();
      ret.pop_back();
      return pc;
    }
    
    vector<Op> ops;
    vector<PC> ret;
  };
}

#endif
