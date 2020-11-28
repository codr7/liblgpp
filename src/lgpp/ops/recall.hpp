#ifndef LGPP_OPS_RECALL_HPP
#define LGPP_OPS_RECALL_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Recall {};

  template <>
  inline const Op *eval(const Op &op, const Recall &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    auto c = pop(stack).as(lgpp::types::Coro);
    if (c.done) { throw runtime_error("Coro is done"); }
    vm.thread().push_coro(c);
    vm.push_ret(op.pc+1);
    return &op - op.pc + c.pc;
  }

}

#endif
