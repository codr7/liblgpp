#ifndef LGPP_OPS_PUSH_HPP
#define LGPP_OPS_PUSH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Push {
    template <typename...Args>
    Push(Args&&...args): val(forward<Args>(args)...) {}
      
    lgpp::Val val;
  };

  template <>
  inline const Op* eval(const Op& op, const Push& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    stack.push_back(imp.val);
    return &op+1;
  }

}

#endif
