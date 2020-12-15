#ifndef LGPP_OPS_BRANCH_LT_HPP
#define LGPP_OPS_BRANCH_LT_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchLt: Branch {
    template <typename...Args>
    BranchLt(Label& target, size_t x_offs, Args&&...args):
      Branch(target, x_offs, forward<Args>(args)...) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchLt& imp, Thread& thread) {
    auto& s = get_stack(thread);
    return (*(s.end()-imp.x_offs-1) < imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
