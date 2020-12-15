#ifndef LGPP_OPS_ISA_HPP
#define LGPP_OPS_ISA_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Isa {};

  template <>
  inline const Op* eval(const Op& op, const Isa& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto &parent(*pop(s).as(types::Meta)), &child(*pop(s).as(types::Meta));
    auto root = isa(child, parent);

    if (root) {
      push(s, types::Meta, root);
    } else {
      push(s, types::Nil, nullptr);      
    }
    
    return &op+1;
  }

}

#endif
