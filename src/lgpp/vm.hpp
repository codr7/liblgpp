#ifndef LGPP_VM_HPP
#define LGPP_VM_HPP

#include <map>
#include <shared_mutex>
#include <vector>

#include "op.hpp"
#include "stack.hpp"
#include "thread.hpp"
#include "types.hpp"

namespace lgpp {

  using namespace std;

  struct VM {
    using shared_lock_t = shared_lock<shared_mutex>;
    using lock_t = unique_lock<shared_mutex>;

    VM() { start_thread(this_thread::get_id()); }
        
    void clear_ops() { thread().ops.clear(); }

    Thread& thread(Thread::Id id = this_thread::get_id()) {
      shared_lock_t lock(thread_mutex);
      auto found = threads.find(id);
      if (found == threads.end()) { throw runtime_error("Thread not found"); }
      return found->second;
    }

    const Thread& thread() const { return const_cast<VM*>(this)->thread(); }

    void push_ret(PC pc, Ret::Opts opts = Ret::Opts::NONE) { thread().push_ret(pc, opts); }
    
    Ret pop_ret() { return thread().pop_ret(); }

    template <typename...Args>
    Thread& start_thread(Args&&...args) {
      Thread t(*this, forward<Args>(args)...);
      lock_t lock(thread_mutex);
      return threads.insert(make_pair(t.id, move(t))).first->second;
    }

    map<Thread::Id, Thread> threads;
    shared_mutex thread_mutex;
  };

  inline const Op& eval(VM &vm, const Op& start_op, Stack &stack) { return eval(vm.thread(), start_op, stack); }

  inline const Op& eval(VM &vm, PC start_pc, Stack& stack) { return eval(vm.thread(), start_pc, stack); }

  template <typename T, typename...Args>
  const T& emit(VM &vm, Args&&...args) { return emit<T, Args...>(vm.thread(), forward<Args>(args)...); }
  
  inline PC emit_pc(const VM &vm) { return emit_pc(vm.thread()); }
}

#endif
