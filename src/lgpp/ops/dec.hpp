#ifndef LGPP_OPS_DEC_HPP
#define LGPP_OPS_DEC_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Dec {
    template <typename...Args>
    Dec(Args&&...args): delta(forward<Args>(args)...) {}

    Val delta;
  };

  template <>
  inline const Op* eval(const Op& op, const Dec& imp, Thread& thread) {
    auto& s = get_stack(thread);
    push(s, pop(s) - imp.delta);
    return &op+1;
  }

}

#endif
