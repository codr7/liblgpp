#ifndef LGPP_OPS_RESUME_HPP
#define LGPP_OPS_RESUME_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Resume {};

  template <>
  inline const Op* eval(const Op& op, const Resume& imp, Thread& thread) {
    auto c = pop(get_stack(thread), thread.vm.Coro);
    return &op - op.pc + resume(c, thread, op.pc+1, op.pos);
  }

}

#endif
