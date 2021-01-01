#ifndef LGPP_TYPES_MACRO_HPP
#define LGPP_TYPES_MACRO_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline void dump(Type<lgpp::Macro>& type, const lgpp::Macro& x, ostream &out) { out << "(Macro " << x.name << ')'; }

  template <>
  inline bool eq(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name == y.as(type).name; }
  
  template <>
  inline bool gt(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name > y.as(type).name; }
  
  template <>
  inline bool lt(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name < y.as(type).name; }

}

#endif
