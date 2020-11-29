#ifndef LGPP_OPS_PAUSE_HPP
#define LGPP_OPS_PAUSE_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Pause {};

  template <>
  inline const Op* eval(const Op& op, const Pause& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    auto c = vm.thread().pop_coro();
    auto ret = vm.pop_ret();
    if (!c) { runtime_error("Missing coro"); }
    if (c->done) { throw runtime_error("Coro is done"); }
    if (!((int)ret.opts & (int)lgpp::Ret::Opts::CORO)) { throw runtime_error("Pause outside of coro"); }
    c->pc = op.pc+1;
    push(stack, lgpp::types::Coro, *c);
    return &op - op.pc + ret.pc;
  }

}

#endif
