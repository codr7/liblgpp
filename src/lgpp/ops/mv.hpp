#ifndef LGPP_OPS_MV_HPP
#define LGPP_OPS_MV_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Mv {
    Mv(size_t offs = 0, size_t len = 1): offs(offs), len(len) {}
    size_t offs, len;
  };

  template <>
  inline const Op* eval(const Op& op, const Mv& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto i = s.end() - imp.offs - 1;
    rotate(i, i+imp.len, s.end());
    return &op+1;
  }

}

#endif
