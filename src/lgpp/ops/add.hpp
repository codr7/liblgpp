#ifndef LGPP_OPS_ADD_HPP
#define LGPP_OPS_ADD_HPP

#include "../op.hpp"
#include "../thread.hpp"

namespace lgpp::ops {

  struct Add {};

  template <>
  inline const Op* eval(const Op& op, const Add& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto y(pop(s)), x(pop(s));
    push(s, x+y);
    return &op+1;
  }

}

#endif
