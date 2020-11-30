#ifndef LGPP_OPS_SUB_HPP
#define LGPP_OPS_SUB_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Sub {};

  template <>
  inline const Op* eval(const Op& op, const Sub& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    Val y(pop(stack)), x(pop(stack));
    push(stack, x - y);
    return &op+1;
  }

}

#endif
