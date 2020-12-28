#ifndef LGPP_OPS_ROT_HPP
#define LGPP_OPS_ROT_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Rot {
    Rot(size_t offs = 2, size_t len = 1): offs(offs), len(len) {}
    size_t offs, len;
  };

  template <>
  inline const Op* eval(const Op& op, const Rot& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto i = s.end() - imp.offs - 1;
    rotate(i, i+imp.len, s.end());
    return &op+1;
  }

}

#endif
