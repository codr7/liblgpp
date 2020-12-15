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

  struct VM;
  
  template <typename...Args>
  Thread& start_thread(VM &vm, Args&&...args);

  Thread& get_thread(VM &vm, Thread::Id id = this_thread::get_id());

  const Thread& get_thread(const VM &vm, Thread::Id id = this_thread::get_id());

  struct VM {
    using shared_lock_t = shared_lock<shared_mutex>;
    using lock_t = unique_lock<shared_mutex>;

    VM() { start_thread(*this, this_thread::get_id()); }
        
    map<Thread::Id, Thread> threads;
    shared_mutex thread_mutex;
  };

  template <typename T, typename...Args>
  const T& emit(VM &vm, Args&&...args) { return emit<T, Args...>(get_thread(vm), forward<Args>(args)...); }
  
  inline PC emit_pc(const VM &vm) { return emit_pc(get_thread(vm)); }

  inline const Op& eval(VM &vm, const Op& start_op) { return eval(get_thread(vm), start_op); }

  inline const Op& eval(VM &vm, PC start_pc) { return eval(get_thread(vm), start_pc); }

  inline void push_ret(VM &vm, PC pc, Ret::Opts opts = Ret::Opts::NONE) { push_ret(get_thread(vm), pc, opts); }
  
  inline Ret pop_ret(VM &vm) { return pop_ret(get_thread(vm)); }
  
  template <typename...Args>
  Thread& start_thread(VM &vm, Args&&...args) {
    Thread t(vm, forward<Args>(args)...);
    VM::lock_t lock(vm.thread_mutex);
    return vm.threads.insert(make_pair(t.id, move(t))).first->second;
  }

  inline Thread& get_thread(VM &vm, Thread::Id id) {
    VM::shared_lock_t lock(vm.thread_mutex);
    auto found = vm.threads.find(id);
    if (found == vm.threads.end()) { throw runtime_error("Thread not found"); }
    return found->second;
  }

  inline const Thread& get_thread(const VM &vm, Thread::Id id) { return get_thread(const_cast<VM &>(vm), id); }

  inline Stack& get_stack(VM &vm) { return get_stack(get_thread(vm)); }

}

#endif
