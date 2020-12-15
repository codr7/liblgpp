#ifndef LGPP_OPS_START_THREAD_HPP
#define LGPP_OPS_START_THREAD_HPP

#include "../op.hpp"
#include "../types.hpp"
#include "../val.hpp"

namespace lgpp::ops {

  struct StartThread {
    StartThread(Label& target): target(target) {}
    Label& target;
  };

  template <>
  inline const Op* eval(const Op& op, const StartThread& imp, Thread& thread) {
    auto &vm(thread.vm);
    auto& t = start_thread(vm, thread, [&vm, &imp]() { eval(vm, *imp.target.pc); });
    push(get_stack(thread), Val(types::Thread, t.id));
    return &op+1;
  }

}

#endif
