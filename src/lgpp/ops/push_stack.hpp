#ifndef LGPP_OPS_PUSH_STACK_HPP
#define LGPP_OPS_PUSH_STACK_HPP

#include "../op.hpp"

namespace lgpp::ops {

  struct PushStack {};

  template <>
  inline const Op* eval(const Op& op, const PushStack& imp, Thread& thread) {
    push_stack(thread);
    return &op+1;
  }

}

#endif
