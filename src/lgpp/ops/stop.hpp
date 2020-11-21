#ifndef LGPP_OPS_STOP_HPP
#define LGPP_OPS_STOP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Stop {};

  template <>
  const Op *eval(const Op &op, const Stop &imp, lgpp::Stack &stack);

}

#endif
