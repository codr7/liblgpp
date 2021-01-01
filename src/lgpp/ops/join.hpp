#ifndef LGPP_OPS_JOIN_HPP
#define LGPP_OPS_JOIN_HPP

#include "../op.hpp"
#include "../val.hpp"

namespace lgpp::ops {

  struct Join {};

  template <>
  inline const Op* eval(const Op& op, const Join& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto tid = pop(s, thread.vm.Thread);
    auto &vm(thread.vm);
    Thread& t = get_thread(vm, tid);

    if (!t.imp.joinable()) { throw runtime_error("Cannot join main thread"); }
    t.imp.join();
    push(s, thread.vm.Stack, get_stack(t));
    
    VM::lock_t lock(vm.thread_mutex);
    auto found = vm.threads.find(tid);
    if (found == vm.threads.end()) { throw runtime_error("Thread not found"); }
    vm.threads.erase(found);

    return &op+1;
  }

}

#endif
