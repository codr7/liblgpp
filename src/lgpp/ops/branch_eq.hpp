#ifndef LGPP_OPS_BRANCH_EQ_HPP
#define LGPP_OPS_BRANCH_EQ_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchEq: Branch {
    template <typename...Args>
    BranchEq(lgpp::Label& target, size_t x_offs, Args&&...args):
      Branch(target, x_offs, forward<Args>(args)...) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchEq& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    return (*(stack.end()-imp.x_offs-1) == imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
