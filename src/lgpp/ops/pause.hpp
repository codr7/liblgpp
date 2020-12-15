#ifndef LGPP_OPS_PAUSE_HPP
#define LGPP_OPS_PAUSE_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Pause {};

  template <>
  inline const Op* eval(const Op& op, const Pause& imp, Thread& thread) {
    auto c = pop_coro(thread);
    auto ret = pop_ret(thread);
    if (!c) { runtime_error("Missing coro"); }
    if (c->done) { throw runtime_error("Coro is done"); }
    if (!((int)ret.opts & (int)lgpp::Ret::Opts::CORO)) { throw runtime_error("Pause outside of coro"); }
    c->pc = op.pc+1;
    push(get_stack(thread), types::Coro, *c);
    return &op - op.pc + ret.pc;
  }

}

#endif
