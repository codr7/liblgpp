#ifndef LGPP_OPS_PUSH_HPP
#define LGPP_OPS_PUSH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Push {
    Push(lgpp::Val val);

    template <typename T>
    Push(lgpp::Type<T> &type, T data): val(type, data) {}
      
    lgpp::Val val;
  };

  
  template <>
  const Op *eval(const Op &op, const Push &imp, lgpp::Stack &stack);

}

#endif
