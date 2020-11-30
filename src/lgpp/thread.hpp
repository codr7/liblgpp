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

  struct VM;
  
  struct Thread {
    using Id = thread::id;
    
    Thread(VM &vm, Id id): vm(vm), id(id) {}

    Thread(VM &vm, const Thread &owner, function<void ()> body):
      vm(vm), ops(owner.ops), stack(owner.stack), imp(body), id(imp.get_id()) {}
    
    VM &vm;
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

  inline const Op& eval(Thread &thread, const Op& start_op, Stack &stack) {
    const Op* pop = nullptr;
    for (const Op* op = &start_op; op; pop = op, op = op->eval(thread, stack));
    return *pop;
  }

  inline const Op& eval(Thread &thread, PC start_pc, Stack& stack) {
    return eval(thread, *(thread.ops.data()+start_pc), stack);
  }

  inline void push_coro(Thread &thread, Coro coro) { thread.coros.emplace_back(coro); }
      
  inline optional<Coro> pop_coro(Thread &thread) {
    if (thread.coros.empty()) { return nullopt; }
    auto c(thread.coros.back());
    thread.coros.pop_back();
    return c;
  }
  
  inline void push_ret(Thread &thread, PC pc, Ret::Opts opts = Ret::Opts::NONE) { thread.rets.emplace_back(pc, opts); }
  
  inline Ret pop_ret(Thread &thread) {
    if (!thread.rets.size()) { throw runtime_error("Ret stack is empty"); }
    auto r = thread.rets.back();
    thread.rets.pop_back();
    return r;
  }
}

#endif
