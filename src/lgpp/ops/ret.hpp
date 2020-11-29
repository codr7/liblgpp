#ifndef LGPP_OPS_RET_HPP
#define LGPP_OPS_RET_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Ret {};

  template <>
  inline const Op* eval(const Op& op, const Ret& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    auto ret = vm.pop_ret();
    
    if ((int)ret.opts & (int)lgpp::Ret::Opts::CORO) {
      auto c = vm.thread().pop_coro();
      if (!c) { throw runtime_error("Missing coro"); }
      if (c->done) { throw runtime_error("Coro is done"); }
      c->pc = op.pc;
      c->done = true;
      push(stack, lgpp::types::Coro, *c);
    }
    
    return &op - op.pc + ret.pc;
  }

}

#endif
