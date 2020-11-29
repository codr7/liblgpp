#ifndef LGPP_OPS_BRANCH_HPP
#define LGPP_OPS_BRANCH_HPP

#include <optional>

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp {
  struct Label;
}

namespace lgpp::ops {
  
  struct Branch {
    template <typename...Args>
    Branch(lgpp::Label& target, size_t x_offs, Args&&...args):
      target(target), x_offs(x_offs), y(forward<Args>(args)...) {}
    
    lgpp::Label& target;
    size_t x_offs;
    lgpp::Val y;
  };

}

#endif
