#ifndef LGPP_VM_HPP
#define LGPP_VM_HPP

#include <map>
#include <mutex>
#include <vector>

#include "op.hpp"
#include "stack.hpp"
#include "thread.hpp"

namespace lgpp {

  using namespace std;

  struct VM {
    using lock_t = lock_guard<mutex>;

    VM() { threads.insert(make_pair(this_thread::get_id(), Thread())); }
    
    template <typename T, typename...Args>
    const T& emit(Args&&...args) { return thread().emit<T, Args...>(forward<Args>(args)...); }

    const Op &last_op() const { return thread().ops.back(); }
    void clear_ops() { thread().ops.clear(); }

    const Op &eval(PC start_pc, Stack &stack) { return eval(*(thread().ops.data()+start_pc), stack); }
  
    const Op &eval(const Op &start_op, Stack &stack) {
      const Op *pop = nullptr;
      for (const Op *op = &start_op; op; pop = op, op = op->eval(stack));
      return *pop;    
    }

    Thread &thread() {
      lock_t lock(thread_mutex);
      auto found = threads.find(this_thread::get_id());
      if (found == threads.end()) { throw runtime_error("Thread not found"); }
      return found->second;
    }

    const Thread &thread() const { return const_cast<VM *>(this)->thread(); }

    map<Thread::Id, Thread> threads;
    mutex thread_mutex;
  };

}

#endif
