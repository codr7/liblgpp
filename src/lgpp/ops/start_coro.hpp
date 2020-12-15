#ifndef LGPP_OPS_START_CORO_HPP
#define LGPP_OPS_START_CORO_HPP

#include "../op.hpp"
#include "../val.hpp"
#include "../vm.hpp"

namespace lgpp::ops {

  struct StartCoro {
    StartCoro(Label& target): target(target) {}
    Label& target;
  };

  template <>
  inline const Op* eval(const Op& op, const StartCoro& imp, Thread& thread) {
    push(get_stack(thread), types::Coro, *imp.target.pc);
    return &op+1;
  }
  
}

#endif
