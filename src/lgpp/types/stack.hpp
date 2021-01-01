#ifndef LGPP_TYPES_STACK_HPP
#define LGPP_TYPES_STACK_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<lgpp::Stack>& type, const lgpp::Stack& x, ostream &out) { dump(x, out); }

  template <>
  inline bool is_true(Type<lgpp::Stack>& type, const lgpp::Stack& x) { return !x.empty(); }

}

#endif
