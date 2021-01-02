#ifndef LGPP_OPS_EQ_HPP
#define LGPP_OPS_EQ_HPP

#include "../val.hpp"
#include "../op.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Eq {};

  template <>
  inline const Op* eval(const Op& op, const Eq& imp, Thread& thread) {
    Stack& s = get_stack(thread);
    Val y = pop(s), x = pop(s);
    push(s, thread.vm.Bool, x == y);
    return &op+1;
  }

}

#endif
