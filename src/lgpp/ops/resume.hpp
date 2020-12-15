#ifndef LGPP_OPS_RESUME_HPP
#define LGPP_OPS_RESUME_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Resume {};

  template <>
  inline const Op* eval(const Op& op, const Resume& imp, Thread& thread) {
    auto c = pop(get_stack(thread), types::Coro);
    if (c.done) { throw runtime_error("Coro is done"); }
    push_coro(thread, c);
    push_ret(thread, op.pc+1, Ret::Opts::CORO);
    return &op - op.pc + c.pc;
  }

}

#endif
