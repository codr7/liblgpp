#ifndef LGPP_OPS_SPLAT_HPP
#define LGPP_OPS_SPLAT_HPP

#include "../op.hpp"
#include "../stack.hpp"
#include "../thread.hpp"

namespace lgpp::ops {

  struct Splat {};

  template <>
  inline const Op* eval(const Op& op, const Splat& imp, Thread& thread) {
    auto& ts = get_stack(thread);
    auto s = pop(ts, thread.vm.Stack);
    copy(s.begin(), s.end(), back_inserter(ts));
    return &op+1;
  }

}

#endif
