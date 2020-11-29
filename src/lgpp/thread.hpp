#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <optional>
#include <thread>

#include "coro.hpp"
#include "op.hpp"
#include "ret.hpp"
#include "stack.hpp"

namespace lgpp {
  
  using namespace std;
  
  struct Thread {
    using Id = thread::id;
    
    Thread(Id id): id(id) {}

    Thread(const Thread &owner, function<void ()> body): ops(owner.ops), imp(body), id(imp.get_id()) {}
    
    void push_ret(PC pc, Ret::Opts opts = Ret::Opts::NONE) { rets.emplace_back(pc, opts); }

    Ret pop_ret() {
      if (!rets.size()) { throw runtime_error("Ret stack is empty"); }
      auto r = rets.back();
      rets.pop_back();
      return r;
    }

    void join() {
      if (imp.joinable()) { imp.join(); }
    }

    void push_coro(Coro coro) { coros.emplace_back(coro); }
      
    optional<Coro> pop_coro() {
      if (coros.empty()) { return nullopt; }
      auto c(coros.back());
      coros.pop_back();
      return c;
    }
    
    vector<Op> ops;
    vector<Ret> rets;
    Stack stack;
    thread imp;
    const Id id;
    vector<Coro> coros;
  };

  template <typename T, typename...Args>
  const T& emit(Thread &thread, Args&&...args) {
    return thread.ops.emplace_back(thread.ops.size(), T(forward<Args>(args)...)).template as<T>();
  }
  
  inline PC emit_pc(const Thread &t) { return t.ops.size(); }

}

#endif
