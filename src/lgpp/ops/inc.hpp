#ifndef LGPP_OPS_INC_HPP
#define LGPP_OPS_INC_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Inc {
    Inc(Val delta): delta(delta) {}
    
    template <typename T>
    Inc(Type<T> &type, T data): delta(type, data) {}

    Val delta;
  };

  template <>
  inline const Op *eval(const Op &op, const Inc &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    push(stack, pop(stack) + imp.delta);
    return &op+1;
  }

}

#endif
