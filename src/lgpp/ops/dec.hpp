#ifndef LGPP_OPS_DEC_HPP
#define LGPP_OPS_DEC_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Dec {
    Dec(Val delta): delta(delta) {}
    
    template <typename T>
    Dec(Type<T> &type, T data): delta(type, data) {}

    Val delta;
  };

  template <>
  inline const Op *eval(const Op &op, const Dec &imp, lgpp::VM &vm, lgpp::Stack &stack) {
    push(stack, pop(stack).sub(imp.delta));
    return &op+1;
  }

}

#endif
