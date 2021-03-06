#ifndef LGPP_OPS_BRANCH_CONST_HPP
#define LGPP_OPS_BRANCH_CONST_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp {
  struct Label;
}

namespace lgpp::ops {
  
  struct BranchConst {
    template <typename...Args>
    BranchConst(Label& target, size_t x_offs, Args&&...args):
      target(target), x_offs(x_offs), y(forward<Args>(args)...) {}
    
    Label& target;
    size_t x_offs;
    Val y;
  };

}

#endif
