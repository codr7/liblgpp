#ifndef LGPP_OPS_BRANCH_EQ_HPP
#define LGPP_OPS_BRANCH_EQ_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"
#include "branch.hpp"

namespace lgpp::ops {
  
  struct BranchEq: Branch {
    BranchEq(lgpp::Label& target, lgpp::Val y, size_t x_offs = 0): Branch(target, y, x_offs) {}

    template <typename T>
    BranchEq(lgpp::Label& target, lgpp::Type<T>& type, T data, size_t x_offs = 0): Branch(target, type, data, x_offs) {}
  };

  template <>
  inline const Op* eval(const Op& op, const BranchEq& imp, lgpp::VM& vm, lgpp::Stack& stack) {
    return (*(stack.end()-imp.x_offs-1) == imp.y) ? &op-op.pc + *imp.target.pc : &op+1;
  }

}

#endif
