#ifndef LGPP_OPS_JMP_HPP
#define LGPP_OPS_JMP_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Jmp {
    Jmp(Label &target): target(target) {}
    Label &target;
  };

  template <>
  inline const Op *eval(const Op &op, const Jmp &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    return &op - op.pc + *imp.target.pc;
  }

}

#endif
