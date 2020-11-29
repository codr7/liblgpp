#ifndef LGPP_OPS_SQUASH_HPP
#define LGPP_OPS_SQUASH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Squash {};

  template <>
  inline const Op* eval(const Op& op, const Squash& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    Stack s;
    move(stack.begin(), stack.end(), back_inserter(s));
    stack.clear();
    push(stack, types::Stack, s);
    return &op+1;
  }

}

#endif
