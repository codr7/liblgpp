#ifndef LGPP_OPS_PUSH_HPP
#define LGPP_OPS_PUSH_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Push {
    Push(lgpp::Val val);
    lgpp::Val val;
  };

  
  template <>
  const Op *eval(const Op &op, const Push &imp, lgpp::Stack &stack);

}

#endif
