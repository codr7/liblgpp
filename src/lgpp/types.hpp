#ifndef LGPP_TYPES_HPP
#define LGPP_TYPES_HPP

#include <iostream>

#include "lgpp/coro.hpp"
#include "lgpp/label.hpp"
#include "lgpp/macro.hpp"
#include "lgpp/pair.hpp"
#include "lgpp/prim.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/type.hpp"

namespace lgpp::types {
  extern Trait Any, Num, Seq;

  extern Type<bool> Bool;

  template <>
  inline void dump(Type<bool>& type, const bool& x, ostream &out) { out << (x ? 'T' : 'F'); }

  template <>
  inline bool is_true(Type<bool>& type, const bool& x) { return x; }


  extern Type<Coro> Coro;

  template <>
  inline PC call(Type<lgpp::Coro>& type, const lgpp::Coro& imp, Thread& thread, PC return_pc, Pos pos) {
    return resume(imp, thread, return_pc, pos); 
  }

  extern Type<int> Int;

  template <>
  inline void dump(Type<int>& type, const int& x, ostream &out) { out << x; }

  template <>
  inline bool is_true(Type<int>& type, const int& x) { return x; }

  template <>
  inline Val add(Type<int>& type, const int& x, Val y) { return Val(type, x + y.as(type)); }
  
  template <>
  inline Val sub(Type<int>& type, const int& x, Val y) { return Val(type, x - y.as(type)); }


  extern Type<lgpp::Label *> Label;

  template <>
  inline PC call(Type<lgpp::Label *>& type, lgpp::Label* const& imp, Thread& thread, PC return_pc, Pos pos) {
    push_call(thread, return_pc);	
    return *imp->pc; 
  }

  template <>
  inline void dump(Type<lgpp::Label *>& type, lgpp::Label* const& x, ostream &out) { 
    out << "(Label ";
    if (x->name) { out << *x->name; } else { out << x; }
    out << ')';
  }


  extern Type<lgpp::Macro> Macro;

  template <>
  inline void dump(Type<lgpp::Macro>& type, const lgpp::Macro& x, ostream &out) { out << "(Macro " << x.name << ')'; }

  template <>
  inline bool eq(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name == y.as(Macro).name; }
  
  template <>
  inline bool gt(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name > y.as(Macro).name; }
  
  template <>
  inline bool lt(Type<lgpp::Macro>& type, const lgpp::Macro& x, Val y) { return x.name < y.as(Macro).name; }

  
  extern Type<lgpp::Trait *> Meta;

  template <>
  inline void dump(Type<lgpp::Trait*>& type, lgpp::Trait* const& x, ostream &out) { out << x->name; }


  extern Type<lgpp::Pair> Pair;

  template <>
  inline void dump(Type<lgpp::Pair>& type, const lgpp::Pair& x, ostream &out) {
    out << x.first << ' ' << x.second;
  }


  extern Type<lgpp::Prim> Prim;

  template <>
  inline PC call(Type<lgpp::Prim>& type, const lgpp::Prim& imp, Thread& thread, PC return_pc, Pos pos) {
    imp.imp(thread, pos);
    return return_pc;
  }

  template <>
  inline void dump(Type<lgpp::Prim>& type, const lgpp::Prim& x, ostream &out) { out << "(Prim " << x.name << ')'; }

  template <>
  inline bool eq(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name == y.as(Prim).name; }
  
  template <>
  inline bool gt(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name > y.as(Prim).name; }
  
  template <>
  inline bool lt(Type<lgpp::Prim>& type, const lgpp::Prim& x, Val y) { return x.name < y.as(Prim).name; }

  
  extern Type<lgpp::Stack> Stack;

  template <>
  inline void dump(Type<lgpp::Stack>& type, const lgpp::Stack& x, ostream &out) { dump(x, out); }

  template <>
  inline bool is_true(Type<lgpp::Stack>& type, const lgpp::Stack& x) { return !x.empty(); }

  extern Type<Thread::Id> Thread;
}

#endif
