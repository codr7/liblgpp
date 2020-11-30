#ifndef LGPP_OPS_ISA_HPP
#define LGPP_OPS_ISA_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Isa {};

  template <>
  inline const Op* eval(const Op& op, const Isa& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    auto &parent(*pop(stack).as(lgpp::types::Meta)), &child(*pop(stack).as(lgpp::types::Meta));
    auto root = lgpp::isa(child, parent);

    if (root) {
      push(stack, lgpp::types::Meta, root);
    } else {
      push(stack, lgpp::types::Nil, nullptr);      
    }
    
    return &op+1;
  }

}

#endif
