#ifndef LGPP_OPS_SPAWN_HPP
#define LGPP_OPS_SPAWN_HPP

#include "../op.hpp"
#include "../types.hpp"
#include "../val.hpp"

namespace lgpp::ops {

  struct Spawn {
    Spawn(Label &target): target(target) {}
    Label &target;
  };

  template <>
  inline const Op *eval(const Op &op, const Spawn &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    Thread &t = vm.spawn(vm.thread(), [&vm, &imp]() {
      vm.eval(imp.target.pc, vm.thread().stack);
    });

    push(stack, Val(lgpp::types::Thread, t.id));
    return &op+1;
  }

}

#endif
