#ifndef LGPP_OPS_ZIP_HPP
#define LGPP_OPS_ZIP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Zip {};

  template <>
  inline const Op* eval(const Op& op, const Zip& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto y(pop(stack)), x(pop(stack));
    push(stack, lgpp::types::Pair, make_pair(x, y));
    return &op+1;
  }

}

#endif
