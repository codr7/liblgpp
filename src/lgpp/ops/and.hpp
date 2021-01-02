#ifndef LGPP_OPS_AND_HPP
#define LGPP_OPS_AND_HPP

#include "../val.hpp"
#include "../op.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct And {};

  template <>
  inline const Op* eval(const Op& op, const And& imp, Thread& thread) {
    Stack& s = get_stack(thread);
    Val y = pop(s), x = pop(s);
    push(s, is_true(x) ? y : x);
    return &op+1;
  }

}

#endif
