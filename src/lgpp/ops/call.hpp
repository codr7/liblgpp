#ifndef LGPP_OPS_CALL_HPP
#define LGPP_OPS_CALL_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Call {
    template <typename...Args>
    Call(Args&&...args): target(forward<Args>(args)...) {}
    
    Val target;
  };

  template <>
  inline const Op* eval(const Op& op, const Call& imp, Thread& thread) {
    return &op - op.pc + call(imp.target, thread, op.pc+1, op.pos);
  }

}

#endif
