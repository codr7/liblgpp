#ifndef LGPP_OPS_BRANCH_GT_HPP
#define LGPP_OPS_BRANCH_GT_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch_const.hpp"

namespace lgpp::ops {
  
  struct BranchGt: BranchConst {
    template <typename...Args>
    BranchGt(Label& target, size_t x_offs, Args&&...args):
      BranchConst(target, x_offs, forward<Args>(args)...) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchGt& imp, Thread& thread) {
    auto& s = get_stack(thread);
    return (*(s.end()-imp.x_offs-1) > imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
