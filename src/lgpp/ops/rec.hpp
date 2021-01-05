#ifndef LGPP_OPS_REC_HPP
#define LGPP_OPS_REC_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Rec {};

  template <>
  inline const Op* eval(const Op& op, const Rec& imp, Thread& thread) {
    auto c = peek_call(thread);
    if (!c) { throw ERun(op.pos, "Rec outside of call"); } 
    return &op - op.pc + c->pc;
  }

}

#endif
