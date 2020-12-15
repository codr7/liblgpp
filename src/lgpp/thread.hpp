#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <deque>
#include <optional>
#include <thread>

#include "coro.hpp"
#include "op.hpp"
#include "ret.hpp"
#include "stack.hpp"

namespace lgpp {
  
  using namespace std;

  struct VM;
  
  Stack& get_stack(Thread& thread);
  Stack& push_stack(Thread& thread);

  struct Thread {
    using Id = thread::id;
    
    Thread(VM& vm, Id id): vm(vm), id(id) {}

    Thread(VM& vm, const Thread& owner, function<void ()> body):
      vm(vm), ops(owner.ops), imp(body), id(imp.get_id()) {
      if (!owner.stacks.empty()) { push_stack(*this) = owner.stacks.back(); }
    }
    
    VM &vm;
    vector<Op> ops;
    vector<Ret> rets;
    deque<Stack> stacks;
    thread imp;
    const Id id;
    vector<Coro> coros;
  };

  template <typename T, typename...Args>
  const T& emit(Thread& thread, Args&&...args) {
    return thread.ops.emplace_back(thread.ops.size(), T(forward<Args>(args)...)).template as<T>();
  }
  
  inline PC emit_pc(const Thread& t) { return t.ops.size(); }

  inline const Op& eval(Thread& thread, const Op& start_op) {
    const Op* pop = nullptr;
    for (const Op* op = &start_op; op; pop = op, op = eval(*op, thread));
    return *pop;
  }

  inline const Op& eval(Thread& thread, PC start_pc) { return eval(thread, *(thread.ops.data()+start_pc)); }

  inline void push_coro(Thread& thread, Coro coro) { thread.coros.emplace_back(coro); }
      
  inline optional<Coro> pop_coro(Thread& thread) {
    if (thread.coros.empty()) { return nullopt; }
    auto c(thread.coros.back());
    thread.coros.pop_back();
    return c;
  }
  
  inline void push_ret(Thread& thread, PC pc, Ret::Opts opts = Ret::Opts::NONE) { thread.rets.emplace_back(pc, opts); }
  
  inline Ret pop_ret(Thread& thread) {
    if (thread.rets.empty()) { throw runtime_error("Ret stack is empty"); }
    auto r = thread.rets.back();
    thread.rets.pop_back();
    return r;
  }

  inline Stack& get_stack(Thread& thread) {
    if (thread.stacks.empty()) { return push_stack(thread); }
    return thread.stacks.back();
  }

  inline Stack& push_stack(Thread& thread) {
    thread.stacks.emplace_back();
    return thread.stacks.back();
  }

  inline Stack pop_stack(Thread& thread) {
    if (thread.stacks.empty()) { throw runtime_error("No stack found"); }
    auto s = thread.stacks.back();
    thread.stacks.pop_back();
    return s;
  }
}

#endif
