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
    if (!c) { runtime_error("Missing coro"); }
    auto call = pop_call(thread);
    assert(!c->done);
    if (!((int)call.opts & (int)lgpp::Call::Opts::CORO)) { throw runtime_error("Pause outside of coro"); }
    c->pc = op.pc+1;
    push(get_stack(thread), thread.vm.Coro, *c);
    return &op - op.pc + call.return_pc;
  }

}

#endif
