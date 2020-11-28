#ifndef LGPP_OPS_YIELD_HPP
#define LGPP_OPS_YIELD_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Yield {
    Yield(unsigned n): n(n) {}
    unsigned n;
  };

  template <>
  inline const Op *eval(const Op &op, const Yield &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    auto c = vm.thread().pop_coro();
    auto ret_pc = vm.pop_ret();
    
    if (c) {
      if (c->done) { throw runtime_error("Coro is done"); }
      move(c->stack.end(), c->stack.end()-imp.n, back_inserter(stack));
      c->pc = op.pc+1;
      push(stack, lgpp::types::Coro, *c);
    } else {
      push(stack, lgpp::types::Coro, Coro(op.pc+1));
    }
    
    return &op - op.pc + ret_pc;
  }

}

#endif
