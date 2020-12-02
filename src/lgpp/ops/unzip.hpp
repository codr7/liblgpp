#ifndef LGPP_OPS_UNZIP_HPP
#define LGPP_OPS_UNZIP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Unzip {};

  template <>
  inline const Op* eval(const Op& op, const Unzip& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto p(pop(stack).as(lgpp::types::Pair));
    push(stack, p.first);
    push(stack, p.second);
    return &op+1;
  }

}

#endif
