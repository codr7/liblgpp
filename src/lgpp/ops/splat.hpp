#ifndef LGPP_OPS_SPLAT_HPP
#define LGPP_OPS_SPLAT_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Splat {};

  template <>
  inline const Op* eval(const Op& op, const Splat& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto s = pop(stack, types::Stack);
    copy(s.begin(), s.end(), back_inserter(stack));
    return &op+1;
  }

}

#endif
