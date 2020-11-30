#ifndef LGPP_OPS_TYPE_OF_HPP
#define LGPP_OPS_TYPE_OF_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct TypeOf {};

  template <>
  inline const Op* eval(const Op& op, const TypeOf& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    push(stack, lgpp::types::Meta, &type_of(pop(stack)));
    return &op+1;
  }

}

#endif
