#ifndef LGPP_OPS_RET_HPP
#define LGPP_OPS_RET_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Ret {};

  template <>
  inline const Op *eval(const Op &op, const Ret &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    return &op - op.pc + vm.pop_ret();
  }

}

#endif
