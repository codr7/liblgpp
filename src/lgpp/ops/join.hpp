#ifndef LGPP_OPS_JOIN_HPP
#define LGPP_OPS_JOIN_HPP

#include "../op.hpp"
#include "../types.hpp"
#include "../val.hpp"

namespace lgpp::ops {

  struct Join {};

  template <>
  inline const Op* eval(const Op& op, const Join& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto tid = pop(stack, lgpp::types::Thread);
    auto &vm(thread.vm);
    Thread& t = vm.thread(tid);

    t.imp.join();
    push(stack, types::Stack, t.stack);
    
    VM::lock_t lock(vm.thread_mutex);
    auto found = vm.threads.find(tid);
    if (found == vm.threads.end()) { throw runtime_error("Thread not found"); }
    vm.threads.erase(found);

    return &op+1;
  }

}

#endif
