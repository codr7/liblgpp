#ifndef LGPP_OPS_DROP_HPP
#define LGPP_OPS_DROP_HPP

#include "../stack.hpp"
#include "../val.hpp"
#include "../op.hpp"

namespace lgpp::ops {

  struct Drop {
    Drop(size_t offs = 0, size_t len = 1);

    size_t offs, len;
  };

  
  template <>
  const Op *eval(const Op &op, const Drop &imp, lgpp::Stack &stack);

}

#endif
