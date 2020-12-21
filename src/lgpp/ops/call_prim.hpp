#ifndef LGPP_OPS_CALL_PRIM_HPP
#define LGPP_OPS_CALL_PRIM_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct CallPrim {    
    CallPrim(const Prim& target): target(target) {}
    const Prim& target;
  };

  template <>
  inline const Op* eval(const Op& op, const CallPrim& imp, Thread& thread) {
    imp.target.imp(thread, op.pos);
    return &op+1;
  }

}

#endif
