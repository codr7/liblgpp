#ifndef LGPP_OPS_OR_HPP
#define LGPP_OPS_OR_HPP

#include "../val.hpp"
#include "../op.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Or {};

  template <>
  inline const Op* eval(const Op& op, const Or& imp, Thread& thread) {
    Stack& s = get_stack(thread);
    Val y = pop(s), x = pop(s);
    push(s, is_true(x) ? x : y);
    return &op+1;
  }

}

#endif
