#ifndef LGPP_TYPES_LABEL_HPP
#define LGPP_TYPES_LABEL_HPP

#include "lgpp/type.hpp"

namespace lgpp::types {

  template <>
  inline PC call(Type<lgpp::Label *>& type, lgpp::Label* const& imp, Thread& thread, PC return_pc, Pos pos) {
    push_call(thread, return_pc);	
    return *imp->pc; 
  }

  template <>
  inline void dump(Type<lgpp::Label *>& type, lgpp::Label* const& x, ostream &out) { 
    out << '(' << type.name << ' ';
    if (x->name) { out << *x->name; } else { out << x; }
    out << ')';
  }

}

#endif
