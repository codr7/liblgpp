#ifndef LGPP_TYPES_PRIM_HPP
#define LGPP_TYPES_PRIM_HPP

#include "lgpp/prim.hpp"
#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline PC call(Type<lgpp::Prim>& type, const lgpp::Prim& imp, Thread& thread, PC return_pc, Pos pos) {
    imp.imp(thread, pos);
    return return_pc;
  }

  template <>
  inline void dump(Type<lgpp::Prim>& type, const lgpp::Prim& x, ostream &out) { out << "(Prim " << x.name << ')'; }

  template <>
  inline bool eq(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name == y.as(type).name; }
  
  template <>
  inline bool gt(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name > y.as(type).name; }
  
  template <>
  inline bool lt(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name < y.as(type).name; }

}

#endif
