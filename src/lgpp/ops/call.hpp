#ifndef LGPP_OPS_CALL_HPP
#define LGPP_OPS_CALL_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Call {
    Label &target;
    
    Call(Label &target): target(target) {}
  };

  template <>
  inline const Op *eval(const Op &op, const Call &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    vm.push_ret(op.pc + 1);
    return &op - op.pc + imp.target.pc;
  }

}

#endif
