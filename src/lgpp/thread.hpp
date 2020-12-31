#ifndef LGPP_THREAD_HPP
#define LGPP_THREAD_HPP

#include <deque>
#include <list>
#include <optional>
#include <thread>

#include "lgpp/call.hpp"
#include "lgpp/coro.hpp"
#include "lgpp/error.hpp"
#include "lgpp/label.hpp"
#include "lgpp/op.hpp"
#include "lgpp/stack.hpp"

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
    list<Label> labels;
    vector<Op> ops;
    vector<Call> calls;
    deque<Stack> stacks;
    thread imp;
    const Id id;
    vector<Coro> coros;
  };

  template <typename...Args>
  Label &push_label(Thread &thread, Args &&...args) { return thread.labels.emplace_back(forward<Args>(args)...); }
  
  template <typename T, typename...Args>
  const T& emit(Thread& thread, Pos pos, Args&&...args) {
    return thread.ops.emplace_back(thread.ops.size(), pos, T(forward<Args>(args)...)).template as<T>();
  }

  template <typename T, typename...Args>
  const T& emit(Thread& thread, Args&&...args) { return emit<T>(thread, Pos("n/a"), forward<Args>(args)...); }

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

  template <typename...Args>
  void push_call(Thread& thread, Args&&...args) { thread.calls.emplace_back(forward<Args>(args)...); }
  
  inline Call pop_call(Thread& thread) {
    if (thread.calls.empty()) { throw runtime_error("Call stack is empty"); }
    auto c = thread.calls.back();
    thread.calls.pop_back();
    return c;
  }

  inline PC resume(const Coro &coro, Thread& thread, PC return_pc, Pos pos) {
    if (coro.done) { throw ERun(pos, "Coro is done"); }
    push_coro(thread, coro);
    push_call(thread, return_pc, Call::Opts::CORO);
    return coro.pc;
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
