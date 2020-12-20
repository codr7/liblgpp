#ifndef LGPP_OPS_BRANCH_HPP
#define LGPP_OPS_BRANCH_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {
  
  struct Branch {
    Branch(const Label& target): target(target) {}    
    const Label& target;
  };

  template <>
  inline const Op* eval(const Op& op, const Branch& imp, Thread& thread) {
    return is_true(pop(get_stack(thread))) ? &op+1 : &op-op.pc + *imp.target.pc;
  }

}

#endif
