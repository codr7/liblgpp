#ifndef LGPP_OPS_GOTO_HPP
#define LGPP_OPS_GOTO_HPP

#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Goto {
    Goto(Label& target): target(target) {}
    Label& target;
  };

  template <>
  inline const Op* eval(const Op& op, const Goto& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    return &op - op.pc + *imp.target.pc;
  }

}

#endif
