#ifndef LGPP_TYPES_META_HPP
#define LGPP_TYPES_META_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<lgpp::Trait*>& type, lgpp::Trait* const& x, ostream &out) { out << x->name; }

}

#endif
