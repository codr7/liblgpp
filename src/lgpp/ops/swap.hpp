#ifndef LGPP_OPS_SWAP_HPP
#define LGPP_OPS_SWAP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Swap {
    Swap(size_t x_offs = 0, size_t y_offs = 1): x_offs(x_offs), y_offs(y_offs) {}
    size_t x_offs, y_offs;
  };

  template <>
  inline const Op* eval(const Op& op, const Swap& imp, Thread& thread) {
    auto& s = get_stack(thread);
    auto n = s.size();
    swap(s[n - imp.x_offs - 1], s[n - imp.y_offs - 1]);
    return &op+1;
  }

}

#endif
