#ifndef LGPP_VM_HPP
#define LGPP_VM_HPP

#include <map>
#include <shared_mutex>
#include <vector>

#include "op.hpp"
#include "stack.hpp"
#include "thread.hpp"

namespace lgpp {

  using namespace std;

  struct VM {
    using shared_lock_t = shared_lock<shared_mutex>;

    VM() { threads.insert(make_pair(this_thread::get_id(), Thread())); }
    
    template <typename T, typename...Args>
    const T& emit(Args&&...args) { return thread().emit<T, Args...>(forward<Args>(args)...); }

    PC emit_pc() const { return thread().emit_pc(); }
    
    const Op &last_op() const { return thread().ops.back(); }
    
    void clear_ops() { thread().ops.clear(); }

    const Op &eval(PC start_pc, Stack &stack) { return eval(*(thread().ops.data()+start_pc), stack); }
  
    const Op &eval(const Op &start_op, Stack &stack) {
      const Op *pop = nullptr;
      for (const Op *op = &start_op; op; pop = op, op = op->eval(*this, stack));
      return *pop;    
    }

    Thread &thread() {
      shared_lock_t lock(thread_mutex);
      auto found = threads.find(this_thread::get_id());
      if (found == threads.end()) { throw runtime_error("Thread not found"); }
      return found->second;
    }

    const Thread &thread() const { return const_cast<VM *>(this)->thread(); }

    void push_ret(PC pc) { thread().push_ret(pc); }
    
    PC pop_ret() { return thread().pop_ret(); }

    map<Thread::Id, Thread> threads;
    shared_mutex thread_mutex;
  };

}

#endif
