#ifndef LGPP_OPS_CALL_HPP
#define LGPP_OPS_CALL_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Call {
    Call() {}

    template <typename...Args>
    Call(Args&&...args) { target.emplace(forward<Args>(args)...); }
    
    optional<Val> target;
  };

  template <>
  inline const Op* eval(const Op& op, const Call& imp, Thread& thread) {
    Val t = imp.target ? *imp.target : pop(get_stack(thread));
    return &op - op.pc + call(t, thread, op.pc, op.pos);
  }

}

#endif
