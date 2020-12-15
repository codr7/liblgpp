#ifndef LGPP_OPS_SUB_HPP
#define LGPP_OPS_SUB_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Sub {};

  template <>
  inline const Op* eval(const Op& op, const Sub& imp, Thread& thread) {
    auto& s = get_stack(thread);
    Val y(pop(s)), x(pop(s));
    push(s, x - y);
    return &op+1;
  }

}

#endif
