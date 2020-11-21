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
    void emit(Args&&...args) {
      thread().emit<T, Args...>(forward<Args>(args)...);
    }

    const Op &last_op() const { return thread().ops.back(); }
      
    const Op &eval(PC start_pc, Stack &stack);
    const Op &eval(const Op &start_op, Stack &stack);

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
