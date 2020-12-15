#ifndef LGPP_OPS_POP_STACK_HPP
#define LGPP_OPS_POP_STACK_HPP

#include "../op.hpp"

namespace lgpp::ops {

  struct PopStack {};

  template <>
  inline const Op* eval(const Op& op, const PopStack& imp, Thread& thread) {
    auto v = pop_stack(thread);
    push(get_stack(thread), types::Stack, v);
    return &op+1;
  }

}

#endif
