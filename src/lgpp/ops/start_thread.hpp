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
  inline const Op* eval(const Op& op, const StartThread& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    auto& t = vm.start_thread(vm.thread(), [&vm, &imp]() {
      vm.eval(*imp.target.pc, vm.thread().stack);
    });

    push(stack, Val(lgpp::types::Thread, t.id));
    return &op+1;
  }

}

#endif
