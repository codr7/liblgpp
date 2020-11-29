#ifndef LGPP_OPS_JOIN_HPP
#define LGPP_OPS_JOIN_HPP

#include "../op.hpp"
#include "../types.hpp"
#include "../val.hpp"

namespace lgpp::ops {

  struct Join {};

  template <>
  inline const Op *eval(const Op &op, const Join &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    vm.join(pop(stack, lgpp::types::Thread), stack);
    return &op+1;
  }

}

#endif
