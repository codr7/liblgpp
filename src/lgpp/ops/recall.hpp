#ifndef LGPP_OPS_RECALL_HPP
#define LGPP_OPS_RECALL_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct Recall {};

  template <>
  inline const Op* eval(const Op& op, const Recall& imp, Thread& thread) {
    auto c = peek_call(thread);
    if (!c) { throw ERun(op.pos, "Recall outside of call"); }
    push_call(thread, c->pc, op.pc+1, c->opts);
    return &op - op.pc + c->pc;
  }

}

#endif
