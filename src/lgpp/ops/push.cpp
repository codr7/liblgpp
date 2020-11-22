#include "push.hpp"

namespace lgpp::ops {  
  Push::Push(lgpp::Val val): val(val) {}

  template <>
  const Op *eval(const Op &op, const Push &imp, lgpp::Stack &stack) {
    stack.push_back(imp.val);
    return &op+1;
  }
}
