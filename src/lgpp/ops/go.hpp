#ifndef LGPP_OPS_GO_HPP
#define LGPP_OPS_GO_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Go {
    Go(const Label& target): target(target) {}
    const Label& target;
  };

  template <>
  inline const Op* eval(const Op& op, const Go& imp, Thread& thread) {
    return &op - op.pc + *imp.target.pc;
  }

}

#endif
