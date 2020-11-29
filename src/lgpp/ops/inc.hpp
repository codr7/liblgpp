#ifndef LGPP_OPS_INC_HPP
#define LGPP_OPS_INC_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Inc {
    template <typename...Args>
    Inc(Args&&...args): delta(forward<Args>(args)...) {}

    Val delta;
  };

  template <>
  inline const Op *eval(const Op &op, const Inc &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    push(stack, pop(stack) + imp.delta);
    return &op+1;
  }

}

#endif
