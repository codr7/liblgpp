#ifndef LGPP_OPS_STOP_HPP
#define LGPP_OPS_STOP_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Stop {};

  template <>
  inline const Op* eval(const Op& op, const Stop& imp, Thread& thread) { return nullptr; }

}

#endif
