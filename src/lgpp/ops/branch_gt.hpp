#ifndef LGPP_OPS_BRANCH_GT_HPP
#define LGPP_OPS_BRANCH_GT_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchGt: Branch {
    template <typename...Args>
    BranchGt(lgpp::Label& target, size_t x_offs, Args&&...args):
      Branch(target, x_offs, forward<Args>(args)...) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchGt& imp, lgpp::Thread& thread, lgpp::Stack& stack) {
    return (*(stack.end()-imp.x_offs-1) > imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
