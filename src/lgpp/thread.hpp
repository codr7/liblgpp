#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <thread>

#include "op.hpp"
#include "pc.hpp"
#include "stack.hpp"

namespace lgpp {
  using namespace std;
  
  struct Thread {
    using Id = thread::id;
    
    Thread(Id id): id(id) {}

    Thread(const Thread &owner, function<void ()> body): ops(owner.ops), imp(body), id(imp.get_id()) {}

    template <typename T, typename...Args>
    const T& emit(Args&&...args) { return ops.emplace_back(ops.size(), T(forward<Args>(args)...)).template as<T>(); }

    PC emit_pc() const { return ops.size(); }
    
    void push_ret(PC pc) { ret.push_back(pc); }

    PC pop_ret() {
      if (!ret.size()) { throw runtime_error("Return stack is empty"); }
      PC pc = ret.back();
      ret.pop_back();
      return pc;
    }

    void join() {
      if (imp.joinable()) { imp.join(); }
    }
    
    vector<Op> ops;
    vector<PC> ret;
    Stack stack;
    thread imp;
    const Id id;
  };
}

#endif
